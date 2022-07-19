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


/*-------------------------LCD ģ������˵��-------------------------------------

--------------------------16λ��������------------------------------------------
��8λ
D0-----------PD14			D1-----------PD15
D2-----------PD0			D3-----------PD1
D4-----------PE7			D5-----------PE8
D6-----------PE9			D7-----------PE10
��8λ
D8-----------PE11			D9-----------PE12
D10-----------PE13			D11-----------PE14
D12-----------PE15			D13-----------PD8
D14-----------PD9			D15-----------PD10

------------------------Һ�������ƽ���---------------------------------------
WR------------PD5----->Һ����д���ݿ����ź�                   
RD------------PD4----->Һ���������ݿ����ź�
RS------------PF12---->Һ��������/��������ź�
RST----------->��λ
CS------------PG12----->Һ����Ƭѡ�����ź�
BL------------PB15----->Һ������������ź�

------------------------���������ƽ���----------------------------------------
T_PEN-----------  PB1------------>�����������жϿ����ź�
T_MISO------------PB2------------>���߶�
T_MOSI------------PF11------------>����д
T_CS--------------PC13------------>Ƭѡ�����ź�
CLK---------------PB0------------>ʱ���ź�
*/

//---------------------------------------------------------------------


#define LED  15        //����˿�            PB15
#define	LCD_LED 			PBout(LED)    

typedef struct
{
	u16 LCD_CMD;
	u16 LCD_DATA;		
} LCD_TypeDef;


typedef struct		//LCD��Ҫ������  
{										    
	u16 width;			//LCD���
	u16 height;			//�߶�
	u16 id;				//ID
	u8  dir;			//��������������1-�ᣬ0-���� 
}_tftlcd_data; 

#define TFTLCD_DIR	0				//0������1����

extern _tftlcd_data tftlcd_data;


#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define TFTLCD            ((LCD_TypeDef *) LCD_BASE)

//������ɫ
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
void LCD_WriteCmdData(u16 cmd,u16 data);						//д��
void LCD_WriteData_Color(u16 color);								//��ɫд��

u16 LCD_ReadData(void);															//������

void LCD_Display_Dir(u8 dir);												//LCD������ʾ����

void LCD_GPIO_INIT(void);														//���ų�ʼ��
void LCD_Init(void);																//��ʼ��

void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);							//���ô���
void LCD_Clear(u16 color);																						//����

void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color);      //ָ�����������ָ����ɫ
void LCD_DrawPoint(u16 x,u16 y);																				//����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);											//����
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn);
void LCD_DrawFRONT_COLOR(u16 x,u16 y,u16 color);
void showpic(int x,int y,int wight,int hight);








#endif
