#ifndef MV_DRAW_RECT_H
#define MV_DRAW_RECT_H
#include<vector>
#include<opencv2/opencv.hpp>
#include"IF_MotionDetectorROI.h"
using namespace cv;
#include"StlGlDefines.h"
#include"cv_version.h"
#include"MvDetect.h"
#include"MvDrawRect.h"

#define MAX_TARGET_NUM 4
#define CC 3
#define MAX_X_POS (1920*9)
#if 1
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
	MotionDetectorROI(int sumTarget,MvDetect *pmv);

	void ChooseNext(int targetidx){chooseNext[targetidx]=true;};
	void ChoosePre(int targetidx){choosePre[targetidx]=true;};
	void ChooseUp(int targetidx){chooseUp[targetidx]=true;};
	void ChooseDown(int targetidx){chooseDown[targetidx]=true;};
	void MoveN(){ moveN=true;};
	void MoveP(){ moveP=true;};

	void resetChooseN(int targetidx){chooseNext[targetidx]=false;};
	void resetChooseP(int targetidx){choosePre[targetidx]=false;};
	void resetChooseUp(int targetidx){chooseUp[targetidx]=false;};
	void resetChooseDown(int targetidx){chooseDown[targetidx]=false;};
	void resetMoveN(){ moveN=false;};
	void resetMoveP(){ moveP=false;};

	bool IsChooseN(int targetidx){return chooseNext[targetidx];};
	bool IsChooseP(int targetidx){return choosePre[targetidx];};
	bool IsChooseUp(int targetidx){return chooseUp[targetidx];};
	bool IsChooseDown(int targetidx){return chooseDown[targetidx];};
	bool IsMoveN(){return moveN;};
	bool IsMoveP(){return moveP;};


	void SetRange(float startAngle,float endAngle){
			range[START]=startAngle;
			range[END]=endAngle;
			if(range[END]>360.0)
				range[END]-=360.0;
		};
//	float GetRange(int startOrend){return range[startOrend];};

	void DrawAllRectOri(int fourOrsix);
	void MRectangle(int fourOrsix,mvRect *p);
	bool ISanySingleRect(){
		for(int i=0;i<m_sumTarget;i++)
		{
			if(ISselectSingleRect(i))
				return true;
		}
		return false;};
	bool ISselectSingleRect(int targetidx){return selectSingleRect[targetidx];};
	void  ResetselectSingleRect(int targetidx){ selectSingleRect[targetidx]=false;};


	//Mat *GetM4(int targetidx){return &m4[targetidx];};
	//Mat *GetM6(int targetidx){return &m6[targetidx];};
	void SetM4data(unsigned char *src){m4.data=src;};
	void SetM6data(unsigned char *src){m6.data=src;};
/*	void SetRectColor(int targetidx,double r,double g,double b)
	{
		RectColor[targetidx][0]=r;
		RectColor[targetidx][1]=g;
		RectColor[targetidx][2]=b;
	}*/

	void SetlastRect(int targetidx,mvRect *prect){lastRect[targetidx]=*prect;};
	void SettargetRect(int targetidx,mvRect *prect){targetRect[targetidx]=*prect;};

	mvRect *GetlastRect(int targetidx){return &lastRect[targetidx];};
	mvRect *GettargetRect(int targetidx){return &targetRect[targetidx];};

	void drawRect(int targetidx,unsigned char *src,int capidx);
	//void put2RankVector(int targetidx,std::vector<cv::Rect> *p_singleRect,int camidx);
//	void RankVectorClear(int targetidx){RankRect[targetidx].clear();};
	mvRect *Rank(int targetidx,int bigOrsmall);

	void RectfromSrc(int fourOrsix,int targetidx,int camIdx,int x,int y,int w,int h);
	unsigned char * GetRoiSrc(int targetidx);
	void SettempSrc4(unsigned char *src);
	void SettempSrc6(unsigned char *src);


	unsigned char * GetpRoiSrc(int targetidx){return RoiSrc[targetidx];};
private:
	double RectColor[MAX_TARGET_NUM][CC];
	float range[2]; //0～右～45～前～135～左～225～后～315～右～360
	bool moveN;
	bool moveP;
	bool chooseNext[MAX_TARGET_NUM];
	bool choosePre[MAX_TARGET_NUM];
	bool chooseUp[MAX_TARGET_NUM];
	bool chooseDown[MAX_TARGET_NUM];
	mvRect lastRect[MAX_TARGET_NUM];//yellow
	mvRect targetRect[MAX_TARGET_NUM];//green
//	std::vector<mvRect> RankRect[MAX_TARGET_NUM];
	unsigned char *tempSrc4;
	unsigned char *tempSrc6;
	Mat m4;//[MAX_TARGET_NUM];
	Mat m6;//[MAX_TARGET_NUM];
	unsigned char *RoiSrc[MAX_TARGET_NUM];
	int m_sumTarget;
	bool selectSingleRect[MAX_TARGET_NUM];
	MvDetect *m_pmv;
};

#endif
#endif
