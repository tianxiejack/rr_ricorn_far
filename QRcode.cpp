#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "QRcode.h"

#define  CorrectWidth    500
#define  CorrectHeight   500
#define  MaxDistance     40
#define  MAXTHEATE       3
#define  MINTHEATE       0.5
#define  EXPENDBOUND     20 

using namespace zbar;     //添加zbar名称空间

/*QR decode enter*/
bool isOrderCode(IplImage* src, char* code)
{
	bool status = false;

	if(src==NULL || code==NULL )
	{
		return status;
	}
//	printf("func: %s\n",__FUNCTION__);
	CQRcode detect;
	return detect.DetectProcess(src,code);
}

CQRcode::CQRcode(void):m_threshold_low(0)
{
	method = "Y800";
}

CQRcode::~CQRcode(void)
{
}

bool CQRcode::DetectProcess(IplImage * src,char* code)
{
	Mat srcImage = cvarrToMat(src,true);

	cvtColor(srcImage,grayImage,CV_RGB2GRAY);                                       
	persImage = grayImage.clone();                     /*copy for gray iamge*/

	Mat gaussImage = Mat::zeros(grayImage.size(),grayImage.type());
	Mat threshImage = Mat::zeros(grayImage.size(),grayImage.type());

	GaussianBlur(grayImage,gaussImage,Size(3,3),0);       
	bool ret = false;
	m_threshold_low = 0;
	do{
		ret = QRcodeZbarDetect(gaussImage, threshImage, code, m_threshold_low);
		if(!ret){
			Mat inDetecImg = Mat::zeros(srcImage.size(),srcImage.type());
			if(!QRcodeProcess(threshImage,inDetecImg))
				continue;
			ret = QRcodeZbarDetect(inDetecImg,code);
		}
	}while( !ret && (m_threshold_low +=20) < 255);
	
	return ret;
}

bool CQRcode::QRcodeZbarDetect(const Mat &gaussImage, Mat &threshImage, char * code, int thresh_low)
{

	threshold(gaussImage,threshImage,thresh_low,255,CV_THRESH_OTSU/*CV_THRESH_BINARY*/);  
	return QRcodeZbarDetect(threshImage,code);
	
}
/*scan and decode QRcode*/
bool CQRcode::QRcodeZbarDetect(Mat inImage, char * code)
{
	string str;
	int Width,Height;	
	ImageScanner scanner;	
	unsigned char* raw;
	unsigned long length;

	string method = "Y800";
	int scanStatus = false;

	Width = inImage.cols;
	Height = inImage.rows;
	length = Width*Height;
	raw = (uchar *)inImage.data;

	scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);        
	Image imageZbar(Width,Height,method,raw,length);           
	scanStatus = scanner.scan(imageZbar);                                         /*1 success*/            
	Image::SymbolIterator symbol = imageZbar.symbol_begin();  

	if(imageZbar.symbol_begin() == imageZbar.symbol_end())  
	{  
		scanStatus = false; 
	}  
	for(;symbol != imageZbar.symbol_end();++symbol)    
	{
		str = symbol->get_data();
		strcpy(code,str.data());                      /*return decode*/
		code[str.length()] = '\0';
		scanStatus = true;
	}
	imageZbar.set_data(NULL,0); 

	return scanStatus;
}

void CQRcode::DrawLines(Mat linesDraw, vector<Vec2f>strlines, Point point1, Point point2)
{
	int i;
	float rho, theta;	
	double a, b, x0, y0;
	
	for(i = 0;i < strlines.size();i++)
	{
		rho = strlines[i][0];
		theta = strlines[i][1];
		a = cos(theta);
		b = sin(theta);
		x0 = a*rho;                            /*xcos()*/
		y0 = b*rho;                            /*ycos()*/
		point1.x = cvRound(x0+1000*(-b));
		point1.y = cvRound(y0+1000*a);
		point2.x = cvRound(x0-1000*(-b));
		point2.y = cvRound(y0-1000*a);
		line(linesDraw,point1,point2,Scalar(255),1,CV_AA);    
	}
}

void CQRcode::PerspectTransform(Mat linesDraw, Point2f *ordPoint, Point2f *corrPoint, Mat &UndistImg)
{
	int i;
	Mat H;
	vector<Point2f>P3,P4;

	for(i = 0;i < 4;i++)       
	{
		P3.push_back(ordPoint[i]);
		P4.push_back(corrPoint[i]);
	}
	H = findHomography(P3,P4,RANSAC);
	if(H.cols==3 && H.rows==3)
	{
		warpPerspective(grayImage,persImage,H,persImage.size(),CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,0,Scalar(255));
		UndistImg = persImage.clone();
	}
}

void CQRcode::SortPoints(Point2f *origPoints,Point2f *sortPoints)
{
	int i,j;
	Point2f tep;
	Point2f temp[4], copyPoint[4];

	for(i = 0;i < 4;i++){
	    copyPoint[i] = origPoints[i];                 
	}

	temp[0] = copyPoint[0];

	for(i= 0;i < 4; i++){                                   /*sort from min to max in height*/
		for(j = i+1;j < 4;j++){
			if(copyPoint[i].y > copyPoint[j].y)
			{
				tep = copyPoint[i];
				copyPoint[i] = copyPoint[j];
				copyPoint[j] = tep ;
			}
		}
	}

	for(i = 0; i < 4; i = i+2){                         	
		if(copyPoint[i].x > copyPoint[i+1].x){
			tep = copyPoint[i];
			copyPoint[i] = copyPoint[i+1];
			copyPoint[i+1] = tep;
		}
	}

	for(i = 0;i < 4;i++){
		sortPoints[i] = copyPoint[i];
	}
}

/**correct points in 4 corners*/
void CQRcode::CorrectCornerPoints(Point2f *srcPoint,Point2f *orderPoint,Point2f *dstPoint)
{
	int x,y;
	Point2f center;
	int temWidth1, temheight1;                               /*topleft is 0，bottom is 2	*/
	int temWidth2, temheight2;
	int corrWidth, corrHeight;
	
	SortPoints(srcPoint,orderPoint);                             
	
	x = (orderPoint[0].x+orderPoint[1].x+orderPoint[2].x+orderPoint[3].x)/4;
	y = (orderPoint[0].y+orderPoint[1].y+orderPoint[2].y+orderPoint[3].y)/4;
	center = Point2f(x,y);

	/*set correctPoints height and width*/
	temWidth1 = orderPoint[1].x - orderPoint[0].x;
	temWidth2 = orderPoint[3].x - orderPoint[2].x;
	temheight1 = orderPoint[2].y - orderPoint[0].y;
	temheight2 = orderPoint[3].y - orderPoint[1].y;

	corrWidth = (MIN(temWidth1,temWidth2))/2;//(MAX(temWidth1,temWidth2))*2/3;
	corrHeight = (MIN(temWidth1,temWidth2))/2;//(MAX(temWidth1,temWidth2))*2/3;

	/*correctPoints */
	dstPoint[0] = Point2f(center.x - corrWidth,center.y - corrHeight);
	dstPoint[1] = Point2f(center.x + corrWidth,center.y - corrHeight);
	dstPoint[2] = Point2f(center.x - corrWidth,center.y + corrHeight);
	dstPoint[3] = Point2f(center.x + corrWidth,center.y + corrHeight);
}

void CQRcode::MergeLines(vector<Vec2f>line,vector<Vec2f>&outline)
{
	int i, j;
	float detaDist, detaTheta;
	Vec2f aveVec, oneVec,temVec;
	Vec2f first_line, next_line;

	float maxDistance = MaxDistance;
	float maxTheate = MAXTHEATE;
	float minTheate = MINTHEATE;
	bool status = false;              
	int count = 0;

	aveVec.val[0] = 0;
    aveVec.val[1] = 0;
	oneVec.val[0] = line[0].val[0];
	oneVec.val[1] = line[0].val[1];

	for(i = 0;i < line.size();i++){
		temVec.val[0] = line[i].val[0];
		temVec.val[1] = line[i].val[1];
		outline.push_back(temVec);
	}
	while((outline.size()> 4) && count++<20)
	{
		for(i=0; i< outline.size(); i++) 
		{
			for(j = i+1;j < outline.size(); j++)
			{
				first_line = outline[i];
				next_line = outline[j];
				detaDist = abs(abs(first_line.val[0]) - abs(next_line.val[0]));
				detaTheta = abs(first_line.val[1] - next_line.val[1]);

			if((detaTheta < minTheate || detaTheta > maxTheate) && detaDist < maxDistance )
			{
				if(detaTheta > maxTheate && detaDist < maxDistance){                     
					aveVec.val[0] = (abs(first_line.val[0]) + abs(next_line.val[0]))/2;
					aveVec.val[1] = 0;
				}else{
					aveVec.val[0] = (/*abs*/(first_line.val[0]) + /*abs*/(next_line.val[0]))/2;         /*average rho,average theata*/
					aveVec.val[1] = (first_line.val[1] + next_line.val[1])/2;
				}
				next_line.val[0] = aveVec.val[0];
				next_line.val[1] = aveVec.val[1];
				outline.erase(outline.begin()+ i);                                                     /*delete current element,save new result*/
				outline.insert(outline.begin()+ i,next_line);
				outline.erase(outline.begin()+ j);						
			}			
		}
		if(outline.size() <= 4)
			break;
		}
	}
}

/**QRcode image preprocess*/
bool CQRcode::QRcodeProcess(Mat thresImage_, Mat &detecImage)
{
	int i;	
	Mat  ROImage;

	vector<Point2f>corners;
	Point postion1, postion2;
	vector<Vec2f>lines, outlines;
	Point2f oriPoint[4], ordPoint[4], corrPoint[4];	 

	Mat cannyImage = Mat::zeros(thresImage_.rows,thresImage_.cols,thresImage_.type());
	Mat morphEdge = Mat::zeros(thresImage_.rows,thresImage_.cols,thresImage_.type());
	Mat erodeImage = Mat::zeros(thresImage_.rows,thresImage_.cols,thresImage_.type());
	Mat corrImage = Mat::zeros(thresImage_.rows,thresImage_.cols,thresImage_.type());	

	getMorphologyEdge_(thresImage_,erodeImage,morphEdge);
	Canny(erodeImage,cannyImage,0,255,3); 
	GetROIRectImage(morphEdge,cannyImage,ROImage);     

	/*detect ROI straight lines*/
	HoughLines(ROImage,lines,2,CV_PI/180*2,90);      	

	Mat DrawLine = Mat::zeros(ROImage.size(),CV_8UC1);
	Mat DrawLine2 = Mat::zeros(ROImage.size(),CV_8UC1);
	DrawLines(DrawLine,lines,postion1,postion2);

	/*if ROI straight lines is more than 4，merge lines*/
	if(lines.size() > 4)
	{                     
		MergeLines(lines,outlines);
		if(outlines.size() != 4)
			return false;

		DrawLines(DrawLine2,outlines,postion1,postion2);
	}else
	{
		DrawLines(DrawLine2,lines,postion1,postion2);
	}

	/*detect cross points*/
	goodFeaturesToTrack(DrawLine2,corners,4,0.1,10,Mat());  

	for(i=0; i<corners.size(); i++)
	{          
		oriPoint[i] = corners[i];
	}

	/*undistort 4 corner points */
	CorrectCornerPoints(oriPoint,ordPoint,corrPoint);           
	PerspectTransform(DrawLine2,ordPoint,corrPoint,corrImage);	 

	GaussianBlur(corrImage,corrImage,Size(3,3),0);	
	Mat threshImage = Mat::zeros(persImage.size(),CV_8UC1);   
	threshold(corrImage,threshImage,m_threshold_low,255,CV_THRESH_OTSU/*CV_THRESH_BINARY*/);        

	detecImage = threshImage.clone();
	return true;
}

void CQRcode::GetROIRectImage(Mat EdgeImage, Mat CannyEdge, Mat &ROimage)
{
	int i;
	double area;
	double maxArea = 0;
	vector<vector<Point> > contours;
	vector<Point> maxContours;
	Mat RectImage,ContourImage;
	Rect roi;
	int tl_x, tl_y;
	int roiWidth, roiHeight;
	int center_x, center_y;
	int srcWidth, srcHeight;
	int dstTl_x, dstTl_y;

	EdgeImage.copyTo(ContourImage);                       /*detect iamge edge */
	findContours(ContourImage,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

	/*find the longest contour*/
	for(i = 0;i < contours.size();i++)
	{
		area = contourArea(contours[i]);
		if(area > maxArea)
		{
			maxArea = area;
			maxContours = contours[i];	
		}
	}
	/*turn contour to rect*/
	Rect maxRect = boundingRect(maxContours);
	persImage.copyTo(RectImage);
	rectangle(persImage,maxRect,Scalar(255));

    /*expand rect*/
	tl_x = (maxRect.x-EXPENDBOUND < 0) ? (maxRect.x) : (maxRect.x-EXPENDBOUND);
	tl_y = (maxRect.y-EXPENDBOUND < 0) ? (maxRect.y) : (maxRect.y-EXPENDBOUND);

	roiWidth = maxRect.width+ EXPENDBOUND*2;
	roiHeight = maxRect.height + EXPENDBOUND*2;

	if((tl_x + roiWidth) > persImage.cols/*WIDTH*/)
		roiWidth = maxRect.width;

	if((tl_y + roiHeight) > persImage.rows /*HEIGHT*/)
		roiHeight = maxRect.height;

	/*put ROimg into the middle of zeros image*/
	Rect srcRect(tl_x,tl_y,roiWidth,roiHeight);               
	Mat ROimg(persImage,srcRect);            
	Mat Image = Mat::zeros(persImage.rows,persImage.cols,tempImage.type());                
	center_x = persImage.cols/2;
	center_y = persImage.rows/2;
	dstTl_x = center_x - roiWidth/2;
	dstTl_y = center_y - roiHeight/2;

	Mat dstImage = Image(Rect(dstTl_x,dstTl_y,roiWidth,roiHeight));
	CannyEdge(srcRect).convertTo(dstImage,dstImage.type(),1,0);
	ROimage = Image.clone();
}

void CQRcode::getMorphologyEdge_(Mat inThreshImg_, Mat &erodeImage_, Mat &morphEdge_)
{
	int i;
	Mat kernel;	 
	Mat dilateImage,m_erodeImage;
	Mat erodeImage = inThreshImg_.clone();

	kernel = getStructuringElement(MORPH_ELLIPSE,Size(7,7));    
	/*erode*/
	for(i = 0; i < 6; i++)
	{                                       
		erode(erodeImage,erodeImage,kernel);   
	}

	erode(erodeImage,m_erodeImage,kernel);
	morphEdge_ = erodeImage - m_erodeImage;          /*erode edge*/
	erodeImage_ = erodeImage.clone();             
}
