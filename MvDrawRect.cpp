#include "MvDrawRect.h"
#include"StlGlDefines.h"


MotionDetectorROI::MotionDetectorROI(int sumTarget,MvDetect *pmv):
m_pmv(pmv),m_sumTarget(sumTarget)
{
	m4=(Mat(FPGA_SCREEN_HEIGHT,FPGA_SCREEN_WIDTH,CV_8UC4));
		m6=(Mat(SDI_HEIGHT,SDI_WIDTH,CV_8UC4));
	for(int i=0;i<m_sumTarget;i++)
	{

	chooseNext[i]=false;
	choosePre[i]=false;
	lastRect[i].outRect.height=-1;
	lastRect[i].outRect.width=-1;
	lastRect[i].outRect.x=-1;
	lastRect[i].outRect.y=-1;
	lastRect[i].x_angle=-1;
	lastRect[i].y_angle=-1;
	//for(int j=0;j<CC;j++)
		//RectColor[i][j]=0;
	RoiSrc[i]=(unsigned char *)malloc(ROIW*ROIH*4);
	}
//	tempSrc4=(unsigned char *)malloc(FPGA_SCREEN_HEIGHT*FPGA_SCREEN_WIDTH*4);
//	tempSrc6=(unsigned char *)malloc(SDI_HEIGHT*SDI_WIDTH*4);
}

bool CmpXsamller(const mvRect &mv1,const mvRect  &mv2)
{
	return mv1.x_angle < mv2.x_angle;
}
bool CmpXbigger(const mvRect &mv1,const mvRect  &mv2)
{
	return mv1.x_angle > mv2.x_angle;
}
bool CmpYsmaller(const mvRect &mv1,const mvRect  &mv2)
{
	return mv1.y_angle < mv2.y_angle;
}
bool CmpYbigger(const mvRect &mv1,const mvRect  &mv2)
{
	return mv1.y_angle > mv2.y_angle;
}
void  MotionDetectorROI::MRectangle(int fourOrsix,mvRect *p)
{
	int startx,starty,endx,endy,w,h;
	if(fourOrsix==MAIN_FPGA_SIX)
	{
		 startx=p->outRect.x/3+(640*(p->camIdx%3));
		 starty=p->outRect.y/2+(540*(p->camIdx/3));
		 w=p->outRect.width/3;
		 h=p->outRect.height/2;//取出容器中rect的值
		 endx=startx+w;
		 endy=starty+h;
		cv::rectangle(m6,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(p->color[0],p->color[1],p->color[2]),2);
	}
	else if (fourOrsix==MAIN_FPGA_FOUR)
	{
		 startx=p->outRect.x/2+(640*((p->camIdx-6)%2));
		 starty=p->outRect.y/2+(540*((p->camIdx-6)/2));
		 w=p->outRect.width/2;
		 h=p->outRect.height/2;//取出容器中rect的值
		 endx=startx+w;
		 endy=starty+h;
		cv::rectangle(m4,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(p->color[0],p->color[1],p->color[2]),2);
	}
}
void MotionDetectorROI:: DrawAllRectOri(int fourOrsix)
{
	vector<mvRect>::iterator  it;
	vector<mvRect> *wholeV;
	vector<mvRect> *wholeVrcv;
	vector<mvRect> tempV;
	wholeVrcv=m_pmv->GetWholeRect();
//将范围内的rect找出
	if(range[END]>range[START]){					//不跨360度
		for(int i=0;i<wholeVrcv->size();i++)
		{
			if((*wholeVrcv)[i].x_angle>range[START]   && (*wholeVrcv)[i].x_angle<=range[END] )
			{
				tempV.push_back((*wholeVrcv)[i]);
			}
		}
	}
	else{//跨360度
		for(int i=0;i<wholeVrcv->size();i++)
		{
			if((*wholeVrcv)[i].x_angle>range[START]   && (*wholeVrcv)[i].x_angle<=360.0)
			{
				tempV.push_back((*wholeVrcv)[i]);
			}
			for(int i=0;i<wholeVrcv->size();i++)
			{
				if((*wholeVrcv)[i].x_angle>0   && (*wholeVrcv)[i].x_angle<=range[END] )
				{
					tempV.push_back((*wholeVrcv)[i]);
				}
			}
		}
	}
	wholeV=&tempV;

		//按x从小到达排序
		sort(wholeV->begin(),wholeV->end(),CmpXsamller);
		/*****找出所以的框*****/
		for(int i=0;i<wholeV->size();i++)
		{
			if(wholeV->size()>m_sumTarget)//个数大于m_sumTarget
			{
				if(i>=0&&i<=m_sumTarget) //前m_sumTarget个绿色
				{
					(*wholeV)[i].color[0]=0;
					(*wholeV)[i].color[1]=255.0;
					(*wholeV)[i].color[2]=0;
					targetRect[i]=(*wholeV)[i];  //将前m_sumTarget个绿色的保留以显示图片
					lastRect[i]=targetRect[i];//黄色target默认targetRect
				}
				else//红色
				{
					(*wholeV)[i].color[0]=255.0;
					(*wholeV)[i].color[1]=0;
					(*wholeV)[i].color[2]=0;
				}
			}
			else if(wholeV->size()<=m_sumTarget)//个数小于等于m_sumTarget 全绿色
			{
				(*wholeV)[i].color[0]=0;
				(*wholeV)[i].color[1]=255.0;
				(*wholeV)[i].color[2]=0;
				targetRect[i]=(*wholeV)[i];//将前n个绿色的保留以显示图片
				lastRect[i]=targetRect[i];//黄色target默认targetRect
			}
		}
		//个数小于m_sumTarget 个，其余的 targetRect lastRect的angle 为-1
		 if(wholeV->size()<=m_sumTarget)
		 {
			int j=m_sumTarget-wholeV->size();
			for(int k=wholeV->size();k<j;k++)
			{
				targetRect[k].x_angle=-1;
				lastRect[k]=targetRect[k];
			}
		 }

		if(ISanySingleRect())//如果有Singletarget在，则再画黄色
		{
			//m_sumTarget 个目标
		for(int targetidx=0;targetidx<m_sumTarget;targetidx++)
		{
	//如果targetidx圈出的rect选择下一个
	 if(IsChooseN(targetidx))
	 {
		 //在所有从小到大排序好的vector中插入lastrect[targetidx],找出lastrect的下一个作为黄色rect
		 //并将这个rect赋值给lastrect[targetidx]
			for(int i=0;i<wholeV->size();i++)
			{
				//如果找到第一个大于last的，则跳出循环
				if(lastRect[targetidx].x_angle<(*wholeV)[i].x_angle)
				{
					(*wholeV)[i].color[0]=0;
					(*wholeV)[i].color[1]=255;
					(*wholeV)[i].color[2]=255;
					lastRect[targetidx]=(*wholeV)[i];
					break;
				}
				//如果没找，没有break出循环，则把第最小的给到lastrect，相当于转了一圈回来
				lastRect[targetidx]=(*wholeV)[0];
			}
	 }//next
	 else if(IsChooseP(targetidx))
		 {
		 	 sort(wholeV->begin(),wholeV->end(),CmpXbigger);
			 //在所有从大到小排序好的vector中插入lastrect[targetidx],找出lastrect的上一个作为黄色rect
			 //并将这个rect赋值给lastrect[targetidx]
				for(int i=0;i<wholeV->size();i++)
				{
					//如果找到第一个小于last的，则跳出循环
					if(lastRect[targetidx].x_angle>(*wholeV)[i].x_angle)
					{
						(*wholeV)[i].color[0]=0;
						(*wholeV)[i].color[1]=255;
						(*wholeV)[i].color[2]=255;
						lastRect[targetidx]=(*wholeV)[i];
						break;
					}
					//如果没找，没有break出循环，则把第最大的给到lastrect，相当于转了一圈回来
					lastRect[targetidx]=(*wholeV)[0];
				}
		 }//pre
	 else if(IsChooseUp(targetidx))
		 {
		 	 sort(wholeV->begin(),wholeV->end(),CmpYbigger);
			 //在所有从大到小排序好的vector中插入lastrect[targetidx],找出lastrect的上一个作为黄色rect
			 //并将这个rect赋值给lastrect[targetidx]
				for(int i=0;i<wholeV->size();i++)
				{
					//如果找到第一个小于last的，则跳出循环
					if(lastRect[targetidx].y_angle>(*wholeV)[i].y_angle)
					{
						(*wholeV)[i].color[0]=0;
						(*wholeV)[i].color[1]=255;
						(*wholeV)[i].color[2]=255;
						lastRect[targetidx]=(*wholeV)[i];
						break;
					}
					//如果没找，没有break出循环，则把第最大的给到lastrect，相当于转了一圈回来
					lastRect[targetidx]=(*wholeV)[0];
				}
		 }//up
	 else if(IsChooseDown(targetidx))
		 {
		 	 sort(wholeV->begin(),wholeV->end(),CmpYsmaller);
			 //在所有从小到大排序好的vector中插入lastrect[targetidx],找出lastrect的下一个作为黄色rect
			 //并将这个rect赋值给lastrect[targetidx]
				for(int i=0;i<wholeV->size();i++)
				{
					//如果找到第一个大于last的，则跳出循环
					if(lastRect[targetidx].y_angle<(*wholeV)[i].y_angle)
					{
						(*wholeV)[i].color[0]=0;
						(*wholeV)[i].color[1]=255;
						(*wholeV)[i].color[2]=255;
						lastRect[targetidx]=(*wholeV)[i];
						break;
					}
					//如果没找，没有break出循环，则把第最小的给到lastrect，相当于转了一圈回来
					lastRect[targetidx]=(*wholeV)[0];
				}
		 }//down
			}//从0～targetNUM次

		}//有target在

		/*******把绿色 红色 黄色 全画了****/
				if(fourOrsix==MAIN_FPGA_FOUR)//只画cam 6~9
				{
					for(int i=0;i<wholeV->size();i++)
					{
						if((*wholeV)[i].camIdx>=6 &&(*wholeV)[i].camIdx<=9)
						{
							MRectangle(MAIN_FPGA_FOUR,&(*wholeV)[i]);
						}
					}
				}
				else if(fourOrsix==MAIN_FPGA_SIX)//只画cam 0~5
				{
					for(int i=0;i<wholeV->size();i++)
					{
						if((*wholeV)[i].camIdx>=0&&(*wholeV)[i].camIdx<=5)
						{
							MRectangle(MAIN_FPGA_SIX,&(*wholeV)[i]);
						}
					}
				}
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
#if 0
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
#endif
unsigned char * MotionDetectorROI::GetRoiSrc(int targetidx)
{
	mvRect temp;
	int transx=-1,
		 transy=-1,
		 transw=-1,
		 transh=-1,
		 midx=-1,
		 midy=-1,
		drawx=-1,
		drawy=-1,
		draww=ROIW,
		drawh=ROIH;
	//是否有选择黄框？有的画就取lastrect位置的图
	if(ISselectSingleRect(targetidx))
	{
		temp=lastRect[targetidx];
	}
	//没有的话就取t位置的图
	else
	{
		temp=targetRect[targetidx];
	}
	 if(temp.camIdx>=0 &&temp.camIdx<=5)//1920*1080
	 {
		 //先到1920*1080的位置
		 transx=temp.outRect.x/3+(640*(temp.camIdx%3));
		 transy=temp.outRect.y/2+(540*(temp.camIdx/3));
		 transw=temp.outRect.width/3;
		 transh=temp.outRect.height/2;
		 midx=transx+transw/2;
		 midy=transy+transh/2;
		 //再按照mid取出对应的roi数据
		 //如果中间点到该相机图像边界左边的距离小于1920/3/4/2
		 if(midx-(temp.camIdx%3)*640<HALF_ROIW)
		 {
			 //左边起点，加半个ROIW
			 midx=HALF_ROIW+(temp.camIdx%3)*640;
		 }
		 else if(((temp.camIdx%3)+1)*640-midx<HALF_ROIW)
		 {
			 //右边起点，减半个ROIW
			 midx=((temp.camIdx%3)+1)*640-HALF_ROIW;
		 }

		 //如果中间点到图像上边的距离小于1080/2/4/2
		 if(midy-(temp.camIdx/3)*540<HALF_ROIH)
		 {
			 //上边起点，加半个ROIH
			 midy=HALF_ROIH+(temp.camIdx/3)*540;
		 }
		 else if(((temp.camIdx/3)+1)*540-midy<HALF_ROIH)
		 {
			 //下边起点，减半个ROIH
			 midy=((temp.camIdx/3)+1)*540-HALF_ROIH;
		 }
		 drawx=midx-HALF_ROIW;
		 drawy=midy-HALF_ROIH;
		 RectfromSrc(MAIN_FPGA_SIX,targetidx,drawx,drawy,draww,drawh);
	 }
/////////////////////////////////////////////
	 else if(temp.camIdx>=6 &&temp.camIdx<=9)//1280*1080
	 {
		 //先到1280*1080的位置
		 transx=temp.outRect.x/2+(640*((temp.camIdx-6)%2));
		 transy=temp.outRect.y/2+(540*((temp.camIdx-6)/2));
		 transw=temp.outRect.width/2;
		 transh=temp.outRect.height/2;
		 midx=transx+transw/2;
		 midy=transy+transh/2;
		 //再按照mid取出对应的roi数据
		 //如果中间点到该相机图像边界左边的距离小于1920/3/4/2
		 		 if(midx-((temp.camIdx-6)%2)*640<HALF_ROIW)
		 		 {
		 			 //左边起点，加半个ROIW
		 			 midx=HALF_ROIW+((temp.camIdx-6)%3)*640;
		 		 }
		 		 else if((((temp.camIdx-6)%2)+1)*640-midx<HALF_ROIW)
		 		 {
		 			 //右边起点，减半个ROIW
		 			 midx=(((temp.camIdx-6)%2)+1)*640-HALF_ROIW;
		 		 }

		 		 //如果中间点到图像上边的距离小于1080/2/4/2
		 		 if(midy-((temp.camIdx-6)/2)*540<HALF_ROIH)
		 		 {
		 			 //上边起点，加半个ROIH
		 			 midy=HALF_ROIH+((temp.camIdx-6)/3)*540;
		 		 }
		 		 else if((((temp.camIdx-6)/2)+1)*540-midy<HALF_ROIH)
		 		 {
		 			 //下边起点，减半个ROIH
		 			 midy=(((temp.camIdx-6)/2)+1)*540-HALF_ROIH;
		 		 }
		 		 drawx=midx-HALF_ROIW;
		 		 drawy=midy-HALF_ROIH;
		 		RectfromSrc(MAIN_FPGA_FOUR,targetidx,drawx,drawy,draww,drawh);
	 }
}
void MotionDetectorROI::RectfromSrc(int fourOrsix,int targetidx,int x,int y,int w,int h)
{
	if(fourOrsix==MAIN_FPGA_SIX)
	{
		Mat SRC(1080,1920,CV_8UC4,tempSrc6);
		Mat DST(h,w,CV_8UC4,RoiSrc[targetidx]);
		Rect rect(x,y,w,h);
		DST=SRC(rect);
	}
	else if(fourOrsix==MAIN_FPGA_FOUR)
	{
		Mat SRC(1080,1280,CV_8UC4,tempSrc4);
		Mat DST(h,w,CV_8UC4,RoiSrc[targetidx]);
		Rect rect(x,y,w,h);
		DST=SRC(rect);
	}
}
void MotionDetectorROI::drawRect(int targetidx,unsigned char *src,int capidx)
{
#if 0
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
#endif
}

mvRect *MotionDetectorROI::Rank(int targetidx,int bigOrsmall)
{
	//lastRect[targetidx];
//	RankRect[targetidx];

	if(bigOrsmall==BIG)
	{
	//	return ;
	}
	else if(bigOrsmall==SMALL)
	{
//		return ;
	}
}

