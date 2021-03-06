#include "multiLayerButtonGroup.h"
#include <stdlib.h>
#include "menu_button.h"
#include <GLMatrixStack.h>
#include <GLFrustum.h>
#include <assert.h>
#include <iostream>
#include <vector>
#if USE_ON_BOARD
#include "IPC_Near_Recv_Message.h"
#endif
#define MARGIN_X_OFFSET 0.05
#define MARGIN_Y_OFFSET (-0.05)

#define BUTTON_WIDTH 0.1
#define GAP_X	(BUTTON_WIDTH*0.03)
#define MARGIN_X 0

#define BUTTON_HEIGHT (1.0/(MENU_BUTTON_VER_COUNT*1.5))
#define GAP_Y (BUTTON_HEIGHT*0.5)
#define MARGIN_Y 0
extern void* getDefaultShaderMgr();

#define HIGHT_LIGHT_WAIT_COUNT 20

static GLfloat vbuttonnormal[]={0.8,0.8,0.0,0.3};
static GLfloat vbuttonchoose[]={0.0,0.3,0.3,0.7};

#define BUTTON_ALLVIEW "m_allview.tga"
#define BUTTON_ANTI_CLOCKWISE			"m_anti_clockwise.tga"
#define BUTTON_AROUND "m_around.tga"
#define BUTTON_CANCLE "m_cancle.tga"
#define BUTTON_CLOCKWISE "m_clockwise.tga"
#define BUTTON_CLOSE "m_close.tga"
#define BUTTON_DOWN "m_down.tga"
#define BUTTON_ENHANCE "m_enhance.tga"
#define BUTTON_ERROR_STATE "m_error_state.tga"
#define BUTTON_FORESIGHT "m_foresight.tga"
#define BUTTON_FRONT "m_front.tga"
#define BUTTON_LEFT "m_left.tga"
#define BUTTON_ENSURET	 "m_ensure.tga"
#define BUTTON_BACK "m_back.tga"
#define BUTTON_MENU "m_menu.tga"
#define BUTTON_MODE "m_mode.tga"
#define BUTTON_NEAR "m_near.tga"
#define BUTTON_OVERLOOK "m_overlook.tga"
#define BUTTON_RECORD "m_record.tga"
#define BUTTON_RETURN "m_return.tga"
#define BUTTON_RIGHT "m_right.tga"
#define BUTTON_SINGLE_CHANNEL "m_single_channel.tga"
#define BUTTON_SINGLE_SELECTION "m_single_selection.tga"
#define BUTTON_START "m_start.tga"
#define BUTTON_STOP "m_stop.tga"
#define BUTTON_TARGET "m_target.tga"
#define BUTTON_TELESCOPE "m_telescope.tga"
#define BUTTON_TROCHOID "m_trochoid.tga"
#define BUTTON_UP "m_up.tga"
#define BUTTON_ZERO_POSITION "m_zero_position.tga"

#define BUTTON_ONE "m_one.tga"
#define BUTTON_TWO "m_two.tga"
#define BUTTON_THREE "m_three.tga"
#define BUTTON_FOUR "m_four.tga"
typedef struct buttonMask{
	int idx_x;
	int idx_y;
	char * tgaFileName;
	int keyCode;
}buttonMask;

enum{
	GO_TO_SUBMENU_mask_default_round_ui = 256,
	GO_TO_SUBMENU_mask_basic_ui,
	GO_TO_SUBMENU_mask_choose_mode_ui,

	GO_TO_SUBMENU_mask_around_mode_ui,
	GO_TO_SUBMENU_mask_foresight_mode_ui,
	GO_TO_SUBMENU_mask_telescope_mode_ui,
	GO_TO_SUBMENU_mask_telescope_mode_teturn_ui,
	GO_TO_SUBMENU_mask_single_channel_mode_ui,

	GO_TO_SUBMENU_mask_mvdetect_mode_ui,
	GO_TO_SUBMENU_mask_mvdetect_tel_mode_ui,
	GO_TO_SUBMENU_mask_enhance_mode_ui,
	GO_TO_SUBMENU_mask_enhance_noforesights_mode_ui,
	GO_TO_SUBMENU_mask_enhance_tel_mode_ui,
	GO_TO_SUBMENU_mask_record_mode_ui,

	GO_TO_SUBMENU_mask_zero_pos_ui,
	GO_TO_SUBMENU_LAST
};

//1.1車長基礎界面
static buttonMask mask_basic_ui[]={
		{0, 3, BUTTON_ALLVIEW, GO_TO_SUBMENU_mask_default_round_ui},
		{0, 1, BUTTON_ERROR_STATE, '?'},
	//	{0, 1, BUTTON_RETURN, '?'}
};

//2.1全景調節界面，默認環視模式
static buttonMask mask_default_round_ui[]={
		{0, 3, BUTTON_ZERO_POSITION, GO_TO_SUBMENU_mask_zero_pos_ui},
		{0, 2, BUTTON_RETURN, GO_TO_SUBMENU_mask_basic_ui},
		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
	//	{2, 0, BUTTON_TARGET, 'O'},
	//	{4, 0, BUTTON_ENHANCE, 'P'},
	//	{6, 0, BUTTON_FORESIGHT, 6},
		{8, 0, BUTTON_RECORD, 'J'}
};

//0位置調整
static buttonMask mask_zero_pos_ui[]={
		{0, 3, BUTTON_ZERO_POSITION, 0},
		{0, 2, BUTTON_RETURN, GO_TO_SUBMENU_mask_default_round_ui},
	//	{0, 1, BUTTON_ERROR_STATE, '?'},

		{0, 7, BUTTON_LEFT, 'x'},
		{0, 5, BUTTON_RIGHT, 'z'},
		{0, 4, BUTTON_ENSURET, 'Z'},

};

//2.2模式選擇界面
static buttonMask mask_choose_mode_ui[]={
		{0, 6, BUTTON_TELESCOPE, GO_TO_SUBMENU_mask_telescope_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'},
		//	{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		//	{4, 0, BUTTON_ENHANCE, 'P'},
		//	{2, 0, BUTTON_TARGET, 'O'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{0, 2, BUTTON_RETURN, GO_TO_SUBMENU_mask_basic_ui},
		{0, 5, BUTTON_SINGLE_CHANNEL, GO_TO_SUBMENU_mask_single_channel_mode_ui},
		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 7, BUTTON_AROUND,GO_TO_SUBMENU_mask_around_mode_ui }








};

//2.3環視模式
static buttonMask mask_around_mode_ui[]={
	//	{0, 7, BUTTON_AROUND, '|'},
	//	{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{2, 0, BUTTON_TARGET, GO_TO_SUBMENU_mask_mvdetect_mode_ui},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_enhance_mode_ui},
		{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

//2.4潛望模式
static buttonMask mask_telescope_mode_ui[]={
	//	{0, 6, BUTTON_TELESCOPE, '|'},
		{0, 5, BUTTON_FRONT, '5'},
		{0, 4, BUTTON_BACK, '6'},
		{0, 3, BUTTON_LEFT, '7'},
		{0, 2, BUTTON_RIGHT, '8'},
	//	{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{2, 0, BUTTON_TARGET, 'Q'},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_enhance_tel_mode_ui},
		//{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

//2.5單通道模式
static buttonMask mask_single_channel_mode_ui[]={

	//	{0, 5, BUTTON_SINGLE_CHANNEL, 't'},
		{0, 4, BUTTON_ANTI_CLOCKWISE, 'u'},
		{0, 3, BUTTON_CLOCKWISE, 'i'},
	//	{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
	//	{2, 0, BUTTON_TARGET, 'O'},
	//	{4, 0, BUTTON_ENHANCE, 'P'},
	//	{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

//3.1目標檢測模式
static buttonMask mask_mvdetect_mode_ui[]={
		{0, 7, BUTTON_UP, 'l'},
		{0, 6, BUTTON_DOWN, 'l'},
		{0, 5, BUTTON_LEFT, 'j'},
		{0, 4, BUTTON_RIGHT, 'k'},
		{0, 3, BUTTON_SINGLE_SELECTION, 'O'},
		{0, 2, BUTTON_CANCLE, 'o'},
		//{0, 1, BUTTON_CLOSE, 'o'},
	//	{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{2, 0, BUTTON_TARGET, GO_TO_SUBMENU_mask_around_mode_ui},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_enhance_noforesights_mode_ui},
	//	{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

static buttonMask mask_mvdetect_return_tel_mode_ui[]={
	//	{0, 7, BUTTON_UP, 'l'},
	//	{0, 6, BUTTON_DOWN, 'l'},
		{0, 5, BUTTON_LEFT, 'j'},
		{0, 4, BUTTON_RIGHT, 'k'},
		{0, 3, BUTTON_SINGLE_SELECTION, 'O'},
		{0, 2, BUTTON_CANCLE, 'o'},
	//	{0, 1, BUTTON_CLOSE, 'o'},
//		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		//{2, 0, BUTTON_TARGET, GO_TO_SUBMENU_mask_telescope_mode_ui},
		{2, 0, BUTTON_TARGET, 'Q'},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_enhance_tel_mode_ui},
	//	{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

//4 分划調節
static buttonMask mask_foresight_mode_ui[]={
		{0, 7, BUTTON_UP, '#'},
		{0, 6, BUTTON_DOWN, '$'},
		{0, 5, BUTTON_LEFT, '!'},
		{0, 4, BUTTON_RIGHT, '@'},
//		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{2, 0, BUTTON_TARGET, GO_TO_SUBMENU_mask_mvdetect_mode_ui},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_enhance_mode_ui},
		{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_around_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}
};

//5增強模式
static buttonMask mask_enhance_mode_ui[]={
		{0, 7, BUTTON_ONE, 'c'},
		{0, 6, BUTTON_TWO, 'v'},
		{0, 5, BUTTON_THREE, 'C'},
		{0, 4, BUTTON_FOUR, 'V'},
		{0, 3, BUTTON_CLOSE, 'p'},
//		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{2, 0, BUTTON_TARGET, GO_TO_SUBMENU_mask_mvdetect_mode_ui},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_around_mode_ui},
		{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

static buttonMask mask_enhance_noforesight_mode_ui[]={
		{0, 7, BUTTON_ONE, 'c'},
		{0, 6, BUTTON_TWO, 'v'},
		{0, 5, BUTTON_THREE, 'C'},
		{0, 4, BUTTON_FOUR, 'V'},
		{0, 3, BUTTON_CLOSE, 'p'},
//		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{2, 0, BUTTON_TARGET, GO_TO_SUBMENU_mask_mvdetect_mode_ui},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_around_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};


static buttonMask mask_enhance_tel_mode_ui[]={
		{0, 7, BUTTON_ONE, 'c'},
		{0, 6, BUTTON_TWO, 'v'},
		{0, 5, BUTTON_THREE, 'C'},
		{0, 4, BUTTON_FOUR, 'V'},
		{0, 3, BUTTON_CLOSE, 'p'},
//		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
		{2, 0, BUTTON_TARGET, GO_TO_SUBMENU_mask_mvdetect_tel_mode_ui},
		{4, 0, BUTTON_ENHANCE, GO_TO_SUBMENU_mask_telescope_mode_teturn_ui},
	//	{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

//6儲存視頻
static buttonMask mask_record_mode_ui[]={
		{0, 4, BUTTON_START	, 'G'},
		{0, 3, BUTTON_CLOSE, 'g'},

		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
	//	{2, 0, BUTTON_TARGET, 'O'},
	//	{4, 0, BUTTON_ENHANCE, 'P'},
	//	{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}
};

//7零位校正
static buttonMask mask_zero_position_mode_ui[]={
		{0, 4, BUTTON_ZERO_POSITION, '|'},//todo
		{0, 3, BUTTON_RETURN, GO_TO_SUBMENU_mask_basic_ui},

		{0, 0, BUTTON_MODE, GO_TO_SUBMENU_mask_choose_mode_ui},
	//	{2, 0, BUTTON_TARGET, 'O'},
	//	{4, 0, BUTTON_ENHANCE, 'P'},
	//	{6, 0, BUTTON_FORESIGHT, GO_TO_SUBMENU_mask_foresight_mode_ui},
		{8, 0, BUTTON_RECORD, 'J'}

};

//8菜單消隱界面
static buttonMask mask_disappear_mode_ui[]={
		{9, 0, BUTTON_MENU, 's'}
};


/**********駕駛員界面**********/
//1.1駕駛員基礎界面
static buttonMask mask_driver_basic_mode_ui[]={

		{0, 1, BUTTON_ERROR_STATE, '?'},
		{0, 2, BUTTON_NEAR, 's'},
		{0, 3, BUTTON_ALLVIEW, 's'},
};

//2.1駕駛員全景調節界面
static buttonMask mask_driver_far_allview_mode_ui[]={

		{0, 2, BUTTON_RETURN, 's'},
		{0, 0, BUTTON_MODE, 's'},
			{2, 0, BUTTON_TARGET, 's'},
			{4, 0, BUTTON_ENHANCE, 's'},
			{6, 0, BUTTON_FORESIGHT, 's'}
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//近景界面
static buttonMask mask_driver_near_allview_mode_ui[]={
		{0, 3, BUTTON_RETURN, 's'},
		{0, 0, BUTTON_SINGLE_CHANNEL, 's'},
		{2, 0, BUTTON_OVERLOOK, 's'},
		{4, 0, BUTTON_ENHANCE, 's'}
};

//駕駛員菜單消隱界面
static buttonMask mask_driver_disappear_mode_ui[]={
		{9, 0, BUTTON_MENU, 's'}
};


//近景單通道
static buttonMask mask_driver_near_single_channel_mode_ui[]={
		{0, 3, BUTTON_TROCHOID, 's'},
		{0, 0, BUTTON_SINGLE_CHANNEL, 's'},
		{2, 0, BUTTON_OVERLOOK, 's'},
		{4, 0, BUTTON_ENHANCE, 's'}
};
//近景增強模式
static buttonMask mask_driver_near_enhance_mode_ui[]={
		{0, 7, BUTTON_ONE, 's'},
		{0, 6, BUTTON_TWO, 's'},
		{0, 5, BUTTON_THREE, 's'},
		{0, 4, BUTTON_FOUR, 's'},
		{0, 3, BUTTON_CLOSE, 's'},

		{0, 0, BUTTON_SINGLE_CHANNEL, 's'},
		{2, 0, BUTTON_OVERLOOK, 's'},
		{4, 0, BUTTON_ENHANCE, 's'}
};

//近景俯瞰模式
static buttonMask mask_driver_near_overlook_mode_ui[]={
		{0, 7, BUTTON_FRONT, 's'},
		{0, 6, BUTTON_BACK, 's'},
		{0, 5, BUTTON_LEFT, 's'},
		{0, 4, BUTTON_RIGHT, 's'},
		{0, 3, BUTTON_TROCHOID, 's'},

		{0, 0, BUTTON_SINGLE_CHANNEL, 's'},
		{2, 0, BUTTON_OVERLOOK, 's'},
		{4, 0, BUTTON_ENHANCE, 's'}
};



#define MAX_LAYEDED_GROUP_COUNT 23
//以下两个数组要同步更改
static buttonMask* pMasks[MAX_LAYEDED_GROUP_COUNT]= {
		mask_basic_ui,						//0
		mask_default_round_ui,				//1
		mask_choose_mode_ui,			//2
		mask_around_mode_ui,			//3
		mask_telescope_mode_ui,			//4
		mask_single_channel_mode_ui,			//5
		mask_mvdetect_mode_ui,			//6
		mask_mvdetect_return_tel_mode_ui,			//7
		mask_foresight_mode_ui,			//8
		mask_enhance_mode_ui,			//9
		mask_enhance_tel_mode_ui,			//10
		mask_record_mode_ui,			//11
		mask_zero_position_mode_ui,			//12
		mask_disappear_mode_ui,			//13
		mask_driver_basic_mode_ui,			//14
		mask_driver_disappear_mode_ui,			//15
		mask_driver_far_allview_mode_ui,			//16
		mask_driver_near_allview_mode_ui,			//17
		mask_driver_near_single_channel_mode_ui,			//18
		mask_driver_near_enhance_mode_ui,			//19
		mask_driver_near_overlook_mode_ui,	//20
		mask_zero_pos_ui,//21;
		mask_enhance_noforesight_mode_ui//22
};

static int MaskLengths[MAX_LAYEDED_GROUP_COUNT] = {
		sizeof(mask_basic_ui)/sizeof(mask_basic_ui[0]),
		sizeof(mask_default_round_ui)/sizeof(mask_default_round_ui[0]),
		sizeof(mask_choose_mode_ui)/sizeof(mask_choose_mode_ui[0]),
		sizeof(mask_around_mode_ui)/sizeof(mask_around_mode_ui[0]),
		sizeof(mask_telescope_mode_ui)/sizeof(mask_telescope_mode_ui[0]),
		sizeof(mask_single_channel_mode_ui)/sizeof(mask_single_channel_mode_ui	[0]),
		sizeof(mask_mvdetect_mode_ui)/sizeof(mask_mvdetect_mode_ui[0]),
		sizeof(mask_mvdetect_return_tel_mode_ui)/sizeof(mask_mvdetect_return_tel_mode_ui[0]),
		sizeof(mask_foresight_mode_ui)/sizeof(mask_foresight_mode_ui[0]),
		sizeof(mask_enhance_mode_ui)/sizeof(mask_enhance_mode_ui[0]),
		sizeof(mask_enhance_tel_mode_ui)/sizeof(mask_enhance_tel_mode_ui[0]),
		sizeof(mask_record_mode_ui)/sizeof(mask_record_mode_ui[0]),
		sizeof(mask_zero_position_mode_ui)/sizeof(mask_zero_position_mode_ui[0]),
		sizeof(mask_disappear_mode_ui)/sizeof(mask_disappear_mode_ui[0]),
		sizeof(mask_driver_basic_mode_ui)/sizeof(mask_driver_basic_mode_ui[0]),
		sizeof(mask_driver_disappear_mode_ui)/sizeof(mask_driver_disappear_mode_ui[0]),
		sizeof(mask_driver_far_allview_mode_ui)/sizeof(mask_driver_far_allview_mode_ui[0]),
		sizeof(mask_driver_near_allview_mode_ui)/sizeof(mask_driver_near_allview_mode_ui[0]),
		sizeof(mask_driver_near_single_channel_mode_ui)/sizeof(mask_driver_near_single_channel_mode_ui[0]),
		sizeof(mask_driver_near_enhance_mode_ui)/sizeof(mask_driver_near_enhance_mode_ui[0]),
		sizeof(mask_driver_near_overlook_mode_ui)/sizeof(mask_driver_near_overlook_mode_ui[0]),
		sizeof(mask_zero_pos_ui)/sizeof(mask_zero_pos_ui[0]),
		sizeof(mask_enhance_noforesight_mode_ui)/sizeof(mask_enhance_noforesight_mode_ui[0]),


};
//---------------------------------------------------------------

BaseBillBoard::BaseBillBoard(GLShaderManager* mgr):
	m_pShaderManager(mgr),	blendmode(1)
{
	if(NULL == m_pShaderManager){
		m_pShaderManager = (GLShaderManager*)getDefaultShaderMgr();
	}
}
BaseBillBoard::~BaseBillBoard()
{

}
bool BaseBillBoard::LoadTGATextureRect(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL)
		return false;

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, wrapMode);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);

    free(pBits);
	std::cout<<"Basebbd Load TGA Texture "<<szFileName<<" ok"<<std::endl;
	return true;
}

void BaseBillBoard::Init(int x, int y, int width, int height)
{
	x=0;  //startx
	y=155; //y must be the height of your own tga's height
	width=300;//x must be the height of your own tga's width
	height=155;//y must be the height of your own tga's height
	HZbatch.Begin(GL_TRIANGLE_FAN, 4, 1);

	// Upper left hand corner
	HZbatch.MultiTexCoord2f(0, 0.0f, height);
	HZbatch.Vertex3f(x, y, 0.0);

	// Lower left hand corner
	HZbatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	HZbatch.Vertex3f(x, y - height, 0.0f);

	// Lower right hand corner
	HZbatch.MultiTexCoord2f(0, width, 0.0f);
	HZbatch.Vertex3f(x + width, y - height, 0.0f);

	// Upper righ hand corner
	HZbatch.MultiTexCoord2f(0, width, height);
	HZbatch.Vertex3f(x + width, y, 0.0f);

	HZbatch.End();
	InitTextures();

}

void BaseBillBoard::SwitchBlendMode(int blendmode)
{
	switch (blendmode)
		{
		case 1:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case 2:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
			break;
		case 3:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case 4:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case 5:
			glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
			break;
		case 6:
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case 7:
			glBlendFuncSeparate(GL_SRC_COLOR, GL_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		default:
			glDisable(GL_BLEND);
		}
}
void BaseBillBoard::DrawBillBoard(int w, int h)
{
	M3DMatrix44f mScreenSpace;
	m3dMakeOrthographicMatrix(mScreenSpace, 0.0f,400.0f, 0.0f, 200.0f, -1.0f, 1.0f);
	// Turn blending on, and depth testing off
	glEnable(GL_BLEND);//使能透明
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	DoTextureBinding();
	SwitchBlendMode(blendmode);
	m_pShaderManager->UseStockShader(GLT_SHADER_TEXTURE_RECT_REPLACE,mScreenSpace,0);
	HZbatch.Draw();
	    // Restore no blending and depth test
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------
//----------------------------inherited billboard class implementation----------------------
MenuBillBoard::MenuBillBoard(const char* filename,GLShaderManager* mgr)
	:BaseBillBoard((mgr))
{
	if(filename)
		strcpy(m_BBDTextureFileName, filename);
}
MenuBillBoard::~MenuBillBoard()
{
	glDeleteTextures(1, m_BBDTextures);
}
void MenuBillBoard::LoadTGATextureRects()
{
    glBindTexture(GL_TEXTURE_RECTANGLE, m_BBDTextures[0]);
	LoadTGATextureRect(m_BBDTextureFileName, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
}

void MenuBillBoard::SetTgaFileName(const char * name)
{
	if(name)
		strcpy(m_BBDTextureFileName, name);
}

void MenuBillBoard::InitTextures(){
    glGenTextures(1, m_BBDTextures);
	LoadTGATextureRects();
}

void MenuBillBoard::DoTextureBinding()
{
	glBindTexture(GL_TEXTURE_RECTANGLE, m_BBDTextures[0]);
}

//----------------------------------------------------------------
void MenuButton::init_button(float * color_normal,float * color_choose,int state,float x,float y,float width,float height,GLShaderManager * shaderManager,int key)
{
		button_color[0]=color_normal;
		button_color[1]=color_choose;
		choose_state=state;
		start_x=x;
		start_y=y;
		button_width=width;
		button_height=height;
		shaderMgr=shaderManager;
		key_data=key;

		buttonBBoard.Init(-width/2,height/2,width,height);
}

void MenuButton::Draw(int width, int height,GLFrustum * viewFrustum,GLMatrixStack *modelViewMatrix, GLMatrixStack *projectionMtrx,
		int id)
{
		static int count=0;
		float x = start_x*width, y = start_y*height,w=button_width*width, h = button_height*height;
		glViewport(x,y,w,h);
		viewFrustum->SetPerspective(90.0f, float(w) / float(h), 1.0f, 4000.0f);
		projectionMtrx->LoadMatrix(viewFrustum->GetProjectionMatrix());

		modelViewMatrix->PushMatrix();
		modelViewMatrix->LoadIdentity();
		// move h since the shadow dimension is [-1,1], use h/2 if it is [0,1]
		modelViewMatrix->Translate(0.0f, 0.0f, -h);
		modelViewMatrix->Scale(w, h, 1.0f);

			if(choose_state==1)
			{
				count++;
				if(count>HIGHT_LIGHT_WAIT_COUNT)
				{
					count=0;
					choose_state=0;
				}
			}
			buttonBBoard.DrawBillBoard(w,h);

		modelViewMatrix->PopMatrix();
}

void MenuButton::SetTgaFileName(const char * name)
{
	buttonBBoard.SetTgaFileName(name);
}

bool MenuButton::FindPointOnButton(float x,float y){
	if(x>=start_x&&x<=(start_x+button_width))
	{
		if(y>=start_y&&y<=(start_y+button_height))
		{

			return true;
		}
	}
	return false;
};

void ButtonGroup::Group_Draw(int width,int height,GLMatrixStack * p_modelViewMatrix,
		GLMatrixStack * p_projectionMtrx, GLFrustum * viewFrustum)
{
	
	window_width=width;
	window_height=height;

	for(int i=0;i<m_buttonsVect.size();i++)
	{
		m_buttonsVect[i]->Draw(width, height, viewFrustum,p_modelViewMatrix,p_projectionMtrx);
	}
};

bool ButtonGroup::Update_State(InterfaceRenderBehavior* p_Host){
	bool isTouched=false;
#if USE_ON_BOARD
	COOR_SYS point_data=getClickCoordinate();

		pos_x=point_data.point_x;
		pos_y=point_data.point_y;
#else
		pos_x=p_Host->GetTouchPosX();
		pos_y=p_Host->GetTouchPosY();


		p_Host->SetTouchPosX(-1);
		p_Host->SetTouchPosY(-1);
#endif
		//pos_x=456;
		//pos_y=162;
if(pos_x>0 ||pos_y>0)
{
	isTouched=true;
}

	window_width=p_Host->GetWindowWidth();
	window_height=p_Host->GetWindowHeight();

	currentHightLightButtonId = FindButton(pos_x,pos_y,window_width,window_height);

	if(currentHightLightButtonId>=0){
		int keyData = m_buttonsVect[currentHightLightButtonId]->getKeycode();
		if(keyData >= 0){
			p_Host->processKeycode(keyData);
		}
	}
return isTouched;

};

void ButtonGroup::acceptButtonMask(buttonMask* mask, int count)
{
	m_Maskbutton.clear();
	for(int i = 0; i < count; i++){
		m_Maskbutton.push_back(&mask[i]) ;
	}
}


void ButtonGroup::init_button_group(int button_count,GLfloat * color_normal,GLfloat * color_choose,int * state,float * x,float * y,float * width,float * height,GLShaderManager * shaderManager)
{
	shaderMgr_group=shaderManager;
	int count = m_Maskbutton.size();
	pbuttons=new MenuButton[count];
	for(int l=0;l<button_count;l++)
	{
		//遍历所有按钮遮罩
		for(int i = 0; i<count; i ++){
//如果这个位置是有按钮的，把此位置对应的按钮初始化并加入按钮组
			if(l == m_Maskbutton[i]->idx_x + MENU_BUTTON_HOR_COUNT * m_Maskbutton[i]->idx_y ){
				pbuttons[i].SetTgaFileName(m_Maskbutton[i]->tgaFileName );
				pbuttons[i].init_button(color_normal,color_choose,state[l],x[l],y[l],width[l],height[l],shaderMgr_group,m_Maskbutton[i]->keyCode);
				pbuttons[i].SetShaderMgr(shaderMgr_group);

				Append_Group(pbuttons+i);
			}
	   }
	}
};

ButtonGroup::~ButtonGroup()
{
	delete [] pbuttons;
}

void ButtonGroup::HightlightButton(int id)
{
	if(id>=0)
		m_buttonsVect[id]->choose_state=1;
}

int ButtonGroup::FindButton(float x,float y,int window_width,int window_height)
{

	int activatedbutton=-1;
	float posx=x/(window_width-1.0);
	float posy=y/(window_height -1.0);
	for(int i=0;i<m_buttonsVect.size();i++)
	{
		if(m_buttonsVect[i]->FindPointOnButton(posx,posy))
		{
			activatedbutton=i;
						m_buttonsVect[i]->choose_state=1;
			break;
		}
	}

	return activatedbutton;
}

void ButtonGroup::Append_Group(MenuButton * button)
{
	m_buttonsVect.push_back( button);
}
//-------------------------------------multiLayerButtonGroup---------------------------
multiLayerButtonGroup::multiLayerButtonGroup(InterfaceRenderBehavior *p,int groupCount)
	:currentActiveBGIndex(0),m_submenuKeycode(0),counter(AUTO_HIDE_COUNT)
{

    p_Host = p;
	pbuttongroups=new ButtonGroup[groupCount];
	for(int l=0;l<groupCount;l++)
	{
		m_layeredButtonGroupsVect.push_back(pbuttongroups+l);
	}
}

multiLayerButtonGroup::~multiLayerButtonGroup(){

	m_layeredButtonGroupsVect.clear();
	delete []pbuttongroups;
}

void multiLayerButtonGroup::Group_Draw()
{
	if(enable_draw)
	{
		if(counter--<=0)
		{
			this->SetEnableDraw(false);
		}
		if(m_submenuKeycode > 255){
           switch(m_submenuKeycode)
           {//不需要host切換模式的時候，只修改submenuIndex
           case GO_TO_SUBMENU_mask_basic_ui:
        	   SetcurrentActiveBGIndex(0);
        	   break;
           case GO_TO_SUBMENU_mask_default_round_ui:
        	   SetcurrentActiveBGIndex(1);
        	   break;
           case   GO_TO_SUBMENU_mask_choose_mode_ui:
        	   SetcurrentActiveBGIndex(2);
        	   break;
           case   GO_TO_SUBMENU_mask_around_mode_ui:
			   SetcurrentActiveBGIndex(3);
			   p_Host->processKeycode('1');
			   break;
           case   GO_TO_SUBMENU_mask_telescope_mode_ui:
			   SetcurrentActiveBGIndex(4);
			   p_Host->processKeycode('2');
			   break;
           case GO_TO_SUBMENU_mask_telescope_mode_teturn_ui:
        	   SetcurrentActiveBGIndex(4);
        	   break;
           case GO_TO_SUBMENU_mask_single_channel_mode_ui:
        	   SetcurrentActiveBGIndex(5);
        	   p_Host->processKeycode('o');
        	   p_Host->processKeycode('3');
        	   p_Host->processKeycode('p');
        	   break;

           case GO_TO_SUBMENU_mask_mvdetect_mode_ui:
        	   SetcurrentActiveBGIndex(6);
        	   p_Host->processKeycode('O');
        	   break;
           case GO_TO_SUBMENU_mask_mvdetect_tel_mode_ui:
        	   SetcurrentActiveBGIndex(4);
        	   p_Host->processKeycode('Q');
        	   break;
           case GO_TO_SUBMENU_mask_foresight_mode_ui:
           	   SetcurrentActiveBGIndex(8);
           	 p_Host->processKeycode('o');
           	   break;
           case GO_TO_SUBMENU_mask_enhance_mode_ui:
         	   SetcurrentActiveBGIndex(9);
         	   p_Host->processKeycode('P');
         	   break;
           case GO_TO_SUBMENU_mask_enhance_noforesights_mode_ui:
        	   SetcurrentActiveBGIndex(22);
			   p_Host->processKeycode('P');
			   break;
           case GO_TO_SUBMENU_mask_enhance_tel_mode_ui:
         	   SetcurrentActiveBGIndex(10);
         	  p_Host->processKeycode('P');
         	   break;
           case   GO_TO_SUBMENU_mask_record_mode_ui:
        	   SetcurrentActiveBGIndex(11);
        	   break;
           case GO_TO_SUBMENU_mask_zero_pos_ui:
        	   SetcurrentActiveBGIndex(21);
        	   p_Host->processKeycode('1');
        	   break;
           default:
        	   break;
           }
		}

		else if(m_submenuKeycode==-1)
		{
			//如果是-1,則沒有最新點擊事件發生
		}
		else {//let glrender handle it

			SetcurrentActiveBGIndex(p_Host->getGroupMenuIndex());
		}
		m_submenuKeycode = -1;//保證keycode只被調用一次，而不被反覆調用
		Update_State();
		m_layeredButtonGroupsVect[currentActiveBGIndex]->Group_Draw(p_Host->GetWindowWidth(),
			p_Host->GetWindowHeight(),this->p_modelViewMatrix, this->p_projectionMatrix,
			this->pViewFrustrm);
	}
	else//檢測觸摸事件
	{
			float pos_x=-1;
			float pos_y=-1;
		#if USE_ON_BOARD
			COOR_SYS point_data=getClickCoordinate();

				pos_x=point_data.point_x;
				pos_y=point_data.point_y;
		#else
				pos_x=p_Host->GetTouchPosX();
				pos_y=p_Host->GetTouchPosY();


				p_Host->SetTouchPosX(-1);
				p_Host->SetTouchPosY(-1);
		#endif
		if(pos_x>0 ||pos_y>0)
			this->SetEnableDraw(true);
	}
}

void multiLayerButtonGroup::Update_State()
{
	if(m_layeredButtonGroupsVect[currentActiveBGIndex]->Update_State(p_Host))
	{
		this->SetEnableDraw(true);
	}
	m_layeredButtonGroupsVect[currentActiveBGIndex]->HightlightButton(m_layeredButtonGroupsVect[currentActiveBGIndex]->GetHighlightButtonId());
}


void multiLayerButtonGroup::init_button_group(GLShaderManager *shaderManager,GLMatrixStack * modelViewMatrix,
		GLMatrixStack * projectionMtrx, GLFrustum * ViewFrustrm)
{
	p_modelViewMatrix=modelViewMatrix;
	p_projectionMatrix= projectionMtrx;
	pViewFrustrm = ViewFrustrm;
	int data_state[MENU_BUTTON_COUNT]={0,1,0};
	float data_x[MENU_BUTTON_COUNT]={-0.5,0.0,0.5};
	float data_y[MENU_BUTTON_COUNT]={-0.5,0.0,0.5};
	float data_width[MENU_BUTTON_COUNT]={0.1,0.2,0.3};
	float data_height[MENU_BUTTON_COUNT]={0.1,0.2,0.3};
	for(int cxm_i=0;cxm_i<MENU_BUTTON_VER_COUNT;cxm_i++)
	{
		for(int cxm_j=0;cxm_j<MENU_BUTTON_HOR_COUNT;cxm_j++)
		{
			data_state[cxm_j+cxm_i*MENU_BUTTON_HOR_COUNT]=0;
			data_width[cxm_j+cxm_i*MENU_BUTTON_HOR_COUNT]=BUTTON_WIDTH;
			data_height[cxm_j+cxm_i*MENU_BUTTON_HOR_COUNT]=BUTTON_HEIGHT;
			data_x[cxm_j+cxm_i*MENU_BUTTON_HOR_COUNT]=MARGIN_X+cxm_j*(BUTTON_WIDTH+GAP_X);
			data_y[cxm_j+cxm_i*MENU_BUTTON_HOR_COUNT]=MARGIN_Y+cxm_i*(BUTTON_HEIGHT+GAP_Y);

		}
	}

	for(int i = 0; i <m_layeredButtonGroupsVect.size(); i++ ){
		assert(i < MAX_LAYEDED_GROUP_COUNT);
		//先确定这７x4个按钮位置那些是有效的，
		m_layeredButtonGroupsVect[i]->acceptButtonMask(pMasks[i],MaskLengths[i]);
		//初始化按钮组
		m_layeredButtonGroupsVect[i]->init_button_group(MENU_BUTTON_COUNT,vbuttonnormal,vbuttonchoose,
				data_state,data_x,data_y,	data_width,data_height,shaderManager);
	}
}

void multiLayerButtonGroup::SetEnableDraw(bool enable)
{
	enable_draw=enable;
	if(enable)
		counter=AUTO_HIDE_COUNT;
	else
		counter=0;
}
