/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��arm_cm4.h
 * ����         ������ģ��ʵ��
 *
 * ʵ��ƽ̨     ��landzo���ӿ�����
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/

**********************************************************************************/

#ifndef _COMMON_H_    //��ֹ�ظ����壨Common_H  ��ͷ)
#define _COMMON_H_

//1��оƬ�Ĵ���ӳ���ļ����������ں������ļ�
#include "MKL25Z4.h"      // ����оƬͷ�ļ�

//2�����忪�����ж�



#define ARM_INTERRUPT_LEVEL_BITS   2       //MQX����ϵͳʹ��

//3��λ�����꺯������λ����λ����üĴ���һλ��״̬��
#define BSET(bit,Register)  ((Register)|= (1<<(bit)))    //�üĴ�����һλ
#define BCLR(bit,Register)  ((Register) &= ~(1<<(bit)))  //��Ĵ�����һλ
#define BGET(bit,Register)  (((Register) >> (bit)) & 1)  //��üĴ���һλ��״̬

//4���ض�������������ͣ����ͱ����궨�壩
typedef unsigned char        uint_8;   // �޷���8λ�����ֽ�
typedef unsigned short int   uint_16;  // �޷���16λ������
typedef unsigned long int    uint_32;  // �޷���32λ��������
typedef char                 int_8;    // �з���8λ��
typedef short int            int_16;   // �з���16λ��
typedef int                  int_32;   // �з���32λ��
//���Ż�����
typedef volatile uint_8      vuint_8;  // ���Ż��޷���8λ�����ֽ�
typedef volatile uint_16     vuint_16; // ���Ż��޷���16λ������
typedef volatile uint_32     vuint_32; // ���Ż��޷���32λ��������
typedef volatile int_8       vint_8;   // ���Ż��з���8λ��
typedef volatile int_16      vint_16;  // ���Ż��з���16λ��
typedef volatile int_32      vint_32;  // ���Ż��з���32λ��

typedef	union
{
	uint_32	DW;
	uint_16	W[2];
	uint_8	B[4];
	struct
	{
		uint_32 b0:1; uint_32 b1:1; uint_32 b2:1; uint_32 b3:1; uint_32 b4:1; uint_32 b5:1; uint_32 b6:1; uint_32 b7:1; 
		uint_32 b8:1; uint_32 b9:1; uint_32 b10:1;uint_32 b11:1;uint_32 b12:1;uint_32 b13:1;uint_32 b14:1;uint_32 b15:1;
		uint_32 b16:1;uint_32 b17:1;uint_32 b18:1;uint_32 b19:1;uint_32 b20:1;uint_32 b21:1;uint_32 b22:1;uint_32 b23:1;
		uint_32 b24:1;uint_32 b25:1;uint_32 b26:1;uint_32 b27:1;uint_32 b28:1;uint_32 b29:1;uint_32 b30:1;uint_32 b31:1;
	};
}Dtype;		//sizeof(Dtype)	Ϊ 4 
/*
 * �������е�RAM�ĺ���
 */
#if defined(__ICCARM__)
	#define 	__RAMFUN	__ramfunc	//IAR �� __ramfunc ������
#else
	#define 	__RAMFUN
#endif


//ι��
#define WDI_FEED() {SIM_SRVCOP = 0x55;	SIM_SRVCOP = 0xAA;}



// 7���������ͷ�ļ�
#include "vectors.h"
#include "io.h"
#include "freedom.h"
#include "arm_cm0.h"
#include "sysinit.h"
#include "startup.h"
#include  "sysinit.h"
#include "MCG.h"
#include "UART.h"
//#include "PIT.h"
#include "MKL_FTFA.h"
#include "ADC.h"

//8�������������
#include "Encoder.h"
#include "PredatorCamera.h"
#include "PredatorCameraSCCB.h"
#include "Motor.h"
#include "OLED.h"
#include "Switch_and_Led.h"

//9������Ӧ��ͷ�ļ�
#include "Arguments.h"
#include "Control.h"
#include "ImageProcess.h"
#include "OutputData.h"
#endif
