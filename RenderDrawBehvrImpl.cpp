/*FBOdraw() will be expanded,
 * so I write it in a single file to support GLRender,
 * by this means I can add various of cases.
 *author: Fwy */
#include "GLRender.h"
#include"GLEnv.h"
#include "thread_idle.h"
extern GLEnv env1,env2;
extern thread_idle tIdle;
void Render::FBOdraw()
{
	//int t[10]={0};
//	static timeval startT[20]={0};
//	static int b=0;
//	b++;
	#if 1
	GLEnv &env=env1;
//	bool needSendData=true;
	switch(fboMode)
	{
	case  FBO_ALL_VIEW_MODE:
		RenderRightPanoView(env,0,1080.0*648.0/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,0,0,0,0,true);
		RenderLeftPanoView(env,0,1080.0*864.0/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,false);
		if(displayMode!=TRIM_MODE)
		RenderOnetimeView(env,0,0,1920.0*1152/1920, 1080.0*648/1080,MAIN);

#if 0
	//	gettimeofday(&startT[4],0);
		 if(g_windowHeight==768)
		{
			RenderRightPanoView(env,0,g_windowHeight*538.0/768.0,g_windowWidth, g_windowHeight*116.0/768.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,g_windowHeight*655.0/768.0,g_windowWidth, g_windowHeight*115.0/768.0,MAIN,false);
		if(displayMode!=TRIM_MODE)
			RenderOnetimeView(env,0,0,g_windowWidth*944.0/1024.0, g_windowHeight*537.0/768.0,MAIN);
		}
		 else
			{
			RenderRightPanoView(env,0,g_windowHeight*648.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,g_windowHeight*864.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,false);
			if(displayMode!=TRIM_MODE)
			RenderOnetimeView(env,0,0,g_windowWidth*1152/1920, g_windowHeight*648/1080,MAIN);
			}
#endif
		break;

	case FBO_PURE_MODE:
	//	selfcheck.closeSelfCheck();
		static int idleOnce=1;
		if(idleOnce)
		{
			tIdle.threadIdle(SUB_CN);
			tIdle.threadIdle(MAIN_CN);
			tIdle.threadIdle(MVDECT_CN);
			idleOnce=0;
		}
		RenderLeftPanoView(env,0,1080.0*(864.0-70-100)/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,true);
		RenderRightPanoView(env,0,1080.0*(648.0-77-200)/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,0,0,0,0,false);

		break;
	case	FBO_ALL_VIEW_559_MODE:
	{
			RenderRightPanoView(env,0,1080.0*(648.0-77)/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,1080.0*(864.0-70)/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,false);
		if(displayMode!=TRIM_MODE)
		{
			RenderOnetimeView(env,1920.0*60.0/1920.0,1080.0*2/1080.0,1920.0*1000.0/1920, 1080.0*562.5/1080,MAIN,MY_ALL_VIEW_559_MODE);
			RenderTwotimesView(env,1920.0*1120.0/1920.0,1080.0*2/1080.0,1920.0*500.0/1920.0, 1080.0*562.5/1080.0,MAIN);
			RenderRulerView(env,-1920.0*3.0/1920.0,1080.0*980.0/1080.0,1920.0,1080.0*140.0/1080.0,RULER_90);
			RenderMilView(CURRENT_FBO_ALL_VIEW_559_MODE ,env,0, 0,1920, 1080);
		}
	}
	 break;

	default:
				break;

	}
//	button_array->Group_Draw();
#endif
}

