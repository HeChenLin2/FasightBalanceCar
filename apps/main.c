#include "dev_led.h"
#include "dev_systickstimer.h"
#include "dev_usart.h"
#include "dev_motor.h"
#include "stdio.h"
#include "dev_mpu6050.h"
#include "inv_mpu_user.h"
#include "dev_oled.h"
#include "task.h"


int main(void)
{
	uint8_t ret,mysystick = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	LedConfig();      //���õ�
	SysTicks_Config(168);  //���õδ�ʱ
	USART_INIT_407(USART1, 115200, 3,0);
	
	
	/********�����ʼ������****************************/
	Motor_init();
	/*************************************************/
	
	
	
	/********mpu6050��ʼ������**************************/
	MPU_Init();
	ret = mpu_dmp_init();
	if(ret){
		printf("mpu_dmp_init fail,ret = %d\r\n",ret);
		while(1);
	}
	/*************************************************/
	
	
	/********oled��ʼ������**************************/
	oled_Init();
	/*************************************************/
	
	while(1)
	{	
			//Ƶ��Ϊ200Hz��ÿ5msִ��һ�Σ���ȡDMP����
			Task_200Hz();
		
		
			//Ƶ��Ϊ100Hz��ÿ10msִ��һ��,���Ƶ���˶�
			if(0 == (mysystick % 2)){
					Task_100Hz();
				}

			
			
			
			//Ƶ��Ϊ10Hz��ÿ100msִ��һ�Σ�oled��ʾ����
			if(0 == (mysystick % 20)){
				OLED_USAER_SHOW();
			}
			
			
			
			mysystick++;
			if(mysystick > 200)	mysystick = 0;
			Systicks_Nms(5);
	}
}



