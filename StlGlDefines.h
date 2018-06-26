/*
 * StlGlDefines.h
 *
 *  Created on: Nov 3, 2016
 *      Author: hoover
 */

#ifndef STLGLDEFINES_H_
#define STLGLDEFINES_H_

#define CORE_AND_POS_LIMIT  4.5
#define INNER_RECT_AND_PANO_ONE_TIME_CAM_LIMIT 7.7
#define	OUTER_RECT_AND_PANO_TWO_TIMES_CAM_LIMIT 16.8
#define PER_CAM_ANGLE  36//(360/CAM_COUNT)

#define OSD_NAME_X	95//240.0
#define OSD_NAME_Y	1//998.0
#define OSD_NAME_W	380.0
#define OSD_NAME_H	480.0
typedef enum{
	CAM_0,
	CAM_1,
	CAM_2,
	CAM_3,
	CAM_4,
	CAM_5,
	CAM_6,
	CAM_7,
	CAM_8,
	CAM_9,
	CAM_10,
	CAM_11,
	CAM_TOTAL_COUNT
} CAM_TEXTURE_IDX;


#if !defined(CAM_COUNT)
#define CAM_COUNT CAM_TOTAL_COUNT
#else
#define PRINT_MACRO_HELPER(x) #x
#define PRINT_MACRO(x) #x"="PRINT_MACRO_HELPER(x)
#pragma message(PRINT_MACRO(CAM_COUNT) )
#endif
//跳过10路拼接的前两个  因为pbosend里面　idx 1, 2 被固定设置为2*3　2*2的纹理
// 所以跳过idx 1,2,设置为３则起始值则为3
#define MAGICAL_NUM 3
#if USE_GAIN
enum{
       RIGHT_ROI,
       LEFT_ROI,
       ROI_COUNT
};
#endif


#define NUM_OF_W 5
#define NUM_OF_H 2

#define FPGA_SCARE_X  (1920.0f/3200.0f)


#define THREAD_L_M_RENDER  99
#define THREAD_L_S_RENDER	99

#define THREAD_L_4  	50
#define THREAD_L_6		50
#define THREAD_L_M_1	50
#define THREAD_L_S_1		50
#define THREAD_L_MVDECT	50

#define THREAD_L_GST	1

enum
{
	MVDECT_ADD_CN=0,
	 FPGA_FOUR_CN  =1,
	 SUB_CN ,
	MAIN_CN ,
	 MVDECT_CN	,
	 FPGA_SIX_CN ,
	 MAX_CC
};

#define NUM2_OF_W 2
#define NUM2_OF_H 2
#define PARTITIONS1 10
#define PARTITIONS2 4
#define FPGA_SCREEN_WIDTH 1280
#define FPGA_SCREEN_HEIGHT 1080



#define DECTOR_TARGET_MAX 40
#define ALARM_MAX_COUNT 4
#define MAX_SCREEN_WIDTH 1920
#define MAX_SCREEN_HEIGHT 1080
#define PAL_WIDTH 720
#define PAL_HEIGHT 576

#define RULER_WIDTH	2160
#define RULER_HEIGHT	70
#define RULER_WIDTH_2	(2160/3)
#define RULER_HEIGHT_2	70
#if USE_12
#define TELXLIMIT 8.0
enum{
	FORESIGHT_POS_LEFT ,
	FORESIGHT_POS_MID ,
	FORESIGHT_POS_RIGHT ,

	TEL_FORESIGHT_POS_FRONT ,
	TEL_FORESIGHT_POS_RIGHT ,
	TEL_FORESIGHT_POS_BACK ,
	TEL_FORESIGHT_POS_LEFT ,

	TRACK_VGA,
	TRACK_SDI,
	TRACK_PAL,

	FORESIGHT_POS_COUNT
};

enum
{
	MAIN,
	SUB,
	MS_COUNT
};
enum
{
	MAIN_FPGA_FOUR,
	MAIN_ONE_OF_TEN,
	MAIN_1,
	MAIN_2,
	MAIN_3,
	MAIN_4,
	MAIN_5,
	MAIN_6,
	MAIN_7,
	MAIN_8,
	MAIN_9,
	MAIN_10,
	MAIN_FPGA_SIX,

	SUB_FPGA_FOUR,
	SUB_ONE_OF_TEN,
	SUB_FPGA_SIX,

	QUE_CHID_COUNT
};

#define   MOVE_UP  1
#define   MOVE_DOWN  2
#define   MOVE_LEFT  3
#define   MOVE_RIGHT 4
#define   MOVE_COUNT 4

enum{
    CHAN_GRAY =1,
	CHAN_YUV ,
};


#define TRACKFRAMEX  60
#define TRACKFRAMEY  60
enum{
	SEND_TRACK_START_X,  //LEFT UP
	SEND_TRACK_START_Y,
	SEND_TRACK_FRAME_X,
	SEND_TRACK_FRAME_Y,
	SEND_TRACK_NUM,
	SEND_COUNT
};

enum{
	RECV_TRACK_START_X,  //LEFT UP
	RECV_TRACK_START_Y,
	RECV_TRACK_FRAME_X,
	RECV_TRACK_FRAME_Y,
	RECV_TRACK_NUM
};
enum{
	CHOSEN_CAM_MAIN,
	CHOSEN_CAM_SUB,
	CHOSEN_CAM_COUNT
};


enum{
	SDI_CAM_0,
//	SDI_CAM_1,
	SDI_CAM_COUNT
};
enum{
	MAIN_TARGET_A0,
	MAIN_TARGET_A1,
	MAIN_TARGET_A2,
	MAIN_TARGET_A3,
	SUB_TARGET_A0,
	SUB_TARGET_A1,
	SUB_TARGET_A2,
	SUB_TARGET_A3,

	MAIN_TARGET_T0,
	MAIN_TARGET_T1,
	SUB_TARGET_T0,
	SUB_TARGET_T1,
	TARGET_CAM_COUNT
};

enum{
	VGA_CAM_0,
	VGA_CAM_COUNT
};
#endif

#define ROIW 400
#define ROIH	400
#define HALF_ROIW 200
#define HALF_ROIH	200



#define VGA_WIDTH 1024
#define VGA_HEIGHT 768

#define SDI_WIDTH 1920
#define SDI_HEIGHT 1080
#define FPGA_SINGLE_PIC_W 640
#define FPGA_SINGLE_PIC_H 540
#define VGA_DEV_NUM 5
#define SDI_DEV_NUM 4

#define VGA_DEV_chId 20
#define SDI_DEV_chId  16
#if !GSTREAM_CAP
#define SCREEN_W 1024
#define SCREEN_H 768
#else
#define SCREEN_W 1920
#define SCREEN_H 1080
#endif
#if  !GSTREAM_CAP
#define CURRENT_SCREEN_WIDTH 1920
#define CURRENT_SCREEN_HEIGHT 1080
#else
#define CURRENT_SCREEN_WIDTH 1920
#define CURRENT_SCREEN_HEIGHT 1080
#endif
#define PBO_ALTERNATE_NUM 2
enum{
	VGA_COUNT=1,
	SDI_COUNT=1,
	HD_CAM_COUNT
};

typedef enum
{
	MY_EMPTY_MODE,
	MY_ALL_VIEW_559_MODE
}myMode;
enum{
	VGA_CHAN_NUM =1,
	SDI_CHAN_NUM,
};

#define DS_CHAN_MAX         (4)

enum{
	MAIN_EXT_0,
	SUB_EXT_1,
	MAIN_AND_SUB_EXT_COUNT
};
enum{
	SUB_EXT_0,
	SUB_EXT_COUNT
};
enum{
	MVDECT_CAM_0,
	MVDECT_CAM_1,
	MVDECT_CAM_2,
	MVDECT_CAM_3,
	MVDECT_CAM_4,
	MVDECT_CAM_5,
	MVDECT_CAM_6,
	MVDECT_CAM_7,
	MVDECT_CAM_8,
	MVDECT_CAM_9,
	MVDECT_CAM_COUNT
};

enum{
	EXT_CAM_0,
	EXT_CAM_1,
	EXT_CAM_COUNT
};
extern int MODLE_CIRCLE;
extern int PER_CIRCLE  ;
extern int BLEND_OFFSET;
extern int LOOP_RIGHT;
extern int BLEND_HEAD;
extern int BLEND_REAR;
extern int ISDYNAMIC;
extern float VALUE_THRESHOLD;
#define FOV 30   // Field of view for perspective mode
/* ViewFlag arguments */
#define ORTHO 1
#define PERSPECTIVE 0
#define GL_YES 1
#define GL_NO 0

/* Other Constants */
#define PI 3.14159265358979323846

#define  H_EXPAND_TIMES  1.0/1.0
#define  W_EXPAND_TIMES  1.0/1.0

#if WHOLE_PIC
#define	DEFAULT_IMAGE_WIDTH  640
#define	DEFAULT_IMAGE_HEIGHT 5400
#define	PANO_TEXTURE_WIDTH 640
#define	PANO_TEXTURE_HEIGHT 5400
#define NUM_OF_W_ORI 1
#define  KX ((PANO_TEXTURE_WIDTH/NUM_OF_W)/	(MAX_SCREEN_WIDTH/NUM_OF_W_ORI))


#else
#define	DEFAULT_IMAGE_WIDTH  1920
#define	DEFAULT_IMAGE_HEIGHT 1080
#define	PANO_TEXTURE_WIDTH 1920
#define	PANO_TEXTURE_HEIGHT 1080
#endif


#define DEFAULT_IMAGE_DEPTH	 3

#define IMAGEWIDTH DEFAULT_IMAGE_WIDTH
#define IMAGEHEIGHT DEFAULT_IMAGE_HEIGHT
#define IMAGEBYTES (IMAGEWIDTH* IMAGEHEIGHT * DEFAULT_IMAGE_DEPTH)

#define DEFAULT_VEHICLE_SCALE             0.95f //1.3f //0.48f //1.6f //
#define DEFAULT_VEHICLE_SCALE_HEIGHT      0.85f //1.3f //0.48f //1.6f //
#define DEFAULT_VEHICLE_TRANSLATION_0     0.0f
#define DEFAULT_VEHICLE_TRANSLATION_1    0.2f
#define DEFAULT_VEHICLE_TRANSLATION_2     0.0f
#define DEFAULT_TRACK2_VEHICLE_WIDTH_RATE 0.95f
#define DEFAULT_TRACK_LENGTH_METER		  (1.0/24*26)
#define DEFAULT_FRONT_TRACK_LENGTH_METER  (3.0/24*26)

#define DEFAULT_SHADOW_TO_VEHICLE_RATE_WIDTH  1.1f //1.1f //2.1f //0.6f //
#define DEFAULT_SHADOW_TO_VEHICLE_RATE_LENGTH 0.9f //0.75f //0.5f //0.95f //
#define DEFAULT_SHADOW_TRANSLATE_LENGTH_METER -0.05f

static const float vBlack[] 	= { 0.0f, 0.0f, 0.0f, 1.0f };
static const float vWhite[] 	= { 1.0f, 1.0f, 1.0f, 1.0f};
static const float vLtBlue[]    = { 0.00f, 0.00f, 1.00f, 0.90f };
static const float vBlue[]    = { 0.00f, 0.00f, 1.00f, 0.90f };
static const float vLtPink[]    = { 0.40f, 0.00f, 0.20f, 0.50f };
static const float vLtYellow[]  = { 0.98f, 0.96f, 0.14f, 0.30f };
static const float vYellow[]    = { 0.98f, 0.96f, 0.00f, 1.00f };
static const float vLtMagenta[] = { 0.83f, 0.04f, 0.83f, 0.70f };
static const float vLtGreen[]   = {18/255.0,77/255.0,36/255.0,1.0f};//{ 0.05f, 1.0f, 0.14f, 0.90f };
static const float vGreen[]     = {0.0f, 255/255.0, 0.0f, 1.0f};
static const float vLtGrey[]    = { 0.85f, 0.85f, 0.85f, 0.9f };
static const float vGrey[]      = { 0.5f, 0.5f, 0.5f, 1.0f };
static const float vRed[]       = {109/255.0,10/255.0,12/255.0,1.0f};//{ 0.7f, 0.0f, 0.0f, 1.0f };
static const float vBRed[]       = {1.0,0.0,0.0,1.0f};//{ 0.7f, 0.0f, 0.0f, 1.0f };
static const float *DEFAULT_TEXT_COLOR = vWhite;


#define VERSION_STRING		       "v0.18"
#define DEFAULT_FRONT_TGA         "lable_00.tga"
#define DEFAULT_REAR_TGA           "lable_03.tga"
#define DEFAULT_FRONT_LEFT_TGA     "lable_05.tga"
#define DEFAULT_REAR_LEFT_TGA	   "lable_04.tga"
#define DEFAULT_FRONT_RIGHT_TGA    "lable_01.tga"
#define DEFAULT_REAR_RIGHT_TGA	   "lable_02.tga"

#define DEFAULT_COMPASS "lable_04.tga"//"compass.tga"

#define DEFAULT_EXT_WG712_TGA	"wg712.tga"

#define DEBUG_TGA "debug.tga"
#define TURRET_TGA			"paota.tga"
#define PANORAMIC_MIRROR_TGA "zhoushijing.tga"
#if USE_12
#define ONEX_REALTIME_TGA     "1X.tga"
#define TWOX_REALTIME_TGA     "2X.tga"
#define FOURX_REALTIME_TGA     "4X.tga"
#define ANGLE_TGA					"angle.tga" //潜望镜下方位抚养

#define LOCATION_TGA				"location.tga" //方位指示 模式
#define RADAR_FRONT_TGA   	"radar_front.tga"
#define RADAR_LEFT_TGA   	"radar_left.tga"
#define RADAR_RIGHT_TGA   	"radar_right.tga"
#define RADAR_BACK_TGA   	"radar_back.tga"

#define SDI1_WHITE_BIG_TGA  "sdi1_white_big.tga"
#define SDI1_WHITE_SMALL_TGA  "sdi1_white_small.tga"
#define SDI2_HOT_BIG_TGA  "sdi2_hot_big.tga"
#define SDI2_HOT_SMALL_TGA  "sdi2_hot_small.tga"

#define PAL1_WHITE_BIG_TGA  "pal1_white_big.tga"
#define PAL1_WHITE_SMALL_TGA  "pal1_white_small.tga"
#define PAL2_HOT_BIG_TGA  "pal2_hot_big.tga"
#define PAL2_HOT_SMALL_TGA  "pal2_hot_small.tga"

#define VGA_WHITE_BIG_TGA  "vga_white_big.tga"
#define VGA_WHITE_SMALL_TGA  "vga_white_small.tga"
#define VGA_HOT_BIG_TGA  "vga_hot_big.tga"
#define VGA_HOT_SMALL_TGA  "vga_hot_small.tga"

#define VGA_FUSE_WOOD_TGA  "vga_fuse_wood.tga"
#define VGA_FUSE_GRASS_TGA  "vga_fuse_grass.tga"
#define VGA_FUSE_SNOW_TGA  "vga_fuse_snow.tga"
#define VGA_FUSE_DESERT_TGA  "vga_fuse_desert.tga"
#define VGA_FUSE_CITY_TGA  "vga_fuse_city.tga"

#define	FINE_TGA						"fine.tga"
#define WRONG_TGA				"wrong.tga"
#define IDLE_TGA						"idle.tga"

#define CHOSEN_FRONT_LEFT_TGA "chosen_front_left.tga"
#define CHOSEN_FRONT_RIGHT_TGA "chosen_front_right.tga"

#define CHOSEN_BACK_LEFT_TGA "chosen_back_left.tga"
#define CHOSEN_BACK_RIGHT_TGA "chosen_back_right.tga"

#define CHOSEN_LEFT_FRONT_TGA "chosen_left_front.tga"
#define CHOSEN_LEFT_MID_TGA "chosen_left_mid.tga"
#define CHOSEN_LEFT_BACK_TGA "chosen_left_back.tga"

#define CHOSEN_RIGHT_FRONT_TGA "chosen_right_front.tga"
#define CHOSEN_RIGHT_MID_TGA "chosen_right_mid.tga"
#define CHOSEN_RIGHT_BACK_TGA "chosen_right_back.tga"

#define OSD_ALL_VIEW_NAME_TGA "osd_all_view_name.tga"
#define OSD_TEL_VIEW_NAME_TGA	"osd_tel_view_name.tga"
#define OSD_SINGLE_VIEW_NAME_TGA	"osd_single_view_name.tga"
#define OSD_HANCING_TGA		"osd_hancing.tga"
#define OSD_MVDETECTING_TGA		"osd_mvdetecting.tga"
#define OSD_RECORDING_TGA		"osd_recording.tga"

#define OSD_FAR_CAM_1_TGA	"osd_far_cam_1.tga"
#define OSD_FAR_CAM_2_TGA	"osd_far_cam_2.tga"
#define OSD_FAR_CAM_3_TGA	"osd_far_cam_3.tga"
#define OSD_FAR_CAM_4_TGA	"osd_far_cam_4.tga"
#define OSD_FAR_CAM_5_TGA	"osd_far_cam_5.tga"
#define OSD_FAR_CAM_6_TGA	"osd_far_cam_6.tga"
#define OSD_FAR_CAM_7_TGA	"osd_far_cam_7.tga"
#define OSD_FAR_CAM_8_TGA	"osd_far_cam_8.tga"
#define OSD_FAR_CAM_9_TGA	"osd_far_cam_9.tga"
#define OSD_FAR_CAM_10_TGA	"osd_far_cam_10.tga"
#define OSD_NEAR_CAM_1_TGA	"osd_near_cam_1.tga"
#define OSD_NEAR_CAM_2_TGA	"osd_near_cam_2.tga"
#define  OSD_NEAR_CAM_3_TGA	"osd_near_cam_3.tga"
#define OSD_NEAR_CAM_4_TGA	"osd_near_cam_4.tga"
#define OSD_SCREEN_TGA	"osd_screen.tga"
#define OSD_MESSAGE_TGA	"osd_message.tga"
#define OSD_GOOD_TGA	"osd_good.tga"
#define OSD_ERROR_TGA	"osd_error.tga"
#define OSD_ROI_TGA		"osd_roi.tga"

typedef enum CURRENT_MODE
{
	CURRENT_CHECK_MYSELF,
	CURRENT_ALL_VIEW_MODE,
	CURRENT_CHOSEN_VIEW_MODE,
	CURRENT_TELESCOPE_RIGHT_MODE,
	CURRENT_TELESCOPE_LEFT_MODE,
	CURRENT_TELESCOPE_FRONT_MODE,
	CURRENT_TELESCOPE_BACK_MODE,
	CURRENT_TRIM_MODE,
	CURRENT_SECOND_559_ALL_VIEW_MODE,
	CURRENT_SECOND_CHOSEN_VIEW_MODE,
	CURRENT_SECOND_TELESCOPE_FRONT_MODE,
	CURRENT_SECOND_TELESCOPE_RIGHT_MODE,
	CURRENT_SECOND_TELESCOPE_BACK_MODE,
	CURRENT_SECOND_TELESCOPE_LEFT_MODE,
	CURRENT_FBO_ALL_VIEW_559_MODE,
	 CURRENT_COUNT
}CurrentMode;
enum CCT_COUNT {
		ONEX_REALTIME_T ,
		TWOX_REALTIME_T,
		FOURX_REALTIME_T,
		ANGLE_T,
		LOCATION_T,
		RADAR_FRONT_T,
		RADAR_LEFT_T,
		RADAR_RIGHT_T,
		RADAR_BACK_T,

		SDI1_WHITE_BIG_T,
		SDI1_WHITE_SMALL_T,
		SDI2_HOT_BIG_T,
		SDI2_HOT_SMALL_T,

		PAL1_WHITE_BIG_T,
		PAL1_WHITE_SMALL_T,
		PAL2_HOT_BIG_T,
		PAL2_HOT_SMALL_T,

		VGA_WHITE_BIG_T,
		VGA_WHITE_SMALL_T,
		VGA_HOT_BIG_T,
		VGA_HOT_SMALL_T,

		VGA_FUSE_WOOD_T,
		VGA_FUSE_GRASS_T,
		VGA_FUSE_SNOW_T,
		VGA_FUSE_DESERT_T,
		VGA_FUSE_CITY_T,

		FINE_T,
		WRONG_T,
		IDLE_T,

		TURRET_T,
		PANORAMIC_MIRROR_T,

		DEBUG_T,

 CHOSEN_RIGHT_FRONT_T,
 CHOSEN_FRONT_RIGHT_T,
 CHOSEN_FRONT_LEFT_T,
 CHOSEN_LEFT_FRONT_T,
 CHOSEN_LEFT_MID_T,
 CHOSEN_LEFT_BACK_T,
 CHOSEN_BACK_LEFT_T,
 CHOSEN_BACK_RIGHT_T,
 CHOSEN_RIGHT_BACK_T,
 CHOSEN_RIGHT_MID_T,

 OSD_ALL_VIEW_NAME_T,
 OSD_TEL_VIEW_NAME_T,
 OSD_SINGLE_VIEW_NAME_T,
 OSD_HANCING_T,
 OSD_MVDETECTING_T,
 OSD_RECORDING_T,
 OSD_FAR_CAM_1_T,
 OSD_FAR_CAM_2_T,
 OSD_FAR_CAM_3_T,
 OSD_FAR_CAM_4_T,
 OSD_FAR_CAM_5_T,
 OSD_FAR_CAM_6_T,
 OSD_FAR_CAM_7_T,
 OSD_FAR_CAM_8_T,
 OSD_FAR_CAM_9_T,
 OSD_FAR_CAM_10_T,
 OSD_NEAR_CAM_1_T,
  OSD_NEAR_CAM_2_T,
  OSD_NEAR_CAM_3_T,
  OSD_NEAR_CAM_4_T,
  OSD_SCREEN_T,
  OSD_MESSAGE_T,
  OSD_GOOD_T,
  OSD_ERROR_T,
  OSD_ROI_T,
		CCT_COUNT
		};

typedef enum TelBreak
{
	TEL_LEFT_BREAK,
	TEL_RIGHT_BREAK,
	TEL_NOT_BREAK
}telBreak;

enum Direction
{
	FRONT,
	RIGHT,
	BACK,
	LEFT
};
typedef enum Fore_Sight_Constitute
{
	pano_cross,
	WHOLE_IN_FRAME,
	WHOLE_OUT_FRAME,
	TEL_CROSS,
	TEL_IN_FRAME,
	TEL_OUT_FRAME,
	FS_CONSTITUTE_COUNT
}fsConstitute;


typedef enum Recv_Show_Mode
{
	RECV_VGA_WHITE_MODE = 0x01,
	RECV_VGA_WHITE_SMALL_USELESS_MODE,
	RECV_VGA_HOT_BIG_MODE ,
	RECV_VGA_HOT_SMALL_MODE,
	RECV_VGA_FUSE_WOOD_MODE ,
	RECV_VGA_FUSE_GRASS_MODE ,
	RECV_VGA_FUSE_SNOW_MODE ,
	RECV_VGA_FUSE_DESERT_MODE,
	RECV_VGA_FUSE_CITY_MODE,
	RECV_WHOLE_MODE,
	RECV_TELESCOPE_MODE,
	RECV_SDI1_WHITE_BIG_MODE,
	RECV_SDI1_WHITE_SMALL_MODE,
	RECV_SDI2_HOT_BIG_MODE,
	RECV_SDI2_HOT_SMALL_MODE,
	RECV_PAL1_WHITE_BIG_MODE,
	RECV_PAL1_WHITE_SMALL_MODE,
	RECV_PAL2_HOT_BIG_MODE,
	RECV_PAL2_HOT_SMALL_MODE,
	RECV_ENABLE_TRACK,
	RECV_SHOWMODE_COUNT
}RECV_MODE;



enum TRACK_VIDEO_CHID{
	SDI_TRACK=0,
	VGA_TRACK,
	PAL_TRACK,
	PAL2_TRACK};
/*
enum Recv_Fuse_Choice
{
	RECV_FUSE_WOODLAND =0x00,
	RECV_FUSE_GRASSLAND,
	RECV_FUSE_SNOWFIELD,
	RECV_FUSE_DESERT,
	RECV_FUSE_CITY,
	RECV_FUSECHOICE_COUND
};



enum Recv_Work_State
{
	RECV_WHITE_BIG=0x00,
	RECV_WHITE_SMALL,
	RECV_RED_BIG,
	RECV_RED_SMALL,
	RECV_WORKSTATE_COUNT
};
*/

typedef enum SelfCheck_State
{
	SELFCHECK_IDLE,
	SELFCHECK_PASS,
	SELFCHECK_FAIL,
	SELFCHECK_STATE_COUNT
}self_check_state;

typedef enum SelfCheck_Item
{
	CHECK_ITEM_CAPTURE,
	CHECK_ITEM_COMMUNICATION,
	CHECK_ITEM_BOARD,
	SELFCHECK_ITEM_COUNT
}self_check_item;


#define insideDH 2.45
#define insideUH 3.54/6
#define insideLen  200

#define outsideLen 100
#define outsideUH 2/3
#define outsideDH 3

#define inside4DH 2.225
#define inside4UH 1.82
#define inside4LW
#define inside4RW

#endif


#define DEFAULT_FIXED_BBD_HEIGHT 1.0f
#define DEFAULT_FIXED_BBD_WIDTH  1.0f
#define DDS_FILE_1M "1m.dds"
#define DDS_FILE_2M "2m.dds"
#define DDS_FILE_5M "5m.dds"
#define DDS_FILE_8M "8m.dds"

#define BIRD_VIEW_WIDTH_RATE (0.45/*1.0*3/7*/)
#define BIRD_VIEW_HEIGHT_RATE (1)
#define OTHER_VIEW_WIDTH_RATE (1-BIRD_VIEW_WIDTH_RATE)
#define OTHRE_VIEW_HEIGHT_RATE (1.0*1/4)


//dynamic track params in meters
#define DEFAULT_DYNAM_TRACK_COUNT 4
#define DEFAULT_WHEEL_DIST	3.2f
#define DEFAULT_AXLE_DIST	6.5f
#define DEFAULT_ARC_WIDTH	0.0f
#define DEFAULT_ARC_WIDTH_INI	0.0f
#define DEFAULT_ARC_WIDTH_SET	0.2f
#define DEFAULT_DELTA_ANGLE	50.0f
#define DEFAULT_REAR_TIRE_OFFSET (DEFAULT_WHEEL_DIST/2 + 1.5f)*(DEFAULT_VEHICLE_SCALE_HEIGHT)
#define DEGREE2RAD(angle) ((angle)*PI/180.0f)
#define RAD2DEGREE(rad)		((rad)*180.0f/PI)

//follow cross
#define CROSS_WIDTH 0.3
#define CROSS_HEIGHT 0.3
#define CROSS_MARGIN_WIDTH 0.05
#define CROSS_MARGIN_HEIGHT 0.05
#define CROSS_HOR_STEP 0.05
#define CROSS_VER_STEP 0.05
#define CROSS_VER_UP_SCALE 0.85
#define CROSS_VER_DOWN_SCALE 0.15


#define INVALID_DIRECTION -1

//sndmsg
#define IPC_SERVER 0xF001
#define IPC_CLIENT 0xF002
#define IPC_FOCAL   0xF003
#define IPC_LASER    0xF004



//corners definitions
typedef enum{
	CORNER_FRONT_LEFT,
	CORNER_FRONT_RIGHT,
	CORNER_REAR_LEFT,
	CORNER_REAR_RIGHT,
    CORNER_COUNT
} CORNER_POSITION;

typedef enum{
	RULER_45=0,
	RULER_90,
	RULER_180,
	RULER_45_samll,
	RULER_90_small,
	RULER_180_small,
	RULER_COUNT
}RULER_ANGLE;
enum{
	ICON_45DEGREESCALE,
	ICON_90DEGREESCALE,
	ICON_180DEGREESCALE,
	CHOSEN_PIC_MAIN,
	CHOSEN_PIC_SUB,
	ICON_45_small_DEGREESCALE,
	ICON_90_small_DEGREESCALE,
	ICON_180_small_DEGREESCALE,
	ICON_COUNT
};
#define MAX_PANO_CAMERA_COUNT 16
#define PANO_SCALE_ARRAY_FILE "pano_scale_array_data.yml"

#define USE_COMPASS_ICON 0
#endif /* STLGLDEFINES_H_ */
