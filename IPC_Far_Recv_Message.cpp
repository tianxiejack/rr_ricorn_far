/*
 * Xin_IPC_Message.c
 *
 *  Created on: 2018年4月23日
 *      Author: xz
 */
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h> 	/* add this: exit返回,不会报提示信息 */
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>  /* ture false 有效*/
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include  <arpa/inet.h>
#include "IPC_Far_Recv_Message.h"

typedef enum {
	TRANSFER_TO_EPHOR = 0x00, TRANSFER_TO_PASSENGER = 0x01, IPC_NUM,
} IPC_NUM_TYPE;

#define IPC_PORT_NUM 125
int ipc_port[IPC_NUM];
key_t ipc_key[IPC_NUM];
int ipc_qid[IPC_NUM];

typedef enum {
	MSG_TYPE_START,
//车长
	CLICK_COORDINATE, //车长触摸屏坐标
	EPHOR_KEY_MESSAGE, //车长键值
	ANGLE_PERISCOPIC, //周视镜角度
	ANGLE_TURRET, //炮塔角度
//载员
	PASSENGER_KEY_MESSAGE, //载员指令
	PASSENGER_DISPLAY_NOMAL,
	PASSENGER_DISPLAY_OVER_TEMP,

	PREPARE_TYPE,
} IPC_MSG_TYPE;

typedef struct {
	long msg_type;
	union Capricorn_far_payload {
		coor_p ephor_click_coord;
		KEY_TYPE key_value;
		int angle_msg;
	} payload;
} IPC_msg;

coor_p ephor_coor = { -1, -1 };
KEY_TYPE KeyType_Ephor = KEY_TYPE_START;//MSG_TYPE_START;
KEY_TYPE KeyType_Passenger =KEY_TYPE_START;// MSG_TYPE_START;
float angle_periscopic = 0.0;
float angle_azimuth = 0.0;
pthread_mutex_t Mutex[2] = PTHREAD_MUTEX_INITIALIZER;

#define  IPC_ftok_path "/home/"
int sockfd;
char buf[22] = { 0 };
struct sockaddr_in servaddr;

void *Recv_ipc_Ephor(void *arg);
void *Recv_ipc_Passenger(void *arg);

int udpinit() {
	int ret_bind;
	struct sockaddr_in localaddr;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(-1);
	}
	
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = 0;
	localaddr.sin_port = htons(6664);
	ret_bind = bind(sockfd, (struct sockaddr*) &localaddr,sizeof(localaddr));
	if (ret_bind < 0) {
		perror("fail to bind!\n");
		return -2;
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("192.9.200.203"); //载员ip
	servaddr.sin_port = htons(6664); //载员网络接收端口
}

void IPC_Init(IPC_NUM_TYPE num) {
	ipc_port[num] = IPC_PORT_NUM + num;
	ipc_key[num] = ftok(IPC_ftok_path, ipc_port[num]);
	if (ipc_key[num] == -1) {
		printf(" port %d ftok key failed erro:  \n", ipc_port[num]);
		perror(IPC_ftok_path);
		exit(-1);
	}

	ipc_qid[num] = msgget(ipc_key[num], IPC_EXCL); /*检查消息队列是否存在*/
	if (ipc_qid[num] < 0) {
		ipc_qid[num] = msgget(ipc_key[num], IPC_CREAT | 0666);/*创建消息队列*/
		if (ipc_qid[num] < 0) {
			printf("failed to create msq | errno=%d \n", errno);
			perror(" ");
			exit(-1);
		}
	} else {
		printf("port %d already exist! \n", ipc_port[num]);
	}
}

void IPC_Init_Ephor() {
	int ret;
	pthread_t thread1;
	IPC_Init(TRANSFER_TO_EPHOR);
	ret = pthread_create(&thread1, NULL, Recv_ipc_Ephor, NULL);
	if (ret != 0) {
		printf("pthread1_create failed!\n");
		exit(ret);
	}
}
void IPC_Init_Passenger() {
	int ret;
	pthread_t thread2;
	IPC_Init(TRANSFER_TO_PASSENGER);
	ret = pthread_create(&thread2, NULL, Recv_ipc_Passenger, NULL);
	if (ret != 0) {
		printf("pthread2_create failed!\n");
		exit(ret);
	}
}

void IPC_Init_All() {
	IPC_Init_Ephor();
	IPC_Init_Passenger();
	udpinit();
}

/*接收线程*/
void *Recv_ipc_Ephor(void *arg) {
	int ret_value;
	IPC_msg msg;
	printf("\n%x\n", ipc_qid[TRANSFER_TO_EPHOR]);
	while (1) {
		memset(&msg, 0, sizeof(IPC_msg));
		ret_value = msgrcv(ipc_qid[TRANSFER_TO_EPHOR], &msg, sizeof(msg.payload), 0, 0);
		if (ret_value < 0) {
			printf("%s Receive  IPC msg failed,errno=%d !!!\n", __FUNCTION__,
			errno);
		}
		switch (msg.msg_type) {
		case CLICK_COORDINATE:
			pthread_mutex_lock(&Mutex[0]);
			ephor_coor.point_x = msg.payload.ephor_click_coord.point_x;
			ephor_coor.point_y = msg.payload.ephor_click_coord.point_y;
			pthread_mutex_unlock(&Mutex[0]);
			break;
		case ANGLE_PERISCOPIC:
			pthread_mutex_lock(&Mutex[0]);
			angle_periscopic = msg.payload.angle_msg;
			pthread_mutex_unlock(&Mutex[0]);
			break;
		case ANGLE_TURRET:
			pthread_mutex_lock(&Mutex[0]);
			angle_azimuth = msg.payload.angle_msg;
			pthread_mutex_unlock(&Mutex[0]);
			break;
		default:
			printf("%s no msg.msg_type ", __FUNCTION__);
		}
	}
}
void *Recv_ipc_Passenger(void *arg) {
	int ret_value;
	IPC_msg msg;
	printf("\n%x\n", ipc_qid[TRANSFER_TO_PASSENGER]);
	while (1) {
		memset(&msg, 0, sizeof(IPC_msg));
		ret_value = msgrcv(ipc_qid[TRANSFER_TO_PASSENGER], &msg, sizeof(msg.payload), 0,
				0);
		if (ret_value < 0) {
			printf("%s Receive  IPC msg failed,errno=%d !!!\n", __FUNCTION__,
			errno);
		}
		switch (msg.msg_type) {
		case PASSENGER_KEY_MESSAGE:
			pthread_mutex_lock(&Mutex[1]);
			KeyType_Passenger = msg.payload.key_value;
			pthread_mutex_unlock(&Mutex[1]);
			break;
		case PASSENGER_DISPLAY_NOMAL:
			pthread_mutex_lock(&Mutex[1]);
			KeyType_Passenger = msg.payload.key_value;
			pthread_mutex_unlock(&Mutex[1]);
			break;
		case PASSENGER_DISPLAY_OVER_TEMP:
			pthread_mutex_lock(&Mutex[1]);
			KeyType_Passenger = msg.payload.key_value;
			pthread_mutex_unlock(&Mutex[1]);
			break;
		default:
			printf("%s no msg.msg_type ", __FUNCTION__);
		}
	}
}

void IPC_Destroy(int num) {
	msgctl(ipc_qid[num], IPC_RMID, 0); //删除消息队列
}
void IPC_DestroyEphor() {
	IPC_Destroy(TRANSFER_TO_EPHOR);
}
void IPC_DestroyPassenger() {
	IPC_Destroy(TRANSFER_TO_PASSENGER);
}

void IPC_Destroy_All() {
	IPC_DestroyEphor();
	IPC_DestroyPassenger();
}

coor_p getEphor_CoorPoint() {
	coor_p coor_buf = ephor_coor;
	ephor_coor.point_x = -1;
	ephor_coor.point_y = -1;
	return coor_buf;
}

KEY_TYPE getPassenger_KeyType() {
	KEY_TYPE key_buf = KeyType_Passenger;
	KeyType_Passenger = KEY_TYPE_START;//MSG_TYPE_START;
	return key_buf;
}
float getAngleFar_PeriscopicLens() {
	return angle_periscopic;
}
float getAngleFar_AzimuthAngle() {
	return angle_azimuth;
}
void SendPowerOnSelfTest() {
	buf[0] = buf[1] = buf[2] = 0;
	buf[3] = 0xc9;
	buf[4] = buf[5] = buf[6] = 0;
	buf[7] = 0xcb;
	buf[8] = buf[9] = buf[10] = 0;
	buf[11] = 0x2;
	buf[12] = buf[13] = buf[14] = 0;
	buf[15] = 0x00;
	buf[16] = 0x00;
	buf[17] = 0x01;
	buf[18] = buf[19] = buf[20] = 0;
	buf[21] = 0x01;

	sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &servaddr,
			sizeof(servaddr));

}
