/*
 * RoiFocusCamidx.cpp
 *
 *  Created on: 2018年6月14日
 *      Author: xavier
 */
#include"RoiFocusCamidx.h"
#include"stddef.h"
static RoiFocusCamidx * pinsFC=NULL;
static RoiFocusCamidx * pinsFCMAIN=NULL;

RoiFocusCamidx* RoiFocusCamidx ::GetInstance()
{
	if(pinsFC==NULL)
	{
		pinsFC=new RoiFocusCamidx;
	}
		return pinsFC;
}

RoiFocusCamidx* RoiFocusCamidx ::GetMainInstance()
{
	if(pinsFCMAIN==NULL)
	{
		pinsFCMAIN=new RoiFocusCamidx;
	}
		return pinsFCMAIN;
}

