/*
 * IF_Ferry.h
 *
 *  Created on: 2018年8月2日
 *      Author: fsmdn121
 */

#ifndef IF_FERRY_H_
#define IF_FERRY_H_

#include"IF_Order_Tickets_Bhv.h"
#include "IF_Get_Tickets_Bhv.h"
#include "IF_Transport_Bhv.h"

class IF_Ferry
{
public:
	virtual void IQ_Put_Data(unsigned char *src ,unsigned char *dst)=0;
	virtual void DQ_Get_Data(unsigned char *src ,unsigned char *dst)=0;
	virtual void ProcessTexture()=0;
};

class Ferry:public IF_Ferry
{
public:
	Ferry(IF_Transport_Bhv *itb, IF_Get_Tickets_Bhv *igtb,IF_Order_Tickets_Bhv *iotb,
			 GLuint *textureID,
			 GLEnv &menv,
			 GLBatch *p_petals,
			 GLBatch *p_overlap,
			 int w,
			 int h,
			 int cc,)
	~Ferry();
	 void IQ_Put_Data(unsigned char *src ,unsigned char *dst);
	 void DQ_Get_Data(unsigned char *src ,unsigned char *dst);
	 void ProcessTexture();
private:
	 GLuint *m_textureID;
	 GLEnv &m_env;
	 GLBatch *mp_petals;
	 GLBatch *mp_overlap;
	 int m_w;
	 int m_h;
	 int m_cc;
	 IF_Transport_Bhv * m_itb;
	 IF_Get_Tickets_Bhv *	m_igtb;
	 IF_Order_Tickets_Bhv *	m_iotb;
};


#endif /* IF_FERRY_H_ */
