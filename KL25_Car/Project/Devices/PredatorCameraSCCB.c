/*****************************************
�� �� ����PredatorCameraSCCB.c
��    ��������ͷSCCB����
�޸����ڣ�2016-3-11
*****************************************/
#include "common.h"
uint8_t  LandzoRAM[ ] ={
   0X13,0X17,0Xa1,0X31,0Xa2,0X0b,0Xa3,0X3f,0Xa4,0X09,0Xa5,0X3f,0Xa6,0X04,0Xa7,0Xa5,
   0Xa8,0X1a,0Xa9,0X52,0Xaa,0X52,0Xab,0X18,0Xac,0X3c,0Xad,0Xf0,0X80,0X92,0X89,0X15,
   0X8a,0X36,0X8b,0X04,0Xc0,0X80,0Xc1,0X00,0Xe2,0X00,0Xe3,0X00,0X12,0X01,0X3a,0X03,
   0X0c,0X00,0X15,0X02,0X35,0X46,0X36,0X01,0X37,0Xf7,0X38,0X45,0X65,0X46,0X66,0X46,
   0X40,0X50,0X41,0X4A,0X42,0X31,0X43,0X1F,0X44,0X15,0X45,0X10,0X46,0X0E,0X47,0X0D,
   0X48,0X0C,0X49,0X0B,0X4b,0X0a,0X4c,0X09,0X4e,0X09,0X4f,0X08,0X50,0X07,0X39,0X80,
   0X3f,0Xa0,0X0a,0Xa0,0X0b,0X0c,0X3b,0X60,0X3e,0Xa4,0X70,0X0a,0X72,0X0F,0X73,0X4F,
   0X74,0X2f,0X75,0X0e,0Xb0,0X94,0Xb3,0X84,0X59,0X0e,0X5a,0X18,0X13,0X17,0x0c,0x00,
   0X1b,0X40,0X11,0X00,0X2d,0X10,0X2e,0X55,0Xf0,0X00,0Xf1,0X00,0X69,0X60,0X1e,0X00,
   0X20,0X02,0X09,0X03,0XB1,0XC6,0XB2,0XCC,0X24,0X82,0X25,0X88,0X55,0X00,0X56,0X40,
   0Xc0,0X80,0Xc1,0X00,0X17,0X00,0X18,0Xa0,0X19,0X00,0X1a,0X78,
   };


/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�BFDly_ms
*  ����˵������ʱ����
*  ����˵����ms
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/

void BFDly_ms(uint8_t ms)
{
  volatile uint16_t ii,jj,n;
  if (ms<1) ms=1;
  for(ii=0;ii<ms;ii++)
    //  for(jj=0;jj<1335;jj++);     //16MHz--1ms
    //for(jj=0;jj<4005;jj++);    //48MHz--1ms  
    //for(jj=0;jj<5341;jj++);    //64MHz--1ms  
    for(jj=0;jj<18200;jj++);     //200MHz--1ms  
} 

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�BFdelay_1us
*  ����˵������ʱ����
*  ����˵����us
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void BFdelay_1us(uint8_t us)                 //1us��ʱ����
{
  volatile uint8_t i ,j ;
  if(us < 1 )  us = 1 ;
  for(i=0;i<us;i++) 
  {
    for(j = 0 ;j < 15 ;j ++);
    
  }
  
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�iic_start
*  ����˵���� ����I2C�����ӳ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void iic_start(void)
{
  BFSDA = 1;  
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 1;
  BFdelay_1us(1);      // ��ʱ5us 
  BFSDA = 0;
  BFdelay_1us(1);  
  BFCLK = 0;
  BFdelay_1us(2);
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�iic_stop
*  ����˵����ֹͣI2C�������ݴ����ӳ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void iic_stop(void)
{ 
  BFSDA = 0;   	   //ʱ�ӱ��ָߣ������ߴӵ͵���һ�����䣬I2Cͨ��ֹͣ
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 1;
  BFdelay_1us(1);
  BFSDA = 1;
  BFdelay_1us(1);
  BFCLK = 0;
  BFdelay_1us(2);
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�slave_ACK
*  ����˵�����ӻ�����Ӧ��λ�ӳ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void slave_ACK(void)
{
  BFSDA = 0; 
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 1;
  BFdelay_1us(1);			
  BFSDA = 1;
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 0;
  BFdelay_1us(2);
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�slave_NOACK
*  ����˵�����ӻ����ͷ�Ӧ��λ�ӳ�����ʹ���ݴ�����̽���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void slave_NOACK(void)
{ 
  BFSDA = 1;  
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 1;
  BFdelay_1us(3);
  BFSDA = 0;
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 0;
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�check_ACK
*  ����˵���� ����Ӧ��λ����ӳ�����ʹ���ݴ�����̽���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
uint8_t check_ACK(void)
{ 
  uint8_t check ;
  
  BFSDA = 1; 
  //gpio_init (PORTE , 12, GPI,GPI_UP); 
  BFCLK = 0;
  BFDDRA = 0 ;
  BFSDAI = 1 ;
  
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 1;
  check = 0;
  if(BFSDAI == 1)    // ��BFSDA=1 ������Ӧ����λ��Ӧ���־F0
    check = 1;
  BFdelay_1us(1);      // ��ʱ1us 
  BFCLK = 0;
  //gpio_init (PORTE , 12, GPO,HIGH);      
  BFDDRA = 1 ;
  BFSDA = 1 ;
  return  check ;
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�IICSendByte
*  ����˵��������һ���ֽ�
*  ����˵����ch
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void IICSendByte(uint8_t ch)
{ 
  uint8_t n=8;     // ��BFSDA�Ϸ���һλ�����ֽڣ�����λ
  while(n--)
  { 
    if((ch&0x80) == 0x80)    // ��Ҫ���͵��������λΪ1����λ1
    {
      BFSDA = 1;    // ����λ1
      BFdelay_1us(1);
      BFCLK = 1;
      BFdelay_1us(2);
      BFCLK = 0;  
      BFdelay_1us(1);
      BFSDA = 0;
      BFdelay_1us(1);
      
    }
    else
    {  
      BFSDA = 0;    // ������λ0
      BFdelay_1us(1);
      BFCLK = 1;
      BFdelay_1us(2);
      BFCLK = 0;  
      BFdelay_1us(2);
    }
    ch = ch<<1;    // ��������һλ
  }
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�IICreceiveByte
*  ����˵��������һ�ֽ��ӳ���
*  ����˵������
*  �������أ�
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
uint8_t IICreceiveByte(void)
{
  uint8_t n=8;    // ��BFSDA���϶�ȡһ�������ֽڣ�����λ
  uint8_t tdata = 0;
  while(n--)
  {
    
    BFSDAI = 1;
    BFdelay_1us(1);
    BFCLK=0;
    BFdelay_1us(2);
    BFCLK = 1;
    BFdelay_1us(2);
    tdata = tdata<<1;    // ����һλ����_crol_(temp,1)
    if(BFSDAI == 1)
      tdata = tdata|0x01;    // �����յ���λΪ1�������ݵ����һλ��1
    else 
      tdata = tdata&0xfe;    // �������ݵ����һλ��0
    BFCLK=0;
  }
  return(tdata);
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�writeNbyte
*  ����˵��������nλ�����ӳ���
*  ����˵����slave_add�ӻ���ַ,nҪ���͵����ݸ���
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
uint8_t writeNbyte(uint8_t slave_add, uint8_t *slave_data,uint16_t n)
{          
  uint8_t send_da,check = 1;
  uint16_t i=0;
  uint8_t *Send_databuff ;
  Send_databuff = slave_data ;
  //  uart_putchar(UART0,slave_data[0]);
  //   uart_putchar(UART0,n>>8);
  iic_start();                // ����I2C
  IICSendByte(slave_add);     // ���͵�ַλ
  check = check_ACK();                // ���Ӧ��λ
  //    uart_putchar(UART0,check);
  if(check == 1)
  { 
    return IICEorr;    // ����Ӧ���������������ѻ����ô����־λSystemError
  }
  while(n--)
  { 
    send_da = Send_databuff[i++];	
    IICSendByte(send_da);     
    check= check_ACK();    // ���Ӧ��λ
    //uart_putchar(UART0,send_da);
    if (check == 1)
    {
      return IICEorr;    // ����Ӧ���������������ѻ����ô����־λSystemError
    }
  }
  iic_stop();         // ȫ��������ֹͣ
  
  return IICOK;
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�receiveNbyte
*  ����˵��������nλ�����ӳ���
*  ����˵����slave_add�ӻ���ַ,Ҫ���յ����ݸ���
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
uint8_t receiveNbyte(uint8_t slave_add,uint8_t *rece_data, uint16_t n)
{ 
  uint8_t receive_da,check;
  uint16_t i=1;
  uint8_t *rece_data_buff ;
  rece_data_buff = rece_data ;
  
  iic_start();
  IICSendByte(0XDC);
  BFdelay_1us(1);      // ��ʱ1us 
  check =check_ACK();
  if(check == 1)
  {
    return IICEorr ;
  }
  IICSendByte(rece_data_buff[0]);
  BFdelay_1us(1);      // ��ʱ1us 
  check =check_ACK();
  if(check == 1)
  {
    return IICEorr ;
  }
  // uart_putchar(UART0,rece_data_buff[0]);
  iic_start();
  IICSendByte(slave_add);
  BFdelay_1us(1);      // ��ʱ1us 
  check =check_ACK();
  if(check == 1)
  {
    return IICEorr ;
  }
  
  for(;n > 1;n--)
  { 
    receive_da=IICreceiveByte();
    rece_data_buff[i++]=receive_da;
    slave_ACK();    // �յ�һ���ֽں���һ��Ӧ��λ
    
    //       uart_putchar(UART0,receive_da);
    
  }
  
  receive_da=IICreceiveByte();
  rece_data_buff[i++]=receive_da;
  slave_NOACK();    // �յ����һ���ֽں���һ����Ӧ��λ
  //   uart_putchar(UART0,receive_da);
  iic_stop();
  return IICOK;
  
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�IICWriteGpio_Init
*  ����˵������ʼ������PTC11   PTC10
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void IICWriteGpio_Init(void)
{
	//BFCLK PTC10
  //���ݴ������pin,ָ�������Ź���ΪGPIO���ܣ��������ſ��ƼĴ�����MUX=0b001��
  PORT_PCR_REG(PORTC_BASE_PTR, 10)=  0 ;
  PORT_PCR_REG(PORTC_BASE_PTR, 10)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTC_BASE_PTR) |= (1 << 10);    //���ö˿ڷ���Ϊ���
  GPIO_PDOR_REG(PTC_BASE_PTR) |=  (1 << 10); //�������Ϊ�ߵ�ƽ 
  //BFSDA  PTC11
  //���ݴ������pin,ָ�������Ź���ΪGPIO���ܣ��������ſ��ƼĴ�����MUX=0b001��
  PORT_PCR_REG(PORTC_BASE_PTR, 11)=  0 ;
  PORT_PCR_REG(PORTC_BASE_PTR, 11)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTC_BASE_PTR) |= (1 << 11);    //���ö˿ڷ���Ϊ���
  GPIO_PDOR_REG(PTC_BASE_PTR) |=  (1 << 11); //�������Ϊ�ߵ�ƽ 
  //gpio_init (PTC10, GPO,HIGH); 
  //gpio_init (PTC11, GPO,HIGH);
 // gpio_init (PTC12, GPO,HIGH);
}
/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�LandzoIICEEROM_INIT
*  ����˵������ʼ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
uint8_t LandzoIICEEROM_INIT(void)
{
  uint8_t LandzoOK ;
  uint16_t i ,EROMCont = 172 ;
  //uint8_t IIClandzo_buff[512] ;
  
  
//  IICWriteGpio_inint() ;    //��ʼ������IIC�˿�
  for(i = 0 ; i < EROMCont ;)
  {
    LandzoOK = writeNbyte(0XDC,&LandzoRAM[i],2);
    if(!LandzoOK) break ;
    i = i +2 ;
  }
  
  BFDly_ms(100) ;                 //�ӳ�100us
  
  if( !LandzoOK )
  {
    IICWriteGpio_Init() ;    //��ʼ������IIC�˿�
    for(i = 0 ; i < EROMCont ;)
    {
      LandzoOK = writeNbyte(0XDC,&LandzoRAM[i],2);
      if(!LandzoOK) break ;
      i = i +2 ;
    }
  }
  
  if(!LandzoOK)
  {return IICEorr;}
  return  IICOK ;
}

