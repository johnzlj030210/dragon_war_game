#include "StdAfx.h"
#include ".\IGBurn.h"
#include "typedefines.h"
#include "..\wrapper\\wrapper.h"
#include "GameArea.h"
#include ".\gameload.h"
#include ".\GBuilding.h"
#include "common.h"
#include "GLogicMap.h"
#include "GSmoke.h"

IGBurnSmoke::IGBurnSmoke(void)
{
	m_template_smoke = NULL;
	m_host = NULL;
	m_smoke = NULL;
}

IGBurnSmoke::~IGBurnSmoke(void)
{
}

void IGBurnSmoke::Load(CGAreaAct * pArea, CGameObject *pObj)
{
	m_host = pObj;
	m_smoke = new CGSmoke(m_template_smoke);
	GAssert(m_smoke);
	CGSmoke *smoke = dynamic_cast<CGSmoke*>(m_smoke);
	if(smoke)
		smoke->Load(pArea, m_host);
}

void IGBurnSmoke::Unload(void)
{
	if(m_smoke)
	{
		m_smoke->Unload();
		delete m_smoke;
		m_smoke = NULL;
	}
}

// timer division
void IGBurnSmoke::TimerCallback_10MS(int delta_t)
{
	if(m_smoke)
		m_smoke->TimerCallback_10MS(delta_t);
}

void IGBurnSmoke::TimerCallback_100MS(int delta_t)
{
	if(m_smoke)
		m_smoke->TimerCallback_100MS(delta_t);
}

void IGBurnSmoke::TimerCallback_500MS(int delta_t)
{
	if(m_smoke)
		m_smoke->TimerCallback_500MS(delta_t);
}

void IGBurnSmoke::TimerCallback_Test(int delta_t)
{
}

// what to do with the given blood
void IGBurnSmoke::LosingBlood(int blood)
{
	CGSmoke *pSmoke;
	if(blood<=30 && blood>0)
	{//smoking
		pSmoke = dynamic_cast<CGSmoke*>(m_smoke);
		if(pSmoke)
			pSmoke->ShowingUp(g_smoke_1);
	}
	if(blood<=0)
	{//ruined
		pSmoke = dynamic_cast<CGSmoke*>(m_smoke);
		if(pSmoke)
			pSmoke->ShowingUp(g_smoke_none);
		m_host->Bombing(g_bomb_1);
	}
}

void IGBurnSmoke::AddingBlood(int blood)
{
	if(blood>30) // cease smoking
	{
		CGSmoke *smoke = dynamic_cast<CGSmoke*>(m_smoke);
		if(smoke)
			smoke->ShowingUp(g_smoke_none);
	}
}

