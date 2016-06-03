/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
* �ļ���       �� adc.c
* ����         ������ģ��ʵ��
*
* ʵ��ƽ̨     ��landzo���ӿ�����
* ��汾       ��
* Ƕ��ϵͳ     ��
*
* ����         ��landzo ������
* �Ա���       ��http://landzo.taobao.com/

**********************************************************************************/
#include "common.h"   //��������Ҫ��ͷ�ļ�
#include "uart.h"

uint8 SStartFlag = 0;
uint8 DStartFlag = 0;
uint8 ZStartFlag = 0;
uint16 USART_RX_STA = 0;
uint8 SComplishFlag = 0;   
uint8 DComplishFlag = 0; 
//uint8 ZComplishFlag = 0; 
//uint8 AComplishFlag = 0; 
uint8  SendFlag=0;
uint8  SendImage = 1;
uint8  USART_RX_BUF[USART_REC_LEN];
uint16 USART_RX_CNT = 0; //�����ַ�����    

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�uart_init
*  ����˵������ʼ��uartģ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ����̣�printf������⺯��
*************************************************************************/
 //UART0  //UART0_RX  PTB16 /UART0_TX  PTB17
void UART_Init (uint32_t baud_rate)
{
  //�ֲ���������
  register uint_16 sbr;
  uint8_t temp;
  uint16_t  clk_kh ;
  
  //���ݴ������uartNo�����ֲ�����uartch1��ֵ
    
    
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UART0 ʱ��
    
    //UART0_RX  PTB16
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << 1); //����PORTx�˿�
    PORT_PCR_REG(PORTB_BASE_PTR, 16) = 0x03 << PORT_PCR_MUX_SHIFT; // ���ù��� , ȷ������ģʽ ,������������������
    //UART0_TX  PTB17
    PORT_PCR_REG(PORTB_BASE_PTR, 17) = 0x03 << PORT_PCR_MUX_SHIFT;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);   //UART0ѡ��MCGFLLCLK_khz=48000Khzʱ��Դ
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    
    //��ʱ�رմ���0��������չ���
    UART0_BASE_PTR->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    
    //���ô��ڹ���ģʽ:8λ��У��ģʽ
    clk_kh = core_clk_khz/2 ;
    sbr = (uint_16)((clk_kh*1000)/(baud_rate * 16));
    temp = UART_BDH_REG(UART0_BASE_PTR) & ~(UART_BDH_SBR(0x1F));
    UART_BDH_REG(UART0_BASE_PTR) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(UART0_BASE_PTR) = (uint8_t)(sbr & UART_BDL_SBR_MASK);
    
    //��ʼ�����ƼĴ��������־λ
    UART0_C4 = 0x0F;
    UART0_C1 = 0x00;
    UART0_C3 = 0x00;
    UART0_MA1 = 0x00;
    UART0_MA2 = 0x00;
    UART0_S1 |= 0x1F;
    UART0_S2 |= 0xC0;
    
    //�������ͽ���
    UART0_BASE_PTR->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�uart_getchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
char uart_getchar (void)
{
  /* Wait until character has been received */
  while (!((UART0_BASE_PTR->S1) & UARTLP_S1_RDRF_MASK ));
  /* Return the 8-bit data from the receiver */
  return UART0_BASE_PTR->D;
}
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�uart_putchar
*  ����˵�������ڷ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ����̣�printf������⺯��
*************************************************************************/
void uart_putchar (char ch)
{
  while(!((UART0_BASE_PTR->S1) & UARTLP_S1_TDRE_MASK));
  //��������
  UART0_BASE_PTR->D = ch;
}
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�uart_irq_EN
*  ����˵���������ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART2��
*  �������أ���
*  �޸�ʱ�䣺2014-9-17
*  ��    ע��
*************************************************************************/
void uart_irq_EN(void)
{
  UART0_BASE_PTR->C2 |= UART_C2_RIE_MASK;        //����UART�����ж�
  enable_irq(UART0SE_irq_no);   //���жϿ�����IRQ�ж�
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�uart_irq_DIS
*  ����˵�����ش��ڽ����ж�
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-9-17
*  ��    ע��
*************************************************************************/
void uart_irq_DIS(void)
{
  UART0_BASE_PTR->C2 &= ~UART_C2_RIE_MASK;               //��ֹUART�����ж�
  disable_irq(UART0SE_irq_no);          //��ֹ�жϿ�����IRQ�ж�
}

/*************************************************************************
*  UART0_IRQHandler
*  ����˵���������ж�
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2016-3-5
*  ��    ע��
*************************************************************************/
void UART0_IRQHandler(void)
{
  //uint16 count;
  uint8 recv;
  recv = uart_getchar();
  USART_RX_BUF[USART_RX_CNT++] =  recv;
  if (USART_RX_CNT>19) USART_RX_CNT = 0;
  if (recv == 's') 
  {
    SendFlag = 1;//���ͱ�־
  }
  else if (recv == 'S')
  {
    SStartFlag = 1;//�ٶ�PID���ձ�־
    USART_RX_CNT = 1;
  }
  else if (recv == 'D') 
  {
    DStartFlag = 1;//����PID
    USART_RX_CNT = 1;
  }
  else if  (recv == 'a')
  {
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 | PORT_PCR_MUX(1) | 0x02|PORT_PCR_IRQC(0) );
    MotorRun(0,BRAKING);
    ServoMotorChangeDegree(MotorParameter.MiddlePositionDuty);
  }
  if (USART_RX_CNT == 19 && SStartFlag == 1) { SComplishFlag = 1;}
  if (USART_RX_CNT == 19 && DStartFlag == 1) { DComplishFlag = 1;}
  /*if (recv == 'Z') {ZStartFlag = 1;}
  if (USART_RX_CNT == 19 && ZStartFlag == 1) { ZComplishFlag = 1;}*/
  JudgeReceive();
  //if (Res == 'A') {AStartFlag = 1;}
  //if (USART_RX_STA == 55 && AStartFlag == 1) { AComplishFlag = 1;}
}

void JudgeReceive()
{
  static int SP,SI,SD,DP,DI,DD;
  if(SendFlag)
  {
    USART_RX_CNT = 0;
    uart_putchar('s');
    SP = (int)(PIDVaule.SpeedControlP*10);
    SI = (int)(PIDVaule.SpeedControlI*10);
    SD = (int)(PIDVaule.SpeedControlD*10);    
    DP = (int)(PIDVaule.DirectionControlP*10);
    DI = (int)(PIDVaule.DirectionControlI*10);
    DD = (int)(PIDVaule.DirectionControlD*10); 
    printf ("%d ",SP);
    printf ("%d ",SI);
    printf ("%d ",SD);
    printf ("%d ",DP);
    printf ("%d ",DI);
    printf ("%d ",DD);
    SendFlag = 0;
  }
  //�յ��ٶ�PIDֵ
  if (SComplishFlag)
  {
     SStartFlag = 0;
     SComplishFlag = 0;
     Set_PID(&PIDVaule.SpeedControlP,&PIDVaule.SpeedControlI,&PIDVaule.SpeedControlD);
     USART_RX_CNT = 0;
     SP = (int)(PIDVaule.SpeedControlP*10);
     SI = (int)(PIDVaule.SpeedControlI*10);
     SD = (int)(PIDVaule.SpeedControlD*10);
     printf ("%d\r\n",SP);
     printf ("%d\r\n",SI);
     printf ("%d\r\n",SD);
  }
  //�յ�����PIDֵ
  if (DComplishFlag)
  {
     DStartFlag = 0;
     DComplishFlag = 0;
     Set_PID(&PIDVaule.DirectionControlP,&PIDVaule.DirectionControlI,&PIDVaule.DirectionControlD);
     DP = (int)(PIDVaule.DirectionControlP*10);
     DI = (int)(PIDVaule.DirectionControlI*10);
     DD = (int)(PIDVaule.DirectionControlD*10);
     printf ("%d\r\n",DP);
     printf ("%d\r\n",DI);
     printf ("%d\r\n",DD);
     USART_RX_CNT = 0;
  }
  /*if (ZComplishFlag)
  {
     ZStartFlag = 0;
     ZComplishFlag = 0;
// 		 printf ("%d ",USART_RX_STA);
// 		 printf ("%s",USART_RX_BUF);
     Set_PID(&PIDVaule.AngleControlP,&PIDVaule.AngleControlI,&PIDVaule.AngleControlD);
     AP = (int)(PIDVaule.AngleControlP*10);
     AI = (int)(PIDVaule.AngleControlI*10);
     AD = (int)(PIDVaule.AngleControlD*10);
     printf ("%d\r\n",AP);
     printf ("%d\r\n",AI);
     printf ("%d\r\n",AD);
     USART_RX_CNT = 0;
   }*/
}
/****************************/
/*���ݽ��յ�����������PIDֵ*/
/***************************/
void Set_PID(float * P,float * I,float * D)
{
	 int ge;
	 int shi;
	 int bai;
	 int qian;
	 int wang;
	 float xiaoshu;
	 char xiaoshuFlag = 0;
	 int i = 0;
	for (i = 1;i < 7;i++)
		 {
			 if (USART_RX_BUF[i] == '.')
				 {
					 xiaoshuFlag = 1;
				   i+=1;
					 break;
				 }
			 if (USART_RX_BUF[i] == ' ') break;
		}
		if (xiaoshuFlag == 1)
		{
				xiaoshuFlag = 0;
			xiaoshu = (USART_RX_BUF[i] - '0')*0.1;//С��ֵ
			i -= 2;//�õ�����λ��
			if (i == 1)
			{
				ge = USART_RX_BUF[i] - '0';
				*P = ge+xiaoshu;
			}
			else if (i == 2)
			{
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i] - '0')*10;
				*P = ge+shi+xiaoshu;
			}
			else if (i == 3)
			{
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i] - '0')*100;
				*P = ge+shi+bai+xiaoshu;
			}
			else
			{
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i] - '0')*1000;
				*P = ge+shi+bai+qian+xiaoshu;
			}
    }
			else
			{
				i -= 1;//�õ�����λ��
			if (i == 1)
			{
				ge = USART_RX_BUF[i] - '0';
				*P = (float)ge + 0.0;
			}
			else if (i == 2)
			{
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i] - '0')*10;
				*P = (float)ge+(float)shi+ 0.0;
			}
			else if (i == 3)
			{
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i] - '0')*100;
				*P = (float)ge+(float)shi+(float)bai+ 0.0;
			}
			else if (i == 4)
			{
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i] - '0')*1000;
				*P = (float)ge+(float)shi+(float)bai+(float)qian+ 0.0;
			}
			else
			{
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i--] - '0')*1000;
				wang = (USART_RX_BUF[i] - '0')*10000;
				*P = (float)ge+(float)shi+(float)bai+(float)qian+(float)wang+ 0.0;
			}
     }
		 for (i = 7;i < 13;i++)
		 {
			 if (USART_RX_BUF[i] == '.')
				 {
					 xiaoshuFlag = 1;
				   i+=1;
					 break;
				 }
			 if (USART_RX_BUF[i] == ' ') break;
		}
		if (xiaoshuFlag == 1)
		{
			xiaoshuFlag = 0;
			xiaoshu = (USART_RX_BUF[i] - '0')*0.1;
			i -= 8;
			if (i == 1)
			{
				i+=6;
				ge = USART_RX_BUF[i] - '0';
				*I = ge+xiaoshu;
			}
			else if (i == 2)
			{
				i+=6;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i] - '0')*10;
				*I = ge+shi+xiaoshu;
			}
			else if (i == 3)
			{
				i+=6;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i] - '0')*100;
				*I = ge+shi+bai+xiaoshu;
			}
			else
			{
				i+=6;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i] - '0')*1000;
				*I = ge+shi+bai+qian+xiaoshu;
			}
    }
			else
			{
				i -= 7;
			if (i == 1)
			{
				i+=6;
				ge = USART_RX_BUF[i] - '0';
				*I = (float)ge + 0.0;
			}
			else if (i == 2)
			{
				i+=6;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i] - '0')*10;
				*I = (float)ge+(float)shi+ 0.0;
			}
			else if (i == 3)
			{
				i+=6;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i] - '0')*100;
				*I = (float)ge+(float)shi+(float)bai+ 0.0;
			}
			else if (i == 4)
			{
				i+=6;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i] - '0')*1000;
				*I = (float)ge+(float)shi+(float)bai+(float)qian+ 0.0;
			}
			else
			{
				i+=6;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i--] - '0')*1000;
				wang = (USART_RX_BUF[i] - '0')*10000;
				*I = (float)ge+(float)shi+(float)bai+(float)qian+(float)wang+ 0.0;
			}
     }
		  for (i = 13;i < 19;i++)
		 {
			 if (USART_RX_BUF[i] == '.')
				 {
					 xiaoshuFlag = 1;
				   i+=1;
					 break;
				 }
			 if (USART_RX_BUF[i] == ' ') break;
		}
		if (xiaoshuFlag == 1)
		{
			xiaoshuFlag = 0;
			xiaoshu = (USART_RX_BUF[i] - '0')*0.1;
			i -= 14;
			if (i == 1)
			{
				i+=12;
				ge = USART_RX_BUF[i] - '0';
				*D = ge+xiaoshu;
			}
			else if (i == 2)
			{
				i+=12;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i] - '0')*10;
				*D = ge+shi+xiaoshu;
			}
			else if (i == 3)
			{
				i+=12;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i] - '0')*100;
				*D = ge+shi+bai+xiaoshu;
			}
			else
			{
				i+=12;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i] - '0')*1000;
				*D = ge+shi+bai+qian+xiaoshu;
			}
    }
			else
			{
				i -= 13;
			if (i == 1)
			{
				i+=12;
				ge = USART_RX_BUF[i] - '0';
				*D = (float)ge + 0.0;
			}
			else if (i == 2)
			{
				i+=12;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i] - '0')*10;
				*D = (float)ge+(float)shi+ 0.0;
			}
			else if (i == 3)
			{
				i+=12;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i] - '0')*100;
				*D = (float)ge+(float)shi+(float)bai+ 0.0;
			}
			else if (i == 4)
			{
				i+=12;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i] - '0')*1000;
				*D = (float)ge+(float)shi+(float)bai+(float)qian+ 0.0;
			}
			else
			{
				i+=12;
				ge = USART_RX_BUF[i--] - '0';
				shi =  (USART_RX_BUF[i--] - '0')*10;
				bai = (USART_RX_BUF[i--] - '0')*100;
				qian = (USART_RX_BUF[i--] - '0')*1000;
				wang = (USART_RX_BUF[i] - '0')*10000;
				*D = (float)ge+(float)shi+(float)bai+(float)qian+(float)wang+ 0.0;
			}
     }
}