/*
 * buffer.h
 *
 *  Created on: Jan 17, 2017
 *      Author: wang
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "osa_buf.h"
#include "queue_display.h"
#include"IF_MotionDect.h"
#include"IF_MotionDetectorROI.h"
void destroy_buffer();
void init_buffer(IF_MotionDect *p,
		IF_MotionDetectorROI *p_mdRoi_mainT,
		IF_MotionDetectorROI *p_mdRoi_subT,
		IF_MotionDetectorROI *p_mdRoi_mainA,
		IF_MotionDetectorROI *p_mdRoi_subA);
void get_buffer(unsigned char* ptr, int currentchanl);
void get_bufferyuv(unsigned char* ptr, int chId);
extern Alg_Obj * queue_dis;

#endif /* BUFFER_H_ */
