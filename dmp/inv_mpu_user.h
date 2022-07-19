#ifndef _INV_MPU_USER_H_
#define _INV_MPU_USER_H_

#include "stm32f4xx.h"
#include "inv_mpu.h"
#include "dev_mpu6050.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct dmp_data{
	float pitch;
	float roll;
	float yaw;
};

struct accelerated_speed{
	short x;
	short y;
	short z;
};

struct gyroscope {
	short x;
	short y;
	short z;
};

extern struct dmp_data myDmpData;			//dmp解算后的姿态角度
extern struct accelerated_speed acc;	//加速度计原始数据
extern struct gyroscope gyrodata; 				//陀螺仪原始数据


uint8_t mpu_dmp_init(void);
uint8_t mpu_dmp_get_data(float *pitch,float *roll,float *yaw);

#endif

