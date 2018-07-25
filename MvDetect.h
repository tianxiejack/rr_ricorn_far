#ifndef MVDETECT_HPP_
#define MVDETECT_HPP_
#include"StlGlDefines.h"
#include "IF_MvDetect.h"
#include"mvdetectInterface.h"
#include<vector>
using namespace std;
//inNumber 代表创建 检测OBJ的实例个数  有效值从1到8
extern void createDetect(unsigned char inNumber,int inwidth,int inheight);
extern void exitDetect();
extern void mvDetect(unsigned char index,unsigned char* inframe,int width,int height,TRK_RECT_INFO *boundRect,bool* done);
extern void setFirst();
extern void deleteZombie();
//index 代表第几个 检测OBJ 执行，boundRect 输出 目标的矩形框参数
//extern void mvDetect(unsigned char index,unsigned char* inframe,int width,int height,vector<cv::Rect> *boundRect);



class MvDetect:public BaseMvDetect
{
public:
	MvDetect();
	~MvDetect();
	void init(int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void m_mvDetect(int idx,unsigned char* inframe,int w=MAX_SCREEN_WIDTH,int h=MAX_SCREEN_HEIGHT);
	void SetoutRect();
	 void MsetFirst(){setFirst();};
	 void MdeleteZombie(){setFirst();};
	void ClearAllVector(bool IsOpen=true){
		if(!IsOpen)
		{
		/*	for(int i=0;i<CAM_COUNT;i++)
			{
				outRect[i].clear();
			}
			m_WholeRect[0].clear();
			m_WholeRect[1].clear();*/
		}
	};
private:
	Rect_Srcptr  tempRect_Srcptr[CAM_COUNT];
	//unsigned char* grayFrame[CAM_COUNT];
	//std::vector<mvRect> outRect[CAM_COUNT];
//	std::vector<mvRect> m_WholeRect[2];
};
#endif
