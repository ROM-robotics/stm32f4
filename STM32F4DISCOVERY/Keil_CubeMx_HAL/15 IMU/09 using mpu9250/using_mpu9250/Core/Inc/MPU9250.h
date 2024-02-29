/*
 * MPU9250.h
 *
 *  Created on: Mar 13, 2022
 *      Author: MarkSherstan
 */

#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_

// Libraries
#include <stdint.h>
#include <math.h>
#include "spi.h"
#include "stdio.h"
#include "arm_math.h"
// Constants
#define RAD2DEG 57.2957795131

// Defines
#define WHO_AM_I_9250_ANS 0x71
#define WHO_AM_I          0x75
#define USER_CTRL         0x6A
#define PWR_MGMT_1        0x6B
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_XOUT_H      0x3B
#define READWRITE         0x80
#define CS_SELECT         0
#define CS_DESELECT       1
#define SPI_TIMOUT_MS     1

/* ROM Dynamic Add */
// Registers related to I2C Master
#define USR_CTRL_REG			106
#define I2C_MSTR_CTRL_REG		36

#define USR_CTRL_REG_I2CEN		0x20
#define I2C_MSTR_CTRL_400		0x0D

#define I2C_SLV0_ADDR_REG		37
#define I2C_SLV0_REG_REG		38
#define I2C_SLV0_CTRL_REG		39
#define I2C_SLV0_DATA_REG		99

#define I2C_SLV0_CTRL_EN		0x80

// Magnetometer
#define AK8963_ADDRESS			0x0C
#define AK8963_CTRL_REG			0x0A
#define AK8963_SEN_ONSET_REG	0x10
#define AK8963_DATA_ONSET_REG	0x03

// Magnetometer operation mode
#define AK8963_PWR_DWN			0x00
#define AK8963_FUSE_ROM			0x0F
#define AK8963_CONT_MEAS2		0x16

#define EXT_SENS_DATA_00_REG 	23

#define XG_OFFSET_H				0x13
#define XG_OFFSET_L				0x14
#define YG_OFFSET_H				0x15
#define YG_OFFSET_L				0x16
#define ZG_OFFSET_H				0x17
#define ZG_OFFSET_L				0x18

#define MAG_X_BIAS				293 // MAYBE -
#define MAG_Y_BIAS				243 // MAYBE 143
#define MAG_Z_BIAS				-316

/* ROM Dynamics END */

// Full scale ranges
enum gyroscopeFullScaleRange
{
    GFSR_250DPS,
    GFSR_500DPS,
    GFSR_1000DPS,
    GFSR_2000DPS
};

enum accelerometerFullScaleRange
{
    AFSR_2G,
    AFSR_4G,
    AFSR_8G,
    AFSR_16G
};

typedef struct{
	float32_t roll;
	float32_t pitch;
	float32_t yaw;
}euler_angles;
// Master structure
typedef struct MPU9250
{
    struct RawData
    {
        int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
    } rawData;

    struct SensorData
    {
        float aScaleFactor, gScaleFactor;
        float ax, ay, az, gx, gy, gz, mx, my, mz;
    } sensorData;

    struct GyroCal
    {
        float x, y, z;
    } gyroCal;

    struct Attitude
    {
        float tau, dt;
        float r, p, y;
    } attitude;
    euler_angles accel_mag_Attitude;
    euler_angles gyro_Attitude;

    struct Settings
    {
    	uint8_t aFullScaleRange, gFullScaleRange;
    	GPIO_TypeDef *CS_PORT;
        uint8_t CS_PIN;
    } settings;
} MPU9250_t;



typedef struct
{
	float32_t ax;
	float32_t ay;
	float32_t az;
	float32_t mx;
	float32_t my;
	float32_t mz;
	float32_t gx;
	float32_t gy;
	float32_t gz;
}imu_norm;

// Functions
uint8_t MPU_begin(SPI_HandleTypeDef *SPIx, MPU9250_t *mpuStruct);
void MPU_REG_READ(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t addr, uint8_t *pRxData, uint16_t RxSize);
void MPU_REG_WRITE(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t *pAddr, uint8_t *pVal);
void MPU_writeGyroFullScaleRange(SPI_HandleTypeDef *SPIx,  MPU9250_t *pMPU9250, uint8_t gScale);
void MPU_writeAccFullScaleRange(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t aScale);
void MPU_calibrateGyro(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint16_t numCalPoints);
void MPU_readProcessedData(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250);
void MPU_calcAttitude(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250);
void MPU_readRawData(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250);
void MPU_CS(MPU9250_t *pMPU9250, uint8_t state);

void ak8963_write_reg(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t reg, uint8_t data);
void ak8963_read_reg(SPI_HandleTypeDef *SPIx, MPU9250_t *pMPU9250, uint8_t reg, uint8_t *data, uint8_t len);

void print_accelero(MPU9250_t *pMPU9250);
void print_gyro(MPU9250_t *pMPU9250);
void print_magneto(MPU9250_t *pMPU9250);

void acc_mag2euler(MPU9250_t *pMPU9250);
void acc_mag_gyro2euler(MPU9250_t *pMPU9250);
#endif /* INC_MPU9250_H_ */
