#ifndef _DEV_USART1_H_
#define  _DEV_USART1_H_

#include "stm32f4xx.h"
#include "stdio.h"

void USART_INIT_407(USART_TypeDef * USARTx,uint32_t Baud_Rate,uint8_t IRQChannel_Priority,uint8_t IRQChannelSubPriority);


#endif
