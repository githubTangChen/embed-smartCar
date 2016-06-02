/*****************************************
文 件 名：Encoder.c
描    述：编码器底层驱动
修改日期：2016-3-5
*****************************************/
#include "common.h"

volatile uint8_t Pulse_INT_Count= 0;
/********************
编码器引脚初始化
对应PTC5
********************/
void EncoderInit()
{
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK; //使能LPT模块时钟
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK << 2; //开启PORTx端口
  // 复用功能 , 确定触发模式 ,开启上拉或下拉电阻
  PORT_PCR_REG(PORTC_BASE_PTR,5) = 0x03 << PORT_PCR_MUX_SHIFT;
  
   // 清状态寄存器
  LPTMR0_CSR = 0x00 ;                                          //先关了LPT，这样才能设置时钟分频,清空计数值等
  LPTMR0_PSR = 0x00 ;
  LPTMR0_CSR = 0x00 ;
  LPTMR0_CMR = 0X00 ;
  
  // 设置累加计数值 ,设置比较值
  LPTMR0_CMR  =  LPTMR_CMR_COMPARE(MaxPulse); 
  
 
  LPTMR0_PSR  =  (0
                  |LPTMR_PSR_PCS(0x00)
                  |LPTMR_PSR_PBYP_MASK
                  |LPTMR_PSR_PRESCALE(0x01)); 
  // 管脚设置、使能中断
  LPTMR0_CSR  =  (0
                  | LPTMR_CSR_TPS(2)// 选择输入管脚 选择
                  | LPTMR_CSR_TMS_MASK // 选择脉冲计数 (注释了表示时间计数模式)
                  | LPTMR_CSR_TPP_MASK //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                  | LPTMR_CSR_TEN_MASK //使能LPT(注释了表示禁用)
                  | LPTMR_CSR_TIE_MASK //中断使能
                  | LPTMR_CSR_TFC_MASK //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                 );
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK ; 
  
  //开引脚的IRQ中断
  enable_irq(28);
  
  //初始脉冲值清零
  LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
}


/*************************************************************************
*  获得脉冲计数值并清除计数值
*  参数说明：无
*  函数返回：脉冲计数值
*  备    注：读取LPTMR0_CNR数据，必须先写入，才可读读取
*************************************************************************/
uint16_t LptmrPulseGet()
{
  uint16_t  LPTCOUNT = 0  ;
  LPTMR0_CNR = 12 ;
  LPTCOUNT = LPTMR0_CNR ;
  LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;//手动清除计数器值
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
  return (uint16_t)LPTCOUNT;
}


/*!
*  @brief      LPTMR中断服务函数
*  当计数脉冲达到 MaxPulse 时进入此中断
*/
void PulaseOverflow_Handler()
{
  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;//清除标志位
  //禁用LPT的时候就会自动清计数器的值
  LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;//手动清除计数器值
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
  Pulse_INT_Count ++ ;//中断次数加一
}