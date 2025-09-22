#pragma once
#include "Gameload.h"

class CGLogicMap
{
public:
	CGLogicMap(void);
	CGLogicMap(CGMap * map);
	virtual ~CGLogicMap(void);
	// register the object in logic map
	void Register(CGameObject * pObj);
	// unregister the object in logic map
	void Unregister(CGameObject * pObj);
private:
	int m_width;
	int m_height;
	int m_blockSizeX;
	int m_blockSizeY;
	// record objects in each logic blocks
	CGQueue<CGameObject *> *m_queue;
public:
	// get the logic coordinate of the position
	bool LogicCoordinate(GPOINT & pos, int & logicX, int & logicY);
	// detect if there's any object in the logic block closed to the position to angle
	bool AnythingClosed(GPOINT &block, CGameObject *pObj, int distance, int angle, CGameObject **pNeighber = 0);
	// detect if anything in the logic block collide with the object
	bool AnythingIntersection(int logic_x, int logic_y, CGameObject * pObj);
	// Get the logic coordinate x-wards to this block
	GPOINT OutSideOf(GPOINT & pos, int wards);
	// who locate the position
	CGameObject * WhoIsHere(GPOINT & pos);
	// all things closed to the position will be injured
	bool AnythingInjured(int logic_x, int logic_y, GPOINT &pos, int blood_reduce);
	// teams is or-ed of all teams which is find nearest to the object
	CGameObject * GetNearestInTeam(CGameObject * pObj, int teams, int distance);
};
