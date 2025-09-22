#pragma once
#include "IGBurn.h"
#include "IGDrive.h"
#include "GCar.h"


class CGCarMoto :
	public CGCar,
	public IGBurnSmoke,
	public IGDriveMoto
{
public:
	CGCarMoto(void);
	CGCarMoto(CGBitmapTempletCar *car,
		CGBitmapTempletSmoke *smoke);
	~CGCarMoto(void);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	// reduce m_blood
	bool FlowBlood(int blood);
protected:
	// Process event when come with the boundary of the map, or other game objects
	void ComewithBoundary(G_SHIFT_RESULT status);
public:
	// Process command from keyboard, mouse, or other devices
	bool CommandProcessor(int cmd, int wParam, int lParam);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
};
