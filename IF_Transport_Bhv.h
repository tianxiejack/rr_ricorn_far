/*
 * IF_Transport_Bhv.h
 *
 *  Created on: 2018年7月31日
 *      Author: fsmdn121
 */
 
#ifndef IF_TRANSPORT_BHV_H_
#define IF_TRANSPORT_BHV_H_
#include "glew.h"
#include <GLShaderManager.h>

class GLShaderManager;
class GLEnv;

class IF_Transport_Bhv
{
public:
	//处理Petal
	virtual void ProcessTexturePetalData(IF_Transport_Bhv *p,GLEnv &m_env,bool needSendData,int idx,enum GLT_STOCK_SHADER shaderId,int *p_petalNum)=0;
	//处理Petal_OverLap
	virtual void ProcessTextureOverLapData(IF_Transport_Bhv *p,GLEnv &m_env,enum GLT_STOCK_SHADER shaderId,int *p_petalNum)=0;
};

class Base_Core_Transport_Bhv:public IF_Transport_Bhv
{
public :
	Base_Core_Transport_Bhv();
	~Base_Core_Transport_Bhv();

	//MAIN SUB 激活的都是同一个overlap ,因为alpha mask 是相同的，不需要多个，需要实现
	void ProcessTextureOverLapData(IF_Transport_Bhv *p,GLEnv &m_env,enum GLT_STOCK_SHADER shaderId,int *p_petalNum);
};

class Core_Same_Transport_Bhv:public Base_Core_Transport_Bhv
{
public:
	Core_Same_Transport_Bhv();
	~Core_Same_Transport_Bhv();
	//相同的时候，默认使用texture[0]发送一次
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLEnv &m_env,bool needSendData,int idx, enum GLT_STOCK_SHADER shaderId,int *p_petalNum);//active alpha mask

};

class Core_Main_Transport_Bhv:public Base_Core_Transport_Bhv
{
public:
	Core_Main_Transport_Bhv();
	~Core_Main_Transport_Bhv();
	//不同的时候，进来的是main,使用texture[0] texture_sub[0]发送两次
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLEnv &m_env,bool needSendData,int idx,enum GLT_STOCK_SHADER shaderId,int *p_petalNum);//active alpha mask
private:
};

class Core_Sub_Transport_Bhv:public Base_Core_Transport_Bhv
{
public:
	Core_Sub_Transport_Bhv();
	~Core_Sub_Transport_Bhv();
	//不同的时候，进来的是sub,bind texture_sub[0]显示
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLEnv &m_env,bool needSendData,int idx, enum GLT_STOCK_SHADER shaderId,int *p_petalNum);//active alpha mask
private:
};

class Decorator_ChooseShader_Bhv:public IF_Transport_Bhv
{
public:
	Decorator_ChooseShader_Bhv(IF_Transport_Bhv *pbh,GLShaderManager *ptr_shaderManager);
	~Decorator_ChooseShader_Bhv();
	//先把core的实现，然后包上一层chooseShader
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLEnv &m_env,bool needSendData,int idx,enum GLT_STOCK_SHADER shaderId,int *p_petalNum);//choose shader
	//先把core的实现，然后包上一层chooseShader
	void ProcessTextureOverLapData(IF_Transport_Bhv *p,GLEnv &m_env,enum GLT_STOCK_SHADER shaderId,int *p_petalNum);
private:
	GLShaderManager *m_shaderManager;
	IF_Transport_Bhv *p_core;
};

#endif /* IF_TRANSPORT_BHV_H_ */
