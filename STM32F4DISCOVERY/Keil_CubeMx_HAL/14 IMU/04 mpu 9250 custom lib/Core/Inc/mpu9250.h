
/*
 *
 *  Created on: Feb 10, 2024
 *      Author: mr_robot
 */

#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define GYRO_CONFIG_REG			27
#define ACCEL_CONFIG_REG		28
#define PWR_MGTM1_REG			107

#define GYRO_FS_250				0
#define GYRO_FS_500				0x08
#define GYRO_FS_1000			0x10
#define GYRO_FS_2000			0x18

#define ACCEL_FS_2G				0
#define ACCEL_FS_4G				0x08
#define ACCEL_FS_8G				0x10
#define ACCEL_FS_16G			0x18

#define ACCEL_XOUT_H			59

#define PWR_MGTM1_PLL_EXSLP		0x01

typedef struct{
	int16_t x_accel;
	int16_t y_accel;
	int16_t z_accel;
	int16_t x_gyro;
	int16_t y_gyro;
	int16_t z_gyro;
	int16_t x_mag;
	int16_t y_mag;
	int16_t z_mag;
} mpu_data;

void mpu_init(void);
void mpu_write_reg(uint8_t reg, uint8_t data);
void mpu_read_reg(uint8_t reg, uint8_t *data, uint8_t len);
void mpu_read_sensor(mpu_data *imu_data);

#endif /* INC_MPU9250_H_ */
