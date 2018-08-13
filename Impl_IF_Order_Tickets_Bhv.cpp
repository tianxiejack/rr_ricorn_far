#include "IF_Order_Tickets_Bhv.h"
#include <string.h>
#include<malloc.h>
void UYVnoXquar(unsigned char *dst,unsigned char *src, int ImgWidth, int ImgHeight)
{
	for(int j =0;j<ImgHeight;j++)
	{
		for(int i=0;i<ImgWidth*2/4;i++)
		{
			*(dst+j*ImgWidth*3+i*6+0)=*(src+j*ImgWidth*2+i*4+0);
			*(dst+j*ImgWidth*3+i*6+1)=*(src+j*ImgWidth*2+i*4+1);
			*(dst+j*ImgWidth*3+i*6+2)=*(src+j*ImgWidth*2+i*4+2);

			*(dst+j*ImgWidth*3+i*6+3)=*(src+j*ImgWidth*2+i*4+0);
			*(dst+j*ImgWidth*3+i*6+4)=*(src+j*ImgWidth*2+i*4+3);
			*(dst+j*ImgWidth*3+i*6+5)=*(src+j*ImgWidth*2+i*4+2);
		}
	}
}
void UYVY2UYV(unsigned char *dst,unsigned char *src, int ImgWidth, int ImgHeight)
{
	if (ImgWidth==FPGA_SCREEN_WIDTH) 
	{
		for(int i=0;i<4;i++)
		{
			UYVnoXquar(dst+i*FPGA_SINGLE_PIC_W*FPGA_SINGLE_PIC_H*3,
				src+i*FPGA_SINGLE_PIC_W*FPGA_SINGLE_PIC_H*2,
				FPGA_SINGLE_PIC_W,FPGA_SINGLE_PIC_H);
		}
	}
	else
	{
		ImgHeight/=4;
		for(int i=0;i<4;i++)
		{
			UYVnoXquar(dst+i*ImgWidth*ImgHeight*3,src+i*ImgWidth*ImgHeight*2,ImgWidth,ImgHeight);
		}
	}
}

Order_Same_Tickets_Bhv::Order_Same_Tickets_Bhv():m_pRectSrc(NULL)
{
}

Order_Same_Tickets_Bhv::~Order_Same_Tickets_Bhv()
{

}
 void Order_Same_Tickets_Bhv::Iq_Put_Bhv(unsigned char *dst ,unsigned char *src,int w,int h,int cc,bool needconvert)
 {	
	 if(needconvert){
		 UYVY2UYV(dst,src, w,h);
	}
 }
 unsigned char *Order_Same_Tickets_Bhv::GetSrctoDrawRect()
 {
	return m_pRectSrc;
 }
 void Order_Same_Tickets_Bhv::SetSrctoDrawRect(unsigned char *src)
 {
	 m_pRectSrc = src;
 }
