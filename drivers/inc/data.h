#ifndef _DATA_H_
#define _DATA_H_

#include "stm32f4xx.h"


struct  Cn32CharTypeDef                   // ������ģ���ݽṹ 
{
	unsigned char  Index[2];            // ������������,һ������ռ�����ֽ�	
	unsigned char   Msk[116];            // ����������(32*29/8) 
};

extern struct  Cn32CharTypeDef const CnChar32x29[];
extern const unsigned char gImage_panda[153600];

#endif
