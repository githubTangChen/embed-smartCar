/*****************************************
�� �� ����ImageProcess.h
��    ����ͼ����
�޸����ڣ�2016-3-16
*****************************************/
#ifndef _IMAGEPROCESS_H_
#define _IMAGEPROCESS_H_

extern unsigned char EndPosition;//��ǰ�����ı߽�������
//unsigned char  MidPoint  = 34;
extern unsigned char MidLineCompleteFlag;//������ȡ��ɱ�־
extern signed char MidPoints[PICTURE_HIGHT];//����ÿ���е�
extern signed char Row;//��
extern signed char Col;//��

void CenterLineExtractionFullScan(void);
#endif