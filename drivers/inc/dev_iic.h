#ifndef _DEV_IIC_H_
#define _DEV_IIC_H_

#include "stm32f4xx.h"
#include "dev_systickstimer.h"
#include "dev_usart.h"
#include "stdio.h"

/*
scl		PD0
sda		PC11
*/

#define	IIC_RCC_CLOCK		(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC)
#define SCL_TYPE	GPIOD
#define	SCL_PIN		GPIO_Pin_0
#define	SCL_HIGH	GPIO_SetBits(SCL_TYPE,SCL_PIN)
#define	SCL_LOW   GPIO_ResetBits(SCL_TYPE,SCL_PIN)


#define	SDA_TYPE	GPIOC
#define	SDA_PIN		GPIO_Pin_11
#define	SDA_HIGH	GPIO_SetBits(SDA_TYPE,SDA_PIN)
#define	SDA_LOW   GPIO_ResetBits(SDA_TYPE,SDA_PIN)
#define	SDA_Read  GPIO_ReadInputDataBit(SDA_TYPE, SDA_PIN)

#define    ACK      0
#define    NOACK    1

void IIC_Config(void);
void IIC_SDA_IN(void);
void IIC_SDA_OUT(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendAck(void);
void IIC_SendNoAck(void);
uint8_t IIC_WaitACK(void);
void IIC_SendByteData(uint8_t DATA);
uint8_t IIC_RxByteData(uint8_t ACKflag);


#endif

