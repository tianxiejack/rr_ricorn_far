#include "IF_Get_Tickets_Bhv.h"
#include<string.h>
#include<malloc.h>
#include "StlGlDefines.h"



void Get_Different_Tickets_Bhv::Dq_Get_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc)
{
	memcpy(dst,src, w*h*cc);
	Set_Sub_ptr(src+w*h*cc);
}

Get_Different_Tickets_Bhv::Get_Different_Tickets_Bhv():m_pHalfPart(NULL)
{

}

Get_Different_Tickets_Bhv::~Get_Different_Tickets_Bhv()
{
	if( m_pHalfPart != NULL){
		m_pHalfPart = NULL;
		
	}
}
void Get_Different_Tickets_Bhv::Set_Sub_ptr(unsigned char *src)
{
	m_pHalfPart = src;
}
unsigned char * Get_Different_Tickets_Bhv::Get_Sub_ptr(int qid)
{
	return m_pHalfPart;

}

 void Get_Same_Tickets_Bhv::Dq_Get_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc)
{
	memcpy(dst, src, w*h*cc);
}


