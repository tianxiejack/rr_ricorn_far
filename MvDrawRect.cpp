#include "MvDrawRect.h"
#include"StlGlDefines.h"
#include <stdio.h>
#include"RoiFocusCamidx.h"

#if 1
extern unsigned char * p_newestMvSrc[CAM_COUNT];
#if MVDECT
extern MvDetect mv_detect;
MotionDetectorROI   mdRoi_mainT(2,&mv_detect,MAIN),mdRoi_subT(2,&mv_detect,SUB);
MotionDetectorROI   mdRoi_mainA(4,&mv_detect,MAIN),mdRoi_subA(4,&mv_detect,SUB);
#endif


bool toprint=false;
MotionDetectorROI::MotionDetectorROI(int sumTarget,MvDetect *pmv,int mainOrsub):
m_pmv(pmv),m_sumTarget(sumTarget),m_MAIN_SUB(mainOrsub)
{
	range[0]=0;
	range[1]=MAX_X_POS-1;
	m4=(Mat(FPGA_SCREEN_HEIGHT,FPGA_SCREEN_WIDTH,CV_8UC4));
		m6=(Mat(SDI_HEIGHT,SDI_WIDTH,CV_8UC4));
		moveN=false;
		moveP=false;
	for(int i=0;i<m_sumTarget;i++)
	{
		targetRect[i].camIdx=0;
		targetRect[i].outRect.x=200;
		targetRect[i].outRect.y=200;
		targetRect[i].outRect.width=400;
		targetRect[i].outRect.height=400;
	chooseNext[i]=false;
	choosePre[i]=false;
	lastRect[i].outRect.height=-1;
	lastRect[i].outRect.width=-1;
	lastRect[i].outRect.x=-1;
	lastRect[i].outRect.y=-1;
	//for(int j=0;j<CC;j++)
		//RectColor[i][j]=0;
	RoiSrc[i]=(unsigned char *)malloc(ROIW*ROIH*3);
	}
//	tempSrc4=(unsigned char *)malloc(FPGA_SCREEN_HEIGHT*FPGA_SCREEN_WIDTH*4);
//	tempSrc6=(unsigned char *)malloc(SDI_HEIGHT*SDI_WIDTH*4);
}

bool CmpAREAbigger( const mvRect &mv1, const mvRect  &mv2);
bool CmpCamIdxSmaller( const mvRect &mv1, const mvRect  &mv2);
bool CmpXsamller( const mvRect &mv1, const mvRect  &mv2);
bool CmpXbigger(const mvRect &mv1,const mvRect  &mv2);
bool CmpYsmaller(const mvRect &mv1,const mvRect  &mv2);
bool CmpYbigger(const mvRect &mv1,const mvRect  &mv2);
bool CmpAREAbigger( const mvRect &mv1, const mvRect  &mv2)
{
	float area1=mv1.outRect.x*mv1.outRect.y;
	float area2=mv2.outRect.x*mv2.outRect.y;
	if(area1==area2)
	{
		return CmpXsamller( mv1,mv2);
	}
	return area1>area2;
}
bool CmpCamIdxSmaller( const mvRect &mv1, const mvRect  &mv2)
{
	int idx1=mv1.camIdx;
	int idx2=mv2.camIdx;
	if(idx1==idx2)
	{
		return CmpAREAbigger( mv1, mv2);
	}
	else
	{
		int focus=RoiFocusCamidx::GetInstance()->GetRoiFocusCamidx();
		if(idx1 < focus)idx1 += CAM_COUNT;
		if(idx2 < focus)idx2 += CAM_COUNT;
		return idx1<=idx2;
	}
}

bool CmpCamIdxSmallerMAIN( const mvRect &mv1, const mvRect  &mv2)
{
	int idx1=mv1.camIdx;
	int idx2=mv2.camIdx;
	if(idx1==idx2)
	{
		return CmpAREAbigger( mv1, mv2);
	}
	else
	{
		int focus=RoiFocusCamidx::GetMainInstance()->GetRoiFocusCamidx();
		if(idx1 < focus)idx1 += CAM_COUNT;
		if(idx2 < focus)idx2 += CAM_COUNT;
		return idx1<=idx2;
	}
}
bool CmpXsamller( const mvRect &mv1, const mvRect  &mv2)
{
	if(mv1.x_angle()!= mv2.x_angle())
	{
		return mv1.x_angle() < mv2.x_angle();
	}
	else if(mv1.x_angle() == mv2.x_angle()
			||mv1.y_angle() != mv2.y_angle())
	{
		return mv1.y_angle() < mv2.y_angle();
	}
	else if(mv1.x_angle() == mv2.x_angle()
			||mv1.y_angle() == mv2.y_angle())
	{
		if(mv1.outRect.width!=mv2.outRect.width)
		{
			return mv1.outRect.width<mv2.outRect.width;
		}
		else if(mv1.outRect.width==mv2.outRect.width)
		{
			return mv1.outRect.height<mv2.outRect.height;
		}
	}
}
bool CmpXbigger(const mvRect &mv1,const mvRect  &mv2)
{
	if(mv1.x_angle() != mv2.x_angle())
	{
		return mv1.x_angle() > mv2.x_angle();
	}
	else if(mv1.x_angle() == mv2.x_angle()
			||mv1.y_angle() != mv2.y_angle())
	{
		return mv1.y_angle() > mv2.y_angle();
	}
	else if(mv1.x_angle() == mv2.x_angle()
				||mv1.y_angle() == mv2.y_angle())
		{
			if(mv1.outRect.width!=mv2.outRect.width)
			{
				return mv1.outRect.width>mv2.outRect.width;
			}
			else if(mv1.outRect.width==mv2.outRect.width)
			{
				return mv1.outRect.height>mv2.outRect.height;
			}
		}
}
bool CmpYsmaller(const mvRect &mv1,const mvRect  &mv2)
{
	if( mv1.y_angle() != mv2.y_angle())
	{
		return mv1.y_angle() < mv2.y_angle();
	}
	else if( mv1.y_angle() == mv2.y_angle()
			||mv1.x_angle()!=mv2.x_angle())
	{
		return mv1.x_angle() < mv2.x_angle();
	}
	else if(mv1.y_angle() == mv2.y_angle()
				||mv1.x_angle() == mv2.x_angle())
	{
		if(mv1.outRect.height!=mv2.outRect.height)
		{
			return mv1.outRect.height<mv2.outRect.height;
		}
		else if(mv1.outRect.height==mv2.outRect.height)
		{
			return mv1.outRect.width<mv2.outRect.width;
		}
	}
}
bool CmpYbigger(const mvRect &mv1,const mvRect  &mv2)
{
	if( mv1.y_angle() != mv2.y_angle())
		{
			return mv1.y_angle() > mv2.y_angle();
		}
		else if( mv1.y_angle() == mv2.y_angle()
				||mv1.x_angle()!=mv2.x_angle())
		{
			return mv1.x_angle() > mv2.x_angle();
		}
		else if(mv1.y_angle() == mv2.y_angle()
						||mv1.x_angle() == mv2.x_angle())
			{
				if(mv1.outRect.height!=mv2.outRect.height)
				{
					return mv1.outRect.height>mv2.outRect.height;
				}
				else if(mv1.outRect.height==mv2.outRect.height)
				{
					return mv1.outRect.width>mv2.outRect.width;
				}
			}
}
void  MotionDetectorROI::MRectangle(int fourOrsix,mvRect *p)
{
	int startx,starty,endx,endy,w,h;
	if(fourOrsix==MAIN_FPGA_SIX)
	{
		 startx=p->outRect.x/3;
		 starty=p->outRect.y/2+(540*p->camIdx);
		 w=p->outRect.width/3;
		 h=p->outRect.height/2;//取出容器中rect的值
		 endx=startx+w;
		 endy=starty+h;
		cv::rectangle(m6,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(p->color[0],p->color[1],p->color[2]),2);
	}
	else if (fourOrsix==MAIN_FPGA_FOUR)
	{
		 startx=p->outRect.x/3;
		 starty=p->outRect.y/2+(540*(p->camIdx-6));
		 w=p->outRect.width/3;
		 h=p->outRect.height/2;//取出容器中rect的值
		 endx=startx+w;
		 endy=starty+h;
		cv::rectangle(m4,cvPoint(startx,starty),cvPoint(endx,endy),cvScalar(p->color[0],p->color[1],p->color[2]),2);
	}
}
void MotionDetectorROI:: DrawAllRectOri(int mainOrsub,int fourOrsix)
{
	vector<mvRect>::iterator  it;
	vector<mvRect> *wholeV;
	vector<mvRect> *wholeVrcv;
	vector<mvRect> tempV;
	wholeVrcv=m_pmv->Getm_WholeRect(mainOrsub);

//将范围内的rect找出
	if(range[END]>range[START]){					//不跨360度
		for(int i=0;i<wholeVrcv->size();i++)
		{
			if((*wholeVrcv)[i].x_angle()>range[START]   && (*wholeVrcv)[i].x_angle()<=range[END] )
			{
				tempV.push_back((*wholeVrcv)[i]);
			}
		}
	}
	else{//跨360度
		for(int i=0;i<wholeVrcv->size();i++)
		{
			if((*wholeVrcv)[i].x_angle()>range[START]   && (*wholeVrcv)[i].x_angle()<=(MAX_X_POS-1))
			{
				tempV.push_back((*wholeVrcv)[i]);
			}
			for(int i=0;i<wholeVrcv->size();i++)
			{
				if((*wholeVrcv)[i].x_angle()>0   && (*wholeVrcv)[i].x_angle()<=range[END] )
				{
					tempV.push_back((*wholeVrcv)[i]);
				}
			}
		}
	}
if(toprint)
{
	for(int i=0;i<tempV.size();i++)
	{
		printf("i=%d START=%f  END=%f x_angle()=%f\n",i,range[START],range[END],tempV[i].x_angle());
	}
}
	wholeV=&tempV;

		//按x从小到达排序
		//sort(wholeV->begin(),wholeV->end(),CmpXsamller);
	if(m_MAIN_SUB==SUB)
	sort(wholeV->begin(),wholeV->end(),CmpCamIdxSmaller);
	else if(m_MAIN_SUB==MAIN)
		sort(wholeV->begin(),wholeV->end(),CmpCamIdxSmallerMAIN);
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
						targetRect[i]=(*wholeV)[i];
					//将前m_sumTarget个绿色的保留以显示图片
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
			//	targetRect[k].outRect.x=-1; //如果target的camIdx  outRect.x是-1,則表示沒有這個target
			//	targetRect[k].camIdx=-1;	//保證get_xangle()<0
			//	lastRect[k]=targetRect[k];
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
		 resetChooseN(targetidx);
		 //在所有从小到大排序好的vector中插入lastrect[targetidx],找出lastrect的下一个作为黄色rect
		 //并将这个rect赋值给lastrect[targetidx]
			for(int i=0;i<wholeV->size();i++)
			{
				//如果找到第一个大于last的，则跳出循环
				if(lastRect[targetidx].x_angle()<(*wholeV)[i].x_angle())
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
		 resetChooseP(targetidx);
		 	 sort(wholeV->begin(),wholeV->end(),CmpXbigger);
			 //在所有从大到小排序好的vector中插入lastrect[targetidx],找出lastrect的上一个作为黄色rect
			 //并将这个rect赋值给lastrect[targetidx]
				for(int i=0;i<wholeV->size();i++)
				{
					//如果找到第一个小于last的，则跳出循环
					if(lastRect[targetidx].x_angle()>(*wholeV)[i].x_angle())
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
		 resetChooseUp(targetidx);
		 	 sort(wholeV->begin(),wholeV->end(),CmpYbigger);
			 //在所有从大到小排序好的vector中插入lastrect[targetidx],找出lastrect的上一个作为黄色rect
			 //并将这个rect赋值给lastrect[targetidx]
				for(int i=0;i<wholeV->size();i++)
				{
					//如果找到第一个小于last的，则跳出循环
					if(lastRect[targetidx].y_angle()>(*wholeV)[i].y_angle())
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
		 resetChooseDown(targetidx);
		 	 sort(wholeV->begin(),wholeV->end(),CmpYsmaller);
			 //在所有从小到大排序好的vector中插入lastrect[targetidx],找出lastrect的下一个作为黄色rect
			 //并将这个rect赋值给lastrect[targetidx]
				for(int i=0;i<wholeV->size();i++)
				{
					//如果找到第一个大于last的，则跳出循环
					if(lastRect[targetidx].y_angle()<(*wholeV)[i].y_angle())
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

		else //沒有target ,移動所有的
		{
			if(IsMoveN())
			{
				resetMoveN();
				sort(wholeV->begin(),wholeV->end(),CmpXsamller);

			}
			else if(IsMoveP())
			{
				resetMoveP();
				sort(wholeV->begin(),wholeV->end(),CmpXbigger);
			}

		}
#if 0
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
#endif
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
	int midx=-1,
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
	drawx=temp.outRect.x;
	drawy=temp.outRect.y;
	draww=temp.outRect.width;
	drawh=temp.outRect.height;
	 midx=drawx+draww/2;
	 midy=drawy+drawh/2;

if(midx<HALF_ROIW)
	drawx=0;
else if(midx>=SDI_WIDTH-HALF_ROIW)
	drawx=SDI_WIDTH-ROIW-1;
if(midy<HALF_ROIH)
	drawy=0;
else if(midy>=SDI_HEIGHT-HALF_ROIH)
	drawy=SDI_HEIGHT-ROIH-1;


//printf("targetidx=%d  camidx=%d  drawx=%d drawy=%d,w=%d,=%d\n",targetidx,temp.camIdx,drawx,drawy,draww,drawh);
	RectfromSrc(-1,targetidx,temp.camIdx,drawx,drawy,draww,drawh);


/*	 if(temp.camIdx>=0 &&temp.camIdx<=5)//1920*1080
	 {
		 RectfromSrc(MAIN_FPGA_SIX,targetidx,drawx,drawy,draww,drawh);
	 }
/////////////////////////////////////////////
	 else if(temp.camIdx>=6 &&temp.camIdx<=9)//1280*1080
	 {
		 		RectfromSrc(MAIN_FPGA_FOUR,targetidx,drawx,drawy,draww,drawh);
	 }*/
return RoiSrc[targetidx];
}
void MotionDetectorROI::RectfromSrc(int fourOrsix,int targetidx,int camIdx,int x,int y,int w,int h)
{
	if(p_newestMvSrc[camIdx]==NULL)
	{
		printf("p_newestMvSrc %d==NULL",camIdx);
	}
	else if(p_newestMvSrc[camIdx]!=NULL)
	{
	//	x=200;
	//	y=200;
	//	w=400;
	//	h=400;
		if(x<0)
		{
			x=0;
		}
		else if(x>1519)
			x=1519;
		if(y<0)
		{
			y=0;
		}
		else if(y>679)
				y=679;
		w=400;
		h=400;
		Mat SRC(1080,1920,CV_8UC3,p_newestMvSrc[camIdx]);
		Mat out(h,w,CV_8UC3);
		Rect rect(x,y,w,h);
		out=SRC(rect);
		for(int j = 0; j< h; j++){
			//unsigned char* pDst = RoiSrc[targetidx]+j*w*3;
			memcpy(RoiSrc[targetidx]+j*w*3, out.row(j).data,w*3);
		}
	}
	/*	if(fourOrsix==MAIN_FPGA_SIX)
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
	}*/
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
#endif
