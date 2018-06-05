#include<opencv2/opencv.hpp>
#include"StlGlDefines.h"
#include"MvDetect.h"
#include<string.h>
#include<stdio.h>
#include"MvDrawRect.h"
using namespace cv;
Mat m4(2160,640,CV_8UC3);
Mat m6(3240,640,CV_8UC3);
#if  MVDECT
extern MvDetect mv_detect;
MotionDetectorROI   mdRoi_mainT(4,&mv_detect),mdRoi_subT(4,&mv_detect);
MotionDetectorROI   mdRoi_mainA(2,&mv_detect),mdRoi_subA(2,&mv_detect);


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
		}
	}

for(int i=0;i<2;i++)
{
		enableMD[i]=false;
		MDopen[i]=false;
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
		mvDetect((unsigned char) (idx+1), grayFrame[idx], w, h,tempRect_Srcptr[idx].tempoutRect.rects);
	}
	//mvDetect((unsigned char) idx, grayFrame[idx], w, h,&outRect[idx]);
}
#endif

void MvDetect::saveConfig()
{
	char buf[4]={0};
	FILE * fp=fopen("./data/MvDetect.txt","w");
	if(fp==NULL)
	{
		printf("MvDetect open failed!\n");
	}
	if(GetMD(MAIN) &&GetMD(SUB))
	{
		sprintf(buf,"%d\n",1);
	}
	else
		sprintf(buf,"%d\n",0);
	fwrite(buf,sizeof(buf),1,fp);
	fclose(fp);
}

void myRect(unsigned char *dst,unsigned char *src,int x,int y,int w,int h)
{
	unsigned char temp[540*640*4];
	Mat SRC(1080,1920,CV_8UC4,src);
	Mat (540,640,CV_8UC4,dst);
	Rect rect(x,y,w,h);
	Mat DST=SRC(rect);
	memcpy(dst,DST.data,540*640*4);
}
void MvDetect::selectFrame(unsigned char *dst,unsigned char *src,int targetId,int camIdx)
{
#if 0
	int startx=0;
	int starty=0;
	int w=640;
	int h=540;
	int midx=0;
	int midy=0;
	if(targetnum[camIdx]==0)
	{
		for(int i=0;i<CAM_COUNT;i++)
		{
			for(int j=0;j<4;j++)
				targetidx[i][j]=0;
		}
		memset(dst,0xffff,640*540*4);
	}
	else
	{
		std::vector<cv::Rect> tempRecv[CAM_COUNT];
		for(int i=0;i<CAM_COUNT;i++)
		{
			tempRecv[i].assign(outRect[i].begin(),outRect[i].end());
		}
		startx=tempRecv[camIdx][targetidx[camIdx][targetId]].x;
		starty=tempRecv[camIdx][targetidx[camIdx][targetId]].y;
		w=tempRecv[camIdx][targetidx[camIdx][targetId]].width;
		h=tempRecv[camIdx][targetidx[camIdx][targetId]].height;
		midx=startx+w/2;
		midy=starty+h/2;
		if(midx<320)
		{
			midx=320;
		}
		else if (midx >1600)
		{
			midx=1600;
		}
		if(midy<270)
			{
				midy=270;
			}
			else if (midy >810)
			{
				midy=810;
			}
		startx=midx-640/2;
		starty=midy-540/2;
		myRect(dst,src,startx,starty,640,540);
	}
#endif

}
void MvDetect::ReadConfig()
{
	char buf[4];
	int Yes=-1;
	FILE * fp=fopen("./data/MvDetect.txt","r");
	if(fp==NULL)
	{
		printf("MvDetect open failed!\n");
	}
	fread(buf,sizeof(buf),1,fp);
	fclose(fp);
	Yes=atoi(buf);
	if(Yes)
	{
		SetMD(true,MAIN);
		SetMD(true,SUB);
	}
	else
	{
		SetMD(false,MAIN);
		SetMD(false,SUB);
	}
}
bool MvDetect::CanUseMD(int mainorsub)
{
	if(enableMD[mainorsub]==true)
		return true;
	else
		return false;
}
std::vector<mvRect> *  MvDetect::GetWholeRect()
{
	m_WholeRect.clear();
	for(int i=0;i<CAM_COUNT;i++)
		m_WholeRect.insert(m_WholeRect.end(),m_outRect[i].begin(),m_outRect[i].end());
	return &m_WholeRect;
}
void MvDetect::SetoutRect(int idx)
{
	outRect[idx].clear();
	Out_Rect_Srcptr tempOut;
	for(int j=0;j<6;j++)
	{
		if(tempRect_Srcptr[idx].tempoutRect.rects[j].x>0)
		{
			tempOut.srcptr=tempRect_Srcptr[idx].srcptr;
			tempOut.m_outRect=tempRect_Srcptr[idx].tempoutRect.rects[j];
			outRect[idx].push_back(tempOut);
		}
	}
#if 0
	m_outRect[idx].clear();
	for(int j=0;j<6;j++)
	{
		if(tempoutRect[idx].rects[j].x>0)
		{
			mvRect mr;
			mr.outRect=tempoutRect[idx].rects[j];
			mr.camIdx=idx;
			mr.x_angle=((float)(tempoutRect[idx].rects[j].x))/1920.0*PER_CAM_ANGLE+idx*PER_CAM_ANGLE;
			mr.y_angle=((float)(tempoutRect[idx].rects[j].y))/1080.0;
			m_outRect[idx].push_back(mr);
		}
	}
#endif
}
void MvDetect::DrawRectOnpic(unsigned char *src,int capidx)
{
#if 1
	std::vector<Out_Rect_Srcptr> tempRecv[CAM_COUNT];
//	MotionDetectorROI mdRoi;

	if(capidx==MAIN_FPGA_SIX)
	{
//		mdRoi_main.SettempSrc6(src);  //todo
//		mdRoi_sub.SettempSrc6(src);
		m6.data=src;
		for(int i=0;i<6;i++)                        //0  1  2
		{															//3  4  5
			tempRecv[i].assign(outRect[i].begin(),outRect[i].end());
			if(tempRecv[i].size()!=0)//容器dix不为空
			{
				for(int rectIdx=0;rectIdx<tempRecv[i].size();rectIdx++)//从容器中一个一个取出
				{
					int startx=tempRecv[i][rectIdx].m_outRect.x/3;
					int starty=tempRecv[i][rectIdx].m_outRect.y/2+540*i;
					int w=tempRecv[i][rectIdx].m_outRect.width/3;
					int h=tempRecv[i][rectIdx].m_outRect.height/2;//取出容器中rect的值
					int endx=startx+w;
					int endy=starty+h;
					cv::rectangle(m6,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(0,0,0),1);
				}
			}
		}
	}

	if(capidx==MAIN_FPGA_FOUR)
		{
	//	mdRoi_main.SettempSrc4(src);
	//	mdRoi_sub.SettempSrc6(src);//todo
		m4.data=src;
			for(int i=6;i<10;i++)						//6   7
			{													//8	 9
				tempRecv[i].assign(outRect[i].begin(),outRect[i].end());
				if(tempRecv[i].size()!=0)//容器dix不为空
				{
					for(int rectIdx=0;rectIdx<tempRecv[i].size();rectIdx++)//从容器中一个一个取出
					{
						int startx=tempRecv[i][rectIdx].m_outRect.x/2;
						int starty=tempRecv[i][rectIdx].m_outRect.y/2+540*i;
						int w=tempRecv[i][rectIdx].m_outRect.width/2;
						int h=tempRecv[i][rectIdx].m_outRect.height/2;//取出容器中rect的值
						int endx=startx+w;
						int endy=starty+h;
						cv::rectangle(m4,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(0,0,0),1);
					}
				}
			}
		}
#if 0
	for(int mOs=0;mOs<2;mOs++)
	{
		if(mOs==MAIN)
			mdRoi=mdRoi_main;
		else if(mOs==SUB)
			mdRoi=mdRoi_sub;
	for(int targetidx=0;targetidx<2;targetidx++)
	{
		if(mdRoi.IsChooseN(targetidx))
		{
			mdRoi.resetChooseN(targetidx);
			mdRoi.RankVectorClear(targetidx);
			float range_start=mdRoi.GetRange(START)/360.0*10.0;
			int cam_idx_start=(int)range_start;
			float range_end=mdRoi.GetRange(END)/360.0*10.0;
			int cam_idx_end=(int)range_end;
			if(range_start>270.0){  //跨过相机0
				for(int i=cam_idx_start;i<CAM_COUNT;i++)
									mdRoi.put2RankVector(targetidx,&tempRecv[i],i);
				for(int i=0;i<cam_idx_end;i++)
									mdRoi.put2RankVector(targetidx,&tempRecv[i],i);
			}
			else{
				for(int i=cam_idx_start;i<cam_idx_end;i++)
					mdRoi.put2RankVector(targetidx,&tempRecv[i],i);
			}
			mdRoi.SetlastRect(targetidx,mdRoi.Rank(targetidx,BIG));
			mdRoi.drawRect(targetidx,src,capidx);

		}



		if(mdRoi.IsChooseP(targetidx))
				{
					mdRoi.resetChooseP(targetidx);
					mdRoi.RankVectorClear(targetidx);
					float range_start=mdRoi.GetRange(START)/360.0*10.0;
					int cam_idx_start=(int)range_start;
					float range_end=mdRoi.GetRange(END)/360.0*10.0;
					int cam_idx_end=(int)range_end;
					if(range_start>270.0){  //跨过相机0
						for(int i=cam_idx_start;i<CAM_COUNT;i++)
											mdRoi.put2RankVector(targetidx,&tempRecv[i],i);
						for(int i=0;i<cam_idx_end;i++)
											mdRoi.put2RankVector(targetidx,&tempRecv[i],i);
					}
					else{
						for(int i=cam_idx_start;i<cam_idx_end;i++)
							mdRoi.put2RankVector(targetidx,&tempRecv[i],i);
					}
					mdRoi.SetlastRect(targetidx,mdRoi.Rank(targetidx,SMALL));
					mdRoi.drawRect(targetidx,src,capidx);

				}

	}
	}
#endif
#endif
}

