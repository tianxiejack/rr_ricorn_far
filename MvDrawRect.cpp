#include "MvDrawRect.h"
#include"StlGlDefines.h"
#include <stdio.h>
#include"RoiFocusCamidx.h"

#if 1
extern unsigned char * p_newestMvSrc[CAM_COUNT];
#if MVDECT
extern MvDetect mv_detect;
MotionDetectorROI   mdRoi_mainA(4,&mv_detect,MAIN),mdRoi_subA(4,&mv_detect,SUB);
#endif

MotionDetectorROI::MotionDetectorROI(int sumTarget,MvDetect *pmv,int mainOrsub):
m_pmv(pmv),m_sumTarget(sumTarget),m_MAIN_SUB(mainOrsub)
{
	for(int i=0;i<m_sumTarget;i++)
	{
		targetRect[i].camIdx=0;
		targetRect[i].outRect.x=200;
		targetRect[i].outRect.y=200;
		targetRect[i].outRect.width=400;
		targetRect[i].outRect.height=400;

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

void MotionDetectorROI:: DrawAllRectOri(int mainOrsub,int fourOrsix)
{
	vector<mvRect>::iterator  it;
	vector<mvRect> *wholeV;
	vector<mvRect> *wholeVrcv;
	vector<mvRect> tempV;
	wholeVrcv=m_pmv->Getm_WholeRect(mainOrsub);
		for(int i=0;i<wholeVrcv->size();i++)
		{
				tempV.push_back((*wholeVrcv)[i]);
		}
	wholeV=&tempV;

		//排序
	if(m_MAIN_SUB==SUB)
	sort(wholeV->begin(),wholeV->end(),CmpCamIdxSmaller);
	else if(m_MAIN_SUB==MAIN)
	sort(wholeV->begin(),wholeV->end(),CmpCamIdxSmallerMAIN);

		/*****找出所以的框*****/
		for(int i=0;i<wholeV->size();i++)
		{
			if(wholeV->size()>m_sumTarget)//个数大于m_sumTarget
			{
				if(i>=0&&i<=m_sumTarget) //前m_sumTarget个
				{
						targetRect[i]=(*wholeV)[i];
					//将前m_sumTarget个保留以显示图片
				}
			}
			else if(wholeV->size()<=m_sumTarget)//个数小于等于m_sumTarget 全绿色
			{
				targetRect[i]=(*wholeV)[i];//将前n保留以显示图片
			}
		}
		//个数小于m_sumTarget 个，其余的 targetRect lastRect的angle 为-1
		 if(wholeV->size()<=m_sumTarget)
		 {
			for(int k=wholeV->size();k<m_sumTarget;k++)
			{
				targetRect[k]=targetRect[0];	//如果实际目标比需求目标少，则剩下的需求目标也是目标0
			}
		 }
}

unsigned char * MotionDetectorROI::GetRoiSrc(int targetidx)
{
	mvRect temp;
	int midx=-1,
		 midy=-1,
		drawx=-1,
		drawy=-1,
		draww=ROIW,
		drawh=ROIH;
		temp=targetRect[targetidx];

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

return RoiSrc[targetidx];
}
void MotionDetectorROI::RectfromSrc(int fourOrsix,int targetidx,int camIdx,int x,int y,int w,int h)
{
	if(p_newestMvSrc[camIdx]==NULL)
	{
	//	printf("p_newestMvSrc %d==NULL",camIdx);
	}
	else if(p_newestMvSrc[camIdx]!=NULL)
	{
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
			memcpy(RoiSrc[targetidx]+j*w*3, out.row(j).data,w*3);
		}
	}
}
#endif
