/*FBOdraw() will be expanded,
 * so I write it in a single file to support GLRender,
 * by this means I can add various of cases.
 *author: Fwy */
#include "GLRender.h"

void Render::FBOdraw()
{
#if 0
	bool needSendData=true;
	switch(fboMode)
	{
	case  FBO_ALL_VIEW_MODE:
		if(displayMode==FRONT_BACK_PANO_ADD_SMALLMONITOR_VIEW_MODE)
		{
			needSendData=false;
		}
			RenderRightPanoView(0,g_windowHeight*0.0/1080.0,g_windowWidth*1920.0/1920.0, g_windowHeight*540.0/1080.0,needSendData);
			RenderLeftPanoView(0,g_windowHeight*540.0/1080.0,g_windowWidth*1920.0/1920.0, g_windowHeight*540.0/1080.0,needSendData);
		break;
	case FBO_VGA_VIEW_MODE:
		if(displayMode==VGA_WHITE_VIEW_MODE)
		{
			needSendData=false;
		}
		break;
	default:
				break;

	}
#endif
}

