/*****************************************
文 件 名：Switch_and_Led.h
描    述：拨码开关引脚以及LED引脚初始化
修改日期：2016-3-12
*****************************************/
#ifndef _SWITCH_AND_LED_H_
#define _SWITCH_AND_LED_H_

#define ThresholdValueAdd 0x07u
#define ThresholdValueDecrease 0x06u
#define MotorDutyAdd 0x05u
#define MotorDutyDecrease 0x04u

#define KEY_UP     1//对应松开状态
#define KEY_DOWN   0//对应按键按下状态

#define SWITCH_ON  0 //对应拨码开关ON状态
#define SWITCH_OFF 1 //对应拨码开关OFF状态

//定义PTB的端口  ——by 蓝宙
//赋值1：输出高
//赋值0：输出低
#define PTB0_OUT     PTB_BASE_PTR->PDORs.PDOR0

//BEEP状态翻转
//赋值1：输出高
//赋值0：输出低
#define BEEP_TURN    PTB_BASE_PTR->PDORs.PDOR2

#define A4_IN     PTA_BASE_PTR->PDIRs.PDIR4//PTA4
#define A5_IN     PTA_BASE_PTR->PDIRs.PDIR5//PTA4
#define A14_IN    PTA_BASE_PTR->PDIRs.PDIR14//PTA4
#define A15_IN    PTA_BASE_PTR->PDIRs.PDIR15//PTA4

//获得单个引脚的状态  ——by 蓝宙
//0：输入为低
//1：输入为高
#define KEY0_IN     PTB_BASE_PTR->PDIRs.PDIR8//PTB8
#define KEY1_IN     PTB_BASE_PTR->PDIRs.PDIR18//PTB18
#define PTC0_IN     PTC_BASE_PTR->PDIRs.PDIR0
#define PTC1_IN     PTC_BASE_PTR->PDIRs.PDIR1
#define PTC2_IN     PTC_BASE_PTR->PDIRs.PDIR2
#define PTC3_IN     PTC_BASE_PTR->PDIRs.PDIR3

void SwitchInit(void);//拨码开关初始化
void LedInit(void);//Led灯引脚初始化
void BeepInit(void);//Beep引脚初始化
void BeepON(void);//Beep响
void BeepOFF(void);//Beep关
void KeyInit(void);//按键初始化
void RemoteControlInit(void);//遥控引脚初始化
uint8 KeyScan(void);//按键与拨码开关扫描
#endif