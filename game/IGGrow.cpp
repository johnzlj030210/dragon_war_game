#include "StdAfx.h"
#include ".\IGGrow.h"
#include "..\wrapper\wrapper.h"
#include "GCarMotoPeng.h"
#include "GBuilding.h"
#include "GWarrior.h"

#define PRODUCE_PERIOD 150

IGGrowConstruct::IGGrowConstruct(void)
{
	m_host_area = NULL;
	m_host = NULL;
	m_persent = 0;
}

IGGrowConstruct::~IGGrowConstruct(void)
{
}

void IGGrowConstruct::Load(CGAreaAct *pArea, CGameObject *pObj)
{
	m_host_area = pArea;
	m_host = pObj;
	m_host->SetBlood(1);
}

void IGGrowConstruct::Unload(void)
{
	m_host_area = NULL;
	m_host = NULL;
}

void IGGrowConstruct::TimerCallback_10MS(int delta_t)
{
}

void IGGrowConstruct::TimerCallback_100MS(int delta_t)
{
	int blood;

	if(100!=m_persent)
	{
		blood = m_host->GetBlood();
		blood++;
		if(blood>100)
			blood = 100;
		m_host->SetBlood(blood);
		m_persent = blood;
	}
}

IGGrowProduce::IGGrowProduce(void)
{
	m_host_area = NULL;
	m_host = NULL;
	m_persent = 0;
	m_product_id = g_item_none;
}

IGGrowProduce::~IGGrowProduce(void)
{
}

void IGGrowProduce::Load(CGAreaAct *pArea, CGameObject *pObj)
{
	m_host_area = pArea;
	m_host = pObj;
}

void IGGrowProduce::Unload(void)
{
	m_host_area = NULL;
	m_host = NULL;
}

void IGGrowProduce::TimerCallback_10MS(int delta_t)
{
	CGameObject *pObj;

	if(m_product_id != g_item_none)
	{
		m_persent = (m_persent+1)%PRODUCE_PERIOD;
		if((m_host->GetConstructPersent()==100) && (m_persent==0))
		{
			switch(m_product_id)
			{
			case g_item_car0:
			case g_item_car1:
			case g_item_car2:
			case g_item_car3:
				pObj = new CGCarPeng(m_host_area->pCarTemplate[m_product_id-1], 
					m_host_area->pSmokeTemplate,
					m_host_area->pBulletTemplate);
				break;
			case g_item_bowman:
				pObj = new CGWarriorBowman(m_host_area->pBowmanTemplate, m_host_area->pBulletTemplate);
				break;
#ifndef WIN_MOBILE
			case g_item_swordman:
				pObj = new CGWarriorSword(m_host_area->pWarriorTemplate);
				break;
			case g_item_cavalry:
				pObj = new CGWarriorSword(m_host_area->pCavalryTemplate);
				break;
			case g_item_chariot:
				pObj = new CGWarriorBowman(m_host_area->pChariotTemplate, m_host_area->pBulletTemplate);
				break;
#endif
			default:
				;
			}
			if(dynamic_cast<CGameObject*>(pObj))
			{
				pObj->Load(m_host_area, m_host->GetPositionCenter());
				pObj->AddtoQueuebyFoot(m_host_area->m_queue);
				m_host_area->pMapCurrent->Localize(pObj);
			}
			m_product_id = g_item_none;
		}
	}
}

void IGGrowProduce::TimerCallback_100MS(int delta_t)
{
}

// produce a car or person
void IGGrowProduce::Produce(G_PRODUCE_ITEM productId)
{
	if((m_host->GetConstructPersent()==100) && (m_persent==0))
	{
		m_product_id = productId;
	}
}
