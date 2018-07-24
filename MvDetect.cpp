#include<opencv2/opencv.hpp>
#include"MvDetect.h"
#include<string.h>
#include<stdio.h>
using namespace cv;

#if  MVDECT

MvDetect::MvDetect()
{
	for(int i=0;i<CAM_COUNT;i++)
	{
		for(int j=0;j<6;j++)
		{
			tempRect_Srcptr[i].tempoutRect.rects[j].targetRect.x=-1;
			tempRect_Srcptr[i].tempoutRect.rects[j].targetRect.y=-1;
			tempRect_Srcptr[i].tempoutRect.rects[j].targetRect.width=-1;
			tempRect_Srcptr[i].tempoutRect.rects[j].targetRect.height=-1;
			tempRect_Srcptr[i].isDetectionDone=false;
		}
	}
		for(int i=0;i<CAM_COUNT;i++)
		{
//			grayFrame[i]=(unsigned char *)malloc(MAX_SCREEN_WIDTH*MAX_SCREEN_HEIGHT*1);
		}
}
MvDetect::~MvDetect()
{
			exitDetect();
			for(int i=0;i<CAM_COUNT;i++)
			{
	//			free(grayFrame[i]);
			}
}

void MvDetect::init(int w,int h)
{
		createDetect((unsigned char)CAM_COUNT,w,h);
}

void MvDetect::m_mvDetect(int idx,unsigned char* inframe,int w,int h)
{
		idx-=1;
		uyvy2gray(inframe,grayFrame[idx],idx);
		{
			tempRect_Srcptr[idx].isDetectionDone = false;
			mvDetect((unsigned char) (idx+1), grayFrame[idx], w, h,
					tempRect_Srcptr[idx].tempoutRect.rects,
					&tempRect_Srcptr[idx].isDetectionDone);
		}
}
#endif


void MvDetect::SetoutRect()
{
	for(int idx=0;idx<CAM_COUNT;idx++)
	{
		if(tempRect_Srcptr[idx].isDetectionDone)//沒有檢測完成，保留上次結果
				outRect[idx].clear();
		mvRect tempOut;
			for(int j=0;j<6;j++)
			{
				if(tempRect_Srcptr[idx].isDetectionDone){
					if(tempRect_Srcptr[idx].tempoutRect.rects[j].targetRect.x>0)
					{
						tempOut.outRect=tempRect_Srcptr[idx].tempoutRect.rects[j];
						tempOut.camIdx=idx;
						outRect[idx].push_back(tempOut);
					}
				}
			}
	}
}
