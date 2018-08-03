/*
 * IF_Ferry_Company.h
 *
 *  Created on: 2018年8月3日
 *      Author: fsmdn121
 */

#ifndef IF_FERRY_COMPANY_H_
#define IF_FERRY_COMPANY_H_

#include "IF_Ferry.h"
enum{
	BOTH_FERRY,
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
	virtual  void IQ_Put_Data(unsigned char *src ,unsigned char *dst)=0;
	virtual  void DQ_Get_Data(unsigned char *src ,unsigned char *dst)=0;
	virtual  void ProcessTexture()=0;
};

class Ferry_Company:public IF_Ferry_Company
{
public:
	Ferry_Company(vector<IF_Ferry *> p_ferry
	);
	~Ferry_Company();
	void SwitchFerry(int ferryId);
	 void IQ_Put_Data(unsigned char *src ,unsigned char *dst);
	 void DQ_Get_Data(unsigned char *src ,unsigned char *dst);
	 void ProcessTexture();
private:
	 vector<IF_Ferry *> mp_ferry
	 IF_Ferry * p_Current_ferry;
};







#endif /* IF_FERRY_COMPANY_H_ */
