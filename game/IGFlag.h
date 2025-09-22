#pragma once
#include "GameArea.h"

class IGFlag
{
public:
	virtual void Load(CGAreaAct *pArea, CGameObject *pObj) = 0;
	virtual void Unload(void) = 0;
	virtual void TimerCallback_10MS(int delta_t) = 0;
protected:
	// the flag template for the moto car to create CGSmoke object
	CGBitmapTempletFlag *m_template_flag;
	CGameObject *m_host;
	CGameObject *m_flag;
};

class IGFlagWave :
	public IGFlag
{
public:
	IGFlagWave(void);
	virtual ~IGFlagWave(void);
public:
	void Load(CGAreaAct *pArea, CGameObject *pObj);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
};
