#ifndef MVDETECT_HPP_
#define MVDETECT_HPP_
#include"StlGlDefines.h"
#include<vector>
using namespace std;
//inNumber 代表创建 检测OBJ的实例个数  有效值从1到8
extern void createDetect(unsigned char inNumber,int inwidth,int inheight);
extern void exitDetect();
extern void mvDetect(unsigned char index,unsigned char* inframe,int width,int height,cv::Rect *boundRect,bool* done);
extern void setFirst();
extern void deleteZombie();
//index 代表第几个 检测OBJ 执行，boundRect 输出 目标的矩形框参数
//extern void mvDetect(unsigned char index,unsigned char* inframe,int width,int height,vector<cv::Rect> *boundRect);
#define  MAX_RECT_COUNT 6
#define MAX_TARGET_NUM 4
struct mvRect
{
	cv::Rect outRect;
	float color[3];
	int camIdx;
	float x_angle()const {return camIdx*SDI_WIDTH+outRect.x;};
	float y_angle()const{return outRect.y;};
};

typedef struct{
	cv::Rect rects[MAX_RECT_COUNT];
}RECT_Array;
typedef struct Rect_Srcptr
{
	//void *srcptr;
	RECT_Array tempoutRect;
	bool isDetectionDone;
}*pRect_Srcptr;


class MvDetect
{
public:
	MvDetect();
	~MvDetect();

	void DrawRectOnpic(unsigned char *src,int capidx,int cc);
	void uyvy2gray(unsigned char* src,unsigned char* dst,int width=MAX_SCREEN_WIDTH,int height=MAX_SCREEN_HEIGHT);
	void init(int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void m_mvDetect(int idx,unsigned char* inframe,int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);

	void SetoutRect();//将检测到的每个通道里6个rect放入对应的6个容器里

	std::vector<mvRect> *Getm_WholeRect(int mainOrsub)
	{
		m_WholeRect[mainOrsub].clear();
		for(int i=0;i<CAM_COUNT;i++)
			m_WholeRect[mainOrsub].insert(m_WholeRect[mainOrsub].end(),outRect[i].begin(),outRect[i].end());
		return &m_WholeRect[mainOrsub];
	}


private:
	Rect_Srcptr  tempRect_Srcptr[CAM_COUNT];
	unsigned char* grayFrame[CAM_COUNT];
	std::vector<mvRect> outRect[CAM_COUNT];
	std::vector<mvRect> m_WholeRect[2];



};
#endif
