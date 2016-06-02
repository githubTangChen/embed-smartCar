/*****************************************
文 件 名：Control.c
描    述：速度与方向控制
修改日期：2016-5-15
*****************************************/
#include "common.h"

extern unsigned char EndPosition;//提前结束的边界行坐标
extern signed char MidPoints[PICTURE_HIGHT];//中线存储数组
extern signed char roadFlag;
extern signed char countTimes;
float setTargetSpeed = 2.5;

STRUCT_MOTORPARAMETER MotorParameter;//电机以及舵机控制变量
STRUCT_PIDVAULE PIDVaule;
unsigned char StartLine = 25;
unsigned char ControlLine = 10;//每次根据ControlLine行的中线控制舵机
//控制行偏差记录 0 单前偏差  1 上次误差
float ServoDeviation[2] = {0.0,0.0};
//偏差的差分记录
float ServoDifference[2] = {0.0,0.0};
/***********************************/
//函数名：ServoMotorSmoothControl
//参数：Time 代表舵机控制次数
//返回值：无
//
//取摄像头采集周期为30ms
//取舵机控制周期为5ms
//当摄像头采集完成后还有30ms  才会采集下一副图
//将这30ms分成6个舵机控制周期
//每个周期计数对应位置的PWM
//
/***********************************/
void ServoMotorSmoothControl(unsigned char Time)
{
  
  //static uint16 LastOutput = 0;
  //int16 error;
  signed int ErrorSum = 0;
  signed char Error;
  unsigned char CountLine = 0;
  unsigned char i;
  unsigned char j;
  int EndPositionError = MidPoints[EndPosition+1]+MidPoints[EndPosition+2]+MidPoints[EndPosition+3]+
	  	MidPoints[EndPosition+4]+MidPoints[EndPosition+5];
  //i = (PICTURE_HIGHT - StartLine - ControlLine);//30
  //这是控制所用到的最远处的点的后一个点
  j = (PICTURE_HIGHT - StartLine - ControlLine*2);//20
  //if (MotorParameter.ServoMotorDuty > 590 || MotorParameter.ServoMotorDuty < 510)
        i = (int)(PICTURE_HIGHT- ControlLine*(Time)  - MotorParameter.Speed*10);
  //else
        //i = (int)(PICTURE_HIGHT  - MotorParameter.Speed*15);
  if((EndPosition+ControlLine+5)>i)
  	i = ControlLine + EndPosition+5;
  if( 40 < i)
  	i = 40;
   OLED_P6x8Str(90,0,"E");
   OLED_PrintValueInt(100,0,EndPosition,2);
    OLED_P6x8Str(90,1,"E");
    OLED_PrintValueInt(100,1,i,2);
  //达到分段控制条件
  if (EndPosition <= 15)
  {
      for (j = 0;j < ControlLine ;j++)
      {
        Error = MidPoints[i] - 34;
        if (Error > 2||Error < -2)
        {
          ErrorSum += Error;
          CountLine++;
        }
        i--;
      }
      ServoDeviation [1] = ServoDeviation[0];
      if (0 == CountLine)
        ServoDeviation [0] = 0.0;
      else
        ServoDeviation [0] = ((float)((float)ErrorSum / (float)ControlLine));
      ServoDifference[1] = ServoDifference[0];
      ServoDifference[0] = ServoDeviation [0] - ServoDeviation [1];
      MotorParameter.ServoMotorDuty = MotorParameter.MiddlePositionDuty 
                                      - ((int16)(ServoDeviation[0]*PIDVaule.DirectionControlP)
                                      //+ (int16)(Servo_A * ServoDifference[1]*PIDVaule.DirectionControlD));
                                      + (int16)(((Servo_A * ServoDifference[1]) + (1-Servo_A)* ServoDifference[0])*PIDVaule.DirectionControlD));
                                      //+ (int16)(ServoDifference[0]*PIDVaule.DirectionControlD));
      if (MotorParameter.ServoMotorDuty > MAX_SERVOMOTOR_PWM_VAULE)//限幅
      {
        MotorParameter.ServoMotorDuty = MAX_SERVOMOTOR_PWM_VAULE;
      }
      if (MotorParameter.ServoMotorDuty < MIN_SERVOMOTOR_PWM_VAULE)//限幅
      {
        MotorParameter.ServoMotorDuty = MIN_SERVOMOTOR_PWM_VAULE;
      }
      ServoMotorChangeDegree(MotorParameter.ServoMotorDuty);
	  //SpeedControl(2.5);
  }
  //达不到
  else
  {
    Error = MidPoints[i] - 34;
		ServoDeviation[1] = ServoDeviation[0];
                ServoDeviation[0] = ((float)Error);
		ServoDifference[1] = ServoDifference[0];
		ServoDifference[0] = ServoDeviation[0] - ServoDeviation[1];
		MotorParameter.ServoMotorDuty = MotorParameter.MiddlePositionDuty
			- ((int16)(ServoDeviation[0] * 10)
			+ (int16)(((Servo_A * ServoDifference[1]) + (1 - Servo_A)* ServoDifference[0])*PIDVaule.DirectionControlD));
     if (MotorParameter.ServoMotorDuty > MAX_SERVOMOTOR_PWM_VAULE)//限幅
      {
        MotorParameter.ServoMotorDuty = MAX_SERVOMOTOR_PWM_VAULE;
      }
     if (MotorParameter.ServoMotorDuty < MIN_SERVOMOTOR_PWM_VAULE)//限幅
      {
        MotorParameter.ServoMotorDuty = MIN_SERVOMOTOR_PWM_VAULE;
      }    
      //输出PWM
      ServoMotorChangeDegree(MotorParameter.ServoMotorDuty);
	  //SpeedControl(1.8);  
  }
  
  if((MotorParameter.ServoMotorDuty > 600 || MotorParameter.ServoMotorDuty < 500))
	{
	  SpeedControl(setTargetSpeed-0.6); 
	  return;
	}
	if ((MotorParameter.ServoMotorDuty > 600 || MotorParameter.ServoMotorDuty < 500) || EndPosition>15)
	{
		SpeedControl(setTargetSpeed-0.5); 
	}
	else
	{
	  //输出PWM
		if(EndPositionError<50 || EndPositionError> 250)
		{
		  SpeedControl(setTargetSpeed-0.4);
		}
		else
		{
		  if(EndPositionError<100 || EndPositionError> 200)
		  {
			  SpeedControl(setTargetSpeed-0.2);
		  }
		  else
		  {
			  SpeedControl(setTargetSpeed);
		  }
		}
	}
 //OLED_PrintValueInt(75,1,MotorParameter.ServoMotorDuty,3);
}
//控制行偏差记录 0 单前偏差  1 上次误差
float SpeedDeviation[2] = {0.0,0.0};
//偏差的差分记录
float SpeedDifference[2] = {0.0,0.0};
/***********************************/
//函数名：SpeedControl
//参数：targetSpeed 代表目标速度
//返回值：无
/***********************************/
void SpeedControl(float targetSpeed)
{ 
  static uint32 count;//脉冲计数
  int16 Error;
  count = LptmrPulseGet();//得到脉冲技术值并清除计数值                                                    
      //保存脉冲计数器计算值,清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
  count += Pulse_INT_Count * MaxPulse;//间隔40ms的脉冲次数
  Pulse_INT_Count =0;//中断次数清0
  MotorParameter.Speed = ((float)count/OneMeterPulse)/0.02;//获得单前速度
  count = 0;
  OLED_PrintValueF(90,4,(MotorParameter.Speed),1);
  
  Error = (int16)((targetSpeed -  MotorParameter.Speed) * 50);
  SpeedDeviation[1] = SpeedDeviation[0];
  SpeedDeviation[0] = ((float)Error);
  SpeedDifference[1] = SpeedDifference[0];
  SpeedDifference[0] = SpeedDeviation[0] - SpeedDeviation[1];
  MotorParameter.MotorDuty = MotorParameter.MotorDuty 
  			+ ((int16)(SpeedDeviation[0] *PIDVaule.SpeedControlP)
			- (int16)(((Servo_A * SpeedDifference[1]) + (1 - Servo_A)* SpeedDifference[0])*PIDVaule.SpeedControlD));
  if (MotorParameter.MotorDuty > MAX_MOTOR_PWM_VAULE)
    MotorParameter.MotorDuty = MAX_MOTOR_PWM_VAULE;
  if (MotorParameter.MotorDuty <100)
    MotorParameter.MotorDuty = 100;
  if((MotorParameter.ServoMotorDuty > 600 || MotorParameter.ServoMotorDuty < 500))
  {
  	MotorParameter.MotorDuty = 160;
  }
  MotorRun(MotorParameter.MotorDuty,FORWARD);
  
}
