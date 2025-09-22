#pragma once
#include "gameload.h"
#include "GameArea.h"
#include "GBitmaptemplet.h"

class IGArmed
{
public:
	virtual bool Attack(CGameObject * pEnemy) = 0;
	virtual void TimerCallback_10MS(int delta_t) = 0;
	virtual void TimerCallback_100MS(int delta_t) = 0;
	virtual void TimerCallback_500MS(int delta_t) = 0;
//	virtual void TimerCallback_Test(int delta_t) = 0;
	virtual void Load(CGAreaAct *pArea, CGameObject *pObj, G_BULLET_TYPE type) = 0;
	virtual void Unload(void) = 0;
protected:
	CGBitmapTempletBullet *m_template_bullet;
	// the enemy object which is set to be shoot at
	CGameObject *m_murdered;
	// range it can fire
	int m_shoot_range;
	CGameObject *m_killer;
protected:
	// return true if the m_murdered is in the shoot range
	bool InShootRange(void);
	// set an enemy as to be killed
	bool SetObjAttack(CGameObject * pEnemy);
};

class IGArmedCannon :
	public IGArmed
{
public:
	IGArmedCannon(void);
	virtual ~IGArmedCannon(void);

	void Load(CGAreaAct *pArea, CGameObject *pObj, G_BULLET_TYPE type);
	void Unload(void);
	// shoot at an enemy
	bool Attack(CGameObject * pEnemy);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
protected:
	CGameObject *m_bullet;
};

class IGArmedSword :
	public IGArmed
{
public:
	IGArmedSword(void);
	virtual ~IGArmedSword(void);
public:
	void Load(CGAreaAct *pArea, CGameObject *pObj, G_BULLET_TYPE type);
	void Unload(void);
	// shoot at an enemy
	bool Attack(CGameObject * pEnemy);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
protected:
	// hurt power
	int m_execute;
};

class IGArmedArrow :
	public IGArmedCannon
{
public:
	IGArmedArrow(void);
	virtual ~IGArmedArrow(void);
	void Load(CGAreaAct *pArea, CGameObject *pObj, G_BULLET_TYPE type);
	void Unload(void);
	// shoot at an enemy
	bool Attack(CGameObject * pEnemy);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
};
