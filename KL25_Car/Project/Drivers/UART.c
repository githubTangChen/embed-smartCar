/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
* 文件名       ： adc.c
* 描述         ：工程模版实验
*
* 实验平台     ：landzo电子开发版
* 库版本       ：
* 嵌入系统     ：
*
* 作者         ：landzo 蓝电子
* 淘宝店       ：http://landzo.taobao.com/

**********************************************************************************/
#include "common.h"   //包含公共要素头文件
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
uint16 USART_RX_CNT = 0; //接收字符计数    

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_init
*  功能说明：初始化uart模块
*  参数说明：
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程，printf会调用这函数
*************************************************************************/
 //UART0  //UART0_RX  PTB16 /UART0_TX  PTB17
void UART_Init (uint32_t baud_rate)
{
  //局部变量声明
  register uint_16 sbr;
  uint8_t temp;
  uint16_t  clk_kh ;
  
  //根据带入参数uartNo，给局部变量uartch1赋值
    
    
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //使能 UART0 时钟
    
    //UART0_RX  PTB16
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << 1); //开启PORTx端口
    PORT_PCR_REG(PORTB_BASE_PTR, 16) = 0x03 << PORT_PCR_MUX_SHIFT; // 复用功能 , 确定触发模式 ,开启上拉或下拉电阻
    //UART0_TX  PTB17
    PORT_PCR_REG(PORTB_BASE_PTR, 17) = 0x03 << PORT_PCR_MUX_SHIFT;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);   //UART0选择MCGFLLCLK_khz=48000Khz时钟源
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    
    //暂时关闭串口0发送与接收功能
    UART0_BASE_PTR->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    
    //配置串口工作模式:8位无校验模式
    clk_kh = core_clk_khz/2 ;
    sbr = (uint_16)((clk_kh*1000)/(baud_rate * 16));
    temp = UART_BDH_REG(UART0_BASE_PTR) & ~(UART_BDH_SBR(0x1F));
    UART_BDH_REG(UART0_BASE_PTR) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(UART0_BASE_PTR) = (uint8_t)(sbr & UART_BDL_SBR_MASK);
    
    //初始化控制寄存器、清标志位
    UART0_C4 = 0x0F;
    UART0_C1 = 0x00;
    UART0_C3 = 0x00;
    UART0_MA1 = 0x00;
    UART0_MA2 = 0x00;
    UART0_S1 |= 0x1F;
    UART0_S2 |= 0xC0;
    
    //启动发送接收
    UART0_BASE_PTR->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_getchar
*  功能说明：无限时间等待串口接受一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：接收到的字节
*  修改时间：2012-1-20
*  备    注：官方例程
*************************************************************************/
char uart_getchar (void)
{
  /* Wait until character has been received */
  while (!((UART0_BASE_PTR->S1) & UARTLP_S1_RDRF_MASK ));
  /* Return the 8-bit data from the receiver */
  return UART0_BASE_PTR->D;
}
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_putchar
*  功能说明：串口发送一个字节
*  参数说明：UARTn       模块号（UART0~UART5）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程，printf会调用这函数
*************************************************************************/
void uart_putchar (char ch)
{
  while(!((UART0_BASE_PTR->S1) & UARTLP_S1_TDRE_MASK));
  //发送数据
  UART0_BASE_PTR->D = ch;
}
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_irq_EN
*  功能说明：开串口接收中断
*  参数说明：UARTn       模块号（UART0~UART2）
*  函数返回：无
*  修改时间：2014-9-17
*  备    注：
*************************************************************************/
void uart_irq_EN(void)
{
  UART0_BASE_PTR->C2 |= UART_C2_RIE_MASK;        //开放UART接收中断
  enable_irq(UART0SE_irq_no);   //开中断控制器IRQ中断
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：uart_irq_DIS
*  功能说明：关串口接收中断
*  参数说明：
*  函数返回：无
*  修改时间：2014-9-17
*  备    注：
*************************************************************************/
void uart_irq_DIS(void)
{
  UART0_BASE_PTR->C2 &= ~UART_C2_RIE_MASK;               //禁止UART接收中断
  disable_irq(UART0SE_irq_no);          //禁止中断控制器IRQ中断
}

/*************************************************************************
*  UART0_IRQHandler
*  功能说明：接收中断
*  参数说明：
*  函数返回：无
*  修改时间：2016-3-5
*  备    注：
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
    SendFlag = 1;//发送标志
  }
  else if (recv == 'S')
  {
    SStartFlag = 1;//速度PID接收标志
    USART_RX_CNT = 1;
  }
  else if (recv == 'D') 
  {
    DStartFlag = 1;//方向PID
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
  //收到速度PID值
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
  //收到方向PID值
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
/*根据接收到的数据设置PID值*/
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
			xiaoshu = (USART_RX_BUF[i] - '0')*0.1;//小数值
			i -= 2;//得到整数位数
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
				i -= 1;//得到整数位数
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