#ifndef _PANO_CAP_GROUP_H_
#define  _PANO_CAP_GROUP_H_
#include "CaptureGroup.h"

class PanoCaptureGroup:public CaptureGroup
{
public:
	static PanoCaptureGroup * GetInstance();
	PanoCaptureGroup(unsigned int w,unsigned int h,int NCHAN,unsigned int capCount=1);
	~PanoCaptureGroup();
	virtual void CreateProducers();
	virtual vector<Consumer>  GetConsumers();
	virtual void OpenProducers();
	bool saveExposureCompensationCapImg();
private:
	PanoCaptureGroup();
	PanoCaptureGroup(const PanoCaptureGroup&);
};


#endif
