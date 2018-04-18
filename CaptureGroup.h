/*
 * CaptureGroup.h
 *
 *  Created on: Dec 16, 2016
 *      Author: wang
 */

#ifndef CAPTUREGROUP_H_
#define CAPTUREGROUP_H_

#include "StlGlDefines.h"
#include "Camera.h"
#include "v4l2camera.h"
#include"HDV4lcap.h"
#include <vector>
using namespace cv;
using namespace std;
typedef struct _Consumer
{
	int idx;
	BaseVCap * pcap;
}Consumer;

class interfaceCapGroup
{
public:
	virtual void CreateProducers()=0;
	virtual void OpenProducers()=0;
	virtual vector<Consumer>  GetConsumers()=0;
};

class CaptureGroup:public interfaceCapGroup
{
public:
	CaptureGroup(unsigned int capCount);
	CaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);

	virtual ~CaptureGroup();

	void SetConsumers(vector<Consumer> h);
	void init();
	void captureCam(GLubyte *ptr, int index,int mainOrsub=MAIN);
	void captureCamFish(GLubyte *ptr, int index);
	void Open();
	bool Append(pInterface_VCap cap);
	bool getScannerPic(IplImage *pImage);
	bool saveCapImg();
	void saveOverLap();
	virtual void StartProducers(){};
	static CaptureGroup* GetPanoCaptureGroup();
	static CaptureGroup* GetMainExtCaptureGroup();
	static CaptureGroup* GetSubExtCaptureGroup();
	static CaptureGroup* GetMvdectCaptureGroup();

	static CaptureGroup* GetExtCaptureGroup();
	static CaptureGroup* GetIconCaptureGroup();
	static CaptureGroup* GetRuler45CaptureGroup();
	static CaptureGroup* GetRuler90CaptureGroup();
	static CaptureGroup* GetRuler180CaptureGroup();
	bool saveSingleCapImg(int cam_num);
#if USE_12
	bool saveExposureCompensationCapImg();
	static CaptureGroup* GetSDICaptureGroup();
	static CaptureGroup* GetVGACaptureGroup();
#endif
protected:
	void Close();
	void fillColorBar();
	const unsigned int m_TotalCamCount;
	unsigned int m_currentIdx;
	pInterface_VCap *capCamera;
	unsigned int width, height, depth;
	unsigned char * Defaultimage;
	CaptureGroup():m_TotalCamCount(0),m_currentIdx(0){};
};




class MainExtCaptureGroup:public CaptureGroup
{
public:
	MainExtCaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual void SetConsumers(){};
	virtual void StartProducers(){};
private:
};

class SubExtCaptureGroup:public CaptureGroup
{
public:
	SubExtCaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual void SetConsumers(){};
	virtual void StartProducers(){};
private:
};

class ExtCaptureGroup:public CaptureGroup
{
public:
	ExtCaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual void SetConsumers(){};
	virtual void StartProducers(){};
private:
};

class IconCaptureGroup:public CaptureGroup
{
public:
	IconCaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual	void SetConsumers(){};
private:
};

class Ruler45CaptureGroup:public CaptureGroup
{
public:
	Ruler45CaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual	void SetConsumers(){};
private:
};

class Ruler90CaptureGroup:public CaptureGroup
{
public:
	Ruler90CaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual	void SetConsumers(){};
private:
};

class Ruler180CaptureGroup:public CaptureGroup
{
public:
	Ruler180CaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual	void SetConsumers(){};
private:
};

class SDICaptureGroup:public CaptureGroup
{
public:
	SDICaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual void StartProducers(){};
	virtual	void SetConsumers(){};
private:
};

class VGACaptureGroup:public CaptureGroup
{
public:
	VGACaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual void StartProducers(){};
	virtual	void SetConsumers(){};
private:
};



#endif /* CAPTUREGROUP_H_ */
