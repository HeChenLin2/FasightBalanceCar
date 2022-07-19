#include "task.h"


volatile short Gyro_Balance = 0,Gyro_Turn = 0;			//x���z��������ֵ
int Balance_Pwm = 0,Velocity_Pwm = 0,Turn_Pwm = 0;	//����PID���������PWMֵ

/***********************************************************************
*������:Task_200Hz
*����:������ƵΪ200Hz�����񣨻�ȡdmp���ݣ�
*�β�:��
*����ֵ:��
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
*������:Task_100Hz
*����:������ƵΪ100Hz�����񣨿��Ƶ���˶���
*�β�:��
*����ֵ:��
***********************************************************************/
void Task_100Hz(void)
{
	uint8_t buf[50];
	int M1_Encoder_Value = 0,M2_Encoder_Value = 0;
	
	//��ȡ���������ת��
	Read_Encoder(&M1_Encoder_Value,&M2_Encoder_Value);
	
	/*++++++++++++++++++++++++++++++++++++ֱ����PD+++++++++++++++++++++++++++++++++++++++++++++*/
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

