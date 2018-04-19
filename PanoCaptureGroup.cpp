#include "PanoCaptureGroup.h"
static PanoCaptureGroup MainPanoGroup(SDI_WIDTH,SDI_HEIGHT,3,CAM_COUNT);

static PanoCaptureGroup SubPanoGroup(SDI_WIDTH,SDI_HEIGHT,3,CAM_COUNT);
static HDAsyncVCap4* pHDAsyncVCap[MAX_CC]={0};
PanoCaptureGroup::PanoCaptureGroup(unsigned int w,unsigned int h,int NCHAN,unsigned int capCount):
		CaptureGroup(w,h,NCHAN,capCount)
{ }

void  PanoCaptureGroup::CreateProducers()
{
	if(m_ProducerOnce)
	{
		m_ProducerOnce=false;
		HDv4l_cam * pv4lcap[MAX_CC]={NULL,&v4lcap1,&v4lcap2,&v4lcap3,&v4lcap4,&v4lcap5};
		int dev_id=FPGA_FOUR_CN;
		if(pHDAsyncVCap[dev_id]==NULL)
			pHDAsyncVCap[dev_id] = new HDAsyncVCap4(auto_ptr<BaseVCap>(pv4lcap[dev_id]),dev_id);
		dev_id=FPGA_SIX_CN;
		if(pHDAsyncVCap[dev_id]==NULL)
			pHDAsyncVCap[dev_id] = new HDAsyncVCap4(auto_ptr<BaseVCap>(pv4lcap[dev_id]),dev_id);
	}
};

vector<Consumer>  PanoCaptureGroup::GetConsumers(int *queueid,int count)
{
	vector<Consumer> v_cons(2);
	 Consumer cons;
	 for(int i=0;i<count;i++)
	 {
		 int qid=*(queueid[i]);
		   cons.pcap = new HDVCap(qid,SDI_WIDTH,SDI_HEIGHT);
		   cons.idx = i;
		   v_cons.push_back(cons);
	 }

	   return v_cons;
}

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


PanoCaptureGroup * PanoCaptureGroup::GetMainInstance()
{
	int queueid[2]={MAIN_FPGA_FOUR,MAIN_FPGA_SIX};
	int count=2;
	static bool once =true;
	if(once){
		MainPanoGroup.init(queueid,count);
		once =false;
	}
	return &MainPanoGroup;
}

PanoCaptureGroup * PanoCaptureGroup::GetSubInstance()
{
	int queueid[2]={SUB_FPGA_FOUR,SUB_FPGA_SIX};
	int count=2;
	static bool once =true;
	if(once){
		SubPanoGroup.init(queueid,count);
		once =false;
	}
	return &SubPanoGroup;
}
