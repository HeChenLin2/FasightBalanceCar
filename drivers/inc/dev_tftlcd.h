#ifndef		_DEV_TFTLCD_H_
#define		_DEV_TFTLCD_H_

#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "dev_led.h"
#include "dev_systickstimer.h"
#include "dev_usart1.h"
#include "stdlib.h"
#include "sys.h"	
#include "data.h"


/*-------------------------LCD 模块引脚说明-------------------------------------

--------------------------16位并口总线------------------------------------------
低8位
D0-----------PD14			D1-----------PD15
D2-----------PD0			D3-----------PD1
D4-----------PE7			D5-----------PE8
D6-----------PE9			D7-----------PE10
高8位
D8-----------PE11			D9-----------PE12
D10-----------PE13			D11-----------PE14
D12-----------PE15			D13-----------PD8
D14-----------PD9			D15-----------PD10

------------------------液晶屏控制接线---------------------------------------
WR------------PD5----->液晶屏写数据控制信号                   
RD------------PD4----->液晶屏读数据控制信号
RS------------PF12---->液晶屏数据/命令控制信号
RST----------->复位
CS------------PG12----->液晶屏片选控制信号
BL------------PB15----->液晶屏背光控制信号

------------------------触摸屏控制接线----------------------------------------
T_PEN-----------  PB1------------>触摸屏触摸中断控制信号
T_MISO------------PB2------------>总线读
T_MOSI------------PF11------------>总线写
T_CS--------------PC13------------>片选控制信号
CLK---------------PB0------------>时钟信号
*/

//---------------------------------------------------------------------


#define LED  15        //背光端口            PB15
#define	LCD_LED 			PBout(LED)    

typedef struct
{
	u16 LCD_CMD;
	u16 LCD_DATA;		
} LCD_TypeDef;


typedef struct		//LCD重要参数集  
{										    
	u16 width;			//LCD宽度
	u16 height;			//高度
	u16 id;				//ID
	u8  dir;			//横屏还是竖屏，1-横，0-竖屏 
}_tftlcd_data; 

#define TFTLCD_DIR	0				//0竖屏，1横屏

extern _tftlcd_data tftlcd_data;


#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define TFTLCD            ((LCD_TypeDef *) LCD_BASE)

//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 				0XFFE0
#define GBLUE				0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 		0XBC40
#define BRRED 		0XFC07 
#define GRAY  		0X8430 


//---------------------------------------------------------------------
void LCD_WriteCmdData(u16 cmd,u16 data);						//写入
void LCD_WriteData_Color(u16 color);								//颜色写入

u16 LCD_ReadData(void);															//读数据

void LCD_Display_Dir(u8 dir);												//LCD屏的显示方向

void LCD_GPIO_INIT(void);														//引脚初始化
void LCD_Init(void);																//初始化

void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);							//设置窗口
void LCD_Clear(u16 color);																						//清屏

void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color);      //指定区域内填充指定颜色
void LCD_DrawPoint(u16 x,u16 y);																				//画点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);											//画线
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn);
void LCD_DrawFRONT_COLOR(u16 x,u16 y,u16 color);
void showpic(int x,int y,int wight,int hight);








#endif
