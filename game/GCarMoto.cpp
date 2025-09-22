#include "StdAfx.h"
#include ".\gcarmoto.h"
#include "..\wrapper\\wrapper.h"

CGCarMoto::CGCarMoto(void)
{
	m_type_id = g_type_carmoto;
}

CGCarMoto::CGCarMoto(CGBitmapTempletCar *car, 
					 CGBitmapTempletSmoke *smoke)
{
	m_type_id = g_type_carmoto;
	pTemplate = car;

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = pTemplate->GetBlockWidth();
	m_rect.bottom = pTemplate->GetBlockHeight();

	m_size = car->GetSize();
	m_default_speed = 20;

	m_team = 1;
	m_template_smoke = smoke;
}

CGCarMoto::~CGCarMoto(void)
{
}

void CGCarMoto::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGCar::Load(pArea, pos);
	IGBurnSmoke::Load(pArea, this);
	IGDriveMoto::Load(this);
}

void CGCarMoto::Unload(void)
{
	CGCar::Unload();
	IGBurnSmoke::Unload();
	IGDriveMoto::Unload();
}

// Process event when come with the boundary of the map, or other game objects
void CGCarMoto::ComewithBoundary(G_SHIFT_RESULT status)
{
	m_angle = (m_angle+1)%8;

	m_speed = 0;

	if(g_shift_collide == status)
	{
		m_blood -= 1;
	}
	LosingBlood(m_blood);
}

// reduce m_blood
bool CGCarMoto::FlowBlood(int blood)
{
	CGCar::FlowBlood(blood);
	LosingBlood(m_blood);
	return m_blood>0;
}

// Process command from keyboard, mouse, or other devices
bool CGCarMoto::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_KEY_LEFT:
	case G_KEY_RIGHT:
	case G_KEY_UP:
	case G_KEY_DOWN:
	case G_KEY_A:
	case G_KEY_D:
		IGDriveMoto::CommandProcessor(cmd, wParam, lParam);
		break;
	case G_MOUSE_RIGHT:
		{
		GPOINT pos;
		pos.x = wParam;
		pos.y = lParam;
		SetDestinePosition(pos);
		}
		break;
	default:
		return false;
	}
	
	pHostArea->SetTrackFocus(true);

	return true;
}
// timer division
void CGCarMoto::TimerCallback_10MS(int delta_t)
{
	CGCar::TimerCallback_10MS(delta_t);
	IGBurnSmoke::TimerCallback_10MS(delta_t);
}
void CGCarMoto::TimerCallback_100MS(int delta_t)
{
	CGCar::TimerCallback_100MS(delta_t);
	IGBurnSmoke::TimerCallback_100MS(delta_t);
}
void CGCarMoto::TimerCallback_500MS(int delta_t)
{
	CGCar::TimerCallback_500MS(delta_t);
	IGBurnSmoke::TimerCallback_500MS(delta_t);
}

void CGCarMoto::TimerCallback_Test(int delta_t)
{
	CGCar::TimerCallback_Test(delta_t);
	IGBurnSmoke::TimerCallback_Test(delta_t);
}