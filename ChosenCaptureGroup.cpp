#include "ChosenCaptureGroup.h"
#include"HDV4lcap.h"
ChosenCaptureGroup ChosenCaptureGroup::MainChosenGroup(SDI_WIDTH,SDI_HEIGHT,3,MAIN_EXT_COUNT);
ChosenCaptureGroup ChosenCaptureGroup::SubChosenGroup(SDI_WIDTH,SDI_HEIGHT,3,MAIN_EXT_COUNT);
static HDAsyncVCap4* pHDAsyncVCap[MAX_CC]={0};
ChosenCaptureGroup::ChosenCaptureGroup(unsigned int w,unsigned int h,int NCHAN,unsigned int capCount):
		HDCaptureGroup(w,h,NCHAN,capCount)
{ }

void  ChosenCaptureGroup::CreateProducers()
{
		int dev_id=MAIN_CN;
		if(pHDAsyncVCap[dev_id]==NULL)
			pHDAsyncVCap[dev_id] = new HDAsyncVCap4(auto_ptr<BaseVCap>(HDv4l_cam_Producer::Get_pHDv4l_cap(dev_id)),dev_id);
};

void  ChosenCaptureGroup::OpenProducers()
{
	int dev_id=FPGA_FOUR_CN;
	 pHDAsyncVCap[dev_id]->Open();
	 dev_id=FPGA_SIX_CN;
	 pHDAsyncVCap[dev_id]->Open();
}

ChosenCaptureGroup::~ChosenCaptureGroup()
{
	for(int i=1 ;i<MAX_CC;i++)//0 is not used
	{
		if(pHDAsyncVCap[i]){
					delete pHDAsyncVCap[i];
					pHDAsyncVCap[i]= NULL;
		}
	}
}

ChosenCaptureGroup * ChosenCaptureGroup::GetMainInstance()
{
	int queueid[1]={MAIN_ONE_OF_TEN};
	int count=1;
	static bool once =true;
	if(once){
		MainChosenGroup.init(queueid,count);
		once =false;
	}
	return &MainChosenGroup;
}

ChosenCaptureGroup * ChosenCaptureGroup:: GetSubInstance()
{
	int queueid[1]={SUB_ONE_OF_TEN};
	int count=1;
	static bool once =true;
	if(once){
		SubChosenGroup.init(queueid,count);
		once =false;
	}
	return &SubChosenGroup;
}
