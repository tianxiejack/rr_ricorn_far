#ifndef _GLENV_H_
#define  _GLENV_H_
#include <GLMatrixStack.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include"StlGlDefines.h"
#include"PBOManager.h"
#include"PBO_FBO_Facade.h"
#include"CaptureGroup.h"
class GLEnv
{
public:
	GLEnv(CaptureGroup *p_pano,CaptureGroup *p_chosen);
	~GLEnv(){};
	GLMatrixStack *GetmodelViewMatrix();
	GLMatrixStack	*GetprojectionMatrix();
	GLGeometryTransform *GettransformPipeline();
	GLFrustum *GetviewFrustum();
	GLBatch *GetPanel_Petal(int idx);
	GLBatch *Getp_Panel_Petal_OverLap(int idx);
	void Setp_Panel_Petal_OverLap(int idx,GLBatch *recv_Panel_OverLap);
	GLBatch *Getp_Panel_OverLap(int idx);
	void Setp_Panel_OverLap(int idx,GLBatch *recv_batch);
	GLBatch *Getp_shadowBatch();
	PBOReceiver *Getp_PBORcr();
	PBOSender *Getp_PBOMgr();
	PBOSender *Getp_PBOExtMgr();
	PBOSender *Getp_PBOVGAMgr();
	PBOSender *Getp_PBOSDIMgr();
	FBOManager *Getp_FBOmgr();
	pPBO_FBO_Facade Getp_FboPboFacade();
	void Set_FboPboFacade(FBOManager  &FBOMgr,PBOReceiver  &PBORcr);
	CaptureGroup *GetPanoCaptureGroup(){return m_panoCaptureGroup;};
	CaptureGroup *GetChosenCaptureGroup(){return m_chosenCaptureGroup;};
private:
	CaptureGroup * m_panoCaptureGroup;
	CaptureGroup * m_chosenCaptureGroup;

	GLMatrixStack	modelViewMatrix;
	GLMatrixStack	projectionMatrix;
	GLGeometryTransform transformPipeline;
	GLFrustum viewFrustum;
	GLBatch Panel_Petal[CAM_COUNT];
	GLBatch *Panel_Petal_OverLap[CAM_COUNT];
	GLBatch *Panel_OverLap[CAM_COUNT];
	GLBatch shadowBatch; //the shadow rect under vehicle
	PBOReceiver PBORcr;
	PBOSender PBOMgr;
	PBOSender PBOExtMgr;
	PBOSender PBOVGAMgr;
	PBOSender PBOSDIMgr;
	FBOManager FBOmgr;
	pPBO_FBO_Facade mp_FboPboFacade;
};
#endif
