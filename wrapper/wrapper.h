#include "..\game\typedefines.h"
#include <assert.h>
//#define WIN_MOBILE
#ifdef WIN_MOBILE
#include "..\ppc_prj\resourceppc.h"
#else
#include "..\project\vcgame.h"
#endif

#define	G_KEY_RESET				1
#define	G_KEY_LEFT				2
#define	G_KEY_RIGHT				3
#define	G_KEY_UP				4
#define	G_KEY_DOWN				5
#define	G_REDRAW				6
#define	G_KEY_A					7
#define	G_KEY_D					8
#define	G_TIMER					9
#define	G_KEY_N					10
#define	G_MOUSE_LEFT			11
#define	G_MOUSE_RIGHT			12
#define	G_MOUSE_SHIFT_LEFT		13
#define	G_MOUSE_SHIFT_RIGHT		14
#define G_MOUSE_WHEEL			15
#define G_MOUSE_MOVE			16
#define G_KEY_O					17
#define	G_TIMER_DIV10			18
#define	G_TIMER_DIV20			19
#define G_TIMER_DIVS			20
#define G_TIMER_DIV2S			21
#define G_WINDOW_MOVE			22
#define	G_TIMER_DIV20d			23

#define STR_COPY strcpy

void WinLoadMap();
void WinLoadResourceArea();
void WinLoadControlMenus();
void WinLoadStatusArea();
void WinLoadGame(HWND hWnd);
void WinLoadConfigure();
void WinGameSendCommand(UINT message, WPARAM wParam, WPARAM lParam);

void ShowBitmapImage();

GHANDLE GGetWindowHandle();
void GGetWindowRect(RECT &rect);
GHANDLE GLoadImage(RECT &rect, GPOINT &origin,RECT &rect1);
void GUpdateImage(RECT &rect1, GPOINT &origin);
GHANDLE GLoadBitmapSurface(char *fileName);
void GUnloadBitmapSurface(GHANDLE surface);
GHANDLE GLoadBitmapSurface(DWORD rcName);

void GSurfaceTransfer(GHANDLE surf, GHANDLE handle, RECT &dstRect, RECT &srcRect, bool overlay);
void GInitDisplayDevice(void);
void GDeinitDisplayDevice(void);
GHANDLE GInitMainSurface(void);
RECT GGetMainSurfaceDimension(void);
void GDeinitBitmapSurface(GHANDLE surface);
void GShowSurface(GHANDLE surface);
void GTrace(const char *c, int d);
//template <class T> void GAssert(T exp);
#define GAssert assert
void GShowSurfOverlay(GHANDLE surface);

GHANDLE GSetupTimer(void);
void GResetTimer(GHANDLE handle);
void GTimerCallback(void);

void GGetCurrentTime(G_TIME &time);

void GLockDisplayBuffer(GHANDLE surf, RECT &rect, char **buffer, int &stride);
void GUnlockDisplayBuffer(GHANDLE surf, RECT &rect);

#ifdef  WIN_MOBILE
#define PRINT_STRING		print_string_16bit
#define SHOW_FOCUS_BAR		ShowFocusBar_16bit
#define DRAW_OUT_FRAME		DrawOutFrame_16bit
#define SHOW_GLOBAL_MAP		ShowGlobalMap_16bit
#define SHOW_CURRENT_VIEW	ShowCurrentView_16bit
#else
#define PRINT_STRING		print_string_argb
#define SHOW_FOCUS_BAR		ShowFocusBar
#define DRAW_OUT_FRAME		DrawOutFrame
#define SHOW_GLOBAL_MAP		ShowGlobalMap
#define SHOW_CURRENT_VIEW	ShowCurrentView
#endif

#define EDGE_COLOR_VALUE	0x0088DD55
#define VIEW_RECTANG_VALUE	0x00ff5500
#define GLOBAL_MAP_COLOR1	0x00555500
#define GLOBAL_MAP_COLOR2	0x00ffffff
#define GLOBAL_MAP_COLOR3	0x00dddd00

#define EDGE_COLOR_VALUE_16B	(short)0x8EEA
#define VIEW_RECTANG_VALUE_16B	(short)0xfaa0
#define GLOBAL_MAP_COLOR1_16B	(short)0x52A0
#define GLOBAL_MAP_COLOR2_16B	(short)0xffff
#define GLOBAL_MAP_COLOR3_16B	(short)0xDEE0

#define FOCUSBAR_COLOR_BACK			0x0055ff00
#define FOCUSBAR_COLOR_FRONT		0x00ff5500
#define FOCUSBAR_COLOR_BACK_16B		(short)0x57e0
#define FOCUSBAR_COLOR_FRONT_16B	(short)0xfaa0
