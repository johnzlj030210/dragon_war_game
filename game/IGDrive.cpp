#include "StdAfx.h"
#include ".\igdrive.h"
#include "..\wrapper\\wrapper.h"
#include "GCar.h"

IGDriveMoto::IGDriveMoto(void)
{
	m_host = NULL;
	m_reverse_gear = false;
}

IGDriveMoto::~IGDriveMoto(void)
{
}

void IGDriveMoto::Load(CGameObject *pObj)
{
	m_host = dynamic_cast<CGCar*>(pObj);
	GAssert(m_host);
}

void IGDriveMoto::Unload(void)
{
	m_host = NULL;
}

// Turn the car 45d left-ward
void IGDriveMoto::TurnLeft(void)
{
	if(m_host->m_speed>=0)		
		m_host->m_angle = (m_host->m_angle+7)%8;
	else
		m_host->m_angle = (m_host->m_angle+1)%8;
}

// Turn the car 45d right-ward
void IGDriveMoto::TurnRight(void)
{
	if(m_host->m_speed>=0)
		m_host->m_angle = (m_host->m_angle+1)%8;
	else
		m_host->m_angle = (m_host->m_angle+7)%8;
}

// Speed up the moto car, forwards or backwards, the direction depends on the car's angle
void IGDriveMoto::SpeedUp(bool forward)
{
	if(forward && m_host->m_speed>=0){
		m_host->m_speed = min(m_host->m_speed+10, 50);
	}else if(!forward && m_host->m_speed<=0){
		m_host->m_speed = max(m_host->m_speed-5, -20);
	}else{//set fault scaler
	}
}

// Stop the moto car, no matter go forwards or backwards.
void IGDriveMoto::BrakeUp(void)
{
	if(m_host->m_speed >0){
		m_host->m_speed = max(0, m_host->m_speed-30);
	}else{
		m_host->m_speed = min(0, m_host->m_speed+30);
	}
}

bool IGDriveMoto::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_KEY_LEFT:
		m_host->m_driving_mode = g_drive_manual;
		TurnLeft();
		break;
	case G_KEY_RIGHT:
		m_host->m_driving_mode = g_drive_manual;
		TurnRight();
		break;
	case G_KEY_UP:
		m_host->m_driving_mode = g_drive_manual;
		BrakeUp();
		break;
	case G_KEY_DOWN:
		m_host->m_driving_mode = g_drive_manual;
		if(m_reverse_gear)
			SpeedUp(false);
		else
			SpeedUp(true);
		break;
	case G_KEY_A:
		m_host->m_driving_mode = g_drive_manual;
		m_reverse_gear = false;
		break;
	case G_KEY_D:
		m_host->m_driving_mode = g_drive_manual;
		m_reverse_gear = true;
		break;
	default:
		return false;
	}
	return true;
}
