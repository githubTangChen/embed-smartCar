//============================================================================
//�ļ����ƣ�uart.h
//���ܸ�Ҫ��UART�ײ���������ͷ�ļ�
//��Ȩ���У����ݴ�ѧ��˼����Ƕ��ʽ����(sumcu.suda.edu.cn)
//���¼�¼��2012-11-12   V1.0
//============================================================================
#ifndef UART_H         //��ֹ�ظ����壨 ��ͷ)
#define UART_H

#include "common.h"   //��������Ҫ��ͷ�ļ�
#include "sysinit.h"


//������"vectors.h"�����ĺ��棡����
//�ж��ض���
#undef  VECTOR_028
#define VECTOR_028   UART0_IRQHandler


#define USART_REC_LEN 40 //��󱣴�����ַ���

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
extern uint16 USART_RX_CNT; //�����ַ�����      




//UART0_RX  PTB16 /UART0_TX  PTB17
void UART_Init (uint32_t baud_rate);//���ڳ�ʼ��

char uart_getchar (void);//���޵ȴ�����1���ֽ�

void uart_putchar (char ch);                    //����1���ֽ�

void uart_irq_EN(void);                               //�����ڽ����ж�
void uart_irq_DIS(void);                               //�ش��ڽ����ж� 

void UART0_IRQHandler(void);//���ڽ����ж�

void JudgeReceive(void);
void Set_PID(float * P,float * I,float * D);


#endif     //��ֹ�ظ����壨 ��β)
