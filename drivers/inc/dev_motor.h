#ifndef _DEV_MOTOR_H_
#define _DEV_MOTOR_H_

#include "stm32f4xx.h"

/*-------------------M1电机相关宏定义------------------------------------*/
#define M1_RCC_CLOCK	(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC)
#define	M1_DIR_F_TYPE	GPIOC
#define M1_DIR_F_PIN	GPIO_Pin_3
#define M1_DIR_F_1		GPIO_SetBits(M1_DIR_F_TYPE,M1_DIR_F_PIN)
#define M1_DIR_F_0		GPIO_ResetBits(M1_DIR_F_TYPE,M1_DIR_F_PIN)

#define	M1_DIR_B_TYPE	GPIOA
#define M1_DIR_B_PIN	GPIO_Pin_3
#define M1_DIR_B_1		GPIO_SetBits(M1_DIR_B_TYPE,M1_DIR_B_PIN)
#define M1_DIR_B_0		GPIO_ResetBits(M1_DIR_B_TYPE,M1_DIR_B_PIN)

/*M1电机pwm输出控制速度，将复用为TIM5_CH3*/
#define M1_SPEED_TYPE	GPIOA
#define M1_SPEED_PIN	GPIO_Pin_2
#define M1_SPEED_1		GPIO_SetBits(M1_SPEED_TYPE,M1_SPEED_PIN)
#define M1_SPEED_0		GPIO_ResetBits(M1_SPEED_TYPE,M1_SPEED_PIN)


/*M1电机编码器A、B相引脚，复用TIM5*/
#define M1_ENCODER_TYPE			GPIOA
#define M1_ENCODER_A_PIN		GPIO_Pin_0
#define M1_ENCODER_B_PIN		GPIO_Pin_1




/*-------------------M2电机相关宏定义------------------------------------*/
#define M2_RCC_CLOCK	(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD)
#define	M2_DIR_F_TYPE	GPIOE
#define M2_DIR_F_PIN	GPIO_Pin_13
#define M2_DIR_F_1		GPIO_SetBits(M2_DIR_F_TYPE,M2_DIR_F_PIN)
#define M2_DIR_F_0		GPIO_ResetBits(M2_DIR_F_TYPE,M2_DIR_F_PIN)

#define	M2_DIR_B_TYPE	GPIOE
#define M2_DIR_B_PIN	GPIO_Pin_15
#define M2_DIR_B_1		GPIO_SetBits(M2_DIR_B_TYPE,M2_DIR_B_PIN)
#define M2_DIR_B_0		GPIO_ResetBits(M2_DIR_B_TYPE,M2_DIR_B_PIN)

/*将复用为TIM4_CH1*/
#define M2_SPEED_TYPE	GPIOD
#define M2_SPEED_PIN	GPIO_Pin_12
#define M2_SPEED_1		GPIO_SetBits(M2_SPEED_TYPE,M2_SPEED_PIN)
#define M2_SPEED_0		GPIO_ResetBits(M2_SPEED_TYPE,M2_SPEED_PIN)


typedef enum dir{
	FORWARD = 0,
	BACK,
	STOP,
}dir_t;


void Motor_init(void);
void Motor_Dir_init(void);
void M1_Encoder_init(void);
void Read_Encoder(int * M1_Encoder_Value,int * M2_Encoder_Value);
void Motor_Speed_init(uint16_t arr,uint16_t psc);
void M1_DIR_Contorl(dir_t dir);
void M2_DIR_Contorl(dir_t dir);
void Balance_Dir_Contorl(dir_t dir);
void Balance_Speed_Contorl(uint32_t M1_Speed,uint32_t M2_Speed);
void Motor_Contorl(int32_t M1_Speed,int32_t M2_Speed);
int	Vertical_Ring_PD(float Angle,short Gyro);
#endif


