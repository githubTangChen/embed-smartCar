/*****************************************
文 件 名：PredatorCamera.c
描    述：摄像头底层驱动
修改日期：2016-3-11
*****************************************/
#include "common.h"

extern unsigned char EndPosition;//提前结束的边界行坐标
extern uint8	  	SendImage;
extern uint8_t ServoMotorControlTimes;//一个周期舵机控制次数
extern unsigned char MidLineCompleteFlag;
unsigned char ThresholdValue = 0;
unsigned int V_Cnt = 0;//行中断触发计数变量
unsigned char H_Cnt = 0;//行计数变量
unsigned char CompleteFlag = 0;//图像采集完成标志
unsigned char BinCompleteFlag = 100;//行二值化完成标志
uint16_t  TimeCount = 0 ;
uint8_t TIMEflag_1000ms;
uint8_t TIMEflag_500ms;
//摄像头控制初始化函数
void Camera_Init(void)
{
  //IO口初始化
  //场信号输入口 PTA1 
  //行信号输入口 PTA17
  GPIO_Init(); 
  //设置中断优先级
  set_irq_priority(PortA_irq_no,0) ;
  //DMA初始化
  // 配置 DMA 触发源
  //PTA16
  DMA_Init();
  //设置中断优先级
  set_irq_priority(DMA0_irq_no,2) ;
}
//DMA初始化配置函数  
 inline void DMA_Init(void)
{
  /* 开启时钟 */
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;//打开DMA多路复用器时钟
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;//打开DMA模块时钟
  
  // DMA_CH0                                    
  DMAMUX0_CHCFG(0)=0x00;
  DMA_DSR_BCR(0)|=DMA_DSR_BCR_DONE_MASK;
  
  /* 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) */
  DMA_SAR(0) = (uint32)&(PTD_BASE_PTR->PDIRByte.Byte0);// 设置  源地址    
  DMA_DAR(0) = (uint32)PixData;// 设置目的地址
  
  //传输数目
  //一个主循环传输字节数
  DMA_DSR_BCR(0) = (0
                    | DMA_DSR_BCR_BCR(PICTURE_WIDTH)
                   );
  //配置DMA
  DMA_DCR(0) = (0
                | DMA_DCR_SSIZE(1)//源地址字节长度
                | DMA_DCR_DSIZE(1)//目的地址字节长度
             // | DMA_DCR_SINC_MASK //传输后源地址增加(根据位宽)
                | DMA_DCR_DINC_MASK //传输后目的地址增加(根据位宽)
                | DMA_DCR_CS_MASK // 0为不停得传输，直到BCR为0；1为一次请求传输一次
             // | DMA_DCR_AA_MASK
             // | DMA_DCR_START_MASK  //软件触发传输
                | DMA_DCR_ERQ_MASK //硬件触发传输（与上面START二选一）
                | DMA_DCR_D_REQ_MASK //传输完成后硬件自动清ERQ
                | DMA_DCR_EADREQ_MASK
            //  | DMA_DCR_SMOD(5) //传输目的地址预留256字节的位置
          //    | DMA_DCR_DMOD(5) //传输目的地址预留256字节的位置
               );
  
  /* 配置 DMA 触发源 */
  //PTA16
  DMAMUX_CHCFG_REG(DMAMUX0_BASE_PTR, 0) = (0
            | DMAMUX_CHCFG_SOURCE(49)/* 通道触发传输源:     */
                                );
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << 0);//开启PORTx端口
  PORT_PCR_REG(PORTA_BASE_PTR, 16) = (0x01 << PORT_PCR_MUX_SHIFT
                                     |0x02 << PORT_PCR_IRQC_SHIFT);//下降沿触发 ,上升沿触发为0x01

  //DMA_IRQ_CLEAN(0);
  //DMA_DSR_BCR(0)|= DMA_DSR_BCR_DONE_MASK;
}
//IO口初始化
 inline void GPIO_Init(void)
{
  /*****************/
  /*摄像头端口配置*/
  /*****************/
  
  
  //场信号输入口 PTA1
  PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x02            //输入下拉      
                                     |PORT_PCR_IRQC(0X0A) //下降沿中断
                                     );
  //设置端口方向为输入
   GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 1); 
  
  
  //行信号输入口 PTA17
  PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x02            //输入下拉      
                                     |PORT_PCR_IRQC(0X0A) //下降沿中断
                                     );
  //设置端口方向为输入
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 17); 
  
  /********************/
  /**摄像头数据口配置*/
  /*******************/
  //PTD0-7
  //根据带入参数pin,指定该引脚功能为GPIO功能（即令引脚控制寄存器的MUX=0b001）
  PORT_PCR_REG(PORTD_BASE_PTR, 0)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 0)= PORT_PCR_MUX(1);
  PORT_PCR_REG(PORTD_BASE_PTR, 1)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 1)= PORT_PCR_MUX(1);
  PORT_PCR_REG(PORTD_BASE_PTR, 2)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 2)= PORT_PCR_MUX(1);
  PORT_PCR_REG(PORTD_BASE_PTR, 3)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 3)= PORT_PCR_MUX(1);
  PORT_PCR_REG(PORTD_BASE_PTR, 4)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 4)= PORT_PCR_MUX(1);
  PORT_PCR_REG(PORTD_BASE_PTR, 5)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 5)= PORT_PCR_MUX(1);
  PORT_PCR_REG(PORTD_BASE_PTR, 6)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 6)= PORT_PCR_MUX(1);
  PORT_PCR_REG(PORTD_BASE_PTR, 7)=  0 ;
  PORT_PCR_REG(PORTD_BASE_PTR, 7)= PORT_PCR_MUX(1);
  //设置PTD0-7方向为输入
  GPIO_PDDR_REG(PTD_BASE_PTR) &= 0xFFFFFF00;  
}

//使用串口发送图像到上位机
void ImageSend()
{
  signed char i,j;
  if (1)
  {
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 | PORT_PCR_MUX(1) | 0x02|PORT_PCR_IRQC(0) );
    uart_putchar(0xff);
    for(i=0;i<PICTURE_HIGHT;i++)
    {
      for(j=0;j<PICTURE_WIDTH;j++)
      {
        if (PixData[i][j] == 0x01) PixData[i][j] = 0xfe;
        uart_putchar(PixData[i][j]);
      }   
    }
  }
}
                    
void OLEDPrintPicture(void)
{
  if (1)
  {
    OLED_PrintPicture();
  }
}

void DMA_CH0_Handler(void)
{
  static char j;
  DMA_DIS(0);
  DMA_IRQ_DIS(0);
  DMA_IRQ_CLEAN(0) ;
  //下面由用户添加实现代码
  //DMA_Over_Flg = 1 ;
  if(H_Cnt == PICTURE_HIGHT)
  {
    //PTB0_OUT = ~PTB0_OUT ; 
    for (j =0 ;j<PICTURE_WIDTH;j++)
    {
      if (PixData[59][j] < ThresholdValue)//设置阀值为
      {
        PixData[59][j] = 0;
      }
      else
      {
        PixData[59][j] = 1;
      }
    }
    PTB0_OUT = 0;
        //关闭中断
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 | PORT_PCR_MUX(1) | 0x02|PORT_PCR_IRQC(0) );
     CompleteFlag = 1;    
      //中线提取完成置0
      MidLineCompleteFlag = 0;
      //分段控制置0
      ServoMotorControlTimes = 0;
      CenterLineExtractionFullScan();
      //OLEDPrintPicture();
     
    /*for (j = 1;j < 8;j++)
    {
      OLED_PrintValueInt(75,j,MidPoints[EndPosition + j],2);
    }
    for (j = 0;j < 8;j++)
    {
      OLED_PrintValueInt(90,j,MidPoints[30+j],2);
    }*/
    ServoMotorSmoothControl(1);
    //MotorRun(120,FORWARD);
   // SpeedControl();
    //打开场中断
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 
                                       | PORT_PCR_MUX(1) 
                                       | 0x03            //输入下拉      
                                       |PORT_PCR_IRQC(0x0a) //下降沿中断
                                       );
    //设置端口方向为输入
    GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 1); 
    enable_irq(PortA_irq_no);
    
  }
  
}
//延时函数
void dlay()
{
  volatile unsigned char  dlaycount ;
  for(dlaycount = 0 ; dlaycount < 3 ; dlaycount ++)
  {
  }
}

//场与行中断服务函数
void PORTA_IRQHandler(void)
{
  static signed char i;
  static unsigned char j;
  //行中断PTA17
  if(PORTA_ISFR & (1 << 17))         //PTA17触发中断
  {
    PORTA_ISFR  |= (1 << 17);        //写1清中断标志位
    
    V_Cnt ++ ; 
    // uart_putchar(UART0,0xbb);
    if((V_Cnt%(480/PICTURE_HIGHT)==0)&&(H_Cnt<PICTURE_HIGHT))
    {    
      //dlay() ;
      DMA_IRQ_CLEAN(0);                             //清除通道传输中断标志位    (这样才能再次进入中断)
      DMA_SAR(0) = (uint32_t)(&PTD_BASE_PTR->PDIRByte.Byte0) ;  
      DMA_DAR(0) = (uint32_t)(&PixData[H_Cnt][0]);
      DMA_DSR_BCR(0) =( 0 | DMA_DSR_BCR_BCR(PICTURE_WIDTH))  ;            //传输数目  
      DMA_DCR(0) |= DMA_DCR_ERQ_MASK ;                  
      DMA_IRQ_EN(0) ;                             //允许DMA通道传输
      DMA_EN(0); //使能通道CHn 硬件请求      (这样才能继续触发DMA传输) 
      H_Cnt ++ ; 
    }
    else
    {
      i = H_Cnt - 1;
      if (i >= 0&& BinCompleteFlag != i)//同一行不要多次处理
      {
        for (j =0 ;j<PICTURE_WIDTH;j++)
        {
          if (PixData[i][j] < ThresholdValue)//设置阀值为
          {
            PixData[i][j] = 0;
            //BlackPointCount++;
          }
          else
          {
            PixData[i][j] = 1;
          }
        }
        BinCompleteFlag =i;//代表第i行已经处理完成
      }
      else//此处可用
      {
        for (j = 1 ;j<PICTURE_WIDTH - 2;j++)
        {
          if (PixData[i][j] == 0&&PixData[i][j - 1] == 1&&PixData[i][j + 1] == 1)//去噪点
          {
             PixData[i][j] = 1;
          }
          if (PixData[i][j] == 1&&PixData[i][j - 1] == 0&&PixData[i][j + 1] == 0)//去噪点
          {
             PixData[i][j] = 0;
          }
        }
      }
    }
  }
  //场中断
  if(PORTA_ISFR & (1 << 1))//PTA1触发中断
  {
  	TimeCount++;
	if(TimeCount%25==0)
	{
		TIMEflag_500ms=1;
	}
	if(TimeCount%100==0)
	{
		TIMEflag_1000ms=1;
	}
    PTB0_OUT = 1;
    PORTA_ISFR  |= (1 << 1);//写1清中断标志位    
    H_Cnt = 0 ;
    V_Cnt=0;
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x02            //输入下拉      
                                     |PORT_PCR_IRQC(0x09) //下降沿中断
                                     );
    //设置端口方向为输入
    GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 17); 
    enable_irq(PortA_irq_no);
    PORTA_ISFR  |= (1 << 17);//写1清中断标志位  
  }
}