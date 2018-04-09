#ifndef MVDETECT_HPP_
#define MVDETECT_HPP_
#include "opencv2/core/types.hpp"
#include"StlGlDefines.h"
//inNumber 代表创建 检测OBJ的实例个数  有效值从1到8
extern void createDetect(unsigned char inNumber,int inwidth,int inheight);
extern void exitDetect();

//index 代表第几个 检测OBJ 执行，boundRect 输出 目标的矩形框参数
extern void mvDetect(unsigned char index,unsigned char* inframe,int width,int height,cv::Rect *boundRect);
class MvDetect
{
public:
	MvDetect();
	~MvDetect();
	void init(int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void m_mvDetect(int idx,unsigned char* inframe,int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
private:
	int inNum[CAM_COUNT];
	cv::Rect OutRect[CAM_COUNT];
};
#endif
