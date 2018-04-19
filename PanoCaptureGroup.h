#ifndef _PANO_CAP_GROUP_H_
#define  _PANO_CAP_GROUP_H_
#include "CaptureGroup.h"

class PanoCaptureGroup:public CaptureGroup
{
public:
	static PanoCaptureGroup * GetMainInstance();
	static PanoCaptureGroup * GetSubInstance();
	PanoCaptureGroup(unsigned int w,unsigned int h,int NCHAN,unsigned int capCount=1);
	~PanoCaptureGroup();
	virtual void CreateProducers();
	virtual vector<Consumer>  GetConsumers(int *queueid,int count);
	virtual void OpenProducers();
	bool saveExposureCompensationCapImg();
private:
	static bool m_ProducerOnce=true;
	PanoCaptureGroup();
	PanoCaptureGroup(const PanoCaptureGroup&);
};


#endif
