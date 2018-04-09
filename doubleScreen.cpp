#include"RenderMain.h"
#include "GLRender.h"
#include "common.h"
extern Render render;
Common comSecondSC;

extern float forward_data;

void InitBowlDS()
{

}


void Render::RenderSceneDS()
{
	//DrawInitView(new Rect(0, 0, g_windowWidth, g_windowHeight), true);
	RenderScene();
}
void Render::SetupRCDS(int windowWidth, int windowHeight)
{
#if 1
	{
		// Initialze Shader Manager
		shaderManager2.InitializeStockShaders();

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// This makes a torus
		gltMakeTorus(torusBatch2, 0.4f, 0.15f, 30, 30);

	    // This make a sphere
	    gltMakeSphere(sphereBatch2, 0.1f, 26, 13);

		floorBatch2.Begin(GL_LINES, 324);
	    for(GLfloat x = -20.0; x <= 20.0f; x+= 0.5) {
	        floorBatch2.Vertex3f(x, -0.55f, 20.0f);
	        floorBatch2.Vertex3f(x, -0.55f, -20.0f);

	        floorBatch2.Vertex3f(20.0f, -0.55f, x);
	        floorBatch2.Vertex3f(-20.0f, -0.55f, x);
	        }
	    floorBatch2.End();
	    }

#else
#define PANO_FLOAT_DATA_FILENAME "panofloatdata.yml"
	GLubyte *pBytes;
	GLint nWidth=DEFAULT_IMAGE_WIDTH, nHeight=DEFAULT_IMAGE_HEIGHT, nComponents=GL_RGBA8;
	GLenum format= GL_BGRA;

	if(!shaderManager.InitializeStockShaders()){
		cout<<"failed to intialize shaders"<<endl;
		exit(1);
	}

	if(!PBOMgr.Init()
				|| !PBOExtMgr.Init()
				||!PBORcr.Init()
				|| !PBOVGAMgr.Init()
				|| !PBOSDIMgr.Init()){
		cout<<"Failed to init PBO manager"<<endl;
			exit(1);
		}
		if(!FBOmgr.Init())
		{
			printf("FBO init failed\n");
			exit(-1);
		}

	// midNight blue background
	glClearColor(0.0f, 0/255.0f, 0.0f, 1.0f);//25/255.0f, 25/255.0f, 112/255.0f, 0.0f);
	glLineWidth(1.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);     //enable blending
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glClearDepth(1.0);				/* Enables Clearing Of The Depth Buffer*/
	glDepthFunc(GL_LESS);			        /* The Type Of Depth Test To Do*/
	glEnable(GL_DEPTH_TEST);		        /* Enables Depth Testing*/
	glShadeModel(GL_SMOOTH);			/* Enables Smooth Color Shading*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	SetView(windowWidth, windowHeight);
	{//setting up models and their textures
	//	overLapRegion::GetoverLapRegion()->SetSingleHightLightState(false);
			ReadPanoFloatDataFromFile(PANO_FLOAT_DATA_FILENAME);

		transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
		InitLineofRuler();


		GenerateCenterView();
		GenerateCompassView();
		GenerateScanPanelView();
		GeneratePanoView();

		GenerateTriangleView();
		GeneratePanoTelView();
		GenerateTrack();

		GenerateLeftPanoView();
		GenerateRightPanoView();
		GenerateLeftSmallPanoView();
		GenerateRightSmallPanoView();

		float x;
		x=(p_LineofRuler->Load())/360.0*(render.get_PanelLoader().Getextent_pos_x()-render.get_PanelLoader().Getextent_neg_x());
		RulerAngle=p_LineofRuler->Load();
		GenerateOnetimeView();
		GenerateOnetimeView2();

		GenerateTwotimesView();
		GenerateTwotimesView2();

		GenerateTwotimesTelView();
		GenerateFourtimesTelView();
		GenerateCheckView();

		GenerateBirdView();
		GenerateFrontView();
		GenerateRearTopView();
		GenerateExtentView();

		GenerateSDIView();
		GenerateVGAView();
		PanoLen=(PanelLoader.Getextent_pos_x()-PanelLoader.Getextent_neg_x());
		PanoHeight=(PanelLoader.Getextent_pos_z()-PanelLoader.Getextent_neg_z());
//		foresightPos.SetPanoLen_Height(PanoLen,PanoHeight);
	//	zodiac_msg.setPanoHeight_Length(PanoHeight,PanoLen);

	//	camonforesight.setPanoheight(PanoHeight);
	//	panocamonforesight.setPanoheight(PanoHeight);
//		panocamonforesight.setPanolen(PanoLen);
//		telcamonforesight.setPanoheight(PanoHeight);
//		telcamonforesight.setPanolen(PanoLen);
	//camonforesight.setPanolen(PanoLen);

		InitALPHA_ZOOM_SCALE();
	//	InitBowl();
		InitScanAngle();
		InitPanoScaleArrayData();
		InitPanel();
		InitFollowCross();
		InitRuler();
		InitCalibrate();
		InitOitVehicle();
		//    glmDelete(VehicleLoader);
		pVehicle->initFBOs(windowWidth, windowHeight);

		InitShadow();
		InitBillBoard();
//		InitFrontTracks();
		InitWheelTracks();
		InitCrossLines();
		InitWealTrack();
		InitDynamicTrack();
		InitCornerMarkerGroup();
		initAlphaMask();
		InitDataofAlarmarea();

				FILE *fp;
				char read_data[20];
				fp=fopen("forward.yml","r");
				if(fp!=NULL)
				{
					fscanf(fp,"%f\n",&forward_data);
					fclose(fp);
					printf("forward:%f\n",forward_data);
				}
		InitForesightGroupTrack();
		DrawNeedleonCompass();
		DrawTriangle();

		pthread_t th_rec;
	   	int arg_rec = 10;
		mp_FboPboFacade=new PBO_FBO_Facade(FBOmgr,PBORcr);
	   	mPresetCamGroup.LoadCameras();
		// Load up CAM_COUNT textures
		glGenTextures(PETAL_TEXTURE_COUNT, textures);

		for(int i = 0; i < CAM_COUNT; i++){
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexImage2D(GL_TEXTURE_2D,0,nComponents,FLEXIBLE_DEFAULT_IMAGE_WIDTH, FLEXIBLE_DEFAULT_IMAGE_HEIGHT, 0,
					format, GL_UNSIGNED_BYTE, 0);
		}

		// Alpha mask: 1/16 size of 1920x1080
		glBindTexture(GL_TEXTURE_2D, textures[ALPHA_TEXTURE_IDX]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,nComponents,ALPHA_MASK_WIDTH, ALPHA_MASK_HEIGHT, 0,
				format, GL_UNSIGNED_BYTE, alphaMask);

		glBindTexture(GL_TEXTURE_2D, textures[ALPHA_TEXTURE_IDX0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,nComponents,ALPHA_MASK_WIDTH, ALPHA_MASK_HEIGHT, 0,
				format, GL_UNSIGNED_BYTE, alphaMask0);

		glBindTexture(GL_TEXTURE_2D, textures[ALPHA_TEXTURE_IDX1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,nComponents,ALPHA_MASK_WIDTH, ALPHA_MASK_HEIGHT, 0,
				format, GL_UNSIGNED_BYTE, alphaMask1);
	}
	//setting up extension textures etc.
	{
		glGenTextures(EXTENSION_TEXTURE_COUNT, extensionTextures);
		for(int i = 0; i < EXTENSION_TEXTURE_COUNT; i++){
			glBindTexture(GL_TEXTURE_2D, extensionTextures[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexImage2D(GL_TEXTURE_2D,0,nComponents,nWidth, nHeight, 0,
					format, GL_UNSIGNED_BYTE, 0);
		}

		glGenTextures(VGA_TEXTURE_COUNT, VGATextures);
				for(int i = 0; i < VGA_TEXTURE_COUNT; i++){
					glBindTexture(GL_TEXTURE_2D, VGATextures[i]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
					glTexImage2D(GL_TEXTURE_2D,0,nComponents,VGA_WIDTH, VGA_HEIGHT, 0,
							format, GL_UNSIGNED_BYTE, 0);
				}

		glGenTextures(SDI_TEXTURE_COUNT, SDITextures);
				for(int i = 0; i < SDI_TEXTURE_COUNT; i++){
					glBindTexture(GL_TEXTURE_2D, SDITextures[i]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
					glTexImage2D(GL_TEXTURE_2D,0,nComponents,SDI_WIDTH, SDI_HEIGHT, 0,
							format, GL_UNSIGNED_BYTE, 0);
				}
	}
	glMatrixMode(GL_MODELVIEW);
#endif
}


void Render::GetFPSDS()
{
	/* Number of samples for frame rate */
#define FR_SAMPLES 10

	static struct timeval last={0,0};
	struct timeval now;
	float delta;
	if (comSecondSC.plusAndGetFrameCount() >= FR_SAMPLES) {
		gettimeofday(&now, NULL);
		delta= (now.tv_sec - last.tv_sec +(now.tv_usec - last.tv_usec)/1000000.0);
		last = now;
		comSecondSC.setFrameRate(FR_SAMPLES / delta);
		comSecondSC.setFrameCount(0);
	}
}
void Render::DrawGLSceneDS()
{
#if 1
	{
	    // Color values
	    static GLfloat vFloorColor[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	    static GLfloat vTorusColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	    static GLfloat vSphereColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };

	    // Time Based animation
		static CStopWatch	rotTimer;
		float yRot = rotTimer.GetElapsedSeconds() * 60.0f*1.5;

		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	    // Save the current modelview matrix (the identity matrix)
		modelViewMatrix2.PushMatrix();

	    M3DMatrix44f mCamera;
	    cameraFrame2.GetCameraMatrix(mCamera);
	    modelViewMatrix2.PushMatrix(mCamera);

		// Draw the ground
		shaderManager2.UseStockShader(GLT_SHADER_FLAT,
									 transformPipeline2.GetModelViewProjectionMatrix(),
									 vFloorColor);
		floorBatch2.Draw();

	    // Draw the spinning Torus
	    modelViewMatrix2.Translate(0.0f, 0.0f, -2.5f*2);

	    // Save the Translation
	    modelViewMatrix2.PushMatrix();

	        // Apply a rotation and draw the torus
	        modelViewMatrix2.Rotate(yRot, 0.0f, 1.0f, 0.0f);
	        shaderManager2.UseStockShader(GLT_SHADER_FLAT, transformPipeline2.GetModelViewProjectionMatrix(),
	                                vTorusColor);
	        torusBatch2.Draw();
	    modelViewMatrix2.PopMatrix(); // "Erase" the Rotation from before

	    // Apply another rotation, followed by a translation, then draw the sphere
	    modelViewMatrix2.Rotate(yRot * -2.0f, 0.0f, 1.0f, 0.0f);
	    modelViewMatrix2.Translate(0.8f, 0.0f, 0.0f);
	    shaderManager2.UseStockShader(GLT_SHADER_FLAT, transformPipeline2.GetModelViewProjectionMatrix(),
	                                     vSphereColor);
	    sphereBatch2.Draw();

		// Restore the previous modleview matrix (the identity matrix)
		modelViewMatrix2.PopMatrix();
	    modelViewMatrix2.PopMatrix();
	    // Do the buffer Swap
	    glutSwapBuffers();

	    // Tell GLUT to do it again
	    glutPostRedisplay();
	    }
#else
	char arg1[128],arg2[128];
		RenderSceneDS();
		glutSwapBuffers();
		glFinish();
		GetFPSDS();  /* Get frame rate stats */

		/* Copy saved window name into temp string arg1 so that we can add stats */
		strcpy (arg1, comSecondSC.getWindowName());

		if (sprintf(arg2, "%.2f FPS", comSecondSC.getFrameRate()))
		{
			strcat (arg1, arg2);
		}

		/* cut down on the number of redraws on window title.  Only draw once per sample*/
		if (comSecondSC.isCountUpdate())
		{
			glutSetWindowTitle(arg1);
		}
#endif
}

void Render::ReSizeGLSceneDS(int Width, int Height)
{
	if (Height==0)	/* Prevent A Divide By Zero If The Window Is Too Small*/
		Height=1;
		glViewport(0, 0, Width, Height);
	    // Create the projection matrix, and load it on the projection matrix stack
		viewFrustum2.SetPerspective(35.0f, float(Width)/float(Height), 1.0f, 100.0f);
		projectionMatrix2.LoadMatrix(viewFrustum2.GetProjectionMatrix());
	    // Set the transformation pipeline to use the two matrix stacks
		transformPipeline2.SetMatrixStacks(modelViewMatrix2, projectionMatrix2);
	comSecondSC.setUpdate(GL_YES);
}



void RenderMain::ReSizeGLSceneDS(int Width, int Height)
{
	render.ReSizeGLSceneDS(Width,Height);
}
void RenderMain::DrawGLSceneDS()
{

    static bool ONCE_FULLSCREEN = true;

	if(ONCE_FULLSCREEN){
		ONCE_FULLSCREEN = false;
		render.ProcessOitKeys('F', 0, 0);
	}
		render.DrawGLSceneDS();
//	render.DrawGLScene();
}
	void 	RenderMain::doubleScreenInit(int argc, char **argv)
	{
		char arg1[256], arg2[256];
		glutCreateSubWindow(2,0,0,500,200);
		glutInitWindowPosition(1921,0);
		glutInitWindowSize(1000, 700);

//		glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

		sprintf(arg1,"Second %s (%s, %s):",VERSION_STRING, __DATE__,__TIME__);
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
			comSecondSC.setWindowName(arg1);

			if (sprintf(arg2, "%.2f FPS", comSecondSC.getFrameRate()))
			{
				strcat (arg1, arg2);
			}

			glutCreateWindow (arg1);


		glutSetCursor(GLUT_CURSOR_NONE);
		glewInit();

			glutDisplayFunc(DrawGLSceneDS); /* Register the function to do all our OpenGL drawing. */
			glutIdleFunc(DrawIdle); /* Even if there are no events, redraw our gl scene. */
			glutReshapeFunc(ReSizeGLSceneDS); /* Register the function called when our window is resized. */
//			glutKeyboardFunc(keyPressed); /* Register the function called when the keyboard is pressed. */
//			glutSpecialFunc(specialkeyPressed); /* Register the special key function */
//			glutMouseFunc(mouseButtonPress); /* Register the function called when the mouse buttons are pressed */
//			glutMotionFunc(mouseMotionPress); /*Register the mouse motion function */
	}

