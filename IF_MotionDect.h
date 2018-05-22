/*
 * IF_MotionDect.h
 *
 *  Created on: 2018年5月22日
 *      Author: fsmdn121
 */

#ifndef IF_MOTIONDECT_H_
#define IF_MOTIONDECT_H_

class IF_MotionDect
{
public:
	virtual void DrawRectOnpic(unsigned char *src,int capidx)=0;
	virtual void selectFrame(unsigned char *dst,unsigned char *src,int targetId,int camIdx)=0;
	virtual bool MDisStart()=0;
};







#endif /* IF_MOTIONDECT_H_ */
