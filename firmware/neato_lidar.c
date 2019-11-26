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

#include "neato_lidar.h"

#include <stdio.h>
#include <stdlib.h>
#include "ch.h"
#include "hal.h"

#define LIDAR_UART_PORT GPIOC
#define LIDAR_UART_TX   10
#define LIDAR_UART_RX   11
#define LIDAR_SERIAL_DRIVER SD3

typedef struct {
  uint8_t  index;
  float    rpm;
  uint16_t dist[4];  
  bool     valid[4];
  bool     warn[4]; 
  uint16_t signal[4];
  bool     checksum_ok; 
} neato_lidar_packet_t;


// need to guard these with mutex if want consistent reads... 
static int distance[360];

#define MOTOR_DUTY_INITIAL 4500
#define MOTOR_TARGET_RPM   300
static int motor_duty = MOTOR_DUTY_INITIAL;
  
static THD_WORKING_AREA(neatoLidarThreadArea, 2048);

static SerialConfig serial_cfg = {
  115200,
  0,
  USART_CR2_STOP1_BITS,
  0
};

static uint16_t checksum(uint8_t *data)
{
  uint32_t chk32=0;
  uint16_t word;
  int i;
	
  for(i=0;i<10;++i)
    {
      word=data[2*i] + (data[2*i+1] << 8);
      chk32 = (chk32 << 1) + word;
    }
	
  uint32_t checksum=(chk32 & 0x7FFF) + (chk32 >> 15);
  return checksum & 0x7FFF;
}

static int parse_packet(uint8_t *bytes, neato_lidar_packet_t *p) {

  uint16_t chk  = checksum(bytes);

  uint16_t packet_chk = bytes[21] << 8;
  packet_chk += bytes[20];

  p->checksum_ok = chk == packet_chk ? true : false;
  p->index = bytes[1] - 0xA0;
  p->rpm = ((float)((uint16_t)bytes[2] + (bytes[3] << 8))) / 64.0f;

  p->valid[0] = !(bytes[5] & 0x80);
  p->warn[0] = bytes[5] & 0x40;
  p->valid[1] = !(bytes[9] & 0x80);
  p->warn[1] = bytes[9] & 0x40;
  p->valid[2] = !(bytes[13] & 0x80);
  p->warn[2] = bytes[13] & 0x40;
  p->valid[3] = !(bytes[17] & 0x80);
  p->warn[3] = bytes[17] & 0x40;

  p->signal[0] = bytes[6] + (bytes[7] << 8);
  p->signal[1] = bytes[10] + (bytes[11] << 8);
  p->signal[2] = bytes[14] + (bytes[15] << 8);
  p->signal[3] = bytes[18] + (bytes[19] << 8);

  p->dist[0] = bytes[4] + ((bytes[5] & 0x1f) << 8);
  p->dist[1] = bytes[8] + ((bytes[9] & 0x1f) << 8);
  p->dist[2] = bytes[12] + ((bytes[13] & 0x1f) << 8);
  p->dist[3] = bytes[16] + ((bytes[17] & 0x1f) << 8);

  return 1; 
}

static int read_packet(neato_lidar_packet_t *p) {

  uint8_t bytes[22];
  int num_read = 0;
  bool start_found = false;
  uint8_t c;
  int i = 0; 

  // Polling serial link for start of packet.
  
  while (!start_found)  {
    num_read = sdReadTimeout(&LIDAR_SERIAL_DRIVER, &c,1, 100);
    if (num_read == 0) return 0;
    if (c == 0xFA) {
      bytes[0] = c;
      break;
    }
  }
  
  for (i = 1; i < 22; i ++) {
    num_read = sdReadTimeout(&LIDAR_SERIAL_DRIVER, &c,1, 100);
    if (num_read == 0) return 0;
    bytes[i] = c;
  }

  return parse_packet(bytes, p); 
}



static THD_FUNCTION(neatoLidarThread, arg) {

  (void) arg;
  char str[256];

  uint32_t num_valid = 0;
  uint32_t num_invalid = 0; 
  
  while (true) {
    neato_lidar_packet_t p;

    
    if (read_packet(&p)) {

      int i = p.index * 4;
      for (int j = 0; j < 4; j ++) {
	if (p.valid[j]) { 
	  distance[i+j] = p.dist[j];
	} else {
	  distance[i+j] = -1;
	}	  
	  
      }

      // Adds a bit of correction to the duty cycle based on current RPM
      // Maybe should filter this for "influence over time" in some way. 
      if (p.rpm < 300) {
	motor_duty ++;
	if (motor_duty > 10000) motor_duty = 10000;
	pwmEnableChannel(&PWMD2, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD2,motor_duty));
      } else if (p.rpm > 315) {
	motor_duty --;
	if (motor_duty < 0) motor_duty = 0;
	pwmEnableChannel(&PWMD2, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD2,motor_duty));
      }
      
    } else {
      //chprintf((BaseSequentialStream *)&SDU1, "TIMEOUT!\n\r");
    }
    
     chThdSleepMilliseconds(1);
  }
}

int calibrate_lidar_motor() {

  neato_lidar_packet_t p;
  
  unsigned int avg_rpm;
  bool done = false;
  
  int duty = 5000;
  int low  = 0;
  int high = 10000;

  while (!done) {

    pwmEnableChannel(&PWMD2, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD2,duty));
    chThdSleepMilliseconds(500);
    avg_rpm = 0;
    for (int i = 0; i < 200; i ++ ) {
      if (read_packet(&p)) {
	avg_rpm += p.rpm;
      }
    }

    if ( abs(high - low) < 5) {
      done = true;
    } else if ( avg_rpm > 60000 ) {
      high = duty;
      duty = low + ((duty - low) / 2);
    } else if ( avg_rpm < 60000) {
      low = duty; 
      duty = high - ((high - duty) / 2);
    }
  }
  
  motor_duty = duty;
  pwmEnableChannel(&PWMD2, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD2,motor_duty));
  return 1;  
}


int neato_lidar_distance(int ix) {
  return distance[ix]; 
}

void neato_lidar_init(void) {

  palSetPadMode(GPIOC, 10, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 11, PAL_MODE_ALTERNATE(7));
  sdStart(&SD3, &serial_cfg);

  // spin up motor
  if (!calibrate_lidar_motor()) {
    //chprintf((BaseSequentialStream *)&SDU1, "ERROR: Could not calibrate Lidar motor\n\r");
    return;
  }
  //pwmEnableChannel(&PWMD2, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 6500));

  (void)chThdCreateStatic(neatoLidarThreadArea,
			  sizeof(neatoLidarThreadArea),
			  NORMALPRIO,
			  neatoLidarThread, NULL); 
  
}
