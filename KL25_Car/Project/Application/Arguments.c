/*****************************************
�� �� ����Arguments.c
��    ����ͼ����
�޸����ڣ�2016-4-12
*****************************************/
#include "common.h"
//������ֵ
void ArgumentsInit()
{
  ThresholdValue = 100;//��ֵ����ֵ
  PIDVaule.DirectionControlP = 3.8;
  PIDVaule.DirectionControlD = 7.0;
  PIDVaule.SpeedControlP = 0.8;
  PIDVaule.SpeedControlD = 0.1;
  MotorParameter.MotorDuty = 140;//�����ʼPWM
  MotorParameter.MiddlePositionDuty = 550;//����м�λ�ö�ӦPWM 0.55
  MotorParameter.ServoMotorDuty = 550;
  MotorParameter.SetSpeed = 1.2;
}
//������д��flash
void ArgumentsSave()
{
  FlashWrite();
}