/*
 * imu_data.c
 *
 *  Created on: Feb 14, 2024
 *      Author: mr_robot
 */

#include "imu_data.h"
#include "stdio.h"

void sensor2imu(mpu_data icm_data, imu_norm* imu)
{
	float32_t norm_acc, norm_mag;
	arm_status arm_status_temp;

	arm_status_temp = arm_sqrt_f32((float32_t)icm_data.x_accel * icm_data.x_accel + (float32_t)icm_data.y_accel * icm_data.y_accel
			+ icm_data.z_accel * icm_data.z_accel, &norm_acc);

	if(arm_status_temp != ARM_MATH_SUCCESS)
	{
		printf("error sqrt! %d \n", arm_status_temp);
		while(1);
	}
	arm_status_temp = arm_sqrt_f32((float32_t)icm_data.x_mag * icm_data.x_mag
			+ (float32_t)icm_data.y_mag * icm_data.y_mag
			+ (float32_t)icm_data.z_mag * icm_data.z_mag, &norm_mag);

	if(arm_status_temp != ARM_MATH_SUCCESS)
	{
		printf("error sqrt! %d \n", arm_status_temp);
		while(1);
	}
	imu->ax =  ((float32_t)icm_data.y_accel) / norm_acc;
	imu->ay =  ((float32_t)icm_data.x_accel) / norm_acc;
	imu->az = -((float32_t)icm_data.z_accel) / norm_acc;

	imu->gx =  ((float32_t)icm_data.y_gyro) * GYRO_SCALE;
	imu->gy =  ((float32_t)icm_data.x_gyro) * GYRO_SCALE;
	imu->gz = -((float32_t)icm_data.z_gyro) * GYRO_SCALE;

	imu->mx =  ((float32_t)icm_data.x_mag) / norm_mag;
	imu->my =  ((float32_t)icm_data.y_mag) / norm_mag;
	imu->mz =  ((float32_t)icm_data.z_mag) / norm_mag;
}
