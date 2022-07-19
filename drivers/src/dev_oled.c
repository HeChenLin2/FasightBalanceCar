#include "dev_oled.h"
#include "oledfont.h" 
#include "inv_mpu_user.h"

/***********************************************************************
*函数名:OLED_WR_Byte()
*功能:oled写入一个字节
*形参:(u8 dat):数据/(u8 cmd):命令
*返回值:无
***********************************************************************/
static void OLED_WR_Byte(u8 dat,u8 cmd)
{
	u8 i;
	OLED_CS=0;						//拉低片选	
	if(cmd)								//数据
		OLED_RS=1;
	else									//命令
		OLED_RS=0;
	for(i=0;i<8;i++){
		OLED_SCLK=0;				//拉低时钟线
		if(dat&0x80)				//根据数据，调整数据线电平
			OLED_SDIN=1;
		else
			OLED_SDIN=0;
		
		OLED_SCLK=1;				//拉高时钟线，形成上升沿
		dat<<=1;						//数据左移
	}
	OLED_RS=1;						
	OLED_CS=1;						//拉高片选
}



/***********************************************************************
*函数名:oled_GPIO_Init()
*功能:初始化OLED所需的GPIO 
*形参:无
*返回值:无
***********************************************************************/
void oled_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(OLED_RCC_CLOCK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode             =GPIO_Mode_OUT;     
	GPIO_InitStruct.GPIO_OType            =GPIO_OType_PP;      
	GPIO_InitStruct.GPIO_Pin              =OLED_SCLK_PIN;
	GPIO_InitStruct.GPIO_Speed						=GPIO_Speed_100MHz;
	GPIO_Init(OLED_SCLK_TYBE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =OLED_SDIN_PIN;
	GPIO_Init(OLED_SDIN_TYBE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =OLED_RS_PIN;
	GPIO_Init(OLED_RS_TYBE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =OLED_RST_PIN;
	GPIO_Init(OLED_RST_TYBE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin              =OLED_CS_PIN;
	GPIO_Init(OLED_CS_TYBE, &GPIO_InitStruct);
	
	OLED_RST=0;
	OLED_DELAY_MS(100);
	OLED_RST=1;
}


/***********************************************************************
*函数名:oled_Mode_Init()
*功能:初始化SSD1306，详细设置请查阅相关手册
*形参:无
*返回值:无
***********************************************************************/
void oled_Mode_Init(void)
{
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0???? 0xa1??
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0???? 0xc8??
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear(0);
	OLED_Set_Pos(0,0); 	
}


/*****************************************************************
*函数名:OLED_Clear()
*功能:清屏函数
*形参:mode,0:清屏全黑，1:清屏全亮
*返回值:无
******************************************************************/
void OLED_Clear(uint8_t mode)  
{  
	u8 i,n,temp = 0xff;
	if(!mode)
		temp = 0;
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    
		OLED_WR_Byte (XLevelL,OLED_CMD);     
		OLED_WR_Byte (XLevelH,OLED_CMD);      
		for(n=0;n<128;n++)OLED_WR_Byte(temp,OLED_DATA); 
	} 
}

/**************************************************************************************************************
*函数名:OLED_Set_Pos()
*功能:设置显示坐标
*形参:(unsigned char x, unsigned char y):坐标值
*返回值:无
**************************************************************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}  

/*************************************************************************************************************
*函数名:oled_Init()
*功能:初始化OLED对应的GPIO和相应的SSD1306驱动芯片
*形参:无
*返回值:无
*************************************************************************************************************/
void oled_Init(void)
{
	oled_GPIO_Init();
	oled_Mode_Init();
	OLED_ShowString(0,0,(const char *)"Farsight", 8);
}

/**************************************************************************************************************
*函数名:OLED_ShowChar()
*功能:显示一个字符
*形参:(u8 x,u8 y):的位置[x:0~128,y:0~7]/(u8 chr):要显示的字符/(u8 size):字符大小
*返回值：无
**************************************************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';	
		if(x>Max_Column-1){x=0;y=y+2;}
		if(size ==8)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
					OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}
}


/**************************************************************************************************************
*函数名:OLED_ShowString()
*功能:显示一个字符串
*形参:(u8 x,u8 y):要显示的位置/(const char *chr):要显示的字符串/(u8 size):字体大小
*返回值:无
**************************************************************************************************************/
void OLED_ShowString(u8 x,u8 y,const char *chr, u8 size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],size);
		x+=size;
		if(x>128) return;
			//{x=0;y+=2;}
			j++;
	}
}


/**************************************************************************************************************
*函数名:OLED_USAER_SHOW()
*功能:用户的显示逻辑
*形参:无
*返回值:无
**************************************************************************************************************/
void OLED_USAER_SHOW(void)
{
	uint8_t buf[50];
	sprintf((char *)buf,"roll: %.2f  ",myDmpData.roll);
	OLED_ShowString(0,2,(const char *)buf, 6);
	
	sprintf((char *)buf,"x: %d  ",gyrodata.x);
	OLED_ShowString(0,3,(const char *)buf, 6);
	
	sprintf((char *)buf,"y: %d  ",gyrodata.y);
	OLED_ShowString(0,4,(const char *)buf, 6);
	
	sprintf((char *)buf,"z: %d  ",gyrodata.z);
	OLED_ShowString(0,5,(const char *)buf, 6);
}





