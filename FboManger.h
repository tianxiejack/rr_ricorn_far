#ifndef _FBOMANGER_H
#define _FBOMANGER_H
#include "StlGlDefines.h"
#include <GL/glew.h> // glUniformxxx()
#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.


#include <stdio.h>
#include <time.h>       /* For our FPS */
#include <string.h>    /* for strcpy and relatives */
#include <unistd.h>     /* needed to sleep*/

#include <math.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <GLBatch.h>
#include <GLShaderManager.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include "GLMatrixStack.h"
class FboManger
{
	public:
	//FboManger(GLMatrixStack &modelViewMat,GLMatrixStack	&projectionMat,GLShaderManager* mgr=NULL);
	FboManger(int TextureW,int TextureH,GLuint rboCid,GLuint rboDid,
			GLuint fboid,
			GLMatrixStack &modelViewMat,
			GLMatrixStack	&projectionMat,
			GLShaderManager* mgr=NULL);
	~FboManger();
	void deleteFBOs(){  // clean up FBO, RBO
	    if(fboSupported)
	    {
	        glDeleteFramebuffers(1, &fboId);
	        fboId = 0;
	        glDeleteRenderbuffers(1, &rboDepthId);
	        rboDepthId = 0;
	    }};
	void updateFBOs();
	bool IsInitOK();
	void flipFBOBit(){
		if(fboUsed)
		{
			fboUsed=false;
		}
		else
	{
			fboUsed=true;
	}};
	void  RenderDirectlyToTexture();
	bool checkFramebufferStatus(GLuint fbo);
	void printFramebufferInfo(GLuint fbo);
	std::string convertInternalFormatToString(GLenum format);
	std::string getTextureParameters(GLuint id);
	std::string getRenderbufferParameters(GLuint id);
	void InitScreenQuad_Matrix(int imageWidth,int imageHeight);
	void SetupProg();
	void ChangeSize(int nWidth, int nHeight);
	void BindOwntextureId(GLuint TEXID){textureId=TEXID;};
	GLuint fboId,textureId;
	bool fboUsed;
	private:
	bool fboSupported;
	int TextureWidth,TextureHeight;
	GLuint rboColorId, rboDepthId;
	GLBatch             screenQuad;
	GLuint oitResolve;
	GLuint msResolve;
	M3DMatrix44f        orthoMatrix;
	GLShaderManager * 	m_pShaderManager;
	GLMatrixStack &		modelViewMatrix;
	GLMatrixStack &		projectionMatrix;
};



#endif
