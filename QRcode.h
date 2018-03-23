#ifndef QRCODEDETECTE_H
#define QRCODEDETECTE_H

#include <zbar.h>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/core/mat.hpp>

using namespace std;            
using namespace cv; 

class CQRcode
{
public:
	CQRcode();
	~CQRcode();
	bool DetectProcess(IplImage * src,char* code);

private:
	void CorrectCornerPoints(Point2f *srcPoint, Point2f*orderPoint, Point2f *dstPoint);                           
	bool QRcodeZbarDetect(Mat inImage, char * code);       
	void DrawLines(Mat linesDraw, vector<Vec2f>line, Point point1, Point point2);
	void SortPoints(Point2f *origPoints,Point2f *sortPoints);
	void PerspectTransform(Mat linesDraw, Point2f  *ordPoint, Point2f *corrPoint, Mat &UndistImage);
	void MergeLines(vector<Vec2f>line,vector<Vec2f>&outline);
	bool QRcodeProcess(Mat Image, Mat &detecImg);
	void GetROIRectImage(Mat DilateEdge, Mat CannyEdge, Mat &ROi);
	void Undistort(Mat SrcImg, Mat &DstImg);
	void getMorphologyEdge_(Mat GrayImage, Mat &erodeImage, Mat &MorphEdge);
	bool QRcodeZbarDetect(const Mat &gaussImage, Mat &threshImage, char * code, int threshold);   

	Mat grayImage;
	Mat copyImage;            
	Mat tempImage, persImage;
	Mat threshImage;

	string method;
	int m_threshold_low;
};

bool isOrderCode(IplImage* src, char* code);

#endif

