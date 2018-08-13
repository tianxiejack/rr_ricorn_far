#include "IF_Transport_Bhv.h"
#include <malloc.h>
#include <string.h>
#include <GLShaderManager.h>
#include "GLEnv.h"
#include "PBOManager.h"
#include "Render_Agent.h"
extern int alpha[12];

extern void capturePanoCam(GLubyte *ptr, int index,GLEnv &env);

Base_Core_Transport_Bhv::Base_Core_Transport_Bhv()
{
}
Base_Core_Transport_Bhv::~Base_Core_Transport_Bhv()
{
}
void Base_Core_Transport_Bhv::ProcessTextureOverLapData(IF_Transport_Bhv *p,
	GLEnv &m_env,enum GLT_STOCK_SHADER shaderId,int *p_petalNum)
{	
	glActiveTexture(Render_Agent::Get_GL_TextureIDs()[ALPHA_TEXTURE_IDX]);
	glBindTexture(GL_TEXTURE_2D,	Render_Agent::Get_textures()[ALPHA_TEXTURE_IDX]);
	glActiveTexture(Render_Agent::Get_GL_TextureIDs()[ALPHA_TEXTURE_IDX0]);
	glBindTexture(GL_TEXTURE_2D, Render_Agent::Get_textures()[ALPHA_TEXTURE_IDX0]);
}

//==================================================================

Core_Same_Transport_Bhv::Core_Same_Transport_Bhv()

{
	
}
Core_Same_Transport_Bhv::~Core_Same_Transport_Bhv()
{
	
}
void Core_Same_Transport_Bhv::ProcessTexturePetalData(IF_Transport_Bhv *p,
		GLEnv &m_env,bool needSendData,int idx,enum GLT_STOCK_SHADER shaderId,int *p_petalNum)
{

	glActiveTexture(Render_Agent::Get_GL_TextureIDs()[0]);  /*Active Petal Texture----Core_Petal*/

	//  pan duan yi xia petal de zhi 

		for(int i =0; i< 2; i++){
			if(needSendData){
				m_env.Getp_PBOMgr()->sendData(m_env,Render_Agent::Get_textures()[0],
						(PFN_PBOFILLBUFFER)capturePanoCam,i);
			}
			else{
				glBindTexture(GL_TEXTURE_2D, Render_Agent::Get_textures()[0]);
			}
		}

}
//===================================================================
Decorator_ChooseShader_Bhv::Decorator_ChooseShader_Bhv(IF_Transport_Bhv *pbh,
		GLShaderManager *ptr_shaderManager):
		m_shaderManager(ptr_shaderManager),
			p_core(pbh)
{

}
Decorator_ChooseShader_Bhv::~Decorator_ChooseShader_Bhv()
{

}
void Decorator_ChooseShader_Bhv::ProcessTexturePetalData(IF_Transport_Bhv *p,
		GLEnv &m_env,bool needSendData,int idx,enum GLT_STOCK_SHADER shaderId,int *p_petalNum)
{
	//  pan duan yi xia petal de zhi 
	p_core->ProcessTexturePetalData(NULL, m_env,needSendData,idx,shaderId, p_petalNum);//core
	
	//this  shader
	if(p_petalNum == NULL ){
		for(int i=0; i<10; i++){
				m_shaderManager->UseStockShader(shaderId, 
						m_env.GettransformPipeline()->GetModelViewProjectionMatrix(), 0,i );
		}
	}
	else
	{
		for(int i=0; i<10; i++){
			if(p_petalNum[i] != -1){
				m_shaderManager->UseStockShader(shaderId, 
						m_env.GettransformPipeline()->GetModelViewProjectionMatrix(), 0,p_petalNum[i]);
			}
		}
	}		
}
//先把core的实现，然后包上一层chooseShader
void Decorator_ChooseShader_Bhv::ProcessTextureOverLapData(IF_Transport_Bhv *p,
		GLEnv &m_env,enum GLT_STOCK_SHADER shaderId,int *p_petalNum)
{
	//core
		p_core->ProcessTextureOverLapData(NULL,m_env, shaderId,p_petalNum);

	if(p_petalNum == NULL ){
		for(int i=0; i<10; i++){
		 m_shaderManager->UseStockShader(shaderId, 
                                                   m_env.GettransformPipeline()->GetModelViewProjectionMatrix(),0,
                                                  0,ALPHA_TEXTURE_IDX0+alpha[i],i);
			}
	}
	else
	{
		for(int i=0; i<10; i++){
			if(p_petalNum[i] != -1){
					m_shaderManager->UseStockShader(shaderId, 
                                                   m_env.GettransformPipeline()->GetModelViewProjectionMatrix(),0,
                                                  0,ALPHA_TEXTURE_IDX0+alpha[i],i);
			}
		}
	}
}
