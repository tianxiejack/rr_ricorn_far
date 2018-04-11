#ifndef _GLENV_
#define  _GLENV_
#include <GLMatrixStack.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include"StlGlDefines.h"
class GLEnv
{
public:
	GLMatrixStack *GetmodelViewMatrix();
	GLMatrixStack	*GetprojectionMatrix();
	GLGeometryTransform *GettransformPipeline();
	GLFrustum *GetviewFrustum();
	GLBatch *GetPanel_Petal(int idx);
	GLBatch *Getp_Panel_Petal_OverLap(int idx);
	void Setp_Panel_Petal_OverLap(int idx,GLBatch *recv_Panel_OverLap);
private:
	GLMatrixStack	modelViewMatrix;
	GLMatrixStack	projectionMatrix;
	GLGeometryTransform transformPipeline;
	GLFrustum viewFrustum;
	GLBatch Panel_Petal[CAM_COUNT];
	GLBatch *Panel_Petal_OverLap[CAM_COUNT];
};
#endif
