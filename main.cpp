#include "RenderMain.h"
#include "GLRender.h"
#include "common.h"
#include "main.h"
#include "thread.h"
#include "recvwheeldata.h"
#include"SelfCheckThread.h"
#include"Zodiac_Message.h"
#include"Zodiac_GPIO_Message.h"



#if TRACK_MODE
#include "VideoProcessTrack.hpp"
#endif

#if MVDETECTOR_MODE
#include "mvDetector.hpp"
#endif
#include"MvDetect.hpp"
RenderMain mainWin;
Common common;
AlarmTarget mainAlarmTarget;
#if MVDECT
MvDetect mv_detect;
#endif
float track_pos[4];

#if TRACK_MODE
void TrkSuccess(UTC_RECT_float m_rctrack)
{
	track_pos[0]=m_rctrack.x;
	track_pos[1]=m_rctrack.y;
	track_pos[2]=m_rctrack.width;
	track_pos[3]=m_rctrack.height;
}
void Trkfailed()
{
	track_pos[0]=0;
	track_pos[1]=0;
	track_pos[2]=0;
	track_pos[3]=0;
}
#endif

#if MVDETECTOR_MODE
void mvDetectorDraw(std::vector<TRK_RECT_INFO> &resTarget,int chId)
{
	int i=0;
	Rect get_rect;
	for(i=0;i<resTarget.size();i++)
		{
			get_rect=resTarget[i].targetRect;
			mainAlarmTarget.SetSingleRectangle(chId,i,get_rect);
		}
		mainAlarmTarget.SetTargetCount(chId,i);
}
#endif
int main(int argc, char** argv)
{

	Parayml param;
	if(!param.readParams("./Param.yml"))
		printf("read param error\n");

//	start_scanner();
//	start_overLap();
//	startrecv( );

#if MVDECT
	mv_detect.init();
#endif
#if USE_GPIO
	InitIPCModule();
	init_GPIO_IPCMessage();
#endif
	start_stitch();
	#if TRACK_MODE
	CVideoProcess* trackTsk=CVideoProcess::getInstance();
	trackTsk->registerCB(TrkSuccess,Trkfailed);
	trackTsk->creat();
	#endif 

	 #if MVDETECTOR_MODE
        mvDetector* mvDetector=mvDetector::getInstance();
        mvDetector->creat();
        mvDetector->init();
        mvDetector->mvDetectorDrawCB(mvDetectorDraw);
        mvDetector->setWarnMode(WARN_MOVEDETECT_MODE,0);
        #endif
	
//	initcabinrecord();//初始化舱内视频记录
//	initscreenrecord();//初始化录屏记录

	mainWin.start(argc, argv);
#if USE_GPIO
	IPC_Destroy();
	delete_GPIO_IPCMessage();
#endif
	//gpio_deinit();

	return 0;
}

