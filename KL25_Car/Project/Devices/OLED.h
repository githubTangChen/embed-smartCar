/*****************************************
文 件 名：OLED.h
描    述：OLED底层驱动
修改日期：2016-3-11
*****************************************/
#ifndef _OELD_H_
#define _OELD_H_

#include "stdint.h"
#include "MKL25Z4.h"
#define uint8 unsigned char
#define uint  unsigned int
/********************************************************************/
/*-----------------------------------------------------------------------
LCD_init          : OLED初始化
-----------------------------------------------------------------------*/
 void OLED_Init(void);
 void OLED_CLS(void);
 void OLED_Set_Pos(uint8_t x, uint8_t y);
 void OLED_WrDat(uint8_t data);
 void Dly_ms(uint16_t ms);
 void OLED_PrintValueInt(uint8 x, uint8 y, signed  int data, uint8 num);
 void OLED_PrintValueFP(uint8 x, uint8 y, unsigned  int data, uint8 num);
 void OLED_PrintValueF(uint8 x, uint8 y, float data, uint8 num);
 void OLED_P6x8Char(uint8 x,uint8 y,uint8 ch); 
 void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_PrintPicture(void);
/******************************************************************/

#endif
