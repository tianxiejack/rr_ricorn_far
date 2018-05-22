#include "MvDrawRect.h"
#include"StlGlDefines.h"
MotionDetectorROI::MotionDetectorROI()
{
	for(int i=0;i<2;i++)
	{
	m4[i]=(Mat(FPGA_SCREEN_HEIGHT,FPGA_SCREEN_WIDTH,CV_8UC4));
	m6[i]=(Mat(SDI_HEIGHT,SDI_WIDTH,CV_8UC4));
	chooseNext[i]=false;
	choosePre[i]=false;
	lastRect[i].height=-1;
	lastRect[i].width=-1;
	lastRect[i].x=-1;
	lastRect[i].y=-1;
	for(int j=0;j<CC;j++)
		RectColor[i][j]=0;
	RoiSrc[i]=(unsigned char *)malloc(640/3*540/2*4);
	}
//	tempSrc4=(unsigned char *)malloc(FPGA_SCREEN_HEIGHT*FPGA_SCREEN_WIDTH*4);
//	tempSrc6=(unsigned char *)malloc(SDI_HEIGHT*SDI_WIDTH*4);
}

void MotionDetectorROI::SettempSrc4(unsigned char *src)
{
	tempSrc4=src;
	//memcpy(tempSrc4,src,FPGA_SCREEN_HEIGHT*FPGA_SCREEN_WIDTH*4);
}
void MotionDetectorROI::SettempSrc6(unsigned char *src)
{
	tempSrc6=src;
	//memcpy(tempSrc6,src,SDI_HEIGHT*SDI_WIDTH*4);
}
void  MotionDetectorROI::put2RankVector(int targetidx,std::vector<cv::Rect> *p_singleRect,int camidx)
{
	Rect2f tempr;
	vector<cv::Rect>::iterator  it;
	float perCamAngle=360.0/(float)(CAM_COUNT);
	//tempr
	for(it=p_singleRect->begin();it!=p_singleRect->end();it++)
	{
		tempr.x=(perCamAngle-((float)((*it).x)/SDI_WIDTH)*perCamAngle)+camidx*perCamAngle;
		tempr.y=(float)(*it).y;
		tempr.width=(float)(*it).width;
		tempr.height=(float)(*it).height;
		RankRect[targetidx].push_back(tempr);
	}
}

void MotionDetectorROI::SetRoiSrc(int targetidx,int startx,int starty,int w,int h,int nowCamidx)
{
	int transx=startx/3;
	int transy=starty/2;
	int transw=w/3;
	int transh=h/2;
	int midx=transx+transw/2;
	int midy=transy+transh/2;
	if(midx<=FPGA_SINGLE_PIC_W/3/2)
	if(nowCamidx>=0 &&nowCamidx<=5)
	{

	}
	else if(nowCamidx>=6&&nowCamidx<=9)
	{

	}
}
void MotionDetectorROI::drawRect(int targetidx,unsigned char *src,int capidx)
{
	int nowCamidx=(int)lastRect[targetidx].x/360.0*10;
	float nowCamidxf=lastRect[targetidx].x/360.0*10;
	if((float)nowCamidx==nowCamidxf)
	{
		nowCamidx-=1;
	}
	if(lastRect[targetidx].x>=(360/CAM_COUNT))
		lastRect[targetidx].x-=(360/CAM_COUNT);
	lastRect[targetidx].x=lastRect[targetidx].x/36.0*SDI_WIDTH;
	int startx=lastRect[targetidx].x/2+(640*((nowCamidx-6)%2));
	int starty=lastRect[targetidx].y/2+(540*((nowCamidx-6)/2));
	int w=lastRect[targetidx].width/2;
	int h=lastRect[targetidx].height/2;//取出容器中rect的值
	int endx=startx+w;
	int endy=starty+h;
	double r=RectColor[targetidx][0];
	double g=RectColor[targetidx][1];
	double b=RectColor[targetidx][2];
if(capidx==MAIN_FPGA_SIX)
{
	if(nowCamidx>=0 &&nowCamidx<=5 )//0~5
	{
		m6[targetidx].data=src;
		cv::rectangle(m6[targetidx],cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(r,g,b),3);
		SetRoiSrc(targetidx,startx,starty,w,h,nowCamidx);
	}
}
	else if(capidx==MAIN_FPGA_FOUR)
	{//6~9
		if(nowCamidx>=6 &&nowCamidx<=9 )
		{
			m4[targetidx].data=src;
			cv::rectangle(m6[targetidx],cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(r,g,b),3);
			SetRoiSrc(targetidx,startx,starty,w,h,nowCamidx);
		}
	}
}

Rect2f *MotionDetectorROI::Rank(int targetidx,int bigOrsmall)
{
	lastRect[targetidx];
	RankRect[targetidx];

	if(bigOrsmall==BIG)
	{
	//	return ;
	}
	else if(bigOrsmall==SMALL)
	{
//		return ;
	}
}

