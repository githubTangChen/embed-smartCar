/*****************************************
�� �� ����Control.h
��    �����ٶ��뷽�����
�޸����ڣ�2016-5-15
*****************************************/
#ifndef _CONTROL_H_
#define _CONTROL_H_

#define MAX_MOTOR_PWM_VAULE            300
//�����
#define MAX_SERVOMOTOR_PWM_VAULE       640
//���ұ�
#define MIN_SERVOMOTOR_PWM_VAULE       460

#define Servo_A 0.7

typedef struct 
{
  float SetSpeed;
  float  Speed;//��ǰ�ٶ�
  int16  MotorDuty;//���ռ�ձ�
  uint16  ServoMotorDuty;//���ռ�ձ�
  uint16  MiddlePositionDuty;//�������ռ�� 550
}STRUCT_MOTORPARAMETER;
extern STRUCT_MOTORPARAMETER MotorParameter;//����Լ�������Ʊ���

typedef struct 
{
  float SpeedControlP,SpeedControlI,SpeedControlD;
  float DirectionControlP,DirectionControlI,DirectionControlD;
}STRUCT_PIDVAULE;
extern STRUCT_PIDVAULE PIDVaule;


void ServoMotorSmoothControl(unsigned char Time);
void SpeedControl(float targetSpeed);
#endif