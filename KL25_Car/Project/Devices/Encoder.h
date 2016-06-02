/*****************************************
文 件 名：Encoder.h
描    述：编码器底层驱动
修改日期：2016-3-5
*****************************************/
#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "stdint.h"
//必须在"vectors.h"包含的后面！！！
//中断重定向
#undef  VECTOR_044
#define VECTOR_044    PulaseOverflow_Handler

//脉冲溢出值，超过则触发中断
#define MaxPulse 65535 //0xFFFF
#define OneMeterPulse 2950.0//一米脉冲数
extern volatile uint8_t Pulse_INT_Count;
void EncoderInit(void);
uint16_t LptmrPulseGet(void);
//此函数在vector.h 对应 VECTOR_044
void PulaseOverflow_Handler(void);
#endif