/*****************************************
�� �� ����OLED.c
��    ����OLED�ײ�����
�޸����ڣ�2016-3-11
*****************************************/
#include "PredatorCamera.h"
#include "OLED.h"
//���ؿ��
extern unsigned char PixData[PICTURE_HIGHT][PICTURE_WIDTH];
/************************************************************************
OLED һ�п�����ʾ16���ַ���������ʾ8�С�
************************************************************************/

#define OLED_SCL  PTE_BASE_PTR->PDORs.PDOR2
#define OLED_SDA  PTE_BASE_PTR->PDORs.PDOR1
#define OLED_RST  PTE_BASE_PTR->PDORs.PDOR5
#define OLED_DC   PTE_BASE_PTR->PDORs.PDOR3
#define OLED_CS   PTE_BASE_PTR->PDORs.PDOR4

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF

/*
4��SPIʹ��˵����
VBT ���ڲ�DC-DC��ѹ��3.3~4.3V�����ʹ��5V��ѹ��Ϊ���������һ��100~500ŷ�ĵ���
VCC ���ڲ��߼���ѹ 1.8~6V
GND ��

BS0 �͵�ƽ
BS1 �͵�ƽ
BS2 �͵�ƽ

CS  Ƭѡ�ܽ�
DC  ��������ѡ��ܽ�
RES ģ�鸴λ�ܽ�
D0��SCLK�� ��ʱ�ӽţ���MCU����
D1��MOSI�� ����������������ݽţ���MCU����

D2 ����
D3-D7 �� �͵�ƽ �� Ҳ�����գ��������Ϊ�͵�ƽ
RD  �͵�ƽ ��Ҳ�����գ��������Ϊ�͵�ƽ
RW  �͵�ƽ ��Ҳ�����գ��������Ϊ�͵�ƽ
RD  �͵�ƽ ��Ҳ�����գ��������Ϊ�͵�ƽ
*/

#define X_WIDTH 128
#define Y_WIDTH 64
//======================================
const uint8_t F6x8[][6] =
{
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
  { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
  { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
  { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
  { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
  { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
  { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
  { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
  { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
  { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
  { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
  { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
  { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
  { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
  { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
  { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
  { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
  { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
  { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
  { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
  { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
  { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
  { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
  { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
  { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
  { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
  { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
  { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
  { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
  { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
  { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
  { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
  { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
  { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
  { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
  { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
  { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
  { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
  { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
  { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
  { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
  { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
  { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
  { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
  { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
  { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
  { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
  { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
  { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
  { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
  { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
  { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
  { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
  { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
  { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
  { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
  { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
  { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
  { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
  { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
  { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
  { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
  { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
  { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
  { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
  { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
  { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
  { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
  { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
  { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
  { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
  { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
  { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
  { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
  { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
  { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
  { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
  { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
  { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
  { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
  { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
  { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
  { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
  { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
  { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
  { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
  { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
  { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
  { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
  { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
  { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
  { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};
/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�Dly_ms
*  ����˵������ʱ����
*  ����˵���� ms
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void Dly_ms(uint16_t ms)
{
  uint16_t ii,jj;
  if (ms<1) ms=1;
  ms = ms*12 ;
  for(ii=0;ii<ms;ii++)
    for(jj=0;jj<1335;jj++);  //16MHz--1ms
  //   for(jj=0;jj<4006;jj++);  //48MHz--1ms
  //for(jj=0;jj<5341;jj++);    //64MHz--1ms
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_WrDat
*  ����˵����д���ݺ���
*  ����˵���� data
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_WrDat(uint8_t data)
{
  uint8_t i=8;
  
  OLED_DC=1;
  OLED_SCL=0;
  while(i--)
  {
    if(data&0x80){OLED_SDA=1;}
    else{OLED_SDA=0;}
    OLED_SCL=1;
    asm("nop");
    OLED_SCL=0;
    data<<=1;
  }
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_Wr6Dat
*  ����˵����д��6λ�ݺ���
*  ����˵���� data
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_Wr6Dat(uint8_t data)
{
  uint8_t i=6;
  
  OLED_DC=1;
  OLED_SCL=0;
  while(i--)
  {
    if(data&0x80){OLED_SDA=1;}
    else{OLED_SDA=0;}
    OLED_SCL=1;
    asm("nop");
    OLED_SCL=0;
    data<<=1;
  }
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_WrCmd
*  ����˵����д�������
*  ����˵���� cmd
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_WrCmd(uint8_t cmd)
{
  uint8_t i=8;

  OLED_DC=0;;
  OLED_SCL=0;;

  while(i--)
  {
    if(cmd&0x80){OLED_SDA=1;}
    else{OLED_SDA=0;}
    OLED_SCL=1;
    asm("nop");

    OLED_SCL=0;
    cmd<<=1;
  }
  
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_Set_Pos
*  ����˵��������λ�ú���
*  ����˵���� x
*             y
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
  OLED_WrCmd(0xb0+y);
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd((x&0x0f)|0x01);
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_Fill
*  ����˵������亯��
*  ����˵���� bmp_data   
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_Fill(uint8_t bmp_data)
{
  uint8_t y,x;
  
  for(y=0;y<8;y++)
  {
    OLED_WrCmd(0xb0+y);
    OLED_WrCmd(0x01);
    OLED_WrCmd(0x10);
    for(x=0;x<X_WIDTH;x++)
      OLED_WrDat(bmp_data);
  }
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_CLS
*  ����˵�����������
*  ����˵���� �� 
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_CLS(void)
{
  uint8_t y,x;
  for(y=0;y<8;y++)
  {
    OLED_WrCmd(0xb0+y);
    OLED_WrCmd(0x01);
    OLED_WrCmd(0x10);
    for(x=0;x<X_WIDTH;x++)
      OLED_WrDat(0);
  }
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_DLY_ms
*  ����˵������ʱ����
*  ����˵����ms
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_DLY_ms(uint16_t ms)
{
  uint16_t a;
  while(ms)
  {
   // a=10000;
    a=20000;
    while(a--);
    ms--;
  }
  return;
}

/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�OLED_Init
*  ����˵������ʼ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-9-13
*  ��    ע��
*************************************************************************/
void OLED_Init(void)
{
  PORT_PCR_REG(PORTE_BASE_PTR, 1)=  0 ;
  PORT_PCR_REG(PORTE_BASE_PTR, 1)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTE_BASE_PTR) |= (1 << 1);
  GPIO_PDOR_REG(PTE_BASE_PTR) |=  (1<<(1));
  PORT_PCR_REG(PORTE_BASE_PTR, 2)=  0 ;
  PORT_PCR_REG(PORTE_BASE_PTR, 2)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTE_BASE_PTR) |= (1 << 2);
  GPIO_PDOR_REG(PTE_BASE_PTR) |=  (1<<(2));
  PORT_PCR_REG(PORTE_BASE_PTR, 3)=  0 ;
  PORT_PCR_REG(PORTE_BASE_PTR, 3)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTE_BASE_PTR) |= (1 << 3);
  GPIO_PDOR_REG(PTE_BASE_PTR) |=  (1<<(3));
  PORT_PCR_REG(PORTE_BASE_PTR, 4)=  0 ;
  PORT_PCR_REG(PORTE_BASE_PTR, 4)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTE_BASE_PTR) |= (1 << 4);
  GPIO_PDOR_REG(PTE_BASE_PTR) |=  (1<<(4));
  PORT_PCR_REG(PORTE_BASE_PTR, 5)=  0 ;
  PORT_PCR_REG(PORTE_BASE_PTR, 5)= PORT_PCR_MUX(1);
  GPIO_PDDR_REG(PTE_BASE_PTR) |= (1 << 5);
  GPIO_PDOR_REG(PTE_BASE_PTR) |=  (1<<(5));
 /* gpio_init (PTE1, GPO,HIGH);
  gpio_init (PTE2, GPO,HIGH);
  gpio_init (PTE3, GPO,HIGH);
  gpio_init (PTE4, GPO,HIGH);
  gpio_init (PTE5, GPO,LOW);*/
  
  OLED_SCL=1;
  OLED_CS = 0 ; ///ʹ���źŶˣ�����ʱ����ʹ��
  OLED_RST=0;
  OLED_DLY_ms(100);
  OLED_RST=1;
  
  OLED_WrCmd(0xae);//--turn off oled panel
  OLED_WrCmd(0x00);//---set low column address
  OLED_WrCmd(0x10);//---set high column address
  OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);//--set contrast control register
  OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
  OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  OLED_WrCmd(0xa6);//--set normal display
  OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  OLED_WrCmd(0x3f);//--1/64 duty
  OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WrCmd(0x00);//-not offset
  OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WrCmd(0xd9);//--set pre-charge period
  OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WrCmd(0xda);//--set com pins hardware configuration
  OLED_WrCmd(0x12);
  OLED_WrCmd(0xdb);//--set vcomh
  OLED_WrCmd(0x40);//Set VCOM Deselect Level
  OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WrCmd(0x02);//
  OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WrCmd(0x14);//--set(0x10) disable
  OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
  OLED_WrCmd(0xaf);//--turn on oled panel
  OLED_Fill(0x00);  //��ʼ����
  //OLED_Set_Pos(0,0);
  //OLED_Fill(0x00);//���� 
  //OLED_DLY_ms(100); 
}

/*************************************************************************
*                           ������ӹ�����
*
//��������LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t *p)
//����������д��һ���׼ASCII�ַ���
//��������ʾ��λ�ã�x,y����yΪҳ��Χ0��7��Ҫ��ʾ���ַ���
//���أ���
*************************************************************************/
void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[])
{
  uint8_t c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    OLED_Set_Pos(x,y);
    for(i=0;i<6;i++)
      OLED_WrDat(F6x8[c][i]);
    x+=6;
    j++;
  }
}
//==============================================================
//��������void OLED_P6x8Char(byte x,byte y,byte ch);
//������������ʾһ��6x8��׼ASCII�ַ�
//������xΪ��ʾ�ĺ�����0~122��yΪҳ��Χ0��7��chҪ��ʾ���ַ�
//���أ���
//============================================================== 
void OLED_P6x8Char(uint8 x,uint8 y,uint8 ch)
{
	uint8 c=0,i=0;     
	c =ch-32;
	if(x>122)
	{
		x=0;
		y++;
	}
	OLED_Set_Pos(x,y);    
	for(i=0;i<6;i++)
	{     
		OLED_WrDat(F6x8[c][i]);  
	}
}
//==============================================================
//��������   
//����������
//������x�ķ�ΧΪ0��62��yΪҳ�ķ�Χ0��7��dataΪ��Ҫת����ʾ����ֵ�����������λ5λ  num��ʾ��λ0~4
//���أ���
//==============================================================
 void OLED_PrintValueInt(uint8 x, uint8 y, signed  int data, uint8 num)
 {
        uint8 z = 0;
 	uint8 m,i,j,k;
        if (data < 0)
        {
          OLED_P6x8Char(x,y,'-');
          data = - data;
          z = 1;
        }
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	
          if (1 == z)
          {
            switch(num)
            {       
                case 1:  	OLED_P6x8Char(x+6,y,k+48);
				break;
		case 2:  	OLED_P6x8Char(x+6,y,j+48);
				OLED_P6x8Char(x+12,y,k+48);
				break;
		case 3:	OLED_P6x8Char(x+6,y,i+48);
				OLED_P6x8Char(x+12,y,j+48);
				OLED_P6x8Char(x+18,y,k+48);
				break;
		case 4: 	OLED_P6x8Char(x+6,y,m+48);
				OLED_P6x8Char(x+12,y,i+48);
				OLED_P6x8Char(x+18,y,j+48);
				OLED_P6x8Char(x+24,y,k+48);
				break;	
            }
          }
          else
          {
            switch(num)
            {
                case 1:  	OLED_P6x8Char(x,y,k+48);
                                OLED_P6x8Char(x+6,y,' ');
				break;
		case 2:  	OLED_P6x8Char(x,y,j+48);
				OLED_P6x8Char(x+6,y,k+48);
                                OLED_P6x8Char(x+12,y,' ');
				break;
		case 3:	OLED_P6x8Char(x,y,i+48);
				OLED_P6x8Char(x+6,y,j+48);
				OLED_P6x8Char(x+12,y,k+48);
                                OLED_P6x8Char(x+18,y,' ');
				break;
		case 4: 	OLED_P6x8Char(x,y,m+48);
				OLED_P6x8Char(x+6,y,i+48);
				OLED_P6x8Char(x+12,y,j+48);
				OLED_P6x8Char(x+18,y,k+48);
                                OLED_P6x8Char(x+24,y,' ');
				break;	
            }
          }
 }
//==============================================================
//��������   
//�������������float��С������
//������x�ķ�ΧΪ0��62��yΪҳ�ķ�Χ0��7��dataΪ��Ҫת����ʾ����ֵ�����������λ5λ  num��ʾ��λ0~4
//���أ���
//==============================================================
 void OLED_PrintValueFP(uint8 x, uint8 y, unsigned  int data, uint8 num)
 {
 	uint8 m,i,j,k;  	
 	OLED_P6x8Char(x, y, '.');
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	switch(num)
	{
		case 1:  	OLED_P6x8Char(x+6,y,k+48);
				break;
		case 2:  	OLED_P6x8Char(x+6,y,j+48);
				OLED_P6x8Char(x+12,y,k+48);
				break;
		case 3:	OLED_P6x8Char(x+6,y,i+48);
				OLED_P6x8Char(x+12,y,j+48);
				OLED_P6x8Char(x+18,y,k+48);
				break;
		case 4: 	OLED_P6x8Char(x+6,y,m+48);
				OLED_P6x8Char(x+12,y,i+48);
				OLED_P6x8Char(x+18,y,j+48);
				OLED_P6x8Char(x+24,y,k+48);
				break;	
	}
 }
//==============================================================
//��������   void OLED_PrintValueF(byte x, byte y, float data, byte num);
//������������һ��float����ת������������5λ��С���ͷ��ŵ����ݲ�������ʾ
//������x�ķ�ΧΪ0��62��yΪҳ�ķ�Χ0��7��dataΪ��Ҫת����ʾ����ֵ�����������λ5λ  num��ʾ������С��λ0~4
//���أ���
//==============================================================
 void OLED_PrintValueF(uint8 x, uint8 y, float data, uint8 num)
 {
 	uint8 l,m,i,j,k;  //��ǧ��ʮ��
 	uint8 databiti = 6; //����λ��
 	unsigned  int tempdataui = 0;
  	int tempdataii = (int)data; //��������
 	long int tempdatalp = (long int)((data - (int)data)*10000); //ȡС��λ��4λ
 	
 	//����������ʾ
 	if(data < 0.0000001)  OLED_P6x8Char(x, y,'-'); 
 	else OLED_P6x8Char(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;  //ȥ���������ָ���
	tempdataui = tempdataii;
 	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
 	if (l != 0)  //��λ
 	{
 		OLED_P6x8Char(x+6,y,l+48);
 		OLED_P6x8Char(x+12,y,m+48);
		OLED_P6x8Char(x+18,y,i+48);
		OLED_P6x8Char(x+24,y,j+48);
		OLED_P6x8Char(x+30,y,k+48);
 	}
 	else if(m != 0) //��λ
 	{
 		databiti = 5;
 		OLED_P6x8Char(x+6,y,m+48);
 		OLED_P6x8Char(x+12,y,i+48);
		OLED_P6x8Char(x+18,y,j+48);
		OLED_P6x8Char(x+24,y,k+48);
 	}
  	else if(i != 0) //��λ
  	{
  		databiti = 4;
  	 	OLED_P6x8Char(x+6,y,i+48);
 		OLED_P6x8Char(x+12,y,j+48);
		OLED_P6x8Char(x+18,y,k+48);
  	}
  	else if(j != 0) //��λ
  	{
    		databiti = 3;	
  		OLED_P6x8Char(x+6,y,j+48);
 		OLED_P6x8Char(x+12,y,k+48);
  	}
	else 	
	{
		databiti = 2;
		OLED_P6x8Char(x+6,y,k+48);
	}	
 	if(tempdatalp < 0)tempdatalp = - tempdatalp;	//ȥ��С�����ָ���
	switch(num)
	{
		case 0: break;
		case 1:  OLED_PrintValueFP(x + databiti * 6, y, (uint)(tempdatalp / 1000),num);break;
		case 2:  OLED_PrintValueFP(x + databiti * 6, y, (uint)(tempdatalp / 100),num);break;
		case 3:  OLED_PrintValueFP(x + databiti * 6, y, (uint)(tempdatalp / 10),num);break;
		case 4:  OLED_PrintValueFP(x + databiti * 6, y, (uint)(tempdatalp),num);break;					
	}
 }
/**************************/
//Oled��ʾ����ͷ����
//��ʼ����Ϊ��5�У�0��
//ÿ��60����
//��60��
/**************************/
void OLED_PrintPicture(void)
{
  uint8 data = 0;
  uint8 i,j,k;
  uint8 x,y;
  for (k = 0;k < 7;k++)//����56�У��������е����㣩
  {
    OLED_Set_Pos(5,k);//�������꣬ÿ����8�У�60��
    x = k*8;//����ʼλ��
    y = (k+1)*8 - 1;//�м���(ֻҪѭ��7�У�����һ�е�����)
    for (i = 0;i< PICTURE_WIDTH;i++)
    {
      //��λ��ʾ��8�е������棬��λ��ʾ��������
      //����0x83����ʾ
      //* 1
      //* 1
      //  0
      //  0
      //  0 
      //  0 
      //  0
      //* 1
      //���Ҫ�Ӹߵ���ɨ��
      for (j = y;j > x;j--)//�ӵ�j��i�е���j+6��i��
      {
          data += PixData[j][i];//��Ӧλ��һ������(ͼ���Ѿ���ֵ����ֻ����0��1)
          data<<=1;//����һλ
      }
      //j+7��i��
      data += PixData[j][i];//�˴���Ҫλ��
      OLED_WrDat(data);//д��8�У�1�е����ݲ���ʾ
      data = 0;
    }
  }
  OLED_Set_Pos(5,7);//�������
  x = 56;//����ʼλ��
  y = 60;//�м���(ֻҪѭ��4��)
  for (i = 0;i < PICTURE_WIDTH;i++)//ÿ��д8�У�ͼ������  60��
  {
    for (j = x;j < y;j++)//�ӵ�56��i�е���59��i��
    {
        data += PixData[j][i];//��Ӧλ��һ(ͼ���Ѿ���ֵ����ֻ����0��1)
        data<<=1;
    }
    data>>=1;
    OLED_WrDat(data);//д��8�У�1�е����ݲ���ʾ
    data = 0;
  }
}