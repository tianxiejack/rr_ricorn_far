#include"GLEnv.h"

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
