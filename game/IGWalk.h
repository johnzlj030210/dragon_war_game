#pragma once
#include "GameLoad.h"

class IGWalk
{
public:
	virtual void Load(CGAreaAct *pArea, CGameObject *pObj) = 0;
	virtual void Unload(void) = 0;
	// return false if the destination is out of map
	virtual bool SetDestinePosition(GPOINT & pos) = 0;
	// Check if the car collide with other objectsin the area
	virtual bool CheckCollideOthers(void) = 0;
	// find path automatically
	virtual void WalkAutoFindPath(double & dx, double & dy) = 0;
};
// A* algorithm
class IGWalkAuto :
	public IGWalk
{
public:
	IGWalkAuto(void);
	virtual ~IGWalkAuto(void);
public:
	void Load(CGAreaAct *pArea, CGameObject *pObj);
	void Unload(void);
	// return false if the destination is out of map
	bool SetDestinePosition(GPOINT & pos);
	// Check if the car collide with other objectsin the area
	bool CheckCollideOthers(void);
	// find path automatically
	void WalkAutoFindPath(double & dx, double & dy);
protected:
	// Find where to go next stop by the path selection algorithm
	virtual G_NEXTSTEP_RESULT FindNextStep(double &dx, double &dy) = 0;
	virtual G_NEXTSTOP_RESULT FindNextStop(void) = 0;
	// return true if the postion has ever been reached
	bool IsFootPrint(GPOINT pos);
protected:
	CGameObject *m_walker;
	CGAreaAct *m_area;
	// next stop in path finding
	GPOINT m_next_stop;
	// where the car will go. be negative if no destination.
	GPOINT m_dest_pos;
	// manually driving, or automatically driving when finding the path automatically
	G_DRIVE_MODE m_driving_mode;
	// moving speed
	int m_speed;
	// the speed of auto driving
	int m_default_speed;
	// list for track line of foot print
	CGQueue<GPOINT> m_track_foot;
};

// A* algorithm
class IGWalkAutoAx :
	public IGWalkAuto
{
protected:
	// Find where to go next stop by the path selection algorithm
	G_NEXTSTEP_RESULT FindNextStep(double &dx, double &dy);
	G_NEXTSTOP_RESULT FindNextStop(void);
	// get the merit of the next point
	int CalculateMerit(GPOINT pos, GPOINT pos_dest, int dir);
};
