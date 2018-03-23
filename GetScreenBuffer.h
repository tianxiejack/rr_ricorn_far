#if 1
#ifndef GET_SCREEN_BUFFER_H
#define GET_SCREEN_BUFFER_H
using namespace std;

#define SCREEN_TEX_COUNT 1
#define WindowWidth 1920
#define WindowHeight 1080
#include"GLRender.h"

class Screen_data_PBO
{
public:
	Screen_data_PBO(){
		memset(data_size,0x00,sizeof(data_size));
	};
	~Screen_data_PBO(){};
	void init_PBOf4pix_buffs();
	GLuint GetscreenTarget(){
		static bool Once=true;
		if(true)
		{
			Once=false;
			return screen_Textures[0];
		}
		else
		{
			Once=true;
			return screen_Textures[1];
		}
	};
	void ReadBuff2PBO(int startx,int starty,int width,int height);
private:
	unsigned char data_size[1920*1080*4];
	GLuint screen_Textures[SCREEN_TEX_COUNT];
	GLuint pixBufferObjs[2];
};


class FBO_Manger
{
public:
	FBO_Manger();
	~FBO_Manger();
	void initFBO()
	{
		// Create and bind an FBO
			glGenFramebuffers(1,&fboName);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboName);

			// Create depth renderbuffer
			glGenRenderbuffers(1, &depthBufferName);
			glBindRenderbuffer(GL_RENDERBUFFER, depthBufferName);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, mirrorTexWidth, mirrorTexHeight);

			// Create the reflection texture
			glGenTextures(1, &mirrorTexture);
			glBindTexture(GL_TEXTURE_2D, mirrorTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mirrorTexWidth, mirrorTexHeight, 0, GL_RGBA, GL_FLOAT, NULL);

			// Attach texture to first color attachment and the depth RBO
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture, 0);
			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);

			// Make sure all went well
			gltCheckErrors();

			// Reset framebuffer binding
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	};
	void init_RBO(int width,int height);
	void CopyFromScreen(int startx,int starty,int w,int h);
private:
	GLuint fboName;
	GLuint	renderbuffernames;
	GLuint depthBufferName;
};


#endif
#endif
