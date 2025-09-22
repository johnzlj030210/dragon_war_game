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

CGMap::CGMap()
{
	m_type_id = g_type_map;

	pTemplate = new CGBitmapTempletMap("..\\pic\\block.bmp");

	m_blockNumX = 50;
	m_blockNumY = 50;
	m_blockSizeX = pTemplate->GetBlockWidth();
	m_blockSizeY = pTemplate->GetBlockHeight();

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = m_blockNumX*m_blockSizeX;
	m_rect.bottom = m_blockNumY*m_blockSizeY;

	m_roll_step = 5;
}

CGMap::CGMap(CGBitmapTempletMap *map)
: m_roll_step(0)
{
	m_type_id = g_type_map;

	pTemplate = map;

	m_blockNumX = 50;
	m_blockNumY = 50;
	m_blockSizeX = pTemplate->GetBlockWidth();
	m_blockSizeY = pTemplate->GetBlockHeight();

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = m_blockNumX*m_blockSizeX;
	m_rect.bottom = m_blockNumY*m_blockSizeY;

	m_size = m_rect;

	m_roll_step = 5;
}

CGMap::~CGMap()
{
}

void CGMap::ShiftTo(int x, int y)
{
	GPOINT pos1;
	pos1.x = x;
	pos1.y = y;

	if(pos1.x<0) pos1.x = 0;
	if(pos1.x+pHostArea->GetWidth()>m_rect.right)
		pos1.x = m_rect.right - pHostArea->GetWidth();
	if(pos1.y<0) pos1.y = 0;
	if(pos1.y+pHostArea->GetHeight()>m_rect.bottom)
		pos1.y = m_rect.bottom - pHostArea->GetHeight();

	SetPosition(pos1);
}
G_SHIFT_RESULT CGMap::Shift(int x, int y)
{
	GPOINT pos1 = GetPosition();
	int ox = pos1.x;
	int oy = pos1.y;

	pos1.x += x;
	pos1.y += y;

	if(pos1.x<0) pos1.x = 0;
	if(pos1.y<0) pos1.y = 0;

	if(pos1.x+pHostArea->GetWidth()>m_rect.right)
		pos1.x = m_rect.right - pHostArea->GetWidth();
	if(pos1.y+pHostArea->GetHeight()>m_rect.bottom)
		pos1.y = m_rect.bottom - pHostArea->GetHeight();
	SetPosition(pos1);
	
	if(ox != pos1.x || oy!=pos1.y)
		return g_shift_ok;
	else
		return g_shift_boundary;
}
void CGMap::UpdateView(bool show)
{
	RECT rect1;
	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);
	CGBitmapTempletMap *tempmap;

	GPOINT pos1 = GetPosition();

	for(int i=0; i<m_blockNumX; i++)
		for(int j=0; j<m_blockNumY; j++){
			tempmap = dynamic_cast<CGBitmapTempletMap*>(pTemplate);
			if(tempmap)
				tempmap->BitmapBlockLayout(surface, rect1, pos1,  i, j);
		}
	if(show)
		GShowSurface(surface);
}

// Change the map's status every delta_t clock, called by the timer automatically
void CGMap::TimerCallback_10MS(int delta_t)
{
	GPOINT pos;
	GPOINT pos1;
	if(pHostArea->IfTrackFocus())
	{
		CGameObject *pObj = pHostArea->GetFocusObject();

		if(g_type_map != pObj->GetTypeId())
		{// if the area focused on a car or a building, turn to it
			int lx, ly, rx, ry, sx, sy;
			pos = pObj->GetPosition();
			pos1 = CoordinateTransformIntoArea(pos);
			// if the object is not shown in the area
			lx = 50; ly = 50; 
			rx = pHostArea->GetWidth()-lx-pObj->GetWidth();
			ry = pHostArea->GetHeight()-ly-pObj->GetHeight();

			sx = pos1.x<lx? pos.x-lx : GetPosition().x;
			sx = pos1.x>rx? pos.x-rx : sx;
			sy = pos1.y<ly? pos.y-ly : GetPosition().y;
			sy = pos1.y>ry? pos.y-ry : sy;

			ShiftTo(sx, sy);
		}
	}
	//else, if the focus is the map
	UpdateView(false);
}


// Process command from keyboard, mouse, or other devices
bool CGMap::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_KEY_LEFT:
		Shift(-m_roll_step, 0);
		break;
	case G_KEY_RIGHT:
		Shift(m_roll_step, 0);
		break;
	case G_KEY_UP:
		Shift(0, -m_roll_step);
		break;
	case G_KEY_DOWN:
		Shift(0, m_roll_step);
		break;
	case G_KEY_A:
		break;
	case G_KEY_D:
		break;
	default:
		return false;
	}
	UpdateView(false);
	return true;
}


// localize the object in the logic map
void CGMap::Localize(CGameObject * pObj)
{
}

void CGMap::Unlocalize(CGameObject * pObj)
{
}

CGameObject * CGMap::WhoIsHere(GPOINT & pos)
{
	return NULL;
}

// Get the width of the map
int CGMap::GetWidth(void)
{
	return m_blockNumX * m_blockSizeX;
}
// Get the height of the map
int CGMap::GetHeight(void)
{
	return m_blockNumY * m_blockSizeY;
}

// load into the area
void CGMap::Load(CGAreaAct * pArea, GPOINT &pos)
{
	pHostArea = pArea;
	m_pos_foot = pos;
	m_origin = pos;
	UpdateView(true);
}
