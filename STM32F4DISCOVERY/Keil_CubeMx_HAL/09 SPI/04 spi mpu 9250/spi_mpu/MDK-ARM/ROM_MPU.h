#include "stm32f4xx_hal.h"

#define RAD_TO_DEG 								57.295779513082320876798154814105

#define WHO_AM_I_REG 							0x75
#define PWR_MGMT_1_REG 						0x6B
#define SMPLRT_DIV_REG 						0x19
#define ACCEL_CONFIG_REG 					0x1C
#define ACCEL_XOUT_H_REG 					0x3B
#define TEMP_OUT_H_REG 						0x41
#define GYRO_CONFIG_REG 					0x1B
#define GYRO_XOUT_H_REG 					0x43
#define MPU6050_ADDR 							0xD0

uint8_t mpu9250_init(void);
void mpu9250_write_reg(uint8_t reg, uint8_t data);

void mpu9250_read_reg(uint8_t reg, uint8_t *data, uint8_t len);
void mpu9250_read_accel(uint8_t *data);

