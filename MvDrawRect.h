#ifndef MV_DRAW_RECT_H
#define MV_DRAW_RECT_H
#include<vector>
#include<opencv2/opencv.hpp>
#include"IF_MotionDetectorROI.h"
using namespace cv;
#include"cv_version.h"
#define FIRST_SECOND_TARGET  2
#define CC 3
enum{
	BIG,
	SMALL
};
enum{
	START,
	END
};
class MotionDetectorROI:public IF_MotionDetectorROI
{
public :
	MotionDetectorROI();
	void ChooseNext(int targetidx){chooseNext[targetidx]=true;};
	void ChoosePre(int targetidx){choosePre[targetidx]=true;};
	void SetRange(float startAngle){
			range[START]=startAngle;
			range[END]=startAngle+90.0;
			if(range[END]>360.0)
				range[END]-=360.0;
		};
	void resetChooseN(int targetidx){chooseNext[targetidx]=false;};
	void resetChooseP(int targetidx){choosePre[targetidx]=false;};
	bool IsChooseN(int targetidx){return chooseNext[targetidx];};
	bool IsChooseP(int targetidx){return choosePre[targetidx];};

	void drawRect(int targetidx,unsigned char *src,int capidx);
	void put2RankVector(int targetidx,std::vector<cv::Rect> *p_singleRect,int camidx);
	void RankVectorClear(int targetidx){RankRect[targetidx].clear();};
	Rect2f *Rank(int targetidx,int bigOrsmall);
	Rect2f *GetlastRect(int targetidx){return &lastRect[targetidx];};
	void SetlastRect(int targetidx,Rect2f *prect){lastRect[targetidx]=*prect;};
	float GetRange(int startOrend){return range[startOrend];};
	Mat *GetM4(int targetidx){return &m4[targetidx];};
	Mat *GetM6(int targetidx){return &m6[targetidx];};
	void SetM4data(int targetidx,unsigned char *src){m4[targetidx].data=src;};
	void SetM6data(int targetidx,unsigned char *src){m6[targetidx].data=src;};
	void SetRectColor(int targetidx,double r,double g,double b)
	{
		RectColor[targetidx][0]=r;
		RectColor[targetidx][1]=g;
		RectColor[targetidx][2]=b;
	}
	void SettempSrc4(unsigned char *src);
	void SettempSrc6(unsigned char *src);
	void SetRoiSrc(int targetidx,int startx,int starty,int w,int h,int nowCamidx);
	unsigned char * GetpRoiSrc(int targetidx){return RoiSrc[targetidx];};
private:
	double RectColor[FIRST_SECOND_TARGET][CC];
	float range[2]; //0～右～45～前～135～左～225～后～315～右～360
	Mat m4[FIRST_SECOND_TARGET];
	Mat m6[FIRST_SECOND_TARGET];
	bool chooseNext[FIRST_SECOND_TARGET];
	bool choosePre[FIRST_SECOND_TARGET];
	Rect2f lastRect[FIRST_SECOND_TARGET];
	std::vector<Rect2f> RankRect[FIRST_SECOND_TARGET];
	unsigned char *tempSrc4;
	unsigned char *tempSrc6;
	unsigned char *RoiSrc[FIRST_SECOND_TARGET];
};


#endif
