/*****************************************
文 件 名：Switch_and_Led.c
描    述：拨码开关引脚以及LED引脚初始化
          按键初始化
修改日期：2016-3-12
*****************************************/
#include "common.h"
//拨码开关初始化
//PTC1-4输入上拉 ，不允许中断
void SwitchInit(void)
{
  //PTC0输入上拉 ，不允许中断
  PORT_PCR_REG(PORTC_BASE_PTR,0) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 0);  //设置端口方向为输入
  //PTC1输入上拉 ，不允许中断
  PORT_PCR_REG(PORTC_BASE_PTR,1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 1);  //设置端口方向为输入
  //PTC2输入上拉 ，不允许中断
  PORT_PCR_REG(PORTC_BASE_PTR,2) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 2);  //设置端口方向为输入
  //PTC3输入上拉 ，不允许中断
  PORT_PCR_REG(PORTC_BASE_PTR,3) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
  GPIO_PDDR_REG(PTC_BASE_PTR) &= ~(1 << 3);  //设置端口方向为输入
}
//Led灯引脚初始化
//PTB0初始输出高电平
//LED不亮
void LedInit(void)
{
  //指定该引脚功能为GPIO功能（即令引脚控制寄存器的MUX=0b001）
  PORT_PCR_REG(PORTB_BASE_PTR, 0)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 0)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) |= (1 << 0); //设置端口方向为输出
  GPIO_PDOR_REG(PTB_BASE_PTR) |=  (1 << 0); //设置输出为高电平    
}
//Beep引脚初始化
//PTB2初始输出高电平
//
void BeepInit(void)
{
  //指定该引脚功能为GPIO功能（即令引脚控制寄存器的MUX=0b001）
  PORT_PCR_REG(PORTB_BASE_PTR, 2)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 2)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) |= (1 << 2); //设置端口方向为输出
  GPIO_PDOR_REG(PTB_BASE_PTR) |=  (1 << 2); //设置输出为高电平    
}
//Beep响
void BeepON(void){
  volatile uint16_t  dlaycount ;
  BEEP_TURN = 1;
  for(dlaycount = 0 ; dlaycount < 50 ; dlaycount ++){
  }
  BEEP_TURN = 0;
  for(dlaycount = 0 ; dlaycount < 50 ; dlaycount ++){
  }
}
//Beep关
void BeepOFF(void)
{
  BEEP_TURN = 0;
}
//遥控引脚初始化
//PTA4，PTA5,PTA14,PTA15
//使能中断，中断服务函数在 PredatorCamera.c 中
void RemoteControlInit(void)
{
  PORT_PCR_REG(PORTA_BASE_PTR, 4) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x0003           //输入下拉      
                                     |PORT_PCR_IRQC(0) //下降沿中断
                                     );
  //设置端口方向为输入
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 4);  
  PORT_PCR_REG(PORTA_BASE_PTR, 5) = (0 
                                     | PORT_PCR_MUX(1)
                                     | 0x0003//输入下拉      
                                     |PORT_PCR_IRQC(0) //下降沿中断
                                     );
  //设置端口方向为输入
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 5); 
  PORT_PCR_REG(PORTA_BASE_PTR, 14) = (0 
                                     | PORT_PCR_MUX(1)
                                     | 0x03            //输入下拉      
                                     |PORT_PCR_IRQC(0) //下降沿中断
                                     );
  //设置端口方向为输入
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 14); 
  PORT_PCR_REG(PORTA_BASE_PTR, 15) = (0 
                                     | PORT_PCR_MUX(1)
                                     | 0x03             //输入下拉      
                                     |PORT_PCR_IRQC(0) //下降沿中断
                                     );
  //设置端口方向为输入
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 15); 
}
//按键引脚初始化
//PTB8
//PTB18
void KeyInit(void)
{
  //指定该引脚功能为GPIO功能（即令引脚控制寄存器的MUX=0b001）
  PORT_PCR_REG(PORTB_BASE_PTR, 8)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 8)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) &= ~(1 << 8);  //设置端口方向为输入
  //指定该引脚功能为GPIO功能（即令引脚控制寄存器的MUX=0b001）
  PORT_PCR_REG(PORTB_BASE_PTR, 18)=  0 ;
  PORT_PCR_REG(PORTB_BASE_PTR, 18)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTB_BASE_PTR) &= ~(1 << 18);  //设置端口方向为输入
}
//按键与拨码开关扫描
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