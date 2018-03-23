// StlTexturex.cpp :
// Author: Hongwei Lu
//
#include "RenderMain.h"
#include "common.h"
#include "main.h"
#include "timing.h"

RenderMain::RenderMain()
{
}
RenderMain::~RenderMain()
{
}
void RenderMain::DrawGLScene()
{





    static bool ONCE_FULLSCREEN = true;

	if(ONCE_FULLSCREEN){
		ONCE_FULLSCREEN = false;

		render.ProcessOitKeys('F', 0, 0);

	}

		render.DrawGLScene();


}
void RenderMain::ReSizeGLScene(int Width, int Height)
{
	render.ReSizeGLScene(Width,Height);
}
void RenderMain::keyPressed(unsigned char key, int x, int y)
{
	render.keyPressed(key,x,y);
}
void RenderMain::specialkeyPressed(int key, int x, int y)
{
	render.specialkeyPressed(key,x,y);
}
void RenderMain::mouseButtonPress(int button, int state, int x, int y)
{
	render.mouseButtonPress(button,state,x,y);
}
void RenderMain::mouseMotionPress(int x, int y)
{
	render.mouseMotionPress(x,y);
}
void RenderMain::DrawIdle()
{
	glutPostRedisplay();
}

void RenderMain::initGlut(int argc, char **argv)
{
	char arg1[256], arg2[256];
	// GLUT Window Initialization:
	glutInit (&argc, argv);
	glutInitWindowSize (1000, 700);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	{
	sprintf(arg1,"Argus %s (%s, %s):",VERSION_STRING, __DATE__,__TIME__);
	strcat (arg1, argv[1]);
	strcat (arg1, "+");
	strcat (arg1, argv[2]);

		/* getting a warning here about passing arg1 of sprinf incompatable pointer type ?? */
		/* WTF ?!? */
		if (sprintf(arg2, " %i+%i Polygons using ", render.BowlGetpoly_count(), render.VehicleGetpoly_count()))
		{
			strcat (arg1, arg2);
		}
		if (sprintf(arg2, "%i+%i Kb", render.BowlGetMemSize()/1024, render.VehicleGetMemSize()/1024))
		{
			strcat (arg1, arg2);
		}

		/* save most of the name for use later */
		common.setWindowName(arg1);

		if (sprintf(arg2, "%.2f FPS", common.getFrameRate()))
		{
			strcat (arg1, arg2);
		}
		glutCreateWindow (arg1);
	}
	glutSetCursor(GLUT_CURSOR_NONE);
	/* Register the event callback functions since we are using GLUT */




	glutDisplayFunc(DrawGLScene); /* Register the function to do all our OpenGL drawing. */
	glutIdleFunc(DrawIdle); /* Even if there are no events, redraw our gl scene. */
	glutReshapeFunc(ReSizeGLScene); /* Register the function called when our window is resized. */
	glutKeyboardFunc(keyPressed); /* Register the function called when the keyboard is pressed. */
	glutSpecialFunc(specialkeyPressed); /* Register the special key function */
	glutMouseFunc(mouseButtonPress); /* Register the function called when the mouse buttons are pressed */
	glutMotionFunc(mouseMotionPress); /*Register the mouse motion function */
}
void RenderMain::parseArgs(int argc, char** argv)
{
	char arg1[64];
	if (argc > 4)//4
	{
		strcpy(arg1, "-o");
		if (strcmp(argv[3], arg1) == 0)
		{
			printf("Running in Ortho View\n");
			common.setViewFlag(ORTHO);
		}
	}
	if (common.isPerspective())
	{
		printf("Running in Perspective View\n");
	}

	if (argc == 5)
	{
		strcpy(arg1, "-f");
		if (strcmp(argv[4], arg1) == 0)
		{
			printf("           Redrawing only on view change\n");
			common.setIdleDraw(GL_NO);
		}
		strcpy(arg1, "-v");
		if (strcmp(argv[4], arg1) == 0)
		{
			printf("           Debug Output Enabled\n");
			common.setVerbose(GL_YES);
		}
	}
	render.BowlParseSTLAscii(argv[1]);
	render.VehicleParseObj(argv[2]);
	/*
	float distance[4];
	distance[0]=atof(argv[3])*1.0;
	distance[1]=atof(argv[4])*1.0;
	distance[2]=atof(argv[5])*1.0;
	distance[3]=atof(argv[6])*1.0;

	if(distance[0]<0.0)
	{
		distance[0]=20.0;
	}
	if(distance[1]<0.0)
	{
		distance[1]=20.0;
	}
	if(distance[2]<0.0)
	{
		distance[2]=20.0;
	}
	if(distance[3]<0.0)
	{
		distance[3]=3.0;
	}

		render.setTankDistance(distance);
*/

	if(argv[3]!=NULL)
	{
		render.PanelParseSTLAscii(argv[3]);
	}
	else
	{
		render.PanelParseSTLAscii(argv[1]);
	}
}
void initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 20, 1}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}
void initGL()
{
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

     // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}
//--------main entry------------------
int RenderMain::start(int argc, char** argv)
{

	parseArgs(argc, argv);
	initGlut(argc, argv);
	initGlew();
	// initGL();
	render.initPixle();

	render.SetupRC(1920, 1080);//1920,1080);//
	glutFullScreen();
	glutMainLoop();
	return 0;
}
