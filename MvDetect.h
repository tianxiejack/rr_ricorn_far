#ifndef MVDETECT_HPP_
#define MVDETECT_HPP_
#include<opencv2/opencv.hpp>
#include"StlGlDefines.h"
#include"IF_MotionDect.h"
#include"cv_version.h"
#include<vector>
using namespace cv;
//inNumber 代表创建 检测OBJ的实例个数  有效值从1到8
extern void createDetect(unsigned char inNumber,int inwidth,int inheight);
extern void exitDetect();
extern void mvDetect(unsigned char index,unsigned char* inframe,int width,int height,cv::Rect *boundRect);

//index 代表第几个 检测OBJ 执行，boundRect 输出 目标的矩形框参数
//extern void mvDetect(unsigned char index,unsigned char* inframe,int width,int height,vector<cv::Rect> *boundRect);


struct mvRect
{
	cv::Rect outRect;
	float color[3];
	int camIdx;
	float x_angle;
	float y_angle;
};

class MvDetect:public IF_MotionDect   //采集里面做
{
public:
	MvDetect();
	~MvDetect();

	void DrawRectOnpic(unsigned char *src,int capidx);
	void selectFrame(unsigned char *dst,unsigned char *src,int targetId,int camIdx);


	void yuyv2gray(unsigned char* src,unsigned char* dst,int width=MAX_SCREEN_WIDTH,int height=MAX_SCREEN_HEIGHT);
	void init(int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void m_mvDetect(int idx,unsigned char* inframe,int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void saveConfig();
	void ReadConfig();
	bool GetMD(int mainorsub){return  enableMD[mainorsub];};
	void SetMD(bool tof,int mainorsub){enableMD[mainorsub]=tof;};
	void OpenMD(int mainorsub){MDopen[mainorsub]=true;};
	void CloseMD(int mainorsub){MDopen[mainorsub]=false;};
	bool CanUseMD(int mainorsub);
	bool MDisStart(){
		if(enableMD[MAIN]==true &&MDopen[MAIN]==true)///*&& enableMD[SUB]==true*/)
			return true;
	else if(enableMD[MAIN]==true&&MDopen[SUB]==true)
			return true;
	else
	return false;};

	std::vector<mvRect> * GetWholeRect();//取得全图的rect
	void SetoutRect(int idx);//将检测到的每个通道里6个rect放入对应的6个容器里
private:
	bool enableMD[2];
	bool MDopen[2];
	static const int MAX_RECT_COUNT=6;
	typedef struct{cv::Rect rects[MAX_RECT_COUNT];}RECT_Array;
	RECT_Array tempoutRect[CAM_COUNT];
	std::vector<mvRect> m_outRect[CAM_COUNT];
	std::vector<mvRect> m_WholeRect;
	unsigned char* grayFrame[CAM_COUNT];
	std::vector<cv::Rect> outRect[CAM_COUNT];
};
#endif
