#include "stdafx.h"
#include "resource.h"
#include "ddraw.h"

#include "vcgame.h"
#include "..\game\common.h"

#define INITGUID
#define SafeRelease(x) if (x) { x->Release(); x=NULL; }

static LPDIRECTDRAW7 lpDD=NULL;// DirectDraw对象的指针
static LPDIRECTDRAWSURFACE7 lpDDSPrimary=NULL;// DirectDraw主页面的指针
static LPDIRECTDRAWCLIPPER lpClip=NULL; // DirectDraw 剪裁对象
static LPDIRECTDRAWSURFACE7 lpBmp=NULL; // 存放背景图的页面的指针

void DrawImage()
{
  if (lpBmp==NULL)
  {
    return;
  }
  lpDDSPrimary->Blt(NULL,lpBmp,NULL,DDBLT_WAIT,NULL);
}

HBITMAP bitmap_load(LPCTSTR file_name, BITMAP &bitmap)
{
  //载入BMP
  HBITMAP bit=(HBITMAP) LoadImage(NULL,file_name,IMAGE_BITMAP,0,0,
    LR_DEFAULTSIZE|LR_LOADFROMFILE);
  if (!bit)//载入失败，返回NULL
    return NULL;
  //获得BMP尺寸
  GetObject(bit, sizeof(BITMAP), &bitmap );

  return bit;
}

extern HINSTANCE			g_hInst;			// current instance
HBITMAP bitmap_load(DWORD rcName, BITMAP &bitmap)
{
	//载入BMP
	HBITMAP bit=(HBITMAP) LoadBitmap(g_hInst, MAKEINTRESOURCE(rcName));
	if (!bit)//载入失败，返回NULL
	{
		DWORD eCode = GetLastError();
		return NULL;
	}
	//获得BMP尺寸
	GetObject(bit, sizeof(BITMAP), &bitmap );

	return bit;
}

void bitmap_unload(HBITMAP bit)
{
	  DeleteObject(bit);
}

LPDIRECTDRAWSURFACE7 setup_bitmap_surface(int width, int height)
{
  LPDIRECTDRAWSURFACE7 surf;

  //创建页面
  HRESULT ddrval;
  DDSURFACEDESC2 ddsd;
  //清空DDSURFACEDESC2结构
  ZeroMemory(&ddsd,sizeof(ddsd));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);
  //设置填充标示
  ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
  //离屏页面
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
  //指定宽高
  ddsd.dwWidth =width;
  ddsd.dwHeight =height;
  //创建离屏页面
  ddrval=lpDD->CreateSurface(&ddsd,&surf,NULL);
  //返回离屏页面指针
  return surf;
}

void reset_bitmap_surface(LPDIRECTDRAWSURFACE7 surf)
{
	SafeRelease(surf);
}

BOOL InitDDrawEx(HWND hWnd)
{
   DDSURFACEDESC2 ddsd1; // DirectDraw的页面描述
   DDSURFACEDESC2 ddsd; // DirectDraw的页面描述
   HRESULT hRet;

  //这里使用了 if (xxx!=DD_OK) 的方法进行错误检测，这是最常用的方法
   if (DirectDrawCreateEx(NULL,(LPVOID*)&lpDD, IID_IDirectDraw7, NULL)!= DD_OK)
	   return FALSE; //创建DirectDraw对象
   if (lpDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL)!= DD_OK) 
       return FALSE; //设置DirectDraw控制级
   ZeroMemory(&ddsd1, sizeof(ddsd1));
//   if(DD_OK==lpDD->GetDisplayMode(&ddsd1))
//	   if(ddsd1.dwWidth!=1024 && ddsd1.dwHeight!=768 && ddsd1.dwDepth!=32)
		   if (lpDD->SetDisplayMode(1024,768,32,0,DDSDM_STANDARDVGAMODE)!= DD_OK) 
			   return FALSE; //设置显示模式
   //创剪裁器(必须在设置完DirectDraw控制级后,创建主页面之前进行)
   hRet=lpDD->CreateClipper(NULL,&lpClip,NULL);
   if (hRet != DD_OK) {
     return FALSE;
   }
   lpClip->SetHWnd(0,hWnd);

   //开始创建主页面，先清空页面描述
  ZeroMemory(&ddsd, sizeof(ddsd));

  //填充页面描述
  ddsd.dwSize = sizeof(ddsd);//给dwSize页面描述的大小
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps =DDSCAPS_PRIMARYSURFACE;//主页面

  if (lpDD->CreateSurface(&ddsd,&lpDDSPrimary, NULL) != DD_OK )
      return FALSE; //创建主页面
  lpDDSPrimary->SetClipper(lpClip);//设置剪裁

  return TRUE;
}
void DeinitDDrawEx()
{
  SafeRelease(lpDDSPrimary);//删除主页面
  SafeRelease(lpClip);//删除剪裁
  SafeRelease(lpDD);//删除ddraw对象
}
void bitmap2surface(LPDIRECTDRAWSURFACE7 surf, HBITMAP bit, RECT dstRect, RECT srcRect)
{ 
    //获得BMP尺寸
    BITMAP bitmap;
    GetObject(bit, sizeof(BITMAP), &bitmap );

	HDC hdc;
    //取得DC
    surf->GetDC(&hdc);
    //与DC取得一致
    HDC bit_dc=CreateCompatibleDC(hdc);
    //
    SelectObject(bit_dc,bit);
    //传送bmp
	BitBlt(hdc,dstRect.left,dstRect.top,
		dstRect.right-dstRect.left,
		dstRect.bottom-dstRect.top,
		bit_dc,srcRect.left,srcRect.top,SRCCOPY);
    //释放DC
    surf->ReleaseDC(hdc);
    DeleteDC(bit_dc);
}
void surface2surface(LPDIRECTDRAWSURFACE7 dest, LPDIRECTDRAWSURFACE7 src, RECT dstRect, RECT srcRect)
{
	dest->BltFast(dstRect.left, dstRect.top, src, &srcRect, DDBLTFAST_WAIT);
}

void surface2surface_overlay(LPDIRECTDRAWSURFACE7 dest, LPDIRECTDRAWSURFACE7 src, RECT dstRect, RECT srcRect)
{
 	DDCOLORKEY ddck;

	ddck.dwColorSpaceLowValue = RGB(255,0,255);
    ddck.dwColorSpaceHighValue = RGB(255,0,255);

	src->SetColorKey(DDCKEY_SRCBLT, &ddck);
	dest->BltFast(dstRect.left, dstRect.top, src, &srcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

void DrawImageSurface(LPDIRECTDRAWSURFACE7 surface)
{
	lpBmp = surface;
	DrawImage();
}

void DrawImageSurfaceOverlay(LPDIRECTDRAWSURFACE7 surface)
{
 	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = RGB(255,0,255);
    ddck.dwColorSpaceHighValue = RGB(255,0,255);
	RECT rect;
	rect.left=100; rect.top=200;
	rect.right = 300; rect.bottom = 400;
	
  lpDDSPrimary->Blt(NULL,surface,NULL,DDBLT_WAIT | DDBLTFAST_SRCCOLORKEY,NULL);
}

void surface_lock(LPDIRECTDRAWSURFACE7 surf, RECT *rect, char **buffer, int &stride)
{
	DDSURFACEDESC2 ddsd;
	//清空DDSURFACEDESC2结构
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	//设置填充标示
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	//离屏页面
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;

	if(DD_OK == surf->Lock(rect, &ddsd, DDLOCK_SURFACEMEMORYPTR, NULL))
	{
		stride = ddsd.lPitch;
		*buffer = (char *)ddsd.lpSurface;
	}
}

void surface_unlock(LPDIRECTDRAWSURFACE7 surf, RECT *rect)
{
	surf->Unlock(rect);
}


