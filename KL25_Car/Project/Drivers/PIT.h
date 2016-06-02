/*****************************************
文 件 名：PIT.h
描    述：定时器驱动
作    者：landzo 蓝电子
*****************************************/
#ifndef _PIT_H_
#define _PIT_H_


//必须在"vectors.h"包含的后面！！！
//中断重定向
#undef  VECTOR_038
#define VECTOR_038    PIT_IRQHandler


//PIT0定时器初始化
//每cnt/1000 ms触发一次中断
void PIT_Init(uint32_t cnt);
//定时器中断服务函数
void PIT_IRQHandler(void);

#endif