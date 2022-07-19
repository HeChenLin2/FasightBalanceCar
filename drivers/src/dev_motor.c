#include "dev_motor.h"
#include "stdlib.h"

float Balance_Kp = 390,Balance_Kd = -10;



/***********************************************************
*函数名:Motor_Dir_init()
*功能:初始化电机方向控制的引脚
*形参:无
*返回值:无
************************************************************/
void Motor_Dir_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(M1_RCC_CLOCK | M2_RCC_CLOCK, ENABLE);
	
	/*-----------M1电机-start-----------------------------------*/
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_OUT;     
	GPIO_InitStruct.GPIO_OType            =GPIO_OType_PP;      
	GPIO_InitStruct.GPIO_Pin              =M1_DIR_F_PIN;
	GPIO_InitStruct.GPIO_Speed						=GPIO_Speed_2MHz;
	GPIO_Init(M1_DIR_F_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =M1_DIR_B_PIN;
	GPIO_Init(M1_DIR_B_TYPE, &GPIO_InitStruct);
	
	M1_DIR_F_0;
	M1_DIR_B_0;

	
	/*-----------M1电机-end-----------------------------------*/
	
	/*-----------M2电机-start-----------------------------------*/
	GPIO_InitStruct.GPIO_Pin              =M2_DIR_F_PIN;
	GPIO_Init(M2_DIR_F_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =M2_DIR_B_PIN;
	GPIO_Init(M2_DIR_B_TYPE, &GPIO_InitStruct);
	
	M2_DIR_F_0;
	M2_DIR_B_0;
	
	/*-----------M2电机-end-----------------------------------*/
}



/***********************************************************
*函数名:M1_DIR_Contorl(dir_t dir)
*功能:M1电机的方向控制
*形参:FORWARD(前进)、BACK(后退)、STOP(停止)
*返回值:无
************************************************************/
void M1_DIR_Contorl(dir_t dir)
{
	switch(dir)
	{
		case FORWARD:
			//前进
			M1_DIR_F_0;
			M1_DIR_B_1;
			break;
		case BACK:
			//后退
			M1_DIR_F_1;
			M1_DIR_B_0;
			break;
		case STOP:
			M1_DIR_F_0;
			M1_DIR_B_0;
			break;
		default:
			break;
	}
}


/***********************************************************
*函数名:M2_DIR_Contorl(dir_t dir)
*功能:M2电机的方向控制
*形参:FORWARD(前进)、BACK(后退)、STOP(停止)
*返回值:无
************************************************************/
void M2_DIR_Contorl(dir_t dir)
{
	switch(dir)
	{
		case FORWARD:
			//前进
			M2_DIR_F_1;
			M2_DIR_B_0;
			break;
		case BACK:
			//后退
			M2_DIR_F_0;
			M2_DIR_B_1;
			break;
		case STOP:
			M2_DIR_F_0;
			M2_DIR_B_0;
			break;
		default:
			break;
	}
}


/***********************************************************
*函数名:Balance_Dir_Contorl(dir_t dir)
*功能:平衡车整体的方向控制
*形参:FORWARD(前进)、BACK(后退)、STOP(停止)
*返回值:无
************************************************************/
void Balance_Dir_Contorl(dir_t dir)
{
	switch(dir)
	{
		case FORWARD:
			M1_DIR_Contorl(FORWARD);
			M2_DIR_Contorl(FORWARD);
			break;
		case BACK:
			M1_DIR_Contorl(BACK);
			M2_DIR_Contorl(BACK);
			break;
		case STOP:
			M1_DIR_Contorl(STOP);
			M2_DIR_Contorl(STOP);
			break;
		default:
			break;
	}
}

/***********************************************************
*函数名:Motor_Speed_init(uint16_t arr,uint16_t psc)
*功能:电机速度的pwm初始化
*形参:arr计数周期、psc时钟分频
*返回值:无
************************************************************/
void Motor_Speed_init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5,ENABLE);  	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE); 
	
	
	RCC_AHB1PeriphClockCmd(M1_RCC_CLOCK | M2_RCC_CLOCK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_AF;     
	GPIO_InitStruct.GPIO_OType            =GPIO_OType_PP;      
	GPIO_InitStruct.GPIO_Pin              =M1_SPEED_PIN;
	GPIO_InitStruct.GPIO_Speed						=GPIO_Speed_2MHz;
	GPIO_Init(M1_SPEED_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =M2_SPEED_PIN;
	GPIO_Init(M2_SPEED_TYPE, &GPIO_InitStruct);
	
	//PA2复用为TIM5_CH3
	GPIO_PinAFConfig(M1_SPEED_TYPE,  GPIO_PinSource2, GPIO_AF_TIM5);
	//PD12复用为TIM4_CH1
	GPIO_PinAFConfig(M2_SPEED_TYPE,  GPIO_PinSource12, GPIO_AF_TIM4);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc; 
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStruct.TIM_Period=arr;  
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);  	
	TIM_OC3Init(TIM5, &TIM_OCInitStruct);  
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  
	
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	TIM_Cmd(TIM4, ENABLE); 
	
  TIM_ARRPreloadConfig(TIM5,ENABLE);
	TIM_Cmd(TIM5, ENABLE);  
}




/***********************************************************
*函数名:Balance_Speed_Contorl(uint32_t M1_Speed,uint32_t M2_Speed)
*功能:电机的速度控制
*形参:M1_Speed(M1电机的速度)，M2_Speed(M2电机的速度)
*返回值:无
************************************************************/
void Balance_Speed_Contorl(uint32_t M1_Speed,uint32_t M2_Speed)
{
	TIM_SetCompare3(TIM5,M1_Speed);
	TIM_SetCompare1(TIM4,M2_Speed);
}





/***********************************************************
*函数名:Motor_Contorl(int32_t M1_Speed,int32_t M2_Speed)
*功能:电机的速度控制（且能控制方向）
*形参:M1_Speed(M1电机的速度)，M2_Speed(M2电机的速度)
*返回值:无
************************************************************/
void Motor_Contorl(int32_t M1_Speed,int32_t M2_Speed)
{
	if(M1_Speed > 0){
			M1_DIR_Contorl(FORWARD);
	}else{
			M1_DIR_Contorl(BACK);
	}
	
	if(M2_Speed > 0){
			M2_DIR_Contorl(FORWARD);
	}else{
			M2_DIR_Contorl(BACK);
	}
	
	Balance_Speed_Contorl(abs(M1_Speed),abs(M2_Speed));

}


/***********************************************************
*函数名:M1_Encoder_init(void)
*功能:电机1编码器初始化
*形参:无
*返回值:无
************************************************************/
void  M1_Encoder_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin	=	M1_ENCODER_A_PIN | M1_ENCODER_B_PIN; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(M1_ENCODER_TYPE, &GPIO_InitStruct);

	
	GPIO_PinAFConfig(M1_ENCODER_TYPE,  GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(M1_ENCODER_TYPE,  GPIO_PinSource1, GPIO_AF_TIM2);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode	 = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period				 = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler		 = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising); // 这里配置了编码器模式

	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 8; 		//输入捕获滤波,这里可以自己设置，或使用默认值
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);	
}



/***********************************************************
*函数名:M1_Read_Encoder(void)
*功能:读取电机1编码器计数的值
*形参:无
*返回值:编码器的计数值
************************************************************/
int M1_Read_Encoder(void)
{
	int encoder_num;
	encoder_num = ((short)(TIM2->CNT)); // 这里尤其需要注意数据类型
	TIM_SetCounter(TIM2, 0);
	return encoder_num;
}


/***********************************************************
*函数名:Read_Encoder
*功能:读取电机编码器计数的值
*形参:无
*返回值:无
************************************************************/
void Read_Encoder(int * M1_Encoder_Value,int * M2_Encoder_Value)
{
	*M1_Encoder_Value = M1_Read_Encoder();
	*M2_Encoder_Value = 0;
}



/**************************************************************************************************************
*函数名:Vertical_Ring_PD()
*功能:直立环PD控制
*形参:(float Angle):x轴的角度/(float Gyro):x轴的角速度
*返回值:经过PID转换之后的PWM值
**************************************************************************************************************/
int	Vertical_Ring_PD(float Angle,short Gyro)
{
	 float Bias;
	 int balance;
   Bias=Angle- 0;
   balance=Balance_Kp * Bias+ Gyro * Balance_Kd;
	 return balance;
}




/***********************************************************
*函数名:Motor_init(void)
*功能:关于电机的初始化
*形参:无
*返回值:无
************************************************************/
void Motor_init(void)
{
	//电机的方向初始化
	Motor_Dir_init();
	
	//电机的速度初始化
	Motor_Speed_init(10000-1,84-1);
	Balance_Dir_Contorl(STOP);
	
	//电机1的编码器初始化
	M1_Encoder_init();
}
