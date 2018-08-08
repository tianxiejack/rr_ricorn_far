/*
 * mvdetectInterface.h
 *
 *  Created on: 2017年9月4日
 *      Author: cr
 */
#ifndef _MVDECTINTERFACE_H_
#define _MVDECTINTERFACE_H_
#include "StlGlDefines.h"
#include "IF_MvDetect.h"

typedef struct _OSA_SemHndl OSA_SemHndl,* pOSA_SemHndl;
typedef void ( *LPNOTIFYFUNC)(void *context, int chId);

class	CMvDectInterface
{
public:
	CMvDectInterface(){};
	virtual ~CMvDectInterface(){};

public:
	virtual	int	init(LPNOTIFYFUNC	notifyFunc, void *context){return 1;};
	virtual	int destroy(){return 1;};
	virtual	void	setFrame(cv::Mat	src ,int srcwidth , int srcheight ,int chId,int accuracy=4,/*0~4  4*/int inputArea=8/*6*/,int inputMaxArea=200,int threshold = 30){};//输入视频帧
	virtual	void	setWarningRoi(std::vector<cv::Point2i>	warnRoi,	int chId	= 0){};//设置缩放前的警戒区域
	virtual	void	setWarnMode(WARN_MODE	warnMode,	int chId	= 0){};//设置警戒模式
	virtual	void	getMoveTarget(std::vector<TRK_RECT_INFO>	&resTarget,	int chId	= 0){};//移动目标
	virtual void	mvPause(){};
};

CMvDectInterface *MvDetector_Create();

class BaseMvDetect:public IF_MvDetect
{
public:
	BaseMvDetect();
	virtual ~BaseMvDetect(){
		for(int i=0;i<CAM_COUNT;i++)
		{
			outRect[i].clear();
		}
		m_WholeRect[0].clear();
		m_WholeRect[1].clear();
	}
	 void	 DrawRectOnpic(unsigned char *src,int capidx,int cc);
	 std::vector<mvRect> *Getm_WholeRect(int mainOrsub)
	{
		m_WholeRect[mainOrsub].clear();
		for(int i=0;i<CAM_COUNT;i++)
			m_WholeRect[mainOrsub].insert(m_WholeRect[mainOrsub].end(),outRect[i].begin(),outRect[i].end());
		return &m_WholeRect[mainOrsub];
	}
	virtual	 void MsetFirst(){};
	virtual	 void MdeleteZombie(){};
	void uyvy2gray(unsigned char* src,unsigned char* dst,int idx,int width=MAX_SCREEN_WIDTH,int height=MAX_SCREEN_HEIGHT);
	void SetLineY(int idx,int startY){/*lineY[idx]=startY;*/};
	void SetLinedalta(int idx,int delta){/*linedelta[idx]=delta;*/};
	void ReSetLineY();
	float GetRoiStartY_OffsetCoefficient(int idx);
    int lineY[CAM_COUNT];
    int linedelta[CAM_COUNT];
    float half_RoiAreah;
	unsigned char* grayFrame[CAM_COUNT];
	std::vector<mvRect>	outRect[CAM_COUNT];
	std::vector<mvRect> m_WholeRect[2];
};

class MvDetectV2:public BaseMvDetect
{
public:
	MvDetectV2();
	~MvDetectV2();
	void init(int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void m_mvDetect(int idx,unsigned char* inframe,int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void SetoutRect();//将检测到的每个通道里6个rect放入对应的6个容器里
	void ClearAllVector(bool IsOpen);
	static  void NotifyFunc(void *context, int chId);
	pOSA_SemHndl GetpSemMV(int idx){return pSemMV[idx];};
	std::vector<TRK_RECT_INFO> * GetptempRect_Srcptr(int idx)
		{
			return &tempRect_Srcptr[idx];
		};
private:
	CMvDectInterface *m_pMovDetector;
    pOSA_SemHndl pSemMV[CAM_COUNT];
    std::vector<TRK_RECT_INFO>  tempRect_Srcptr[CAM_COUNT];
};









#endif /* MVDECTINTERFACE_H_ */
