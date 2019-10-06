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

	  char str[128];

	  float acc_x, acc_y, acc_z;
	  float gyr_x, gyr_y, gyr_z;

	  if (imu_get_data(&acc_x, &acc_y, &acc_z,
			   &gyr_x, &gyr_y, &gyr_z)) { 
	  
	    snprintf(str, 128, "accel: %f, %f, %f\n\r", acc_x, acc_y, acc_z);
	  
	    chprintf((BaseSequentialStream *)&SDU1, "%s", str);

	    snprintf(str, 128, "gyro: %f, %f, %f\n\r", gyr_x, gyr_y, gyr_z);
	    chprintf((BaseSequentialStream *)&SDU1, "%s", str);
	  } else {
	    chprintf((BaseSequentialStream *)&SDU1,"Error: Not able to read data from IMU\n\r");
	  }
	  
	  chThdSleepMilliseconds(100);
	}
}
