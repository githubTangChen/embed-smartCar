/*****************************************/
//文 件 名：ADC.h
//描    述：ADC模块
/*****************************************/
#ifndef _ADC_H_
#define _ADC_H_
//PTE20
void ADC_Init(void);//ADC0_CH0初始化
uint_16 ADC_HardwareAverage(char HardwareAverageNTimes);//硬件平均N次
#endif