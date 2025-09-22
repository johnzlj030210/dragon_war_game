#pragma once

#include "resource.h"
#include "ddraw.h"

BOOL InitDDraw(HWND hWnd, RECT &rect, int x, int y);
void DrawImage();
void GetBmpSize(LONG &width, LONG &height);
HBITMAP bitmap_load(LPCTSTR file_name, BITMAP &bitmap);
HBITMAP bitmap_load(DWORD rcName, BITMAP &bitmap);
void bitmap_unload(HBITMAP bit);
BOOL InitDDrawEx(HWND hWnd);
void DeinitDDrawEx();
LPDIRECTDRAWSURFACE7 setup_bitmap_surface(int width, int height);
void reset_bitmap_surface(LPDIRECTDRAWSURFACE7 surf);
void bitmap2surface(LPDIRECTDRAWSURFACE7 surf, HBITMAP bit, RECT dstRect, RECT srcRect);
void DrawImageSurface(LPDIRECTDRAWSURFACE7 surface);
void DrawImageSurfaceOverlay(LPDIRECTDRAWSURFACE7 surface);
void surface2surface(LPDIRECTDRAWSURFACE7 dest, LPDIRECTDRAWSURFACE7 src, RECT dstRect, RECT srcRect);
void surface2surface_overlay(LPDIRECTDRAWSURFACE7 dest, LPDIRECTDRAWSURFACE7 src, RECT dstRect, RECT srcRect);

void surface_lock(LPDIRECTDRAWSURFACE7 surf, RECT *rect, char **buffer, int &stride);
void surface_unlock(LPDIRECTDRAWSURFACE7 surf, RECT *rect);

