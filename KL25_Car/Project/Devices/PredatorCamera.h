/*****************************************
文 件 名：PredatorCamera.h
描    述：摄像头底层驱动
修改日期：2016-3-11
*****************************************/
#ifndef _PREDATORCAMERA_H_
#define _PREDATORCAMERA_H_
#include "MKL25Z4.h"
#include "arm_cm0.h"
//必须在"vectors.h"包含的后面！！！
//中断重定向
#undef  VECTOR_016
#define VECTOR_016    DMA_CH0_Handler

//必须在"vectors.h"包含的后面！！！
//中断重定向
//场与行中断服务函数
#undef  VECTOR_046
#define VECTOR_046    PORTA_IRQHandler

//高度建议不修改
#define PICTURE_HIGHT 60
//如果改宽度，OLED里定义的Width也要改
//宽度不要太长，采集70点PLL设置为80
#define PICTURE_WIDTH 70

//允许DMA通道传输完成中断
#define  DMA_IRQ_EN(DMA_CHn)    do{DMA_DCR(DMA_CHn)  |= DMA_DCR_EINT_MASK;enable_irq(0);}while(0)

//禁止DMA通道传输完成中断
#define  DMA_IRQ_DIS(DMA_CHn)   do{DMA_DCR(DMA_CHn)  &= ~DMA_DCR_EINT_MASK;enable_irq(0);}while(0)

//清除通道传输中断标志位
#define  DMA_IRQ_CLEAN(DMA_CHn) DMA_DSR_BCR(DMA_CHn)|= DMA_DSR_BCR_DONE_MASK

//使能通道硬件DMA请求
#define  DMA_EN(DMA_CHn)        DMAMUX0_CHCFG(DMA_CHn) |= DMAMUX_CHCFG_ENBL_MASK

//禁止通道硬件DMA请求
#define  DMA_DIS(DMA_CHn)       DMAMUX0_CHCFG(DMA_CHn) &= ~DMAMUX_CHCFG_ENBL_MASK

extern unsigned char ThresholdValue;//二值化阀值
extern unsigned char PixData[PICTURE_HIGHT][PICTURE_WIDTH];//图像数据
extern unsigned int V_Cnt;//行中断触发计数变量
extern unsigned char H_Cnt;//实际采集行计数变量
extern unsigned char CompleteFlag; //图像采集完成标志
extern unsigned char BinCompleteFlag;//行二值化完成

//关闭图像采集
inline void CameraOFF(void)
{
     //gpio_Interrupt_init(PTA17, GPI_DOWN, GPI_DISAB) ;       //行中断 
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 | PORT_PCR_MUX(1) | 0x02|PORT_PCR_IRQC(0x0) );
    //GPIO_PDDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));
    PTA_BASE_PTR->PDDR &= ~(1 << 17);
    //gpio_Interrupt_init(PTA1, GPI_UP,GPI_DISAB) ; 
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0X0) );
    PTA_BASE_PTR->PDDR &= ~(1 << 0);
    disable_irq(30);
}
//开启图像采集
inline void CameraON(void)
{
  V_Cnt = 0;
  H_Cnt = 0;
  CompleteFlag = 0;
    /********************
      打开场中断，在场中断中打开行中断
      ********************/
  //场信号输入口 PTA1
  PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0X0A) );
  //设置端口方向为输入
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 1); 
  PORTA_ISFR  |= 0x00000002; //写1清中断标志位    
  DMA_DAR(0) = (uint32_t)PixData;//重载目的地址
  //PortA_irq_no 30
  enable_irq(30);//打开相应的中断来采集图像
}

void dlay(void);//延时函数
void Camera_Init(void);//摄像头控制初始化函数
void DMA_Init(void);//DMA初始化   static inline
void GPIO_Init(void);//IO口初始化
void ImageSend(void);//使用串口发送图像到上位机
void OLEDPrintPicture(void);//在OLED上显示图片
void DMA_CH0_Handler(void);//DMA完成中断
void PORTA_IRQHandler(void);//场与行中断服务函数
#endif