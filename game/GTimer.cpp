#include "StdAfx.h"
#include ".\gtimer.h"
#include "..\wrapper\\wrapper.h"

CGTimer::CGTimer(void)
{
	handle = GSetupTimer();
}

CGTimer::~CGTimer(void)
{
	GResetTimer(handle);
}

// Timer callback function
void CGTimer::TimerCallback_10MS(void)
{
}
