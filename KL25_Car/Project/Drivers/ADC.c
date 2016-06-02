/*****************************************/
//文 件 名：ADC.c
//描    述：ADC模块
/*****************************************/
#include "common.h"
#include "ADC.h"

//PTE20
//ADC0_CH0初始化
void ADC_Init(void)
{
	SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //开启ADC0时钟
	SIM_SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
	SIM_SOPT7 |= SIM_SOPT7_ADC0TRGSEL(0);
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << 4); 
	//PTE20
	PORT_PCR_REG(PORTE_BASE_PTR, 20) = 0x00 << PORT_PCR_MUX_SHIFT;
}
uint_16 ADC_HardwareAverage(char HardwareAverageNTimes)
{
  uint8_t ADCCfg = 0;
  uint_16 ADCResult = 0;
  //0   1   2   3
  //8  12  10  16
  ADCCfg = ADC_CFG1_MODE(1);//精度12位
  ADC0_SC3 = 0 | ADC_SC3_AVGE_MASK | ADC_SC3_AVGS((uint8_t)HardwareAverageNTimes);   //SC3寄存器硬件触发

  ADCCfg |=  (ADC_CFG1_ADIV(2) | ADC_CFG1_ADICLK(1));
  
  ADC0_CFG1 = ADCCfg;
  
  ADC0_CFG2 = 0 | ADC_CFG2_ADHSC_MASK;
  
  ADC0_SC1A = ADC_SC1_ADCH(0);//CH0
  
  while((ADC0_SC1A & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK)
  {
  }
  
  ADCResult = (uint_16)ADC0_RA;
  
  ADC0_SC1A &= ~ADC_SC1_COCO_MASK;
  
  return ADCResult;
}