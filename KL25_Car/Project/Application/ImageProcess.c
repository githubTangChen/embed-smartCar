/*****************************************
文 件 名：ImageProcess.c
描    述：图像处理
修改日期：2016-3-16
*****************************************/
#include "common.h"
unsigned char EndPosition = 0;//提前结束的边界行坐标
unsigned char MidLineCompleteFlag = 0;//中线提取完成标志
signed char MidPoints[PICTURE_HIGHT] = {0};//保存每行中点
signed char Row;//行
signed char Col;//列
signed char roadFlag;
signed char countTimes;
//图像数据
extern unsigned char PixData[PICTURE_HIGHT][PICTURE_WIDTH];
void CenterLineExtractionFullScan(void)
	{
		signed char leftBoudanry=-1,rightBoudanry=-1,midPonit,stop,lastRoadWidth;
		signed char Error[3],flag;
		//roadFlag &= 0xfe;
		countTimes=0;
		Row=PICTURE_HIGHT-1;
		//找首行的中点
		for(Col=3;Col<PICTURE_WIDTH-2;Col++)
		{
			//找左边界
			if(PixData[Row][Col]==1 && PixData[Row][Col-1]==0)
				leftBoudanry=Col;
			//找右边界
			if(PixData[Row][Col]==0 && PixData[Row][Col-1]==1)
				rightBoudanry=Col;
		}
		//找到左边界但是没有找到右边界且右边界为白
		if(rightBoudanry<0 && leftBoudanry>0 && PixData[Row][PICTURE_WIDTH-2]==1)
		{
			rightBoudanry=PICTURE_WIDTH-1;
		}
		//找到右边界但是没有找到左边界
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
			//左右边界都没有找到
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
			//消除误差
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
			if (Error[0] > 5)//简单误差消除
            {
                Error[1] = MidPoints[Row + 1] - MidPoints[Row + 2];
                for (int i = 0; i< 3; i++)
                {
                    PixData[Row - i][MidPoints[Row - i]] = 1;//取消原来中点
					MidPoints[Row-i] = MidPoints[Row - i + 1] + Error[1];
					PixData[Row - i][MidPoints[Row - i]] = 0;//中点变黑
                }
                Row -= 1;
             }
             if (MidPoints[Row] > 60 || MidPoints[Row] < 10)//边界点，直接退出
             {
				PixData[Row][MidPoints[Row]] = 0;//中点变黑
				break;
             }
		}
		if((countTimes>10))
			roadFlag |= 0x01;
		EndPosition = Row;//记下循环结束时的行坐标
		MidLineCompleteFlag = 1;
	}