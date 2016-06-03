/*****************************************
文 件 名：OutputData.h
描    述：发送数据到示波器
修改日期：2016-5-31
*****************************************/
#ifndef _OUTPUTDATA_H
#define _OUTPUTDATA_H

extern float OutData[4];

unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void Output_Data(void);
#endif 
