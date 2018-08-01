#pragma once
#include <vector>
#define MENU_BUTTON_HOR_COUNT 10
#define MENU_BUTTON_VER_COUNT 8
#define MENU_BUTTON_COUNT (MENU_BUTTON_HOR_COUNT*MENU_BUTTON_VER_COUNT)
#define MENU_GROUP_COUNT 23
class MenuButton;
class GLShaderManager;
class ButtonGroup;
class GLMatrixStack;
class GLFrustum;

class InterfaceRenderBehavior{
public:
   virtual int GetWindowWidth() = 0;
   virtual int GetWindowHeight()= 0;
   virtual int GetTouchPosX()=0;
   virtual int GetTouchPosY()=0;
   virtual void SetTouchPosX(int x)=0;
   virtual void SetTouchPosY(int x)=0;
   virtual int getGroupMenuIndex() = 0;
   virtual void processKeycode(int keycode)=0;
};

class InterfaceButtonGroup{

public:
	virtual void Group_Draw()=0;
	virtual void Update_State()=0;
	virtual void init_button_group(GLShaderManager * shaderManager,GLMatrixStack * modelViewMatrix,
			GLMatrixStack * projectionMtrx,GLFrustum *     pViewFrustrm)=0;
	virtual void SetEnableDraw(bool enable)=0;
	virtual bool GetEnableDraw()=0;
	virtual void SetcurrentActiveBGIndex(int idx)=0;
	virtual void SetSubmenuKeycode(int keycode)=0;
};



