#include "rcc.h"

void Rcc_Config(void)
{
	RCC_HSEConfig(RCC_HSE_ON);
	while(  SUCCESS != RCC_WaitForHSEStartUp());
	RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);    //…Ë÷√À¯œ‡ª∑
	RCC_PLLCmd(ENABLE);
	while( SET != RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div4);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while( RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL);	
}
