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

typedef void(*PFN_PBOFILLBUFFER)(GLubyte *,int,int);
typedef struct _OSA_SemHndl OSA_SemHndl,* pOSA_SemHndl;
class PBOBase
{
public:
		PBOBase(unsigned int PBOchcnt=CAM_COUNT, unsigned int w = DEFAULT_IMAGE_WIDTH, unsigned int h=DEFAULT_IMAGE_HEIGHT, unsigned int cc=3,GLenum format = GL_BGR
			,unsigned int pbo_mode=2,bool buse_pbo=true);
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
	PBOSender(unsigned int PBOchcnt=CAM_COUNT, unsigned int w = PANO_TEXTURE_WIDTH, unsigned int h=PANO_TEXTURE_HEIGHT, unsigned int cc=3,GLenum format = GL_BGR);
	~PBOSender();
	bool Init();
	void sendData(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx,int  mainOrsub=MAIN,bool bPBO=true);
private:
	void sendDataPBO(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx,int mainOrsub=MAIN);
	void sendDataNoPBO(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx,int mainOrsub=MAIN);
};

class PBOReceiver :public PBOBase,
										 public InterfacepboDrawCB
{
public:
	PBOReceiver(unsigned int PBOchcnt=2, unsigned int w = MAX_SCREEN_WIDTH, unsigned int h=MAX_SCREEN_HEIGHT, unsigned int cc=3,GLenum format = GL_BGR);
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
