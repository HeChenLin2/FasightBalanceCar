#include "dev_usart.h"


int fputc (int ch,FILE *f)
{
	while(SET !=USART_GetFlagStatus(USART1,USART_FLAG_TC));
	USART_SendData(USART1,ch);
	return ch;
}




void USART_INIT_407(USART_TypeDef * USARTx,uint32_t Baud_Rate,uint8_t IRQChannel_Priority,uint8_t IRQChannelSubPriority)
{
	USART_InitTypeDef   USART_InitStruct;
	GPIO_InitTypeDef  GPIO_InitStruct;
		NVIC_InitTypeDef NVIC_InitStruct;
	
	if(USART1  == USARTx){

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);   //开GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
		GPIO_InitStruct.GPIO_Mode                =GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType               =GPIO_OType_PP;   //推挽输出
		GPIO_InitStruct.GPIO_Pin                    =GPIO_Pin_9;
		GPIO_InitStruct.GPIO_PuPd                 =GPIO_PuPd_NOPULL;
		GPIO_InitStruct.GPIO_Speed                =GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Mode                =GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Pin                    = GPIO_Pin_10;
		GPIO_InitStruct.GPIO_PuPd                 =GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);   //将引脚复用到usart1    
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_USART1);

		USART_InitStruct.USART_BaudRate                        = Baud_Rate;
		USART_InitStruct.USART_HardwareFlowControl             =USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode                               =USART_Mode_Rx|USART_Mode_Tx;
		USART_InitStruct.USART_Parity                              =USART_Parity_No;
		USART_InitStruct.USART_StopBits                           =USART_StopBits_1;
		USART_InitStruct.USART_WordLength                      =USART_WordLength_8b;
		USART_Init(USARTx, & USART_InitStruct);
		
		NVIC_InitStruct.NVIC_IRQChannel       =USART1_IRQn;

	}
	
	if(USART2  == USARTx){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);   //开GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		GPIO_InitStruct.GPIO_Mode                =GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType               =GPIO_OType_PP;   //推挽输出
		GPIO_InitStruct.GPIO_Pin                    =GPIO_Pin_2;
		GPIO_InitStruct.GPIO_PuPd                 =GPIO_PuPd_NOPULL;
		GPIO_InitStruct.GPIO_Speed                =GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Mode                =GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Pin                    = GPIO_Pin_3;
		GPIO_InitStruct.GPIO_PuPd                 =GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);   //将引脚复用到usart1    
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_USART2);

		NVIC_InitStruct.NVIC_IRQChannel       =USART2_IRQn;
	}
	
	if(USART3  == USARTx){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);   //开GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		GPIO_InitStruct.GPIO_Mode                =GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_OType               =GPIO_OType_PP;   //推挽输出
		GPIO_InitStruct.GPIO_Pin                    =GPIO_Pin_10;
		GPIO_InitStruct.GPIO_PuPd                 =GPIO_PuPd_NOPULL;
		GPIO_InitStruct.GPIO_Speed                =GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Mode                =GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Pin                    = GPIO_Pin_11;
		GPIO_InitStruct.GPIO_PuPd                 =GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);   //将引脚复用到usart1    
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_USART3);

		NVIC_InitStruct.NVIC_IRQChannel       =USART3_IRQn;
	}
	
	USART_InitStruct.USART_BaudRate                        = Baud_Rate;
	USART_InitStruct.USART_HardwareFlowControl             =USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                               =USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity                              =USART_Parity_No;
	USART_InitStruct.USART_StopBits                           =USART_StopBits_1;
	USART_InitStruct.USART_WordLength                      =USART_WordLength_8b;
	USART_Init(USARTx, & USART_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannelCmd                   =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority    = IRQChannel_Priority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority           = IRQChannelSubPriority;
	NVIC_Init(&NVIC_InitStruct);

	USART_ClearFlag(USARTx,USART_FLAG_RXNE|USART_FLAG_IDLE);

	USART_ITConfig(USARTx, USART_IT_RXNE,ENABLE);
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USARTx, ENABLE);

}

