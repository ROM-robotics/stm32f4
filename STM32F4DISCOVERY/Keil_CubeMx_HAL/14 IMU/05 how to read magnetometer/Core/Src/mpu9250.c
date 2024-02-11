#include "mpu9250.h"
#include "stdio.h"
/*
 *
 *  Created on: Feb 10, 2024
 *      Author: mr_robot
 */
extern SPI_HandleTypeDef hspi2;
static uint8_t mag_adjust[3];

static void activate_spi2_mpu(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

static void deactivate_spi2_mpu(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}


void ak8963_write_reg(uint8_t reg, uint8_t data)
{
	mpu_write_reg(I2C_SLV0_ADDR_REG, AK8963_ADDRESS);
	mpu_write_reg(I2C_SLV0_REG_REG, reg);
	mpu_write_reg(I2C_SLV0_DATA_REG, data);
	mpu_write_reg(I2C_SLV0_DATA_REG, (0x01|0x80));
}

void ak8963_read_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
	mpu_write_reg(I2C_SLV0_ADDR_REG, (0x80 | AK8963_ADDRESS) );
	mpu_write_reg(I2C_SLV0_REG_REG, reg);
	mpu_write_reg(I2C_SLV0_DATA_REG, (len|0x80));
	HAL_Delay(10);
	mpu_read_reg(EXT_SENS_DATA_00_REG, data, len);
}
void mpu_init(void)
{
	// GYRO configuration
	mpu_write_reg(GYRO_CONFIG_REG, GYRO_FS_500);

	// ACCELERO configuration
	mpu_write_reg(ACCEL_CONFIG_REG, ACCEL_FS_4G);

	// EXTING from sleep mode
	mpu_write_reg(PWR_MGTM1_REG, PWR_MGTM1_PLL_EXSLP);

	// Enable I2C Master setting 400Khz
	mpu_write_reg(USR_CTRL_REG, USR_CTRL_REG_I2CEN);
	mpu_write_reg(I2C_MSTR_CTRL_REG, I2C_MSTR_CTRL_400);

	// POWER down magneto
	ak8963_write_reg(AK8963_CTRL_REG, AK8963_PWR_DWN);
	HAL_Delay(10);
	ak8963_write_reg(AK8963_CTRL_REG, AK8963_FUSE_ROM);
	HAL_Delay(10);
	ak8963_read_reg(AK8963_SEN_ONSET_REG, mag_adjust, 3);

	ak8963_write_reg(AK8963_CTRL_REG, AK8963_PWR_DWN);
	HAL_Delay(10);
	ak8963_write_reg(AK8963_CTRL_REG, AK8963_CONT_MEAS2);
}

void mpu_write_reg(uint8_t reg, uint8_t data)
{
	activate_spi2_mpu();
	HAL_SPI_Transmit(&hspi2, &reg, 1, 100);
	HAL_SPI_Transmit(&hspi2, &data,1, 100);
	deactivate_spi2_mpu();
}

void mpu_read_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
	uint8_t temp_data = 0x80 | reg;
	activate_spi2_mpu();
	HAL_SPI_Transmit(&hspi2, &temp_data, 1, 100);
	HAL_SPI_Receive(&hspi2, data, len, 100);
	deactivate_spi2_mpu();
}

void mpu_read_sensor(mpu_data *imu_data)
{
	uint8_t data[14];
	activate_spi2_mpu();
	uint8_t temp_reg = 0x80 | ACCEL_XOUT_H;
	HAL_SPI_Transmit(&hspi2, &temp_reg, 1, 100);
	HAL_SPI_Receive(&hspi2, data, 14, 100);
	deactivate_spi2_mpu();

	imu_data->x_accel = ((int16_t)data[0]<<8) + data[1];
	imu_data->y_accel = ((int16_t)data[2]<<8) + data[3];
	imu_data->z_accel = ((int16_t)data[4]<<8) + data[55];

	imu_data->x_gyro = ((int16_t)data[8]<<8) + data[9];
	imu_data->y_gyro = ((int16_t)data[10]<<8) + data[11];
	imu_data->z_gyro = ((int16_t)data[12]<<8) + data[13];

	printf("Accelero : %d, %d, and %d \n", imu_data->x_accel, imu_data->y_accel, imu_data->z_accel);
	printf("Gyro : %d, %d, and %d \n", imu_data->x_gyro, imu_data->y_gyro, imu_data->z_gyro);

}
