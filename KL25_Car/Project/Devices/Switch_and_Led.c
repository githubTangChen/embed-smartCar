/*****************************************
�� �� ����Switch_and_Led.c
��    �������뿪�������Լ�LED���ų�ʼ��
          ������ʼ��
�޸����ڣ�2016-3-12
*****************************************/
#include "common.h"
//���뿪�س�ʼ��
//PTC1-4�������� ���������ж�
void SwitchInit(void)
{
  //PTC0�������� ���������ж�
  PORT_PCR_REG(PORTC_BASE_PTR,0) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 0);  //���ö˿ڷ���Ϊ����
  //PTC1�������� ���������ж�
  PORT_PCR_REG(PORTC_BASE_PTR,1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 1);  //���ö˿ڷ���Ϊ����
  //PTC2�������� ���������ж�
  PORT_PCR_REG(PORTC_BASE_PTR,2) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 2);  //���ö˿ڷ���Ϊ����
  //PTC3�������� ���������ж�
  PORT_PCR_REG(PORTC_BASE_PTR,3) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 3);  //���ö˿ڷ���Ϊ����
}
//Led�����ų�ʼ��
//PTB0��ʼ����ߵ�ƽ
//LED����
void LedInit(void)
{
  //ָ�������Ź���ΪGPIO���ܣ��������ſ��ƼĴ�����MUX=0b001��
  PORT_PCR_REG(PORTB_BASE_PTR, 0)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 0)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) |= (1 << 0); //���ö˿ڷ���Ϊ���
  GPIO_PDOR_REG(PTB_BASE_PTR) |=  (1 << 0); //�������Ϊ�ߵ�ƽ    
}
//Beep���ų�ʼ��
//PTB2��ʼ����ߵ�ƽ
//
void BeepInit(void)
{
  //ָ�������Ź���ΪGPIO���ܣ��������ſ��ƼĴ�����MUX=0b001��
  PORT_PCR_REG(PORTB_BASE_PTR, 2)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 2)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) |= (1 << 2); //���ö˿ڷ���Ϊ���
  GPIO_PDOR_REG(PTB_BASE_PTR) |=  (1 << 2); //�������Ϊ�ߵ�ƽ    
}
//Beep��
void BeepON(void){
  volatile uint16_t  dlaycount ;
  BEEP_TURN = 1;
  for(dlaycount = 0 ; dlaycount < 50 ; dlaycount ++){
  }
  BEEP_TURN = 0;
  for(dlaycount = 0 ; dlaycount < 50 ; dlaycount ++){
  }
}
//Beep��
void BeepOFF(void)
{
  BEEP_TURN = 0;
}
//ң�����ų�ʼ��
//PTA4��PTA5,PTA14,PTA15
//ʹ���жϣ��жϷ������� PredatorCamera.c ��
void RemoteControlInit(void)
{
  PORT_PCR_REG(PORTA_BASE_PTR, 4) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x0003           //��������      
                                     |PORT_PCR_IRQC(0) //�½����ж�
                                     );
  //���ö˿ڷ���Ϊ����
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 4);  
  PORT_PCR_REG(PORTA_BASE_PTR, 5) = (0 
                                     | PORT_PCR_MUX(1)
                                     | 0x0003//��������      
                                     |PORT_PCR_IRQC(0) //�½����ж�
                                     );
  //���ö˿ڷ���Ϊ����
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 5); 
  PORT_PCR_REG(PORTA_BASE_PTR, 14) = (0 
                                     | PORT_PCR_MUX(1)
                                     | 0x03            //��������      
                                     |PORT_PCR_IRQC(0) //�½����ж�
                                     );
  //���ö˿ڷ���Ϊ����
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 14); 
  PORT_PCR_REG(PORTA_BASE_PTR, 15) = (0 
                                     | PORT_PCR_MUX(1)
                                     | 0x03             //��������      
                                     |PORT_PCR_IRQC(0) //�½����ж�
                                     );
  //���ö˿ڷ���Ϊ����
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 15); 
}
//�������ų�ʼ��
//PTB8
//PTB18
void KeyInit(void)
{
  //ָ�������Ź���ΪGPIO���ܣ��������ſ��ƼĴ�����MUX=0b001��
  PORT_PCR_REG(PORTB_BASE_PTR, 8)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 8)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) &= ~(1 << 8);  //���ö˿ڷ���Ϊ����
  //ָ�������Ź���ΪGPIO���ܣ��������ſ��ƼĴ�����MUX=0b001��
  PORT_PCR_REG(PORTB_BASE_PTR, 18)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 18)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) &= ~(1 << 18);  //���ö˿ڷ���Ϊ����
}
//�����벦�뿪��ɨ��
uint8_t KeyScan(void)
{
  unsigned char Status;
  Status = PTC3_IN | PTC2_IN << 1 | PTC1_IN <<2 | PTC0_IN <<3;
  //OLED_PrintValueInt(90,4,PTC4_IN,3);
  if (KEY0_IN == KEY_UP)
  {
    return 0;
  }
  else if (Status == ThresholdValueAdd && KEY0_IN == KEY_DOWN)
  {
    return (uint8_t)ThresholdValueAdd;
  }
  else if (Status == ThresholdValueDecrease && KEY0_IN == KEY_DOWN)
  {
    return (uint8_t)ThresholdValueDecrease;
  }
  else if (Status == MotorDutyAdd && KEY0_IN == KEY_DOWN)
  {
    return (uint8_t)MotorDutyAdd;
  }
  else if (Status == MotorDutyDecrease && KEY0_IN == KEY_DOWN)
  {
    return (uint8_t)MotorDutyDecrease;
  }
  else
  {
    return 0;
  }
}