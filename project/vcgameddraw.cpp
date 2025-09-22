#include "stdafx.h"
#include "resource.h"
#include "ddraw.h"

#include "vcgame.h"
#include "..\game\common.h"

#define INITGUID
#define SafeRelease(x) if (x) { x->Release(); x=NULL; }

static LPDIRECTDRAW7 lpDD=NULL;// DirectDraw�����ָ��
static LPDIRECTDRAWSURFACE7 lpDDSPrimary=NULL;// DirectDraw��ҳ���ָ��
static LPDIRECTDRAWCLIPPER lpClip=NULL; // DirectDraw ���ö���
static LPDIRECTDRAWSURFACE7 lpBmp=NULL; // ��ű���ͼ��ҳ���ָ��

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
  //����BMP
  HBITMAP bit=(HBITMAP) LoadImage(NULL,file_name,IMAGE_BITMAP,0,0,
    LR_DEFAULTSIZE|LR_LOADFROMFILE);
  if (!bit)//����ʧ�ܣ�����NULL
    return NULL;
  //���BMP�ߴ�
  GetObject(bit, sizeof(BITMAP), &bitmap );

  return bit;
}

extern HINSTANCE			g_hInst;			// current instance
HBITMAP bitmap_load(DWORD rcName, BITMAP &bitmap)
{
	//����BMP
	HBITMAP bit=(HBITMAP) LoadBitmap(g_hInst, MAKEINTRESOURCE(rcName));
	if (!bit)//����ʧ�ܣ�����NULL
	{
		DWORD eCode = GetLastError();
		return NULL;
	}
	//���BMP�ߴ�
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

  //����ҳ��
  HRESULT ddrval;
  DDSURFACEDESC2 ddsd;
  //���DDSURFACEDESC2�ṹ
  ZeroMemory(&ddsd,sizeof(ddsd));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);
  //��������ʾ
  ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
  //����ҳ��
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
  //ָ�����
  ddsd.dwWidth =width;
  ddsd.dwHeight =height;
  //��������ҳ��
  ddrval=lpDD->CreateSurface(&ddsd,&surf,NULL);
  //��������ҳ��ָ��
  return surf;
}

void reset_bitmap_surface(LPDIRECTDRAWSURFACE7 surf)
{
	SafeRelease(surf);
}

BOOL InitDDrawEx(HWND hWnd)
{
   DDSURFACEDESC2 ddsd1; // DirectDraw��ҳ������
   DDSURFACEDESC2 ddsd; // DirectDraw��ҳ������
   HRESULT hRet;

  //����ʹ���� if (xxx!=DD_OK) �ķ������д����⣬������õķ���
   if (DirectDrawCreateEx(NULL,(LPVOID*)&lpDD, IID_IDirectDraw7, NULL)!= DD_OK)
	   return FALSE; //����DirectDraw����
   if (lpDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL)!= DD_OK) 
       return FALSE; //����DirectDraw���Ƽ�
   ZeroMemory(&ddsd1, sizeof(ddsd1));
//   if(DD_OK==lpDD->GetDisplayMode(&ddsd1))
//	   if(ddsd1.dwWidth!=1024 && ddsd1.dwHeight!=768 && ddsd1.dwDepth!=32)
		   if (lpDD->SetDisplayMode(1024,768,32,0,DDSDM_STANDARDVGAMODE)!= DD_OK) 
			   return FALSE; //������ʾģʽ
   //��������(������������DirectDraw���Ƽ���,������ҳ��֮ǰ����)
   hRet=lpDD->CreateClipper(NULL,&lpClip,NULL);
   if (hRet != DD_OK) {
     return FALSE;
   }
   lpClip->SetHWnd(0,hWnd);

   //��ʼ������ҳ�棬�����ҳ������
  ZeroMemory(&ddsd, sizeof(ddsd));

  //���ҳ������
  ddsd.dwSize = sizeof(ddsd);//��dwSizeҳ�������Ĵ�С
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps =DDSCAPS_PRIMARYSURFACE;//��ҳ��

  if (lpDD->CreateSurface(&ddsd,&lpDDSPrimary, NULL) != DD_OK )
      return FALSE; //������ҳ��
  lpDDSPrimary->SetClipper(lpClip);//���ü���

  return TRUE;
}
void DeinitDDrawEx()
{
  SafeRelease(lpDDSPrimary);//ɾ����ҳ��
  SafeRelease(lpClip);//ɾ������
  SafeRelease(lpDD);//ɾ��ddraw����
}
void bitmap2surface(LPDIRECTDRAWSURFACE7 surf, HBITMAP bit, RECT dstRect, RECT srcRect)
{ 
    //���BMP�ߴ�
    BITMAP bitmap;
    GetObject(bit, sizeof(BITMAP), &bitmap );

	HDC hdc;
    //ȡ��DC
    surf->GetDC(&hdc);
    //��DCȡ��һ��
    HDC bit_dc=CreateCompatibleDC(hdc);
    //
    SelectObject(bit_dc,bit);
    //����bmp
	BitBlt(hdc,dstRect.left,dstRect.top,
		dstRect.right-dstRect.left,
		dstRect.bottom-dstRect.top,
		bit_dc,srcRect.left,srcRect.top,SRCCOPY);
    //�ͷ�DC
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
	//���DDSURFACEDESC2�ṹ
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	//��������ʾ
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	//����ҳ��
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


