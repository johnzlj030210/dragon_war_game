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

#define LOAD_POS_INTERVAL	20
#define LOAD_POS_MAXTIME	800

CGameObject::CGameObject()
: pHostArea(NULL)
, m_type_id(g_type_none)
, m_angle(0)
, m_blood(100)
, m_team(0)
, m_defense(0)
, m_del_count(0)
, m_ConstructPersent(0)
, pTemplate(NULL)
{
	memset(&m_rect, 0, sizeof(m_rect));
	memset(&m_size, 0, sizeof(m_size));
	m_origin.x=0;
	m_origin.y=0;
	m_pos_foot.x = 0;
	m_pos_foot.y = 0;
	m_logic_pos.x = 0;
	m_logic_pos.y = 0;
}

CGameObject::~CGameObject()
{
}

// load into the area
void CGameObject::Load(CGAreaAct * pArea, GPOINT &pos)
{
	pHostArea = pArea;
	m_pos_foot = GetFreePositionNear(pos);
	m_origin = GetOriginFromFootPos(m_pos_foot);
	UpdateView(true);
	pHostArea->GetMapRect(m_boundary);
	m_boundary.bottom -= m_rect.bottom;
	m_boundary.right -= m_rect.right;
	m_ConstructPersent = 100;
}
G_SHIFT_RESULT CGameObject::Shift(int x, int y)
{
	return G_SHIFT_RESULT();
}

void CGameObject::TimerCallback_100MS(int delta_t)
{
}

void CGameObject::TimerCallback_500MS(int delta_t)
{
}

void CGameObject::TimerCallback_Test(int delta_t)
{
}

void CGameObject::TimerCallback_2S(int delta_t)
{
}

bool CGameObject::CommandProcessor(int cmd, int wParam, int lParam)
{
	return false;
}

// Get the main surface of the host area for picture drawing
GHANDLE CGameObject::GetObjectSurface(int number)
{
	if(number==0)
		return GHANDLE(pHostArea->main_Surface);
	if(number==1)
		return GHANDLE(pHostArea->second_Surface);
	else
		return 0;
}

// Transform the coordinate in map into the active area
GPOINT CGameObject::CoordinateTransformIntoArea(GPOINT &pos)
{
	GPOINT pos0;
	GPOINT pos1 = pHostArea->GetMapPosition();
	pos0.x = pos.x - pos1.x;
	pos0.y = pos.y - pos1.y;
	return pos0;
}

// Get the object's position
GPOINT CGameObject::GetPosition(void)
{
	return m_origin;
}

// get the center position of the object
GPOINT CGameObject::GetPositionCenter(void)
{
	GPOINT pos;
	pos.x = m_origin.x + ((m_size.left + m_size.right)>>1);
	pos.y = m_origin.y + ((m_size.top + m_size.bottom)>>1);
	return pos;
}

void CGameObject::SetPosition(GPOINT & pos)
{
	m_origin = pos;
	m_pos_foot.x = pos.x + (m_size.left - m_rect.left);
	m_pos_foot.y = pos.y + (m_rect.bottom - m_rect.top);
}

void CGameObject::SetPositionFoot(GPOINT & pos)
{
	m_pos_foot = pos;
	m_origin = GetOriginFromFootPos(m_pos_foot);
}

// Get the object width
int CGameObject::GetWidth(void)
{
	return m_rect.right - m_rect.left;
}

// Get the object height
int CGameObject::GetHeight(void)
{
	return m_rect.bottom - m_rect.top;
}

// Get the object's type id: map, building, car, etc
G_TYPE_ID CGameObject::GetTypeId(void)
{
	return m_type_id;
}

// Get the object's location size
RECT CGameObject::GetSize(void)
{
	return this->m_size;
}

// Get the object's angle, 0 if it's a building or map
int CGameObject::GetAngle(void)
{
	return m_angle;
}

// bombing an object
void CGameObject::Bombing(G_BOMB_TYPE type)
{
	if(pHostArea)
	{
		if(type != g_bomb_none)
		{
			pHostArea->StartBombing(this, type);
		}
	}	
}
void CGameObject::Unload(void)
{
	m_blood = 0;
	m_ConstructPersent = 0;
}

// whether the object has already been unloaded
bool CGameObject::Unloaded(void)
{
	return m_blood<=0;
}

bool CGameObject::CanBeDeletedSafely(void)
{
	if(m_blood<=0)
		m_del_count++;
	return m_del_count>100;
}

RECT CGameObject::GetLocation(void)
{
	RECT rect1;
	rect1.left = m_origin.x + m_size.left;
	rect1.top = m_origin.y + m_size.top;
	rect1.right = m_origin.x + m_size.right;
	rect1.bottom = m_origin.y + m_size.bottom;
	return rect1;
}

// the distance to the other object
int CGameObject::DistanceToAnother(CGameObject * pObj)
{
	return Distance2(GetLocation(), pObj->GetLocation());
}

// being shoot by enemy by using arm
void CGameObject::BeingShoot(G_ARM_TYPE arm)
{
	switch(arm)
	{
		case g_arm_none:
			break;
		case g_arm_1:
			m_blood-=10;
			break;
		default:
			;
	}
}

// reduce m_blood
bool CGameObject::FlowBlood(int blood)
{
	int value = blood-m_defense;
	if(value<0)
		value = 0;
	m_blood -= value;
	return m_blood>0;
}

// The gun position of the object
GPOINT CGameObject::GetPositionShoot(void)
{
	GPOINT pos;
	pos.x = m_origin.x + ((m_size.left + m_size.right)>>1);
	pos.y = m_origin.y + ((m_size.top + m_size.bottom)>>1);
	return pos;
}

// set the object's blood
void CGameObject::SetBlood(int blood)
{
	m_blood = blood;
}
// get the blood of the object
int CGameObject::GetBlood(void)
{
	return m_blood;
}

// get the smoke position of the object
GPOINT CGameObject::GetPositionSmoke(void)
{
	return m_origin;
}
// get the bomb position of the object
GPOINT CGameObject::GetPositionBomb(void)
{
	return m_origin;
}
// get the bomb position of the object
GPOINT CGameObject::GetPositionFlag(void)
{
	GPOINT pos;
	pos.x = m_origin.x + ((m_size.left + m_size.right)>>1) - 3;
	pos.y = m_origin.y;
	return pos;
}
// insert the object in the queue according to foot position
void CGameObject::AddtoQueuebyFoot(CGQueue<CGameObject*> & queue)
{
	CGameObject *pObj;
	for(bool b = queue.head(pObj); 
		b==true; 
		b = queue.next(pObj))
	{
			if(pObj->GetTypeId()==g_type_map)
				continue;
			GPOINT pos1  = pObj->GetPositionFoot();
			if(pos1.y>m_pos_foot.y)
				break;
	}
	queue.insert_current(this);
}

// update the position in the queue according to its foot position change
void CGameObject::UpdateQueuebyFoot(CGQueue<CGameObject*> & queue)
{
	CGameObject *pObj;

// erase will delete current object, so entry2/leave2 is not safe
//	queue.entry2();
	queue.erase(this);
	for(bool b = queue.head(pObj); 
		b==true; 
		b = queue.next(pObj))
	{
			if(pObj->GetTypeId()==g_type_map)
				continue;
			GPOINT pos1  = pObj->GetPositionFoot();
			if(pos1.y>m_pos_foot.y)
				break;
	}
	queue.insert_current(this);
//	queue.leave2();
}

// get spare position near the foot_pos
GPOINT CGameObject::GetFreePositionNear(GPOINT & foot_pos)
{
	int dx, dy;
	int angle = 0;
	int dist = 1;
	bool first = true;
	GPOINT orign;
	RECT size1 = GetSize();
	RECT rect2;
	GPOINT foot_pos1 = foot_pos;

	orign = GetOriginFromFootPos(foot_pos);

	rect2.left = orign.x + size1.left;
	rect2.top = orign.y + size1.top;
	rect2.right = orign.x + size1.right;
	rect2.bottom = orign.y + size1.bottom;

	if(pHostArea->InterSectWithOthers(rect2) == false
		&& pHostArea->OverrangeMapBoundary(rect2) == false)
		return foot_pos;

	for(dist = 1; dist<LOAD_POS_MAXTIME; dist++)
	{
		dx = LOAD_POS_INTERVAL*dist; dy = -LOAD_POS_INTERVAL*dist;
		for(int i = -dist; i<dist; i++)
		{
			foot_pos1.x = foot_pos.x + dx;
			foot_pos1.y = foot_pos.y + dy;
			orign = GetOriginFromFootPos(foot_pos1);

			rect2.left = orign.x + size1.left;
			rect2.top = orign.y + size1.top;
			rect2.right = orign.x + size1.right;
			rect2.bottom = orign.y + size1.bottom;

			if(pHostArea->InterSectWithOthers(rect2) == false
				&& pHostArea->OverrangeMapBoundary(rect2) == false)
				return foot_pos1;
			dy+=LOAD_POS_INTERVAL;
		}
		dx = LOAD_POS_INTERVAL*dist; dy = LOAD_POS_INTERVAL*dist;
		for(int i = -dist; i<dist; i++)
		{
			foot_pos1.x = foot_pos.x + dx;
			foot_pos1.y = foot_pos.y + dy;
			orign = GetOriginFromFootPos(foot_pos1);

			rect2.left = orign.x + size1.left;
			rect2.top = orign.y + size1.top;
			rect2.right = orign.x + size1.right;
			rect2.bottom = orign.y + size1.bottom;

			if(pHostArea->InterSectWithOthers(rect2) == false
				&& pHostArea->OverrangeMapBoundary(rect2) == false)
				return foot_pos1;
			dx-=LOAD_POS_INTERVAL;
		}
		dx = -LOAD_POS_INTERVAL*dist; dy = LOAD_POS_INTERVAL*dist;
		for(int i = -dist; i<dist; i++)
		{
			foot_pos1.x = foot_pos.x + dx;
			foot_pos1.y = foot_pos.y + dy;
			orign = GetOriginFromFootPos(foot_pos1);

			rect2.left = orign.x + size1.left;
			rect2.top = orign.y + size1.top;
			rect2.right = orign.x + size1.right;
			rect2.bottom = orign.y + size1.bottom;

			if(pHostArea->InterSectWithOthers(rect2) == false
				&& pHostArea->OverrangeMapBoundary(rect2) == false)
				return foot_pos1;
			dy-=LOAD_POS_INTERVAL;
		}
		dx = -LOAD_POS_INTERVAL*dist; dy = -LOAD_POS_INTERVAL*dist;
		for(int i = -dist; i<dist; i++)
		{
			foot_pos1.x = foot_pos.x + dx;
			foot_pos1.y = foot_pos.y + dy;
			orign = GetOriginFromFootPos(foot_pos1);

			rect2.left = orign.x + size1.left;
			rect2.top = orign.y + size1.top;
			rect2.right = orign.x + size1.right;
			rect2.bottom = orign.y + size1.bottom;

			if(pHostArea->InterSectWithOthers(rect2) == false
				&& pHostArea->OverrangeMapBoundary(rect2) == false)
				return foot_pos1;
			dx+=LOAD_POS_INTERVAL;
		}
	}
	return foot_pos;
}

// transform foot position into origin position
GPOINT CGameObject::GetOriginFromFootPos(GPOINT & foot_pos)
{
	GPOINT orig_pos;
	orig_pos.x = foot_pos.x - (m_size.left - m_rect.left);
	orig_pos.y = foot_pos.y - (m_size.bottom - m_rect.top);
	return orig_pos;
}
