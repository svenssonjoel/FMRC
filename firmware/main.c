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

/* Information 
   
   2x DRV8833 - four motors

   DRV8833-1:  (TIM2)
   AIN1 - PA0 
   AIN2 - PA1
   BIN1 - PA2
   BIN2 - PA3 

   DRV8833-2:  (TIM3) 
   AIN1 - PC6
   AIN2 - PC7
   BIN1 - PC8 
   BIN2 - PC9

   xIN1    |  xIN2   |   Function
   PWM     |   0     |   Forward PWM - fast decay
    1      |  PWM    |   Forward PWM - slow decay
    0      |  PWM    |   Backwards PWM - fast decay 
   PWM     |   1     |   Backwards PWM - slow decay

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

#include "shell.h"
#include "chprintf.h"

#include "usbcfg.h"

#include "heap.h"
#include "symrepr.h"
#include "extensions.h"
#include "eval_cps.h"
#include "print.h"
#include "tokpar.h"
#include "prelude.h"

#include "led.h"

// Definitions

#define GPIO_AF_TIM2          ((uint8_t)0x01) 
#define GPIO_AF_TIM3          ((uint8_t)0x02)


static PWMConfig pwmcfg1 = {
  500000,  
  1000,
  NULL,
  { // per channel conf
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}, 
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}  
  },
  0,
  0
};

static PWMConfig pwmcfg2 = {
  500000,  
  1000,
  NULL,
  { // per channel conf
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}, 
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  0,
  0
};


VALUE ext_set_led(VALUE *args, int argn) {
  if (argn != 2) {
    return enc_sym(symrepr_nil());
  }

  int led_num = dec_i(args[1]);
  int state   = dec_i(args[0]);

  led_write(led_num, state);

  return enc_sym(symrepr_true());
}


void drv_init(void) {

  palSetPadMode(GPIOA, 0,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM2) |
  		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);
  palSetPadMode(GPIOA, 1,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM2) |
		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);
  palSetPadMode(GPIOA, 2,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM2) |
		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);
  palSetPadMode(GPIOA, 3,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM2) |
		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);


  palSetPadMode(GPIOC, 6,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM3) |
  		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);
  palSetPadMode(GPIOC, 7,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM3) |
		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);
  palSetPadMode(GPIOC, 8,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM3) |
		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);
  palSetPadMode(GPIOC, 9,
		PAL_MODE_ALTERNATE(GPIO_AF_TIM3) |
		PAL_STM32_OSPEED_HIGHEST |
		PAL_STM32_PUPDR_FLOATING);

  pwmStart(&PWMD2, &pwmcfg1);
  pwmStart(&PWMD3, &pwmcfg2);

  pwmEnableChannel(&PWMD2, 0, 0);
  pwmEnableChannel(&PWMD2, 1, 0);
  pwmEnableChannel(&PWMD2, 2, 0);
  pwmEnableChannel(&PWMD2, 3, 0);

  pwmEnableChannel(&PWMD3, 0, 0);
  pwmEnableChannel(&PWMD3, 1, 0);
  pwmEnableChannel(&PWMD3, 2, 0);
  pwmEnableChannel(&PWMD3, 3, 0);
}

VALUE ext_set_duty(VALUE *args, int argn) {
  if (argn != 3) return enc_sym(symrepr_nil());

  int driver = dec_i(args[2]);
  int chn  = dec_i(args[1]);
  int duty = dec_i(args[0]);

  if (chn >= 0 && chn <= 4) {
    if (duty >= 0 && duty <= 10000) {
      switch(driver){
      case 0:
	pwmEnableChannel(&PWMD2, chn , PWM_PERCENTAGE_TO_WIDTH(&PWMD2, duty));
	break;
      case 1:
	pwmEnableChannel(&PWMD3, chn , PWM_PERCENTAGE_TO_WIDTH(&PWMD3, duty));
	break;
      default:
	return enc_sym(symrepr_nil());
      }

      return enc_sym(symrepr_true());
    }
  }

  return enc_sym(symrepr_nil());
}

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(10*2048)

unsigned char inbyte(BaseSequentialStream *chp) {
  unsigned char c;
  while (streamRead(chp, &c, 1) == 0);
  return c;
}

void outbyte(BaseSequentialStream *chp, char c) {
  streamPut(chp, c);
}

int inputline(BaseSequentialStream *chp, char *buffer, int size) {
  int n = 0;
  unsigned char c;
  for (n = 0; n < size - 1; n++) {

    c = inbyte(chp);
    switch (c) {
    case 127: /* fall through to below */
    case '\b': /* backspace character received */
      if (n > 0)
        n--;
      buffer[n] = 0;
      outbyte(chp,'\b'); /* output backspace character */
      n--; /* set up next iteration to deal with preceding char location */
      break;
    case '\n': /* fall through to \r */
    case '\r':
      buffer[n] = 0;
      return n;
    default:
      if (isprint(c)) { /* ignore non-printable characters */
        outbyte(chp,c);
        buffer[n] = c;
      } else {
        n -= 1;
      }
      break;
    }
  }
  buffer[size - 1] = 0;
  return 0; // Filled up buffer without reading a linebreak
}

static void cmd_duty(BaseSequentialStream *chp, int argc, char *argv[]) {
  int duty = atoi(argv[0]);

  chprintf(chp,"args: %d\n\r", argc);
  chprintf(chp,"setting duty: %d\n\r", duty);

  if (duty >= 0 && duty <= 10000) {
    pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, duty));
  }

}

static void cmd_repl(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void) argc;
  (void) argv;

  size_t len = 1024;
  char *str = malloc(1024);
  char *outbuf = malloc(1024);
  int res = 0;

  heap_state_t heap_state;

  res = symrepr_init();
  if (res)
    chprintf(chp,"Symrepr initialized.\n\r");
  else {
    chprintf(chp,"Error initializing symrepr!\n\r");
    return;
  }
  int heap_size = 2048;
  res = heap_init(heap_size);
  if (res)
    chprintf(chp,"Heap initialized. Free cons cells: %u\n\r", heap_num_free());
  else {
    chprintf(chp,"Error initializing heap!\n\r");
    return;
  }

  res = eval_cps_init(true);
  if (res)
    chprintf(chp,"Evaluator initialized.\n\r");
  else {
    chprintf(chp,"Error initializing evaluator.\n\r");
  }

  if (extensions_add("set-duty", ext_set_duty)) {
    chprintf(chp,"set-duty extension added.\n\r");
  } else {
    chprintf(chp,"set-duty extension failed!\n\r");
  }

  if(extensions_add("set-led",  ext_set_led)) {
    chprintf(chp,"set-led extension added.\n\r");
  } else {
    chprintf(chp,"set-led extension failed!\n\r");
  }

  VALUE prelude = prelude_load();
  eval_cps_program(prelude);

  chprintf(chp,"Lisp REPL started (FMRC)!\n\r");

  while (1) {
    chprintf(chp,"# ");
    memset(str,0,len);
    memset(outbuf,0, 1024);
    inputline(chp,str, len);
    chprintf(chp,"\n\r");

    if (strncmp(str, ":info", 5) == 0) {
      chprintf(chp,"##(FMRC)####################################################\n\r");
      chprintf(chp,"Used cons cells: %lu \n\r", heap_size - heap_num_free());
      chprintf(chp,"ENV: "); simple_snprint(outbuf,1023, eval_cps_get_env()); chprintf(chp, "%s \n\r", outbuf);
      heap_get_state(&heap_state);
      chprintf(chp,"GC counter: %lu\n\r", heap_state.gc_num);
      chprintf(chp,"Recovered: %lu\n\r", heap_state.gc_recovered);
      chprintf(chp,"Marked: %lu\n\r", heap_state.gc_marked);
      chprintf(chp,"Free cons cells: %lu\n\r", heap_num_free());
      chprintf(chp,"############################################################\n\r");
      memset(outbuf,0, 1024);
    } else if (strncmp(str, ":quit", 5) == 0) {
      break;
    } else {

      VALUE t;
      t = tokpar_parse(str);

      t = eval_cps_program(t);

      if (dec_sym(t) == symrepr_eerror()) {
	chprintf(chp,"Error\n");
      } else {
	chprintf(chp,"> "); simple_snprint(outbuf, 1023, t); chprintf(chp,"%s \n\r", outbuf);
      }
    }
  }

  symrepr_del();
  heap_del();
}

static const ShellCommand commands[] = {
		{"repl", cmd_repl},
		{"duty", cmd_duty},
		{NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
		(BaseSequentialStream *)&SDU1,
		commands
};

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

	shellInit();

	/*
	 * Normal main() thread activity, spawning shells.
	 */
	while (true) {
		if (SDU1.config->usbp->state == USB_ACTIVE) {
			thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
					"shell", NORMALPRIO + 1,
					shellThread, (void *)&shell_cfg1);
			chThdWait(shelltp);
		}
		chThdSleepMilliseconds(1000);
	}
}
