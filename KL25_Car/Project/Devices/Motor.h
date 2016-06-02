/*****************************************
文 件 名：Motor.h
描    述：马达驱动板控制底层驱动
          以及舵机控制底层驱动
修改日期：2016-3-5
*****************************************/
#ifndef _MOTOR_H_
#define _MOTOR_H_

#define BRAKING   2 //制动
#define FORWARD   1 //前进
#define BACKWARD  0 //后退

#define PRECISON 1000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TPM_PRECISON

extern signed int ServoControlOutput[6];//电机控制变量


void MotorInit(void);//马达驱动板初始化
void MotorRun(uint32 PwmVaule,char Direction);//马达改变转速
void SpeedControl(float targetSpeed);//速度控制

void ServoMotorInit(void);//舵机初始化
void ServoMotorChangeDegree(uint32);//舵机改变角度
void ServoMotorSmoothControl(unsigned char);//舵机平滑控制
#endif