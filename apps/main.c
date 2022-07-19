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
	LedConfig();      //配置灯
	SysTicks_Config(168);  //配置滴答定时
	USART_INIT_407(USART1, 115200, 3,0);
	
	
	/********电机初始化部分****************************/
	Motor_init();
	/*************************************************/
	
	
	
	/********mpu6050初始化部分**************************/
	MPU_Init();
	ret = mpu_dmp_init();
	if(ret){
		printf("mpu_dmp_init fail,ret = %d\r\n",ret);
		while(1);
	}
	/*************************************************/
	
	
	/********oled初始化部分**************************/
	oled_Init();
	/*************************************************/
	
	while(1)
	{	
			//频率为200Hz，每5ms执行一次，读取DMP数据
			Task_200Hz();
		
		
			//频率为100Hz，每10ms执行一次,控制电机运动
			if(0 == (mysystick % 2)){
					Task_100Hz();
				}

			
			
			
			//频率为10Hz，每100ms执行一次，oled显示数据
			if(0 == (mysystick % 20)){
				OLED_USAER_SHOW();
			}
			
			
			
			mysystick++;
			if(mysystick > 200)	mysystick = 0;
			Systicks_Nms(5);
	}
}



