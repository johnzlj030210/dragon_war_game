#include "stdafx.h"
#include "resource.h"
#include "ddraw.h"

#include "vcgame.h"

#define INITGUID
#define SafeRelease(x) if (x) { x->Release(); x=NULL; }


LPCTSTR PicName="e:\\zlj\\vcgame\\vcgame\\seaside.bmp";

/*=============================================================================
LPDIRECTDRAW7和LPDIRECTDRAWSURFACE7类型(7是版本号)是在ddraw.h头文件里预定义的，
指向IDirectDraw7和IDirectDrawSurface7类型的长型指针（前面加的LP代表Long Point），
从后面用的是"->"而不是"."也可以看出这一点。

DD是DirectDraw的缩写，DDS是DirectDrawSurface的缩写，
因此习惯上我们把变量名起为lpDD和lpDDSXXX。


虽然VC.net自带的DirectX SDK是8.1版的，
但是由于Microsoft从DirectX 8.0起停止了对DirectDraw的更新，
所以DirectDraw目前的最新版本还是7.0。
===============================================================================*/

LPDIRECTDRAW7 lpDD=NULL;// DirectDraw对象的指针
LPDIRECTDRAWSURFACE7 lpDDSPrimary=NULL;// DirectDraw主页面的指针
LPDIRECTDRAWCLIPPER lpClip=NULL; // DirectDraw 剪裁对象
LPDIRECTDRAWSURFACE7 lpBmp=NULL; // 存放背景图的页面的指针
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

  //载入BMP
  bit=(HBITMAP) LoadImage(NULL,file_name,IMAGE_BITMAP,0,0,
    LR_DEFAULTSIZE|LR_LOADFROMFILE);
  if (!bit)//载入失败，返回NULL
    return NULL;
  //获得BMP尺寸
  BITMAP bitmap;
  GetObject(bit, sizeof(BITMAP), &bitmap );
  //保存BMP尺寸
  surf_width = dispWidth;//bitmap.bmWidth;
  surf_height = dispHeight;//bitmap.bmHeight;

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
  ddsd.dwWidth =1024;//*(1+dispWidth/1024.0);// dispWidth, dispHeight;
  ddsd.dwHeight =768;//*(1+dispHeight/768.0);
  //创建离屏页面
  ddrval=lpDD->CreateSurface(&ddsd,&surf,NULL);
  if (ddrval!=DD_OK) {
    //失败的话，释放BMP
    DeleteObject(bit);
    return NULL;
  } 
  else //创建成功
  {
	HDC hdc;
    //取得DC
    surf->GetDC(&hdc);
    //与DC取得一致
    /*HDC */bit_dc=CreateCompatibleDC(hdc);
    //
    SelectObject(bit_dc,bit);
    //传送bmp
	BitBlt(hdc,dispRect.left,dispRect.top,surf_width,surf_height,bit_dc,x,y,SRCCOPY);
    //释放DC
    surf->ReleaseDC(hdc);
//    DeleteDC(bit_dc);
  }
  //释放BMP
  DeleteObject(bit);
  //返回离屏页面指针
  return surf;
}

//************************************************************
//函数：InitDDraw()
//功能：DX初始化函数
//************************************************************
BOOL InitDDraw(HWND hWnd1, RECT &rect, int x, int y)
{
  DDSURFACEDESC2 ddsd; // DirectDraw的页面描述
  HRESULT hRet;
  hWnd = hWnd1;
   /*
       在一切工作之前，记得加入两个lib文件
       在菜单Project-Setting下的Link页的Object/Library Modules栏里加入
       "ddraw.lib"和"dxguid.lib" ,后者可以不加入，而用#define INITGUID代替
       PS:最好选择setting for All Configurations 
       这样会在 debug 和 release 下都有效
   */
   /*
       如果要使用DirectDraw，必须创建一个DirectDraw对象，它是DirectDraw接口的核心。
       用DirectDrawCreateEx( )函数可以创建DirectDraw对象，
       DirectDrawCreateEx( )函数是在ddraw.h中定义的，它的原型如下：

             HRESULT WINAPI DirectDrawCreateEx(
                GUID FAR *lpGUID, //指向DirectDraw接口的GUID的指针，NULL表示采用默认（当前）
                LPVOID *lplpDD,     //用来接受初始化的DirectDraw对象的地址
                REFIID iid,             //IID_IDirectDraw7，当前版本
                IUnknown FAR *pUnkOuter   //NULL   保留
       ); 
             所有的DirectDraw函数的返回值都是HRESULT类型，它是一个32位的值。
       函数调用成功用 "DD_OK"表示，
       所有的错误值标志开头都为"DDERR"，如：
       DDERR_DIRECTDRAWALREADYCREATED
       DDERR_OUTOFMEMORY 
   */
   //这里使用了 if (xxx!=DD_OK) 的方法进行错误检测，这是最常用的方法
   if (DirectDrawCreateEx(NULL,(LPVOID*)&lpDD, IID_IDirectDraw7, NULL)!= DD_OK)
	   return FALSE; //创建DirectDraw对象
   /*
   DirectDrawCreate函数调用成功后，lpDD已经指向了一个DirectDraw对象，
   它是整个DirectDraw接口的最高层领导，以后的步骤都是在它的控制之下。
   我们用IDirectDraw7::SetCooperativeLevel( )来设置DirectDraw程序对系统的控制级。
   它的原型如下：
   HRESULT SetCooperativeLevel (HWND hWnd, DWORD dwFlags )
   第一个参数是窗口句柄，我们给它hWnd，使DirectDraw对象与主窗口联系上。
         第二个参数是控制级标志。这里使用DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN，
   表示我们期望DirectDraw以独占和全屏方式工作。
         
   控制级描述了DirectDraw是怎样与显示设备及系统作用的。
   DirectDraw控制级一般被用来决定应用程序是运行于全屏模式（必须与独占模式同时使用），
   还是运行于窗口模式。但DirectDraw的控制级还可设置如下两项：
         (1)允许按Ctrl+Alt+Del重新启动（仅用于独占模式，为DDSCL_ALLOWREBOOT）。
         (2)不允许对DirectDraw应用程序最小化或还原 (DDSCL_NOWINDOWCHANGES)。
         
   普通的控制级(DDSCL_NORMAL)表明我们的DirectDraw应用程序将以窗口的形式运行。
   在这种控制级下，我们将不能改变显示器分辨率，或进行换页操作(这是一个重要的操作)。
   除此之外，我们也不能够调用那些会对显存产生激烈反应的函数，如Lock()。
         当应用程序为全屏并且独占的控制级时，我们就可以充分的利用硬件资源了。
   此时其它应用程序仍可创建页面、使用DirectDraw或GDI的函数，只是无法改变显示模式。
   */ 
         if (lpDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL)!= DD_OK) 
//         if (lpDD->SetCooperativeLevel(hWnd,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)!= DD_OK) 
               return FALSE; //设置DirectDraw控制级
   /*
   下一步我们使用IDirectDraw7::SetDisplayMode( )来设置显示模式，其原形为：
   HRESULT SetDisplayMode(
   DWORD dwWidth, 
   DWORD dwHeight, //dwWidth and dwHeight用来设置显示模式的宽度和高度。
   DWORD dwBPP, //dwBPP用来设置显示模式的颜色位数。
   DWORD dwRefreshRate, //dwRefreshRate设置屏幕的刷新率，0为使用默认值。
   DWORD dwFlags //dwFlags现在唯一有效的值是DDSDM_STANDARDVGAMODE
   );
   */
         if (lpDD->SetDisplayMode(1024,768,32,0,DDSDM_STANDARDVGAMODE)!= DD_OK) 
               return FALSE; //设置显示模式
   /*
   HRESULT CreateClipper(
   DWORD dwFlags, //现在不用，必须设为0
   LPDIRECTDRAWCLIPPER FAR *lplpDDClipper,//指向剪裁器对象的指针
   IUnknown FAR *pUnkOuter // //现在不用，必须设为NULL
   );
   成功的时候返回DD_OK
   */

   //创剪裁器(必须在设置完DirectDraw控制级后,创建主页面之前进行)
   hRet=lpDD->CreateClipper(NULL,&lpClip,NULL);
   if (hRet != DD_OK) {
     return FALSE;
   }
   lpClip->SetHWnd(0,hWnd);

       /*
     创建一个页面之前，首先需要填充一个DDSURFACEDESC2结构，
     它是DirectDraw Surface Description的缩写，意思是DirectDraw的页面描述。
     它的结构非常庞大，这里只能作一个最简单的介绍。
     要注意的是在填充此结构前一定要将其清空！
  */

  //开始创建主页面，先清空页面描述
  ZeroMemory(&ddsd, sizeof(ddsd));

  //填充页面描述
       ddsd.dwSize = sizeof(ddsd);//给dwSize页面描述的大小
  ddsd.dwFlags = DDSD_CAPS;
       ddsd.ddsCaps.dwCaps =DDSCAPS_PRIMARYSURFACE;//主页面
       /*
  CreateSurface( )函数的第一个参数是被填充了页面信息的DDSURFACEDESC2结构的地址，
  此处为&ddsd；
  第二个参数是接收主页面指针的地址，此处为&lpDDSPrimary；
  第三个参数现在必须为NULL，为该函数所保留。
       如果函数调用成功，lpDDSPrimary将成为一个合法的主页面对象。
  由于在前面已经设置了该程序的工作模式为独占和全屏，
  所以，此时主页面所代表的实际上是我们的整个显示屏幕。
  在主页面上所绘制的图形将立即反映到我们的显示屏幕上。
  */
         if (lpDD->CreateSurface(&ddsd,&lpDDSPrimary, NULL) != DD_OK )
             return FALSE; //创建主页面
       lpDDSPrimary->SetClipper(lpClip);//设置剪裁
  //载入图片
  lpBmp=bitmap_surface(PicName, rect, x, y);
  //画图
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
    //取得DC
  HDC hdc;
  lpBmp->GetDC(&hdc);
  //传送bmp
  BitBlt(hdc,dispRect.left,dispRect.top,dispWidth,dispHeight,bit_dc,x,y,SRCCOPY);
  //释放DC
  lpBmp->ReleaseDC(hdc);

  //画图
  DrawImage();
}
void GetBmpSize(LONG &width, LONG &height)
{
  //获得BMP尺寸
  BITMAP bitmap;
  GetObject(bit, sizeof(BITMAP), &bitmap );
  //保存BMP尺寸
  width = bitmap.bmWidth;
  height = bitmap.bmHeight;
}
void Cleanup(void)
{
  //释放接口
  SafeRelease(lpBmp);
  SafeRelease(lpDDSPrimary);//删除主页面
  SafeRelease(lpClip);//删除剪裁
  SafeRelease(lpDD);//删除ddraw对象
  //ps:与创建的顺序正好相反
}
