#if MVDECT
#include"MvDetect.hpp"

MvDetect::MvDetect()
{
	for(int i=0;i<CAM_COUNT;i++)
	{
		inNum[CAM_COUNT]=i;
	}
}
MvDetect::~MvDetect()
{
			exitDetect();
}

void MvDetect::init(int w,int h)
{
	for(int i=0;i<CAM_COUNT;i++)
	{
		createDetect((unsigned char)i,w,h);
	}

}
void MvDetect::m_mvDetect(int idx,unsigned char* inframe,int w,int h)
{
	mvDetect((unsigned char) idx, inframe, w, h,&OutRect[idx]);
}
#endif
