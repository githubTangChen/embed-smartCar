/*****************************************
文 件 名：Control.h
描    述：速度与方向控制
修改日期：2016-5-15
*****************************************/
#ifndef _CONTROL_H_
#define _CONTROL_H_

#define MAX_MOTOR_PWM_VAULE            300
//最左边
#define MAX_SERVOMOTOR_PWM_VAULE       640
//最右边
#define MIN_SERVOMOTOR_PWM_VAULE       460

#define Servo_A 0.7

typedef struct 
{
  float SetSpeed;
  float  Speed;//单前速度
  int16  MotorDuty;//马达占空比
  uint16  ServoMotorDuty;//舵机占空比
  uint16  MiddlePositionDuty;//舵机正中占比 550
}STRUCT_MOTORPARAMETER;
extern STRUCT_MOTORPARAMETER MotorParameter;//电机以及舵机控制变量

typedef struct 
{
  float SpeedControlP,SpeedControlI,SpeedControlD;
  float DirectionControlP,DirectionControlI,DirectionControlD;
}STRUCT_PIDVAULE;
extern STRUCT_PIDVAULE PIDVaule;


void ServoMotorSmoothControl(unsigned char Time);
void SpeedControl(float targetSpeed);
#endif