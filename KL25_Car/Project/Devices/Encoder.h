/*****************************************
�� �� ����Encoder.h
��    �����������ײ�����
�޸����ڣ�2016-3-5
*****************************************/
#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "stdint.h"
//������"vectors.h"�����ĺ��棡����
//�ж��ض���
#undef  VECTOR_044
#define VECTOR_044    PulaseOverflow_Handler

//�������ֵ�������򴥷��ж�
#define MaxPulse 65535 //0xFFFF
#define OneMeterPulse 2950.0//һ��������
extern volatile uint8_t Pulse_INT_Count;
void EncoderInit(void);
uint16_t LptmrPulseGet(void);
//�˺�����vector.h ��Ӧ VECTOR_044
void PulaseOverflow_Handler(void);
#endif