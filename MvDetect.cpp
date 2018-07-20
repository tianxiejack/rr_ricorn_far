#include<opencv2/opencv.hpp>
#include"MvDetect.h"
#include<string.h>
#include<stdio.h>
using namespace cv;
Mat m4(2160,640,CV_8UC3);
Mat m6(3240,640,CV_8UC3);

Mat m4_2cc(2160,640,CV_8UC2);
Mat m6_2cc(3240,640,CV_8UC2);
unsigned char * p_newestMvSrc[CAM_COUNT]={NULL,NULL,NULL,NULL,NULL,NULL,NULL
,NULL,NULL,NULL};
extern MvDetect mv_detect;

#if  MVDECT
MvDetect::MvDetect()
{
	for(int i=0;i<CAM_COUNT;i++)
	{
		for(int j=0;j<6;j++)
		{
			tempRect_Srcptr[i].tempoutRect.rects[j].x=-1;
			tempRect_Srcptr[i].tempoutRect.rects[j].y=-1;
			tempRect_Srcptr[i].tempoutRect.rects[j].width=-1;
			tempRect_Srcptr[i].tempoutRect.rects[j].height=-1;
			tempRect_Srcptr[i].isDetectionDone=false;
		}
	}
		for(int i=0;i<CAM_COUNT;i++)
		{
			grayFrame[i]=(unsigned char *)malloc(MAX_SCREEN_WIDTH*MAX_SCREEN_HEIGHT*1);
		}
}
MvDetect::~MvDetect()
{
			exitDetect();
			for(int i=0;i<CAM_COUNT;i++)
			{
				free(grayFrame[i]);
			}
}
void MvDetect::init(int w,int h)
{
		createDetect((unsigned char)CAM_COUNT,w,h);
}
void MvDetect::uyvy2gray(unsigned char* src,unsigned char* dst,int width,int height)
{
	for(int i=0;i<width*height;i++)
		{
	    *(dst++) =*(++src) ;
	    src+=1;
		}
}
void MvDetect::m_mvDetect(int idx,unsigned char* inframe,int w,int h)
{
		idx-=1;
		uyvy2gray(inframe,grayFrame[idx]);
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
					if(tempRect_Srcptr[idx].tempoutRect.rects[j].x>0)
					{
						tempOut.outRect=tempRect_Srcptr[idx].tempoutRect.rects[j];
						tempOut.camIdx=idx;
						outRect[idx].push_back(tempOut);
					}
				}
			}
	}
}



void MvDetect::DrawRectOnpic(unsigned char *src,int capidx,int cc)
{
	std::vector<mvRect> tempRecv[CAM_COUNT];
	if(capidx==MAIN_FPGA_SIX)
	{
		if(cc==3)
		{
			m6.data=src;
		}
		else if(cc==2)
		{
			m6_2cc.data=src;
		}
		for(int i=0;i<6;i++)                        //0  1  2
		{															//3  4  5
			tempRecv[i].assign(outRect[i].begin(),outRect[i].end());
			if(tempRecv[i].size()!=0)//容器dix不为空
			{
				for(int rectIdx=0;rectIdx<tempRecv[i].size();rectIdx++)//从容器中一个一个取出
				{
					int startx=tempRecv[i][rectIdx].outRect.x/3;
					int starty=tempRecv[i][rectIdx].outRect.y/2+540*i;
					int w=tempRecv[i][rectIdx].outRect.width/3;
					int h=tempRecv[i][rectIdx].outRect.height/2;//取出容器中rect的值
					int endx=startx+w;
					int endy=starty+h;
					if(cc==3)
					{
						cv::rectangle(m6,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(0,0,0),2);
					}
					else if(cc==2)
					{
						cv::rectangle(m6_2cc,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(0,0,0),2);
					}
				}
			}
		}
	}
	if(capidx==MAIN_FPGA_FOUR)
	{
		if(cc==3)
		{
		m4.data=src;
		}
		else if(cc==2)
		{
			m4_2cc.data=src;
		}
		for(int i=6;i<10;i++)						//6   7
		{															//8	 9
			tempRecv[i].assign(outRect[i].begin(),outRect[i].end());
			if(tempRecv[i].size()!=0)//容器dix不为空
			{
				for(int rectIdx=0;rectIdx<tempRecv[i].size();rectIdx++)//从容器中一个一个取出
				{
					int startx=tempRecv[i][rectIdx].outRect.x/3;
					int starty=tempRecv[i][rectIdx].outRect.y/2+540*(i-6);
					int w=tempRecv[i][rectIdx].outRect.width/3;
					int h=tempRecv[i][rectIdx].outRect.height/2;//取出容器中rect的值
					int endx=startx+w;
					int endy=starty+h;
					if(cc==3)
					{
						cv::rectangle(m4,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(0,0,0),2);
					}
					else if(cc==2)
					{
						cv::rectangle(m4_2cc,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(0,0,0),2);
					}

				}
			}
		}
	}
}

