#include "HDV4lcap.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/ocl.hpp>
#include "StlGlDefines.h"
#include "thread.h"
#include <osa_buf.h>
#include "buffer.h"
#include"StlGlDefines.h"
#include <osa_sem.h>
#if TRACK_MODE
#include "VideoProcessTrack.hpp"
#endif

#include"MvDetect.hpp"

extern Alg_Obj * queue_main_sub;
#define MEMCPY memcpy

#define INPUT_IMAGE_WIDTH 1920
#define INPUT_IMAGE_HEIGHT 1080

using namespace std;
using namespace cv::ocl;
static bool Once_buffer=true;
int m_bufId[QUE_CHID_COUNT]={0};
extern void DeinterlaceYUV_Neon(unsigned char *lpYUVFrame, int ImgWidth, int ImgHeight, int ImgStride);
//Mat SDI_frame,VGA_frame;
//unsigned char * sdi_data_main[6];
//unsigned char * sdi_data_sub[6];

//static HDv4l_cam hdv4lcap(0,SDI_WIDTH,SDI_HEIGHT);

HDv4l_cam_Producer hdCamProducer;




unsigned char * select_bgr_data_sub=NULL;
unsigned char * select_bgr_data_main=NULL;
unsigned char * FPGA6_bgr_data_sub=NULL;
unsigned char * FPGA6_bgr_data_main=NULL;
unsigned char * FPGA4_bgr_data_sub=NULL;
unsigned char * FPGA4_bgr_data_main=NULL;
unsigned char * GRAY_data_sub[CAM_COUNT];
unsigned char * GRAY_data_main[CAM_COUNT];

unsigned char * vga_data=NULL;


extern MvDetect mv_detect;
HDv4l_cam::HDv4l_cam(int devId,int width,int height):io(IO_METHOD_MMAP/*IO_METHOD_MMAP*/),imgwidth(width),
imgheight(height),buffers(NULL),memType(MEMORY_NORMAL),cur_CHANnum(0),
force_format(1),m_devFd(-1),n_buffers(0),bRun(false),Id(devId),BaseVCap()
{
		imgformat 	= V4L2_PIX_FMT_YUYV;
		sprintf(dev_name, "/dev/video%d",devId);
			imgstride 	= imgwidth*2;
			bufSize 	= imgwidth * imgheight * 2;
			imgtype     = CV_8UC2;
			memType = MEMORY_NORMAL;
			bufferCount = 6;
			if(Once_buffer)
			{
				init_buffer();
				Once_buffer=false;
			}
}

HDv4l_cam::~HDv4l_cam()
{
	stop_capturing();
	uninit_device();
	close_device();
}

void YUYV2UYVx(unsigned char *ptr,unsigned char *Yuyv, int ImgWidth, int ImgHeight)
{
	for(int j =0;j<ImgHeight;j++)
	{
		for(int i=0;i<ImgWidth*2/4;i++)
		{
			*(ptr+j*ImgWidth*4+i*8+1)=*(Yuyv+j*ImgWidth*2+i*4);
			*(ptr+j*ImgWidth*4+i*8+0)=*(Yuyv+j*ImgWidth*2+i*4+1);
			*(ptr+j*ImgWidth*4+i*8+2)=*(Yuyv+j*ImgWidth*2+i*4+3);
			*(ptr+j*ImgWidth*4+i*8+3)=0;

			*(ptr+j*ImgWidth*4+i*8+5)=*(Yuyv+j*ImgWidth*2+i*4+2);
			*(ptr+j*ImgWidth*4+i*8+4)=*(Yuyv+j*ImgWidth*2+i*4+1);
			*(ptr+j*ImgWidth*4+i*8+6)=*(Yuyv+j*ImgWidth*2+i*4+3);
			*(ptr+j*ImgWidth*4+i*8+7)=0;
		}
	}
}

void HDv4l_cam::YUYV2RGB(unsigned char * src,unsigned char * dst,int w,int h)
{
	bool enhance=false;
	if(w==MAX_SCREEN_WIDTH)//６副图
	{
		Mat Src(h,w,CV_8UC4,src);
		Mat Dst(h,w,CV_8UC3,dst);
		cvtColor(Src,Dst,CV_YUV2BGR_YUYV);
	}
	else if (w==FPGA_SCREEN_WIDTH) //4副先进行切割
	{
		Mat Src(SDI_HEIGHT,SDI_WIDTH,CV_8UC4,src);
		Rect rect(0,0,w,h);
		Mat Roi=Src(rect);
		Mat Dst(h,w,CV_8UC3,dst);
		cvtColor(Src,Roi,CV_YUV2BGR_YUYV);
		//如果w=1280 h=1080,则进行截取
		//否则直接转换
	}
	else
	{
		printf("there is no width like this :%d!\n",w);
		assert(false);
	}
	if(enhance)
	{
//todo
	}
}

void HDv4l_cam::YUYV2GRAY(unsigned char * src,unsigned char * dst,int w,int h)
{
	for(int j = 0;j<h*w;j++)
	{
			dst[j] = src[2*j +1];
	}
	return ;
}

void save_SDIyuyv_pic(void *pic)
{
	FILE * fp;
	fp=fopen("./Van_save_YUV.yuv","w");
	fwrite(pic,1920*1080*2,1,fp);
	fclose(fp);
}

bool HDv4l_cam::Open()
{
	int ret;
/*	setCurChannum();
	if(ret < 0)
			return false;
*/
	static bool Once=true;
	if(Once)
	{
		for(int i=1;i<6;i++)
		{
		//	sdi_data_main[i]=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*2);
	//		sdi_data_sub[i]=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*2);
		}
		for(int i=0;i<10;i++)
		{
			GRAY_data_sub[i]=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*1);
			GRAY_data_main[i]=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*1);
		}
		select_bgr_data_sub=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*3);
		select_bgr_data_main=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*3);
		FPGA6_bgr_data_sub=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*3);
		FPGA6_bgr_data_main=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*3);
		FPGA4_bgr_data_sub=(unsigned char *)malloc(FPGA_SCREEN_WIDTH*FPGA_SCREEN_HEIGHT*2);
		FPGA4_bgr_data_main=(unsigned char *)malloc(FPGA_SCREEN_WIDTH*FPGA_SCREEN_HEIGHT*2);
		Once=false;
	}
	 start_queue(queue_main_sub);
	ret = open_device();
	if(ret < 0)
		return false;

	ret = init_device();
	if(ret < 0)
		return false;


	run();
	return true;
}

void HDv4l_cam::run()
{
	start_capturing();


	bRun = true;
}

void HDv4l_cam::stop()
{
	stop_capturing();
	bRun = false;
}


int HDv4l_cam::open_device(void)
{
	struct stat st;

	if (-1 == stat(dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n", dev_name, errno, strerror(errno));
			return FAILURE_DEVICEOPEN;
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", dev_name);
		return FAILURE_DEVICEOPEN;
	}

	m_devFd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);
	printf("opened_dev_name=%s\n",dev_name);
	if (-1 == m_devFd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno, strerror(errno));
		return FAILURE_DEVICEOPEN;
	}
	return EXIT_SUCCESS;
}




int HDv4l_cam::init_device(void)
{
	struct v4l2_capability cap;
		struct v4l2_cropcap cropcap;
		struct v4l2_crop crop;
		struct v4l2_format fmt;
		unsigned int min;

		if (-1 == xioctl(m_devFd, VIDIOC_QUERYCAP, &cap)) {
			if (EINVAL == errno) {
				fprintf(stderr, "%s is no V4L2 device\n", dev_name);
				return FAILURE_DEVICEINIT;
			} else {
				errno_exit("HD_VIDIOC_QUERYCAP");
			}
		}

		if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
			fprintf(stderr, "%s is no video capture device\n", dev_name);
			return FAILURE_DEVICEINIT;
		}

		switch (io) {
		case IO_METHOD_READ:
			if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
				fprintf(stderr, "%s does not support read i/o\n", dev_name);
				return FAILURE_DEVICEINIT;
			}
			break;
		case IO_METHOD_MMAP:
		case IO_METHOD_USERPTR:
			if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
				fprintf(stderr, "%s does not support streaming i/o\n", dev_name);
				return FAILURE_DEVICEINIT;
			}
			break;
		}

		/* Select video input, video standard and tune here. */

		CLEAR(cropcap);

		cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		if (0 == xioctl(m_devFd, VIDIOC_CROPCAP, &cropcap)) {
			crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			crop.c = cropcap.defrect; /* reset to default */

			if (-1 == xioctl(m_devFd, VIDIOC_S_CROP, &crop)) {
			}
		} else {
			/* Errors ignored. */
		}

		CLEAR(fmt);

		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (force_format) {
			fprintf(stderr, "Set HDyuyv\r\n");
			fmt.fmt.pix.width       = imgwidth; //replace
			fmt.fmt.pix.height      = imgheight; //replace
			fmt.fmt.pix.pixelformat = imgformat;// V4L2_PIX_FMT_YUYV   (HD);
			fmt.fmt.pix.field       = V4L2_FIELD_ANY;
			//fmt.fmt.pix.code=0;
			//printf("******width =%d height=%d\n",fmt.fmt.pix.width,fmt.fmt.pix.height);

			if (-1 == xioctl(m_devFd, VIDIOC_S_FMT, &fmt))
			{
				 errno_exit("HD_VIDIOC_S_FMT");
			}

			/* Note VIDIOC_S_FMT may change width and height. */
		} else {
			/* Preserve original settings as set by v4l2-ctl for example */
			if (-1 == xioctl(m_devFd, VIDIOC_G_FMT, &fmt))
				errno_exit("HD_VIDIOC_G_FMT");
		}


		/* Buggy driver paranoia. */
		min = fmt.fmt.pix.width * 2;
		if (fmt.fmt.pix.bytesperline < min)
			fmt.fmt.pix.bytesperline = min;
		min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
		if (fmt.fmt.pix.sizeimage < min)
			fmt.fmt.pix.sizeimage = min;

		switch (io) {
		case IO_METHOD_READ:
			init_read(fmt.fmt.pix.sizeimage);
			break;
		case IO_METHOD_MMAP:
			init_mmap();
			break;
		case IO_METHOD_USERPTR:
			init_userp(fmt.fmt.pix.sizeimage);
			break;
		}
		return EXIT_SUCCESS;
}




void HDv4l_cam::errno_exit(const char *s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

int HDv4l_cam::xioctl(int fh, int request, void *arg)
{
	int ret;
	do {
		ret = ioctl(fh, request, arg);
	} while (-1 == ret && EINTR == errno);

	return ret;
}

void yuyv2gray(unsigned char* src,unsigned char* dst,int width,int height)
{
	for(int j = 0;j<height*width;j++)
	{
			dst[j] = src[2*j +1];
	}
	return ;
}

int HDv4l_cam::GetNowPicIdx()
{
	int picIdx=-1;
	//todo
	return picIdx;
}
int HDv4l_cam::ChangeIdx2chid(int idx)
{//0~9
	int picidx=(GetNowPicIdx()+2+idx*13);
	return picidx;
}



int HDv4l_cam::read_frame(int now_pic_format)
{
	struct v4l2_buffer buf;
	int i=0;
	static int  count=0;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

		if (-1 == xioctl(m_devFd, VIDIOC_DQBUF, &buf)) {
					switch (errno) {
					case EAGAIN:
						return -1;
					case EIO:
					//	printf("EIO~~~~~~~~~~~");
						/* Could ignore EIO, see spec. */
						/* fall through */
					default:
						errno_exit("VIDIOC_DQBUF");
					}
				}
				assert(buf.index < n_buffers);
			 if (buffers[buf.index].start!=NULL)
			{
				 if(now_pic_format==0)
				 {
					 printf("now_pic_format=0,0 dev is not used!\n");
					 assert(false);
				 }
					int chid[2]={-1,-1};
					int nowGrayidx=-1;
					int nowpicW=SDI_WIDTH,nowpicH=SDI_HEIGHT;
					unsigned char *transformed_src_main=NULL;
					unsigned char *transformed_src_sub=NULL;
					switch(now_pic_format)
					{
					case FPGA_FOUR_CN:
						chid[MAIN]=MAIN_FPGA_FOUR;
						chid[SUB]=SUB_FPGA_FOUR;
						nowpicW=FPGA_SCREEN_WIDTH;
						nowpicH=FPGA_SCREEN_HEIGHT;
						transformed_src_main=FPGA4_bgr_data_main;
						transformed_src_sub=FPGA4_bgr_data_sub;
						break;
					case SUB_CN:
						chid[SUB]=SUB_ONE_OF_TEN;
						transformed_src_sub=select_bgr_data_sub;
						break;
					case MAIN_CN:
						chid[MAIN]=MAIN_ONE_OF_TEN;
						transformed_src_main=select_bgr_data_main;
						break;
					case MVDECT_CN:
						chid[MAIN]=ChangeIdx2chid(MAIN);
						chid[SUB]=ChangeIdx2chid(SUB);
						nowGrayidx=GetNowPicIdx();
						transformed_src_main=GRAY_data_main[nowGrayidx];
						transformed_src_sub=GRAY_data_sub[nowGrayidx];
						break;
					case FPGA_SIX_CN:
						chid[MAIN]=MAIN_FPGA_SIX;
						chid[SUB]=SUB_FPGA_SIX;
						transformed_src_main=FPGA6_bgr_data_main;
						transformed_src_sub=FPGA6_bgr_data_sub;
						break;
					default:
						break;
					}
					if(chid[MAIN]!=-1) //车长
					{
						if(Data2Queue(queue_main_sub,transformed_src_main,nowpicW,nowpicH,chid[MAIN]))
						{
							if(getEmpty(queue_main_sub,&transformed_src_main, chid[MAIN]))
							{
								if(now_pic_format==MVDECT_CN)//移动检测
								{
									YUYV2GRAY((unsigned char *)buffers[buf.index].start,transformed_src_main,SDI_WIDTH,SDI_HEIGHT);
								}
								else //４副　６副　　车长１０选一
								{
									YUYV2RGB((unsigned char *)buffers[buf.index].start,transformed_src_main,nowpicW,nowpicH);
								}
							}
						}
					}
					if(chid[SUB]!=-1)//驾驶员
					{
						if(Data2Queue(queue_main_sub,transformed_src_sub,nowpicW,nowpicH,chid[MAIN]))
						{
							if(getEmpty(queue_main_sub,&transformed_src_sub, chid[MAIN]))
							{
								if(now_pic_format==SUB_CN)//如果等于驾驶员十选一，则要进行rgb转换
								{
									YUYV2RGB((unsigned char *)buffers[buf.index].start,transformed_src_sub,nowpicW,nowpicH);
								}
								else if(now_pic_format==MVDECT_CN)//移动检测
								{
									//拷贝gray数据
									memcpy(transformed_src_sub,transformed_src_main,nowpicW*nowpicH*1);
								}
								else//如果不等于驾驶员十选一＆不等于检测的gray数据，则直接将main里的已经转换好的数据进行拷贝
								{
									memcpy(transformed_src_sub,transformed_src_main,nowpicW*nowpicH*2);
								}
							}
						}
					}


#if 0
					OSA_semWait(&sem[now_pic_format],OSA_TIMEOUT_FOREVER);
							currentWR[now_pic_format]=1;
							if(currentWR[now_pic_format]==0) //０的时候线程里在写
							{
								printf("%d OSA_sem failed\n",now_pic_format);
							}
							memcpy(sdi_data[now_pic_format],buffers[buf.index].start,SDI_WIDTH*SDI_HEIGHT*2);
				//			unsigned char* abcd = (unsigned char*)malloc(1920*1080*1);
				//			yuyv2gray((unsigned char *)buffers[buf.index].start,abcd,1920,1080);
			//				Mat tmp = Mat(1080,1920,CV_8UC1,abcd);
			//				imshow("abcd",tmp);
			//				waitKey(0);
						//	mvDetect(uchar index,uchar* inframe,int width,int height,cv::Rect *boundRect);
						//	mvDetect((unsigned char)now_pic_format,*(unsigned char *) buffers[buf.index].start,1920,1080,Rect *boundRect);
							//index 代表第几个 检测OBJ 执行，boundRect 输出 目标的矩形框参数
							#if MVDECT
							mv_detect.m_mvDetect(now_pic_format,(unsigned char*)sdi_data[now_pic_format]);
							#endif
							OSA_semSignal(&sem[now_pic_format]);
#endif
			}
					if (-1 ==xioctl(m_devFd, VIDIOC_QBUF, &buf)){
						fprintf(stderr, "VIDIOC_QBUF error %d, %s\n", errno, strerror(errno));
						exit(EXIT_FAILURE);
					}
	return 0;
}

void HDv4l_cam::stop_capturing(void)
{
	enum v4l2_buf_type type;
		switch (io) {
	//	case IO_METHOD_READ:
	//		/* Nothing to do. */
	//		break;
		case IO_METHOD_MMAP:
		case IO_METHOD_USERPTR:
			type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			if (-1 == xioctl(m_devFd, VIDIOC_STREAMOFF, &type))
				errno_exit("VIDIOC_STREAMOFF");
			break;
		}
}

void HDv4l_cam::start_capturing(void)
{
	unsigned int i;
	enum v4l2_buf_type type;

	switch (io) {
//	case IO_METHOD_READ:
//		/* Nothing to do. */
//		break;
	case IO_METHOD_MMAP:
		for (i = 0; i < n_buffers; ++i) {
			struct v4l2_buffer buf;

			CLEAR(buf);
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.index = i;

			if (-1 == xioctl(m_devFd, VIDIOC_QBUF, &buf))
				errno_exit("HD_VIDIOC_QBUF");
		}
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(m_devFd, VIDIOC_STREAMON, &type))
			errno_exit("HD_VIDIOC_STREAMON");
		break;
	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i) {
			struct v4l2_buffer buf;

			CLEAR(buf);
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_USERPTR;
			buf.index = i;
			buf.m.userptr = (unsigned long)buffers[i].start;
			buf.length = buffers[i].length;

			if (-1 == xioctl(m_devFd, VIDIOC_QBUF, &buf))
				errno_exit("HD_VIDIOC_QBUF");
		}
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(m_devFd, VIDIOC_STREAMON, &type))
			errno_exit("HD_VIDIOC_STREAMON");
		break;
	default:
		break;
	}
}

void HDv4l_cam::uninit_device(void)
{
	unsigned int i;

	switch (io) {
	case IO_METHOD_READ:
		free(buffers[0].start);
		break;
	case IO_METHOD_MMAP:
		for (i = 0; i < n_buffers; ++i)
			if (-1 == munmap(buffers[i].start, buffers[i].length))
				errno_exit("munmap");
		break;
	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i){
			if(memType == MEMORY_NORMAL){
				free(buffers[i].start);
			}else{
					cudaFreeHost(buffers[i].start);
						}
		}
		break;
	default:
		break;
	}

	free(buffers);
}

void HDv4l_cam::init_read(unsigned int buffer_size)
{
	buffers = (struct buffer *)calloc(1, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	buffers[0].length = buffer_size;
	buffers[0].start = (struct buffer *) malloc(buffer_size);

	if (!buffers[0].start) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
}

void HDv4l_cam::init_mmap(void)
{
	struct v4l2_requestbuffers req;

	CLEAR(req);

	req.count = bufferCount;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(m_devFd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}

	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n", dev_name);
		exit(EXIT_FAILURE);
	}

//	printf("%s qbuf cnt = %d\n", dev_name, req.count);

	buffers = (struct buffer *)calloc(req.count, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;

		if (-1 == xioctl(m_devFd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start  =	mmap(NULL /* start anywhere */,
										buf.length,
										PROT_READ | PROT_WRITE /* required */,
										MAP_SHARED /* recommended */,
										m_devFd, buf.m.offset);
		memset(buffers[n_buffers].start,0x80,buf.length);

		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit("mmap");
	}
}

void HDv4l_cam::init_userp(unsigned int buffer_size)
{
	struct v4l2_requestbuffers req;
	cudaError_t ret = cudaSuccess;
	CLEAR(req);

	req.count  = bufferCount;//  different
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;

	if (-1 == xioctl(m_devFd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
					"user pointer i/o\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}
	if (req.count < 2) {
			fprintf(stderr, "Insufficient buffer memory on %s\n", dev_name);
			exit(EXIT_FAILURE);
		}
//	printf("%s qbuf cnt = %d\n", dev_name, req.count);
	buffers = (struct buffer  *)calloc(req.count, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		buffers[n_buffers].length = buffer_size;
		if(memType == MEMORY_NORMAL)
			buffers[n_buffers].start = malloc(buffer_size);
	else // MEMORY_LOCKED
			 ret = cudaHostAlloc(&buffers[n_buffers].start, buffer_size, cudaHostAllocDefault);
			assert(ret == cudaSuccess);
		//cudaFreeHost();

		if (!buffers[n_buffers].start) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}
	}
}


void HDv4l_cam::close_device(void)
{
	if (-1 == close(m_devFd))
		errno_exit("close");

	m_devFd = -1;
}

void HDv4l_cam::mainloop(int now_pic_format)
{

	fd_set fds;
	struct timeval tv;
	int ret;

		FD_ZERO(&fds);
		FD_SET(m_devFd, &fds);

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		ret = select(m_devFd + 1, &fds, NULL, NULL, &tv);

		if (-1 == ret)
		{
			if (EINTR == errno)
				return;

			errno_exit("select");
		}else if (0 == ret)
		{
			fprintf(stderr, "select timeout\n");
			exit(EXIT_FAILURE);
		}
		if (-1 == read_frame(now_pic_format))  /* EAGAIN - continue select loop. */
			return;
}


//--------------the decorator cap class------------
HDAsyncVCap4::HDAsyncVCap4(auto_ptr<BaseVCap> coreCap,int now_format):
	m_core(coreCap),thread_state(THREAD_READY),sleepMs(DEFAULT_THREAD_SLEEP_MS)
{
	pic_format=now_format;
	initLock();
	start_thread(capThread,this);
	cout<<" HDstart a capture thread"<<endl;
}
HDAsyncVCap4:: ~HDAsyncVCap4()
{
	Close();
}
bool HDAsyncVCap4::Open()
{
	if(thread_state != THREAD_READY)
		return false;//do not support reopen.
	bool isOpened = m_core->Open();
	if(isOpened){
		Start();
	}
	return isOpened;
}

void HDAsyncVCap4::Close()
{
	thread_state = THREAD_STOPPING;
	while( THREAD_IDLE != thread_state){
		usleep(100*1000);
	}
	m_core->Close();
}


void HDAsyncVCap4::Capture(char* ptr)
{
	lock_read(ptr);
}

void HDAsyncVCap4::SetDefaultImg(char *p)
{
	m_core->SetDefaultImg(p);
}

 void HDAsyncVCap4::SavePic(const char* name)
 {
	m_core->SavePic(name);
 }

void HDAsyncVCap4::Run()
{

	do{
		usleep(100*1000);
	}while(THREAD_READY == thread_state);
	//cap in background thread
	while(thread_state == THREAD_RUNNING)
	{
		HDv4l_cam * pcore = dynamic_cast<HDv4l_cam*>(m_core.get());
		if(pcore){
			pcore->mainloop(pic_format);
		}
//		usleep(sleepMs*1000);
	}
	thread_state = THREAD_IDLE;
	destroyLock();

}

void* HDAsyncVCap4::capThread(void*p)
{
	HDAsyncVCap4 *thread = (HDAsyncVCap4*)p;
	thread->Run();
	return NULL;
}

void HDAsyncVCap4::lock_read(char *ptr)
{
	int pic_size=0;
	char *pImg = m_core->GetDefaultImg();
	pthread_rwlock_rdlock(&rwlock);
	pic_size=SDI_WIDTH*SDI_HEIGHT*2;
	MEMCPY(ptr, pImg,pic_size);
	pthread_rwlock_unlock(&rwlock);
}

void HDAsyncVCap4::lock_write(char *ptr)
{
	int pic_size=0;
	char *pImg = m_core->GetDefaultImg();
	pthread_rwlock_wrlock(&rwlock);
	pic_size=SDI_WIDTH*SDI_HEIGHT*2;
	MEMCPY(pImg,ptr,pic_size);
	pthread_rwlock_unlock(&rwlock);
}
void  HDAsyncVCap4::Start()
{
	thread_state = THREAD_RUNNING;
}
void HDAsyncVCap4::destroyLock()
{
	pthread_rwlock_destroy(&rwlock);
	pthread_rwlockattr_destroy(&rwlockattr);
}

void HDAsyncVCap4::initLock()
{
	pthread_rwlockattr_init(&rwlockattr);
	pthread_rwlockattr_setpshared(&rwlockattr,2);
	pthread_rwlock_init(&rwlock,&rwlockattr);
}


bool HDv4l_cam::getEmpty(Alg_Obj * p_queue,unsigned char** pBGRBuf, int chId)
{
	int status=0;
	bool ret = true;
	while(1)
	{
		status = OSA_bufGetEmpty(&p_queue->bufHndl[chId],&m_bufId[chId],0);
		if(status == 0)
		{
			*pBGRBuf = (unsigned char*)p_queue->bufHndl[chId].bufInfo[m_bufId[chId]].virtAddr;
			break;
		}else{
			if(!OSA_bufGetFull(&p_queue->bufHndl[chId],&m_bufId[chId],OSA_TIMEOUT_FOREVER))
			{
				if(!OSA_bufPutEmpty(&p_queue->bufHndl[chId],m_bufId[chId]))
				{
					;
				}
			}
		}
	}
	 return ret;
}

bool HDv4l_cam::Data2Queue(Alg_Obj * p_queue,unsigned char *pYuvBuf,int width,int height,int chId)
{
	int status;

	chId = 0;
	if(chId>=1)//if(chId >= CAM_COUNT+1)
		return false;
	p_queue->bufHndl[chId].bufInfo[m_bufId[chId]].width=width;
	p_queue->bufHndl[chId].bufInfo[m_bufId[chId]].height=height;
	p_queue->bufHndl[chId].bufInfo[m_bufId[chId]].strid=width;

	OSA_bufPutFull(&p_queue->bufHndl[chId],m_bufId[chId]);
	return true;
}

void  HDv4l_cam::start_queue(Alg_Obj * p_queue)
{
	int num=SUB_1-MAIN_1;
	int j=0;
	for (int i = MAIN_1; i < MAIN_1+CAM_COUNT; i++) {
		j=i+num;
		getEmpty(p_queue,&GRAY_data_main[i], i);
		getEmpty(p_queue,&GRAY_data_sub[j], j);
	}
	getEmpty(p_queue,&select_bgr_data_main,MAIN_ONE_OF_TEN);
	getEmpty(p_queue,&select_bgr_data_sub,SUB_ONE_OF_TEN );
	getEmpty(p_queue,&FPGA6_bgr_data_main,MAIN_FPGA_SIX);
	getEmpty(p_queue,&FPGA6_bgr_data_sub, SUB_FPGA_SIX);
	getEmpty(p_queue,&FPGA4_bgr_data_main,MAIN_FPGA_FOUR);
	getEmpty(p_queue,&FPGA4_bgr_data_sub, SUB_FPGA_FOUR);
}

//-------------------HDv4l_cam_Producer methods----------------------

static HDv4l_cam * pHDv4l_cap[MAX_CC]={ 0};
HDv4l_cam *HDv4l_cam_Producer::Get_pHDv4l_cap(int idx)
{
	 assert(idx != 0);
	if(pHDv4l_cap[idx] == NULL){
		pHDv4l_cap[idx] = new HDv4l_cam(idx,SDI_WIDTH,SDI_HEIGHT);
	}
    assert(pHDv4l_cap[idx] != NULL);
	return pHDv4l_cap[idx];
}

