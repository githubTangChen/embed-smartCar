//============================================================================
//文件名称：uart.h
//功能概要：UART底层驱动构件头文件
//版权所有：苏州大学飞思卡尔嵌入式中心(sumcu.suda.edu.cn)
//更新记录：2012-11-12   V1.0
//============================================================================
#ifndef UART_H         //防止重复定义（ 开头)
#define UART_H

#include "common.h"   //包含公共要素头文件
#include "sysinit.h"


//必须在"vectors.h"包含的后面！！！
//中断重定向
#undef  VECTOR_028
#define VECTOR_028   UART0_IRQHandler


#define USART_REC_LEN 40 //最大保存接收字符数

extern uint8	  	SendImage;
extern uint8	  	SendFlag;
extern uint8	  	SComplishFlag;
extern uint8	  	DComplishFlag;
extern uint8	  	ZComplishFlag;
//extern uint8	  	AComplishFlag;
extern uint8	  	SStartFlag;
extern uint8	  	DStartFlag;
extern uint8	  	ZStartFlag;
//extern uint8	  	AStartFlag;
extern uint8  USART_RX_BUF[USART_REC_LEN];
extern uint16 USART_RX_CNT; //接收字符计数      




//UART0_RX  PTB16 /UART0_TX  PTB17
void UART_Init (uint32_t baud_rate);//串口初始化

char uart_getchar (void);//无限等待接受1个字节

void uart_putchar (char ch);                    //发送1个字节

void uart_irq_EN(void);                               //开串口接收中断
void uart_irq_DIS(void);                               //关串口接收中断 

void UART0_IRQHandler(void);//串口接收中断

void JudgeReceive(void);
void Set_PID(float * P,float * I,float * D);


#endif     //防止重复定义（ 结尾)
