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
#include"ForeSight.h"
class GLEnv
{
public:
	GLEnv(CaptureGroup *p_pano=NULL,CaptureGroup *p_chosen=NULL,CaptureGroup *p_misc=NULL);
	~GLEnv(){
	delete p_ForeSightFacade;
	delete p_ForeSightFacade2;
	delete p_ForeSightFacade_Track;};
	void init(CaptureGroup *p_pano,CaptureGroup *p_chosen,CaptureGroup *p_mvdetect,CaptureGroup *p_misc);
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
	PBOSender *Getp_PBOChosenMgr();
	FBOManager *Getp_FBOmgr();
	pPBO_FBO_Facade Getp_FboPboFacade();
	void Set_FboPboFacade(FBOManager  &FBOMgr,PBOReceiver  &PBORcr);
	CaptureGroup *GetPanoCaptureGroup(){return m_panoCaptureGroup;};
	CaptureGroup *GetChosenCaptureGroup(){return m_chosenCaptureGroup;};
	CaptureGroup *GetMiscCaptureGroup(){return m_miscCaptureGroup;};
	GLBatch *Getdegreescale45Batch(){return &degreescale45Batch;};
	GLBatch *Getdegreescale90Batch(){return &degreescale90Batch;};
	GLBatch *Getdegreescale180Batch(){return &degreescale180Batch;};

	ForeSightFacade * Getp_ForeSightFacade(){return p_ForeSightFacade;};
		ForeSightFacade * Getp_ForeSightFacade2(){return p_ForeSightFacade2;};
		ForeSightFacade * Getp_ForeSightFacade_Track(){return p_ForeSightFacade_Track;};
void SetForeSightFacade(PInterfaceForeSight p_IF,ForeSightPos &pos ,PInterfaceCamonForeSight  p_cam)
{
	p_ForeSightFacade->SetPInterfaceCamonForeSight(p_cam);
	p_ForeSightFacade->SetPInterfaceForeSight(p_IF);
	p_ForeSightFacade->SetForeSightPos(pos);
};
void SetForeSightFacade2(PInterfaceForeSight p_IF,ForeSightPos &pos ,PInterfaceCamonForeSight  p_cam)
{
	p_ForeSightFacade2->SetForeSightPos(pos);
	p_ForeSightFacade2->SetPInterfaceCamonForeSight(p_cam);
	p_ForeSightFacade2->SetPInterfaceForeSight(p_IF);
};
void SetForeSightFacade_Track(PInterfaceForeSight p_IF,ForeSightPos &pos ,PInterfaceCamonForeSight  p_cam)
{
	p_ForeSightFacade_Track->SetForeSightPos(pos);
	p_ForeSightFacade_Track->SetPInterfaceCamonForeSight(p_cam);
	p_ForeSightFacade_Track->SetPInterfaceForeSight(p_IF);
};

private:
	CaptureGroup * m_panoCaptureGroup;
	CaptureGroup * m_chosenCaptureGroup;
	CaptureGroup * m_miscCaptureGroup;
	CaptureGroup *m_mvdectCaptureGroup;
	GLMatrixStack	modelViewMatrix;
	GLMatrixStack	projectionMatrix;
	GLGeometryTransform transformPipeline;
	GLFrustum viewFrustum;
	GLBatch Panel_Petal[CAM_COUNT];
	GLBatch *Panel_Petal_OverLap[CAM_COUNT];
	GLBatch *Panel_OverLap[CAM_COUNT];
	GLBatch shadowBatch; //the shadow rect under vehicle
	GLBatch degreescale45Batch; //the degree scale (45-0-45)
	GLBatch degreescale90Batch; //the degree scale (90-0-90)
	GLBatch degreescale180Batch; //the degree scale (180-0-180)
	PBOReceiver PBORcr;
	PBOSender PBOMgr;
	PBOSender PBOExtMgr;
	PBOSender PBOVGAMgr;
	PBOSender PBOSDIMgr;
	PBOSender PBOChosenMgr;
	FBOManager FBOmgr;
	pPBO_FBO_Facade mp_FboPboFacade;

	ForeSightFacade * p_ForeSightFacade;
	ForeSightFacade * p_ForeSightFacade2;
	ForeSightFacade * p_ForeSightFacade_Track;
};
#endif
