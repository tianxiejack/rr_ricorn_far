#include "IF_Ferry.h"
#include "IF_Ferry_Company.h"
#include <string.h>
#include <malloc.h>

Ferry::Ferry(IF_Transport_Bhv *itb, IF_Get_Tickets_Bhv *igtb,IF_Order_Tickets_Bhv *iotb,
			 GLEnv &menv,
			 GLBatch *p_petals,
			 GLBatch *p_overlap,
			 int w,
			 int h,
			 int cc,enum GLT_STOCK_SHADER gssId_overlap,enum GLT_STOCK_SHADER gssId_petal,bool needconvert):m_env(menv),
			 m_w(w),m_h(h),m_cc(cc),m_itb(itb),m_igtb(igtb),m_iotb(iotb),m_gssId_overlap(gssId_overlap),m_gssId_petal(gssId_petal),m_needconvert(needconvert)
{
}
Ferry::~Ferry()
{
		delete m_itb;
		delete m_igtb;
		delete m_iotb;
}
 void Ferry::IQ_Put_Data(unsigned char *src ,unsigned char *dst,int w,int h,int cc)
 {	 	
	m_iotb->Iq_Put_Bhv(src, dst, w, h, m_cc,m_needconvert);
 }
void Ferry::DQ_Get_Data(unsigned char *src ,unsigned char *dst,int FpgaDataType)
{
	int tempw=-1;
	if(FpgaDataType == MAIN_FPGA_FOUR)
	{
		tempw= 1280;
	}
	else if(FpgaDataType == MAIN_FPGA_SIX){
		tempw= 1920;
	}
	m_igtb->Dq_Get_Bhv(src,dst, tempw, m_h, m_cc);
}
void Ferry::ProcessTexture(bool needSendData,int idx,int *p_petalNum)
{
	m_itb->ProcessTexturePetalData(m_itb,m_env, needSendData,idx, m_gssId_petal, p_petalNum);
	//draw
	if( p_petalNum == NULL){
		for(int i=0;i<10;i++){
			m_env.GetPanel_Petal(i)->Draw();
		}
	}
	else{
		for(int i=0;i<10;i++){
				if( p_petalNum[i] != -1){
				m_env.GetPanel_Petal(p_petalNum[i])->Draw();
				}
		}
	}
	m_itb->ProcessTextureOverLapData(m_itb,m_env, m_gssId_overlap, p_petalNum);
	//draw
	if( p_petalNum == NULL){
				for(int i=0;i<10;i++){
					m_env.Getp_Panel_Petal_OverLap(i)->Draw();
				}
		}
		else{
				for(int i=0;i<10;i++){
							if( p_petalNum[i] != -1){
							m_env.Getp_Panel_Petal_OverLap(p_petalNum[i])->Draw();
							}
				}
		}
}
//===================================================================================
Ferry_Company::Ferry_Company(std::vector<IF_Ferry*> p_ferry):m_ptrferry(p_ferry)
{		
}
Ferry_Company::~Ferry_Company()
{
}
void Ferry_Company::SwitchFerry(int ferryId)
{
	p_Current_ferry = m_ptrferry[ferryId];		
}
 void Ferry_Company::IQ_Put_Data(unsigned char *src ,unsigned char *dst,int w,int h,int cc)
 {
	 p_Current_ferry->IQ_Put_Data( src, dst,w,h,cc);
 }
 void Ferry_Company::DQ_Get_Data(unsigned char *src ,unsigned char *dst,int FpgaDataType)
 {
	p_Current_ferry->DQ_Get_Data(src, dst,FpgaDataType);
 }
 void Ferry_Company::ProcessTexture(bool needSendData,int idx,enum GLT_STOCK_SHADER shaderId, int shaderIdoverlap,int *p_petalNum,GLuint *textures)
 {
	p_Current_ferry->ProcessTexture( needSendData,idx,p_petalNum);
 }
