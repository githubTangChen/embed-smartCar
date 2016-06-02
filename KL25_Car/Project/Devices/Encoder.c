/*****************************************
�� �� ����Encoder.c
��    �����������ײ�����
�޸����ڣ�2016-3-5
*****************************************/
#include "common.h"

volatile uint8_t Pulse_INT_Count= 0;
/********************
���������ų�ʼ��
��ӦPTC5
********************/
void EncoderInit()
{
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK; //ʹ��LPTģ��ʱ��
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK << 2; //����PORTx�˿�
  // ���ù��� , ȷ������ģʽ ,������������������
  PORT_PCR_REG(PORTC_BASE_PTR,5) = 0x03 << PORT_PCR_MUX_SHIFT;
  
   // ��״̬�Ĵ���
  LPTMR0_CSR = 0x00 ;                                          //�ȹ���LPT��������������ʱ�ӷ�Ƶ,��ռ���ֵ��
  LPTMR0_PSR = 0x00 ;
  LPTMR0_CSR = 0x00 ;
  LPTMR0_CMR = 0X00 ;
  
  // �����ۼӼ���ֵ ,���ñȽ�ֵ
  LPTMR0_CMR  =  LPTMR_CMR_COMPARE(MaxPulse); 
  
 
  LPTMR0_PSR  =  (0
                  |LPTMR_PSR_PCS(0x00)
                  |LPTMR_PSR_PBYP_MASK
                  |LPTMR_PSR_PRESCALE(0x01)); 
  // �ܽ����á�ʹ���ж�
  LPTMR0_CSR  =  (0
                  | LPTMR_CSR_TPS(2)// ѡ������ܽ� ѡ��
                  | LPTMR_CSR_TMS_MASK // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                  | LPTMR_CSR_TPP_MASK //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                  | LPTMR_CSR_TEN_MASK //ʹ��LPT(ע���˱�ʾ����)
                  | LPTMR_CSR_TIE_MASK //�ж�ʹ��
                  | LPTMR_CSR_TFC_MASK //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                 );
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK ; 
  
  //�����ŵ�IRQ�ж�
  enable_irq(28);
  
  //��ʼ����ֵ����
  LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
}


/*************************************************************************
*  ����������ֵ���������ֵ
*  ����˵������
*  �������أ��������ֵ
*  ��    ע����ȡLPTMR0_CNR���ݣ�������д�룬�ſɶ���ȡ
*************************************************************************/
uint16_t LptmrPulseGet()
{
  uint16_t  LPTCOUNT = 0  ;
  LPTMR0_CNR = 12 ;
  LPTCOUNT = LPTMR0_CNR ;
  LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;//�ֶ����������ֵ
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
  return (uint16_t)LPTCOUNT;
}


/*!
*  @brief      LPTMR�жϷ�����
*  ����������ﵽ MaxPulse ʱ������ж�
*/
void PulaseOverflow_Handler()
{
  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;//�����־λ
  //����LPT��ʱ��ͻ��Զ����������ֵ
  LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;//�ֶ����������ֵ
  LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
  Pulse_INT_Count ++ ;//�жϴ�����һ
}