#include "stdafx.h"
#include <stdio.h>

#include "..\wrapper\wrapper.h"
#include "..\game\GameLoad.h"
#include "..\game\GameArea.h"
#include "..\project\resource.h"
#include "..\game\GamePlay.h"
#include "..\game\typedefines.h"
#include "..\project\vcgame.h"
#include "..\game\common.h"

#include "winuser.h"
#include "zmouse.h"

static HWND hGameWnd;
static CGame *game;

#define MAIN_SURFACE_WIDTH	1024
#define MAIN_SURFACE_HEIGHT	768

void WinGameSendCommand(UINT message, WPARAM wParam, WPARAM lParam)
{
	if (!game)
		return;
	switch (message)
	{
	case WM_KEYUP:
		game->SendCommand(G_KEY_RESET, 0, 0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 37: //left
			game->SendCommand(G_KEY_LEFT, 0, 0);
			break;
		case 38: //top
			game->SendCommand(G_KEY_UP, 0, 0);
			break;
		case 39: //right;
			game->SendCommand(G_KEY_RIGHT, 0, 0);
			break;
		case 40://down
			game->SendCommand(G_KEY_DOWN, 0, 0);
			break;
		case 65:
			game->SendCommand(G_KEY_A, 0, 0);
			break;
		case 68:
			game->SendCommand(G_KEY_D, 0, 0);
			break;
		case 78://N
			game->SendCommand(G_KEY_N, 0, 0);
			break;
		case 79://o
			game->SendCommand(G_KEY_O, 0, 0);
			break;
		}
		break;
	case WM_PAINT:
		game->SendCommand(G_REDRAW, 0, 0);
		break;
	case WM_TIMER:
		{
		static short t1=0;
		static short t2=0;
		static short t3=0;
		static short t4=0;
		game->SendCommand(G_TIMER, 0, 0);
		if(t1==25)
			game->SendCommand(G_TIMER_DIV10, 0, 0);
		t1=(t1+1)%50;
		if(t2==2)
			game->SendCommand(G_TIMER_DIV20, 0, 0);
		if(t2==7)
			game->SendCommand(G_TIMER_DIV20d, 0, 0);
		t2=(t2+1)%10;
		if(t3==2)
			game->SendCommand(G_TIMER_DIVS, 0, 0);
		t3=(t3+1)%5;
		if(t4==90)
			game->SendCommand(G_TIMER_DIV2S, 0, 0);
		t4=(t4+1)%200;
		}
		break;
	case WM_LBUTTONDOWN:
		game->SendCommand(G_MOUSE_LEFT, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		game->SendCommand(G_MOUSE_RIGHT, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		game->SendCommand(G_MOUSE_MOVE, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		game->SendCommand(G_MOUSE_WHEEL, HIWORD(wParam), 0);
		break;
	case WM_MOVE:
		game->SendCommand(G_WINDOW_MOVE, LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		break;
	}
}


void WinLoadMap()
{
}
void WinLoadResourceArea()
{
}
void WinLoadControlMenus()
{
}
void WinLoadStatusArea()
{
}
void WinLoadGame(HWND hWnd)
{
	hGameWnd = hWnd;
	game = new CGame;
}
void WinLoadConfigure()
{
}
void ShowBitmapImage()
{
//	LoadImage(NULL, (LPCTSTR)IDB_BITMAP_GAME_MAP, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
}
GHANDLE GGetWindowHandle()
{
	return 0;
}
void GGetWindowRect(RECT &rect)
{
  RECT clientRect, windowRect;

  GetClientRect(hGameWnd, &clientRect);
  GetWindowRect(hGameWnd, &windowRect);

  LONG boundLine = ((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left))/2;
  LONG boundTop = (windowRect.bottom - windowRect.top)-(clientRect.bottom - clientRect.top)-boundLine;

  rect.left = windowRect.left+boundLine;
  rect.right = windowRect.right-boundLine;
  rect.top = windowRect.top+boundTop;
  rect.bottom = windowRect.bottom-boundLine;
}

GHANDLE GLoadImage(RECT &rect, GPOINT &origin, RECT &rect1)
{
	rect1.left = 0;
	rect1.top = 0;
	return 0;
}
void GUpdateImage(RECT &rect1, GPOINT &origin)
{
}

GHANDLE GLoadBitmapSurface(char *fileName)
{
	BITMAP bitmap;
	HBITMAP bit = bitmap_load((LPCTSTR)fileName, bitmap);
	LPDIRECTDRAWSURFACE7 surf = setup_bitmap_surface(bitmap.bmWidth, bitmap.bmHeight);
	
	GAssert(surf);

	RECT rect;
	rect.left = 0; rect.top = 0; 
	rect.right = bitmap.bmWidth; rect.bottom = bitmap.bmHeight;

	bitmap2surface(surf, bit, rect, rect);
	bitmap_unload(bit);

	return (GHANDLE)surf;
}
GHANDLE GLoadBitmapSurface(DWORD rcName)
{
	BITMAP bitmap;
	HBITMAP bit = bitmap_load(rcName, bitmap);
	LPDIRECTDRAWSURFACE7 surf = setup_bitmap_surface(bitmap.bmWidth, bitmap.bmHeight);
	
	GAssert(surf);

	RECT rect;
	rect.left = 0; rect.top = 0; 
	rect.right = bitmap.bmWidth; rect.bottom = bitmap.bmHeight;

	bitmap2surface(surf, bit, rect, rect);
	bitmap_unload(bit);

	return (GHANDLE)surf;
}
void GUnloadBitmapSurface(GHANDLE surface)
{
	reset_bitmap_surface((LPDIRECTDRAWSURFACE7)surface);
}

void GSurfaceTransfer(GHANDLE dest, GHANDLE src, RECT &dstRect, RECT &srcRect, bool overlay)
{
	int dx, dy;
	RECT rect = GGetMainSurfaceDimension();

	// for safe guard for over-ranging of surface
	if((dstRect.right < rect.left)
		|| (dstRect.left > rect.right)
		|| (dstRect.bottom < rect.top)
		|| (dstRect.top > rect.bottom))
		return;
	if((dx = dstRect.left - rect.left)<0)
	{
		dstRect.left = rect.left;
		srcRect.left -= dx;
	}else if((dx = dstRect.right - rect.right)>0)
	{
		dstRect.right = rect.right;
		srcRect.right -= dx;
	}
	if((dy = dstRect.top - rect.top)<0)
	{
		dstRect.top = rect.top;
		srcRect.top -= dy;
	}else if((dy = dstRect.bottom - rect.bottom)>0)
	{
		dstRect.bottom = rect.bottom;
		srcRect.bottom -= dy;
	}

	if(!overlay)
		surface2surface((LPDIRECTDRAWSURFACE7)dest, (LPDIRECTDRAWSURFACE7)src, dstRect, srcRect);
	else
		surface2surface_overlay((LPDIRECTDRAWSURFACE7)dest, (LPDIRECTDRAWSURFACE7)src, dstRect, srcRect);
}
void GInitDisplayDevice(void)
{
	InitDDrawEx(hGameWnd);//≥ı ºªØDX
}
void GDeinitDisplayDevice(void)
{
	DeinitDDrawEx();
}
GHANDLE GInitMainSurface(void)
{
	return (GHANDLE)setup_bitmap_surface(MAIN_SURFACE_WIDTH, MAIN_SURFACE_HEIGHT);
}
RECT GGetMainSurfaceDimension(void)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = MAIN_SURFACE_WIDTH - 1;
	rect.bottom = MAIN_SURFACE_HEIGHT - 1;
	return rect;
}
void GDeinitBitmapSurface(GHANDLE surface)
{
	reset_bitmap_surface((LPDIRECTDRAWSURFACE7)surface);
}
void GShowSurface(GHANDLE surface)
{
	DrawImageSurface((LPDIRECTDRAWSURFACE7)surface);
}
void GShowSurfOverlay(GHANDLE surface)
{
	DrawImageSurfaceOverlay((LPDIRECTDRAWSURFACE7)surface);
}
void GTrace(const char *c, int d)
{
	int n;

	char str[100];
	memset(str, 0, 100);
	sprintf(str, c, d);

	for(n=0;str[n]!=0;n++);

	OutputDebugString(str);
}

GHANDLE GSetupTimer(void)
{
	UINT_PTR timer =SetTimer(hGameWnd, 1, 10, (TIMERPROC)GTimerCallback);
	return GHANDLE(timer);
}

void GResetTimer(GHANDLE handle)
{
	KillTimer(hGameWnd, 1);
}

void GTimerCallback(void)
{
	WinGameSendCommand(WM_TIMER, 0, 0);
}

void GGetCurrentTime(G_TIME &time)
{
	SYSTEMTIME time1;
	GetSystemTime(&time1);
	memcpy(&time, &time1, sizeof(time1));
//	Sleep(1);
}

void GLockDisplayBuffer(GHANDLE surf, RECT &rect, char **buffer, int &stride)
{
	// limit rect value for safe surface lock
	RECT rect1 = GGetMainSurfaceDimension();
	// for safe guard for over-ranging of surface
	if((rect.right <= rect1.left)
		|| (rect.left >= rect1.right)
		|| (rect.bottom <= rect1.top)
		|| (rect.top >= rect1.bottom))
	{
		*buffer = NULL;
		return;
	}
	if(rect.left < rect1.left)
		rect.left = rect1.left;
	else if(rect.right > rect1.right)
		rect.right = rect1.right;
	if(rect.top < rect1.top)
		rect.top = rect1.top;
	else if(rect.bottom > rect1.bottom)
		rect.bottom = rect1.bottom;

	surface_lock((LPDIRECTDRAWSURFACE7)surf, &rect, buffer, stride);
}
void GUnlockDisplayBuffer(GHANDLE surf, RECT &rect)
{
	surface_unlock((LPDIRECTDRAWSURFACE7)surf, &rect);
}
