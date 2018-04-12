/*
 * PBOManager.cpp
 *
 *  Created on: Nov 10, 2016
 *      Author: hoover
 */
#include "GL/glew.h"
#include "GL/glext.h"

#include "glInfo.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GLRender.h"
#include "pboProcessSrcThread.h"
#include <osa_sem.h>
#include "PBOManager.h"
using namespace std;
extern Render render;
PBOBase::PBOBase(unsigned int PBOchcnt, unsigned int w, unsigned int h, unsigned int cc,GLenum format):
		width(w),height(h),chncnt(cc),pboMode(2),pixel_format(format),bUsePBO(true),PBOChannelCount(PBOchcnt)
{
	dataSize=w*h*cc;
	PBOBufferCount = pboMode*PBOChannelCount;
	pboIds = new GLuint[ PBOBufferCount];
	pIndex = new unsigned int[PBOChannelCount];
	for(int i = 0; i <PBOChannelCount; i++ ){
		pIndex[i] = i;
	}
	nowPboId=-1;
}

PBOBase::~PBOBase()
{

}

/***********************PBOSender******************************/

PBOSender::PBOSender(unsigned int PBOchcnt, unsigned int w , unsigned int h, unsigned int cc,GLenum format)
{
	width=w;
	height=h;
	chncnt=cc;
	pboMode=2;
	pixel_format=format;
	bUsePBO=true;
	PBOChannelCount=PBOchcnt;

	dataSize=w*h*cc;
	PBOBufferCount = pboMode*PBOChannelCount;
	pboIds = new GLuint[ PBOBufferCount];
	pIndex = new unsigned int[PBOChannelCount];
	for(int i = 0; i <PBOChannelCount; i++ ){
		pIndex[i] = i;
	}
}

PBOSender::~PBOSender()
{
	glDeleteBuffersARB(PBOBufferCount,pboIds);
	delete [] pboIds;
	delete [] pIndex;
}

bool PBOSender::Init()
{
	bool pboSupported = false;
    // get OpenGL info
    glInfo glInfo;
    glInfo.getInfo();
	 if(glInfo.isExtensionSupported("GL_ARB_pixel_buffer_object"))
	    {
	        pboSupported = true;
	        std::cout << "Video card supports GL_ARB_pixel_buffer_object." << std::endl;
	    }
	    else
	    {
	        pboSupported = false;
	        std::cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << std::endl;
	    }
	 if(pboSupported)
	    {
	        // create 2 pixel buffer objects, you need to delete them when program exits.
	        // glBufferDataARB with NULL pointer reserves only memory space.
	        glGenBuffersARB(PBOBufferCount, pboIds);
		for(int i = 0; i < PBOBufferCount; i++){
	        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[i]);
	        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
		}
	        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	    }
	 return pboSupported;
}

void PBOSender::sendDataNoPBO(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx)
{
       static GLubyte buffer[DEFAULT_IMAGE_WIDTH*DEFAULT_IMAGE_HEIGHT*DEFAULT_IMAGE_DEPTH];
	   if(fxn){
		(*fxn)(buffer,idx);
	   	}
	glBindTexture(GL_TEXTURE_2D, textureId);
       glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, pixel_format, GL_UNSIGNED_BYTE, buffer);
}

void PBOSender::sendDataPBO(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx)
{
	int index = 0, nextIndex = 0;                  // pbo index used for next frame
	idx = idx %PBOChannelCount;
	GLenum error= 0;
	// "index" is used to copy pixels from a PBO to a texture object
	// "nextIndex" is used to update pixels in a PBO
	if(pboMode == 1)
	{
		// In single PBO mode, the index and nextIndex are set to 0
		index = nextIndex = pIndex[idx];
	}
	else if(pboMode == 2)
	{
		// In dual PBO mode, increment current index first then get the next index
		index = pIndex[idx] = (pIndex[idx]+PBOChannelCount) % PBOBufferCount;
		nextIndex = (index + PBOChannelCount) % PBOBufferCount;
	}
	// bind the texture and PBO
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[index]);
	error =glGetError();
			if(GL_NO_ERROR != error){
				cout<<"0 GLError = "<<gluErrorString(error)<<endl;
			}
	// copy pixels from PBO to texture object
	// Use offset instead of pointer.

#if WHOLE_PIC
if(idx==0)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width*3/5, height, pixel_format, GL_UNSIGNED_BYTE, 0);
else if(idx==1)
	glTexSubImage2D(GL_TEXTURE_2D, 0, width*3/5, 0, width*2/5, height, pixel_format, GL_UNSIGNED_BYTE, 0);
#else
if(idx==0)
glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, pixel_format, GL_UNSIGNED_BYTE, 0);
else if(idx==1)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1280, 1080, pixel_format, GL_UNSIGNED_BYTE, 0);

#endif
	// bind PBO to update pixel values
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[nextIndex]);
	error =glGetError();
			if(GL_NO_ERROR != error){
				cout<<"1 GLError = "<<gluErrorString(error)<<endl;
			}
	// map the buffer object into client's memory
	// Note that glMapBufferARB() causes sync issue.
	// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
	// for GPU to finish its job. To avoid waiting (stall), you can call
	// first glBufferDataARB() with NULL pointer before glMapBufferARB().
	// If you do that, the previous data in PBO will be discarded and
	// glMapBufferARB() returns a new allocated pointer immediately
	// even if GPU is still working with the previous data.
	GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	if(ptr && fxn)
	{
		// update data directly on the mapped buffer
		(*fxn)(ptr,idx);
// let OpenGL release it
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
		error =glGetError();
		if(GL_NO_ERROR != error){
			cout<<"3 GLError = "<<gluErrorString(error)<<endl;
		}
	}
	else
	{
		cout<<"PBOSender Err! ptr="<<(ptr!=NULL)<<", fxn="<<(fxn!=NULL)<<endl;
	}

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[nextIndex]);
	error =glGetError();
			if(GL_NO_ERROR != error){
				cout<<"4 GLError = "<<gluErrorString(error)<<endl;
			}
	// copy pixels from PBO to texture object
	// Use offset instead of pointer.
if(idx==0)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width*3/5, height, pixel_format, GL_UNSIGNED_BYTE, 0);
else if(idx==1)
	glTexSubImage2D(GL_TEXTURE_2D, 0, width*3/5, 0, width*2/5, height, pixel_format, GL_UNSIGNED_BYTE, 0);
	
    // it is good idea to release PBOs with ID 0 after use.
    // Once bound with 0, all pixel operations behave normal ways.
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    error =glGetError();
    if(GL_NO_ERROR != error){
    	cout<<" 5 GLError = "<<gluErrorString(error)<<endl;
    }
    glFlush();
}

void PBOSender::sendData(GLuint textureId, PFN_PBOFILLBUFFER fxn, GLuint idx, bool bPBO)
{
	if(bPBO&&bUsePBO)
             sendDataPBO(textureId, fxn, idx);
	else
		sendDataNoPBO(textureId, fxn, idx);
}

/***********************PBOReceiver******************************/

PBOReceiver::PBOReceiver(unsigned int PBOchcnt, unsigned int w, unsigned int h, unsigned int cc,GLenum format)
{
	width=w;
	height=h;
	chncnt=cc;
	pboMode=2;
	pixel_format=format;
	bUsePBO=true;
	PBOChannelCount=PBOchcnt;

	dataSize=w*h*cc;
	PBOBufferCount = pboMode*PBOChannelCount;
	pboIds = new GLuint[ PBOBufferCount];
	pIndex = new unsigned int[PBOChannelCount];
	for(int i = 0; i <PBOChannelCount; i++ ){
		pIndex[i] = i;
	}
	for(int i=0;i<2;i++)
		pPixelBuffer[i]=(unsigned char *)malloc(dataSize);
	pSemPBO=(OSA_SemHndl *)malloc(sizeof(OSA_SemHndl)) ;
}
PBOReceiver::~PBOReceiver()
{
	glDeleteBuffersARB(PBOBufferCount,pboIds);
	delete [] pboIds;
	delete [] pIndex;
	free (pSemPBO);
}

bool PBOReceiver::Init()
{
	int ret=OSA_semCreate(pSemPBO,1,0);
	if(ret<0)
	{
		printf("OSA_semCreate failed\n");
	}

	bool pboSupported = false;
    // get OpenGL info
    glInfo glInfo;
    glInfo.getInfo();
	 if(glInfo.isExtensionSupported("GL_ARB_pixel_buffer_object"))
	    {
	        pboSupported = true;
	        std::cout << "Video card supports GL_ARB_pixel_buffer_object." << std::endl;
	    }
	    else
	    {
	        pboSupported = false;
	        std::cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << std::endl;
	    }
	 if(pboSupported)
	    {
	        // create 2 pixel buffer objects, you need to delete them when program exits.
	        // glBufferDataARB with NULL pointer reserves only memory space.
	        glGenBuffersARB(PBOBufferCount, pboIds);
	       	 {
	       			for(int i = 0; i < PBOBufferCount; i++){
	       			        glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, pboIds[i]);
	       			        glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, dataSize, 0, GL_STREAM_READ_ARB);
	       				}
	       	 }
	       			        glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
	    }
	 return pboSupported;
}

void PBOReceiver::getDataPBO(int startX,int startY,int w,int h, GLuint idx)
{
	static timeval startT[20]={0};
	int t[10]={0};
	gettimeofday(&startT[0],0);

	int index = 0, nextIndex = 0;                  // pbo index used for next frame
	idx = idx %PBOChannelCount;

	GLenum error= 0;
	// "index" is used to copy pixels from a PBO to a texture object
	// "nextIndex" is used to update pixels in a PBO
	if(pboMode == 1)
	{
		// In single PBO mode, the index and nextIndex are set to 0
		index = nextIndex = pIndex[idx];
	}
	else if(pboMode == 2)
	{
		// In dual PBO mode, increment current index first then get the next index
		index = pIndex[idx] = (pIndex[idx]+PBOChannelCount) % PBOBufferCount;
		nextIndex = (index + PBOChannelCount) % PBOBufferCount;
	}


//read data from FBO to PBO
//glReadPixels() will return immediately
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, pboIds[index]);
	gettimeofday(&startT[4],0);
	glReadPixels(startX,startY,w,h,GL_BGRA,GL_UNSIGNED_BYTE,0);
	gettimeofday(&startT[5],0);
	error =glGetError();
	if(GL_NO_ERROR != error){
		cout<<"0 GLError = "<<gluErrorString(error)<<endl;
	}

	t[2]=(startT[5].tv_sec-startT[4].tv_sec)*1000000+(startT[5].tv_usec-startT[4].tv_usec);
//	printf("deltatimet[5]-t[4] =%d us\n",t[2]);


	// copy pixels from PBO to texture object
	// Use offset instead of pointer.
	// bind PBO to update pixel values
	gettimeofday(&startT[6],0);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, pboIds[nextIndex]);
	error =glGetError();
	if(GL_NO_ERROR != error){
		cout<<"1 GLError = "<<gluErrorString(error)<<endl;
	}
	gettimeofday(&startT[7],0);
	t[3]=(startT[7].tv_sec-startT[6].tv_sec)*1000000+(startT[7].tv_usec-startT[6].tv_usec);
	// map the buffer object into client's memory
	// Note that glMapBufferARB() causes sync issue.
	// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
	// for GPU to finish its job. To avoid waiting (stall), you can call
	// first glBufferDataARB() with NULL pointer before glMapBufferARB().
	// If you do that, the previous data in PBO will be discarded and
	// glMapBufferARB() returns a new allocated pointer immediately
	// even if GPU is still working with the previous data.
	glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, dataSize, NULL, /*GL_DYNAMIC_DRAW_ARB*/GL_STREAM_READ_ARB);
	error =glGetError();
	if(GL_NO_ERROR != error){
			cout<<"2 GLError = "<<gluErrorString(error)<<endl;
		}
	pPixelBuffer[nextIndex] = (GLubyte*)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
	nowPboId=nextIndex;
#if 0
			int processId=render.GetPBORcr()->getCurrentPBOIdx();
			Mat testData(CURRENT_SCREEN_HEIGHT, CURRENT_SCREEN_WIDTH, CV_8UC4);
			static int a=0;
			a++;
			if(a==50)
			{
				memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
				imwrite("./data/111TEST_PBO.bmp",testData);
			}
			if(a==100)
			{
				memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
				imwrite("./data/100TEST_PBO.bmp",testData);
			}
			if(a==150)
			{
				memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
				imwrite("./data/150TEST_PBO.bmp",testData);
			}
			if(a==200)
			{
				memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
				imwrite("./data/200TEST_PBO.bmp",testData);
			}
			if(a==250)
			{
				memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
				imwrite("./data/250TEST_PBO.bmp",testData);
			}
			if(a==300)
			{
				memcpy(testData.data, (char *)*render.GetPBORcr()->getPixelBuffer(processId),CURRENT_SCREEN_HEIGHT*CURRENT_SCREEN_WIDTH*4);
				imwrite("./data/300TEST_PBO.bmp",testData);
			}

#endif

			if(pPixelBuffer[nextIndex])
	{
				OSA_semSignal(pSemPBO);
		static bool once=true;
		if(once)
		{
			start_pbo_process_thread();
			once=false;
		}
// let OpenGL release it
		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB); // release pointer to mapping buffer
		error =glGetError();
		if(GL_NO_ERROR != error){
			cout<<"3 GLError = "<<gluErrorString(error)<<endl;
		}
	}
	else
	{
		cout<<"PBOSender Err!"<<endl;
	}
	// copy pixels from PBO to texture object
	// Use offset instead of pointer.
    // it is good idea to release PBOs with ID 0 after use.
    // Once bound with 0, all pixel operations behave normal ways.
    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
    error =glGetError();
    if(GL_NO_ERROR != error){
    	cout<<" 5 GLError = "<<gluErrorString(error)<<endl;
    }
}

void PBOReceiver::getData(int startX,int startY,int w,int h,GLuint idx, bool bPBO)
{
	if(bPBO&&bUsePBO)
		getDataPBO(startX,startY, w,h,idx);
	else
	{

	}
}

void PBOReceiver::callbackPBODraw()
{
      getData();
}
