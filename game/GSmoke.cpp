#include "StdAfx.h"
#include ".\gsmoke.h"
#include "..\wrapper\\wrapper.h"

#define MAX_SMOKE_SCALE 20

CGSmoke::CGSmoke(void)
: m_seq(0)
, pHostObject(NULL)
, m_scale(0)
{
}

CGSmoke::~CGSmoke(void)
{
}

CGSmoke::CGSmoke(CGBitmapTempletSmoke *smoke)
: m_seq(0)
, pHostObject(NULL)
, m_scale(0)
{
	m_type_id = g_type_smoke;
	pTemplate = smoke;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();
}
void CGSmoke::Load(CGAreaAct *pArea, CGameObject *pObj, G_SMOKE_TYPE type)
{
	pHostArea = pArea;
	pHostObject = pObj;
	m_type = type;
	switch(type)
	{
	case g_smoke_none:
		break;
	case g_smoke_1:
		m_scale = 5;
		break;
	case g_smoke_2:
		m_scale = 10;
		break;
	default:
		m_scale = 3;
	}
	m_pos_array = new GPOINT[m_scale];
	memset(m_pos_array, 0, sizeof(GPOINT)*m_scale);
}

void CGSmoke::Load(CGAreaAct *pArea, GPOINT &pos)
{
	pHostArea = pArea;
	SetPosition(pos);
	m_scale = 5;
	m_pos_array = new GPOINT[m_scale];
	memset(m_pos_array, 0, sizeof(GPOINT)*m_scale);
	m_type = g_smoke_1;
}

void CGSmoke::Load(CGAreaAct *pArea, CGameObject *pObj)
{
	pHostArea = pArea;
	pHostObject = pObj;

	m_pos_array = new GPOINT[MAX_SMOKE_SCALE];
	memset(m_pos_array, 0, sizeof(GPOINT)*MAX_SMOKE_SCALE);
}

void CGSmoke::Unload(void)
{
	delete []m_pos_array;
	pHostArea = NULL;
	pHostObject = NULL;
	m_type = g_smoke_none;
	m_scale = 0;
}

// timer callback function of the smoking object
void CGSmoke::TimerCallback_10MS(int delta_t)
{
	if(!m_scale)
		return;
	if(NULL == pHostObject)
		return;
	for(int i=m_scale-1; i>0; i--)
		m_pos_array[i] = m_pos_array[i-1];
	m_pos_array[0] = pHostObject->GetPositionSmoke();
	SetPosition(m_pos_array[0]);
	UpdateView(false);
}

void CGSmoke::TimerCallback_100MS(int delta_t)
{
	if(!m_scale)
		return;
	m_seq = (m_seq+1)%5;
}
void CGSmoke::UpdateView(bool show)
{
	RECT rect1;
	GPOINT pos, pos1;
	CGBitmapTempletSmoke *tempsmoke;

	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);

	for(short index=0; index<m_scale; index++)
	{
		pos1.x = m_pos_array[index].x;
		pos1.y = m_pos_array[index].y - (8+index*8);
		pos = CoordinateTransformIntoArea(pos1);
		tempsmoke = dynamic_cast<CGBitmapTempletSmoke*>(pTemplate);
		if(tempsmoke)
			tempsmoke->BitmapBlockLayout(surface, rect1, pos, index, m_seq);
	}

	if(show)
		GShowSurface(surface);
}


void CGSmoke::ShowingUp(G_SMOKE_TYPE type)
{
	m_type = type;
	switch(type)
	{
	case g_smoke_none:
		m_scale = 0;
		break;
	case g_smoke_1:
		m_scale = 5;
		break;
	case g_smoke_2:
		m_scale = 10;
		break;
	default:
		m_scale = 3;
	}
}
