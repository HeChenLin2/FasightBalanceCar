#include "dev_led.h"

void LedConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(LED_RCC_CLOCK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_OUT;     
	GPIO_InitStruct.GPIO_OType            =GPIO_OType_PP;      
	GPIO_InitStruct.GPIO_Pin              =LED_R_PIN;
	GPIO_InitStruct.GPIO_Speed						=GPIO_Speed_2MHz;
	GPIO_Init(LED_R_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =LED_G_PIN;
	GPIO_Init(LED_G_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =LED_B_PIN;
	GPIO_Init(LED_B_TYPE, &GPIO_InitStruct);
	LedControl(WHIT,CLOSE);
	
}


void LedControl(ledtype_t ledtype,ledstu_t ledstu)
{
	switch(ledtype)
	{
		case RED:
			if(ledstu == OPEN)	GPIO_ResetBits(LED_R_TYPE,LED_R_PIN);
			else	GPIO_SetBits(LED_R_TYPE,LED_R_PIN);
			break;
		case GREEN:
			if(ledstu == OPEN)	GPIO_ResetBits(LED_G_TYPE,LED_G_PIN);
			else	GPIO_SetBits(LED_G_TYPE,LED_G_PIN);
			break;
		case BLUE:
			if(ledstu == OPEN)	GPIO_ResetBits(LED_B_TYPE,LED_B_PIN);
			else	GPIO_SetBits(LED_B_TYPE,LED_B_PIN);
			break;
		case WHIT:
			if(ledstu == OPEN){
				LedControl(RED,OPEN);
				LedControl(GREEN,OPEN);
				LedControl(BLUE,OPEN);
			}else{
				LedControl(RED,CLOSE);
				LedControl(GREEN,CLOSE);
				LedControl(BLUE,CLOSE);
			}
			break;
		default:
			break;
	}

}