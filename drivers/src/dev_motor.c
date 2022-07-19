#include "dev_motor.h"
#include "stdlib.h"

float Balance_Kp = 390,Balance_Kd = -10;



/***********************************************************
*������:Motor_Dir_init()
*����:��ʼ�����������Ƶ�����
*�β�:��
*����ֵ:��
************************************************************/
void Motor_Dir_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(M1_RCC_CLOCK | M2_RCC_CLOCK, ENABLE);
	
	/*-----------M1���-start-----------------------------------*/
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_OUT;     
	GPIO_InitStruct.GPIO_OType            =GPIO_OType_PP;      
	GPIO_InitStruct.GPIO_Pin              =M1_DIR_F_PIN;
	GPIO_InitStruct.GPIO_Speed						=GPIO_Speed_2MHz;
	GPIO_Init(M1_DIR_F_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =M1_DIR_B_PIN;
	GPIO_Init(M1_DIR_B_TYPE, &GPIO_InitStruct);
	
	M1_DIR_F_0;
	M1_DIR_B_0;

	
	/*-----------M1���-end-----------------------------------*/
	
	/*-----------M2���-start-----------------------------------*/
	GPIO_InitStruct.GPIO_Pin              =M2_DIR_F_PIN;
	GPIO_Init(M2_DIR_F_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =M2_DIR_B_PIN;
	GPIO_Init(M2_DIR_B_TYPE, &GPIO_InitStruct);
	
	M2_DIR_F_0;
	M2_DIR_B_0;
	
	/*-----------M2���-end-----------------------------------*/
}



/***********************************************************
*������:M1_DIR_Contorl(dir_t dir)
*����:M1����ķ������
*�β�:FORWARD(ǰ��)��BACK(����)��STOP(ֹͣ)
*����ֵ:��
************************************************************/
void M1_DIR_Contorl(dir_t dir)
{
	switch(dir)
	{
		case FORWARD:
			//ǰ��
			M1_DIR_F_0;
			M1_DIR_B_1;
			break;
		case BACK:
			//����
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
*������:M2_DIR_Contorl(dir_t dir)
*����:M2����ķ������
*�β�:FORWARD(ǰ��)��BACK(����)��STOP(ֹͣ)
*����ֵ:��
************************************************************/
void M2_DIR_Contorl(dir_t dir)
{
	switch(dir)
	{
		case FORWARD:
			//ǰ��
			M2_DIR_F_1;
			M2_DIR_B_0;
			break;
		case BACK:
			//����
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
*������:Balance_Dir_Contorl(dir_t dir)
*����:ƽ�⳵����ķ������
*�β�:FORWARD(ǰ��)��BACK(����)��STOP(ֹͣ)
*����ֵ:��
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
*������:Motor_Speed_init(uint16_t arr,uint16_t psc)
*����:����ٶȵ�pwm��ʼ��
*�β�:arr�������ڡ�pscʱ�ӷ�Ƶ
*����ֵ:��
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
	
	//PA2����ΪTIM5_CH3
	GPIO_PinAFConfig(M1_SPEED_TYPE,  GPIO_PinSource2, GPIO_AF_TIM5);
	//PD12����ΪTIM4_CH1
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
*������:Balance_Speed_Contorl(uint32_t M1_Speed,uint32_t M2_Speed)
*����:������ٶȿ���
*�β�:M1_Speed(M1������ٶ�)��M2_Speed(M2������ٶ�)
*����ֵ:��
************************************************************/
void Balance_Speed_Contorl(uint32_t M1_Speed,uint32_t M2_Speed)
{
	TIM_SetCompare3(TIM5,M1_Speed);
	TIM_SetCompare1(TIM4,M2_Speed);
}





/***********************************************************
*������:Motor_Contorl(int32_t M1_Speed,int32_t M2_Speed)
*����:������ٶȿ��ƣ����ܿ��Ʒ���
*�β�:M1_Speed(M1������ٶ�)��M2_Speed(M2������ٶ�)
*����ֵ:��
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
*������:M1_Encoder_init(void)
*����:���1��������ʼ��
*�β�:��
*����ֵ:��
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
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising); // ���������˱�����ģʽ

	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 8; 		//���벶���˲�,��������Լ����ã���ʹ��Ĭ��ֵ
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);	
}



/***********************************************************
*������:M1_Read_Encoder(void)
*����:��ȡ���1������������ֵ
*�β�:��
*����ֵ:�������ļ���ֵ
************************************************************/
int M1_Read_Encoder(void)
{
	int encoder_num;
	encoder_num = ((short)(TIM2->CNT)); // ����������Ҫע����������
	TIM_SetCounter(TIM2, 0);
	return encoder_num;
}


/***********************************************************
*������:Read_Encoder
*����:��ȡ���������������ֵ
*�β�:��
*����ֵ:��
************************************************************/
void Read_Encoder(int * M1_Encoder_Value,int * M2_Encoder_Value)
{
	*M1_Encoder_Value = M1_Read_Encoder();
	*M2_Encoder_Value = 0;
}



/**************************************************************************************************************
*������:Vertical_Ring_PD()
*����:ֱ����PD����
*�β�:(float Angle):x��ĽǶ�/(float Gyro):x��Ľ��ٶ�
*����ֵ:����PIDת��֮���PWMֵ
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
*������:Motor_init(void)
*����:���ڵ���ĳ�ʼ��
*�β�:��
*����ֵ:��
************************************************************/
void Motor_init(void)
{
	//����ķ����ʼ��
	Motor_Dir_init();
	
	//������ٶȳ�ʼ��
	Motor_Speed_init(10000-1,84-1);
	Balance_Dir_Contorl(STOP);
	
	//���1�ı�������ʼ��
	M1_Encoder_init();
}
