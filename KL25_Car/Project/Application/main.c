/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
* �ļ���       �� main.c
* ����         ������ģ��ʵ��
*
* ʵ��ƽ̨     ��landzo���ӿ�����
* ��汾       ��
* Ƕ��ϵͳ     ��
*
* ����         ��landzo ������
* �Ա���       ��http://landzo.taobao.com/
* ��ע         ��  clk PTC10 ,SDA PTC11 , HEY A17,VSY A13,CLK A16,Y0-Y7 PTC0-PTC7 
**********************************************************************************/
#include "common.h"

const  uint32_t FlashAddress@0x10000;
uint8_t TIMEflag_5ms  ;
uint8_t TIMEflag_10ms  ;
uint8_t TIMEflag_20ms  ;
uint8_t TIMEflag_30ms  ;
uint8_t TIMEflag_500ms ;
uint8_t TIMEflag_300ms ;

float speed;
uint8_t KeyScanResult;//����ɨ����
uint8_t ServoMotorControlTimes = 0;//һ�����ڶ�����ƴ���
uint8_t checkflag = 0;//sccb��д����
uint32 count;//�������
int8_t K = 0;
uint8_t RoadLine = 59;
extern volatile u8 Pulse_INT_Count;
extern uint8 SendFlag;//�������ͱ�־
extern uint16 USART_RX_CNT; //�����ַ�����   
extern signed char MidPoints[PICTURE_HIGHT];
extern unsigned char MidLineCompleteFlag;
extern signed char roadFlag;

//ͼ������//ͼ������
//�ɼ���ɺ��Ѿ�����ֵ��
//���Ϊ0 �����ɫ   Ϊ1 �����ɫ
unsigned char PixData[PICTURE_HIGHT][PICTURE_WIDTH] = {0};

void ddlay(void){
  volatile uint16_t  dlaycount ;
   
  for(dlaycount = 0 ; dlaycount < 1000 ; dlaycount ++){
  }
}
void main()
{
  
  DisableInterrupts;
  OLED_Init();//��ʾ����ʼ��
  ArgumentsInit();
  //PIT_Init(1000); //��ʼ��PIT0����ʱʱ��Ϊ�� 1ms
  UART_Init (115200);//���ڳ�ʼ��  UART0_RX  PTB16 /UART0_TX  PTB17
  uart_irq_EN();
  ADC_Init();//adc��ʼ��
  SwitchInit();//���뿪�س�ʼ��
  KeyInit();//������ʼ��
  LedInit();//Led�����ų�ʼ��
  BeepInit();//BEEP���ų�ʼ��
  RemoteControlInit();//ң�����ų�ʼ��
  EncoderInit();//��������ʼ��
  ServoMotorInit();//���PWM��ʼ��
  ServoMotorChangeDegree(MotorParameter.MiddlePositionDuty);//�������  
  MotorInit();//���PWM��ʼ��
  MotorRun(100,FORWARD);//���PWM 0  ��ת
  //IICWriteGpio_Init() ;//SCCB���ų�ʼ��
  /*****************************************************
  whileѭ��������������IIC����
  ���ⲿ�ִ�������״̬������ͷת�Ӱ忪�ز���MM��Ĭ��״̬
  *****************************************************/
 /*while(checkflag != 1 )//�˴������λ�ʹ��λ��һֱ����
  {
    checkflag = LandzoIICEEROM_INIT();
    BFdelay_1us(100);      // ��ʱ100us 
  }*/
  //���ж�Ϊ PTA1 ,���ж�ΪPTA17 ,CLKΪPTA16��Y0-Y7ΪPTD0-PTD7
  Camera_Init();//����ͷ�����Լ�DMA����
  //flash();
  //printf ("\t\n%d \t\n",MotorParameter.MiddlePositionDuty);
  EnableInterrupts;
  enable_irq(PortA_irq_no);
   BEEP_TURN = 0;
   PTB0_OUT = 0;
  while(1)
  {
    printf("%d\r\n",(int)(MotorParameter.Speed*100));
    if (A5_IN ==1)
    {
      PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
      PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 | PORT_PCR_MUX(1) | 0x02|PORT_PCR_IRQC(0) );
      MotorRun(0,BRAKING);
    }
	/*if(roadFlag & 0x01)
	{
		BeepON();
		//roadFlag &= 0xfe;
	}
	else
	{
		BeepOFF();
	}*/
  }
  
}