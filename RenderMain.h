/*
 * RenderMain.h
 *
 *  Created on: Dec 15, 2016
 *      Author: wang
 */

#ifndef RENDERMAIN_H_
#define RENDERMAIN_H_

#include "GL/glew.h"
#include "GL/glut.h"
#include <math.h>
#include <iostream>
#include "gst/gst.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv/highgui.h"
#include "GLRender.h"
#include "LoadShader.h"
#include "ShaderParamArrays.h"
#include <GLTools.h>
#include "StlGlDefines.h"

using namespace cv;
using namespace std;
/*-------------
 * for stl parser
 *
 */


class RenderMain{
public:
	RenderMain();
	~RenderMain();
	void parseArgs(int argc, char** argv);
	void initGlut(int argc, char **argv);
	void captureUSB(GLubyte *ptr);
	void captureCSI(GLubyte *ptr);
	static void DrawGLScene();
	static void DrawIdle();
	static void ReSizeGLScene(int Width, int Height);
	static void keyPressed(unsigned char key, int x, int y);
	static void specialkeyPressed (int key, int x, int y);
	static void mouseButtonPress(int button, int state, int x, int y);
	static void mouseMotionPress(int x, int y);
	int start(int argc, char** argv);
private:
};

#endif /* RENDERMAIN_H_ */
