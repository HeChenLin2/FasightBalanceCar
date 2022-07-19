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
	SCL_LOW;       //�������ݵ���Ч�ԣ�SCLΪ�͵�ƽʱ������Ч������Ҫ�ı�SDA�����������ʱ��Ϊ�˷�ֹ����ֹͣ�źźͿ�ʼ�źţ�����SDA��������Ч����ȥ����Ӧ�ĸı�
	IIC_SDA_OUT();//��ʼ�ź�����������һ���½��أ����ó����ģʽ
	
	SDA_HIGH;//�Ƚ�SDA��1
	SCL_HIGH;//ʹsda�ϵ�������Ч
	Systicks_Nus(5);//��ʱ
	SDA_LOW;//ʹsda�ɸߵ�ƽ��ɵ͵�ƽ�������½���
	Systicks_Nus(5);//��ʱ
	
	SCL_LOW;//ͨ��û�н�����ǯλס��ƽ��ʹ�������߲��ܷ���iicͨ��
}

void IIC_Stop(void)
{
	SCL_LOW;   //�������ݵ���Ч�ԣ�SCLΪ�͵�ƽʱ������Ч������Ҫ�ı�SDA�����������ʱ��Ϊ�˷�ֹ����ֹͣ�źźͿ�ʼ�źţ�����SDA��������Ч����ȥ����Ӧ�ĸı�
	IIC_SDA_OUT();//ֹͣ�ź�����������һ�������أ����ó����ģʽ

	SDA_LOW;//�Ƚ�SDA��0
	SCL_HIGH;//ʹsda�ϵ�������Ч
	Systicks_Nus(5);//��ʱ
	SDA_HIGH;//ʹsda�ɵ͵�ƽ��ɸߵ�ƽ������������
	Systicks_Nus(5);//��ʱ  ����Ҫǯλס��ƽ��ͨ�Ž������ͷ�����
}

void IIC_SendAck(void)
{
	SCL_LOW; //�������ݵ���Ч�ԣ�SCLΪ�͵�ƽʱ������Ч������Ҫ�ı�SDA�����������ʱ��Ϊ�˷�ֹ����ֹͣ�źźͿ�ʼ�źţ�����SDA��������Ч����ȥ����Ӧ�ĸı�
	IIC_SDA_OUT();	//����Ӧ���ź�������Ϊ���շ�����ӻ�����Ӧ���źţ����ó����ģʽ

	SDA_LOW;//�Ƚ�SDA��0
	SCL_HIGH;//ʹsda�ϵ�������Ч
	Systicks_Nus(5);//��ʱ

	SCL_LOW;//ͨ��û�н�����ǯλס��ƽ��ʹ�������߲��ܷ���iicͨ��	
}

void IIC_SendNoAck(void)
{
	SCL_LOW;//�������ݵ���Ч�ԣ�SCLΪ�͵�ƽʱ������Ч������Ҫ�ı�SDA�����������ʱ��Ϊ�˷�ֹ����ֹͣ�źźͿ�ʼ�źţ�����SDA��������Ч����ȥ����Ӧ�ĸı�
	IIC_SDA_OUT();//����Ӧ���ź�������Ϊ���շ�����ӻ����ͷ�Ӧ���źţ����ó����ģʽ

	SDA_HIGH;//�Ƚ�SDA��1
	SCL_HIGH;//ʹsda�ϵ�������Ч
	Systicks_Nus(5);//��ʱ

	SCL_LOW;//ͨ��û�н�����ǯλס��ƽ��ʹ�������߲��ܷ���iicͨ��		
}

uint8_t IIC_WaitACK(void)//�ȴ��ӻ�Ӧ��
{
	uint8_t count=0;//����һ����ʱ������
	SCL_LOW;//�������ݵ���Ч�ԣ�SCLΪ�͵�ƽʱ������Ч������Ҫ�ı�SDA�����������ʱ��Ϊ�˷�ֹ����ֹͣ�źźͿ�ʼ�źţ�����SDA��������Ч����ȥ����Ӧ�ĸı�
	IIC_SDA_IN();//����Ӧ���ź�������Ϊ���շ�����ӻ����ͷ�Ӧ���źţ����ó����ģʽ

	SCL_HIGH;//�Ƚ�SCL��1,ʹSDA�ϵ�������Ч
	Systicks_Nus(4);//��ʱ
	while(SDA_Read)//�ȴ��ӻ�����ACK
	{
		count++; //δ����Ӧ���źţ��������Ӽӣ�������ȡ
		if(count>250)//��ȡ�Ƿ�ʱ
		{
			IIC_Stop();//��ʱ,ֹͣIIC
			return NOACK;//���ط�Ӧ��
		}
	}
	SCL_LOW;//��ȡ��Ӧ���źţ�ǯλ��ƽ
	return ACK;//����Ӧ���ź�
}

void IIC_SendByteData(uint8_t DATA)//iic����һ���ֽ�����
{
	uint8_t i;//һ���ֽ�8��λ�����з�8�Σ�����һ��������i
	SCL_LOW;//�������ݵ���Ч�ԣ�SCLΪ�͵�ƽʱ������Ч������Ҫ�ı�SDA�����������ʱ��Ϊ�˷�ֹ����ֹͣ�źźͿ�ʼ�źţ�����SDA��������Ч����ȥ����Ӧ�ĸı�
	IIC_SDA_OUT();//����Ӧ���ź�������Ϊ���շ�����ӻ����ͷ�Ӧ���źţ����ó����ģʽ

	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ�η���һ��λ������
	{
		if(DATA&0x80)//IIC�Ǹ�λ���е��������ȷ���λ��
		{
			SDA_HIGH;//��λΪ1��sda��1
		}
		else
		{
			SDA_LOW;//��λΪ0��sda��0
		}
		SCL_HIGH;//ʹsda�ϵ�������Ч
		Systicks_Nus(5);//��ʱ
		SCL_LOW;//���ͣ�ʹsda�ϵ�������Ч����ȥ�ı�sda��ֵ
		Systicks_Nus(5);	//��ʱ
		DATA=DATA<<1;//���θ�λ�������λ���ٽ��з���
	}
	SCL_LOW;////ͨ��û�н�����ǯλס��ƽ��ʹ�������߲��ܷ���iicͨ��	
}

 uint8_t IIC_RxByteData(uint8_t ACKflag)//iic����һ���ֽڵ�����
 {
 	uint8_t i;//һ���ֽ�8��λ�����з�8�Σ�����һ��������i
	uint8_t data=0;//����һ���ڴ浥Ԫ��������Ž��յ�������
	SCL_LOW;//�������ݵ���Ч�ԣ�SCLΪ�͵�ƽʱ������Ч������Ҫ�ı�SDA�����������ʱ��Ϊ�˷�ֹ����ֹͣ�źźͿ�ʼ�źţ�����SDA��������Ч����ȥ����Ӧ�ĸı�
	IIC_SDA_IN();//����Ӧ���ź�������Ϊ���շ�����ӻ����ͷ�Ӧ���źţ����ó����ģʽ

	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ�ν���һ��λ������
	{
		data=data<<1;//�����ƣ���ֹ�������ݲ���ȷ
		SCL_HIGH;//ʹSCL�ϵ�������Ч
		Systicks_Nus(4);//��ʱ
		if(SDA_Read)//��ȡSDA���͹���������
		{
			data=data|0x01;//��������ߵ�ƽ������ȡ��ֵ���룬�ڴ浥Ԫ
		}
		SCL_LOW;//���ͣ�ʹsda�ϵ�������Ч
		Systicks_Nus(4);//��ʱ		
	}
	
	if(ACKflag==ACK)//����ack����noack
	{
		IIC_SendAck();//����ACK
	}
	else
	{
		IIC_SendNoAck();//����noack
	}
	SCL_LOW;////ͨ��û�н�����ǯλס��ƽ��ʹ�������߲��ܷ���iicͨ��	
	return data;//���ض�ȡ��������
 }
 

 
 
 