#include "StdAfx.h"
#include ".\gflag.h"
#include "GBitmapTemplet.h"
#include "..\wrapper\\wrapper.h"
#include "common.h"
#include ".\igflag.h"

CGFlag::CGFlag(CGBitmapTempletFlag *flag)
{
	m_type_id = g_type_flag;
	pTemplate = flag;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	pHostArea = NULL;
}

CGFlag::~CGFlag(void)
{
}

void CGFlag::Load(CGAreaAct *pArea, CGameObject *pObj)
{
	pHostArea = pArea;
	pHostObject = pObj;

	m_type = (G_FLAG_TYPE)pHostObject->GetTeamId();;
	m_seq = RandomValue()%7;
}

void CGFlag::Unload(void)
{
	pHostObject = NULL;
	pHostArea = NULL;
}

// timer callback function of the flag object
void CGFlag::TimerCallback_10MS(int delta_t)
{
	if(pHostArea && pHostObject)
	{
		UpdateView(false);
	}
}
void CGFlag::TimerCallback_100MS(int delta_t)
{
	if(pHostArea && pHostObject)
	{
		m_seq = (m_seq+1)%7;
	}
}

void CGFlag::UpdateView(bool show)
{
	RECT rect1;
	CGBitmapTempletFlag *tempflag;
//	GPOINT pos;

	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);
	if(g_type_none != m_type)
	{
		GPOINT pos1 = pHostObject->GetPositionFlag();
		GPOINT pos = CoordinateTransformIntoArea(pos1);
		tempflag = dynamic_cast<CGBitmapTempletFlag*>(pTemplate);
		if(tempflag)
			tempflag->BitmapBlockLayout(surface, rect1, pos, m_type-1, m_seq);
	}
	if(show)
		GShowSurface(surface);
}

