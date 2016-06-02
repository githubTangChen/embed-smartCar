/*****************************************
�� �� ����ImageProcess.c
��    ����ͼ����
�޸����ڣ�2016-3-16
*****************************************/
#include "common.h"
unsigned char EndPosition = 0;//��ǰ�����ı߽�������
unsigned char MidLineCompleteFlag = 0;//������ȡ��ɱ�־
signed char MidPoints[PICTURE_HIGHT] = {0};//����ÿ���е�
signed char Row;//��
signed char Col;//��
signed char roadFlag;
signed char countTimes;
//ͼ������
extern unsigned char PixData[PICTURE_HIGHT][PICTURE_WIDTH];
void CenterLineExtractionFullScan(void)
	{
		signed char leftBoudanry=-1,rightBoudanry=-1,midPonit,stop,lastRoadWidth;
		signed char Error[3],flag;
		//roadFlag &= 0xfe;
		countTimes=0;
		Row=PICTURE_HIGHT-1;
		//�����е��е�
		for(Col=3;Col<PICTURE_WIDTH-2;Col++)
		{
			//����߽�
			if(PixData[Row][Col]==1 && PixData[Row][Col-1]==0)
				leftBoudanry=Col;
			//���ұ߽�
			if(PixData[Row][Col]==0 && PixData[Row][Col-1]==1)
				rightBoudanry=Col;
		}
		//�ҵ���߽絫��û���ҵ��ұ߽����ұ߽�Ϊ��
		if(rightBoudanry<0 && leftBoudanry>0 && PixData[Row][PICTURE_WIDTH-2]==1)
		{
			rightBoudanry=PICTURE_WIDTH-1;
		}
		//�ҵ��ұ߽絫��û���ҵ���߽�
		if(rightBoudanry>0 && leftBoudanry<0)
		{
			leftBoudanry=0;
		}
		if(rightBoudanry<0 && leftBoudanry<0)
		{
			leftBoudanry=0;
			rightBoudanry=PICTURE_WIDTH-1;
		}
		lastRoadWidth = rightBoudanry - leftBoudanry;
		midPonit = (unsigned char)(leftBoudanry+rightBoudanry)/2;
		MidPoints[Row]=midPonit;
		PixData[Row][MidPoints[Row]]=0;
		for(Row--;Row>0;Row--)
		{
			leftBoudanry=-1;
			rightBoudanry=-1;
			if(midPonit<2)
			{
				leftBoudanry=0;
			}
			if(midPonit>(PICTURE_WIDTH-3))
			{
				rightBoudanry=PICTURE_WIDTH-1;
			}
			if(rightBoudanry==-1)
			{
				stop = PICTURE_WIDTH;
				for(Col=midPonit;Col<stop;Col++)
				{
					if(PixData[Row][Col]==0 && PixData[Row][Col-1]==1)
					{
						rightBoudanry=Col;
						break;
					}
					if(Col==(stop-1) && rightBoudanry==-1)
					{
						rightBoudanry=PICTURE_WIDTH;
						break;
					}
				}
			}
			if(leftBoudanry==-1)
			{
				stop = 0;
				for(Col=midPonit;Col>stop;Col--)
				{
					if(PixData[Row][Col]==0 && PixData[Row][Col+1]==1)
					{
						leftBoudanry=Col;
						break;
					}
					if(Col==(stop-1) && leftBoudanry==-1)
					{
						leftBoudanry=-1;
						break;
					}
				}
			}
			//���ұ߽綼û���ҵ�
			if(leftBoudanry==-1 && rightBoudanry==PICTURE_WIDTH && Row<(PICTURE_WIDTH-10))
			{
				leftBoudanry=2;
				rightBoudanry=PICTURE_WIDTH-3;
				countTimes++;
			}
			else if(countTimes<5 || countTimes>20)
			{
				countTimes=0;
			}
			//if(lastRoadWidth<(rightBoudanry - leftBoudanry-10) && (rightBoudanry - leftBoudanry)>=(PICTURE_WIDTH-10))
			//	flag |= 0x01;
			//if(lastRoadWidth>(rightBoudanry - leftBoudanry) && lastRoadWidth>=(PICTURE_WIDTH-5))
			//	flag |= 0x02;
			lastRoadWidth = rightBoudanry - leftBoudanry;
			midPonit = (unsigned char)(leftBoudanry+rightBoudanry)/2;
			MidPoints[Row]=midPonit;
			PixData[Row][MidPoints[Row]]=0;
		}
		Row = 59;
		Error[0] = MidPoints[Row] - MidPoints[Row - 1];
		Error[1] = MidPoints[Row - 1] - MidPoints[Row - 2];
		Error[2] = Error[0] - Error[1];
		Error[2] = Error[2] < 0 ? -Error[2] : Error[2];
		Error[0] = Error[0] < 0 ? -Error[0] : Error[0];
		Error[1] = Error[1] < 0 ? -Error[1] : Error[1];
		if (Error[2] > 10)
		{
			//�������
			if (Error[0] > Error[1] && Error[1] < 4)
			{
				MidPoints[Row] = MidPoints[Row - 1];
			}
			if (Error[1] > Error[0] && Error[0] < 4)
			{
				MidPoints[Row - 2] = MidPoints[Row - 1];
			}
		}
        Row -= 3;
        for (; Row >= 5; Row--)
		{
            Error[0] = MidPoints[Row] - MidPoints[Row - 2];
            Error[1] = MidPoints[Row] - MidPoints[Row + 1];
            Error[0] = Error[0] < 0 ? -Error[0] : Error[0];
			if (Error[0] > 5)//���������
            {
                Error[1] = MidPoints[Row + 1] - MidPoints[Row + 2];
                for (int i = 0; i< 3; i++)
                {
                    PixData[Row - i][MidPoints[Row - i]] = 1;//ȡ��ԭ���е�
					MidPoints[Row-i] = MidPoints[Row - i + 1] + Error[1];
					PixData[Row - i][MidPoints[Row - i]] = 0;//�е���
                }
                Row -= 1;
             }
             if (MidPoints[Row] > 60 || MidPoints[Row] < 10)//�߽�㣬ֱ���˳�
             {
				PixData[Row][MidPoints[Row]] = 0;//�е���
				break;
             }
		}
		if((countTimes>10))
			roadFlag |= 0x01;
		EndPosition = Row;//����ѭ������ʱ��������
		MidLineCompleteFlag = 1;
	}