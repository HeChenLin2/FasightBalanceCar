#ifndef  _DEV_SYSTICKSTIMER_H_
#define _DEV_SYSTICKSTIMER_H_

#include "stm32f4xx.h"

void SysTicks_Config(u8 clock);
void Systicks_Nus(u32 Nus);  //us��ʱ
void Systicks_Nms(u32 Nms);//����ms��ʱ
void delay_xms(u16 nms);
void dely(u32 m);
#endif
