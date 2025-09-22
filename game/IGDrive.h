#pragma once
#include "GameLoad.h"

class IGDrive
{
public:
	virtual void Load(CGameObject *pObj) = 0;
	virtual void Unload(void) = 0;
	// Turn the car 45d left-ward
	virtual void TurnLeft(void) = 0;
	// Turn the car 45d right-ward
	virtual void TurnRight(void) = 0;
	// Speed up the moto car, forwards or backwards, the direction depends on the car's angle
	virtual void SpeedUp(bool forward) = 0;
	// Stop the moto car, no matter go forwards or backwards.
	virtual void BrakeUp(void) = 0;
	// Process command from keyboard, mouse, or other devices
	virtual bool CommandProcessor(int cmd, int wParam, int lParam) = 0;
protected:
	// The car's reverse gear: 0-forward; 1-backward
	bool m_reverse_gear;
};

class IGDriveMoto :
	public IGDrive
{
public:
	IGDriveMoto(void);
	virtual ~IGDriveMoto(void);
public:
	void Load(CGameObject *pObj);
	void Unload(void);
	// Turn the car 45d left-ward
	void TurnLeft(void);
	// Turn the car 45d right-ward
	void TurnRight(void);
	// Speed up the moto car, forwards or backwards, the direction depends on the car's angle
	void SpeedUp(bool forward);
	// Stop the moto car, no matter go forwards or backwards.
	void BrakeUp(void);
	// Process command from keyboard, mouse, or other devices
	bool CommandProcessor(int cmd, int wParam, int lParam);
protected:
	class CGCar *m_host;
};
