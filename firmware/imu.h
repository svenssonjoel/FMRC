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


#ifndef _IMU_H_
#define _IMU_H_

#include "bmi160.h"
#include "bmi160_defs.h"
#include "i2c_bb.h"
#include "stdbool.h"

/* IMU pins

   PB5 - IMU_INT
   PB6 - SCL
   PB7 - SDA
*/

#define BMI160_SCL_GPIO GPIOB
#define BMI160_SCL_PIN  6
#define BMI160_SDA_GPIO GPIOB
#define BMI160_SDA_PIN  7

extern void imu_init(void); 

extern bool imu_get_data(float *accel_x, float *accel_y, float *accel_z,
			 float *gyro_x, float *gyro_y, float *gyro_z);



#endif
