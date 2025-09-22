#include "StdAfx.h"
#include ".\gbullet.h"
#include "..\wrapper\\wrapper.h"
#include "common.h"
#include "GLogicMap.h"

#define PARAPOLA_HEIGHT		(m_range/8)
#define PARAPOLA_HEIGHT_LOW	(m_range/16)

CGBullet::CGBullet(void)
: m_fired(false)
, m_seq(0)
, m_speed(0)
, m_execute(0)
, m_range(0)
{
}

CGBullet::~CGBullet(void)
{
}

CGBullet::CGBullet(CGBitmapTempletBullet *bullet)
: m_fired(false)
{
	m_type_id = g_type_bullet;
	pTemplate = bullet;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_speed = 150;

	m_target_pos.x = 0;
	m_target_pos.y = 0;
	m_start_pos.x = 0;
	m_start_pos.y = 0;
	pHostObject = NULL;
	pHostArea = NULL;
}

void CGBullet::Load(CGAreaAct *pArea, CGameObject *pObj, G_BULLET_TYPE type)
{
	pHostArea = pArea;
	pHostObject = pObj;

	m_type = type;
	m_seq = 0;
	m_execute = 10;
	m_range = 0;
}

void CGBullet::Unload(void)
{
	m_fired = false;
	pHostObject = NULL;
	pHostArea = NULL;
}

// timer callback function of the bullet object
void CGBullet::TimerCallback_10MS(int delta_t)
{
	if(pHostArea && (true == m_fired))
	{
		if(false == FindNextStep()){// reach at the target
			Bombing(g_bomb_2);
			DoHarm();
			m_fired = false;
		}
		UpdateView(false);
		m_seq = (m_seq+1)%4;
	}
}

void CGBullet::UpdateView(bool show)
{
	RECT rect1;
	CGBitmapTempletBullet *tempbullet;
//	GPOINT pos;

	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);
	if(g_bomb_none != m_type)
	{
		GPOINT pos = CoordinateTransformIntoArea(m_pos_parabola);
		tempbullet = dynamic_cast<CGBitmapTempletBullet*>(pTemplate);
		if(tempbullet)
			tempbullet->BitmapBlockLayout(surface, rect1, pos, m_type, m_seq, m_angle);
	}
	if(show)
		GShowSurface(surface);
}

// the bullet fire the target at the position
void CGBullet::Fire(GPOINT & pos)
{
	int len_x, len_y;
	// fire again must be after the last one
	if(pHostObject && !m_fired)
	{
		m_fired = true;
		m_seq = 0;
		m_target_pos = pos;
		m_start_pos = pHostObject->GetPositionShoot();
		m_pos = m_start_pos;
		m_pos_parabola = m_start_pos;
		len_x = m_target_pos.x - m_start_pos.x;
		len_y = m_target_pos.y - m_start_pos.y;
		m_range = root2(len_x*len_x + len_y*len_y);
		if(0==m_range) // too closed
			m_fired = false;
	}
}

// The bullet's track line
bool CGBullet::FindNextStep(void)
{
	bool flag;
	int step_x;
	int step_y;
	int step_z;
	int temp;
	int step = (int)(m_speed/5.0);
	int len_x = m_target_pos.x - m_pos.x;
	int len_y = m_target_pos.y - m_pos.y;
	int len   = root2(len_x*len_x + len_y*len_y);
	GPOINT pos1;

	m_angle = VectorDirection(m_pos, m_target_pos);

	if(len >= step)
	{
		step_x = (int)(step * len_x / len);
		step_y = (int)(step * len_y / len);
		pos1.x = m_pos_parabola.x - 32;
		pos1.y = m_pos_parabola.y - 32;
		SetPosition(pos1);
		flag = true;
	}else{
		step_x = 0;
		step_y = 0;
		pos1.x = (m_pos_parabola.x*4 + m_target_pos.x*6)/10 - 32;
		pos1.y = (m_pos_parabola.y*4 + m_target_pos.y*6)/10 - 32;
		SetPosition(pos1);
		flag = false;
	}
	m_pos.x += step_x;
	m_pos.y += step_y;

	temp = len-m_range/2;
	step_z = PARAPOLA_HEIGHT-4*PARAPOLA_HEIGHT*temp*temp/(m_range*m_range);

	m_pos_parabola.x = m_pos.x;
	m_pos_parabola.y = m_pos.y - step_z;

	return flag;
}

bool CGBullet::DoHarm(void)
{
	CGameObject *pObj =	pHostArea->WhoIsHere(m_target_pos, NULL);
	if(NULL == pObj)
		return false;
//	GTrace("buttle: %d\n", m_execute);
	pObj->FlowBlood(m_execute);
	return true;
}

CGArrow::~CGArrow(void)
{
}

CGArrow::CGArrow(CGBitmapTempletBullet *bullet)
{
	m_type_id = g_type_bullet;
	pTemplate = bullet;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_speed = 150;

	m_target_pos.x = 0;
	m_target_pos.y = 0;
	m_start_pos.x = 0;
	m_start_pos.y = 0;
	pHostObject = NULL;
	pHostArea = NULL;
	m_fired = false;
}

// The arrow's track line
bool CGArrow::FindNextStep(void)
{
	bool flag;
	int step_x;
	int step_y;
	int step_z;
	int temp;
	int step = (int)(m_speed/5.0);
	int len_x = m_target_pos.x - m_pos.x;
	int len_y = m_target_pos.y - m_pos.y;
	int len   = root2(len_x*len_x + len_y*len_y);
	GPOINT pos1;

	m_angle = VectorDirection(m_pos, m_target_pos);

	if(len >= step)
	{
		step_x = (int)(step * len_x / len);
		step_y = (int)(step * len_y / len);
		pos1.x = m_pos_parabola.x - 32;
		pos1.y = m_pos_parabola.y - 32;
		SetPosition(pos1);
		flag = true;
	}else{
		step_x = 0;
		step_y = 0;
		pos1.x = (m_pos_parabola.x*4 + m_target_pos.x*6)/10 - 32;
		pos1.y = (m_pos_parabola.y*4 + m_target_pos.y*6)/10 - 32;
		SetPosition(pos1);
		flag = false;
	}
	m_pos.x += step_x;
	m_pos.y += step_y;

	temp = len-m_range/2;
	step_z = PARAPOLA_HEIGHT_LOW-4*PARAPOLA_HEIGHT_LOW*temp*temp/(m_range*m_range);

	m_pos_parabola.x = m_pos.x;
	m_pos_parabola.y = m_pos.y - step_z;

	return flag;
}
