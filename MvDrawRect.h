#ifndef MV_DRAW_RECT_H
#define MV_DRAW_RECT_H
#include<vector>
#include<opencv2/opencv.hpp>
using namespace cv;
#include"StlGlDefines.h"
#include"cv_version.h"
#include"MvDetect.h"
#include"mvdetectInterface.h"
#define MAX_TARGET_NUM 4
#define CC 3

enum{
	BIG,
	SMALL
};
enum{
	START,
	END
};
class MotionDetectorROI//:public IF_MotionDetectorROI
{
public :
	MotionDetectorROI(int sumTarget,IF_MvDetect *pmv,int mainOrsub);
	void DrawAllRectOri(int mainOrsub,int fourOrsix=0);
	void RectfromSrc(int fourOrsix,int targetidx,int camIdx,int x,int y,int w,int h);
	unsigned char * GetRoiSrc(int targetidx);
private:
	mvRect targetRect[MAX_TARGET_NUM];//green
	unsigned char *RoiSrc[MAX_TARGET_NUM];
	int m_sumTarget;
	IF_MvDetect *m_pmv;
	int m_MAIN_SUB;
};

#endif
