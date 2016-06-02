/*****************************************
�� �� ����PIT.c
��    ������ʱ������
��    �ߣ�landzo ������
*****************************************/
#include "common.h"
#include "PIT.h"


extern uint8_t TIMEflag_5ms   ;
extern uint8_t TIMEflag_10ms  ;
extern uint8_t TIMEflag_20ms  ;
extern uint8_t TIMEflag_30ms  ;
extern uint8_t TIMEflag_500ms ;
extern uint8_t TIMEflag_300ms ;

uint16_t  TimeCount = 0 ;

//PIT0��ʱ����ʼ��
//ÿcnt/1000 ms����һ���ж�
void PIT_Init(uint32_t cnt)
{
  cnt = cnt * bus_clk_khz/1000;
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;          //ʹ��PITʱ��

  PIT_MCR  = (0
              //| PIT_MCR_MDIS_MASK       //����PIT��ʱ��ʱ��ѡ��0��ʾʹ��PIT��1��ʾ��ֹPIT��
              //| PIT_MCR_FRZ_MASK        //����ģʽ��ֹͣ���У�0��ʾ�������У�1��ʾֹͣ���У�
               );
  PIT_LDVAL(0)  = cnt - 1 ;                    //��������ж�ʱ��

  PIT_TFLG(0) |= PIT_TFLG_TIF_MASK;        //���жϱ�־��д1 ��ձ�־λ��                          //���жϱ�־λ

  PIT_TCTRL(0) &= ~ PIT_TCTRL_TEN_MASK;        //��ֹPITn��ʱ����������ռ���ֵ��
  PIT_TCTRL(0)  = ( 0
                    | PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
                    | PIT_TCTRL_TIE_MASK        //��PITn�ж�
                  );
  enable_irq(PIT_irq_no);            //���ж�
  set_irq_priority(PIT_irq_no,3) ;
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PIT_IRQHandler
*  ����˵����PIT0 ��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*  ��    ע��
*************************************************************************/

void PIT_IRQHandler(void)
{
  //���жϱ�־��д1 ��ձ�־λ��
  PIT_TFLG(0) |= PIT_TFLG_TIF_MASK;
  TimeCount ++ ;
  if(TimeCount%5 == 0 )
  {
    TIMEflag_5ms = 1;
  }
  if(TimeCount%10 == 0 )
  {
    TIMEflag_10ms = 1;    
  }
  if(TimeCount%20 == 0 )
  {
    TIMEflag_20ms = 1;
  }
  if(TimeCount%30 == 0 )
  {
    TIMEflag_30ms = 1;
  }
  if(TimeCount%300 == 0 )
  {
    TIMEflag_300ms = 1;
  }
  if(TimeCount%500 == 0 )
  {
    TIMEflag_500ms = 1;
    TimeCount = 0 ;
    
  }
}
