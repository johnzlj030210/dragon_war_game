#pragma once
#include "typedefines.h"
#include "GameLoad.h"
#include "GTimer.h"

class CGame
{
public:
	CGame();
	virtual ~CGame();
protected:
	CGAreaAct *pActiveArea;
	// tiny area
	CGAreaTiny *pTinyArea;
	CGAreaMenu *pMenuArea;
public:
	void SendCommand(int cmd, int wParam, int lParam);
protected:
	// The timer base of the whole game
	CGTimer *pTimer;
private:
	// decide which area the mouse locates
	CGArea * WhichAreaIsMouse(GPOINT & pos);
};

