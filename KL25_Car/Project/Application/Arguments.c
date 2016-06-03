/*****************************************
文 件 名：Arguments.c
描    述：图像处理
修改日期：2016-4-12
*****************************************/
#include "common.h"
//参数赋值
void ArgumentsInit()
{
  ThresholdValue = 100;//二值化阀值
  PIDVaule.DirectionControlP = 3.8;
  PIDVaule.DirectionControlD = 7.0;
  PIDVaule.SpeedControlP = 0.8;
  PIDVaule.SpeedControlD = 0.1;
  MotorParameter.MotorDuty = 140;//电机初始PWM
  MotorParameter.MiddlePositionDuty = 550;//舵机中间位置对应PWM 0.55
  MotorParameter.ServoMotorDuty = 550;
  MotorParameter.SetSpeed = 1.2;
}
//将参数写入flash
void ArgumentsSave()
{
  FlashWrite();
}