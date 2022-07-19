#ifndef  _DEV_LED_H_
#define  _DEV_LED_H_

#include "stm32f4xx.h"


#define LED_R_TYPE	GPIOE
#define LED_R_PIN		GPIO_Pin_0

#define LED_G_TYPE	GPIOB
#define LED_G_PIN		GPIO_Pin_9

#define LED_B_TYPE	GPIOB
#define LED_B_PIN		GPIO_Pin_8

#define LED_RCC_CLOCK	(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB)

typedef enum ledtype{
	RED = 0,	
	GREEN,
	BLUE,
	WHIT,
}ledtype_t;

typedef enum ledstu{
	OPEN = 0,
	CLOSE,
}ledstu_t;


void LedConfig(void);
void LedControl(ledtype_t ledtype,ledstu_t ledstu);


#endif
