/*FBOdraw() will be expanded,
 * so I write it in a single file to support GLRender,
 * by this means I can add various of cases.
 *author: Fwy */
#include "GLRender.h"
#include"GLEnv.h"
extern GLEnv env1,env2;
void Render::FBOdraw()
{
	int t[10]={0};
	static timeval startT[20]={0};
	#if 1
	GLEnv &env=env1;
	bool needSendData=true;
	switch(fboMode)
	{
	case  FBO_ALL_VIEW_MODE:
		if(displayMode==ALL_VIEW_MODE)
		{
			needSendData=true;
		}
		gettimeofday(&startT[4],0);
		RenderRightPanoView(env,0,g_windowHeight*864.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,0,0,0,0,needSendData);
		gettimeofday(&startT[5],0);

		RenderLeftPanoView(env,0,g_windowHeight*648.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,needSendData);
		gettimeofday(&startT[6],0);
		RenderOnetimeView(env,0,0,g_windowWidth*1152/1920, g_windowHeight*648/1080,MAIN);
		gettimeofday(&startT[7],0);
	    RenderRulerView(env,g_windowWidth*0/1920.0,g_windowHeight*0.0/1080.0,g_windowWidth,g_windowHeight*140.0/1080,RULER_90);
			    RenderRulerView(env,g_windowWidth*0/1920.0,g_windowHeight*540/1080.0,g_windowWidth,g_windowHeight*140.0/1080,RULER_180);

		t[2]=(startT[5].tv_sec-startT[4].tv_sec)*1000000+(startT[5].tv_usec-startT[4].tv_usec);
		t[3]=(startT[6].tv_sec-startT[5].tv_sec)*1000000+(startT[6].tv_usec-startT[5].tv_usec);
		t[4]=(startT[7].tv_sec-startT[6].tv_sec)*1000000+(startT[7].tv_usec-startT[6].tv_usec);

	//	printf("deltatimet[5]-t[4] =%d us\n",t[2]);
	//	printf("deltatimet[6]-t[5] =%d us\n",t[3]);
	//	printf("deltatimet[7]-t[6] =%d us\n",t[4]);
		break;
	case FBO_CHOSEN_VIEW_MODE:
		RenderChosenView(env,0,0,g_windowWidth, g_windowHeight,true);
		break;
	default:
				break;

	}
#endif
}

