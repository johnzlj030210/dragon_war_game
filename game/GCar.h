#pragma once
#include "typedefines.h"
#include "IGWalk.h"

class CGCar : public CGameObject, public IGWalkAutoAx
{
	friend class IGDriveMoto;
public:
	CGCar();
	virtual ~CGCar();
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	// Shift the car to the point(x, y)
	G_SHIFT_RESULT Shift(int x, int y);
	// Change and show the car
	void UpdateView(bool show);
	// Move the car to the point(x,y)
	void ShiftTo(int x, int y);
protected:
	// show a high light bar under the focus car object
	void ShowFocusBar(RECT &rect);
	void ShowFocusBar_16bit(RECT &rect);
	// Process event when come with the boundary of the map, or other game objects
	virtual void ComewithBoundary(G_SHIFT_RESULT status) = 0;
public:
	// Change the car's status every delta_t clock, called by the timer automatically
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
protected:
	// used for shifting
	double delta_x, delta_y;
};

