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
#include "GLEnv.h"

class GLBatch;

class IF_Ferry
{
public:
	virtual void IQ_Put_Data(unsigned char *src ,unsigned char *dst,int w,int h,int cc)=0;
	virtual void DQ_Get_Data(unsigned char *src ,unsigned char *dst,int FpgaDataType)=0;
	virtual void ProcessTexture(bool needSendData,int idx,int *p_petalNum)=0;
};

class Ferry:public IF_Ferry
{
public:
	Ferry(IF_Transport_Bhv *itb, IF_Get_Tickets_Bhv *igtb,IF_Order_Tickets_Bhv *iotb,
			 GLEnv &menv,
			 GLBatch *p_petals,
			 GLBatch *p_overlap,
			 int w,
			 int h,
			 int cc,GLT_STOCK_SHADER gssId_overlap,enum GLT_STOCK_SHADER gssId_petal,bool needconvert=true);
	~Ferry();
	 void IQ_Put_Data(unsigned char *src ,unsigned char *dst,int w,int h,int cc);
	 void DQ_Get_Data(unsigned char *src ,unsigned char *dst,int FpgaDataType);
	 void ProcessTexture(bool needSendData, int idx,int *p_petalNum);
	
private:
	 GLEnv &m_env;
	 int m_w;
	 int m_h;
	 int m_cc;
	 IF_Transport_Bhv * m_itb;
	 IF_Get_Tickets_Bhv *	m_igtb;
	 IF_Order_Tickets_Bhv *	m_iotb;
	 bool m_needconvert;
	 enum GLT_STOCK_SHADER m_gssId_overlap;
	 enum GLT_STOCK_SHADER m_gssId_petal;
};


#endif /* IF_FERRY_H_ */
