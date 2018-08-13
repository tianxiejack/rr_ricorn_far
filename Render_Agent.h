/*
 * Render_Agent.h
 *
 *  Created on: 2018年8月11日
 *      Author: fsmdn121
 */

#ifndef RENDER_AGENT_H_
#define RENDER_AGENT_H_
#include <GL/glew.h> // glUniformxxx()
class Render_Agent
{
public:
	static void GetData(unsigned char *bufdata,unsigned char *ptr,int FpgaDataType);
	static GLuint *Get_textures();
	static GLuint *Get_GL_TextureIDs();
	static GLuint *Get_textures_sub();
	static GLuint *Get_GL_TextureIDs_sub();
};






#endif /* RENDER_AGENT_H_ */
