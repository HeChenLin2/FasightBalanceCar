#ifndef _TASK_H_
#define _TASK_H_

#include "stm32f4xx.h"
#include "dev_systickstimer.h"
#include "dev_usart.h"
#include "dev_motor.h"
#include "stdio.h"
#include "dev_mpu6050.h"
#include "inv_mpu_user.h"
#include "dev_oled.h"

void Task_200Hz(void);
void Task_100Hz(void);

#endif

