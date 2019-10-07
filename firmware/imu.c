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

#include "imu.h"
#include "led.h"

struct bmi160_dev sensor;

static const GPTConfig gpt1cfg = {
  1000000, // 1 MHz timer clock.
  NULL, // No callback
  0, 0
};


static i2c_bb_state i2c_state;


int8_t user_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
  i2c_state.has_error = 0;

  uint8_t txbuf[1];
  txbuf[0] = reg_addr;
  return i2c_bb_tx_rx(&i2c_state, dev_addr, txbuf, 1, data, len) ? BMI160_OK : BMI160_E_COM_FAIL;
}

int8_t user_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
  i2c_state.has_error = 0;

  uint8_t txbuf[len + 1];
  txbuf[0] = reg_addr;
  memcpy(txbuf + 1, data, len);
  return i2c_bb_tx_rx(&i2c_state, dev_addr, txbuf, len + 1, 0, 0) ? BMI160_OK : BMI160_E_COM_FAIL;
}

void user_delay_ms(uint32_t ms) {
  chThdSleepMilliseconds(ms);
}


void imu_init(void) {

  led_write(LED_RED, 0);
  led_write(LED_GREEN, 0);
  
  
  palSetPadMode(GPIOB, 6 , /* pad settings for I2CD1 SCL */
  		PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN |
  		PAL_STM32_OSPEED_MID1 | PAL_STM32_PUPDR_PULLUP);
  palSetPadMode(GPIOB, 7, /* pad settings for I2CD1 SDA */
  		PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN |
  		PAL_STM32_OSPEED_MID1 | PAL_STM32_PUPDR_PULLUP);
 
  i2c_state.scl_gpio = GPIOB;
  i2c_state.scl_pin  = 6;
  i2c_state.sda_gpio = GPIOB;
  i2c_state.sda_pin  = 7;
  i2c_state.gpt = &GPTD1;
  
  i2c_bb_init(&i2c_state);

  // start a GPT for usleep functionality
  gptStart(&GPTD1, &gpt1cfg);

  sensor.id = BMI160_I2C_ADDR;
  sensor.interface = BMI160_I2C_INTF;
  sensor.read = user_i2c_read;
  sensor.write = user_i2c_write;
  sensor.delay_ms = user_delay_ms;

  int8_t rslt = BMI160_OK;
  rslt = bmi160_init(&sensor);

  if (rslt == BMI160_OK) led_write(LED_RED, 0);
  else led_write(LED_RED, 1);
  
  /* Select the Output data rate, range of accelerometer sensor */
  sensor.accel_cfg.odr = BMI160_ACCEL_ODR_200HZ;
  sensor.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
  sensor.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

  /* Select the power mode of accelerometer sensor */
  sensor.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

  /* Select the Output data rate, range of Gyroscope sensor */
  sensor.gyro_cfg.odr = BMI160_GYRO_ODR_200HZ;
  sensor.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
  sensor.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

  /* Select the power mode of Gyroscope sensor */
  sensor.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE; 

  /* Set the sensor configuration */
  rslt = bmi160_set_sens_conf(&sensor);

  if (rslt == BMI160_OK) led_write(LED_GREEN, 1);   
}


extern bool imu_get_data(float *accel_x, float *accel_y, float *accel_z,
			 float *gyro_x, float *gyro_y, float *gyro_z) {


  int8_t rslt = BMI160_OK;
  struct bmi160_sensor_data accel;
  struct bmi160_sensor_data gyro;
  
  rslt = bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &accel, &gyro, &sensor);

  if (rslt == BMI160_OK) {   
    *accel_x = (float)accel.x * 16.0 / 32768.0;
    *accel_y = (float)accel.y * 16.0 / 32768.0;
    *accel_z = (float)accel.z * 16.0 / 32768.0;
    
    *gyro_x = (float)gyro.x * 2000 / 32768.0;
    *gyro_y = (float)gyro.y * 2000 / 32768.0;
    *gyro_z = (float)gyro.z * 2000 / 32768.0;
    return true;
  }
  
  return false;
}
