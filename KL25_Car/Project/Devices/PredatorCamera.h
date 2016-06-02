/*****************************************
�� �� ����PredatorCamera.h
��    ��������ͷ�ײ�����
�޸����ڣ�2016-3-11
*****************************************/
#ifndef _PREDATORCAMERA_H_
#define _PREDATORCAMERA_H_
#include "MKL25Z4.h"
#include "arm_cm0.h"
//������"vectors.h"�����ĺ��棡����
//�ж��ض���
#undef  VECTOR_016
#define VECTOR_016    DMA_CH0_Handler

//������"vectors.h"�����ĺ��棡����
//�ж��ض���
//�������жϷ�����
#undef  VECTOR_046
#define VECTOR_046    PORTA_IRQHandler

//�߶Ƚ��鲻�޸�
#define PICTURE_HIGHT 60
//����Ŀ�ȣ�OLED�ﶨ���WidthҲҪ��
//��Ȳ�Ҫ̫�����ɼ�70��PLL����Ϊ80
#define PICTURE_WIDTH 70

//����DMAͨ����������ж�
#define  DMA_IRQ_EN(DMA_CHn)    do{DMA_DCR(DMA_CHn)  |= DMA_DCR_EINT_MASK;enable_irq(0);}while(0)

//��ֹDMAͨ����������ж�
#define  DMA_IRQ_DIS(DMA_CHn)   do{DMA_DCR(DMA_CHn)  &= ~DMA_DCR_EINT_MASK;enable_irq(0);}while(0)

//���ͨ�������жϱ�־λ
#define  DMA_IRQ_CLEAN(DMA_CHn) DMA_DSR_BCR(DMA_CHn)|= DMA_DSR_BCR_DONE_MASK

//ʹ��ͨ��Ӳ��DMA����
#define  DMA_EN(DMA_CHn)        DMAMUX0_CHCFG(DMA_CHn) |= DMAMUX_CHCFG_ENBL_MASK

//��ֹͨ��Ӳ��DMA����
#define  DMA_DIS(DMA_CHn)       DMAMUX0_CHCFG(DMA_CHn) &= ~DMAMUX_CHCFG_ENBL_MASK

extern unsigned char ThresholdValue;//��ֵ����ֵ
extern unsigned char PixData[PICTURE_HIGHT][PICTURE_WIDTH];//ͼ������
extern unsigned int V_Cnt;//���жϴ�����������
extern unsigned char H_Cnt;//ʵ�ʲɼ��м�������
extern unsigned char CompleteFlag; //ͼ��ɼ���ɱ�־
extern unsigned char BinCompleteFlag;//�ж�ֵ�����

//�ر�ͼ��ɼ�
inline void CameraOFF(void)
{
     //gpio_Interrupt_init(PTA17, GPI_DOWN, GPI_DISAB) ;       //���ж� 
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 | PORT_PCR_MUX(1) | 0x02|PORT_PCR_IRQC(0x0) );
    //GPIO_PDDR_REG(GPIOX_BASE(ptxn)) &= ~(1 << PTn(ptxn));
    PTA_BASE_PTR->PDDR &= ~(1 << 17);
    //gpio_Interrupt_init(PTA1, GPI_UP,GPI_DISAB) ; 
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0X0) );
    PTA_BASE_PTR->PDDR &= ~(1 << 0);
    disable_irq(30);
}
//����ͼ��ɼ�
inline void CameraON(void)
{
  V_Cnt = 0;
  H_Cnt = 0;
  CompleteFlag = 0;
    /********************
      �򿪳��жϣ��ڳ��ж��д����ж�
      ********************/
  //���ź������ PTA1
  PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0X0A) );
  //���ö˿ڷ���Ϊ����
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 1); 
  PORTA_ISFR  |= 0x00000002; //д1���жϱ�־λ    
  DMA_DAR(0) = (uint32_t)PixData;//����Ŀ�ĵ�ַ
  //PortA_irq_no 30
  enable_irq(30);//����Ӧ���ж����ɼ�ͼ��
}

void dlay(void);//��ʱ����
void Camera_Init(void);//����ͷ���Ƴ�ʼ������
void DMA_Init(void);//DMA��ʼ��   static inline
void GPIO_Init(void);//IO�ڳ�ʼ��
void ImageSend(void);//ʹ�ô��ڷ���ͼ����λ��
void OLEDPrintPicture(void);//��OLED����ʾͼƬ
void DMA_CH0_Handler(void);//DMA����ж�
void PORTA_IRQHandler(void);//�������жϷ�����
#endif