/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：arm_cm4.h
 * 描述         ：工程模版实验
 *
 * 实验平台     ：landzo电子开发版
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/

**********************************************************************************/

#ifndef _COMMON_H_    //防止重复定义（Common_H  开头)
#define _COMMON_H_

//1．芯片寄存器映射文件及处理器内核属性文件
#include "MKL25Z4.h"      // 包含芯片头文件

//2．定义开关总中断



#define ARM_INTERRUPT_LEVEL_BITS   2       //MQX操作系统使用

//3．位操作宏函数（置位、清位、获得寄存器一位的状态）
#define BSET(bit,Register)  ((Register)|= (1<<(bit)))    //置寄存器的一位
#define BCLR(bit,Register)  ((Register) &= ~(1<<(bit)))  //清寄存器的一位
#define BGET(bit,Register)  (((Register) >> (bit)) & 1)  //获得寄存器一位的状态

//4．重定义基本数据类型（类型别名宏定义）
typedef unsigned char        uint_8;   // 无符号8位数，字节
typedef unsigned short int   uint_16;  // 无符号16位数，字
typedef unsigned long int    uint_32;  // 无符号32位数，长字
typedef char                 int_8;    // 有符号8位数
typedef short int            int_16;   // 有符号16位数
typedef int                  int_32;   // 有符号32位数
//不优化类型
typedef volatile uint_8      vuint_8;  // 不优化无符号8位数，字节
typedef volatile uint_16     vuint_16; // 不优化无符号16位数，字
typedef volatile uint_32     vuint_32; // 不优化无符号32位数，长字
typedef volatile int_8       vint_8;   // 不优化有符号8位数
typedef volatile int_16      vint_16;  // 不优化有符号16位数
typedef volatile int_32      vint_32;  // 不优化有符号32位数

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
}Dtype;		//sizeof(Dtype)	为 4 
/*
 * 定义运行到RAM的函数
 */
#if defined(__ICCARM__)
	#define 	__RAMFUN	__ramfunc	//IAR 用 __ramfunc 来声明
#else
	#define 	__RAMFUN
#endif


//喂狗
#define WDI_FEED() {SIM_SRVCOP = 0x55;	SIM_SRVCOP = 0xAA;}



// 7、包含相关头文件
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

//8、包含相关外设
#include "Encoder.h"
#include "PredatorCamera.h"
#include "PredatorCameraSCCB.h"
#include "Motor.h"
#include "OLED.h"
#include "Switch_and_Led.h"

//9、包含应用头文件
#include "Arguments.h"
#include "Control.h"
#include "ImageProcess.h"
#include "OutputData.h"
#endif
