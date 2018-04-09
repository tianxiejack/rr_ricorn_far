#if MVDECT
#include"MvDetect.hpp"

MvDetect::MvDetect()
{
		for(int i=0;i<CAM_COUNT;i++)
		{
			inNum[CAM_COUNT]=i;
			grayFrame[i]=(unsigned char *)malloc(MAX_SCREEN_WIDTH*MAX_SCREEN_HEIGHT*1);
		}
}
MvDetect::~MvDetect()
{
			exitDetect();
			for(int i=0;i<CAM_COUNT;i++)
			{
				free(grayFrame[i]);
			}
}

void MvDetect::init(int w,int h)
{
	for(int i=0;i<CAM_COUNT;i++)
	{
		createDetect((unsigned char)i,w,h);
	}

}
void MvDetect::yuyv2gray(unsigned char* src,unsigned char* dst,int width,int height)
{
	for(int j = 0;j<height*width;j++)
	{
			dst[j] = src[2*j +1];
	}
	return ;
}
void MvDetect::m_mvDetect(int idx,unsigned char* inframe,int w,int h)
{
	yuyv2gray(inframe,grayFrame[idx]);
	mvDetect((unsigned char) idx, grayFrame[idx], w, h,&outRect[idx]);
}
#endif
