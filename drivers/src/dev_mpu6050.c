#include "dev_mpu6050.h"

//mpu6050向寄存器写入一个值
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data)
{
	IIC_Start();
	IIC_SendByteData(MPU_ADDR<<1);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
		return 1;
	}
	
	IIC_SendByteData(reg);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
		return 2;
	}
	
	IIC_SendByteData(data);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
		return 3;
	}
	
	IIC_Stop();
	return 0;
}

//mpu6050从寄存器读出一个值
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t data;
	IIC_Start();
	IIC_SendByteData(MPU_ADDR<<1);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
		return 1;
	}
	
	IIC_SendByteData(reg);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
		return 2;
	}
	do
	{
		IIC_Start();
		IIC_SendByteData((MPU_ADDR<<1) + 1);
	}while(NOACK == IIC_WaitACK());	
	
	data = IIC_RxByteData(NOACK);
	IIC_Stop();
	return data;
}

uint8_t MPU_Get_ID(void)
{
	uint8_t id;
	MPU_Read_Len(MPU_ADDR,MPU_DEVICE_ID_REG,1,&id);
	printf("mpu6050 id :%#x\r\n",id);
	return id;
}

uint8_t MPU_Init(void)
{
	IIC_Config();
	//验证MPU6050 ID是否正确，验证IIC通信是否成功
	if(0x68 != MPU_Get_ID())
	{
		printf("MPU6050 Get id fail\r\n");
		return 1;
	}
	
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
	Systicks_Nms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);										//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);										//加速度传感器,±2g
	MPU_Set_Rate(50);												//设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);		//关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);		//关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);		//设置CLKSEL,PLL X轴为参考
	MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);		//加速度与陀螺仪都工作
	MPU_Set_Rate(50);													//设置采样率为50Hz
	return 0;
}


/**********************************************
函数名称：MPU_Set_Gyro_Fsr
函数功能：设置MPU6050陀螺仪传感器满量程范围
函数参数：fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3); //设置陀螺仪满量程范围
}

/**********************************************
函数名称：MPU_Set_Accel_Fsr
函数功能：设置MPU6050加速度传感器满量程范围
函数参数：fsr:0,±2g;1,±4g;2,±8g;3,±16g
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3); //设置加速度传感器满量程范围  
}

/**********************************************
函数名称：MPU_Set_LPF
函数功能：设置MPU6050的数字低通滤波器
函数参数：lpf:数字低通滤波频率(Hz)
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}


/**********************************************
函数名称：MPU_Set_Rate
函数功能：设置MPU6050的采样率(假定Fs=1KHz)
函数参数：rate:4~1000(Hz)  初始化中rate取50
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);											//自动设置LPF为采样率的一半
}

uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i;
	IIC_Start();
	IIC_SendByteData((addr<<1) | 0);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
//		printf("---9----\r\n");
		return 1;
	}
	
	IIC_SendByteData(reg);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
//		printf("---10----\r\n");
		return 2;
	}
	
	for(i = 0;i < len;i++)
	{
			IIC_SendByteData(buf[i]);
			if(NOACK == IIC_WaitACK()){
				IIC_Stop();
//				printf("---11----\r\n");
				return 3;
		}
	}
	IIC_Stop();
	return 0;
}

uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i;
	IIC_Start();
	IIC_SendByteData((addr<<1) | 0);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
		return 1;
	}
	
	IIC_SendByteData(reg);
	if(NOACK == IIC_WaitACK()){
		IIC_Stop();
		return 2;
	}
	
	do
	{
		IIC_Start();
		IIC_SendByteData((addr<<1) | 1);
	}while(NOACK == IIC_WaitACK());	
	
	for(i = 0;i < len;i++)
	{
		if(i != len -1 )
			buf[i] = IIC_RxByteData(ACK);
		else
			buf[i] = IIC_RxByteData(NOACK);
	}

	
	IIC_Stop();
	return 0;
}


