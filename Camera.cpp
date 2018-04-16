/*
 * Camera.cpp
 *
 *  Created on: Dec 31, 2016
 *      Author: wang
 * Refactored on Jan 6, 2017 by Hoover
 */
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <string.h>
#include <stdbool.h>
#include "Camera.h"
#include "thread.h"
#include <unistd.h>
#include "buffer.h"
#include "overLapBuffer.h"
#include "StlGlDefines.h"
#include "FishCalib.h"
#include<sys/time.h>
#define MEMCPY memcpy


using namespace std;
using namespace cv;
unsigned char * temp_data_main[MAX_CC];
unsigned char * temp_data_sub[MAX_CC];

BaseVCap::~BaseVCap()
{
}
void BaseVCap::SavePic(const char* name)
{
	char * src = GetDefaultImg();
	Capture(src);
	cvSaveImage(name,src);
}

void SmallVCap::Capture(char* ptr,int mainORsub)
{
	get_buffer((unsigned char*)ptr, m_chId);
//	printf("capture:%d\n",m_chId);
};

void SmallVCap::CaptureFish(char* ptr)
{
//	printf("captureFish:pp\n");
	Mat outIMG = Mat(IMAGEHEIGHT, IMAGEWIDTH, CV_8UC4, ptr);//imread("./in.bmp");
	Mat inIMG= outIMG.clone();
	get_buffer((unsigned char*)inIMG.data, m_chId);
	//workaround bug#00038 green corner flashes from time to time,
	//by always setting first line to black.
	if((inIMG.data[0] != 192 && inIMG.data[1]!= 64)||
			(inIMG.data[2] != 112 && inIMG.data[3]!=64)){
		for(int i = 0; i < IMAGEWIDTH;  ){
			inIMG.data[i++] = 127;
			inIMG.data[i++] = 15;
			inIMG.data[i++] = 127;
			inIMG.data[i++] = 18;

			inIMG.data[i++] = 128;
			inIMG.data[i++] = 16;
			inIMG.data[i++] = 127;
			inIMG.data[i++] = 19;
		}
	}
	get_img(inIMG, outIMG);
//	printf("captureFish:%d\n",m_chId);
}

#define CLAP(min, max , val)	(val<min)?(min):((val>max)?(max):(val))



void  expandYuyvSDI(unsigned char* lpYUVFrame, unsigned char* pRGB, int width, int height)
{
#if	!NO_ARM_NEON
     int stride8x8 = width*2/8;
	static const int RGBADepth = 4;
    #pragma omp parallel for
	for(int y =0 ; y < height; y++){
		uint8x8_t * __restrict__ pSrc8x8_t,  * __restrict__ pDst08x8_t, * __restrict__ pDst18x8_t;
		int i, ii=0;
		pSrc8x8_t = (uint8x8_t *)(lpYUVFrame + y*width*2);
		pDst08x8_t = (uint8x8_t *)(pRGB + y*width*RGBADepth);
		pDst18x8_t = pDst08x8_t+1;



		for(i = 0; i < stride8x8; i ++, ii+=2){
				uint8x8_t src1, src2,tmp;
				tmp = src1 = src2 = pSrc8x8_t[i];
				tmp = vcopy_lane_u8(tmp,0,src1,2 );
				tmp = vcopy_lane_u8(tmp,4,src1,6 );
				src1 = vreinterpret_u8_u32(vcopy_lane_u32( vreinterpret_u32_u8(src1), 1, vreinterpret_u32_u8(tmp),0));

					pDst08x8_t[ii] = src1;
					src2 = vreinterpret_u8_u32(vcopy_lane_u32( vreinterpret_u32_u8(src2), 0, vreinterpret_u32_u8(src2),1));
					src2 = vreinterpret_u8_u32(vcopy_lane_u32( vreinterpret_u32_u8(src2), 1, vreinterpret_u32_u8(tmp),1));
					pDst18x8_t[ii] = src2;
		}
	}
#endif
}

void yuyv2YUxV(unsigned char *pYuvBuf, unsigned char* ptr, int width,int height)
{
	expandYuyvSDI( pYuvBuf,  ptr, width, height);
}


static void UYVx2RGB(unsigned char *lpYUV, int width, int height, Mat* RGBData)
{
	int i, j;
	unsigned char *lpSrc, *lpDst;
	int Y, U, V, R, G, B;

	for(j=0; j<height; j++)
	{
		for(i=0; i<width; i++)
		{
			lpSrc = lpYUV + j*width*4 + 4*i;
			lpDst = RGBData->data + j*width*3 + 3*i;
			U = *lpSrc;Y = *(lpSrc+1);V = *(lpSrc+2);
		   B = (Y + 1.402 * (U - 128.0));
		   G = (Y - 0.344 * (V - 128.0) - 0.714 * (U - 128.0));
		   R = (Y + 1.772 * (V - 128.0));

			*lpDst = CLAP(0,255,B); *(lpDst+1) = CLAP(0,255,G); *(lpDst+2) = CLAP(0,255,R);
		}
	}
}





void SmallVCap::SavePic(const char* name)
{
	get_buffer(m_buffer, m_chId);
	Mat img(576,720,CV_8UC3);
	UYVx2RGB(m_buffer, 720, 576, &img);
	try{
		imwrite(name,img);
//		waitKey(0);
	}catch(cv::Exception& os)
	{

	}
}
void SmallVCap::saveOverLap()
{
	get_bufferyuv(m_buffer, m_chId);
	UYVx2RGB(m_buffer, 720, 576, &yuvImg);
//	overLap.updateGap(m_chId,yuvImg);
}




void HDVCap::SavePic(const char* name)
{
	static unsigned char buffer[SDI_WIDTH*SDI_HEIGHT*2];
	static unsigned char ptr[SDI_WIDTH*SDI_HEIGHT*4];
	Mat img(SDI_HEIGHT,SDI_WIDTH,CV_8UC3);
//	OSA_semWait(&sem[m_chId],OSA_TIMEOUT_FOREVER);
//	memcpy(buffer,&sdi_data[m_chId],sizeof(buffer));
//	YUYV2UYVx(ptr,buffer,SDI_WIDTH,SDI_HEIGHT);
//	UYVx2RGB(ptr, SDI_WIDTH, SDI_HEIGHT, &img);
//	OSA_semSignal(&sem[m_chId]);
//	try{
//		imwrite(name,img);
//		waitKey(0);
//	}catch(cv::Exception& os)
}

void save_yuyv_pic2(void *pic,int idx)
{
	FILE * fp;
	char buf[30];
	sprintf(buf,"./save_transYUV_%d.yuv",idx);
	fp=fopen(buf,"w");
	char data[20];
	fwrite(pic,1920*1080*2,1,fp);
	fclose(fp);
}


void HDVCap::Capture(char* ptr,int mainORsub){
	static bool once=true;
	int chid[2]={-1,-1};
	int nowpicW=SDI_WIDTH,nowpicH=SDI_HEIGHT;
	if(once)
	{
		for(int i=0;i<MAX_CC;i++)
		{
			if(i==MVDECT_CN)
			{
			//	printf{"检测不需要创建\n"};
			}
			else if(i==FPGA_FOUR_CN)//采集的时候大，但是已经经过了转换
			{
				temp_data_main[i]=(unsigned char * )malloc(FPGA_SCREEN_WIDTH*FPGA_SCREEN_HEIGHT*4);
				temp_data_sub[i]=(unsigned char * )malloc(FPGA_SCREEN_WIDTH*FPGA_SCREEN_HEIGHT*4);
			}
			else//普通1920*1080数据
			{
				temp_data_main[i]=(unsigned char * )malloc(SDI_WIDTH*SDI_HEIGHT*4);
				temp_data_sub[i]=(unsigned char * )malloc(SDI_WIDTH*SDI_HEIGHT*4);
			}
		}
		once=false;
	}

					switch(m_chId)
					{
					case FPGA_FOUR_CN:
						chid[MAIN]=MAIN_FPGA_FOUR;
						chid[SUB]=SUB_FPGA_FOUR;
						nowpicW=FPGA_SCREEN_WIDTH;
						nowpicH=FPGA_SCREEN_HEIGHT;
						break;
					case SUB_CN:
						chid[SUB]=SUB_ONE_OF_TEN;
						break;
					case MAIN_CN:
						chid[MAIN]=MAIN_ONE_OF_TEN;
						break;
			    case MVDECT_CN:
			    	printf("there's no right use Mvdect  gray pic !!\n");
			    	assert(false);
					break;
					case FPGA_SIX_CN:
						chid[MAIN]=MAIN_FPGA_SIX;
						chid[SUB]=SUB_FPGA_SIX;
						break;
					default:
						break;
					}
					if(mainORsub==MAIN) //车长
					{
						get_buffer((unsigned char *)temp_data_main[m_chId],chid[MAIN]);
						memcpy(ptr,temp_data_main[m_chId],nowpicW*nowpicH*4);
					}
					else if(mainORsub==SUB)//载员
					{
						get_buffer((unsigned char *)temp_data_sub[m_chId],chid[SUB]);
						memcpy(ptr,temp_data_sub[m_chId],nowpicW*nowpicH*4);
					}
					else
					{
						printf("input main or sub is out of limit!\n");
						assert(false);
					}
}



//--------------the decorator cap class------------
AsyncVCap::AsyncVCap(auto_ptr<BaseVCap> coreCap):
	m_core(coreCap),thread_state(THREAD_READY),sleepMs(DEFAULT_THREAD_SLEEP_MS)
{
	initLock();
	start_thread(capThread,this);
	cout<<" start a capture thread"<<endl;
}
AsyncVCap:: ~AsyncVCap()
{
	Close();
}
bool AsyncVCap::Open()
{
	if(thread_state != THREAD_READY)
		return false;//do not support reopen.
	bool isOpened = m_core->Open();
	if(isOpened){
		Start();
	}
	return isOpened;
}

void AsyncVCap::Close()
{
	thread_state = THREAD_STOPPING;
	while( THREAD_IDLE != thread_state){
		usleep(100*1000);
	}
	m_core->Close();
}
void AsyncVCap::Capture(char* ptr,int mainORsub)
{
	lock_read(ptr);
}

void AsyncVCap::SetDefaultImg(char *p)
{
	m_core->SetDefaultImg(p);
}

 void AsyncVCap::SavePic(const char* name)
 {
	m_core->SavePic(name);
 }
 
void AsyncVCap::Run()
{
	char *p = new char[m_core->GetTotalBytes()];
	do{
		usleep(100*1000);
	}while(THREAD_READY == thread_state);
	//cap in background thread
	while(thread_state == THREAD_RUNNING)
	{
		m_core->Capture(p);
		lock_write(p);
		usleep(sleepMs*1000);
	}
	thread_state = THREAD_IDLE;
	destroyLock();
	delete []p;
}

void* AsyncVCap::capThread(void*p)
{
	AsyncVCap *thread = (AsyncVCap*)p;
	thread->Run();
	return NULL;
}

void AsyncVCap::lock_read(char *ptr)
{
	char *pImg = m_core->GetDefaultImg();
	pthread_rwlock_rdlock(&rwlock);
	MEMCPY(ptr, pImg,m_core->GetTotalBytes());
	pthread_rwlock_unlock(&rwlock);
}

void AsyncVCap::lock_write(char *ptr)
{
	char *pImg = m_core->GetDefaultImg();
	pthread_rwlock_wrlock(&rwlock);
	MEMCPY(pImg,ptr,m_core->GetTotalBytes());
	pthread_rwlock_unlock(&rwlock);
}
void  AsyncVCap::Start()
{
	thread_state = THREAD_RUNNING;
}
void AsyncVCap::destroyLock()
{
	pthread_rwlock_destroy(&rwlock);
	pthread_rwlockattr_destroy(&rwlockattr);
}

void AsyncVCap::initLock()
{
	pthread_rwlockattr_init(&rwlockattr);
	pthread_rwlockattr_setpshared(&rwlockattr,2);
	pthread_rwlock_init(&rwlock,&rwlockattr);
}
//--------------V4lVcap instance------------------
V4lVcap:: ~V4lVcap()
{
	Close();
}
bool V4lVcap::Open()
{
	if(capUSB)
		cvReleaseCapture(&capUSB);
	capUSB = cvCreateCameraCapture(m_deviceId);

	if(capUSB){
		cvSetCaptureProperty(capUSB, CV_CAP_PROP_FRAME_WIDTH, width);
		cvSetCaptureProperty(capUSB, CV_CAP_PROP_FRAME_HEIGHT, height);
		cvSetCaptureProperty(capUSB, CV_CAP_PROP_FPS, 30);
		cout<<" starting a USB capture at "<<width<<"x"<<height<<endl;
	}
	return capUSB != NULL;
}
void V4lVcap::Close()
{
	if(capUSB)
		cvReleaseCapture(&capUSB);
	capUSB= NULL;
}
void V4lVcap::Capture(char* ptr,int mainORsub)
{
	assert(ptr);
	IplImage *tmp;
	char * src = NULL;
	if(capUSB){
		tmp = cvQueryFrame(capUSB);
		if(tmp){
	//		cout<<"img depth="<<tmp->depth<<",width="<<tmp->width<<",height="<<tmp->height<<",widthstep="<<tmp->widthStep<<endl;
			src = tmp->imageData;
		}
	}
	
	if(!src){
		src= GetDefaultImg();
	}
	if(src != ptr)
		MEMCPY(ptr,src , GetTotalBytes());
}
//-------------------BMPVCap implementation-------
BMPVcap::BMPVcap(const char * fileName):pic(NULL)
{
	assert(fileName);
	strncpy(pFileName, fileName, 64);
}
BMPVcap:: ~BMPVcap()
{
	Close();
}
bool BMPVcap::Open()
{
	bool ret = false;
//	cout<<"   BMPVCcap opening "<<pFileName<<endl;
	if(pic)
		cvReleaseImage(&pic);
	pic = cvLoadImage(pFileName);
	if(pic == NULL)
	{
		cerr<<"failed 2 load "<<pFileName<<".bmp filled with color bar"<<endl;
		pic = cvCreateImage(Size(width, height), IPL_DEPTH_8U, depth);
		if(pic != NULL){
			memcpy(pic->imageData,GetDefaultImg(),GetTotalBytes());
			ret = true;
		}
	}
	else{
		Mat ycrcb;
		Mat im=cvarrToMat(pic);


	//	cvtColor( im, ycrcb, CV_RGB2YCrCb );
		cvtColor( im,yuv_alpha,CV_RGB2BGR); //RGB->RGB
		memcpy(yuv_alpha.data,im.data,im.rows*im.cols*3);
/*
		Vec3b pix;
		Vec4b pix_alpha;
		for (int r = 0; r < ycrcb.rows; r++)
		{
			for (int c = 0; c < ycrcb.cols; c++)
			{
				pix = ycrcb.at<Vec3b>(r,c);
				pix_alpha.val[0]=pix.val[1];
				pix_alpha.val[1]=pix.val[0];
				pix_alpha.val[2]=pix.val[2];
				pix_alpha.val[3]=pix.val[0];
				yuv_alpha.at<Vec4b>(r,c) = pix_alpha;
			}
		}
		*/
		cout<<"BMPVCap Open "<<pFileName<<" OK."<<endl;
		ret = true;
	}
	return ret;
}
void BMPVcap::Close()
{
	if(pic)
		cvReleaseImage(&pic);
	pic = NULL;
}
void BMPVcap::Capture(char* ptr,int mainORsub)
{
	assert(ptr);
	if(pic)
	{
		memcpy(ptr,yuv_alpha.data,W_EXPAND_TIMES*H_EXPAND_TIMES*yuv_alpha.rows*yuv_alpha.cols*yuv_alpha.channels());
	}
}

//-----------------NVcamVcap implementation--------
NVcamVcap::~NVcamVcap()
{
	Close();
}

bool NVcamVcap::Open()
{
	if((capCSI !=NULL) && (capCSI->isOpened()))
		capCSI->release();
	
	char pGstPipelineString[1024];
	sprintf(pGstPipelineString,
			"nvcamerasrc sensor-id=%d ! video/x-raw(memory:NVMM), width=(int)%d, height=(int)%d,format=(string)I420, framerate=(fraction)60/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! appsink",
			m_sensorId, width, height);
	cout<<"NVcamVcap opening "<<pGstPipelineString<<endl;
	capCSI = new VideoCapture(pGstPipelineString); //open the default camera
	if((capCSI==NULL) || (!capCSI->isOpened())) { // check if we succeeded
		cerr << "Failed 2 open camera "<<m_sensorId<< endl;
		return false;
	}
	cout<<"NVcamVcap opening OK"<<endl;
	return true;
}
void NVcamVcap::Close()
{
	if((capCSI !=NULL) && (capCSI->isOpened()))
		capCSI->release();
	delete capCSI;
}
void NVcamVcap::Capture(char* ptr)
{
	assert(ptr);
	if(capCSI && capCSI->isOpened()){
		Mat frameDst(height,width,CV_8UC3,ptr);
		*capCSI>>(frameDst);
	}
	else{
		char * src = GetDefaultImg();
		if(ptr != src)
		    MEMCPY(ptr, src, GetTotalBytes());
	}
}

