
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <pthread.h>
#include "pboProcessSrcThread.h"
#include<string.h>
#include <glew.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "GLRender.h"
#include "PBOManager.h"
#include <osa_sem.h>
#include "gst_capture.h"
using namespace cv;
extern Render render;

#if GSTREAM_CAP
extern RecordHandle * record_handle;
#endif

void *pbo_process_thread(void *arg)
{

	static bool once4=true;
    Mat testData(CURRENT_SCREEN_HEIGHT, CURRENT_SCREEN_WIDTH, CV_8UC4);
#if GSTREAM_CAP
 initGstCap();
#endif
	while(1)
	{
		OSA_semWait(render.GetPBORcr()->getSemPBO(),100000);
		int processId=render.GetPBORcr()->getCurrentPBOIdx();
#if GSTREAM_CAP
		gstCapturePushData(record_handle, (char *)*render.GetPBORcr()->getPixelBuffer(processId) , CURRENT_SCREEN_WIDTH*CURRENT_SCREEN_HEIGHT*4);

		#else
		static int a=0;
		a++;
					if(a==50)
					{
						memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
						imwrite("./data/50TEST_PBO.bmp",testData);
					}
#if 0
					if(a==100)
					{
						memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
						imwrite("./data/100TEST_PBO.bmp",testData);
					}
					if(a==150)
					{
						memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
						imwrite("./data/150TEST_PBO.bmp",testData);
					}
					if(a==200)
					{
						memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
						imwrite("./data/200TEST_PBO.bmp",testData);
					}
					if(a==250)
					{
						memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
						imwrite("./data/250TEST_PBO.bmp",testData);
					}
					if(a==300)
					{
						memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
						imwrite("./data/300TEST_PBO.bmp",testData);
					}
#endif
#endif
	}
}


void start_pbo_process_thread(void)
{
	pthread_t tid;
	int ret;
	ret = pthread_create( &tid, NULL,pbo_process_thread, NULL );
}
