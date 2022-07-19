#include "dev_iic.h"


void IIC_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(IIC_RCC_CLOCK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_OUT;     
	GPIO_InitStruct.GPIO_OType            =GPIO_OType_PP;      
	GPIO_InitStruct.GPIO_Pin              =SCL_PIN;
	GPIO_InitStruct.GPIO_Speed						=GPIO_Speed_100MHz;
	GPIO_Init(SCL_TYPE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =SDA_PIN;
	GPIO_Init(SDA_TYPE, &GPIO_InitStruct);
}

void IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_OUT;     
	GPIO_InitStruct.GPIO_OType            =GPIO_OType_PP;      
	GPIO_InitStruct.GPIO_Pin              =SDA_PIN;
	GPIO_InitStruct.GPIO_Speed						=GPIO_Speed_100MHz;
	GPIO_Init(SDA_TYPE, &GPIO_InitStruct);
}

void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_IN;         
	GPIO_InitStruct.GPIO_Pin              =SDA_PIN;
	GPIO_InitStruct.GPIO_PuPd							=GPIO_PuPd_UP;
	GPIO_Init(SDA_TYPE, &GPIO_InitStruct);
}

void IIC_Start(void)
{
	SCL_LOW;       //根据数据的有效性，SCL为低电平时数据无效，当我要改变SDA引脚输入输出时，为了防止产生停止信号和开始信号，先让SDA的数据无效，再去做相应的改变
	IIC_SDA_OUT();//起始信号由主机发送一个下降沿，设置成输出模式
	
	SDA_HIGH;//先将SDA置1
	SCL_HIGH;//使sda上的数据有效
	Systicks_Nus(5);//延时
	SDA_LOW;//使sda由高电平变成低电平，产生下降沿
	Systicks_Nus(5);//延时
	
	SCL_LOW;//通信没有结束，钳位住电平，使其他总线不能发起iic通信
}

void IIC_Stop(void)
{
	SCL_LOW;   //根据数据的有效性，SCL为低电平时数据无效，当我要改变SDA引脚输入输出时，为了防止产生停止信号和开始信号，先让SDA的数据无效，再去做相应的改变
	IIC_SDA_OUT();//停止信号由主机发送一个上升沿，设置成输出模式

	SDA_LOW;//先将SDA置0
	SCL_HIGH;//使sda上的数据有效
	Systicks_Nus(5);//延时
	SDA_HIGH;//使sda由低电平变成高电平，产生上升沿
	Systicks_Nus(5);//延时  不需要钳位住电平，通信结束，释放总线
}

void IIC_SendAck(void)
{
	SCL_LOW; //根据数据的有效性，SCL为低电平时数据无效，当我要改变SDA引脚输入输出时，为了防止产生停止信号和开始信号，先让SDA的数据无效，再去做相应的改变
	IIC_SDA_OUT();	//发送应答信号主机作为接收方，向从机发送应答信号，设置成输出模式

	SDA_LOW;//先将SDA置0
	SCL_HIGH;//使sda上的数据有效
	Systicks_Nus(5);//延时

	SCL_LOW;//通信没有结束，钳位住电平，使其他总线不能发起iic通信	
}

void IIC_SendNoAck(void)
{
	SCL_LOW;//根据数据的有效性，SCL为低电平时数据无效，当我要改变SDA引脚输入输出时，为了防止产生停止信号和开始信号，先让SDA的数据无效，再去做相应的改变
	IIC_SDA_OUT();//发送应答信号主机作为接收方，向从机发送非应答信号，设置成输出模式

	SDA_HIGH;//先将SDA置1
	SCL_HIGH;//使sda上的数据有效
	Systicks_Nus(5);//延时

	SCL_LOW;//通信没有结束，钳位住电平，使其他总线不能发起iic通信		
}

uint8_t IIC_WaitACK(void)//等待从机应答
{
	uint8_t count=0;//定义一个超时计数器
	SCL_LOW;//根据数据的有效性，SCL为低电平时数据无效，当我要改变SDA引脚输入输出时，为了防止产生停止信号和开始信号，先让SDA的数据无效，再去做相应的改变
	IIC_SDA_IN();//发送应答信号主机作为接收方，向从机发送非应答信号，设置成输出模式

	SCL_HIGH;//先将SCL置1,使SDA上的数据有效
	Systicks_Nus(4);//延时
	while(SDA_Read)//等待从机发送ACK
	{
		count++; //未读到应答信号，计数器加加，继续读取
		if(count>250)//读取是否超时
		{
			IIC_Stop();//超时,停止IIC
			return NOACK;//返回非应答
		}
	}
	SCL_LOW;//读取到应答信号，钳位电平
	return ACK;//返回应答信号
}

void IIC_SendByteData(uint8_t DATA)//iic发送一个字节数据
{
	uint8_t i;//一个字节8个位，串行发8次，定义一个计数器i
	SCL_LOW;//根据数据的有效性，SCL为低电平时数据无效，当我要改变SDA引脚输入输出时，为了防止产生停止信号和开始信号，先让SDA的数据无效，再去做相应的改变
	IIC_SDA_OUT();//发送应答信号主机作为接收方，向从机发送非应答信号，设置成输出模式

	for(i=0;i<8;i++)//循环8次，每次发送一个位的数据
	{
		if(DATA&0x80)//IIC是高位先行的器件，先发高位，
		{
			SDA_HIGH;//高位为1，sda置1
		}
		else
		{
			SDA_LOW;//高位为0，sda清0
		}
		SCL_HIGH;//使sda上的数据有效
		Systicks_Nus(5);//延时
		SCL_LOW;//拉低，使sda上的数据无效，再去改变sda的值
		Systicks_Nus(5);	//延时
		DATA=DATA<<1;//将次高位移置最高位，再进行发送
	}
	SCL_LOW;////通信没有结束，钳位住电平，使其他总线不能发起iic通信	
}

 uint8_t IIC_RxByteData(uint8_t ACKflag)//iic接收一个字节的数据
 {
 	uint8_t i;//一个字节8个位，串行发8次，定义一个计数器i
	uint8_t data=0;//定义一个内存单元，用来存放接收到的数据
	SCL_LOW;//根据数据的有效性，SCL为低电平时数据无效，当我要改变SDA引脚输入输出时，为了防止产生停止信号和开始信号，先让SDA的数据无效，再去做相应的改变
	IIC_SDA_IN();//发送应答信号主机作为接收方，向从机发送非应答信号，设置成输出模式

	for(i=0;i<8;i++)//循环8次，每次接收一个位的数据
	{
		data=data<<1;//先左移，防止最后的数据不正确
		SCL_HIGH;//使SCL上的数据有效
		Systicks_Nus(4);//延时
		if(SDA_Read)//读取SDA发送过来的数据
		{
			data=data|0x01;//如果读到高电平，将读取的值存入，内存单元
		}
		SCL_LOW;//拉低，使sda上的数据无效
		Systicks_Nus(4);//延时		
	}
	
	if(ACKflag==ACK)//发送ack或者noack
	{
		IIC_SendAck();//发送ACK
	}
	else
	{
		IIC_SendNoAck();//发送noack
	}
	SCL_LOW;////通信没有结束，钳位住电平，使其他总线不能发起iic通信	
	return data;//返回读取到的数据
 }
 

 
 
 