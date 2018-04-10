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
