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
#include <math.h>

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

int uart_get_char(void) {
  int n = 0;
  uint8_t c;

  while ( n <= 0 ) {
    n = sdReadTimeout(&SD1, &c, 1, 10);
  }

  if (n == 1) {
    return c;
  }
  return -1;
}

int uart_inputline(char *buffer, int size) {
  int n = 0;
  int c;
  for (n = 0; n < size - 1; n++) {

    c = uart_get_char();
    switch (c) {
    case 127: /* fall through to below */
    case '\b': /* backspace character received */
      if (n > 0)
        n--;
      buffer[n] = 0;
      //uart_put_char('\b'); /* output backspace character */
      n--; /* set up next iteration to deal with preceding char location */
      break;
    case '\n': /* fall through to \r */
    case '\r':
      buffer[n] = 0;
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


int main(void) {
	halInit();
	chSysInit();

	drv_init();
	led_init();
	imu_init();

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

	//createReplThread((BaseSequentialStream *)&SDU1);

	sdStart(&SD1, &serial_cfg);		

	/*
	 *  Main thread activity...
	 */
	while (true) {

	  int num = 0;  

	  char buffer[256];
	  memset(buffer, 0, 256);
	  uart_inputline(buffer,256);
	  
  
	  float ang = 0;
	  float mag = 0;

	  int r = 0;

	  
	  r = sscanf(buffer, "%f ; %f\n", &ang, &mag);

	  chprintf((BaseSequentialStream *)&SDU1,"angmag: %d %d\n\r",(int)(10000*ang), (int)(10000*mag));

	  
	  float duty_l;
	  float duty_r; 
	  
	  if (r == 2) {
	    
	    if (ang > 0.0) { //left side

	      if (ang > M_PI/2) { // top
		float a = ang - M_PI/2; 
		duty_l = mag * (20000 * (a / (M_PI/2)) - 10000);
		duty_r = mag * 10000;

	      } else { // bottom
		duty_r = mag * (20000 * (fabs(ang) / (M_PI/2)) - 10000);
		duty_l = -mag * 10000;
	      }
	      
	    } else { // right side 

	      if (fabs(ang) > M_PI/2) { // top
		float a = fabs(ang) - M_PI/2; 
		duty_r = mag * (20000 * (a / (3.14/2)) - 10000);
		duty_l = mag * 10000;
		
	      } else { // bottom
		duty_l = mag * (20000 * (fabs(ang) / (M_PI/2)) - 10000);
		duty_r = -mag * 10000;
	      }	
	    }
	  }

	  //chprintf((BaseSequentialStream *)&SDU1,"%d %d\n\r",(int) duty_r, (int) duty_l);
	  
	  pwmEnableChannel(&PWMD3, 2, 0);
	  pwmEnableChannel(&PWMD3, 3, 0);
	  pwmEnableChannel(&PWMD2, 0, 0);
	  pwmEnableChannel(&PWMD2, 1, 0);

	  if (duty_r > 10000) duty_r = 10000;
	  if (duty_r < -10000) duty_r = -10000;
	  if (duty_l > 10000) duty_l = 10000;
	  if (duty_l < -10000) duty_l = -10000;
	  
	  
	  if (duty_r > 0) {
	    
	    pwmEnableChannel(&PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, (int)duty_r));
	    pwmEnableChannel(&PWMD3, 3, 0);
	    
	  } else {
	    pwmEnableChannel(&PWMD3, 2, 0);
	    pwmEnableChannel(&PWMD3, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, (int)(-duty_r)));
	  }

	  if (duty_l > 0) {
	    pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, (int)duty_l));
	    pwmEnableChannel(&PWMD2, 1, 0);
	  }
	  else {
	    pwmEnableChannel(&PWMD2, 0, 0);
	    pwmEnableChannel(&PWMD2, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, (int)-duty_l));	    
	  }
	  
	 
	  
	  chThdSleepMilliseconds(1);
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
