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


#define MEMCPY memcpy

#define INPUT_IMAGE_WIDTH 1920
#define INPUT_IMAGE_HEIGHT 1080

using namespace std;
using namespace cv::ocl;
static int once_buffer;
int m_bufId[8]={0};
extern void DeinterlaceYUV_Neon(unsigned char *lpYUVFrame, int ImgWidth, int ImgHeight, int ImgStride);
//Mat SDI_frame,VGA_frame;
unsigned char * sdi_data[CAM_COUNT];
unsigned char * vga_data=NULL;
OSA_SemHndl sem[CAM_COUNT];
int CAM_READ[6]={-1,-1,-1,-1,-1,-1};
int CAM_WRITE[6]={-1,-1,-1,-1,-1,-1};
HDv4l_cam::HDv4l_cam(int devId,int width,int height):io(IO_METHOD_MMAP/*IO_METHOD_MMAP*/),imgwidth(width),
imgheight(height),buffers(NULL),memType(MEMORY_NORMAL),cur_CHANnum(0),
force_format(1),m_devFd(-1),n_buffers(0),bRun(false),Id(devId)
{
		imgformat 	= V4L2_PIX_FMT_YUYV;
		sprintf(dev_name, "/dev/video%d",devId);
			imgstride 	= imgwidth*2;
			bufSize 	= imgwidth * imgheight * 2;
			imgtype     = CV_8UC2;
			memType = MEMORY_NORMAL;
			bufferCount = 6;
}

HDv4l_cam::~HDv4l_cam()
{
	stop_capturing();
	uninit_device();
	close_device();
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
				static bool Once=true;
				#if TRACK_MODE
				static  unsigned char * sdi_data2= NULL;
				#endif
					if(Once)
					{
						for(int i=0;i<CAM_COUNT;i++)
						{
							int ret=OSA_semCreate(&sem[i],1,1);
							if(ret<0)
							{
								printf("%d OSA_semCreate failed\n",i);
							}
							sdi_data[i]=(unsigned char *)malloc(SDI_WIDTH*SDI_HEIGHT*2);
						}
						Once=false;
					}

					OSA_semWait(&sem[now_pic_format],OSA_TIMEOUT_NONE);
					if(CAM_READ[now_pic_format]==1)
							printf("wirte but read busy,error\n");
					else
						CAM_WRITE[now_pic_format]=1;
							memcpy(sdi_data[now_pic_format],buffers[buf.index].start,SDI_WIDTH*SDI_HEIGHT*2);
						CAM_WRITE[now_pic_format]=0;
						OSA_semSignal(&sem[now_pic_format]);

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





