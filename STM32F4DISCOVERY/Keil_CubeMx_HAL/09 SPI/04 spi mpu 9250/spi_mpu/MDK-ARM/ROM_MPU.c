#include "ROM_MPU.h"
extern SPI_HandleTypeDef hspi2;

int16_t accel_data;
uint8_t imu_data[14];

uint8_t mpu9250_init(void)
{
	uint8_t check;
	uint8_t data;
	// check whoami
	
	
	// everything is 0x68 ( is OK? ) check
	mpu9250_read_reg(WHO_AM_I_REG, &check, 1);
	if( check == 0x68)
	{
		// power management register 0X6B we should write all 0's to wake the sensor up
    data = 0;
		mpu9250_write_reg(PWR_MGMT_1_REG, data);
		
		// set Data Rate of 1KHz by writting SMPLRT_DIV register
		data = 0x07;
		mpu9250_write_reg(SMPLRT_DIV_REG, data);
		
		// set accelero config
		data = 0x08;
		mpu9250_write_reg(ACCEL_CONFIG_REG, data); 	//+- 4G(01) 
		
		// set gyro config
		data = 0x00;
		mpu9250_write_reg(GYRO_CONFIG_REG, data);		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s
		
		return 0;
	}
	return 1;
}

void mpu9250_write_reg(uint8_t reg, uint8_t value)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &reg, 1, 2);
	HAL_SPI_Transmit(&hspi2, &value, 1, 2);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

void mpu9250_read_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
	uint8_t read_reg = 0x80 | reg; 								// read mode
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &read_reg, 1, 2);
	HAL_SPI_Receive(&hspi2, data, len, 2);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}


void mpu9250_read_accel(uint8_t *data)
{
	mpu9250_read_reg(ACCEL_XOUT_H_REG, data, sizeof(imu_data));
	accel_data = ((int16_t)imu_data[0]<<8) + imu_data[1];
}
