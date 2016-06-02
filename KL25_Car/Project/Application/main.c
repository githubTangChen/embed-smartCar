/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
* 文件名       ： main.c
* 描述         ：工程模版实验
*
* 实验平台     ：landzo电子开发版
* 库版本       ：
* 嵌入系统     ：
*
* 作者         ：landzo 蓝电子
* 淘宝店       ：http://landzo.taobao.com/
* 备注         ：  clk PTC10 ,SDA PTC11 , HEY A17,VSY A13,CLK A16,Y0-Y7 PTC0-PTC7 
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
uint8_t KeyScanResult;//按键扫描结果
uint8_t ServoMotorControlTimes = 0;//一个周期舵机控制次数
uint8_t checkflag = 0;//sccb读写检验
uint32 count;//脉冲计数
int8_t K = 0;
uint8_t RoadLine = 59;
extern volatile u8 Pulse_INT_Count;
extern uint8 SendFlag;//参数发送标志
extern uint16 USART_RX_CNT; //接收字符计数   
extern signed char MidPoints[PICTURE_HIGHT];
extern unsigned char MidLineCompleteFlag;
extern signed char roadFlag;

//图像数据//图像数据
//采集完成后已经被二值化
//如果为0 代表黑色   为1 代表白色
unsigned char PixData[PICTURE_HIGHT][PICTURE_WIDTH] = {0};

void ddlay(void){
  volatile uint16_t  dlaycount ;
   
  for(dlaycount = 0 ; dlaycount < 1000 ; dlaycount ++){
  }
}
void main()
{
  
  DisableInterrupts;
  OLED_Init();//显示屏初始化
  ArgumentsInit();
  //PIT_Init(1000); //初始化PIT0，定时时间为： 1ms
  UART_Init (115200);//串口初始化  UART0_RX  PTB16 /UART0_TX  PTB17
  uart_irq_EN();
  ADC_Init();//adc初始化
  SwitchInit();//拨码开关初始化
  KeyInit();//按键初始化
  LedInit();//Led灯引脚初始化
  BeepInit();//BEEP引脚初始化
  RemoteControlInit();//遥控引脚初始化
  EncoderInit();//编码器初始化
  ServoMotorInit();//舵机PWM初始化
  ServoMotorChangeDegree(MotorParameter.MiddlePositionDuty);//舵机对中  
  MotorInit();//电机PWM初始化
  MotorRun(100,FORWARD);//输出PWM 0  正转
  //IICWriteGpio_Init() ;//SCCB引脚初始化
  /*****************************************************
  while循环部分用于配置IIC数据
  将这部分处于屏蔽状态，摄像头转接板开关拨到MM即默认状态
  *****************************************************/
 /*while(checkflag != 1 )//此处不屏蔽会使复位后一直卡死
  {
    checkflag = LandzoIICEEROM_INIT();
    BFdelay_1us(100);      // 延时100us 
  }*/
  //场中断为 PTA1 ,行中断为PTA17 ,CLK为PTA16，Y0-Y7为PTD0-PTD7
  Camera_Init();//摄像头引脚以及DMA配置
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