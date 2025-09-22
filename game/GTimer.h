#pragma once
#include "typedefines.h"

class CGTimer
{
public:
	CGTimer(void);
	virtual ~CGTimer(void);
protected:
	// The handle to the timer instance
	GHANDLE handle;
public:
	// Timer callback function
	void TimerCallback_10MS(void);
};
