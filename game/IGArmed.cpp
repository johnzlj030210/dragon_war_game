#include "StdAfx.h"
#include ".\igarmed.h"
#include "typedefines.h"
#include "..\wrapper\\wrapper.h"
#include "GameArea.h"
#include ".\gameload.h"
#include ".\GBuilding.h"
#include "common.h"
#include "GLogicMap.h"
#include "GBullet.h"
#include "GWarrior.h"

// return true if the m_murdered is in the shoot range
bool IGArmed::InShootRange(void)
{
	if(m_murdered)
		if(m_killer->DistanceToAnother(m_murdered) < m_shoot_range*m_shoot_range)
		{
		return true;
		}
	return false;
}

// set an enemy as to be killed
bool IGArmed::SetObjAttack(CGameObject * pEnemy)
{
	m_murdered = pEnemy;
	return true;
}

IGArmedCannon::IGArmedCannon(void)
{
	m_murdered = NULL;
	m_bullet = NULL;
	m_template_bullet = NULL;
	m_shoot_range = 128;
	m_killer = NULL;
}

IGArmedCannon::~IGArmedCannon(void)
{
}

// shoot at an enemy
bool IGArmedCannon::Attack(CGameObject * pEnemy)
{
	if(NULL == pEnemy)
		return false;
	if(pEnemy->Unloaded())
		return false;
	if(m_killer->DistanceToAnother(pEnemy) > m_shoot_range*m_shoot_range)
		return false;
	CGBullet *bullet = dynamic_cast<CGBullet*>(m_bullet);
	if(bullet)
		bullet->Fire(pEnemy->GetPositionCenter());
	return true;
}

// timer division
void IGArmedCannon::TimerCallback_10MS(int delta_t)
{
	if(m_bullet)
		m_bullet->TimerCallback_10MS(delta_t);
}

void IGArmedCannon::TimerCallback_100MS(int delta_t)
{
	if(m_bullet)
		m_bullet->TimerCallback_100MS(delta_t);
}

void IGArmedCannon::TimerCallback_500MS(int delta_t)
{
	if(!Attack(m_murdered))
	{
		if(m_murdered)
			if(m_murdered->Unloaded())
				m_murdered = NULL;
	}
	if(m_bullet)
		m_bullet->TimerCallback_500MS(delta_t);
}

void IGArmedCannon::TimerCallback_Test(int delta_t)
{
}

void IGArmedCannon::Load(CGAreaAct * pArea, CGameObject *pObj, G_BULLET_TYPE type)
{
	m_killer = pObj;
	m_bullet = new CGBullet(m_template_bullet);
	GAssert(m_bullet);
	CGBullet *bullet = dynamic_cast<CGBullet*>(m_bullet);
	if(bullet)
		bullet->Load(pArea, m_killer, type);
}

void IGArmedCannon::Unload(void)
{
	if(m_bullet)
	{
		m_bullet->Unload();
		delete m_bullet;
		m_bullet = NULL;
	}
}

IGArmedSword::IGArmedSword(void)
: m_execute(0)
{
	m_murdered = NULL;
	m_template_bullet = NULL;
	m_shoot_range = 0;
	m_killer = NULL;
	m_execute = 0;
}

IGArmedSword::~IGArmedSword(void)
{
}


// shoot at an enemy
bool IGArmedSword::Attack(CGameObject * pEnemy)
{
	CGWarriorSword *warrior = dynamic_cast<CGWarriorSword*>(m_killer);

	if(NULL == pEnemy)
		return false;
	if(pEnemy->Unloaded()
		|| m_killer->DistanceToAnother(pEnemy) > m_shoot_range*m_shoot_range)
	{
		if(warrior)
		{
			if(warrior->m_speed!=0)
				warrior->m_posture = g_warrior_walk;
			else
				warrior->m_posture = g_warrior_standby;
		}
		return false;
	}
	if(warrior)
	{
		warrior->m_posture = g_warrior_stick;
		warrior->m_driving_mode	= g_drive_manual;
		warrior->m_angle = VectorDirection(m_killer->GetPositionFoot(),
				m_murdered->GetPositionFoot());
		// stick the sword
		if(warrior->m_step == 8) // at instance when sword touch the enemy
			pEnemy->FlowBlood(m_execute);
	}
	return true;
}

// timer division
void IGArmedSword::TimerCallback_10MS(int delta_t)
{
}

void IGArmedSword::TimerCallback_100MS(int delta_t)
{
	if(!Attack(m_murdered))
	{
		if(m_murdered){
			if(m_murdered->Unloaded())
				m_murdered = NULL;
		}
	}
}

void IGArmedSword::TimerCallback_500MS(int delta_t)
{
}

void IGArmedSword::TimerCallback_Test(int delta_t)
{
}

void IGArmedSword::Load(CGAreaAct * pArea, CGameObject *pObj, G_BULLET_TYPE type)
{
	m_killer = pObj;
}

void IGArmedSword::Unload(void)
{
	m_killer = NULL;
}

IGArmedArrow::IGArmedArrow(void)
{
}

IGArmedArrow::~IGArmedArrow(void)
{
}

void IGArmedArrow::Load(CGAreaAct * pArea, CGameObject *pObj, G_BULLET_TYPE type)
{
	m_killer = pObj;
	m_bullet = new CGArrow(m_template_bullet);
	GAssert(m_bullet);
	CGArrow *bullet = dynamic_cast<CGArrow*>(m_bullet);
	if(bullet)
		bullet->Load(pArea, m_killer, type);
}

void IGArmedArrow::Unload(void)
{
	if(m_bullet)
	{
		m_bullet->Unload();
		delete m_bullet;
		m_bullet = NULL;
	}
}

// shoot at an enemy
bool IGArmedArrow::Attack(CGameObject * pEnemy)
{
	CGWarriorBowman *warrior = dynamic_cast<CGWarriorBowman*>(m_killer);

	if(NULL == pEnemy)
		return false;
	if(pEnemy->Unloaded()
		|| m_killer->DistanceToAnother(pEnemy) > m_shoot_range*m_shoot_range)
	{
		if(warrior)
		{
			if(warrior->m_speed!=0)
				warrior->m_posture = g_warrior_walk;
			else
				warrior->m_posture = g_warrior_standby;
		}
		return false;
	}
	if(warrior)
	{
		warrior->m_posture = g_warrior_shoot;
		warrior->m_driving_mode	= g_drive_manual;
		warrior->m_angle = VectorDirection(m_killer->GetPositionFoot(),
				m_murdered->GetPositionFoot());
		CGArrow *bullet = dynamic_cast<CGArrow*>(m_bullet);
		if(warrior->m_step == 8) // at instance when the bow lunch the arrow
		{
			if(bullet)
				bullet->Fire(pEnemy->GetPositionCenter());
		}
	}
	return true;
}

void IGArmedArrow::TimerCallback_100MS(int delta_t)
{
	if(!Attack(m_murdered))
	{
		if(m_murdered)
			if(m_murdered->Unloaded())
				m_murdered = NULL;
	}
	if(m_bullet)
		m_bullet->TimerCallback_100MS(delta_t);
}

void IGArmedArrow::TimerCallback_500MS(int delta_t)
{
}
