#include "PanoCaptureGroup.h"
static PanoCaptureGroup m_PanoGroup(SDI_WIDTH,SDI_HEIGHT,3,CAM_COUNT);

static HDAsyncVCap4* pHDAsyncVCap[MAX_CC]={0};
PanoCaptureGroup::PanoCaptureGroup(unsigned int w,unsigned int h,int NCHAN,unsigned int capCount):
		CaptureGroup(w,h,NCHAN,capCount)
{ }

vector<Consumer>  PanoCaptureGroup::GetConsumers()
{
	vector<Consumer> v_cons(2);
	 Consumer cons;
	   dev_id=FPGA_FOUR_CN;
	   cons.pcap = new HDVCap(dev_id,SDI_WIDTH,SDI_HEIGHT);
	   cons.idx = 0;
	   v_cons.push_back(cons);

	   dev_id=FPGA_SIX_CN;
	   cons.pcap =new HDVCap(dev_id,SDI_WIDTH,SDI_HEIGHT);
	   cons.idx = 1;
	   v_cons.push_back(cons);

	   return v_cons;
}

void  PanoCaptureGroup::CreateProducers()
{
		int dev_id=FPGA_FOUR_CN;
		if(pHDAsyncVCap[dev_id]==NULL)
			pHDAsyncVCap[dev_id] = new HDAsyncVCap4(auto_ptr<BaseVCap>(pv4lcap[dev_id]),dev_id);
		dev_id=FPGA_SIX_CN;
		if(pHDAsyncVCap[dev_id]==NULL)
			pHDAsyncVCap[dev_id] = new HDAsyncVCap4(auto_ptr<BaseVCap>(pv4lcap[dev_id]),dev_id);
};

void  PanoCaptureGroup::OpenProducers()
{
	int dev_id=FPGA_FOUR_CN;
	 pHDAsyncVCap[dev_id]->Open();
	 dev_id=FPGA_SIX_CN;
	 pHDAsyncVCap[dev_id]->Open();
}

PanoCaptureGroup::~PanoCaptureGroup()
{
	for(int i=1 ;i<MAX_CC;i++)//0 is not used
	{
		if(pHDAsyncVCap[i]){
					delete pHDAsyncVCap[i];
					pHDAsyncVCap[i]= NULL;
		}
	}
}


PanoCaptureGroup * PanoCaptureGroup::GetInstance()
{
	static bool once =true;
	if(once){
		m_PanoGroup.init();
		once =false;
	}
	return &m_PanoGroup;
}
