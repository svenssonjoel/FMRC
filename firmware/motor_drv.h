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
#ifndef _MOTOR_DRV_H_
#define _MOTOR_DRV_H_

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

extern void drv_init(void);

#endif
