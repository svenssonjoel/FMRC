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
#include "bm_if.h"
#include "neato_lidar.h"

static unsigned char nrf_program[100];

void nrf_swd_example(void) {
	bm_change_swd_pins(GPIOB, 14, GPIOB, 15);
	
	if (bm_connect() != 8) {
		return;
	}
	
	if (bm_erase_flash_all() != 1) {
		return;
	}
	
	if (bm_write_flash(0, nrf_program, sizeof(nrf_program) != 1)) {
		return;
	}
	
	if (bm_reboot() != 1) {
		return;
	}
	
	bm_disconnect();
}


int main(void) {
	halInit();
	chSysInit();
	
//	nrf_swd_example();

	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

	led_init();
	drv_init();
	imu_init();
	neato_lidar_init();
	
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
	  chThdSleepMilliseconds(100);
	}
}
