#ifndef _DEV_OLED_H_
#define _DEV_OLED_H_

#include "stm32f4xx.h"
#include "dev_mpu6050.h"
#include "inv_mpu_user.h"
#include "dev_systickstimer.h"
#include "sys.h"


#define OLED_DELAY_MS(nms)	Systicks_Nms(nms)
#define OLED_RCC_CLOCK			(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC)
#define OLED_SCLK_TYBE			GPIOD
#define OLED_SCLK_PIN				GPIO_Pin_3

#define OLED_SDIN_TYBE			GPIOD
#define OLED_SDIN_PIN				GPIO_Pin_5

#define OLED_RS_TYBE				GPIOD
#define OLED_RS_PIN					GPIO_Pin_1

#define OLED_RST_TYBE				GPIOC
#define OLED_RST_PIN				GPIO_Pin_12

#define OLED_CS_TYBE				GPIOC
#define OLED_CS_PIN					GPIO_Pin_10


#define OLED_SCLK PDout(3)			//oledʱ���ߣ�PD3����
#define OLED_SDIN PDout(5)			//oled�����ߣ�PD5����
#define OLED_RS		PDout(1)			//oled �����������ߣ�PD1���ţ�0��д���1��д����
#define OLED_RST 	PCout(12)			//oled��λ�ߣ�PC12����
#define OLED_CS 	PCout(10)			//oledƬѡ�ߣ�PC10����

#define XLevelL			0x00		//���ù���е�ַ�ĵ�4λ
#define XLevelH			0x10		//���ù���е�ַ�ĸ�4λ
#define Max_Column	128			//��Ļ����������
#define Max_Row			64			//��Ļ�߶��������
#define X_WIDTH 		128			
#define Y_WIDTH 		64			

#define OLED_CMD 	0
#define OLED_DATA 1

void oled_GPIO_Init(void);
void oled_Mode_Init(void);
void oled_Init(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Clear(uint8_t mode);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const char *chr, uint8_t size);
void OLED_USAER_SHOW(void);




#endif

