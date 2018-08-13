/*
 * IF_Order_Tickets_Bhv.h
 *
 *  Created on: 2018年8月1日
 *      Author: fsmdn121
 */

#ifndef IF_ORDERTICKETSBHV_H_
#define IF_ORDERTICKETSBHV_H_
#include "StlGlDefines.h"

class IF_Order_Tickets_Bhv
{
public:
	//数据入列
	virtual void Iq_Put_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc,bool needconvert)=0;
	virtual unsigned char *GetSrctoDrawRect()=0;
	virtual void  SetSrctoDrawRect(unsigned char *src)=0;
};

class Order_Same_Tickets_Bhv:public IF_Order_Tickets_Bhv
{
public:
	//相同时，入列
	
	Order_Same_Tickets_Bhv();
	virtual ~Order_Same_Tickets_Bhv();
	
	 void Iq_Put_Bhv(unsigned char *dst ,unsigned char *src,int w,int h,int cc,bool needconvert);
	 unsigned char *GetSrctoDrawRect();
	 void  SetSrctoDrawRect(unsigned char *src);
private:
	 unsigned char* m_pRectSrc;
	 
};

#endif /* IF_ORDERTICKETSBHV_H_ */
