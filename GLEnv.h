#ifndef _GLENV_
#define  _GLENV_
#include <GLMatrixStack.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
class GLEnv
{
public:
	GLMatrixStack *GetmodelViewMatrix();
	GLMatrixStack	*GetprojectionMatrix();
	GLGeometryTransform *GettransformPipeline();
	GLFrustum *GetviewFrustum();
private:
	GLMatrixStack	modelViewMatrix;
	GLMatrixStack	projectionMatrix;
	GLGeometryTransform transformPipeline;
	GLFrustum viewFrustum;
};
#endif
