#include "stdafx.h"
#include "GameLoad.h"
#include "typedefines.h"
#include "..\wrapper\\wrapper.h"
#include "GameArea.h"
#include "GCarMoto.h"
#include "GBuilding.h"
#include "GCarMotoPeng.h"
#include "GSmoke.h"
#include "GBomb.h"
#include "GLogicMap.h"
#include ".\gamearea.h"
#include "common.h"
#include "GWarrior.h"

CGArea::CGArea()
: main_Surface(0)
, second_Surface(0)
{
}
CGArea::~CGArea()
{
}
CGAreaAct::CGAreaAct()
: pFocusObj(NULL)
, pMapCurrent(NULL)
, pBuildTemplate(NULL)
, m_IsTrackFocus(false)
, pSmokeTemplate(NULL)
, m_bombs(NULL)
, pWarriorTemplate(NULL)
, pCavalryTemplate(NULL)
{
	main_Surface = GInitMainSurface();

	GGetWindowRect(m_rect);
	m_rect.right -= TINY_AREA_WIDTH;

	m_pos.x = 0;
	m_pos.y = 0;

	pBuildTemplate = new CGBitmapTempletBuild(IDB_BITMAP_BUILD);
	pMapTemplate = new CGBitmapTempletMap(IDB_BITMAP_BLOCK);
	pCarTemplate[0] = new CGBitmapTempletCar(IDB_BITMAP_KIA);
	pCarTemplate[1] = new CGBitmapTempletCar(IDB_BITMAP_FALALI);
	pCarTemplate[2] = new CGBitmapTempletCar(IDB_BITMAP_PIKA);
	pCarTemplate[3] = new CGBitmapTempletCar(IDB_BITMAP_MINECAR);
	pSmokeTemplate = new CGBitmapTempletSmoke(IDB_BITMAP_SMOKE);
	pBombTemplate = new CGBitmapTempletBomb(IDB_BITMAP_BOMB);
	pBulletTemplate = new CGBitmapTempletBullet(IDB_BITMAP_BULLET);
	pConstructTemplate = new CGBitmapTempletConstructor(IDB_BITMAP_CONSTRUCT);
	pFlagTemplate = new CGBitmapTempletFlag(IDB_BITMAP_FLAG);
	pBowmanTemplate = new CGBitmapTempletBowman(IDB_BITMAP_BOWMAN);
#ifndef WIN_MOBILE
	pWarriorTemplate = new CGBitmapTempletWarrior(IDB_BITMAP_WARRIOR);
	pCavalryTemplate = new CGBitmapTempletCavalry(IDB_BITMAP_HORSE);
	pChariotTemplate = new CGBitmapTempletChariot(IDB_BITMAP_CHARIOT);
#endif

	m_cursor_pos.x = -1; m_cursor_pos.y = -1;
	m_bombs = new CGBomb(pBombTemplate);

	GPOINT pos;
	pos.x = 0; pos.y = 0;

	CGMap *pMap = new CGMap(pMapTemplate);
	pFocusObj = pMap;
	pMapCurrent = pMap;
	// map first
	pMap->Load(this, pos);
	m_queue.push(pMap);
}

CGAreaAct::~CGAreaAct()
{
	GDeinitBitmapSurface(main_Surface);

	delete m_bombs;

	delete pBuildTemplate;
	delete pMapTemplate;
	delete pCarTemplate[0];
	delete pCarTemplate[1];
	delete pCarTemplate[2];
	delete pCarTemplate[3];
	delete pSmokeTemplate;
	delete pBombTemplate;
	delete pBulletTemplate;
	delete pConstructTemplate;
	delete pFlagTemplate;
	delete pWarriorTemplate;
	delete pCavalryTemplate;
	delete pBowmanTemplate;
	delete pChariotTemplate;
}

void CGAreaAct::GetRect(RECT &rect1)
{
	rect1 = m_rect;
}

// shift map the center at x,y
void CGAreaAct::ShiftGameMapTo(int x, int y)
{
	int x1, y1;
	int x2, y2;
	x1 = x - GetWidth()/2;
	y1 = y - GetHeight()/2;
	x2 = pMapCurrent->GetWidth() - GetWidth();
	y2 = pMapCurrent->GetHeight() - GetHeight();
	if(x1>x2)	x1 = x2;
	if(y1>y2)	y1 = y2;
	if(x1<0)	x1 = 0;
	if(y1<0)	y1 = 0;
	pMapCurrent->ShiftTo(x1, y1);
	pMapCurrent->UpdateView(false);
}

void CGAreaAct::ShiftGampMap(int x, int y)
{
	pMapCurrent->Shift(x,y);
	pMapCurrent->UpdateView(false);
}

// on message that window is redrawed
void CGAreaAct::OnRedraw(void)
{
	CGameObject *pObj;

	GGetWindowRect(m_rect);
	m_rect.right -= TINY_AREA_WIDTH;

	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		pObj->UpdateView(false);
	}
	GShowSurface(main_Surface);
}

// Change the area's status every delta_t clock, called by the timer automatically
void CGAreaAct::TimerCallback_10MS(int delta_t)
{
	CGameObject *pObj;

	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		if(!pObj->Unloaded())
			pObj->TimerCallback_10MS(delta_t);
	}
	m_bombs->TimerCallback_10MS(delta_t);
	// map roll by cursor
	ShiftGameMapWithCursor();
	GShowSurface(main_Surface);
}
// tiemer division
void CGAreaAct::TimerCallback_500MS(int delta_t)
{
	CGameObject *pObj;
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		pObj->TimerCallback_500MS(delta_t);
	}
	m_bombs->TimerCallback_500MS(delta_t);
}

// tiemer division
void CGAreaAct::TimerCallback_2S(int delta_t)
{
	CGameObject *pObj;
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		pObj->TimerCallback_2S(delta_t);
	}
}

// tiemer division
void CGAreaAct::TimerCallback_100MS(int delta_t)
{
	CGameObject *pObj;
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		if(pObj->Unloaded()){
			if(pFocusObj == pObj)
				pFocusObj = pMapCurrent;
			pMapCurrent->Unlocalize(pObj);
			pObj->Unload();
			if(pObj->CanBeDeletedSafely())
			{
				m_queue.erase(pObj);
				delete pObj;
			}
		}else{
			pObj->TimerCallback_100MS(delta_t);
		}
	}
	m_bombs->TimerCallback_100MS(delta_t);
}

// Change the area's status every delta_t clock, called by the timer automatically
void CGAreaAct::TimerCallback_Test(int delta_t)
{
	CGameObject *pObj;
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		pObj->TimerCallback_Test(delta_t);
	}
	m_bombs->TimerCallback_Test(delta_t);
}

// Get the m_origin of the object
GPOINT CGAreaAct::GetObjectPosition(CGameObject * obj)
{
	return obj->GetPosition();
}

// Get the m_origin of the map in area
GPOINT CGAreaAct::GetMapPosition(void)
{
	return pMapCurrent->GetPosition();
}

// Get the focused car object in the area
CGameObject * CGAreaAct::GetFocusObject(void)
{
	return pFocusObj;
}

// Get the m_rect of the map in the area
void CGAreaAct::GetMapRect(RECT & rect1)
{
	rect1 = pMapCurrent->m_rect;
}

// Load all game objects stored in the m_queue
void CGAreaAct::LoadAllObjects(void)
{
	int i;

	GPOINT pos[50];
	CGameObject *pObjs[50];

	memset(pObjs, 0, sizeof(pObjs));

	pos[1].x = 20; pos[1].y = 100;
	pos[2].x = 400; pos[2].y = 110;
	pos[3].x = 50; pos[3].y = 290;
	pos[4].x = 200; pos[4].y = 200;
	pos[5].x = 500; pos[5].y = 600;
	pos[6].x = 600; pos[6].y = 700;
	pos[7].x = 700; pos[7].y = 600;
	pos[8].x = 1100; pos[8].y = 1100;
	pos[9].x = 1100; pos[9].y = 1200;
	pos[10].x = 1100; pos[10].y = 1300;
	pos[11].x = 1200; pos[11].y = 1100;
	pos[12].x = 1200; pos[12].y = 1200;
	pos[13].x = 1200; pos[13].y = 1300;
	pos[14].x = 1300; pos[14].y = 1100;
	pos[15].x = 1300; pos[15].y = 1200;
	pos[16].x = 1300; pos[16].y = 1300;
	pos[17].x = 1400; pos[17].y = 1100;
	pos[18].x = 1400; pos[18].y = 1200;
	pos[19].x = 1400; pos[19].y = 1300;
	pos[20].x = 1000; pos[20].y = 220;
	pos[21].x = 700; pos[21].y = 1000;
	pos[22].x = 500; pos[22].y = 1000;
	pos[23].x = 80; pos[23].y = 150;

	pos[24].x = 1300; pos[24].y = 1400;
	pos[25].x = 1300; pos[25].y = 1500;
	pos[26].x = 1400; pos[26].y = 1300;
	pos[27].x = 1400; pos[27].y = 1400;
	pos[28].x = 1400; pos[28].y = 1500;
	pos[29].x = 1400; pos[29].y = 200;
	pos[30].x = 1000; pos[30].y = 400;
	pos[31].x = 700; pos[31].y = 300;
	pos[32].x = 500; pos[32].y = 300;
	pos[33].x = 1400; pos[33].y = 1400;

												
	pObjs[1] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 1);
	pObjs[2] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 2);
	pObjs[3] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[4] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 4);
	pObjs[5] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 5);
	pObjs[6] = new CGFort(pConstructTemplate, pSmokeTemplate, pFlagTemplate, pBulletTemplate, 1);
	pObjs[7] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 1);
	pObjs[8] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[9] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[10] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[11] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[12] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[13] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[14] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[15] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[16] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[17] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[18] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);
	pObjs[19] = new CGBuilding(pBuildTemplate, pSmokeTemplate, pFlagTemplate, 3);

	pObjs[20] = new CGKiA(pCarTemplate[0], pSmokeTemplate, pBulletTemplate);
	pObjs[21] = new CGKiA(pCarTemplate[0], pSmokeTemplate, pBulletTemplate);
	for(i=22; i<26; i++)
		pObjs[i] = new CGCarPeng(pCarTemplate[1], pSmokeTemplate, pBulletTemplate);
	for(i=26; i<28; i++)
		pObjs[i] = new CGCarPeng(pCarTemplate[3], pSmokeTemplate, pBulletTemplate);
	for(i=28; i<31; i++)
		pObjs[i] = new CGCarPeng(pCarTemplate[2], pSmokeTemplate, pBulletTemplate);
	pObjs[31] = new CGWarriorBowman(pBowmanTemplate, pBulletTemplate);
	pObjs[32] = new CGBuildingHouse(pConstructTemplate, pSmokeTemplate, pFlagTemplate, 2);
#ifndef WIN_MOBILE
	pObjs[33] = new CGWarriorSword(pCavalryTemplate);
#endif

	for(i=1;i<34;i++)
	{
		if(dynamic_cast<CGameObject*>(pObjs[i]))
		{
			pObjs[i]->Load(this, pos[i]);

			// insert according to sequence of foot position
			pObjs[i]->AddtoQueuebyFoot(m_queue);

			if(g_type_map!=pObjs[i]->GetTypeId())
				pMapCurrent->Localize(pObjs[i]);
		}
	}
}

// Unload all game objects from the area
void CGAreaAct::UnloadAllObjects(void)
{
	CGameObject *pObj;
	while(true == m_queue.pop(pObj)){
		pObj->Unload();
		delete pObj;
	}

}

// Set an object as focus in tha area
void CGAreaAct::SetFocusObject(CGameObject * pObj)
{
	pFocusObj = pObj;
}

// Set the next object in the area as focus
bool CGAreaAct::SetFocusObjectNext(void)
{
	CGameObject *pObj = NULL;
	if(m_queue.neighbor(pFocusObj, pObj)){
		SetFocusObject(pObj);
		return true;
	}
	return false;
}

// Set focus on the object at the position, or on the map
void CGAreaAct::SetFocusObjectAt(GPOINT & pos)
{
	CGameObject *pObj;
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
			if(pObj->m_type_id == g_type_map)
				continue;
			RECT rect1 = pObj->GetLocation();
			if(pos.x>rect1.left && pos.y>rect1.top
				&& pos.x<rect1.right
				&& pos.y<rect1.bottom)
			{
				SetFocusObject(pObj);
				return;
			}
	}
	// no object's hit, focus on map
	SetFocusObject(pMapCurrent);
}

// enable/disable tracking with focus object
void CGAreaAct::SetTrackFocus(bool track)
{
	m_IsTrackFocus = track;
}

// decide if tracking with the focus object
bool CGAreaAct::IfTrackFocus(void)
{
	return m_IsTrackFocus;
}

// Notify the area the new cursor position when mouse is moving
void CGAreaAct::NotifyCursorPositionChange(GPOINT & pos)
{
	m_cursor_pos = pos;
}

// get the cursor position
void CGAreaAct::GetCursorPostion(GPOINT & pos)
{
	pos = m_cursor_pos;
}

// active area command processor
bool CGAreaAct::CommandProcessor(int cmd, int wParam, int lParam)
{
	GPOINT pos;
	short dir;

	switch(cmd)
	{
	case G_KEY_N:
		SetTrackFocus(true);
		SetFocusObjectNext();
		break;
	case G_KEY_O:
		SetTrackFocus(true);
		break;
	case G_MOUSE_LEFT:
		pos.x = wParam + GetMapPosition().x;
		pos.y = lParam + GetMapPosition().y;

		if(m_operate.typeId==g_menu_op_type_1)
		{
			CGameObject *pObj = WhoIsHere(pos, NULL);

			SetTrackFocus(false);

			if(NULL == pObj)
			{
				CGameObject *pObjs;
				m_operate.typeId = g_menu_op_type_none;
				switch(m_operate.itemId)
				{
				case g_item_fort:
					pObjs = new CGFort(pConstructTemplate, pSmokeTemplate, pFlagTemplate, pBulletTemplate, 1);
					break;
				case g_item_house:
					pObjs = new CGBuildingHouse(pConstructTemplate, pSmokeTemplate, pFlagTemplate, 2);
					break;
				case g_item_factory:
					pObjs = new CGBuildingFactory(pConstructTemplate, pSmokeTemplate, pFlagTemplate, 3);
					break;
				default:
					;
				}
				if(pObjs)
				{
					pos.x -= pObjs->GetWidth()/2;
					pos.y += pObjs->GetHeight()/8;
					pObjs->Load(this, pos);

					// insert according to sequence of foot position
					pObjs->AddtoQueuebyFoot(m_queue);

					pMapCurrent->Localize(pObjs);
				}
			}
		}else
		{
			SetTrackFocus(true);
			SetFocusObjectAt(pos);
		}
		break;
	case G_MOUSE_RIGHT:
		if(pFocusObj)
		{
			pos.x = wParam + GetMapPosition().x;
			pos.y = lParam + GetMapPosition().y;
			pFocusObj->CommandProcessor(cmd, pos.x, pos.y);
		}
		if(m_operate.typeId==g_menu_op_type_1)
			m_operate.typeId = g_menu_op_type_none;

		break;
	case G_MOUSE_MOVE:
		pos.x = wParam; 
		pos.y = lParam;
		if(pos.x>0 && pos.x<GetWidth()
			&& pos.y>0 && pos.y<GetHeight())
			SetTrackFocus(false);
		NotifyCursorPositionChange(pos);
		break;
	case G_MOUSE_WHEEL:
		dir = (short)wParam;
		SetTrackFocus(false);
		ShiftGampMap(0, dir>0? -20:20);
		break;
	default:
		;
	}
	return false;
}

// When cursor reach the edge, the map should move along with it
void CGAreaAct::ShiftGameMapWithCursor(void)
{
	RECT rect1;
	int lx = 0, ly = 0;
	if(pMapCurrent && m_cursor_pos.x>=0 && m_cursor_pos.y>=0)
	{
		GetRect(rect1);
		rect1.right += TINY_AREA_WIDTH;

		lx = m_cursor_pos.x<10? -5 : 0;
		lx = m_cursor_pos.x > (rect1.right-rect1.left-10)? 5 : lx;
		if(m_cursor_pos.x < (rect1.right-rect1.left-TINY_AREA_WIDTH))
		{
			ly = m_cursor_pos.y<10? -5:0;
			ly = m_cursor_pos.y > (rect1.bottom-rect1.top-10)? 5 : ly;
		}
		pMapCurrent->Shift(lx, ly);
	}
}

// bombing the object in the area, return the bomb id
void CGAreaAct::StartBombing(CGameObject * pObj, G_BOMB_TYPE  type)
{
	m_bombs->Load(this, pObj, type);
}

CGMap * CGAreaAct::GetCurrentMap(void)
{
	return pMapCurrent;
}

// Which object locates at the position
CGameObject * CGAreaAct::WhoIsHere(GPOINT & pos, CGameObject *notObject)
{
	CGameObject *pObj;
	m_queue.entry2();
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
			if(pObj->m_type_id == g_type_map)
				continue;
			if(pObj == notObject)
				continue;

			RECT rect1 = pObj->GetLocation();
			if(pos.x>rect1.left && pos.y>rect1.top
				&& pos.x<rect1.right
				&& pos.y<rect1.bottom)
			{
				m_queue.leave2();
				return pObj;
			}
	}
	// no object's hit, focus on map
	m_queue.leave2();
	return NULL;
}

// 100MS timer callback with delta diferrence
void CGAreaAct::TimerCallback_100MS_delta1(int delta_t)
{
	CGameObject *pObj;
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		if((pObj->GetTypeId() != g_type_map)
			&& (pObj->GetTypeId() != g_type_building))
		{
			pObj->UpdateQueuebyFoot(m_queue);
		}
	}
}

// limit rect within the area boundary
void CGAreaAct::LimitWithin(RECT & rect)
{
	RECT rect2;
	GetRect(rect2);
	if(rect.left<rect2.left)		rect.left = rect2.left;
	if(rect.top<rect2.top)			rect.top = rect2.top;
	if(rect.right>rect2.right)		rect.right = rect2.right;
	if(rect.bottom>rect2.bottom)	rect.bottom = rect2.bottom;
}


// if the rect intersect with other objects, return true
bool CGAreaAct::InterSectWithOthers(RECT & rect, CGameObject *pObjEx/*=NULL*/)
{
	RECT rect1;
	CGameObject *pObj;
	m_queue.entry2();
	for(bool b = m_queue.head(pObj); 
		b==true; 
		b = m_queue.next(pObj))
	{
		if(pObj->GetTypeId() != g_type_map
			&& pObj != pObjEx)
		{
			rect1 = pObj->GetLocation();
			if(InterSection(rect, rect1))
				return true;
		}
	}
	m_queue.leave2();
	return false;
}

// if the rect over-range the current map's boundary, return true
bool CGAreaAct::OverrangeMapBoundary(RECT & rect)
{
	return Overrange(rect, pMapCurrent->GetSize());
}
