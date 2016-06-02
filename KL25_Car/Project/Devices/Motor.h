/*****************************************
�� �� ����Motor.h
��    ���������������Ƶײ�����
          �Լ�������Ƶײ�����
�޸����ڣ�2016-3-5
*****************************************/
#ifndef _MOTOR_H_
#define _MOTOR_H_

#define BRAKING   2 //�ƶ�
#define FORWARD   1 //ǰ��
#define BACKWARD  0 //����

#define PRECISON 1000u     //����ռ�ձȾ��ȣ�100������Ϊ1%��1000u�򾫶�Ϊ0.1%������ռ�ձ� duty �βδ��룬��ռ�ձ�Ϊ duty/TPM_PRECISON

extern signed int ServoControlOutput[6];//������Ʊ���


void MotorInit(void);//����������ʼ��
void MotorRun(uint32 PwmVaule,char Direction);//���ı�ת��
void SpeedControl(float targetSpeed);//�ٶȿ���

void ServoMotorInit(void);//�����ʼ��
void ServoMotorChangeDegree(uint32);//����ı�Ƕ�
void ServoMotorSmoothControl(unsigned char);//���ƽ������
#endif