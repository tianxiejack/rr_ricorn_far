#include"GLEnv.h"
#include"StlGlDefines.h"
GLEnv::GLEnv():
PBOMgr(PBOSender(CAM_COUNT,PANO_TEXTURE_WIDTH,PANO_TEXTURE_HEIGHT,3,GL_BGR)),
FBOmgr(FBOManager(CURRENT_SCREEN_WIDTH,CURRENT_SCREEN_HEIGHT,GL_BGR,GL_RGB8)),
PBORcr(PBOReceiver(PBO_ALTERNATE_NUM,CURRENT_SCREEN_WIDTH,CURRENT_SCREEN_HEIGHT,3,GL_BGR)),
PBOExtMgr(PBOSender(CAM_COUNT+EXT_CAM_COUNT)),
PBOVGAMgr(PBOSender(VGA_CAM_COUNT,VGA_WIDTH,VGA_HEIGHT)),
PBOSDIMgr(PBOSender(SDI_CAM_COUNT,SDI_WIDTH,SDI_HEIGHT))
{

}

GLMatrixStack *GLEnv::GetmodelViewMatrix()
{
	return &modelViewMatrix;
}
GLMatrixStack	*GLEnv::GetprojectionMatrix()
{
	return &projectionMatrix;
}
GLGeometryTransform *GLEnv::GettransformPipeline()
{
	return &transformPipeline;
}
GLFrustum *GLEnv::GetviewFrustum()
{
	return &viewFrustum;
}
GLBatch *GLEnv::GetPanel_Petal(int idx)
{
	return &Panel_Petal[idx];
}
GLBatch *GLEnv::Getp_Panel_Petal_OverLap(int idx)
{
	return Panel_Petal_OverLap[idx];
}
void GLEnv::Setp_Panel_Petal_OverLap(int idx,GLBatch *recv_Panel_OverLap)
{
	Panel_Petal_OverLap[idx]=recv_Panel_OverLap;
}

void GLEnv::Setp_Panel_OverLap(int idx,GLBatch *recv_batch)
{
	 Panel_OverLap[idx]=recv_batch;
}
GLBatch *GLEnv::Getp_Panel_OverLap(int idx)
{
	return Panel_OverLap[idx];
}
GLBatch *GLEnv::Getp_shadowBatch()
{
	return &shadowBatch;
}
PBOReceiver *GLEnv::Getp_PBORcr()
{
	return 	&PBORcr;
}
PBOSender *GLEnv::Getp_PBOMgr()
{
	return 	&	 PBOMgr;
}
PBOSender *GLEnv::Getp_PBOExtMgr()
{
	return 	&	 PBOExtMgr;
}
PBOSender *GLEnv::Getp_PBOVGAMgr()
{
	return 	&	 PBOVGAMgr;
}
PBOSender *GLEnv::Getp_PBOSDIMgr()
{
	return 	&	 PBOSDIMgr;
}
FBOManager *GLEnv::Getp_FBOmgr()
{
	return 	&	 FBOmgr;
}
pPBO_FBO_Facade GLEnv::Getp_FboPboFacade()
{
	return 	mp_FboPboFacade;
}
void GLEnv::Set_FboPboFacade(FBOManager  &FBOMgr,PBOReceiver  &PBORcr)
{
	mp_FboPboFacade=new PBO_FBO_Facade(FBOMgr,PBORcr);
}

