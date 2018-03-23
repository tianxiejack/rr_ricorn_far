/*
 * OitVehicle.h
 *	Order-Independent Transparent Vehicle implementation
 *  Created on: Nov 20, 2016
 *      Author: hoover
 */

#ifndef OITVEHICLE_H_
#define OITVEHICLE_H_
#include "GLMatrixStack.h"
class GLMmodel;
class GLShaderManager;
class OitVehicle{
public:
	OitVehicle(GLMatrixStack &modelViewMat,GLMatrixStack	&projectionMat,GLShaderManager* mgr=NULL);
	~OitVehicle(){};
	void DrawVehicle();
	void DrawVehicle_second();
	void DrawVehicle_third();
	inline void SetLoader(GLMmodel* pLoader){pVehicleLoader=pLoader;};
	void InitVehicle();
	void InitShaders();
	void initFBOs(int w, int h);
	void updateFBOs();
	void PrepareBlendMode();
	void SetBlendMode(int md, int blendmd){mode = md; blendmode = blendmd;};
	void SetupOITResolveProg();
	void SetupResolveProg();
	void GenerateOrtho2DMat(GLuint imageWidth, GLuint imageHeight);
	void ChangeSize(int nWidth, int nHeight);
	const GLfloat* GetDimensions() const{return m_dimensions;};
	const GLfloat* GetYMaxMins() const{return m_yMaxMins;}
	void SetScale(GLfloat s){m_vehicleScale = s;};// dynamically scale the vehicle;
	GLfloat GetScale(){return m_vehicleScale;};
	void SetTranslate(GLfloat t[3]){m_vehicleTranslation[0] = t[0];
									m_vehicleTranslation[1] = t[1];
									m_vehicleTranslation[2] = t[2];};
	void SetTankDis(GLfloat t[4]){m_vehicleTranslation[1] = t[0];
	m_vehicleTranslation[3] = t[1];
	m_vehicleTranslation[4] = t[2];
	m_vehicleTranslation[5] = t[4];};
	static const int USER_OIT  = 1; 
	static const int USER_BLEND = 2;	
	GLuint		   msFBO;
private:
	OitVehicle():modelViewMatrix(modelViewMatrix),projectionMatrix(projectionMatrix){};
	bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void initDimensions();
	GLBatch             screenQuad;
	M3DMatrix44f        orthoMatrix;
	GLTriangleBatch				m_vehicle;
	GLShaderManager * 	m_pShaderManager;
	GLMatrixStack &		modelViewMatrix;
	GLMatrixStack &		projectionMatrix;
	GLMmodel     	*	pVehicleLoader;
	int 					mode;
	int 					blendmode;
	GLuint              msResolve;
	GLuint              oitResolve;
	GLuint		    	msTexture[1];
	GLuint		 depthTextureName;
	int 			m_screenWidth, m_screenHeight;

	GLfloat           m_dimensions[3];
	GLfloat		m_yMaxMins[2];
	GLfloat			m_vehicleScale;
	GLfloat		m_vehicleTranslation[6];
	GLuint  vehicleTexture;
	GLuint	ADSTextureShader;		// The diffuse light shader
	GLint	locAmbient;			// The location of the ambient color
	GLint   locDiffuse;			// The location of the diffuse color
	GLint   locSpecular;		// The location of the specular color
	GLint	locLight;			// The location of the Light in eye coordinates
	GLint	locMVP;				// The location of the ModelViewProjection matrix uniform
	GLint	locMV;				// The location of the ModelView matrix uniform
	GLint	locNM;				// The location of the Normal matrix uniform
    GLuint  locTexture;			//The location of texture

};



#endif /* OITVEHICLE_H_ */
