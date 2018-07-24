/*
 * IF_MvDetect.h
 *
 *  Created on: 2018年7月21日
 *      Author: fsmdn121
 */
#include "infoHead.h"
#ifndef IF_MVDETECT_H_
#define IF_MVDETECT_H_

#define  MAX_RECT_COUNT 10
#define MAX_TARGET_NUM 4
struct mvRect
{
//	cv::Rect outRect;
	TRK_RECT_INFO		outRect;
	float color[3];
	int camIdx;
	float x_angle()const {return camIdx*SDI_WIDTH+outRect.targetRect.x;};
	float y_angle()const{return outRect.targetRect.y;};
};

typedef struct{
	TRK_RECT_INFO rects[MAX_RECT_COUNT];
}RECT_Array;
typedef struct Rect_Srcptr
{
	//void *srcptr;
	RECT_Array tempoutRect;
	bool isDetectionDone;
}*pRect_Srcptr;
class IF_MvDetect
{
public:
	virtual void	 DrawRectOnpic(unsigned char *src,int capidx,int cc)=0;
	virtual void init(int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT)=0;
	virtual void m_mvDetect(int idx,unsigned char* inframe,int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT)=0;
	virtual void SetoutRect()=0;
	virtual 	std::vector<mvRect> *Getm_WholeRect(int mainOrsub)=0;
	virtual void MsetFirst()=0;
	virtual void MdeleteZombie()=0;
	virtual void ClearAllVector(bool IsOpen)=0;
};









#endif /* IF_MVDETECT_H_ */
