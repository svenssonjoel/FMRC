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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ch.h"
#include "hal.h"
#include "hal_pwm.h"
#include "hal_pal.h"

#include "chprintf.h"

#include "usbcfg.h"

#include "repl.h"

// Definitions

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/
/*
static const ShellCommand commands[] = {
		{"repl", cmd_repl},
		{"duty", cmd_duty},
		{NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
		(BaseSequentialStream *)&SDU1,
		commands
};
*/

int main(void) {
	halInit();
	chSysInit();

	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

	led_init();

	drv_init();
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
	
	//shellInit();

	/*
	 * Normal main() thread activity, spawning shells.
	 */
	while (true) {
	  /*
		if (SDU1.config->usbp->state == USB_ACTIVE) {
			thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
					"shell", NORMALPRIO + 1,
					shellThread, (void *)&shell_cfg1);
			chThdWait(shelltp);
		}
	  */
		chThdSleepMilliseconds(1000);
	}
}
