/*
 * IF_Get_Tickets_Bhv.h
 *
 *  Created on: 2018年8月2日
 *      Author: fsmdn121
 */

#ifndef IF_GETTICKETS_BHV_H_
#define IF_GETTICKETS_BHV_H_

class IF_Get_Tickets_Bhv
{
public:
	//buffer.cpp getbuffer里进行获取数据
	virtual void Dq_Get_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc)=0;
	virtual unsigned char * Get_Sub_ptr(int qid)=0;
};


class Get_Different_Tickets_Bhv:public IF_Get_Tickets_Bhv
{
public:
	Get_Different_Tickets_Bhv();
	~Get_Different_Tickets_Bhv();
	//buffer.cpp getbuffer里进行获取数据 不同时，第二个指针指向数据
	 void Dq_Get_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc);
	void Set_Sub_ptr(unsigned char *src);
	unsigned char * Get_Sub_ptr(int qid);
private:
	unsigned char * Sub_ptr[2];
	unsigned char *m_pHalfPart;
};


class Get_Same_Tickets_Bhv:public IF_Get_Tickets_Bhv
{
public:
	
	//buffer.cpp getbuffer里进行获取数据
	 void Dq_Get_Bhv(unsigned char *src ,unsigned char *dst,int w,int h,int cc);
	unsigned char * Get_Sub_ptr(int qid){ };
private:
	
	unsigned char *m_pSrc;
};


#endif /* IF_GETTICKETS_BHV_H_ */
