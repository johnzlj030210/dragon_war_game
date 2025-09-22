#include "StdAfx.h"
#include ".\gwarrior.h"
#include "typedefines.h"
#include "..\wrapper\\wrapper.h"
#include "common.h"

CGWarrior::CGWarrior(void)
: m_step(0)
, m_bDied(false)
{
}

CGWarrior::~CGWarrior(void)
{
}

void CGWarrior::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGCar::Load(pArea, pos);
}

void CGWarrior::Unload(void)
{
	CGCar::Unload();
	if(!m_bDied)
		Bombing(g_bomb_1);
	m_bDied = true;
}

void CGWarrior::ComewithBoundary(G_SHIFT_RESULT status)
{
	m_posture = g_warrior_standby;
}

// Process command from keyboard, mouse, or other devices
bool CGWarrior::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_MOUSE_RIGHT:
		{
		GPOINT pos;
		pos.x = wParam;
		pos.y = lParam;
		SetDestinePosition(pos);
		m_posture = g_warrior_walk;
		}
		break;
	default:
		return false;
	}
	
	pHostArea->SetTrackFocus(true);

	return true;
}

// get the bomb position of the object
GPOINT CGWarrior::GetPositionBomb(void)
{
	GPOINT pos = m_origin;
	pos.x += (m_rect.right/3);
	return pos;
}

// timer division
void CGWarrior::TimerCallback_100MS(int delta_t)
{
	if(m_speed == 0 && m_posture == g_warrior_walk)
		m_posture = g_warrior_standby;

	CGBitmapTempletWarrior *man = dynamic_cast<CGBitmapTempletWarrior *>(pTemplate);

	switch(m_posture)
	{
	case g_warrior_standby:
		m_step = (m_step+1)%man->m_block_stride_standby;
		break;
	case g_warrior_walk:
		m_step = (m_step+1)%man->m_block_stride_walk;
		break;
	case g_warrior_stick:
		m_step = (m_step+1)%man->m_block_stride_stick;
		break;
	case g_warrior_shoot:
		m_step = (m_step+1)%man->m_block_stride_shoot;
		break;
	case g_warrior_fire:
		m_step = (m_step+1)%man->m_block_stride_fire;
		break;
	default:
		m_step = (m_step+1)%5;
		;
	}
}
// Change and show the car
void CGWarrior::UpdateView(bool show)
{
	RECT rect1;
	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);
	CGBitmapTempletWarrior *tempman;

	GPOINT pos = CoordinateTransformIntoArea(GetPosition());
	// display focus bar
	if(this == dynamic_cast<CGWarrior *>(pHostArea->GetFocusObject()))
	{
		RECT rect;
		rect.left = pos.x+rect1.left+m_size.left; 
		rect.top = pos.y+rect1.top + pTemplate->m_tall;
		rect.right = pos.x+rect1.left+m_size.right;
		rect.bottom = pos.y+rect1.top+m_size.bottom;
		SHOW_FOCUS_BAR(rect);
	}

	// note: CGBitmapTempletCavalry/CGBitmapTempletBowman based on CGBitmapTempletWarrior
	if(NULL != (tempman = dynamic_cast<CGBitmapTempletWarrior*>(pTemplate)))
		tempman->BitmapBlockLayout(surface, rect1, pos, m_angle, m_posture, m_step);

	if(show)
		GShowSurface(surface);
}

CGWarriorSword::CGWarriorSword(CGBitmapTempletWarrior *man)
{
	pTemplate = man;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_size = man->GetSize();
	m_default_speed = 5;
	m_type_id = g_type_warrior;
	m_posture = g_warrior_standby;
	m_defense = 8;
	m_shoot_range = 20;
	m_execute = 30;
}

CGWarriorSword::CGWarriorSword(CGBitmapTempletCavalry *man)
{
	pTemplate = man;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_size = man->GetSize();
	m_default_speed = 10;
	m_type_id = g_type_warrior;
	m_posture = g_warrior_standby;
	m_defense = 8;
	m_shoot_range = 20;
	m_execute = 30;
}

CGWarriorSword::~CGWarriorSword(void)
{
}

void CGWarriorSword::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGWarrior::Load(pArea, pos);
	IGArmedSword::Load(pArea, this, g_bullet_none);
}

void CGWarriorSword::Unload(void)
{
	CGWarrior::Unload();
	IGArmedSword::Unload();
}

// Process command from keyboard, mouse, or other devices
bool CGWarriorSword::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_MOUSE_RIGHT:
		{
		GPOINT pos;
		pos.x = wParam;
		pos.y = lParam;
		SetDestinePosition(pos);
		SetObjAttack(pHostArea->WhoIsHere(pos, this));
		m_posture = g_warrior_walk;
		/*m_angle = VectorDirection(m_pos_foot, pos);
		m_posture = g_warrior_standby;*/
		}
		break;
	default:
		return CGWarrior::CommandProcessor(cmd, wParam, lParam);
	}

	return true;
}

// timer division
void CGWarriorSword::TimerCallback_10MS(int delta_t)
{
	CGWarrior::TimerCallback_10MS(delta_t);
	IGArmedSword::TimerCallback_10MS(delta_t);
	if(InShootRange())
		m_speed = 0;
}

void CGWarriorSword::TimerCallback_100MS(int delta_t)
{
	CGWarrior::TimerCallback_100MS(delta_t);
	IGArmedSword::TimerCallback_100MS(delta_t);
}

CGWarriorBowman::CGWarriorBowman(CGBitmapTempletBowman *man, CGBitmapTempletBullet *bullet)
{
	pTemplate = man;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_size = man->GetSize();
	m_default_speed = 5;
	m_type_id = g_type_warrior;
	m_posture = g_warrior_standby;
	m_defense = 8;

	IGArmedArrow::m_template_bullet = bullet;
	IGArmedArrow::m_shoot_range = 128;
//	m_execute = 30;
}

CGWarriorBowman::CGWarriorBowman(CGBitmapTempletChariot *man, CGBitmapTempletBullet *bullet)
{
	pTemplate = man;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_size = man->GetSize();
	m_default_speed = 5;
	m_type_id = g_type_warrior;
	m_posture = g_warrior_standby;
	m_defense = 8;

	IGArmedArrow::m_template_bullet = bullet;
	IGArmedArrow::m_shoot_range = 256;
//	m_execute = 30;
}

CGWarriorBowman::~CGWarriorBowman(void)
{
}

void CGWarriorBowman::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGWarrior::Load(pArea, pos);
	IGArmedArrow::Load(pArea, this, g_arrow_1);
}

void CGWarriorBowman::Unload(void)
{
	CGWarrior::Unload();
	IGArmedArrow::Unload();
}

// Process command from keyboard, mouse, or other devices
bool CGWarriorBowman::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_MOUSE_RIGHT:
		{
		GPOINT pos;
		pos.x = wParam;
		pos.y = lParam;
		SetDestinePosition(pos);
		SetObjAttack(pHostArea->WhoIsHere(pos, this));
		m_posture = g_warrior_walk;
		/*m_angle = VectorDirection(m_pos_foot, pos);
		m_posture = g_warrior_standby;*/
		}
		break;
	default:
		return CGWarrior::CommandProcessor(cmd, wParam, lParam);
	}

	return true;
}

// timer division
void CGWarriorBowman::TimerCallback_10MS(int delta_t)
{
	CGWarrior::TimerCallback_10MS(delta_t);
	IGArmedArrow::TimerCallback_10MS(delta_t);
	if(InShootRange())
		m_speed = 0;
}

void CGWarriorBowman::TimerCallback_100MS(int delta_t)
{
	CGWarrior::TimerCallback_100MS(delta_t);
	IGArmedArrow::TimerCallback_100MS(delta_t);
}

void CGWarriorBowman::TimerCallback_500MS(int delta_t)
{
	CGWarrior::TimerCallback_500MS(delta_t);
	IGArmedArrow::TimerCallback_500MS(delta_t);
}

// get the position where the arrow is shot
GPOINT CGWarriorBowman::GetPositionShoot(void)
{
	GPOINT pos;
	pos.x = m_origin.x + ((m_size.left /*+ m_size.right*/)/*>>1*/);
	pos.y = m_origin.y + ((m_rect.top + m_rect.bottom)/3);
	return pos;
}
