/*
 * IF_Ferry_Company.h
 *
 *  Created on: 2018年8月3日
 *      Author: fsmdn121
 */

#ifndef IF_FERRY_COMPANY_H_
#define IF_FERRY_COMPANY_H_

#include "IF_Ferry.h"
#include<vector>
#include<iostream>

using namespace std;
enum Ferry_Types{
	FERRY_MNOMVNOENH_SNOMVNOENH,
	MAIN_FERRY,
	SUB_FERRY,
	BOTH_ENHANCE_FERRY,
	MAIN_ENHANCE_FERRY,
	SUB_ENHANCE_FERRY,
	FERRY_COUNT
};

class IF_Ferry_Company
{
public:
	virtual void SwitchFerry(int ferryId)=0;
	virtual  void IQ_Put_Data(unsigned char *src ,unsigned char *dst,int w,int h,int cc)=0;
	virtual  void DQ_Get_Data(unsigned char *src ,unsigned char *dst,int FpgaDataType)=0;
	virtual  void ProcessTexture( bool needSendData, int idx,enum GLT_STOCK_SHADER shaderId, int shaderIdoverlap,int *p_petalNum, GLuint *textures)=0;
	virtual IF_Ferry* getCurFerry()=0;
};

class Ferry_Company:public IF_Ferry_Company
{
public:
	Ferry_Company(std::vector<IF_Ferry *> p_ferry);
	~Ferry_Company();
	void SwitchFerry(int ferryId);
	void IQ_Put_Data(unsigned char *src ,unsigned char *dst,int w,int h,int cc);
	void DQ_Get_Data(unsigned char *src ,unsigned char *dst,int FpgaDataType);
	void ProcessTexture(bool needSendData, int idx,enum GLT_STOCK_SHADER shaderId, int shaderIdoverlap,int *p_petalNum, GLuint *textures);
	IF_Ferry* getCurFerry(){return p_Current_ferry; };
private:
	 std::vector<IF_Ferry *> m_ptrferry;
	 IF_Ferry * p_Current_ferry;
};







#endif /* IF_FERRY_COMPANY_H_ */
