#pragma once
#include "gcarmoto.h"
#include "IGArmed.h"

class CGKiA :
	public CGCarMoto,
	public IGArmedCannon
{
public:
	CGKiA(void);
	CGKiA(CGBitmapTempletCar *car,
		CGBitmapTempletSmoke *smoke,
		CGBitmapTempletBullet *bullet);
	virtual ~CGKiA(void);

	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
	// Process command from keyboard, mouse, or other devices
	bool CommandProcessor(int cmd, int wParam, int lParam);
};

class CGCarPeng :
	public CGKiA
{
public:
	CGCarPeng(void);
	CGCarPeng(CGBitmapTempletCar *car,
		CGBitmapTempletSmoke *smoke,
		CGBitmapTempletBullet *bullet);
	virtual ~CGCarPeng(void);
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void ComewithBoundary(G_SHIFT_RESULT status);
};
