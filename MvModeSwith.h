/*
 * MvModeSwith.h
 *
 *  Created on: 2018年5月24日
 *      Author: fsmdn121
 */

#ifndef MVMODESWITH_H_
#define MVMODESWITH_H_
enum{
	MIAN_MV_ALL_VIEW_SWITCH,
	MIAN_MV_TEL_VIEW_SWITCH,
	SUB_MV_ALL_VIEW_SWITCH,
	SUB_MV_TEL_VIEW_SWITCH,
	SWITCH_COUNT
};
class MVModeSwith
{
public:
	MVModeSwith();
	void OpenSwitch(int switch_idx){mvSwitch[switch_idx]=true;};
	void CloseSwitch(int switch_idx){mvSwitch[switch_idx]=false;};
	bool IsSwitchOpen(int switch_idx)
	{
		return mvSwitch[SWITCH_COUNT];
	};
private:
	bool mvSwitch[SWITCH_COUNT];
};



#endif /* MVMODESWITH_H_ */
