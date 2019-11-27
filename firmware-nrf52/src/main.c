/*
    Copyright 2019 Joel Svensson	svenssonjoel@yahoo.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <drivers/uart.h>
#include <drivers/gpio.h>
#include <zephyr.h>
#include <sys/ring_buffer.h>

#include <settings/settings.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/bas.h>
#include <bluetooth/services/hrs.h>

#include <uart.h>

/* 
   pin 6 | p0.25 UART TX to STM32       -> PA10 on STM32 (USART1_RX)
   pin 7 | p0.26 UART RX from STM32     -> PA9 on STM32  (USART1_TX)

   pin 38 | p0.20 led red
   pin 36 | p0.21 led green
 */

#define RING_BUF_SIZE 1024
u8_t in_ring_buffer[RING_BUF_SIZE];
u8_t out_ring_buffer[RING_BUF_SIZE];

u8_t ble_in_ring_buffer[RING_BUF_SIZE];

K_MUTEX_DEFINE(uart_io_mutex);
K_MUTEX_DEFINE(ble_uart_mutex);

struct device *dev;

struct ring_buf in_ringbuf;
struct ring_buf out_ringbuf;
struct ring_buf ble_in_ringbuf; 

static void interrupt_handler(struct device *dev)
{
  while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
    if (uart_irq_rx_ready(dev)) {
      int recv_len, rb_len;
      u8_t buffer[64];
      size_t len = MIN(ring_buf_space_get(&in_ringbuf),
		       sizeof(buffer));

      recv_len = uart_fifo_read(dev, buffer, len);

      rb_len = ring_buf_put(&in_ringbuf, buffer, recv_len);
      if (rb_len < recv_len) {
	//silently dropping bytes
      }
    }

    if (uart_irq_tx_ready(dev)) {
      u8_t buffer[64];
      int rb_len, send_len;

      rb_len = ring_buf_get(&out_ringbuf, buffer, sizeof(buffer));
      if (!rb_len) {
	uart_irq_tx_disable(dev);
	continue;
      }

      send_len = uart_fifo_fill(dev, buffer, rb_len);
      if (send_len < rb_len) {
	//LOG_ERR("Drop %d bytes", rb_len - send_len);
      }
    }
  }
}

int get_char() {

  k_mutex_lock(&uart_io_mutex, K_FOREVER);
  int n;
  u8_t c;
  unsigned int key = irq_lock();
  n = ring_buf_get(&in_ringbuf, &c, 1);
  irq_unlock(key);
  k_mutex_unlock(&uart_io_mutex);
  if (n == 1) {
    return c;
  }
  return -1;
}

void put_char(int i) {
  if (i >= 0 && i < 256) {
    k_mutex_lock(&uart_io_mutex, K_FOREVER);

    u8_t c = (u8_t)i;
    unsigned int key = irq_lock();
    ring_buf_put(&out_ringbuf, &c, 1);
    uart_irq_tx_enable(dev);
    irq_unlock(key);
    k_mutex_unlock(&uart_io_mutex);
  }
}

void usb_printf(char *format, ...) {
  k_mutex_lock(&uart_io_mutex, K_FOREVER);

  va_list arg;
  va_start(arg, format);
  int len;
  static char print_buffer[4096];

  len = vsnprintf(print_buffer, 4096,format, arg);
  va_end(arg);

  int num_written = 0;
  while (len - num_written > 0) {
    unsigned int key = irq_lock();
    num_written +=
      ring_buf_put(&out_ringbuf,
		   (print_buffer + num_written),
		   (len - num_written));
    irq_unlock(key);
    uart_irq_tx_enable(dev);
  }
  k_mutex_unlock(&uart_io_mutex);
}


int inputline(char *buffer, int size) {
  int n = 0;
  int c;
  for (n = 0; n < size - 1; n++) {

    c = get_char();
    switch (c) {
    case 127: /* fall through to below */
    case '\b': /* backspace character received */
      if (n > 0)
        n--;
      buffer[n] = 0;
      put_char('\b'); /* output backspace character */
      n--; /* set up next iteration to deal with preceding char location */
      break;
    case '\n': /* fall through to \r */
    case '\r':
      buffer[n] = 0;
      return n;
    default:
      if (c != -1 && c < 256) {
	put_char(c);
	buffer[n] = c;
      } else {
	n --;
      }

      break;
    }
  }
  buffer[size - 1] = 0;
  return 0; // Filled up buffer without reading a linebreak
}

// 0x180d  -- Heart rate service
// 0x1805  -- Current time service
// 0x180A  -- Device Information Service (activated via the prj.conf file)


static const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA_BYTES(BT_DATA_UUID128_ALL, // Advertice BLE UART service
		0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0,
		0x93, 0xF3, 0xA3, 0xB5, 0x01, 0x00, 0x40, 0x6E)
};

static struct bt_uuid_128 bt_uart_base_uuid =
  BT_UUID_INIT_128(0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0,
		   0x93, 0xF3, 0xA3, 0xB5, 0x01, 0x00, 0x40, 0x6E);


static struct bt_uuid_128 bt_uart_tx_uuid =
  BT_UUID_INIT_128(0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0,
		   0x93, 0xF3, 0xA3, 0xB5, 0x02, 0x00, 0x40, 0x6E);

static struct bt_uuid_128 bt_uart_rx_uuid =
  BT_UUID_INIT_128(0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0,
		   0x93, 0xF3, 0xA3, 0xB5, 0x03, 0x00, 0x40, 0x6E);

static u8_t bt_uart_read_buf[20] = "apa";

static ssize_t bt_uart_write(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			     const void *buf, u16_t len, u16_t offset,
			     u8_t flags) {

  k_mutex_lock(&ble_uart_mutex, K_FOREVER);
  int n = ring_buf_put(&ble_in_ringbuf,buf, len);

  k_mutex_unlock(&ble_uart_mutex);
  return n;
}

static ssize_t bt_uart_read(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			    void *buf, u16_t len, u16_t offset)
{
	const char *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 strlen(value));
}

static void bt_uart_ccc_changed(const struct bt_gatt_attr *attr,
				       u16_t value)
{
  (void) attr;

  // bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
}


BT_GATT_SERVICE_DEFINE(bt_uart,
		       BT_GATT_PRIMARY_SERVICE(&bt_uart_base_uuid),
		       BT_GATT_CHARACTERISTIC(&bt_uart_tx_uuid.uuid, // TX from the point of view of the central
					      BT_GATT_CHRC_WRITE,    // central is allowed to write to tx
					      BT_GATT_PERM_WRITE,
					      NULL, bt_uart_write, NULL),
		       BT_GATT_CHARACTERISTIC(&bt_uart_rx_uuid.uuid, // RX from point of view of central
					      BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
					      BT_GATT_PERM_READ,
					      bt_uart_read, NULL, bt_uart_read_buf),
		       BT_GATT_CCC(bt_uart_ccc_changed,
				   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE));


int ble_get_char(void) {

  int n = 0; 
  u8_t c;
  k_mutex_lock(&ble_uart_mutex, K_FOREVER);  
  
  n = ring_buf_get(&ble_in_ringbuf, &c, 1);

  k_mutex_unlock(&ble_uart_mutex);
  if (n == 1) {
    return c;
  }
  return -1;
 
}

void ble_put_char(int i) {

  u8_t c = (u8_t) i;

  bt_gatt_notify(NULL, &bt_uart.attrs[2], &c, 1);
}

void ble_printf(char *format, ...) {

  va_list arg;
  va_start(arg, format);
  int len;
  static char print_buffer[4096];

  len = vsnprintf(print_buffer, 4096,format, arg);
  va_end(arg);

  int offset = 0;
  int bytes_left = len;
  while (bytes_left > 0) {
    int chunk_size = (bytes_left > 20 ? 20 : bytes_left);
    bt_gatt_notify(NULL,&bt_uart.attrs[2], print_buffer+offset, chunk_size);
    bytes_left -= chunk_size;
    offset += chunk_size;
  }
}

int ble_inputline(char *buffer, int size) {
  int n = 0;
  int c;
  for (n = 0; n < size - 1; n++) {

    c = ble_get_char();
    switch (c) {
    case 127: /* fall through to below */
    case '\b': /* backspace character received */
      if (n > 0)
        n--;
      buffer[n] = 0;
      //ble_put_char('\b'); /* output backspace character */
      n--; /* set up next iteration to deal with preceding char location */
      break;
    case '\n': /* fall through to \r */
    case '\r':
      buffer[n] = c;
      buffer[n+1] = 0;
      return n;
    default:
      if (c != -1 && c < 256) {
	//ble_put_char(c);
	buffer[n] = c;
      } else {
	n --;
      }

      break;
    }
  }
  buffer[size - 1] = 0;
  return 0; // Filled up buffer without reading a linebreak
}

static void connected(struct bt_conn *conn, u8_t err)
{
	if (err) {
	  //usb_printf("Connection failed (err 0x%02x)\n\r", err);
	} else {
	  //usb_printf("Connected\n\r");
	}
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
  //usb_printf("Disconnected (reason 0x%02x)\n\r", reason);
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};


static void bt_ready(int err)
{
	if (err) {
	  //usb_printf("Bluetooth init failed (err %d)\n", err);
	   return;
	}

	//usb_printf("Bluetooth initialized\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
	  //usb_printf("Advertising failed to start (err %d)\n", err);
		return;
	}

	//printk("Advertising successfully started\n");
}



static void uart_tx(struct device *uart, char *str, int n) {

  for (int i = 0; i < n; i ++) {
    uart_poll_out(uart, str[i]); 
  }
  
}

void main(void)
{

  u32_t baudrate, dtr = 0U;
  int ret;
  int err;

  struct device *uart;

  uart = device_get_binding("UART_0");
  if (!uart) {
    return;
  }

  /*  
  dev = device_get_binding("CDC_ACM_0");
  if (!dev) {
    return;
  }
  */
  

  ring_buf_init(&in_ringbuf, sizeof(in_ring_buffer), in_ring_buffer);
  ring_buf_init(&out_ringbuf, sizeof(out_ring_buffer), out_ring_buffer);
  ring_buf_init(&ble_in_ringbuf, sizeof(ble_in_ring_buffer), ble_in_ring_buffer);

  /*
  while (true) {
    uart_line_ctrl_get(dev, LINE_CTRL_DTR, &dtr);
    if (dtr) {
      break;
    } else {
      k_sleep(100);
    }
  }

  ret = uart_line_ctrl_set(dev, LINE_CTRL_DCD, 1);
  if (ret) {
    //LOG_WRN("Failed to set DCD, ret code %d", ret);
  }

  ret = uart_line_ctrl_set(dev, LINE_CTRL_DSR, 1);
  if (ret) {
    //LOG_WRN("Failed to set DSR, ret code %d", ret);
  }
  

  k_busy_wait(1000000);

  ret = uart_line_ctrl_get(dev, LINE_CTRL_BAUD_RATE, &baudrate);
  if (ret) {
    //LOG_WRN("Failed to get baudrate, ret code %d", ret);
  } else {
    //LOG_INF("Baudrate detected: %d", baudrate);
  }

  uart_irq_callback_set(dev, interrupt_handler);
  
  uart_irq_rx_enable(dev);
  */
  
  err = bt_enable(bt_ready);

  if (err) {
    ble_printf("Error enabling BLE\n");
  }

  bt_set_name("FMRC_BLE");
  bt_conn_cb_register(&conn_callbacks);
  

  struct device *d_led0;
  struct device *d_led1;

  d_led0 = device_get_binding(DT_ALIAS_LED0_GPIOS_CONTROLLER);
  d_led1 = device_get_binding(DT_ALIAS_LED1_GPIOS_CONTROLLER);
  gpio_pin_configure(d_led0, DT_ALIAS_LED0_GPIOS_PIN, GPIO_DIR_OUT);
  gpio_pin_configure(d_led1, DT_ALIAS_LED1_GPIOS_PIN, GPIO_DIR_OUT);
  
  int cnt = 0; 
  while (true) {


    u32_t ringbuf_left = ring_buf_space_get(&ble_in_ringbuf);

    if ( ringbuf_left < 10 ) {
      gpio_pin_write(d_led0, DT_ALIAS_LED0_GPIOS_PIN, 1);
      gpio_pin_write(d_led1, DT_ALIAS_LED1_GPIOS_PIN, 1);
    }

    
    unsigned char c[256];

    memset(c,0,256);

    cnt = ble_inputline(c, 256);    
    
    //usb_printf("OUTPUT: %d -- %s", cnt, c);

    
    uart_tx(uart,c, cnt+1);

    //ring_buf_reset(&ble_in_ringbuf);
    
    

    //gpio_pin_write(d_led0, DT_ALIAS_LED0_GPIOS_PIN, cnt % 2);
    //gpio_pin_write(d_led1, DT_ALIAS_LED1_GPIOS_PIN, cnt % 2);
    //cnt ++; 

    k_sleep(1);
  }
}
