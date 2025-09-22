#include "stdafx.h"
#include "GameLoad.h"
#include "typedefines.h"
#include "..\wrapper\\wrapper.h"
//#include "Resource.h"
#include "GameArea.h"
#include ".\gameload.h"
#include ".\GBuilding.h"
#include "common.h"
#include "GLogicMap.h"
#include "GSmoke.h"
#include "GBullet.h"
#include "GCar.h"

CGCar::CGCar()
: delta_x(0)
, delta_y(0)
{
}

CGCar::~CGCar()
{
}

void CGCar::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGameObject::Load(pArea, pos);
	IGWalkAutoAx::Load(pArea, this);
}

void CGCar::Unload(void)
{
	CGameObject::Unload();
	IGWalkAutoAx::Unload();
}

// Shift the car to the point(x, y)
G_SHIFT_RESULT CGCar::Shift(int x, int y)
{
	int ox, oy;
	GPOINT pos1;
	GPOINT init_pos = GetPositionFoot();
	ox = init_pos.x + x;
	oy = init_pos.y + y;

	RECT rect1;
	pHostArea->GetMapRect(rect1);

	if(ox>=0 && oy<=rect1.bottom
		&& ox+(m_size.right - m_size.left)<=rect1.right 
		&& oy-(m_size.bottom - m_size.top)>=rect1.top){

			pos1.x = ox;
			pos1.y = oy;
			SetPositionFoot(pos1);

			if(x!=0 || y!=0){
				if(CheckCollideOthers()){
					SetPositionFoot(init_pos); // get back
					return g_shift_collide;
				}else{
					pHostArea->GetCurrentMap()->Localize(this);
					return g_shift_ok;
				}
			}else
				return g_shift_ok;
		}
	return g_shift_boundary;
}

// Change and show the car
void CGCar::UpdateView(bool show)
{
	RECT rect1;
	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);
	CGBitmapTempletCar *tempcar;

	GPOINT pos = CoordinateTransformIntoArea(GetPosition());
	// display focus bar
	if(this == dynamic_cast<CGCar *>(pHostArea->GetFocusObject()))
	{
		RECT rect;
		rect.left = pos.x+rect1.left+m_size.left; 
		rect.top = pos.y+rect1.top+pTemplate->m_tall;
		rect.right = pos.x+rect1.left+m_size.right;
		rect.bottom = pos.y+rect1.top+m_size.bottom;
		SHOW_FOCUS_BAR(rect);
	}

	tempcar = dynamic_cast<CGBitmapTempletCar*>(pTemplate);
	if(tempcar)
		tempcar->BitmapBlockLayout(surface, rect1, pos, m_angle);

	if(show)
		GShowSurface(surface);
}

// Move the car to the point(x,y)
void CGCar::ShiftTo(int x, int y)
{
	GPOINT pos1;
	pos1.x = x;
	pos1.y = y;
	SetPositionFoot(pos1);
}

// Change the car's status every delta_t clock, called by the timer automatically
void CGCar::TimerCallback_10MS(int delta_t)
{
	double dx = 0, dy = 0;
	G_SHIFT_RESULT status;

	WalkAutoFindPath(delta_x, delta_y);

	if(g_shift_ok != (status=Shift((int)delta_x, (int)delta_y)))
		ComewithBoundary(status);

	delta_x -= (int)delta_x;//cumulative for fractional shift step
	delta_y -= (int)delta_y;

	UpdateView(false);
}

void CGCar::TimerCallback_100MS(int delta_t)
{
}
// timer division
void CGCar::TimerCallback_500MS(int delta_t)
{
}

void CGCar::TimerCallback_Test(int delta_t)
{
}
// show a high light bar under the focus car object
void CGCar::ShowFocusBar(RECT &rect)
{
	int stride;
	char *ptr = 0;
	long *ptr1;
	RECT rect0 = rect;
	pHostArea->LimitWithin(rect0);
	RECT rect1 = rect0;

	GLockDisplayBuffer(GetObjectSurface(0), rect1, &ptr, stride);
	if(!ptr)
		return;

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int sector;

	if(m_blood <= 0)
		sector = rect.left;
	else if(m_blood >= 100)
		sector =rect.right;
	else
		sector = rect.left + width*m_blood/100;
		
	for(int j=rect.left; j<sector; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.top; i<rect.top+4; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (long *)ptr + (i-rect1.top)*stride/4 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_FRONT;
				}
			}
		}
	}
	for(int j=sector; j<rect.right; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.top; i<rect.top+4; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (long *)ptr + (i-rect1.top)*stride/4 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_BACK;
				}
			}
		}
	}

	GUnlockDisplayBuffer(GetObjectSurface(0), rect1);
}

// show a high light bar under the focus car object
void CGCar::ShowFocusBar_16bit(RECT &rect)
{
	int stride;
	char *ptr = 0;
	short *ptr1;
	RECT rect0 = rect;
	pHostArea->LimitWithin(rect0);
	RECT rect1 = rect0;

	GLockDisplayBuffer(GetObjectSurface(0), rect1, &ptr, stride);
	if(!ptr)
		return;

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int sector;

	if(m_blood <= 0)
		sector = rect.left;
	else if(m_blood >= 100)
		sector =rect.right;
	else
		sector = rect.left + width*m_blood/100;
		
	for(int j=rect.left; j<sector; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.top; i<rect.top+4; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (short *)ptr + (i-rect1.top)*stride/2 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_FRONT_16B;
				}
			}
		}
	}
	for(int j=sector; j<rect.right; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.top; i<rect.top+4; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (short *)ptr + (i-rect1.top)*stride/2 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_BACK_16B;
				}
			}
		}
	}

	GUnlockDisplayBuffer(GetObjectSurface(0), rect1);
}
