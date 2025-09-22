#pragma once
#include "gameload.h"
#include "GameArea.h"
#include "GBitmaptemplet.h"

class IGGrow
{
public:
	virtual void Load(CGAreaAct *pArea, CGameObject *pObj) = 0;
	virtual void Unload(void) = 0;
	virtual void TimerCallback_10MS(int delta_t) = 0;
protected:
	// the constructed building
	CGameObject *m_host;
	CGAreaAct *m_host_area;
	// constructing persent
	int m_persent;
};

class IGGrowConstruct :
	public IGGrow
{
public:
	IGGrowConstruct(void);
	virtual ~IGGrowConstruct(void);
public:
	void Load(CGAreaAct *pArea, CGameObject *pObj);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
protected:
	// if construction finished
	bool IsConstructFinished(void){	return (m_persent==100);}
};

class IGGrowProduce :
	public IGGrow
{
public:
	IGGrowProduce(void);
	virtual ~IGGrowProduce(void);
public:
	void Load(CGAreaAct *pArea, CGameObject *pObj);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
public:
	// produce a car or person
	void Produce(G_PRODUCE_ITEM productId);
protected:
	// what will be produced
	G_PRODUCE_ITEM m_product_id;
};
