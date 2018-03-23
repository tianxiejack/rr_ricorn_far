#ifndef _FBOMANAGER_H
#define _FBOMANAGER_H
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
#include "RenderDrawBehaviour.h"

class InterfacepboDrawCB{
	public:
		virtual void callbackPBODraw() = 0;
};

class FBOManager
{
	public:
	FBOManager(int TextureW,int TextureH);
	~FBOManager();
	bool Init();
	void PboDraw(InterfacepboDrawCB & icb);
	void SetDrawBehaviour(p_InterFaceDrawBehaviour DrawBehaviour);
	bool IsFboUsed()
	{
		return fboUsed;
	}
	private:
	void deleteFBOs(){
		    if(fboSupported)
		    {
		        glDeleteFramebuffers(1, &fboId);
		        fboId = 0;
		        glDeleteRenderbuffers(1, &rboDepthId);
		        rboDepthId = 0;
		    }};
	void DrawFrame();
	void InToFrameBuffer();
	void OutOfFrameBuffer();
	bool checkFramebufferStatus(GLuint fbo);
	void printFramebufferInfo(GLuint fbo);
	std::string convertInternalFormatToString(GLenum format);
	std::string getTextureParameters(GLuint id);
	std::string getRenderbufferParameters(GLuint id);
	p_InterFaceDrawBehaviour mp_DrawBehaviour;
	bool fboUsed;
	bool fboSupported;
	int TextureWidth,TextureHeight;
	GLuint rboColorId, rboDepthId;
	GLuint fboId,textureId;
};
#endif
