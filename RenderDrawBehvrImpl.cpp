/*FBOdraw() will be expanded,
 * so I write it in a single file to support GLRender,
 * by this means I can add various of cases.
 *author: Fwy */
#include "GLRender.h"

void Render::FBOdraw()
{
	bool needSendData=true;
	switch(fboMode)
	{
	case  FBO_ALL_VIEW_MODE:
		if(displayMode==ALL_VIEW_FRONT_BACK_ONE_DOUBLE_MODE)
		{
			needSendData=false;
		}
		RenderRightPanoView(0,g_windowHeight*440.0/1080.0,g_windowWidth, g_windowHeight*320.0/1080.0,needSendData);
		RenderLeftPanoView(0,g_windowHeight*760.0/1080.0,g_windowWidth, g_windowHeight*320.0/1080.0,needSendData);
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

}

