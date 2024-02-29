/*
 * MPU9250.c
 *
 *  Created on: Mar 13, 2022
 *      Author: MarkSherstan
 */

#include "MPU9250.h"

static uint8_t mag_adjust[3];

/// @brief Check for connection, reset IMU, and set full range scale
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
uint8_t MPU_begin(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250)
{
    // Initialize variables
    uint8_t check, addr, val;

    // Confirm device
    MPU_REG_READ(SPIx, pMPU9250, WHO_AM_I, &check, 1);
    if (check == WHO_AM_I_9250_ANS)
    {
        // Startup / reset the sensor
        addr = PWR_MGMT_1;
        val = 0x00;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);

        // Disable I2C (SPI only)
        addr = USER_CTRL;
        val = 0x10;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);

        // Set the full scale ranges
        MPU_writeAccFullScaleRange(SPIx, pMPU9250, pMPU9250->settings.aFullScaleRange);
        // smplrt_div = 0x00
        // enable accelero low pass filter
        //addr = 29;
        //val  = ;
        //MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);


        MPU_writeGyroFullScaleRange(SPIx, pMPU9250, pMPU9250->settings.gFullScaleRange);
        // digital filter for gyro , smplrt_div, dlpf

        /* ROM START */
        // Enable I2C Master setting 400Khz
        uint8_t reg, value;
        reg = USR_CTRL_REG; value = USR_CTRL_REG_I2CEN;
        MPU_REG_WRITE(SPIx, pMPU9250, &reg, &value);
        reg = I2C_MSTR_CTRL_REG; value = I2C_MSTR_CTRL_400;
        MPU_REG_WRITE(SPIx, pMPU9250, &reg, &value);

        // POWER down Magnetometer
        reg = AK8963_CTRL_REG; value = AK8963_PWR_DWN;
        MPU_REG_WRITE(SPIx, pMPU9250, &reg, &value);
        HAL_Delay(10);

        ak8963_write_reg(SPIx, pMPU9250, AK8963_CTRL_REG, AK8963_FUSE_ROM);
        HAL_Delay(10);

        ak8963_read_reg(SPIx, pMPU9250, AK8963_SEN_ONSET_REG, mag_adjust, 3);

        ak8963_write_reg(SPIx, pMPU9250, AK8963_CTRL_REG, AK8963_PWR_DWN);
        HAL_Delay(10);

        ak8963_write_reg(SPIx, pMPU9250, AK8963_CTRL_REG, AK8963_CONT_MEAS2);
        HAL_Delay(10);

        // measure high, low 6 bytes and overflow
        uint8_t temp_data[7];
        ak8963_read_reg(SPIx, pMPU9250, AK8963_DATA_ONSET_REG, temp_data, 7);
        HAL_Delay(10);
        /* ROM END */
        return 1;
    }
    else
    {
        return 0;
    }
}

/// @brief Read a specific registry address
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
/// @param pAddr Pointer to address to be written to
/// @param pVal Pointer of value to write to given address
void MPU_REG_WRITE(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t *pAddr, uint8_t *pVal)
{
    MPU_CS(pMPU9250, CS_SELECT);
    HAL_SPI_Transmit(SPIx, pAddr, 1, SPI_TIMOUT_MS);
    HAL_SPI_Transmit(SPIx, pVal, 1, SPI_TIMOUT_MS);
    MPU_CS(pMPU9250, CS_DESELECT);
}

/// @brief Read a specific registry address
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
/// @param addr Address to start reading at
/// @param pRxData Pointer to data buffer
/// @param RxSize Size of data buffer
void MPU_REG_READ(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t addr, uint8_t *pRxData, uint16_t RxSize)
{
    MPU_CS(pMPU9250, CS_SELECT);
    uint8_t writeAddr = addr | READWRITE;
    HAL_SPI_Transmit(SPIx, &writeAddr, 1, SPI_TIMOUT_MS);
    HAL_SPI_Receive(SPIx, pRxData, RxSize, SPI_TIMOUT_MS);
    MPU_CS(pMPU9250, CS_DESELECT);
}

void ak8963_write_reg(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t reg, uint8_t data)
{
	uint8_t r = I2C_SLV0_ADDR_REG; uint8_t v = AK8963_ADDRESS;
	MPU_REG_WRITE(SPIx, pMPU9250, &r, &v);
	r = I2C_SLV0_REG_REG; v = reg;
	MPU_REG_WRITE(SPIx, pMPU9250, &r, &v);
	r = I2C_SLV0_DATA_REG; v = data;
	MPU_REG_WRITE(SPIx, pMPU9250, &r, &v);
	r = I2C_SLV0_CTRL_REG; v = (I2C_SLV0_CTRL_EN|0x01);
	MPU_REG_WRITE(SPIx, pMPU9250, &r, &v);
}

void ak8963_read_reg(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t reg, uint8_t *data, uint8_t len)
{
	uint8_t r = I2C_SLV0_ADDR_REG; uint8_t v = (0x80 | AK8963_ADDRESS);
	MPU_REG_WRITE(SPIx, pMPU9250, &r, &v);

	r = I2C_SLV0_REG_REG; v = reg;
	MPU_REG_WRITE(SPIx, pMPU9250, &r, &v);

	r = I2C_SLV0_CTRL_REG; v = (I2C_SLV0_CTRL_EN|len);
	MPU_REG_WRITE(SPIx, pMPU9250, &r, &v);
	HAL_Delay(10);

	MPU_REG_READ(SPIx, pMPU9250, EXT_SENS_DATA_00_REG, data, len);
}

/// @brief Set CS state to either start or end transmissions
/// @param pMPU9250 Pointer to master MPU9250 struct
/// @param state Set low to select, high to deselect
void MPU_CS(MPU9250_t *pMPU9250, uint8_t state)
{
    HAL_GPIO_WritePin(pMPU9250->settings.CS_PORT, pMPU9250->settings.CS_PIN, state);
}

/// @brief Set the accelerometer full scale range
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
/// @param aScale Set 0 for ±2g, 1 for ±4g, 2 for ±8g, and 3 for ±16g
void MPU_writeAccFullScaleRange(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t aScale)
{
    // Variable init
    uint8_t addr = ACCEL_CONFIG;
    uint8_t val;

    // Set the value
    switch (aScale)
    {
    case AFSR_2G:
        pMPU9250->sensorData.aScaleFactor = 16384.0;
        val = 0x00;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    case AFSR_4G:
        pMPU9250->sensorData.aScaleFactor = 8192.0;
        val = 0x08;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    case AFSR_8G:
        pMPU9250->sensorData.aScaleFactor = 4096.0;
        val = 0x10;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    case AFSR_16G:
        pMPU9250->sensorData.aScaleFactor = 2048.0;
        val = 0x18;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    default:
        pMPU9250->sensorData.aScaleFactor = 8192.0;
        val = 0x08;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    }
}

/// @brief Set the gyroscope full scale range
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
/// @param gScale Set 0 for ±250°/s, 1 for ±500°/s, 2 for ±1000°/s, and 3 for ±2000°/s
void MPU_writeGyroFullScaleRange(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t gScale)
{
    // Variable init
    uint8_t addr = GYRO_CONFIG;
    uint8_t val;

    // Set the value
    switch (gScale)
    {
    case GFSR_250DPS:
        pMPU9250->sensorData.gScaleFactor = 131.0;
        val = 0x00;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    case GFSR_500DPS:
        pMPU9250->sensorData.gScaleFactor = 65.5;
        val = 0x08;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    case GFSR_1000DPS:
        pMPU9250->sensorData.gScaleFactor = 32.8;
        val = 0x10;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    case GFSR_2000DPS:
        pMPU9250->sensorData.gScaleFactor = 16.4;
        val = 0x18;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    default:
        pMPU9250->sensorData.gScaleFactor = 65.5;
        val = 0x08;
        MPU_REG_WRITE(SPIx, pMPU9250, &addr, &val);
        break;
    }
}

/// @brief Read raw data from IMU
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
void MPU_readRawData(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250)
{
    // Init buffer
    uint8_t buf[14];

    // Subroutine for reading the raw data
    MPU_REG_READ(SPIx, pMPU9250, ACCEL_XOUT_H, &buf[0], 14);

    // Bit shift the data
    pMPU9250->rawData.ax = buf[0] << 8 | buf[1];
    pMPU9250->rawData.ay = buf[2] << 8 | buf[3];
    pMPU9250->rawData.az = buf[4] << 8 | buf[5];
    // temperature = buf[6] << 8 | buf[7];
    pMPU9250->rawData.gx = buf[8]  << 8 | buf[9];
    pMPU9250->rawData.gy = buf[10] << 8 | buf[11];
    pMPU9250->rawData.gz = buf[12] << 8 | buf[13];

    //pMPU9250->rawData.mx = buf[15] << 8 | buf[14];
    //pMPU9250->rawData.my = buf[17] << 8 | buf[16];
    //pMPU9250->rawData.mz = buf[19] << 8 | buf[18];
}

/// @brief Find offsets for each axis of gyroscope
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
/// @param numCalPoints Number of data points to average
void MPU_calibrateGyro(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint16_t numCalPoints)
{
    // Init
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    // Zero guard
    if (numCalPoints == 0)
    {
        numCalPoints = 1;
    }

    // Save specified number of points
    for (uint16_t ii = 0; ii < numCalPoints; ii++)
    {
        MPU_readRawData(SPIx, pMPU9250);
        x += pMPU9250->rawData.gx;
        y += pMPU9250->rawData.gy;
        z += pMPU9250->rawData.gz;
        HAL_Delay(3);
    }

    // Average the saved data points to find the gyroscope offset
    pMPU9250->gyroCal.x = (float)x / (float)numCalPoints;
    pMPU9250->gyroCal.y = (float)y / (float)numCalPoints;
    pMPU9250->gyroCal.z = (float)z / (float)numCalPoints;
}

/// @brief Calculate the real world sensor values
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
void MPU_readProcessedData(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250)
{
    // Get raw values from the IMU
    MPU_readRawData(SPIx, pMPU9250);

    // Convert accelerometer values to g's
    pMPU9250->sensorData.ax = pMPU9250->rawData.ax / pMPU9250->sensorData.aScaleFactor;
    pMPU9250->sensorData.ay = pMPU9250->rawData.ay / pMPU9250->sensorData.aScaleFactor;
    pMPU9250->sensorData.az = pMPU9250->rawData.az / pMPU9250->sensorData.aScaleFactor;

    // Compensate for gyro offset
    pMPU9250->sensorData.gx = pMPU9250->rawData.gx - pMPU9250->gyroCal.x;
    pMPU9250->sensorData.gy = pMPU9250->rawData.gy - pMPU9250->gyroCal.y;
    pMPU9250->sensorData.gz = pMPU9250->rawData.gz - pMPU9250->gyroCal.z;

    // Convert gyro values to deg/s
    pMPU9250->sensorData.gx /= pMPU9250->sensorData.gScaleFactor;
    pMPU9250->sensorData.gy /= pMPU9250->sensorData.gScaleFactor;
    pMPU9250->sensorData.gz /= pMPU9250->sensorData.gScaleFactor;

    // ROM ADD
    pMPU9250->sensorData.mx = pMPU9250->rawData.mx - MAG_X_BIAS;
    pMPU9250->sensorData.my = pMPU9250->rawData.my - MAG_Y_BIAS;
    pMPU9250->sensorData.mz = pMPU9250->rawData.mz - MAG_Z_BIAS;

    // Normalize data
    /* Magneto */
    float32_t norm_accelero, norm_magneto;
    arm_status arm_status_temp;
    arm_status_temp = arm_sqrt_f32((float32_t)pMPU9250->sensorData.mx * pMPU9250->sensorData.mx
    			+ (float32_t)pMPU9250->sensorData.my * pMPU9250->sensorData.my
    			+ (float32_t)pMPU9250->sensorData.mz * pMPU9250->sensorData.mz, &norm_magneto);
    if(arm_status_temp != ARM_MATH_SUCCESS)
    {
    	printf("error sqrt of magneto norm! %d \n", arm_status_temp);
    	while(1);
    }
    pMPU9250->sensorData.mx /= norm_magneto;
    pMPU9250->sensorData.my /= norm_magneto;
    pMPU9250->sensorData.mz /= norm_magneto;

    /* Accelero */
    arm_status_temp = arm_sqrt_f32((float32_t)pMPU9250->sensorData.ax * pMPU9250->sensorData.ax
        			+ (float32_t)pMPU9250->sensorData.ay * pMPU9250->sensorData.ay
        			+ (float32_t)pMPU9250->sensorData.az * pMPU9250->sensorData.az, &norm_accelero);
    if(arm_status_temp != ARM_MATH_SUCCESS)
    {
    	printf("error sqrt of accelero norm! %d \n", arm_status_temp);
        while(1);
    }
    pMPU9250->sensorData.ax /= norm_accelero;
    pMPU9250->sensorData.ay /= norm_accelero;
    pMPU9250->sensorData.az /= norm_accelero;
}

/// @brief Calculate the attitude of the sensor in degrees using a complementary filter
/// @param SPIx Pointer to SPI structure config
/// @param pMPU9250 Pointer to master MPU9250 struct
void MPU_calcAttitude(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250)
{
    // Read processed data
    MPU_readProcessedData(SPIx, pMPU9250);

    // Complementary filter
    float accelPitch = atan2(pMPU9250->sensorData.ay, pMPU9250->sensorData.az) * RAD2DEG;
    float accelRoll = atan2(pMPU9250->sensorData.ax, pMPU9250->sensorData.az) * RAD2DEG;

    //float accelPitch = atan2(pMPU9250->sensorData.ay, pMPU9250->sensorData.az);
    //float accelRoll = atan2(pMPU9250->sensorData.ax, pMPU9250->sensorData.az);

    pMPU9250->attitude.r = pMPU9250->attitude.tau * (pMPU9250->attitude.r - pMPU9250->sensorData.gy * pMPU9250->attitude.dt) + (1 - pMPU9250->attitude.tau) * accelRoll;
    pMPU9250->attitude.p = pMPU9250->attitude.tau * (pMPU9250->attitude.p - pMPU9250->sensorData.gx * pMPU9250->attitude.dt) + (1 - pMPU9250->attitude.tau) * accelPitch;
    pMPU9250->attitude.y += (pMPU9250->sensorData.gz * pMPU9250->attitude.dt);
}

void acc_mag2euler(MPU9250_t *pMPU9250)
{
	float32_t pitch_sin, pitch_cos, roll_sin, roll_cos;
	float32_t ay_az_sqrt;

	arm_sqrt_f32(pMPU9250->sensorData.ay * pMPU9250->sensorData.ay +
			pMPU9250->sensorData.az * pMPU9250->sensorData.az, &ay_az_sqrt);

	// [pi,-pi]
	pMPU9250->accel_mag_Attitude.pitch = atan2(-pMPU9250->sensorData.ax, ay_az_sqrt)+0.001;
	pMPU9250->accel_mag_Attitude.roll = atan2(pMPU9250->sensorData.ay, pMPU9250->sensorData.az)+0.001;

	pitch_sin = -pMPU9250->sensorData.ax;
	pitch_cos = ay_az_sqrt;
	roll_sin = pMPU9250->sensorData.ay / ay_az_sqrt;
	roll_cos = pMPU9250->sensorData.az / ay_az_sqrt;

	pMPU9250->accel_mag_Attitude.yaw = atan2(pMPU9250->sensorData.mz * roll_sin -
	pMPU9250->sensorData.my * roll_cos, pMPU9250->sensorData.mx * pitch_cos + pitch_sin *
	(pMPU9250->sensorData.mz * roll_cos + pMPU9250->sensorData.my * roll_sin))+0.001;
}

void acc_mag_gyro2euler(MPU9250_t *pMPU9250)
{
	float32_t pitch_sin, pitch_cos, roll_sin, roll_cos;
	pitch_sin = arm_sin_f32(pMPU9250->accel_mag_Attitude.pitch);
	pitch_cos = arm_cos_f32(pMPU9250->accel_mag_Attitude.pitch);
	roll_sin  = arm_sin_f32(pMPU9250->accel_mag_Attitude.roll);
	roll_cos  = arm_cos_f32(pMPU9250->accel_mag_Attitude.roll);
	pMPU9250->gyro_Attitude.roll += ((float32_t)pMPU9250->sensorData.gx + pitch_sin / pitch_cos *
			((float32_t)pMPU9250->sensorData.gy * roll_sin +
					(float32_t)pMPU9250->sensorData.gz * roll_cos));
	pMPU9250->gyro_Attitude.pitch += ((float32_t)pMPU9250->sensorData.gy * roll_cos
			- (float32_t)pMPU9250->sensorData.gz * roll_sin);
	pMPU9250->gyro_Attitude.yaw +=  ((float32_t)pMPU9250->sensorData.gy * roll_sin
			+ (float32_t)pMPU9250->sensorData.gz *roll_cos) / pitch_cos;

	pitch_cos = arm_cos_f32(pMPU9250->gyro_Attitude.pitch);
	// alternative solution
	if(pitch_cos < 0)
	{
		pMPU9250->gyro_Attitude.pitch = PI - pMPU9250->gyro_Attitude.pitch;
		pMPU9250->gyro_Attitude.roll = PI + pMPU9250->gyro_Attitude.roll ;
		pMPU9250->gyro_Attitude.yaw = PI + pMPU9250->gyro_Attitude.yaw;
	}


	// [pi, -pi]
	if(pMPU9250->gyro_Attitude.roll > PI)
	{
		pMPU9250->gyro_Attitude.roll = pMPU9250->gyro_Attitude.roll - 2 * PI;
	}
	if(pMPU9250->gyro_Attitude.roll < -PI)
	{
		pMPU9250->gyro_Attitude.roll = pMPU9250->gyro_Attitude.roll + 2 * PI;
	}

	if(pMPU9250->gyro_Attitude.pitch > PI)
	{
		pMPU9250->gyro_Attitude.pitch = pMPU9250->gyro_Attitude.pitch - 2 * PI;
	}
	if(pMPU9250->gyro_Attitude.pitch < -PI)
	{
		pMPU9250->gyro_Attitude.pitch = pMPU9250->gyro_Attitude.pitch + 2 * PI;
	}

	if(pMPU9250->gyro_Attitude.yaw > PI)
	{
		pMPU9250->gyro_Attitude.yaw = pMPU9250->gyro_Attitude.yaw - 2 * PI;
	}
	if(pMPU9250->gyro_Attitude.yaw < -PI)
	{
		pMPU9250->gyro_Attitude.yaw = pMPU9250->gyro_Attitude.yaw + 2 * PI;
	}

}
void print_accelero(MPU9250_t *pMPU9250)
{
	printf("Accelero : %d, %d, and %d \n", (int)pMPU9250->sensorData.ax, (int)pMPU9250->sensorData.ay, (int)pMPU9250->sensorData.az);
}
void print_gyro(MPU9250_t *pMPU9250)
{
	printf("Gyro : %d, %d, and %d \n", (int)pMPU9250->sensorData.gx, (int)pMPU9250->sensorData.gy, (int)pMPU9250->sensorData.gz);
}
void print_magneto(MPU9250_t *pMPU9250)
{

	printf("Magneto : %d, %d, and %d \n", (int)pMPU9250->sensorData.mx, (int)pMPU9250->sensorData.my, (int)pMPU9250->sensorData.mz);
}
