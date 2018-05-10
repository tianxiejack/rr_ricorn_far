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
	static int b=0;
	b++;
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
	//	gettimeofday(&startT[4],0);

		 if(g_windowHeight==768)
		{
			RenderRightPanoView(env,0,g_windowHeight*538.0/768.0,g_windowWidth, g_windowHeight*116.0/768.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,g_windowHeight*655.0/768.0,g_windowWidth, g_windowHeight*115.0/768.0,MAIN,false);
			RenderOnetimeView(env,0,0,g_windowWidth*944.0/1024.0, g_windowHeight*537.0/768.0,MAIN);
		}
		 else
			{
			RenderRightPanoView(env,0,g_windowHeight*648.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,g_windowHeight*864.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,false);
			RenderOnetimeView(env,0,0,g_windowWidth*1152/1920, g_windowHeight*648/1080,MAIN);
			}
		break;
	case FBO_CHOSEN_VIEW_MODE:
		RenderChosenView(env,0,0,g_windowWidth, g_windowHeight,true);
		break;
	case	FBO_ALL_VIEW_559_MODE:
	{
		 if(g_windowHeight==768)
		{
			RenderRightPanoView(env,0,g_windowHeight*538.0/768.0,g_windowWidth, g_windowHeight*116.0/768.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,g_windowHeight*655.0/768.0,g_windowWidth, g_windowHeight*115.0/768.0,MAIN,false);
			RenderOnetimeView(env,0,0,g_windowWidth*944.0/1024.0, g_windowHeight*537.0/768.0,MAIN);
			RenderTwotimesView(env,0,0,g_windowWidth*944.0/1024.0, g_windowHeight*537.0/768.0,MAIN);
			RenderRulerView(env,g_windowWidth*0/1920.0,g_windowHeight*540/1080.0,g_windowWidth,g_windowHeight*140.0/1080,RULER_180);
//1X 2X  MIWEI
		}
		 else
			{
			RenderRightPanoView(env,0,g_windowHeight*648.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,g_windowHeight*864.0/1080.0,g_windowWidth, g_windowHeight*216.0/1080.0,MAIN,false);
			RenderOnetimeView(env,0,0,g_windowWidth*1152/1920, g_windowHeight*648/1080,MAIN);
			RenderTwotimesView(env,0,0,g_windowWidth*944.0/1024.0, g_windowHeight*537.0/768.0,MAIN);
			//	    RenderRulerView(env,g_windowWidth*0/1920.0,g_windowHeight*0.0/1080.0,g_windowWidth,g_windowHeight*140.0/1080,RULER_90);
	//		RenderRulerView(env,g_windowWidth*0/1920.0,g_windowHeight*540/1080.0,g_windowWidth,g_windowHeight*140.0/1080,RULER_180);
			//1X 2X  MIWEI
			}
		 break;
	}
	default:
				break;

	}
#endif
}

