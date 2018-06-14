/*
 * RoiFocusCamidx.h
 *
 *  Created on: 2018年6月14日
 *      Author: xavier
 */
#ifndef ROIFOCUSCAMIDX_H_
#define ROIFOCUSCAMIDX_H_

 class RoiFocusCamidx
 {
public:
	static RoiFocusCamidx *GetInstance();
	int GetRoiFocusCamidx(){return roiFocusCamidx;};
	void increaseRoiFocusCamidx(){roiFocusCamidx++;roiFocusCamidx%=CAM_COUNT;};
	void decreaseRoiFocusCamidx(){roiFocusCamidx+=(CAM_COUNT-1);roiFocusCamidx%=CAM_COUNT;};
	void flipRoiFocusCamidx(){roiFocusCamidx=CAM_COUNT-roiFocusCamidx;};
private:
	 RoiFocusCamidx():roiFocusCamidx(0){};
	 int roiFocusCamidx;
 };



#endif /* ROIFOCUSCAMIDX_H_ */
