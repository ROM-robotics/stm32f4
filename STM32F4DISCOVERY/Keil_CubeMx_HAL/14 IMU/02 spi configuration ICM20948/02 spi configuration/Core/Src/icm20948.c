#include "icm20948.h"
#include "stdio.h"

static void sel_user_bank(user_bank ub);

static void activate_imu()
{
	HAL_GPIO_WritePin(IMU_CS_PORT, IMU_CS_PIN,
			GPIO_PIN_RESET);
}
static void deactivate_imu()
{
	HAL_GPIO_WritePin(IMU_CS_PORT, IMU_CS_PIN,
				GPIO_PIN_SET);
}
static void sel_user_bank(user_bank ub)
{
	uint8_t data = ub;
	uint8_t reg = REG_BANK_SEL;
	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &reg, 1, 100);
	HAL_SPI_Transmit(&IMU_SPI, &data, 1, 100);
	deactivate_imu();
}
void icm_20948_read_reg(user_bank ub, uint8_t address, uint8_t *data)
{
	uint8_t temp_data = 0x80|address;
	sel_user_bank(ub);
	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &temp_data , 1, 100);
	HAL_SPI_Receive(&IMU_SPI, data, 1, 100);
	deactivate_imu();
}

void icm_20948_write_reg(user_bank ub, uint8_t reg, uint8_t data)
{
	sel_user_bank(ub);
	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &reg, 1, 100);
	HAL_SPI_Transmit(&IMU_SPI, &data, 1, 100);
	deactivate_imu();
}
void icm_20948_init()
{
	uint8_t temp_data;
	// IMU reset-sleep-clock, page 37, 0xc1 = 0b1100 0001
	icm_20948_write_reg(_b0, PWR_MGMT_1, 0xc1);
	HAL_Delay(100);
	// Exit from sleep mode, selecting the clock page 37, 0x01 = 0b0000 0001
	icm_20948_write_reg(_b0, PWR_MGMT_1, 0x01);

	// Accelerometer configuration, sample rate divider = 0, page 63
	icm_20948_write_reg(_b2, ACCEL_SMPLRT_DIV_1, 0x00);
	icm_20948_write_reg(_b2, ACCEL_SMPLRT_DIV_2, 0x00);

	// Accelerometer configuration, accelerometer range set and enable digital filter, page 64
	icm_20948_write_reg(_b2, ACCEL_CONFIG, ((ACCEL_RANGE_VALUE << 1)|0x01));

	// Put the serial interface in SPI mode only, page 36, 0x10 = 0b0001 0000
	temp_data = 0x10;
	icm_20948_write_reg(_b0, USER_CTRL, temp_data);

	// Gyroscope configuration, sample rate divider = 0, page 59
	icm_20948_write_reg(_b2, GYRO_SMPLRT_DIV, 0x00);

	// Gyroscope configuration, gyroscope range set and enable digital filter, page 59
	icm_20948_write_reg(_b2, GYRO_CONFIG_1, ((GYRO_RANGE_VALUE << 1)|0x01));

	sel_user_bank(_b0);

}

uint8_t icm_20948_read_data(icm_20948_data* data)
{
	static uint8_t data_rx[12];
	uint8_t temp_data = 0x80|ACCEL_XOUT_H;
	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &temp_data, 1, 1000);
	HAL_SPI_Receive(&IMU_SPI, data_rx, 12, 1000);
	deactivate_imu();
	data ->x_accel = ((int16_t)data_rx[0]<<8)| (int16_t)data_rx[1];
	data ->y_accel = ((int16_t)data_rx[2]<<8)| (int16_t) data_rx[3];
	data ->z_accel = ((int16_t)data_rx[4]<<8)| (int16_t) data_rx[5];

	data ->x_gyro = ((int16_t)data_rx[6]<<8) | (int16_t) data_rx[7];
	data ->y_gyro = ((int16_t)data_rx[8]<<8) | (int16_t)data_rx[9];
	data ->z_gyro = ((int16_t)data_rx[10]<<8)| (int16_t)data_rx[11];

	return 0;
}
