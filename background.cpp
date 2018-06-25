#include "background.hpp"


char ctrl = NULL;





void CBgMvDetect::init(int inwidth,int inheight)
{
	pFrame = NULL;
	pFroundImg = NULL;
	pBackgroundImg = NULL;

	SrcImg_gray 	= 	NULL;//源图像的灰度图像
	SrcImg_grayf 	=	NULL;  //单通道浮点图像用于背景建模
	FroundImgf 		= 	NULL;
	BackgroundImgf 	= 	NULL;
	FroundImg_temp 	= 	NULL;

	return ;
}

void CBgMvDetect::destroy()
{
	cvReleaseImage(&pFroundImg);
	cvReleaseImage(&pBackgroundImg);
	return ;
}


CBgMvDetect::CBgMvDetect()
{

}

CBgMvDetect::~CBgMvDetect()
{

}


void CBgMvDetect::gbMvDetect(uchar* src,uchar *dst,int width,int height,Rect* boundRect,uchar frameindex)
{

	pFrame = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	pFrame->imageData=(char*)src;

	if(frameindex == 0)
	{
		pBackgroundImg 	= cvCreateImage(cvGetSize(pFrame),  8,1);
		pFroundImg 		= cvCreateImage(cvGetSize(pFrame),  8,1);
	}

	BackgroundDiff(pFrame,pFroundImg,pBackgroundImg, frameindex, CV_THRESH_OTSU);  //普通OTSU
	//BackgroundDiff(pFrame,pFroundImg_c,pBackgroundImg_c, nFrmNum, CV_THRESH_BINARY); //阈值筛选后的OTSU

	//打印视频信息,画面控制
	//PrintVedioInfo(pCapture, pFroundImg);

	Mat getframe = Mat(pFroundImg->height,pFroundImg->width,CV_8UC1,pFroundImg->imageData);
	Mat gettmpframe = getframe.clone();

	vector< vector<Point> > contours;
	findContours(gettmpframe, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//drawContours(result, contours, -1, Scalar(0, 0, 255), 2);//在result上绘制轮廓
	//7.查找正外接矩形
	//vector<Rect> boundRect(contours.size());
	int tmpnum = 6;
	if(tmpnum > contours.size())
		tmpnum = contours.size();

	Mat ppptmp = Mat(pFrame->height,pFrame->width,CV_8UC3,pFrame->imageData);
	for (int i = 0; i < tmpnum; i++)
	{
		boundRect[i] = boundingRect(contours[i]);
		//printf("boundRect[%d].x = %d,boundRect[%d].y = %d \n",i,boundRect[i].x,i,boundRect[i].y);
		//printf("boundRect[%d].width = %d,boundRect[%d].height = %d \n",i,boundRect[i].width,i,boundRect[i].height);
		rectangle(ppptmp, boundRect[i], Scalar(0, 0, 255), 2);//在result上绘制正外接矩形
	}

	memcpy(dst,pFrame->imageData,pFrame->height*pFrame->width*3);
	//imshow("aaaaaaaaaaaaaaaa",ppptmp);
	//cvShowImage("video", pFrame);
	//cvShowImage("background", pBackgroundImg);
	//cvShowImage("OTSU foreground", pFroundImg);

	return ;
}

/*
 *输出文字到图像
 */
void PrintVedioInfo(CvCapture* pCapture, IplImage* img)
{
	assert( pCapture != NULL);
	double frames = cvGetCaptureProperty(pCapture, CV_CAP_PROP_POS_FRAMES);  //视频当前帧数
	double fps = cvGetCaptureProperty(pCapture,CV_CAP_PROP_FPS); //获得视频每秒帧数
	char str[255];
	sprintf(str,"%4.2f FPS %4.2f frames",fps,frames);  // 将浮点数转化为字符串
	CvPoint location = cvPoint(20,20); // 建立字符串打印的位置
	CvScalar color = cvScalar(255,255,255);
	CvFont font;  //建立字体变量
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0,1.0);  //字体设置
	cvPutText(img, str, location, &font,color);  //打印文本到图像
}


/********
*背景差分函数,求前景目标
*重要: 函数退出之后,函数中的动态变量会随着栈的退出全部清空.
*要保存上次操作的结果,则在函数内声明为静态变量.或者在要调用的函数里先声明
********/
void CBgMvDetect::BackgroundDiff(IplImage* SrcImg, IplImage* FroundImg, IplImage* BackgroundImg,
		int nFrmNum, int threshold_method)
{
	threshold_method = CV_THRESH_OTSU;
	if(nFrmNum == 0)
	{
		SrcImg_gray    = cvCreateImage(cvGetSize(SrcImg),  8,1);
		FroundImg_temp = cvCreateImage(cvGetSize(SrcImg),  8,1);
		BackgroundImgf = cvCreateImage(cvGetSize(SrcImg),  32,1);  //浮点图像
		FroundImgf	   = cvCreateImage(cvGetSize(SrcImg),  32,1);
		SrcImg_grayf   = cvCreateImage(cvGetSize(SrcImg),  32,1);

		//RGB图像先转化成8位单通道图像,再转化为浮点.
		cvCvtColor(SrcImg, BackgroundImg, CV_BGR2GRAY);
		cvCvtColor(SrcImg, FroundImg, CV_BGR2GRAY);
		cvConvert(BackgroundImg,BackgroundImgf);
		cvConvert(FroundImg,FroundImgf);
	}
	else
	{
		cvCvtColor(SrcImg, SrcImg_gray, CV_BGR2GRAY);//SrcImg_gray在上次函数退出的时候被程序栈回收
		cvConvert(SrcImg_gray,SrcImg_grayf);
		cvSmooth(SrcImg_grayf, SrcImg_grayf, CV_GAUSSIAN, 3, 0, 0);
		//当前帧跟背景图相减
		cvAbsDiff(SrcImg_grayf, BackgroundImgf, FroundImgf);
		cvConvert(FroundImgf,FroundImg_temp);//浮点转化为整点

		//二值化前景图
		int threshold_otsu =0;
		cvOtsu(FroundImg_temp, &threshold_otsu);

		if(threshold_method == CV_THRESH_OTSU)
		{
			cvThreshold(FroundImg_temp, FroundImg, 0, 255.0, CV_THRESH_OTSU); //对比自适应阈值化
		}
		else
		{
			cvThreshold(FroundImg_temp, FroundImg, threshold_otsu, 255.0, CV_THRESH_BINARY);
		}

		//cvSegmentFGMask( FroundImg ); //对前景做连通域分割

		//cvShowImage("haha2", FroundImg);

		//更新背景
		cvRunningAvg(SrcImg_grayf, BackgroundImgf, 0.4, 0);  //必须是浮点图像,因为会有小数出现
		cvConvert(BackgroundImgf,BackgroundImg);
	}
}

/********
 *OTSU大津法
 * thresholdValue 为使类间方差最大的阈值
 * 当找到的阈值小于一个修正阈值,返回此修正阈值.防止没有前景物体时,将背景找出来
 ********/
void cvOtsu(IplImage *src, int *thresholdValue)
{
    int deltaT = 0; //光照调节参数
	uchar grayflag =1;
	IplImage* gray = NULL;
	if(src->nChannels != 1) //检查源图像是否为灰度图像
	{
		gray = cvCreateImage(cvGetSize(src), 8, 1);
		cvCvtColor(src, gray, CV_BGR2GRAY);
		grayflag = 0;
	}
	else
		gray = src;

	uchar* ImgData=(uchar*)(gray->imageData);
	int thresholdValue_temp = 1;
    int ihist[256];   //图像直方图,256个点

    int i, imgsize; //循环变量,图像尺寸
    int n, n1, n2;  //n 非零像素个数, n1 前景像素个数, n2 背景像素个数
    double m1, m2, sum, csum, fmax, sb;//m1前景灰度均值,m2背景灰度均值

    //对直方图置零
    memset(ihist, 0, sizeof(ihist));
    //生成直方图
    imgsize = (gray->widthStep)*(gray->height);//图像数据总数
    for (i=0; i<imgsize;i++)
    {
		ihist[((int)(*ImgData))&255]++;//灰度统计 '&255'防止指针溢出
		ImgData++;//像素遍历
    }

    // set up everything
    sum=csum=0.0;
    n=0;
    for (i=0; i<255; i++)
    {
		sum += (double)i*(double)ihist[i];  // x*f(x)质量矩
		n += ihist[i];   //f(x)质量 像素总数
    }

    deltaT = (int)(sum/imgsize); //像素平均灰度
    deltaT = deltaT>>1; //与之矫正,delatT = v*n; v=0.5

    if (!n)
    {
    	//图像全黑,输出警告
    	fprintf (stderr, "NOT NORMAL thresholdValue=160\n");
    }
    // OTSU算法
    fmax=-1.0;
    n1=0;
    for (i=0; i<255; i++)
    {
        n1+= ihist[i];
        if (n1==0) {continue;}
        n2=n-n1;
        if (n2==0) {break;}
        csum += (double)i *ihist[i];
        m1=csum/n1;
        m2=(sum-csum)/n2;
        sb=(double)n1*(double)n2*(m1-m2)*(m1-m2); //计算类间方差,  公式已简化
        if (sb>fmax)
        {
            fmax=sb;
            thresholdValue_temp=i;  //找到使类间方差最大的灰度值i
        }
    }

	if(thresholdValue_temp < 20)
		*thresholdValue = 20;  //阈值筛选
	else
		*thresholdValue = thresholdValue_temp;

	if( ctrl == 'p')  //ctrl  = cvWaitKey(100),且是全局变量
	{
	   cout << "OTSU thresholdValue = " << thresholdValue_temp<<", Returned thresholdValue = " << *thresholdValue<<'\n'<<endl;
	}
	if(!grayflag)
		cvReleaseImage(&gray);
}

/***********
*轮廓提取
************/
void Labeling(IplImage *src, IplImage *dst)
{
    CvMemStorage* storage = 0;
    storage = cvCreateMemStorage(0); //开辟默认大小的空间
    CvSeq* contour=0;
    cvCopy(src,dst,0);
    cvFindContours( dst, storage, &contour, sizeof(CvContour),
              CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE ); //外边缘
    int num=0;
    for( ;contour!=0; contour=contour->h_next)
    {
          CvRect rect;
      rect = cvBoundingRect(contour,0);//得到目标外接矩形
          num++;
        if((rect.height + rect.width) >= 16)
        cvRectangle(src,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),
                  CV_RGB(255, 255,255),1,8);//绘制目标外接矩形
// cvRectangle(dst,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),
 //                 CV_RGB(255, 255,255),1,8);//绘制目标外接矩形
    }
}
