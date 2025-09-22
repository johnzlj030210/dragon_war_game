#include "StdAfx.h"
#include ".\gcarmotopeng.h"
#include "common.h"
#include "..\wrapper\\wrapper.h"
#include "GSmoke.h"

CGKiA::CGKiA(void)
{
}

CGKiA::~CGKiA(void)
{
}

CGKiA::CGKiA(CGBitmapTempletCar *car,
							 CGBitmapTempletSmoke *smoke,
							 CGBitmapTempletBullet *bullet)
{
	m_type_id = g_type_carmoto;
	pTemplate = car;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_size = car->GetSize();
	m_default_speed = 30;

	m_team = 1;
	m_template_bullet = bullet;
	m_template_smoke = smoke;
	m_defense = 9;
	m_shoot_range = 256;
}
void CGKiA::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGCarMoto::Load(pArea, pos);
	IGArmedCannon::Load(pArea, this, g_bullet_2);
}

void CGKiA::Unload(void)
{
	CGCarMoto::Unload();
	IGArmedCannon::Unload();
}

// timer division
void CGKiA::TimerCallback_10MS(int delta_t)
{
	CGCarMoto::TimerCallback_10MS(delta_t);
	IGArmedCannon::TimerCallback_10MS(delta_t);
	if(InShootRange())
	{
		if(m_driving_mode == g_drive_automatic)
			m_speed = 0;
	}
}
void CGKiA::TimerCallback_100MS(int delta_t)
{
	CGCarMoto::TimerCallback_100MS(delta_t);
	IGArmedCannon::TimerCallback_100MS(delta_t);
}
void CGKiA::TimerCallback_500MS(int delta_t)
{
	CGCarMoto::TimerCallback_500MS(delta_t);
	IGArmedCannon::TimerCallback_500MS(delta_t);
}

void CGKiA::TimerCallback_Test(int delta_t)
{
	CGCarMoto::TimerCallback_Test(delta_t);
	IGArmedCannon::TimerCallback_Test(delta_t);
}

// Process command from keyboard, mouse, or other devices
bool CGKiA::CommandProcessor(int cmd, int wParam, int lParam)
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
		}
		break;
	default:
		return CGCarMoto::CommandProcessor(cmd, wParam, lParam);
	}

	return true;
}
CGCarPeng::CGCarPeng(void)
{
}

CGCarPeng::~CGCarPeng(void)
{
}

CGCarPeng::CGCarPeng(CGBitmapTempletCar *car,
							 CGBitmapTempletSmoke *smoke,
							 CGBitmapTempletBullet *bullet)
{
	m_type_id = g_type_carmoto;
	pTemplate = car;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_size = car->GetSize();
	m_default_speed = 30;

	m_team = 2;
	m_template_bullet = bullet;
	m_template_smoke = smoke;
	m_defense = 1;
	m_shoot_range = 128;
}
void CGCarPeng::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGCarMoto::Load(pArea, pos);
	IGArmedCannon::Load(pArea, this, g_bullet_1);
}

// When the PengPeng char come with boundary, it will turn right but not stop
void CGCarPeng::ComewithBoundary(G_SHIFT_RESULT status)
{
	int	angle1 = (m_angle + RandomValue())%8;
	m_angle = m_angle==angle1? (m_angle+1)%8 : angle1;
	if(m_driving_mode == g_drive_automatic)
		m_speed = 0;
	if(g_shift_collide == status)
	{
		m_blood -= 1;
	}
	LosingBlood(m_blood);
}

