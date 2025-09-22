#include "StdAfx.h"
#include ".\igflag.h"
#include "typedefines.h"
#include "..\wrapper\\wrapper.h"
#include "GameArea.h"
#include ".\gameload.h"
#include "GFlag.h"

IGFlagWave::IGFlagWave(void)
{
	m_template_flag = NULL;
	m_host = NULL;
	m_flag = NULL;
}

IGFlagWave::~IGFlagWave(void)
{
}

void IGFlagWave::Load(CGAreaAct * pArea, CGameObject *pObj)
{
	m_host = pObj;
	m_flag = new CGFlag(m_template_flag);
	GAssert(m_flag);
	CGFlag *flag = dynamic_cast<CGFlag*>(m_flag);
	if(flag)
		flag->Load(pArea, m_host);
}

void IGFlagWave::Unload(void)
{
	if(m_flag)
	{
		m_flag->Unload();
		delete m_flag;
		m_flag = NULL;
	}
}

void IGFlagWave::TimerCallback_10MS(int delta_t)
{
	if(m_flag)
		m_flag->TimerCallback_10MS(delta_t);
}

void IGFlagWave::TimerCallback_100MS(int delta_t)
{
	if(m_flag)
		m_flag->TimerCallback_100MS(delta_t);
}
