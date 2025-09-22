#include "stdafx.h"
#include "GamePlay.h"
#include "GameLoad.h"
#include "..\wrapper\\wrapper.h"
#include ".\gameplay.h"

CGame::CGame()
: pTimer(NULL)
, pTinyArea(NULL)
{
	pTimer = new CGTimer;
	GInitDisplayDevice();
	pActiveArea = new CGAreaAct;
	pActiveArea->LoadAllObjects();
	pTinyArea = new CGAreaTiny(pActiveArea);
	pMenuArea = new CGAreaMenu(pActiveArea);
}

CGame::~CGame()
{
	pActiveArea->UnloadAllObjects();
	delete pMenuArea;
	delete pTinyArea;
	delete pActiveArea;
	GDeinitDisplayDevice();
	delete pTimer;
}

void CGame::SendCommand(int cmd, int wParam, int lParam)
{
	CGameObject *pObj = NULL;

	switch (cmd)
	{
	case G_KEY_RESET:
		break;
	case G_KEY_LEFT:
	case G_KEY_RIGHT:
	case G_KEY_UP:
	case G_KEY_DOWN:
	case G_KEY_A:
	case G_KEY_D:
		pObj = pActiveArea->GetFocusObject();
		if(pObj)
			pObj->CommandProcessor(cmd, wParam, lParam);
		break;
	case G_REDRAW:
	case G_WINDOW_MOVE:
		pActiveArea->OnRedraw();
		pTinyArea->OnRedraw();
		pMenuArea->OnRedraw();
		break;
	case G_TIMER:
		pMenuArea->TimerCallback_10MS(10);
		pTinyArea->TimerCallback_10MS(10);
		pActiveArea->TimerCallback_10MS(10);
		break;
	case G_TIMER_DIV10:
		pTimer->TimerCallback_10MS();
		pActiveArea->TimerCallback_500MS(10);
		break;
	case G_TIMER_DIV20:
		pActiveArea->TimerCallback_100MS(10);
		break;
	case G_TIMER_DIV20d:
		pActiveArea->TimerCallback_100MS_delta1(10);
		break;
	case G_TIMER_DIV2S:
		pActiveArea->TimerCallback_2S(10);
		break;
	case G_TIMER_DIVS:
		pActiveArea->TimerCallback_Test(10);
		break;
	case G_KEY_O:
	case G_KEY_N:
	case G_MOUSE_MOVE:
	case G_MOUSE_WHEEL:
		pActiveArea->CommandProcessor(cmd, wParam, lParam);
		break;
	case G_MOUSE_LEFT:
	case G_MOUSE_RIGHT:
		{
		GPOINT pos;
		CGArea *p_area;
		pos.x = wParam;
		pos.y = lParam;
		p_area = WhichAreaIsMouse(pos);
		if(p_area)
			p_area->CommandProcessor(cmd, wParam, lParam);
		}
		break;
	default:
		;
	}
}
// decide which area the mouse locates
CGArea * CGame::WhichAreaIsMouse(GPOINT & pos)
{
	RECT rect;
	CGArea *p_area[3];
	int x0, y0;

	pActiveArea->GetRect(rect);
	x0 = rect.left;
	y0 = rect.top;

	p_area[0] = pActiveArea;
	p_area[1] = pTinyArea;
	p_area[2] = pMenuArea;
	for(int i=0; i<3 && p_area[i]!=0; i++)
	{
		p_area[i]->GetRect(rect);
		rect.left-=x0; rect.right-=x0;
		rect.top-=y0; rect.bottom-=y0;
		if(pos.x>=rect.left && pos.x<rect.right
			&& pos.y>=rect.top && pos.y<rect.bottom)
			return p_area[i];
	}
	return NULL;
}
