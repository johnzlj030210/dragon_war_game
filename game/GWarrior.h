#pragma once
#include "gcar.h"
#include "IGArmed.h"

class CGWarrior :
	public CGCar
{
public:
	CGWarrior(void);
	virtual ~CGWarrior(void);

	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	void TimerCallback_100MS(int delta_t);
	bool CommandProcessor(int cmd, int wParam, int lParam);
	// Process event when come with the boundary of the map, or other game objects
	void ComewithBoundary(G_SHIFT_RESULT status);
	// Change and show the car
	void UpdateView(bool show);
	GPOINT GetPositionBomb(void);
protected:
	// step posture when standing or walking or in battle
	int m_step;
	// standing, walking, sticking, shooting, or firing
	G_WARRIOR_POSTURE m_posture;
	// whether died
	bool m_bDied;
};

class CGWarriorSword :
	public CGWarrior,
	public IGArmedSword
{
	friend class IGArmedSword;
public:
	CGWarriorSword(CGBitmapTempletWarrior *man);
	CGWarriorSword(CGBitmapTempletCavalry *man);
	~CGWarriorSword(void);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	bool CommandProcessor(int cmd, int wParam, int lParam);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
};

class CGWarriorBowman :
	public CGWarrior,
	public IGArmedArrow
{
	friend class IGArmedArrow;
public:
	CGWarriorBowman(CGBitmapTempletBowman *man, CGBitmapTempletBullet *bullet);
	CGWarriorBowman(CGBitmapTempletChariot *man, CGBitmapTempletBullet *bullet);
	~CGWarriorBowman(void);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	bool CommandProcessor(int cmd, int wParam, int lParam);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	// get the position where the arrow is shot
	GPOINT GetPositionShoot(void);
};
