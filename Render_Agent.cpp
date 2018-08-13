/*
 *  Render_Agent.cpp
 *
 *  Created on: 2018年8月11日
 *      Author: fsmdn121
 */
#include"Render_Agent.h"
#include "GLRender.h"
#include"IF_Ferry_Company.h"
extern Render render;
void Render_Agent::GetData(unsigned char *bufdata,unsigned char *ptr,int FpgaDataType)
 {
	  render.Get_FerryCompany()->DQ_Get_Data(bufdata,ptr,FpgaDataType);
 }

GLuint *Render_Agent::Get_textures(){
	return render.Get_textures();
}
GLuint *Render_Agent::Get_GL_TextureIDs(){
	return render.Get_GL_TextureIDs();
}
GLuint *Render_Agent::Get_textures_sub(){
	return render.Get_textures_sub();
}
GLuint *Render_Agent::Get_GL_TextureIDs_sub(){
	return render.Get_GL_TextureIDs_sub();
}
