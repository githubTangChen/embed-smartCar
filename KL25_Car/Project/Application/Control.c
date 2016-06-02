/*****************************************
�� �� ����Control.c
��    �����ٶ��뷽�����
�޸����ڣ�2016-5-15
*****************************************/
#include "common.h"

extern unsigned char EndPosition;//��ǰ�����ı߽�������
extern signed char MidPoints[PICTURE_HIGHT];//���ߴ洢����
extern signed char roadFlag;
extern signed char countTimes;
float setTargetSpeed = 2.5;

STRUCT_MOTORPARAMETER MotorParameter;//����Լ�������Ʊ���
STRUCT_PIDVAULE PIDVaule;
unsigned char StartLine = 25;
unsigned char ControlLine = 10;//ÿ�θ���ControlLine�е����߿��ƶ��
//������ƫ���¼ 0 ��ǰƫ��  1 �ϴ����
float ServoDeviation[2] = {0.0,0.0};
//ƫ��Ĳ�ּ�¼
float ServoDifference[2] = {0.0,0.0};
/***********************************/
//��������ServoMotorSmoothControl
//������Time ���������ƴ���
//����ֵ����
//
//ȡ����ͷ�ɼ�����Ϊ30ms
//ȡ�����������Ϊ5ms
//������ͷ�ɼ���ɺ���30ms  �Ż�ɼ���һ��ͼ
//����30ms�ֳ�6�������������
//ÿ�����ڼ�����Ӧλ�õ�PWM
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
  //���ǿ������õ�����Զ���ĵ�ĺ�һ����
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
  //�ﵽ�ֶο�������
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
      if (MotorParameter.ServoMotorDuty > MAX_SERVOMOTOR_PWM_VAULE)//�޷�
      {
        MotorParameter.ServoMotorDuty = MAX_SERVOMOTOR_PWM_VAULE;
      }
      if (MotorParameter.ServoMotorDuty < MIN_SERVOMOTOR_PWM_VAULE)//�޷�
      {
        MotorParameter.ServoMotorDuty = MIN_SERVOMOTOR_PWM_VAULE;
      }
      ServoMotorChangeDegree(MotorParameter.ServoMotorDuty);
	  //SpeedControl(2.5);
  }
  //�ﲻ��
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
     if (MotorParameter.ServoMotorDuty > MAX_SERVOMOTOR_PWM_VAULE)//�޷�
      {
        MotorParameter.ServoMotorDuty = MAX_SERVOMOTOR_PWM_VAULE;
      }
     if (MotorParameter.ServoMotorDuty < MIN_SERVOMOTOR_PWM_VAULE)//�޷�
      {
        MotorParameter.ServoMotorDuty = MIN_SERVOMOTOR_PWM_VAULE;
      }    
      //���PWM
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
	  //���PWM
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
//������ƫ���¼ 0 ��ǰƫ��  1 �ϴ����
float SpeedDeviation[2] = {0.0,0.0};
//ƫ��Ĳ�ּ�¼
float SpeedDifference[2] = {0.0,0.0};
/***********************************/
//��������SpeedControl
//������targetSpeed ����Ŀ���ٶ�
//����ֵ����
/***********************************/
void SpeedControl(float targetSpeed)
{ 
  static uint32 count;//�������
  int16 Error;
  count = LptmrPulseGet();//�õ����弼��ֵ���������ֵ                                                    
      //�����������������ֵ,����������������ֵ��������գ��������ܱ�֤����ֵ׼ȷ��
  count += Pulse_INT_Count * MaxPulse;//���40ms���������
  Pulse_INT_Count =0;//�жϴ�����0
  MotorParameter.Speed = ((float)count/OneMeterPulse)/0.02;//��õ�ǰ�ٶ�
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
