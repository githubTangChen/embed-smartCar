/*****************************************
�� �� ����PIT.h
��    ������ʱ������
��    �ߣ�landzo ������
*****************************************/
#ifndef _PIT_H_
#define _PIT_H_


//������"vectors.h"�����ĺ��棡����
//�ж��ض���
#undef  VECTOR_038
#define VECTOR_038    PIT_IRQHandler


//PIT0��ʱ����ʼ��
//ÿcnt/1000 ms����һ���ж�
void PIT_Init(uint32_t cnt);
//��ʱ���жϷ�����
void PIT_IRQHandler(void);

#endif