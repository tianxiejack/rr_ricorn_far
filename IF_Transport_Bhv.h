/*
 * IF_Transport_Bhv.h
 *
 *  Created on: 2018年7月31日
 *      Author: fsmdn121
 */

#ifndef IF_TRANSPORT_BHV_H_
#define IF_TRANSPORT_BHV_H_

#include "GLEnv.h"
#include "GLRender.h"

class IF_Transport_Bhv
{
	//处理Petal
	virtual void ProcessTexturePetalData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env)=0;
	//处理Petal_OverLap
	virtual void ProcessTextureOverLapData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env)=0;
};


class Base_Core_Transport_Bhv:public IF_Transport_Bhv
{
public :
	Base_Core_Transport_Bhv();
	~Base_Core_Transport_Bhv();
	//MAIN SUB 激活的都是同一个overlap ,因为alpha mask 是相同的，不需要多个，需要实现
	virtual	void ProcessTextureOverLapData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env);
};

class Core_Same_Transport_Bhv:public Base_Core_Transport_Bhv
{
public:
	Core_Same_Transport_Bhv();
	~Core_Same_Transport_Bhv();
	//相同的时候，默认使用texture[0]发送一次
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env);//active alpha mask
private:
};

class Core_Main_Transport_Bhv:public Base_Core_Transport_Bhv
{
public:
	Core_Main_Transport_Bhv();
	~Core_Main_Transport_Bhv();
	//不同的时候，进来的是main,使用texture[0] texture_sub[0]发送两次
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env);//active alpha mask
private:
};

class Core_Sub_Transport_Bhv:public Base_Core_Transport_Bhv
{
public:
	Core_Sub_Transport_Bhv();
	~Core_Sub_Transport_Bhv();
	//不同的时候，进来的是sub,bind texture_sub[0]显示
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env);//active alpha mask
private:
};

class Decorator_ChooseShader_Bhv:public IF_Transport_Bhv
{
public:
	Decorator_ChooseShader_Bhv(IF_Transport_Bhv *pbh);
	~Decorator_ChooseShader_Bhv(){};
	//先把core的实现，然后包上一层chooseShader
	void ProcessTexturePetalData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env);//choose shader
	//先把core的实现，然后包上一层chooseShader
	void ProcessTextureOverLapData(IF_Transport_Bhv *p,GLuint *textureID,GLEnv &m_env);
};






#endif /* IF_TRANSPORT_BHV_H_ */
