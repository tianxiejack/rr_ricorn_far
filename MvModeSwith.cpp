/*
 * MVModeSwith.cpp

 *
 *  Created on: 2018年5月24日
 *      Author: fsmdn121
 */
#include"MvModeSwith.h"

MVModeSwith   mvSwitch;
MVModeSwith::MVModeSwith()
{
	for(int i=0;i<SWITCH_COUNT;i++)
		mvSwitch[i]=false;
}

