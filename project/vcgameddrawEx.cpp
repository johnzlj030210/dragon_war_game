#include "stdafx.h"
#include "resource.h"
#include "ddraw.h"

#include "vcgame.h"

#define INITGUID
#define SafeRelease(x) if (x) { x->Release(); x=NULL; }


LPCTSTR PicName="e:\\zlj\\vcgame\\vcgame\\seaside.bmp";

/*=============================================================================
LPDIRECTDRAW7��LPDIRECTDRAWSURFACE7����(7�ǰ汾��)����ddraw.hͷ�ļ���Ԥ����ģ�
ָ��IDirectDraw7��IDirectDrawSurface7���͵ĳ���ָ�루ǰ��ӵ�LP����Long Point����
�Ӻ����õ���"->"������"."Ҳ���Կ�����һ�㡣

DD��DirectDraw����д��DDS��DirectDrawSurface����д��
���ϰ�������ǰѱ�������ΪlpDD��lpDDSXXX��


��ȻVC.net�Դ���DirectX SDK��8.1��ģ�
��������Microsoft��DirectX 8.0��ֹͣ�˶�DirectDraw�ĸ��£�
����DirectDrawĿǰ�����°汾����7.0��
===============================================================================*/

LPDIRECTDRAW7 lpDD=NULL;// DirectDraw�����ָ��
LPDIRECTDRAWSURFACE7 lpDDSPrimary=NULL;// DirectDraw��ҳ���ָ��
LPDIRECTDRAWCLIPPER lpClip=NULL; // DirectDraw ���ö���
LPDIRECTDRAWSURFACE7 lpBmp=NULL; // ��ű���ͼ��ҳ���ָ��
RECT dispRect;
HBITMAP bit;
int surf_width;
int surf_height;
HWND hWnd;
HDC bit_dc;

void DrawImage()
{
  if (lpBmp==NULL)
  {
    return;
  }
//  lpDDSPrimary->SetOverlayPosition(max(0,dispRect.left), max(0,dispRect.right));
  lpDDSPrimary->Blt(NULL,lpBmp,NULL,DDBLT_WAIT,NULL);
  //lpDDSPrimary->Blt(&sourceRect,lpBmp,&sourceRect,DDBLT_WAIT,NULL);
}

LPDIRECTDRAWSURFACE7 bitmap_surface(LPCTSTR file_name, RECT clientRect, int x, int y)
{
  LPDIRECTDRAWSURFACE7 surf;

  RECT windowRect;
  LONG dispWidth, dispHeight;
//  GetClientRect(hWnd, &clientRect);
  GetWindowRect(hWnd, &windowRect);

  dispRect.left= windowRect.left+clientRect.left;
  dispRect.right= windowRect.left+clientRect.right;
  dispRect.top= windowRect.top+clientRect.top;
  dispRect.bottom= windowRect.top+clientRect.bottom;

  dispWidth = dispRect.right-dispRect.left;
  dispHeight = dispRect.bottom-dispRect.top;

//  lpDDSPrimary->SetOverlayPosition(dispRect.left, dispRect.right);

  //����BMP
  bit=(HBITMAP) LoadImage(NULL,file_name,IMAGE_BITMAP,0,0,
    LR_DEFAULTSIZE|LR_LOADFROMFILE);
  if (!bit)//����ʧ�ܣ�����NULL
    return NULL;
  //���BMP�ߴ�
  BITMAP bitmap;
  GetObject(bit, sizeof(BITMAP), &bitmap );
  //����BMP�ߴ�
  surf_width = dispWidth;//bitmap.bmWidth;
  surf_height = dispHeight;//bitmap.bmHeight;

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
  ddsd.dwWidth =1024;//*(1+dispWidth/1024.0);// dispWidth, dispHeight;
  ddsd.dwHeight =768;//*(1+dispHeight/768.0);
  //��������ҳ��
  ddrval=lpDD->CreateSurface(&ddsd,&surf,NULL);
  if (ddrval!=DD_OK) {
    //ʧ�ܵĻ����ͷ�BMP
    DeleteObject(bit);
    return NULL;
  } 
  else //�����ɹ�
  {
	HDC hdc;
    //ȡ��DC
    surf->GetDC(&hdc);
    //��DCȡ��һ��
    /*HDC */bit_dc=CreateCompatibleDC(hdc);
    //
    SelectObject(bit_dc,bit);
    //����bmp
	BitBlt(hdc,dispRect.left,dispRect.top,surf_width,surf_height,bit_dc,x,y,SRCCOPY);
    //�ͷ�DC
    surf->ReleaseDC(hdc);
//    DeleteDC(bit_dc);
  }
  //�ͷ�BMP
  DeleteObject(bit);
  //��������ҳ��ָ��
  return surf;
}

//************************************************************
//������InitDDraw()
//���ܣ�DX��ʼ������
//************************************************************
BOOL InitDDraw(HWND hWnd1, RECT &rect, int x, int y)
{
  DDSURFACEDESC2 ddsd; // DirectDraw��ҳ������
  HRESULT hRet;
  hWnd = hWnd1;
   /*
       ��һ�й���֮ǰ���ǵü�������lib�ļ�
       �ڲ˵�Project-Setting�µ�Linkҳ��Object/Library Modules�������
       "ddraw.lib"��"dxguid.lib" ,���߿��Բ����룬����#define INITGUID����
       PS:���ѡ��setting for All Configurations 
       �������� debug �� release �¶���Ч
   */
   /*
       ���Ҫʹ��DirectDraw�����봴��һ��DirectDraw��������DirectDraw�ӿڵĺ��ġ�
       ��DirectDrawCreateEx( )�������Դ���DirectDraw����
       DirectDrawCreateEx( )��������ddraw.h�ж���ģ�����ԭ�����£�

             HRESULT WINAPI DirectDrawCreateEx(
                GUID FAR *lpGUID, //ָ��DirectDraw�ӿڵ�GUID��ָ�룬NULL��ʾ����Ĭ�ϣ���ǰ��
                LPVOID *lplpDD,     //�������ܳ�ʼ����DirectDraw����ĵ�ַ
                REFIID iid,             //IID_IDirectDraw7����ǰ�汾
                IUnknown FAR *pUnkOuter   //NULL   ����
       ); 
             ���е�DirectDraw�����ķ���ֵ����HRESULT���ͣ�����һ��32λ��ֵ��
       �������óɹ��� "DD_OK"��ʾ��
       ���еĴ���ֵ��־��ͷ��Ϊ"DDERR"���磺
       DDERR_DIRECTDRAWALREADYCREATED
       DDERR_OUTOFMEMORY 
   */
   //����ʹ���� if (xxx!=DD_OK) �ķ������д����⣬������õķ���
   if (DirectDrawCreateEx(NULL,(LPVOID*)&lpDD, IID_IDirectDraw7, NULL)!= DD_OK)
	   return FALSE; //����DirectDraw����
   /*
   DirectDrawCreate�������óɹ���lpDD�Ѿ�ָ����һ��DirectDraw����
   ��������DirectDraw�ӿڵ���߲��쵼���Ժ�Ĳ��趼�������Ŀ���֮�¡�
   ������IDirectDraw7::SetCooperativeLevel( )������DirectDraw�����ϵͳ�Ŀ��Ƽ���
   ����ԭ�����£�
   HRESULT SetCooperativeLevel (HWND hWnd, DWORD dwFlags )
   ��һ�������Ǵ��ھ�������Ǹ���hWnd��ʹDirectDraw��������������ϵ�ϡ�
         �ڶ��������ǿ��Ƽ���־������ʹ��DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN��
   ��ʾ��������DirectDraw�Զ�ռ��ȫ����ʽ������
         
   ���Ƽ�������DirectDraw����������ʾ�豸��ϵͳ���õġ�
   DirectDraw���Ƽ�һ�㱻��������Ӧ�ó�����������ȫ��ģʽ���������ռģʽͬʱʹ�ã���
   ���������ڴ���ģʽ����DirectDraw�Ŀ��Ƽ����������������
         (1)����Ctrl+Alt+Del���������������ڶ�ռģʽ��ΪDDSCL_ALLOWREBOOT����
         (2)�������DirectDrawӦ�ó�����С����ԭ (DDSCL_NOWINDOWCHANGES)��
         
   ��ͨ�Ŀ��Ƽ�(DDSCL_NORMAL)�������ǵ�DirectDrawӦ�ó����Դ��ڵ���ʽ���С�
   �����ֿ��Ƽ��£����ǽ����ܸı���ʾ���ֱ��ʣ�����л�ҳ����(����һ����Ҫ�Ĳ���)��
   ����֮�⣬����Ҳ���ܹ�������Щ����Դ�������ҷ�Ӧ�ĺ�������Lock()��
         ��Ӧ�ó���Ϊȫ�����Ҷ�ռ�Ŀ��Ƽ�ʱ�����ǾͿ��Գ�ֵ�����Ӳ����Դ�ˡ�
   ��ʱ����Ӧ�ó����Կɴ���ҳ�桢ʹ��DirectDraw��GDI�ĺ�����ֻ���޷��ı���ʾģʽ��
   */ 
         if (lpDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL)!= DD_OK) 
//         if (lpDD->SetCooperativeLevel(hWnd,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)!= DD_OK) 
               return FALSE; //����DirectDraw���Ƽ�
   /*
   ��һ������ʹ��IDirectDraw7::SetDisplayMode( )��������ʾģʽ����ԭ��Ϊ��
   HRESULT SetDisplayMode(
   DWORD dwWidth, 
   DWORD dwHeight, //dwWidth and dwHeight����������ʾģʽ�Ŀ�Ⱥ͸߶ȡ�
   DWORD dwBPP, //dwBPP����������ʾģʽ����ɫλ����
   DWORD dwRefreshRate, //dwRefreshRate������Ļ��ˢ���ʣ�0Ϊʹ��Ĭ��ֵ��
   DWORD dwFlags //dwFlags����Ψһ��Ч��ֵ��DDSDM_STANDARDVGAMODE
   );
   */
         if (lpDD->SetDisplayMode(1024,768,32,0,DDSDM_STANDARDVGAMODE)!= DD_OK) 
               return FALSE; //������ʾģʽ
   /*
   HRESULT CreateClipper(
   DWORD dwFlags, //���ڲ��ã�������Ϊ0
   LPDIRECTDRAWCLIPPER FAR *lplpDDClipper,//ָ������������ָ��
   IUnknown FAR *pUnkOuter // //���ڲ��ã�������ΪNULL
   );
   �ɹ���ʱ�򷵻�DD_OK
   */

   //��������(������������DirectDraw���Ƽ���,������ҳ��֮ǰ����)
   hRet=lpDD->CreateClipper(NULL,&lpClip,NULL);
   if (hRet != DD_OK) {
     return FALSE;
   }
   lpClip->SetHWnd(0,hWnd);

       /*
     ����һ��ҳ��֮ǰ��������Ҫ���һ��DDSURFACEDESC2�ṹ��
     ����DirectDraw Surface Description����д����˼��DirectDraw��ҳ��������
     ���Ľṹ�ǳ��Ӵ�����ֻ����һ����򵥵Ľ��ܡ�
     Ҫע����������˽ṹǰһ��Ҫ������գ�
  */

  //��ʼ������ҳ�棬�����ҳ������
  ZeroMemory(&ddsd, sizeof(ddsd));

  //���ҳ������
       ddsd.dwSize = sizeof(ddsd);//��dwSizeҳ�������Ĵ�С
  ddsd.dwFlags = DDSD_CAPS;
       ddsd.ddsCaps.dwCaps =DDSCAPS_PRIMARYSURFACE;//��ҳ��
       /*
  CreateSurface( )�����ĵ�һ�������Ǳ������ҳ����Ϣ��DDSURFACEDESC2�ṹ�ĵ�ַ��
  �˴�Ϊ&ddsd��
  �ڶ��������ǽ�����ҳ��ָ��ĵ�ַ���˴�Ϊ&lpDDSPrimary��
  �������������ڱ���ΪNULL��Ϊ�ú�����������
       ����������óɹ���lpDDSPrimary����Ϊһ���Ϸ�����ҳ�����
  ������ǰ���Ѿ������˸ó���Ĺ���ģʽΪ��ռ��ȫ����
  ���ԣ���ʱ��ҳ���������ʵ���������ǵ�������ʾ��Ļ��
  ����ҳ���������Ƶ�ͼ�ν�������ӳ�����ǵ���ʾ��Ļ�ϡ�
  */
         if (lpDD->CreateSurface(&ddsd,&lpDDSPrimary, NULL) != DD_OK )
             return FALSE; //������ҳ��
       lpDDSPrimary->SetClipper(lpClip);//���ü���
  //����ͼƬ
  lpBmp=bitmap_surface(PicName, rect, x, y);
  //��ͼ
       DrawImage();
  return TRUE;
}
void DDrawUpdate(RECT &clientRect, int x, int y)
{
  RECT windowRect;
  LONG dispWidth, dispHeight;
//  GetClientRect(hWnd, &clientRect);
  GetWindowRect(hWnd, &windowRect);

  dispRect.left= windowRect.left+clientRect.left;
  dispRect.right= windowRect.left+clientRect.right;
  dispRect.top= windowRect.top+clientRect.top;
  dispRect.bottom= windowRect.top+clientRect.bottom;

  dispWidth = dispRect.right-dispRect.left;
  dispHeight = dispRect.bottom-dispRect.top;
    //ȡ��DC
  HDC hdc;
  lpBmp->GetDC(&hdc);
  //����bmp
  BitBlt(hdc,dispRect.left,dispRect.top,dispWidth,dispHeight,bit_dc,x,y,SRCCOPY);
  //�ͷ�DC
  lpBmp->ReleaseDC(hdc);

  //��ͼ
  DrawImage();
}
void GetBmpSize(LONG &width, LONG &height)
{
  //���BMP�ߴ�
  BITMAP bitmap;
  GetObject(bit, sizeof(BITMAP), &bitmap );
  //����BMP�ߴ�
  width = bitmap.bmWidth;
  height = bitmap.bmHeight;
}
void Cleanup(void)
{
  //�ͷŽӿ�
  SafeRelease(lpBmp);
  SafeRelease(lpDDSPrimary);//ɾ����ҳ��
  SafeRelease(lpClip);//ɾ������
  SafeRelease(lpDD);//ɾ��ddraw����
  //ps:�봴����˳�������෴
}
