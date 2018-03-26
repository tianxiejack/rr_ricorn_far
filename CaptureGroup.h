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
using namespace cv;

class CaptureGroup
{
public:
	CaptureGroup(unsigned int capCount);
	CaptureGroup(unsigned int wide,unsigned int height,int NCHAN,unsigned int capCount=1);
	virtual ~CaptureGroup();
	void captureCam(GLubyte *ptr, int index);
	void captureCamFish(GLubyte *ptr, int index);
	void Open();
	bool Append(pInterface_VCap cap);
	bool getScannerPic(IplImage *pImage);
	bool saveCapImg();

	void saveOverLap();
	static CaptureGroup* GetPanoCaptureGroup();
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
private:
	CaptureGroup():m_TotalCamCount(0),m_currentIdx(0){};
	void Close();
	void fillColorBar();
	const unsigned int m_TotalCamCount;
	unsigned int m_currentIdx;
	pInterface_VCap *capCamera;
	unsigned int width, height, depth;
	unsigned char * Defaultimage;
};

#endif /* CAPTUREGROUP_H_ */
