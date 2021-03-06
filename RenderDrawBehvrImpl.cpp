/*FBOdraw() will be expanded,
 * so I write it in a single file to support GLRender,
 * by this means I can add various of cases.
 *author: Fwy */
#include "GLRender.h"
#include"GLEnv.h"
#include "thread_idle.h"
extern GLEnv env1,env2;
extern thread_idle tIdle;
float ruler_move=0.0;
extern float xxx;
extern float yyy;
extern float ruler_90_pos;
extern float ruler_180_pos;
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
		break;

	case FBO_PURE_MODE:
	//	selfcheck.closeSelfCheck();
		static int idleOnce=1;
		if(idleOnce)
		{
			tIdle.threadIdle(SUB_CN);
			tIdle.threadIdle(MAIN_CN);
			tIdle.threadIdle(MVDECT_CN);
			tIdle.threadIdle(MVDECT_ADD_CN);
			idleOnce=0;
		}
		RenderLeftPanoView(env,0,1080.0*(864.0-70-100)/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,true);
		RenderRightPanoView(env,0,1080.0*(648.0-77-200)/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,0,0,0,0,false);

		break;
	case	FBO_ALL_VIEW_559_MODE:
	{
			RenderRightPanoView(env,0,1080.0*(648.0-77)/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,0,0,0,0,true);
			RenderLeftPanoView(env,0,1080.0*825/1080.0,1920.0, 1080.0*216.0/1080.0,MAIN,false);
		if(displayMode!=TRIM_MODE)
		{
			RenderOnetimeView(env,1920.0*60.0/1920.0,1080.0*2/1080.0,1920.0*1000.0/1920, 1080.0*562.5/1080,MAIN,MY_ALL_VIEW_559_MODE);
			RenderTwotimesView(env,1920.0*1120.0/1920.0,1080.0*2/1080.0,1920.0*500.0/1920.0, 1080.0*562.5/1080.0,MAIN);
#if USE_BMPCAP
		RenderRulerView(env,(ruler_90_pos)/1920.0,1080.0*1026/1080.0,1920.0*3,1080.0*140.0/1080.0/2.0,RULER_90);
		RenderRulerView(env,(ruler_180_pos)/1920.0,1080.0*772/1080.0,1920.0*3,1080.0*140.0/1080.0/2.0,RULER_180);
#else
			RenderRulerView(env,(-1920.0*1920.0)/1920.0,1080.0*1026/1080.0,1920.0*3,1080.0*140.0/1080.0/2.0,RULER_90);
			RenderRulerView(env,(-1920.0*1920.0)/1920.0,1080.0*772/1080.0,1920.0*3,1080.0*140.0/1080.0/2.0,RULER_180);
#endif
			RenderMilView(CURRENT_FBO_ALL_VIEW_559_MODE ,env,0, 0,1920, 1080);
		}

		p_ChineseCBillBoard->ChooseTga=TURRET_T;
		RenderChineseCharacterBillBoardAt(env,1920.0*1115.0/1920.0, 1080.0*182.0/1080.0, 1920.0*800.0/1920.0,1080.0*1000.0/1920.0);
		p_ChineseCBillBoard->ChooseTga=PANORAMIC_MIRROR_T;
		RenderChineseCharacterBillBoardAt(env,1920.0*1115.0/1920.0, 1080.0*90.0/1080.0, 1920.0*800.0/1920.0,1080.0*1000.0/1920.0);

		p_ChineseCBillBoard->ChooseTga=ANGLE_T;
		RenderChineseCharacterBillBoardAt(env,1920.0*820.0/1920.0, 1080.0*174.0/1080.0, 1920.0*1100.0/1920.0,1080.0*960.0/1080.0);


	}
	 break;

	default:
				break;

	}
//	button_array->Group_Draw();
#endif
}

