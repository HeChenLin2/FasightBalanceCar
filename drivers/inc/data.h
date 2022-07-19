#ifndef _DATA_H_
#define _DATA_H_

#include "stm32f4xx.h"


struct  Cn32CharTypeDef                   // 汉字字模数据结构 
{
	unsigned char  Index[2];            // 汉字内码索引,一个汉字占两个字节	
	unsigned char   Msk[116];            // 点阵码数据(32*29/8) 
};

extern struct  Cn32CharTypeDef const CnChar32x29[];
extern const unsigned char gImage_panda[153600];

#endif
