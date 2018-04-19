#include"BMPCaptureGroup.h"
#include"Camera.h"
BMPPanoGroup BMPPanoGroup::bmpCaptureGroup(SDI_WIDTH,SDI_HEIGHT,3,CAM_COUNT);

vector<Consumer>  BMPPanoGroup::GetConsumers(int *queueid,int count)
{
	vector<Consumer> v_cons(count);
	 Consumer cons;
	 char *str[]={"00","01","02","03","04","05","06","07","08","09"};
	 char filename[64];
	 for(int i=0;i<count;i++)
	 {
		 sprintf(filename,"./data/%s.bmp",str[i]);
		   cons.pcap = new BMPVcap(filename);
		   cons.idx = i;
		   v_cons.push_back(cons);
	 }
	   return v_cons;
}

BMPPanoGroup * BMPPanoGroup::GetInstance()
{
			static bool once =true;
			if(once){
				bmpCaptureGroup.init(NULL,CAM_COUNT);
				once =false;
			}
			return &bmpCaptureGroup;
}


