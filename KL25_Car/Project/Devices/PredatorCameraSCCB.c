/*****************************************
文 件 名：PredatorCameraSCCB.c
描    述：摄像头SCCB操作
修改日期：2016-3-11
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
*                           蓝宙电子工作室
*
*  函数名称：BFDly_ms
*  功能说明：延时函数
*  参数说明：ms
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
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
*                           蓝宙电子工作室
*
*  函数名称：BFdelay_1us
*  功能说明：延时函数
*  参数说明：us
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
void BFdelay_1us(uint8_t us)                 //1us延时函数
{
  volatile uint8_t i ,j ;
  if(us < 1 )  us = 1 ;
  for(i=0;i<us;i++) 
  {
    for(j = 0 ;j < 15 ;j ++);
    
  }
  
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：iic_start
*  功能说明： 启动I2C总线子程序
*  参数说明：无
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
void iic_start(void)
{
  BFSDA = 1;  
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 1;
  BFdelay_1us(1);      // 延时5us 
  BFSDA = 0;
  BFdelay_1us(1);  
  BFCLK = 0;
  BFdelay_1us(2);
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：iic_stop
*  功能说明：停止I2C总线数据传送子程序
*  参数说明：无
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
void iic_stop(void)
{ 
  BFSDA = 0;   	   //时钟保持高，数据线从低到高一次跳变，I2C通信停止
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 1;
  BFdelay_1us(1);
  BFSDA = 1;
  BFdelay_1us(1);
  BFCLK = 0;
  BFdelay_1us(2);
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：slave_ACK
*  功能说明：从机发送应答位子程序
*  参数说明：无
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
void slave_ACK(void)
{
  BFSDA = 0; 
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 1;
  BFdelay_1us(1);			
  BFSDA = 1;
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 0;
  BFdelay_1us(2);
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：slave_NOACK
*  功能说明：从机发送非应答位子程序，迫使数据传输过程结束
*  参数说明：无
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
void slave_NOACK(void)
{ 
  BFSDA = 1;  
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 1;
  BFdelay_1us(3);
  BFSDA = 0;
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 0;
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：check_ACK
*  功能说明： 主机应答位检查子程序，迫使数据传输过程结束
*  参数说明：无
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
uint8_t check_ACK(void)
{ 
  uint8_t check ;
  
  BFSDA = 1; 
  //gpio_init (PORTE , 12, GPI,GPI_UP); 
  BFCLK = 0;
  BFDDRA = 0 ;
  BFSDAI = 1 ;
  
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 1;
  check = 0;
  if(BFSDAI == 1)    // 若BFSDA=1 表明非应答，置位非应答标志F0
    check = 1;
  BFdelay_1us(1);      // 延时1us 
  BFCLK = 0;
  //gpio_init (PORTE , 12, GPO,HIGH);      
  BFDDRA = 1 ;
  BFSDA = 1 ;
  return  check ;
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：IICSendByte
*  功能说明：发送一个字节
*  参数说明：ch
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
void IICSendByte(uint8_t ch)
{ 
  uint8_t n=8;     // 向BFSDA上发送一位数据字节，共八位
  while(n--)
  { 
    if((ch&0x80) == 0x80)    // 若要发送的数据最高位为1则发送位1
    {
      BFSDA = 1;    // 传送位1
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
      BFSDA = 0;    // 否则传送位0
      BFdelay_1us(1);
      BFCLK = 1;
      BFdelay_1us(2);
      BFCLK = 0;  
      BFdelay_1us(2);
    }
    ch = ch<<1;    // 数据左移一位
  }
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：IICreceiveByte
*  功能说明：接收一字节子程序
*  参数说明：无
*  函数返回：
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
uint8_t IICreceiveByte(void)
{
  uint8_t n=8;    // 从BFSDA线上读取一上数据字节，共八位
  uint8_t tdata = 0;
  while(n--)
  {
    
    BFSDAI = 1;
    BFdelay_1us(1);
    BFCLK=0;
    BFdelay_1us(2);
    BFCLK = 1;
    BFdelay_1us(2);
    tdata = tdata<<1;    // 左移一位，或_crol_(temp,1)
    if(BFSDAI == 1)
      tdata = tdata|0x01;    // 若接收到的位为1，则数据的最后一位置1
    else 
      tdata = tdata&0xfe;    // 否则数据的最后一位置0
    BFCLK=0;
  }
  return(tdata);
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：writeNbyte
*  功能说明：发送n位数据子程序
*  参数说明：slave_add从机地址,n要发送的数据个数
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
uint8_t writeNbyte(uint8_t slave_add, uint8_t *slave_data,uint16_t n)
{          
  uint8_t send_da,check = 1;
  uint16_t i=0;
  uint8_t *Send_databuff ;
  Send_databuff = slave_data ;
  //  uart_putchar(UART0,slave_data[0]);
  //   uart_putchar(UART0,n>>8);
  iic_start();                // 启动I2C
  IICSendByte(slave_add);     // 发送地址位
  check = check_ACK();                // 检查应答位
  //    uart_putchar(UART0,check);
  if(check == 1)
  { 
    return IICEorr;    // 若非应答表明器件错误或已坏，置错误标志位SystemError
  }
  while(n--)
  { 
    send_da = Send_databuff[i++];	
    IICSendByte(send_da);     
    check= check_ACK();    // 检查应答位
    //uart_putchar(UART0,send_da);
    if (check == 1)
    {
      return IICEorr;    // 若非应答表明器件错误或已坏，置错误标志位SystemError
    }
  }
  iic_stop();         // 全部发完则停止
  
  return IICOK;
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：receiveNbyte
*  功能说明：接收n位数据子程序
*  参数说明：slave_add从机地址,要接收的数据个数
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
uint8_t receiveNbyte(uint8_t slave_add,uint8_t *rece_data, uint16_t n)
{ 
  uint8_t receive_da,check;
  uint16_t i=1;
  uint8_t *rece_data_buff ;
  rece_data_buff = rece_data ;
  
  iic_start();
  IICSendByte(0XDC);
  BFdelay_1us(1);      // 延时1us 
  check =check_ACK();
  if(check == 1)
  {
    return IICEorr ;
  }
  IICSendByte(rece_data_buff[0]);
  BFdelay_1us(1);      // 延时1us 
  check =check_ACK();
  if(check == 1)
  {
    return IICEorr ;
  }
  // uart_putchar(UART0,rece_data_buff[0]);
  iic_start();
  IICSendByte(slave_add);
  BFdelay_1us(1);      // 延时1us 
  check =check_ACK();
  if(check == 1)
  {
    return IICEorr ;
  }
  
  for(;n > 1;n--)
  { 
    receive_da=IICreceiveByte();
    rece_data_buff[i++]=receive_da;
    slave_ACK();    // 收到一个字节后发送一个应答位
    
    //       uart_putchar(UART0,receive_da);
    
  }
  
  receive_da=IICreceiveByte();
  rece_data_buff[i++]=receive_da;
  slave_NOACK();    // 收到最后一个字节后发送一个非应答位
  //   uart_putchar(UART0,receive_da);
  iic_stop();
  return IICOK;
  
}

/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：IICWriteGpio_Init
*  功能说明：初始化程序PTC11   PTC10
*  参数说明：无
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
void IICWriteGpio_Init(void)
{
	//BFCLK PTC10
  //根据带入参数pin,指定该引脚功能为GPIO功能（即令引脚控制寄存器的MUX=0b001）
  PORT_PCR_REG(PORTC_BASE_PTR, 10)=  0 ;
  PORT_PCR_REG(PORTC_BASE_PTR, 10)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTC_BASE_PTR) |= (1 << 10);    //设置端口方向为输出
  GPIO_PDOR_REG(PTC_BASE_PTR) |=  (1 << 10); //设置输出为高电平 
  //BFSDA  PTC11
  //根据带入参数pin,指定该引脚功能为GPIO功能（即令引脚控制寄存器的MUX=0b001）
  PORT_PCR_REG(PORTC_BASE_PTR, 11)=  0 ;
  PORT_PCR_REG(PORTC_BASE_PTR, 11)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTC_BASE_PTR) |= (1 << 11);    //设置端口方向为输出
  GPIO_PDOR_REG(PTC_BASE_PTR) |=  (1 << 11); //设置输出为高电平 
  //gpio_init (PTC10, GPO,HIGH); 
  //gpio_init (PTC11, GPO,HIGH);
 // gpio_init (PTC12, GPO,HIGH);
}
/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：LandzoIICEEROM_INIT
*  功能说明：初始化程序
*  参数说明：无
*  函数返回：无
*  修改时间：2014-9-13
*  备    注：
*************************************************************************/
uint8_t LandzoIICEEROM_INIT(void)
{
  uint8_t LandzoOK ;
  uint16_t i ,EROMCont = 172 ;
  //uint8_t IIClandzo_buff[512] ;
  
  
//  IICWriteGpio_inint() ;    //初始化读入IIC端口
  for(i = 0 ; i < EROMCont ;)
  {
    LandzoOK = writeNbyte(0XDC,&LandzoRAM[i],2);
    if(!LandzoOK) break ;
    i = i +2 ;
  }
  
  BFDly_ms(100) ;                 //延迟100us
  
  if( !LandzoOK )
  {
    IICWriteGpio_Init() ;    //初始化读入IIC端口
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

