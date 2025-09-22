#pragma once
#include "gameload.h"

class CGBullet :
	public CGameObject
{
public:
	CGBullet(void);
	CGBullet(CGBitmapTempletBullet *bullet);
	virtual ~CGBullet(void);
public:
	void Load(CGAreaAct *pArea, CGameObject *pObj, G_BULLET_TYPE type);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void UpdateView(bool show);
public:
	// the bullet fire the target at the position
	void Fire(GPOINT & pos);
protected:
	G_BULLET_TYPE m_type;
	GPOINT m_target_pos;
	// flag is it's fired
	bool m_fired;
	// instantaneous position
	GPOINT m_pos;
	GPOINT m_pos_parabola;
	int m_seq;
	int m_speed;
	GPOINT m_start_pos;
	// Who fires the bullet
	CGameObject *pHostObject;
	// The bullet's track line
	virtual bool FindNextStep(void);
	// The bullet harm someone when bombing
	bool DoHarm(void);
	int m_execute;
	int m_range;
};

class CGArrow :
	public CGBullet
{
public:
	CGArrow(CGBitmapTempletBullet *bullet);
	virtual ~CGArrow(void);
protected:
	// The arrow's track line
	bool FindNextStep(void);
};
