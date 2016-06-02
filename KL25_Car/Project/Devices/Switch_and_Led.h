/*****************************************
�� �� ����Switch_and_Led.h
��    �������뿪�������Լ�LED���ų�ʼ��
�޸����ڣ�2016-3-12
*****************************************/
#ifndef _SWITCH_AND_LED_H_
#define _SWITCH_AND_LED_H_

#define ThresholdValueAdd 0x07u
#define ThresholdValueDecrease 0x06u
#define MotorDutyAdd 0x05u
#define MotorDutyDecrease 0x04u

#define KEY_UP     1//��Ӧ�ɿ�״̬
#define KEY_DOWN   0//��Ӧ��������״̬

#define SWITCH_ON  0 //��Ӧ���뿪��ON״̬
#define SWITCH_OFF 1 //��Ӧ���뿪��OFF״̬

//����PTB�Ķ˿�  ����by ����
//��ֵ1�������
//��ֵ0�������
#define PTB0_OUT     PTB_BASE_PTR->PDORs.PDOR0

//BEEP״̬��ת
//��ֵ1�������
//��ֵ0�������
#define BEEP_TURN    PTB_BASE_PTR->PDORs.PDOR2

#define A4_IN     PTA_BASE_PTR->PDIRs.PDIR4//PTA4
#define A5_IN     PTA_BASE_PTR->PDIRs.PDIR5//PTA4
#define A14_IN    PTA_BASE_PTR->PDIRs.PDIR14//PTA4
#define A15_IN    PTA_BASE_PTR->PDIRs.PDIR15//PTA4

//��õ������ŵ�״̬  ����by ����
//0������Ϊ��
//1������Ϊ��
#define KEY0_IN     PTB_BASE_PTR->PDIRs.PDIR8//PTB8
#define KEY1_IN     PTB_BASE_PTR->PDIRs.PDIR18//PTB18
#define PTC0_IN     PTC_BASE_PTR->PDIRs.PDIR0
#define PTC1_IN     PTC_BASE_PTR->PDIRs.PDIR1
#define PTC2_IN     PTC_BASE_PTR->PDIRs.PDIR2
#define PTC3_IN     PTC_BASE_PTR->PDIRs.PDIR3

void SwitchInit(void);//���뿪�س�ʼ��
void LedInit(void);//Led�����ų�ʼ��
void BeepInit(void);//Beep���ų�ʼ��
void BeepON(void);//Beep��
void BeepOFF(void);//Beep��
void KeyInit(void);//������ʼ��
void RemoteControlInit(void);//ң�����ų�ʼ��
uint8 KeyScan(void);//�����벦�뿪��ɨ��
#endif