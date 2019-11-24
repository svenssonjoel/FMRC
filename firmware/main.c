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

#include "imu.h"
#include "neato_lidar.h"

void forward(int speed) {

  pwmEnableChannel(&PWMD3, 0 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, speed));
  pwmEnableChannel(&PWMD3, 1 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
  pwmEnableChannel(&PWMD3, 2 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, speed));
  pwmEnableChannel(&PWMD3, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
}

void turn(int speed) {
  pwmEnableChannel(&PWMD3, 0 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
  pwmEnableChannel(&PWMD3, 1 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, speed));
  pwmEnableChannel(&PWMD3, 2 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, speed));
  pwmEnableChannel(&PWMD3, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
}

void stop(void) {
  pwmEnableChannel(&PWMD3, 0 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
  pwmEnableChannel(&PWMD3, 1 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
  pwmEnableChannel(&PWMD3, 2 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
  pwmEnableChannel(&PWMD3, 3 , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 0));
}


typedef enum {
  go,
  turning 
} state_enum;

static SerialConfig serial_cfg = {
  115200,
  0,
  USART_CR2_STOP1_BITS,
  0
};

int main(void) {
	halInit();
	chSysInit();

	drv_init();
	led_init();
	imu_init();

	sdStart(&SD1, &serial_cfg);
	palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));

	
	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

	/*
	 * Activates the USB driver and then the USB bus pull-up on D+.
	 * Note, a delay is inserted in order to not have to disconnect the cable
	 * after a reset.
	 */
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(1500);
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(500);
        
	//neato_lidar_init();

	createReplThread((BaseSequentialStream *)&SDU1);

	/*
	 *  Main thread activity...
	 */

	
	uint8_t buffer[256];
	while (true) {

	  int num = 0;  

	  while ((num = sdReadTimeout(&SD1, buffer, 255, 100)) > 0)  {
	    
	    buffer[num+1] = 0; 
	   
	    chprintf((BaseSequentialStream *)&SDU1,"%d %s\n\r", num, buffer);
	    memset(buffer, 0, 256);
	  }
  

	  chThdSleepMilliseconds(100);
	}
	
	/*

	state_enum state = go;
	while (true) {
	  led_write(LED_RED, 0);
	  led_write(LED_GREEN,1);

	  int sum = 0;
	  int readings = 0;
	  
	  for (int i = 0; i < 20; i ++) {

	    int d0 = neato_lidar_distance(359 - i);
	    int d1 = neato_lidar_distance(i);
	    
	    if ( d0 != -1) {
	      sum += d0;
	      readings++;
	    }

	    if ( d1 != -1) {
	      sum += d1;
	      readings++; 
	    }
	  }

	  if (readings == 0) {

	    if (state == go) {
	      stop();
	      state = turning;
	    } else if (state == turning) {
	      turn(5000);
	    }
	    
	    led_write(LED_RED, 1);
	    led_write(LED_GREEN, 0);
	  } else {

	    float dist = (float)sum / readings; 

	    if (dist < 500.0) {
	      if (state == go) {
		stop(); 
		state = turning;
	      }else if (state == turning) {
		turn(5000);
	      }
	      
	    
	      led_write(LED_RED, 1);
	    } else {

	      if (state == go) { 
		forward(5000);
	      } else if (state == turning) {
		state = go; 
	      }
	    }
	    
	  }
	  chThdSleepMilliseconds(100);
	}
	*/
}
