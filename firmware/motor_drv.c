
#include "motor_drv.h"

#include "hal.h"
#include "hal_pwm.h"
#include "hal_pal.h"

#define GPIO_AF_TIM2          ((uint8_t)0x01)
#define GPIO_AF_TIM3          ((uint8_t)0x02)

static PWMConfig pwmcfg1 = {
  84000000,
  4200,
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
  84000000,
  4200, 
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
