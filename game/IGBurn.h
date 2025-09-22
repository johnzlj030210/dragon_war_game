#pragma once
#include "gameload.h"
#include "GameArea.h"
#include "GBitmaptemplet.h"

class IGBurn
{
public:
	virtual void Load(CGAreaAct *pArea, CGameObject *pObj) = 0;
	virtual void Unload(void) = 0;
	virtual void TimerCallback_10MS(int delta_t) = 0;
	virtual void TimerCallback_100MS(int delta_t) = 0;
	virtual void TimerCallback_500MS(int delta_t) = 0;
	virtual void TimerCallback_Test(int delta_t) = 0;
protected:
	// what to do with the given blood
	virtual void LosingBlood(int blood) = 0;
	virtual void AddingBlood(int blood) = 0;
protected:
	// the smoke template for the moto car to create CGSmoke object
	CGBitmapTempletSmoke *m_template_smoke;
	CGameObject *m_host;
	CGameObject *m_smoke;
};

class IGBurnSmoke :
	public IGBurn
{
public:
	IGBurnSmoke(void);
	virtual ~IGBurnSmoke(void);
	void Load(CGAreaAct *pArea, CGameObject *pObj);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
protected:
	// what to do with the given blood
	void LosingBlood(int blood);
	void AddingBlood(int blood);
};
