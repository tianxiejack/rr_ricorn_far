/*
 * CaptureGroup.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: wang
 */

#include "CaptureGroup.h"
#include <stdbool.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include "buffer.h"
#include "updatebuffer.h"

#if USE_12
#include"HDV4lcap.h"
#endif
using namespace std;



//static HDv4l_cam v4lcap(0,SDI_WIDTH,SDI_HEIGHT);
static HDv4l_cam v4lcap1(1,SDI_WIDTH,SDI_HEIGHT);
static HDv4l_cam v4lcap2(2,SDI_WIDTH,SDI_HEIGHT);
static HDv4l_cam v4lcap3(3,SDI_WIDTH,SDI_HEIGHT);
static HDv4l_cam v4lcap4(4,SDI_WIDTH,SDI_HEIGHT);
static HDv4l_cam v4lcap5(5,SDI_WIDTH,SDI_HEIGHT);


static MainExtCaptureGroup m_MainExtGroup(SDI_WIDTH,SDI_HEIGHT,3,MAIN_EXT_COUNT);
static SubExtCaptureGroup m_SubExtGroup(SDI_WIDTH,SDI_HEIGHT,3,SUB_EXT_COUNT);




static ExtCaptureGroup m_ExtGroup(SDI_WIDTH,SDI_HEIGHT,3,EXT_CAM_COUNT);

static IconCaptureGroup m_IconGroup(SDI_WIDTH,SDI_HEIGHT,3,ICON_COUNT);
static Ruler45CaptureGroup m_Ruler45Group(SDI_WIDTH,SDI_HEIGHT,3,ICON_COUNT);
static Ruler90CaptureGroup m_Ruler90Group(SDI_WIDTH,SDI_HEIGHT,3,ICON_COUNT);
static Ruler180CaptureGroup m_Ruler180Group(SDI_WIDTH,SDI_HEIGHT,3,ICON_COUNT);




CaptureGroup::CaptureGroup(unsigned int capCount):m_TotalCamCount(capCount),m_currentIdx(0),
		width(SDI_WIDTH),height(SDI_HEIGHT),depth(DEFAULT_IMAGE_DEPTH),Defaultimage(NULL)
{
	capCamera = new pInterface_VCap[m_TotalCamCount];
	for(int i = 0; i < capCount; i ++){
		capCamera[i] = new PseudoVCap();
	}
	//memset((void*)m_V4LImag,0,sizeof(m_V4LImag));
	Defaultimage = new unsigned char [capCount*width* height * depth];
	fillColorBar();
}
CaptureGroup::CaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount):
		m_TotalCamCount(capCount),m_currentIdx(0),width(wide),height(height),
		depth(NCHAN),Defaultimage(NULL)
{
	capCamera = new pInterface_VCap[m_TotalCamCount];
	for(int i = 0; i < capCount; i ++){
		capCamera[i] = new PseudoVCap();
	}
	//memset((void*)m_V4LImag,0,sizeof(m_V4LImag));
	Defaultimage = new unsigned char [capCount*width* height * depth];
	fillColorBar();
}

void CaptureGroup::init()
{
	CreateProducers();
	SetConsumers(GetConsumers());
	OpenProducers();
	Open();
}

CaptureGroup::~CaptureGroup()
{
	Close();
	for(int i=0; i < m_TotalCamCount; i ++){
		BaseVCap *pCamera = dynamic_cast<BaseVCap*>(capCamera[i]);
		if(pCamera){
			delete pCamera;
		}
		//delete update[i];
	}

	delete [] capCamera;
	delete [] Defaultimage;
}

void CaptureGroup::SetConsumers(vector<Consumer>  h)
{
     int size = h.size();
     for(int i = 0; i < size; i++){
    //re-oder the cameras according to idx
    	 for(int j = 0; j<size;j++){
    			 if(h[j].idx == i){
    				 Append(h[j].pcap);
    			 }
    	 }
     }
}


void CaptureGroup::fillColorBar()
{
	float factor = 18.45f;
	for(int i=0; i<m_TotalCamCount; i++)
	{
		char *pBuff = (char*)(& Defaultimage[i* width * height* depth]);
		unsigned int offset = (i+1)*width*height;
		for(int j = 0; j < width*height; j++)
		{
			int real = (int)factor;
			*pBuff++ = (int)(((j +offset%255)*(real*real)))&0xFF;
			*(pBuff++) = (int)(((j+offset%255)*real))&0xFF;
			*(pBuff++) = (int)(((m_TotalCamCount-1-i)*(j+offset%255)))&0xFF;//init a bluer stripe texture in the buffers.
			factor *= 3.30f;
			offset = (int)(offset*real)&0xff;
		}
	}
}

bool CaptureGroup:: Append(pInterface_VCap cap)
{
	if(m_currentIdx < m_TotalCamCount ){
		char * pImg = (char*)&Defaultimage[m_currentIdx * width * height* depth];
		// delete the pseudoCamera before assigning new one.
		delete capCamera[m_currentIdx];
		capCamera[m_currentIdx] = cap;
		cap->SetDefaultImg(pImg);
		m_currentIdx++;
		return true;
	}
	return false;
}
void CaptureGroup::Open()
{
	for(int i = 0; i < m_currentIdx; i ++){
		if(!capCamera[i]->Open()){
			cout<<"Error: fail to open camera["<<i<<"]"<<endl;
		}
	}
}

void CaptureGroup::Close()
{
	for(int i=0; i<m_currentIdx; i++)
	{
		capCamera[i]->Close();
	}
}

void CaptureGroup::captureCam(GLubyte *ptr, int index,int mainOrsub)
{
	capCamera[index]->Capture((char*)ptr,mainOrsub);
}
void CaptureGroup::captureCamFish(GLubyte *ptr, int index)
{
	capCamera[index]->CaptureFish((char*)ptr);
}
bool CaptureGroup::getScannerPic(IplImage *pImage)
{
	if(capCamera[0])
	{
		capCamera[0]->Capture(pImage->imageData);
		return true;
	}
	return false;
}

bool CaptureGroup::saveCapImg()
{
	char buf[32];
	for(int i=0; i<CAM_COUNT; i++)
	{
		sprintf(buf,"./data/%02d.bmp",i);
		if(capCamera[i])
			capCamera[i]->SavePic(buf);
		else
			printf("cap:%d error\n",i);
	}
	return true;
}

bool CaptureGroup::saveExposureCompensationCapImg()
{
	char buf[32];
	for(int i=0; i<CAM_COUNT; i++)
	{
		sprintf(buf,"./data/gain/%02d.bmp",i);
		if(capCamera[i])
			capCamera[i]->SavePic(buf);
		else
			printf("cap:%d error\n",i);
	}
	return true;
}



void CaptureGroup::saveOverLap()
{
	for(int i=0; i<CAM_COUNT; i++)
	{
		if(capCamera[i])
			capCamera[i]->saveOverLap();
		else
			printf("cap:%d error\n",i);
	}
}

CaptureGroup* CaptureGroup::GetPanoCaptureGroup()
{
	HDv4l_cam * pv4lcap[MAX_CC]={NULL,&v4lcap1,&v4lcap2,&v4lcap3,&v4lcap4,&v4lcap5};
	static bool Once = false;
	if(!Once){
		Once = true;
		char fileName[64];
		int i = 0;
#if USE_COMPASS_ICON
		CaptureGroup::GetIconCaptureGroup();
#endif
		CaptureGroup::GetRuler45CaptureGroup();

#if  !USE_BMPCAP
		for(int i=0; i < 2; i++){
			sprintf(fileName,"./data/pic%d.bmp",i);
				m_PanoGroup.Append(new BMPVcap(fileName));
		}
#else

		dev_id=FPGA_FOUR_CN;
		m_PanoGroup.Append(new HDVCap(dev_id,SDI_WIDTH,SDI_HEIGHT));
		dev_id=FPGA_SIX_CN;
		m_PanoGroup.Append(new HDVCap(dev_id,SDI_WIDTH,SDI_HEIGHT));

		dev_id=FPGA_FOUR_CN;
		pHDAsyncVCap[dev_id]->Open();
		dev_id=FPGA_SIX_CN;
		pHDAsyncVCap[dev_id]->Open();

#endif
		m_PanoGroup.Open();
	}
	return &m_PanoGroup;
}
CaptureGroup* CaptureGroup:: GetMainExtCaptureGroup()
{
		static bool once=false;
		if(!once){
			char filename[64];
			once = true;
	#if !USE_BMPCAP
			sprintf(filename,"./data/pic0.bmp");
			for(int i=0;i<MAIN_EXT_COUNT;i++)
			{
				m_MainExtGroup.Append(new BMPVcap(filename));
			}
	#else
				HDv4l_cam * pv4lcap[MAX_CC]={NULL,&v4lcap1,&v4lcap2,&v4lcap3,&v4lcap4,&v4lcap5};
				int dev_id=MAIN_CN;
				if(pHDAsyncVCap[dev_id]==NULL)
					pHDAsyncVCap[dev_id] = new HDAsyncVCap4(auto_ptr<BaseVCap>(pv4lcap[dev_id]),dev_id);
			m_MainExtGroup.Append(new HDVCap(dev_id,SDI_WIDTH,SDI_HEIGHT));
			pHDAsyncVCap[dev_id]->Open();
	#endif
			m_MainExtGroup.Open();
		}
		return &m_MainExtGroup;
}

CaptureGroup* CaptureGroup:: GetSubExtCaptureGroup()
{
		static bool once=false;
		if(!once){
			char filename[64];
			once = true;
	#if !USE_BMPCAP
			sprintf(filename,"./data/pic2.bmp");
			for(int i=0;i<MAIN_EXT_COUNT;i++)
			{
				m_SubExtGroup.Append(new BMPVcap(filename));
			}
	#else
				HDv4l_cam * pv4lcap[MAX_CC]={NULL,&v4lcap1,&v4lcap2,&v4lcap3,&v4lcap4,&v4lcap5};
				int dev_id=SUB_CN;
				if(pHDAsyncVCap[dev_id]==NULL)
					pHDAsyncVCap[dev_id] = new HDAsyncVCap4(auto_ptr<BaseVCap>(pv4lcap[dev_id]),dev_id);
				m_SubExtGroup.Append(new HDVCap(dev_id,SDI_WIDTH,SDI_HEIGHT));
				pHDAsyncVCap[dev_id]->Open();
	#endif
			m_SubExtGroup.Open();
		}
		return &m_SubExtGroup;
}





CaptureGroup* CaptureGroup::GetExtCaptureGroup()
{
/*	static bool once=false;
	if(!once){
		char filename[64];
		once = true;
#if USE_BMPCAP
		sprintf(filename,"./data/other.bmp");
		for(int i=0;i<EXT_CAM_COUNT;i++)
		{
			m_ExtGroup.Append(new BMPVcap(filename));
		}
#else

		m_ExtGroup.Append(new SmallVCap(CAM_COUNT));
		m_ExtGroup.Append(new SmallVCap(CAM_COUNT+1));

		if(pAsyncVCap44==NULL)
						pAsyncVCap44 = new AsyncVCap4(auto_ptr<BaseVCap>(&v4lcap4));
		pAsyncVCap44->Open();
#endif
		m_ExtGroup.Open();

	}
	return &m_ExtGroup;*/
}

#if USE_12
/*
CaptureGroup* CaptureGroup::GetVGACaptureGroup()
{
	static bool once=false;
	if(!once){
		char filename[64];
		once = true;
#if USE_BMPCAP
		sprintf(filename,"./data/other.bmp");
		m_VGAGroup.Append(new BMPVcap(filename));
#else
		m_VGAGroup.Append(new HDVCap(VGA_DEV_NUM,VGA_WIDTH,VGA_HEIGHT));
		if(pHDAsyncVCap45==NULL)
		{
			pHDAsyncVCap45= new HDAsyncVCap4(auto_ptr<BaseVCap>(&VGA_cap),VGA_DEV_NUM);
		}

		pHDAsyncVCap45->Open();
		#endif
		m_VGAGroup.Open();
	}
	return &m_VGAGroup;
}

CaptureGroup* CaptureGroup::GetSDICaptureGroup()
{
	static bool once=false;
	if(!once){
		char filename[64];
		once = true;
#if USE_BMPCAP
		sprintf(filename,"./data/other.bmp");
		m_SDIGroup.Append(new BMPVcap(filename));
#else
		m_SDIGroup.Append(new HDVCap(SDI_DEV_NUM,SDI_WIDTH,SDI_HEIGHT));
		if(pHDAsyncVCap44==NULL)
				{
					pHDAsyncVCap44 = new HDAsyncVCap4(auto_ptr<BaseVCap>(&SDI_cap),SDI_DEV_NUM);
				}

		pHDAsyncVCap44->Open();
#endif
	 	m_SDIGroup.Open();
	}
	return &m_SDIGroup;
}
*/


#endif


CaptureGroup* CaptureGroup::GetIconCaptureGroup()
{
	static bool once=false;
	if(!once){

		char filename[64];
		once = true;
#if USE_COMPASS_ICON
		sprintf(filename,"./data/anglecompass.bmp");
		m_IconGroup.Append(new BMPVcap(filename));

		m_IconGroup.Open();
#endif
	}
	return &m_IconGroup;
}

CaptureGroup* CaptureGroup::GetRuler45CaptureGroup()
{
	static bool once=false;
	if(!once){
		char filename[64];
		once = true;

		sprintf(filename,"./data/45.bmp");
		m_Ruler45Group.Append(new BMPVcap(filename));

		m_Ruler45Group.Open();

	}
	return &m_Ruler45Group;
}

CaptureGroup* CaptureGroup::GetRuler90CaptureGroup()
{
	static bool once=false;
	if(!once){
		char filename[64];
		once = true;

		sprintf(filename,"./data/90.bmp");
		m_Ruler90Group.Append(new BMPVcap(filename));

		m_Ruler90Group.Open();

	}
	return &m_Ruler90Group;
}

CaptureGroup* CaptureGroup::GetRuler180CaptureGroup()
{
	static bool once=false;
	if(!once){
		char filename[64];
		once = true;

		sprintf(filename,"./data/180.bmp");
		m_Ruler180Group.Append(new BMPVcap(filename));

		m_Ruler180Group.Open();

	}
	return &m_Ruler180Group;
}

bool CaptureGroup::saveSingleCapImg(int cam_num)
{
	char buf[32];
	int i=0;
	i=cam_num;
		sprintf(buf,"./data/%02d.bmp",i);
		if(capCamera[i])
			capCamera[i]->SavePic(buf);
		else
			printf("cap:%d error\n",i);

	return true;
}






