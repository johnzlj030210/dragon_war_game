#include "StdAfx.h"
#include ".\gbomb.h"
#include "..\wrapper\\wrapper.h"

#define MAX_BOMBS 50

CGBomb::CGBomb(void)
{
}

CGBomb::~CGBomb(void)
{
	delete m_seq;
	delete m_type;
	delete m_pos;
}
CGBomb::CGBomb(CGBitmapTempletBomb *bomb)
{
	m_type_id = g_type_bomb;
	pTemplate = bomb;
	pHostArea = NULL;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	index = 0;
	m_seq = new int[MAX_BOMBS];
	m_type = new G_BOMB_TYPE[MAX_BOMBS];
	m_pos = new GPOINT[MAX_BOMBS];
	memset(m_seq, 0, MAX_BOMBS*sizeof(int));
	memset(m_type, 0, MAX_BOMBS*sizeof(G_BOMB_TYPE));
	memset(m_pos, 0, MAX_BOMBS*sizeof(GPOINT));

	for(int i=0; i<MAX_BOMBS; i++)
		m_type[i] = g_bomb_none;
}

void CGBomb::Load(CGAreaAct *pArea, CGameObject *pObj, G_BOMB_TYPE type)
{
	pHostArea = pArea;

	m_pos[index] = pObj->GetPositionBomb();
	m_seq[index] = 0;
	m_type[index] = type;

	index = (index+1)%MAX_BOMBS;
}

void CGBomb::Load(CGAreaAct *pArea, GPOINT &pos)
{
	pHostArea = pArea;

	m_pos[index] = pos;
	m_seq[index] = 0;
	m_type[index] = g_bomb_1;

	index = (index+1)%MAX_BOMBS;
}

// timer callback function of the smoking object
void CGBomb::TimerCallback_10MS(int delta_t)
{
	if(pHostArea)
		UpdateView(false);
}

void CGBomb::TimerCallback_100MS(int delta_t)
{
	for(int i=0; i<MAX_BOMBS; i++)
	{
		if(m_type[i] == g_bomb_none){
		}else if(m_type[i] == g_bomb_1){
			m_seq[i]++;
			if(m_seq[i]>4)
				m_type[i] = g_bomb_none;
		}else if(m_type[i] == g_bomb_2){
			m_seq[i]++;
			if(m_seq[i]>1)
				m_type[i] = g_bomb_none;
		}
	}
}

void CGBomb::UpdateView(bool show)
{
	RECT rect1;
	GPOINT pos;
	CGBitmapTempletBomb *tempbomb;

	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);

	for(int i=0; i<MAX_BOMBS; i++)
	{
		if(g_bomb_none != m_type[i]){
			pos = CoordinateTransformIntoArea(m_pos[i]);
			tempbomb = dynamic_cast<CGBitmapTempletBomb*>(pTemplate);
			if(tempbomb)
				tempbomb->BitmapBlockLayout(surface, rect1, pos, m_seq[i]);
		}
	}
	if(show)
		GShowSurface(surface);
}
