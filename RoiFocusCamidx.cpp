/*
 * RoiFocusCamidx.cpp
 *
 *  Created on: 2018年6月14日
 *      Author: xavier
 */
#include"RoiFocusCamidx.h"
#include"stddef.h"
static RoiFocusCamidx * pinsFC=NULL;


RoiFocusCamidx* RoiFocusCamidx ::GetInstance()
{
	if(pinsFC==NULL)
	{
		pinsFC=new RoiFocusCamidx;
	}
		return pinsFC;
}
