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
#include <ctype.h>

#include "ch.h"
#include "hal.h"
#include "usbcfg.h"
#include "chprintf.h"

#include "led.h"
#include "motor_drv.h"
#include "repl.h"

#include "bmi160.h"
#include "bmi160_defs.h"
#include "i2c_bb.h"

/* IMU pins

   PB5 - IMU_INT
   PB6 - SCL
   PB7 - SDA

 */

#define BMI160_SCL_GPIO GPIOB
#define BMI160_SCL_PIN  6
#define BMI160_SDA_GPIO GPIOB
#define BMI160_SDA_PIN  7

struct bmi160_dev sensor;

static const GPTConfig gpt1cfg = {
  1000000, // 1 MHz timer clock.
  NULL, // No callback
  0, 0
};


static const I2CConfig i2cconfig = {
  OPMODE_I2C,
  100000,
  STD_DUTY_CYCLE // FAST_DUTY_CYCLE_2
};

static i2c_bb_state i2c_state;


int8_t user_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
  i2c_state.has_error = 0;

  uint8_t txbuf[1];
  txbuf[0] = reg_addr;
  return i2c_bb_tx_rx(&i2c_state, dev_addr, txbuf, 1, data, len) ? BMI160_OK : BMI160_E_COM_FAIL;
}

int8_t user_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
  i2c_state.has_error = 0;

  uint8_t txbuf[len + 1];
  txbuf[0] = reg_addr;
  memcpy(txbuf + 1, data, len);
  return i2c_bb_tx_rx(&i2c_state, dev_addr, txbuf, len + 1, 0, 0) ? BMI160_OK : BMI160_E_COM_FAIL;
}

void user_delay_ms(uint32_t ms) {
  chThdSleepMilliseconds(ms);
}


void imu_init(void) {

  led_write(LED_RED, 0);
  led_write(LED_GREEN, 0);
  
  
  palSetPadMode(GPIOB, 6 , /* pad settings for I2CD1 SCL */
  		PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN |
  		PAL_STM32_OSPEED_MID1 | PAL_STM32_PUPDR_PULLUP);
  palSetPadMode(GPIOB, 7, /* pad settings for I2CD1 SDA */
  		PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN |
  		PAL_STM32_OSPEED_MID1 | PAL_STM32_PUPDR_PULLUP);
  

  i2cStart(&I2CD1, &i2cconfig);

  i2c_state.scl_gpio = GPIOB;
  i2c_state.scl_pin  = 6;
  i2c_state.sda_gpio = GPIOB;
  i2c_state.sda_pin  = 7;
  i2c_state.gpt = &GPTD1;
  
  i2c_bb_init(&i2c_state);

  // start a GPT for usleep functionality
  gptStart(&GPTD1, &gpt1cfg);

  sensor.id = BMI160_I2C_ADDR;
  sensor.interface = BMI160_I2C_INTF;
  sensor.read = user_i2c_read;
  sensor.write = user_i2c_write;
  sensor.delay_ms = user_delay_ms;

  int8_t rslt = BMI160_OK;
  rslt = bmi160_init(&sensor);

  if (rslt == BMI160_OK) led_write(LED_RED, 0);
  else led_write(LED_RED, 1);
  
  /* Select the Output data rate, range of accelerometer sensor */
  sensor.accel_cfg.odr = BMI160_ACCEL_ODR_200HZ;
  sensor.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
  sensor.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

  /* Select the power mode of accelerometer sensor */
  sensor.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

  /* Select the Output data rate, range of Gyroscope sensor */
  sensor.gyro_cfg.odr = BMI160_GYRO_ODR_200HZ;
  sensor.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
  sensor.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

  /* Select the power mode of Gyroscope sensor */
  sensor.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE; 

  /* Set the sensor configuration */
  rslt = bmi160_set_sens_conf(&sensor);

  if (rslt == BMI160_OK) led_write(LED_GREEN, 1);   
}


int main(void) {
	halInit();
	chSysInit();

	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

	led_init();
	drv_init();
	imu_init();
	
	/*
	 * Activates the USB driver and then the USB bus pull-up on D+.
	 * Note, a delay is inserted in order to not have to disconnect the cable
	 * after a reset.
	 */
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(1500);
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);

	createReplThread((BaseSequentialStream *)&SDU1);

	/*
	 *  Main thread activity...
	 */
	while (true) {
	  led_write(LED_RED, 0);
	  int8_t rslt = BMI160_OK;
	  struct bmi160_sensor_data accel;
	  struct bmi160_sensor_data gyro;

	  rslt = bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &accel, &gyro, &sensor);

	  float acc_x = (float)accel.x * 16.0 / 32768.0;
	  float acc_y = (float)accel.y * 16.0 / 32768.0;
	  float acc_z = (float)accel.z * 16.0 / 32768.0;

	  float gyr_x = (float)gyro.x * 2000 / 32768.0;
	  float gyr_y = (float)gyro.y * 2000 / 32768.0;
	  float gyr_z = (float)gyro.z * 2000 / 32768.0;

	  char str[128];
	  
	  snprintf(str, 128, "accel: %f, %f, %f\n\r", acc_x, acc_y, acc_z);
	  
	  chprintf((BaseSequentialStream *)&SDU1, "%s", str);

	  snprintf(str, 128, "gyro: %f, %f, %f\n\r", gyr_x, gyr_y, gyr_z);
	  chprintf((BaseSequentialStream *)&SDU1, "%s", str);
	  
	  
	  
	  
	  if (rslt != BMI160_OK) {
	    led_write(LED_RED, 1);
	  }
	  
	  chThdSleepMilliseconds(100);
	}
}
