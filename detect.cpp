#include <stdio.h>
#include "detect.hpp"
#include "osa_thr.h"



CDetect::CDetect()
{
	pThis = this;
	frameindex = 0;
}

CDetect::~CDetect()
{

}

void CDetect::init(int inwidth,int inheight)
{
	temp = Mat(inheight,inwidth,CV_8UC1);

	return ;
}


void CDetect::xtMoveDetect(Mat temp, Mat frame,Rect *boundRect, bool* done)
{
	//Mat drawtemp = frame;
    //1.将background和frame转为灰度图
    Mat gray1 = temp;
    Mat gray2 = frame;

    //2.将background和frame做差
    Mat diff;
    absdiff(gray1, gray2, diff);
    //imshow("diff", diff);
    //3.对差值图diff_thresh进行阈值化处理
    Mat diff_thresh;
    threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
    //imshow("diff_thresh", diff_thresh);
    //4.腐蚀
    Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
    erode(diff_thresh, diff_thresh, kernel_erode);
    //imshow("erode", diff_thresh);
    //5.膨胀
    dilate(diff_thresh, diff_thresh, kernel_dilate);
    //imshow("dilate", diff_thresh);

    //6.查找轮廓并绘制轮廓
    vector< vector<Point> > contours;
    findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //drawContours(result, contours, -1, Scalar(0, 0, 255), 2);//在result上绘制轮廓
    //7.查找正外接矩形
    //vector<Rect> boundRect(contours.size());
    int tmpnum = 6;
    if(tmpnum > contours.size())
    	tmpnum = contours.size();

    for (int i = 0; i < tmpnum; i++)
    {
        boundRect[i] = boundingRect(contours[i]);
        //printf("boundRect[%d].x = %d,boundRect[%d].y = %d \n",i,boundRect[i].x,i,boundRect[i].y);
        //printf("boundRect[%d].width = %d,boundRect[%d].height = %d \n",i,boundRect[i].width,i,boundRect[i].height);
        //rectangle(drawtemp, boundRect[i], Scalar(0, 0, 255), 2);//在result上绘制正外接矩形
    }

	if(tmpnum<5)
	{
		for(int i=tmpnum;i<6;i++)
		{
			boundRect[i].x=0;
			boundRect[i].y=0;
			boundRect[i].width=-1;
			boundRect[i].height=-1;
		}
	}

	*done = true;
    return ;
}




void CDetect::detect(uchar* inframe,int width,int height,Rect *boundRect,uchar frameindex, bool* done)
{
	if(height <= 1080 && width <= 1920)
	{
		Mat frame = Mat(height,width,CV_8UC1,inframe);
		if (frameindex == 0)//如果为第一帧
		{
			xtMoveDetect(frame, frame,boundRect, done);//调用MoveDetect()进行运动物体检测，返回值存入result
		}
		else//若不是第一帧（temp有值了）
		{
			xtMoveDetect(temp, frame,boundRect,done);//调用MoveDetect()进行运动物体检测，返回值存入result
		}
		memcpy(temp.data,frame.data,height*width);//temp = frame.clone();//
//if(boundRect->x>0)
{

//	printf("frameindex=%x  x=%d  y=%d  w=%d  h=%d\n",frameindex,boundRect->x,boundRect->y,boundRect->width,boundRect->height);
}
	}
	OSA_thrExit(0);
	return ;
}
