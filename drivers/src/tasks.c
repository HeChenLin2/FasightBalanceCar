#include "task.h"


volatile short Gyro_Balance = 0,Gyro_Turn = 0;			//x轴和z轴陀螺仪值
int Balance_Pwm = 0,Velocity_Pwm = 0,Turn_Pwm = 0;	//三个PID环计算出的PWM值

/***********************************************************************
*函数名:Task_200Hz
*功能:运行主频为200Hz的任务（获取dmp数据）
*形参:无
*返回值:无
***********************************************************************/
void Task_200Hz(void)
{
	static struct dmp_data oldDmpData={0,0,0};
	if(0 != mpu_dmp_get_data(&myDmpData.pitch,&myDmpData.roll,&myDmpData.yaw)){
		myDmpData = oldDmpData;
	}else
		oldDmpData = myDmpData;
		Gyro_Balance = gyrodata.x;
}



/***********************************************************************
*函数名:Task_100Hz
*功能:运行主频为100Hz的任务（控制电机运动）
*形参:无
*返回值:无
***********************************************************************/
void Task_100Hz(void)
{
	uint8_t buf[50];
	int M1_Encoder_Value = 0,M2_Encoder_Value = 0;
	
	//获取电机编码器转速
	Read_Encoder(&M1_Encoder_Value,&M2_Encoder_Value);
	
	/*++++++++++++++++++++++++++++++++++++直立环PD+++++++++++++++++++++++++++++++++++++++++++++*/
	Balance_Pwm = Vertical_Ring_PD(myDmpData.roll,Gyro_Balance);
	
	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	
	if(Balance_Pwm > 8500) Balance_Pwm = 8500;
	else if(Balance_Pwm < -8500) Balance_Pwm = -8500;
	if(myDmpData.roll > 30 || myDmpData.roll < -30)
		Balance_Dir_Contorl(STOP);
	else
		Motor_Contorl(Balance_Pwm,Balance_Pwm);
	
	sprintf((char *)buf,"M1: %d  ",M1_Encoder_Value);
	OLED_ShowString(0,6,(const char *)buf, 6);
	
}

