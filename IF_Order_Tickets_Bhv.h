/*
 * IF_Order_Tickets_Bhv.h
 *
 *  Created on: 2018年8月1日
 *      Author: fsmdn121
 */

#ifndef IF_ORDERTICKETSBHV_H_
#define IF_ORDERTICKETSBHV_H_

class IF_Order_Tickets_Bhv
{
public:
	//数据入列
	virtual void Iq_Put_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc)=0;
	virtual unsigned char *GetSrctoDrawRect(unsigned char *src)=0;
};

class Order_Same_Tickets_Bhv:public IF_Order_Tickets_Bhv
{
public:
//相同时，入列
	 void Iq_Put_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc);
	unsigned char *GetSrctoDrawRect(unsigned char *src);
private:
};

class Order_Different_Tickets_Bhv:public IF_Order_Tickets_Bhv
{
public:
	//不同时入列
	 void Iq_Put_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc);
	 unsigned char *GetSrctoDrawRect(unsigned char *src);
private:
};

#endif /* IF_ORDERTICKETSBHV_H_ */
