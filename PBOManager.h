/*
 * PBOManager.h
 *
 *  Created on: Nov 10, 2016
 *      Author: hoover
 */

#ifndef PBOMANAGER_H_
#define PBOMANAGER_H_

#include "GL/gl.h"
#include "StlGlDefines.h"
#include "stdio.h"
#include "FBOManager.h"

typedef void(*PFN_PBOFILLBUFFER)(GLubyte *,int);
typedef struct _OSA_SemHndl OSA_SemHndl,* pOSA_SemHndl;
class PBOBase
{
public:
	PBOBase(unsigned int PBOchcnt=CAM_COUNT, unsigned int w = DEFAULT_IMAGE_WIDTH, unsigned int h=DEFAULT_IMAGE_HEIGHT, unsigned int cc=4,GLenum format = GL_BGRA);
	virtual ~PBOBase()=0;
protected:
	unsigned int PBOChannelCount;// stitching cam + one individual video
	unsigned int PBOBufferCount;
	unsigned int width, height, chncnt;
	GLenum pixel_format;
	unsigned int    dataSize;
	unsigned int* pIndex;						//latest pbo index
	int pboMode;							//single/double buffer mode
	GLuint *pboIds;                   // IDs of PBO
	bool bUsePBO;
	 int nowPboId;
};

class PBOSender:public PBOBase
{
public:
	PBOSender(unsigned int PBOchcnt=CAM_COUNT, unsigned int w = DEFAULT_IMAGE_WIDTH, unsigned int h=DEFAULT_IMAGE_HEIGHT, unsigned int cc=4,GLenum format = GL_BGRA);
	~PBOSender();
	bool Init();
	void sendData(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx, bool bPBO=true);
private:
	void sendDataPBO(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx);
	void sendDataNoPBO(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx);
};

class PBOReceiver :public PBOBase,
										 public InterfacepboDrawCB
{
public:
	PBOReceiver(unsigned int PBOchcnt=2, unsigned int w = DEFAULT_IMAGE_WIDTH, unsigned int h=DEFAULT_IMAGE_HEIGHT, unsigned int cc=4,GLenum format = GL_BGRA);
	~PBOReceiver();
	bool Init();
	inline int getCurrentPBOIdx(){return nowPboId;};
	GLubyte** getPixelBuffer(int idx){return &pPixelBuffer[idx];};
    virtual void callbackPBODraw();
    pOSA_SemHndl getSemPBO(){return pSemPBO;};
private:
    pOSA_SemHndl pSemPBO;
	void getDataPBO(int startX,int startY,int w,int h,GLuint idx=0);
	void delete_POBReceiver();
	void getData(int startX=0,int startY=0,int w=CURRENT_SCREEN_WIDTH,int h=CURRENT_SCREEN_HEIGHT,  GLuint idx=0, bool bPBO=true);

	 GLubyte* pPixelBuffer[PBO_ALTERNATE_NUM];
};

#endif /* PBOMANAGER_H_ */
