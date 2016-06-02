/*****************************************
文 件 名：ImageProcess.h
描    述：图像处理
修改日期：2016-3-16
*****************************************/
#ifndef _IMAGEPROCESS_H_
#define _IMAGEPROCESS_H_

extern unsigned char EndPosition;//提前结束的边界行坐标
//unsigned char  MidPoint  = 34;
extern unsigned char MidLineCompleteFlag;//中线提取完成标志
extern signed char MidPoints[PICTURE_HIGHT];//保存每行中点
extern signed char Row;//行
extern signed char Col;//列

void CenterLineExtractionFullScan(void);
#endif