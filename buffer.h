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
		IF_MotionDetectorROI *pTel_roi_M,
		IF_MotionDetectorROI *pTel_roi_S,
		IF_MotionDetectorROI *pAll_roi_M_0,
		IF_MotionDetectorROI *pAll_roi_M_1,
		IF_MotionDetectorROI *pAll_roi_S_0,
		IF_MotionDetectorROI *pAll_roi_S_1);
void get_buffer(unsigned char* ptr, int currentchanl);
void get_bufferyuv(unsigned char* ptr, int chId);
extern Alg_Obj * queue_dis;

#endif /* BUFFER_H_ */
