/*
 * IF_MotionDetectorROI.h
 *
 *  Created on: 2018年5月22日
 *      Author: fsmdn121
 */

#ifndef IF_MOTIONDETECTORROI_H_
#define IF_MOTIONDETECTORROI_H_

class IF_MotionDetectorROI
{
	public:
		virtual 	void SettempSrc4(unsigned char *src)=0;
		virtual void SettempSrc6(unsigned char *src)=0;
};







#endif /* IF_MOTIONDETECTORROI_H_ */
