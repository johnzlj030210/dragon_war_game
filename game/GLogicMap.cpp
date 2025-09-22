#include "StdAfx.h"
#include ".\glogicmap.h"
#include "common.h"
#include "..\wrapper\\wrapper.h"

#define SAFE_GUARD 0
#define BOMB_GUARD 20

CGLogicMap::CGLogicMap(void)
: m_width(0)
, m_height(0)
, m_blockSizeX(0)
, m_blockSizeY(0)
{
}
CGLogicMap::CGLogicMap(CGMap * map)
: m_width(0)
, m_height(0)
, m_blockSizeX(0)
, m_blockSizeY(0)
{
	m_blockSizeX = 256;
	m_blockSizeY = 256;
	m_width = map->m_blockSizeX * map->m_blockNumX / m_blockSizeX + 1;
	m_height = map->m_blockSizeY * map->m_blockNumY / m_blockSizeY + 1;

	m_queue = new CGQueue<CGameObject*>[m_width*m_height];
}

CGLogicMap::~CGLogicMap(void)
{
	delete []m_queue;
}

// register the object in logic map
void CGLogicMap::Register(CGameObject * pObj)
{
	int x, y;
	LogicCoordinate(pObj->GetPosition(), x, y);
	pObj->m_logic_pos.x = x;
	pObj->m_logic_pos.y = y;
	m_queue[y*m_width+x].push(pObj);
}

// unregister the object in logic map
void CGLogicMap::Unregister(CGameObject * pObj)
{
	int x, y;
	x = pObj->m_logic_pos.x;
	y = pObj->m_logic_pos.y;
	m_queue[y*m_width+x].erase(pObj);
}

// get the logic coordinate of the position
bool CGLogicMap::LogicCoordinate(GPOINT & pos, int & logicX, int & logicY)
{
	logicX = pos.x/m_blockSizeX;
	logicY = pos.y/m_blockSizeY;
	if(logicX>=0 && logicX<m_width && logicY>=0 && logicY<m_height)
		return true;
	return false;
}

// detect if there's any object in the logic block closed to the position to angle
bool CGLogicMap::AnythingClosed(GPOINT &block, CGameObject *pObj, int distance, 
								int angle, CGameObject **pNeighber/* = 0*/)
{
	int logic_x = block.x;
	int logic_y = block.y;
	
	if(pNeighber)
		*pNeighber = NULL;

	if(logic_x < 0 || logic_x >= m_width
		|| logic_y < 0 || logic_y >= m_height)
		return false;

	CGameObject *pObj1 = NULL;
	CGQueue<CGameObject*> *q = &m_queue[logic_y*m_width+logic_x];
	RECT rect = pObj->GetLocation();
	rect.left -= SAFE_GUARD; rect.right += SAFE_GUARD; rect.top -= SAFE_GUARD; rect.bottom += SAFE_GUARD;

	for(bool b = q->head(pObj1);
		b == true;
		b = q->next(pObj1))
	{
		RECT rect1 = pObj1->GetLocation();
		rect1.left -= SAFE_GUARD; rect1.right += SAFE_GUARD; rect1.top -= SAFE_GUARD; rect1.bottom += SAFE_GUARD;
		if(pObj == pObj1)
			continue;
		if(pNeighber)
			*pNeighber = pObj1;
		switch (angle)
		{
		case 0:
			if(rect1.bottom <= rect.bottom
				&& rect1.left <= rect.right
				&& rect1.right >= rect.left
				&& (rect.top-rect1.bottom) < distance)
				return true;
			break;
		case 1:
			if(rect1.top <= rect.bottom
				&& rect1.right >= rect.left
				&& Distance2(rect, rect1) < distance*distance)
				return true;
			break;
		case 2:
			if(rect1.left >= rect.left
				&& rect1.top <= rect.bottom
				&& rect1.bottom >= rect.top
				&& (rect1.left-rect.right) < distance)
				return true;
			break;
		case 3:
			if(rect1.bottom > rect.top
				&& rect1.right > rect.left
				&& Distance2(rect, rect1) < distance*distance)
				return true;
			break;
		case 4:
			if(rect1.top >= rect.top
				&& rect1.left <= rect.right
				&& rect1.right >= rect.left
				&& (rect1.top-rect.bottom) < distance)
				return true;
			break;
		case 5:
			if(rect1.bottom >= rect.top
				&& rect1.left <= rect.right
				&& Distance2(rect, rect1) < distance*distance)
				return true;
			break;
		case 6:
			if(rect1.right <= rect.right
				&& rect1.top <= rect.bottom
				&& rect1.bottom >= rect.top
				&& (rect1.right-rect.left) < distance)
				return true;
			break;
		case 7:
			if(rect1.top <= rect.bottom
				&& rect1.left <= rect.right
				&& Distance2(rect, rect1) < distance*distance)
				return true;
			break;
		default://other else, whichever direction
			if(Distance2(rect, rect1) < distance*distance)
				return true;
		}
	}
	if(pNeighber)
		*pNeighber = NULL;
	return false;
}

// detect if anything in the logic block collide with the object
bool CGLogicMap::AnythingIntersection(int logic_x, int logic_y, CGameObject * pObj)
{
	bool retValue = false;

	if(logic_x < 0 || logic_x >= m_width
		|| logic_y < 0 || logic_y >= m_height)
		return false;

	CGameObject *pObj1 = NULL;
	CGQueue<CGameObject*> *q = &m_queue[logic_y*m_width+logic_x];
	RECT rect = pObj->GetLocation();

	q->entry2();

	for(bool b = q->head(pObj1);
		b == true;
		b = q->next(pObj1))
	{
		RECT rect1 = pObj1->GetLocation();
		if(pObj == pObj1)
			continue;
		if(InterSection(rect, rect1)){
			retValue = true;
			break;
		}
	}
	q->leave2();
	return retValue;
}

// Get the logic coordinate x-wards to this block
GPOINT CGLogicMap::OutSideOf(GPOINT & pos, int wards)
{
	GPOINT pos1 = pos;
	switch(wards)
	{
	case 0: pos1.y -= 1; 
		break;
	case 1: pos1.x += 1; pos1.y -= 1;
		break;
	case 2: pos1.x += 1;
		break;
	case 3: pos1.x += 1; pos1.y += 1;
		break;
	case 4: pos1.y += 1;
		break;
	case 5: pos1.x -= 1; pos1.y += 1;
		break;
	case 6: pos1.x -= 1;
		break;
	case 7: pos1.x -= 1; pos1.y -= 1;
		break;
	default:;
	}
	return pos1;
}

// who locate the position
CGameObject * CGLogicMap::WhoIsHere(GPOINT & pos)
{
	int logic_x, logic_y;
	CGameObject *pObj = NULL;

	LogicCoordinate(pos, logic_x, logic_y);

	for(int i=-1; i<=1; i++)
		for(int j=-1; j<=1; j++)
		{
			if(logic_y+i<0 || logic_y+i>m_height
				|| logic_x+j<0 || logic_x+j>m_width)
				continue;
			CGQueue<CGameObject*> *q = &m_queue[(logic_y+i)*m_width+logic_x+j];
			RECT rect;

			q->entry2();
			for(bool b = q->head(pObj);
				b == true;
				b = q->next(pObj))
			{
				rect = pObj->GetLocation();
				if(pos.x>=rect.left && pos.x<=rect.right
					&& pos.y>=rect.top && pos.y<=rect.bottom)
					break;
			}
			q->leave2();
		}

	return pObj;
}

// all things closed to the position will be injured
bool CGLogicMap::AnythingInjured(int logic_x, int logic_y, GPOINT &pos, int blood_reduce)
{
	bool retValue = false;
	CGameObject *pObj = NULL;

	if(logic_x < 0 || logic_x >= m_width
		|| logic_y < 0 || logic_y >= m_height)
		return false;

	CGQueue<CGameObject*> *q = &m_queue[logic_y*m_width+logic_x];

	q->entry2();

	for(bool b = q->head(pObj);
		b == true;
		b = q->next(pObj))
	{
		RECT rect = pObj->GetLocation();
		GTrace("rect: %d,", rect.left);
		GTrace(" %d,", rect.top);
		GTrace(" %d,", rect.right);
		GTrace(" %d\n", rect.bottom);
		if(pos.x>=(rect.left-BOMB_GUARD) && pos.x<=(rect.right+BOMB_GUARD)
			&& pos.y>=(rect.top-BOMB_GUARD) && pos.y<=(rect.bottom+BOMB_GUARD))
		{
			pObj->FlowBlood(blood_reduce);
			retValue = true;
		}
	}
	q->leave2();
	return retValue;
}

// teams is or-ed of all teams which is find nearest to the object
CGameObject * CGLogicMap::GetNearestInTeam(CGameObject * pObj, int teams, int distance)
{
	int i, j;
	int x = pObj->m_logic_pos.x;
	int y = pObj->m_logic_pos.y;
	CGQueue<CGameObject*> *q;
	CGameObject *pObj1, *pObj2 = NULL;
	RECT rect = pObj->GetLocation();
	int dist = distance*distance;

	for(i=-1; i<=1; i++)
		for(j=-1; j<=1; j++)
	{
		if((y+i)<0 || (x+j)<0 || (y+i)>=m_height || (x+j)>=m_width)
			continue;
		q = &m_queue[(y+i)*m_width+(x+j)];
		for(bool b = q->head(pObj1);
			b == true && (pObj1->m_team & teams) && (pObj!=pObj1);
			b = q->next(pObj1))
		{
			RECT rect1 = pObj1->GetLocation();
			int dist1 = Distance2(rect, rect1);
			if(dist1<dist){
				dist = dist1;
				pObj2 = pObj1;
			}
		}
	}

	return pObj2;
}
