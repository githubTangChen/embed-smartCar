/*****************************************
�� �� ����PredatorCamera.c
��    ��������ͷ�ײ�����
�޸����ڣ�2016-3-11
*****************************************/
#include "common.h"

extern unsigned char EndPosition;//��ǰ�����ı߽�������
extern uint8	  	SendImage;
extern uint8_t ServoMotorControlTimes;//һ�����ڶ�����ƴ���
extern unsigned char MidLineCompleteFlag;
unsigned char ThresholdValue = 0;
unsigned int V_Cnt = 0;//���жϴ�����������
unsigned char H_Cnt = 0;//�м�������
unsigned char CompleteFlag = 0;//ͼ��ɼ���ɱ�־
unsigned char BinCompleteFlag = 100;//�ж�ֵ����ɱ�־
uint16_t  TimeCount = 0 ;
uint8_t TIMEflag_1000ms;
uint8_t TIMEflag_500ms;
//����ͷ���Ƴ�ʼ������
void Camera_Init(void)
{
  //IO�ڳ�ʼ��
  //���ź������ PTA1 
  //���ź������ PTA17
  GPIO_Init(); 
  //�����ж����ȼ�
  set_irq_priority(PortA_irq_no,0) ;
  //DMA��ʼ��
  // ���� DMA ����Դ
  //PTA16
  DMA_Init();
  //�����ж����ȼ�
  set_irq_priority(DMA0_irq_no,2) ;
}
//DMA��ʼ�����ú���  
 inline void DMA_Init(void)
{
  /* ����ʱ�� */
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;//��DMA��·������ʱ��
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;//��DMAģ��ʱ��
  
  // DMA_CH0                                    
  DMAMUX0_CHCFG(0)=0x00;
  DMA_DSR_BCR(0)|=DMA_DSR_BCR_DONE_MASK;
  
  /* ���� DMA ͨ�� �� ������ƿ� TCD ( Transfer Control Descriptor ) */
  DMA_SAR(0) = (uint32)&(PTD_BASE_PTR->PDIRByte.Byte0);// ����  Դ��ַ    
  DMA_DAR(0) = (uint32)PixData;// ����Ŀ�ĵ�ַ
  
  //������Ŀ
  //һ����ѭ�������ֽ���
  DMA_DSR_BCR(0) = (0
                    | DMA_DSR_BCR_BCR(PICTURE_WIDTH)
                   );
  //����DMA
  DMA_DCR(0) = (0
                | DMA_DCR_SSIZE(1)//Դ��ַ�ֽڳ���
                | DMA_DCR_DSIZE(1)//Ŀ�ĵ�ַ�ֽڳ���
             // | DMA_DCR_SINC_MASK //�����Դ��ַ����(����λ��)
                | DMA_DCR_DINC_MASK //�����Ŀ�ĵ�ַ����(����λ��)
                | DMA_DCR_CS_MASK // 0Ϊ��ͣ�ô��䣬ֱ��BCRΪ0��1Ϊһ��������һ��
             // | DMA_DCR_AA_MASK
             // | DMA_DCR_START_MASK  //�����������
                | DMA_DCR_ERQ_MASK //Ӳ���������䣨������START��ѡһ��
                | DMA_DCR_D_REQ_MASK //������ɺ�Ӳ���Զ���ERQ
                | DMA_DCR_EADREQ_MASK
            //  | DMA_DCR_SMOD(5) //����Ŀ�ĵ�ַԤ��256�ֽڵ�λ��
          //    | DMA_DCR_DMOD(5) //����Ŀ�ĵ�ַԤ��256�ֽڵ�λ��
               );
  
  /* ���� DMA ����Դ */
  //PTA16
  DMAMUX_CHCFG_REG(DMAMUX0_BASE_PTR, 0) = (0
            | DMAMUX_CHCFG_SOURCE(49)/* ͨ����������Դ:     */
                                );
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << 0);//����PORTx�˿�
  PORT_PCR_REG(PORTA_BASE_PTR, 16) = (0x01 << PORT_PCR_MUX_SHIFT
                                     |0x02 << PORT_PCR_IRQC_SHIFT);//�½��ش��� ,�����ش���Ϊ0x01

  //DMA_IRQ_CLEAN(0);
  //DMA_DSR_BCR(0)|= DMA_DSR_BCR_DONE_MASK;
}
//IO�ڳ�ʼ��
 inline void GPIO_Init(void)
{
  /*****************/
  /*����ͷ�˿�����*/
  /*****************/
  
  
  //���ź������ PTA1
  PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x02            //��������      
                                     |PORT_PCR_IRQC(0X0A) //�½����ж�
                                     );
  //���ö˿ڷ���Ϊ����
   GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 1); 
  
  
  //���ź������ PTA17
  PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x02            //��������      
                                     |PORT_PCR_IRQC(0X0A) //�½����ж�
                                     );
  //���ö˿ڷ���Ϊ����
  GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 17); 
  
  /********************/
  /**����ͷ���ݿ�����*/
  /*******************/
  //PTD0-7
  //���ݴ������pin,ָ�������Ź���ΪGPIO���ܣ��������ſ��ƼĴ�����MUX=0b001��
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
  //����PTD0-7����Ϊ����
  GPIO_PDDR_REG(PTD_BASE_PTR) &= 0xFFFFFF00;  
}

//ʹ�ô��ڷ���ͼ����λ��
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
  //�������û����ʵ�ִ���
  //DMA_Over_Flg = 1 ;
  if(H_Cnt == PICTURE_HIGHT)
  {
    //PTB0_OUT = ~PTB0_OUT ; 
    for (j =0 ;j<PICTURE_WIDTH;j++)
    {
      if (PixData[59][j] < ThresholdValue)//���÷�ֵΪ
      {
        PixData[59][j] = 0;
      }
      else
      {
        PixData[59][j] = 1;
      }
    }
    PTB0_OUT = 0;
        //�ر��ж�
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 | PORT_PCR_MUX(1) | 0x03|PORT_PCR_IRQC(0) );
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 | PORT_PCR_MUX(1) | 0x02|PORT_PCR_IRQC(0) );
     CompleteFlag = 1;    
      //������ȡ�����0
      MidLineCompleteFlag = 0;
      //�ֶο�����0
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
    //�򿪳��ж�
    PORT_PCR_REG(PORTA_BASE_PTR, 1) = (0 
                                       | PORT_PCR_MUX(1) 
                                       | 0x03            //��������      
                                       |PORT_PCR_IRQC(0x0a) //�½����ж�
                                       );
    //���ö˿ڷ���Ϊ����
    GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 1); 
    enable_irq(PortA_irq_no);
    
  }
  
}
//��ʱ����
void dlay()
{
  volatile unsigned char  dlaycount ;
  for(dlaycount = 0 ; dlaycount < 3 ; dlaycount ++)
  {
  }
}

//�������жϷ�����
void PORTA_IRQHandler(void)
{
  static signed char i;
  static unsigned char j;
  //���ж�PTA17
  if(PORTA_ISFR & (1 << 17))         //PTA17�����ж�
  {
    PORTA_ISFR  |= (1 << 17);        //д1���жϱ�־λ
    
    V_Cnt ++ ; 
    // uart_putchar(UART0,0xbb);
    if((V_Cnt%(480/PICTURE_HIGHT)==0)&&(H_Cnt<PICTURE_HIGHT))
    {    
      //dlay() ;
      DMA_IRQ_CLEAN(0);                             //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
      DMA_SAR(0) = (uint32_t)(&PTD_BASE_PTR->PDIRByte.Byte0) ;  
      DMA_DAR(0) = (uint32_t)(&PixData[H_Cnt][0]);
      DMA_DSR_BCR(0) =( 0 | DMA_DSR_BCR_BCR(PICTURE_WIDTH))  ;            //������Ŀ  
      DMA_DCR(0) |= DMA_DCR_ERQ_MASK ;                  
      DMA_IRQ_EN(0) ;                             //����DMAͨ������
      DMA_EN(0); //ʹ��ͨ��CHn Ӳ������      (�������ܼ�������DMA����) 
      H_Cnt ++ ; 
    }
    else
    {
      i = H_Cnt - 1;
      if (i >= 0&& BinCompleteFlag != i)//ͬһ�в�Ҫ��δ���
      {
        for (j =0 ;j<PICTURE_WIDTH;j++)
        {
          if (PixData[i][j] < ThresholdValue)//���÷�ֵΪ
          {
            PixData[i][j] = 0;
            //BlackPointCount++;
          }
          else
          {
            PixData[i][j] = 1;
          }
        }
        BinCompleteFlag =i;//�����i���Ѿ��������
      }
      else//�˴�����
      {
        for (j = 1 ;j<PICTURE_WIDTH - 2;j++)
        {
          if (PixData[i][j] == 0&&PixData[i][j - 1] == 1&&PixData[i][j + 1] == 1)//ȥ���
          {
             PixData[i][j] = 1;
          }
          if (PixData[i][j] == 1&&PixData[i][j - 1] == 0&&PixData[i][j + 1] == 0)//ȥ���
          {
             PixData[i][j] = 0;
          }
        }
      }
    }
  }
  //���ж�
  if(PORTA_ISFR & (1 << 1))//PTA1�����ж�
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
    PORTA_ISFR  |= (1 << 1);//д1���жϱ�־λ    
    H_Cnt = 0 ;
    V_Cnt=0;
    PORT_PCR_REG(PORTA_BASE_PTR, 17) = (0 
                                     | PORT_PCR_MUX(1) 
                                     | 0x02            //��������      
                                     |PORT_PCR_IRQC(0x09) //�½����ж�
                                     );
    //���ö˿ڷ���Ϊ����
    GPIO_PDDR_REG(PTA_BASE_PTR) &= ~(1 << 17); 
    enable_irq(PortA_irq_no);
    PORTA_ISFR  |= (1 << 17);//д1���жϱ�־λ  
  }
}