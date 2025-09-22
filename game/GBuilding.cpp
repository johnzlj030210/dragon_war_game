#include "StdAfx.h"
#include ".\gbuilding.h"
#include ".\gamearea.h"
#include "..\wrapper\wrapper.h"
#include "GSmoke.h"
#include "GBullet.h"
#include "GLogicMap.h"
#include "common.h"

CGBuilding::CGBuilding(void)
{
	m_type_id = g_type_building;
	pTemplate = NULL;
	m_angle = 0;
}

CGBuilding::CGBuilding(CGBitmapTempletBuild *build,
					   CGBitmapTempletSmoke *smoke,
					   CGBitmapTempletFlag *flag,
					   BUILD_ID id)
{
	RECT rect1;

	m_type_id = g_type_building;
	pTemplate = build;

	build->SourceBlockFetch(id, m_blood, rect1);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = rect1.right - rect1.left;
	m_rect.bottom = rect1.bottom - rect1.top;

	m_size.left = m_rect.left + 5;
	m_size.right = m_rect.right - 5;
	m_size.top = m_rect.top + 10;
	m_size.bottom = m_rect.bottom;
//	area.top = area.bottom/2;

	m_build_id = id;
	m_angle = 0;
	m_defense = 5;
	m_team = 2;
	m_template_smoke = smoke;
	m_template_flag = flag;
	m_ConstructPersent = 100;
}

CGBuilding::~CGBuilding(void)
{
}

void CGBuilding::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGameObject::Load(pArea, pos);
	IGBurnSmoke::Load(pArea, this);
	IGFlagWave::Load(pArea, this);
}

void CGBuilding::Unload(void)
{
	CGameObject::Unload();
	IGBurnSmoke::Unload();
	IGFlagWave::Unload();
}

// Fresh the building's display
void CGBuilding::TimerCallback_10MS(int delta_t)
{
	AddingBlood(m_blood);
	UpdateView(false);
	// call interface's time callback after the building's updateview
	// so that flag and smoke overlap upon the building
	IGBurnSmoke::TimerCallback_10MS(delta_t);
	IGFlagWave::TimerCallback_10MS(delta_t);
}
void CGBuilding::TimerCallback_100MS(int delta_t)
{
	IGBurnSmoke::TimerCallback_100MS(delta_t);
	IGFlagWave::TimerCallback_100MS(delta_t);
}
void CGBuilding::TimerCallback_Test(int delta_t)
{
	IGBurnSmoke::TimerCallback_Test(delta_t);
}
// timer division
void CGBuilding::TimerCallback_500MS(int delta_t)
{
	IGBurnSmoke::TimerCallback_500MS(delta_t);
}

// Show buildings
void CGBuilding::UpdateView(bool show)
{
	RECT rect1;
	pHostArea->GetRect(rect1);
	GHANDLE surface = GetObjectSurface(0);
	CGBitmapTempletBuild *tempbuild;

	GPOINT pos = CoordinateTransformIntoArea(GetPosition());
	// display focus bar
	if(this == dynamic_cast<CGBuilding *>(pHostArea->GetFocusObject())){
		RECT rect;
		rect.left = rect1.left + pos.x + m_size.left; 
		rect.top = rect1.top + pos.y + m_size.top;
		rect.right = rect1.left + pos.x + m_size.right;
		rect.bottom = rect1.top + pos.y + m_size.bottom;
		SHOW_FOCUS_BAR(rect);
	}
	tempbuild = dynamic_cast<CGBitmapTempletBuild*>(pTemplate);
	if(tempbuild)
		tempbuild->BitmapBlockLayout(surface, rect1, pos, m_build_id, m_ConstructPersent);

	if(show)
		GShowSurface(surface);
}

// Process command from keyboard, mouse, or other devices
bool CGBuilding::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_KEY_LEFT:
	case G_KEY_RIGHT:
	case G_KEY_UP:
	case G_KEY_DOWN:
		break;
	case G_KEY_A:
		break;
	case G_KEY_D:
		break;
	default:
		return false;
	}

	pHostArea->SetTrackFocus(true);

	return true;
}

// show a high light bar under the focus car object
void CGBuilding::ShowFocusBar(RECT &rect)
{
	int stride;
	char *ptr = 0;
	long *ptr1;
	RECT rect0 = rect;
	pHostArea->LimitWithin(rect0);
	RECT rect1 = rect0;

	GLockDisplayBuffer(GetObjectSurface(0), rect1, &ptr, stride);
	if(!ptr)
		return;
	
	int width = rect.right-rect.left;
	int height = rect.bottom-rect.top;
	int width1 = rect1.right-rect1.left;
	int height1 = rect1.bottom-rect1.top;
	int sector;

	if(m_blood <= 0)
		sector = rect.left;
	else if(m_blood >= 100)
		sector =rect.right;
	else
		sector = rect.left + width*m_blood/100;

	for(int i=rect.top; i<rect.bottom; i++)
	{
		if(i>=rect1.top && i<rect1.bottom)
		{
			if(rect.left==rect1.left)
			{
				ptr1 = (long *)ptr + (i-rect1.top)*stride/4;
				*ptr1 = FOCUSBAR_COLOR_BACK;
			}
			if(rect.right==rect1.right)
			{
				ptr1 = (long *)ptr + (i-rect1.top)*stride/4 + width1;
				*ptr1 = FOCUSBAR_COLOR_BACK;
			}
		}
	}
	for(int j=rect.left; j<rect.right; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			if(rect.top == rect1.top)
			{
				ptr1 = (long *)ptr + (j-rect1.left);
				*ptr1 = FOCUSBAR_COLOR_BACK;
			}
		}
	}
	for(int j=rect.left; j<sector; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.bottom-4; i<rect.bottom; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (long *)ptr + (i-rect1.top)*stride/4 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_FRONT;
				}
			}
		}
	}
	for(int j=sector; j<rect.right; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.bottom-4; i<rect.bottom; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (long *)ptr + (i-rect1.top)*stride/4 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_BACK;
				}
			}
		}
	}
	GUnlockDisplayBuffer(GetObjectSurface(0), rect1);
}


// show a high light bar under the focus car object
void CGBuilding::ShowFocusBar_16bit(RECT &rect)
{
	int stride;
	char *ptr = 0;
	short *ptr1;
	RECT rect0 = rect;
	pHostArea->LimitWithin(rect0);
	RECT rect1 = rect0;

	GLockDisplayBuffer(GetObjectSurface(0), rect1, &ptr, stride);
	if(!ptr)
		return;
	
	int width = rect.right-rect.left;
	int height = rect.bottom-rect.top;
	int width1 = rect1.right-rect1.left;
	int height1 = rect1.bottom-rect1.top;
	int sector;

	if(m_blood <= 0)
		sector = rect.left;
	else if(m_blood >= 100)
		sector =rect.right;
	else
		sector = rect.left + width*m_blood/100;

	for(int i=rect.top; i<rect.bottom; i++)
	{
		if(i>=rect1.top && i<rect1.bottom)
		{
			if(rect.left==rect1.left)
			{
				ptr1 = (short *)ptr + (i-rect1.top)*stride/2;
				*ptr1 = FOCUSBAR_COLOR_BACK_16B;
			}
			if(rect.right==rect1.right)
			{
				ptr1 = (short *)ptr + (i-rect1.top)*stride/2 + width1;
				*ptr1 = FOCUSBAR_COLOR_BACK_16B;
			}
		}
	}
	for(int j=rect.left; j<rect.right; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			if(rect.top == rect1.top)
			{
				ptr1 = (short *)ptr + (j-rect1.left);
				*ptr1 = FOCUSBAR_COLOR_BACK_16B;
			}
		}
	}
	for(int j=rect.left; j<sector; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.bottom-4; i<rect.bottom; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (short *)ptr + (i-rect1.top)*stride/2 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_FRONT_16B;
				}
			}
		}
	}
	for(int j=sector; j<rect.right; j++)
	{
		if(j>=rect1.left && j<rect1.right)
		{
			for(int i=rect.bottom-4; i<rect.bottom; i++)
			{
				if(i>=rect1.top && i<rect1.bottom)
				{
					ptr1 = (short *)ptr + (i-rect1.top)*stride/2 + (j-rect1.left);
					*ptr1 = FOCUSBAR_COLOR_BACK_16B;
				}
			}
		}
	}
	GUnlockDisplayBuffer(GetObjectSurface(0), rect1);
}

// reduce m_blood
bool CGBuilding::FlowBlood(int blood)
{
	CGameObject::FlowBlood(blood);
	LosingBlood(m_blood);
	return m_blood>0;
}


CGFort::CGFort(void)
{
	m_type_id = g_type_building;
	pTemplate = NULL;
	m_angle = 0;
}

CGFort::~CGFort(void)
{
}

CGFort::CGFort(CGBitmapTempletBuild *build,
					   CGBitmapTempletSmoke *smoke,
					   CGBitmapTempletFlag *flag,
					   CGBitmapTempletBullet *bullet,
					   BUILD_ID id)
{
	RECT rect1;

	m_type_id = g_type_building;
	pTemplate = build;

	build->SourceBlockFetch(id, m_blood, rect1);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = rect1.right - rect1.left;
	m_rect.bottom = rect1.bottom - rect1.top;

	m_size.left = m_rect.left + 20;
	m_size.right = m_rect.right - 20;
	m_size.top = m_rect.top + 160;
	m_size.bottom = m_rect.bottom - 20;

	m_build_id = id;
	m_angle = 0;
	m_defense = 9;
	m_shoot_range = 1024;
	m_team = 1;
	m_template_smoke = smoke;
	m_template_bullet = bullet;
	m_template_flag = flag;
}

void CGFort::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGBuildingConstructor::Load(pArea, pos);
	IGArmedCannon::Load(pArea, this, g_bullet_1);
}

void CGFort::Unload(void)
{
	CGBuildingConstructor::Unload();
	IGArmedCannon::Unload();
}

// Fresh the building's display
void CGFort::TimerCallback_10MS(int delta_t)
{
	CGBuildingConstructor::TimerCallback_10MS(delta_t);
	if(IsConstructFinished())
		IGArmedCannon::TimerCallback_10MS(delta_t);
}
void CGFort::TimerCallback_100MS(int delta_t)
{
	CGBuildingConstructor::TimerCallback_100MS(delta_t);
	if(IsConstructFinished())
		IGArmedCannon::TimerCallback_100MS(delta_t);
}
void CGFort::TimerCallback_Test(int delta_t)
{
	CGBuildingConstructor::TimerCallback_Test(delta_t);
	if(IsConstructFinished())
		IGArmedCannon::TimerCallback_Test(delta_t);
}
// timer division
void CGFort::TimerCallback_500MS(int delta_t)
{
	CGBuildingConstructor::TimerCallback_500MS(delta_t);
	if(IsConstructFinished())
		IGArmedCannon::TimerCallback_500MS(delta_t);
}

void CGFort::SetAttack(CGameObject *pObj)
{
	SetObjAttack(pObj);
}
// Process command from keyboard, mouse, or other devices
bool CGFort::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch(cmd)
	{
	case G_MOUSE_RIGHT:
		{
		GPOINT pos;
		pos.x = wParam;
		pos.y = lParam;
		SetObjAttack(pHostArea->WhoIsHere(pos, this));
		}
		break;
	default:
		CGBuilding::CommandProcessor(cmd, wParam, lParam);
	}

	return true;
}


// The gun position of the Fort
GPOINT CGFort::GetPositionShoot(void)
{
	GPOINT pos;
	GPOINT pos1 = GetPosition();
	pos.x = pos1.x + ((m_size.left + m_size.right)>>1);
	pos.y = pos1.y + 100;
	return pos;
}

// get the flag position of the object
GPOINT CGFort::GetPositionFlag(void)
{
	GPOINT pos;

	GPOINT pos1 = GetPosition();
	pos.x = pos1.x + 80;
	pos.y = pos1.y;

	switch(m_ConstructPersent/25)
	{
	case 0:
		pos.y += 190;
		break;
	case 1:
		pos.y += 170;
		break;
	case 2:
		pos.y += 140;
		break;
	case 3:
		pos.y += 90;
		break;
	case 4:
		pos.y += 50;
		break;
	}
	return pos;
}

CGBuildingConstructor::CGBuildingConstructor(void)
{
}

CGBuildingConstructor::~CGBuildingConstructor(void)
{
}

CGBuildingConstructor::CGBuildingConstructor(
		CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		BUILD_ID id)
{
	RECT rect1;

	m_type_id = g_type_building;
	pTemplate = build;

	build->SourceBlockFetch(id, m_blood, rect1);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = rect1.right - rect1.left;
	m_rect.bottom = rect1.bottom - rect1.top;

	m_size.left = m_rect.left + 20;
	m_size.right = m_rect.right - 20;
	m_size.top = m_rect.top + 160;
	m_size.bottom = m_rect.bottom - 20;

	m_build_id = id;
	m_angle = 0;
	m_defense = 5;
	m_team = 0;
	m_template_smoke = smoke;
	m_template_flag = flag;
}

void CGBuildingConstructor::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGBuilding::Load(pArea, pos);
	IGGrowConstruct::Load(pArea, this);
	m_ConstructPersent = IGGrowConstruct::m_persent;
}

void CGBuildingConstructor::Unload(void)
{
	CGBuilding::Unload();
	IGGrowConstruct::Unload();
}

// Fresh the building's display
void CGBuildingConstructor::TimerCallback_10MS(int delta_t)
{
	CGBuilding::TimerCallback_10MS(delta_t);
	IGGrowConstruct::TimerCallback_10MS(delta_t);
}
void CGBuildingConstructor::TimerCallback_100MS(int delta_t)
{
	CGBuilding::TimerCallback_100MS(delta_t);
	IGGrowConstruct::TimerCallback_100MS(delta_t);
	m_ConstructPersent = IGGrowConstruct::m_persent;
}
void CGBuildingConstructor::TimerCallback_Test(int delta_t)
{
	CGBuilding::TimerCallback_Test(delta_t);
//	IGGrowConstruct::TimerCallback_Test(delta_t);
}
// timer division
void CGBuildingConstructor::TimerCallback_500MS(int delta_t)
{
	CGBuilding::TimerCallback_500MS(delta_t);
//	IGGrowConstruct::TimerCallback_500MS(delta_t);
}

GPOINT CGBuildingConstructor::GetPositionSmoke(void)
{
	GPOINT pos;

	GPOINT pos1 = GetPosition();
	pos.x = pos1.x + 40;
	pos.y = pos1.y;

	switch(m_ConstructPersent/25)
	{
	case 0:
		pos.y += 190;
		break;
	case 1:
		pos.y += 170;
		break;
	case 2:
		pos.y += 140;
		break;
	case 3:
		pos.y += 80;
		break;
	case 4:
		pos.y += 50;
		break;
	}
	return pos;
}

GPOINT CGBuildingConstructor::GetPositionBomb(void)
{
	GPOINT pos;

	GPOINT pos1 = GetPosition();
	pos.x = pos1.x + 40;
	pos.y = pos1.y;

	switch(m_ConstructPersent/25)
	{
	case 0:
		pos.y += 190;
		break;
	case 1:
		pos.y += 170;
		break;
	case 2:
		pos.y += 150;
		break;
	case 3:
		pos.y += 120;
		break;
	case 4:
		pos.y += 120;
		break;
	}
	return pos;
}
CGBuildingFactory::CGBuildingFactory(void)
{
}

CGBuildingFactory::~CGBuildingFactory(void)
{
}

CGBuildingFactory::CGBuildingFactory(
		CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		BUILD_ID id)
{
	RECT rect1;

	m_type_id = g_type_building;
	pTemplate = build;

	build->SourceBlockFetch(id, m_blood, rect1);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = rect1.right - rect1.left;
	m_rect.bottom = rect1.bottom - rect1.top;

	m_size.left = m_rect.left + 20;
	m_size.right = m_rect.right - 20;
	m_size.top = m_rect.top + 160;
	m_size.bottom = m_rect.bottom - 20;

	m_build_id = id;
	m_angle = 0;
	m_defense = 5;
	m_team = 1;
	m_template_smoke = smoke;
	m_template_flag = flag;
}

void CGBuildingFactory::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGBuildingConstructor::Load(pArea, pos);
	IGGrowProduce::Load(pArea, this);
}

void CGBuildingFactory::Unload(void)
{
	CGBuildingConstructor::Unload();
	IGGrowProduce::Unload();
}

// Fresh the building's display
void CGBuildingFactory::TimerCallback_10MS(int delta_t)
{
	CGBuildingConstructor::TimerCallback_10MS(delta_t);
	IGGrowProduce::TimerCallback_10MS(delta_t);
}

// get the flag position of the object
GPOINT CGBuildingFactory::GetPositionFlag(void)
{
	GPOINT pos;

	GPOINT pos1 = GetPosition();
	pos.x = pos1.x + 70;
	pos.y = pos1.y - 20;

	switch(m_ConstructPersent/25)
	{
	case 0:
		pos.y += 190;
		break;
	case 1:
		pos.y += 150;
		break;
	case 2:
		pos.y += 110;
		break;
	case 3:
		pos.y += 100;
		break;
	case 4:
		pos.y += 80;
		break;
	}
	return pos;
}


CGBuildingHouse::CGBuildingHouse(void)
{
}

CGBuildingHouse::~CGBuildingHouse(void)
{
}
CGBuildingHouse::CGBuildingHouse(
		CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		BUILD_ID id)
{
	RECT rect1;

	m_type_id = g_type_building;
	pTemplate = build;

	build->SourceBlockFetch(id, m_blood, rect1);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = rect1.right - rect1.left;
	m_rect.bottom = rect1.bottom - rect1.top;

	m_size.left = m_rect.left + 20;
	m_size.right = m_rect.right - 20;
	m_size.top = m_rect.top + 160;
	m_size.bottom = m_rect.bottom - 20;

	m_build_id = id;
	m_angle = 0;
	m_defense = 5;
	m_team = 1;
	m_template_smoke = smoke;
	m_template_flag = flag;
}

void CGBuildingHouse::Load(CGAreaAct * pArea, GPOINT &pos)
{
	CGBuildingConstructor::Load(pArea, pos);
	IGGrowProduce::Load(pArea, this);
}

void CGBuildingHouse::Unload(void)
{
	CGBuildingConstructor::Unload();
	IGGrowProduce::Unload();
}

// Fresh the building's display
void CGBuildingHouse::TimerCallback_10MS(int delta_t)
{
	CGBuildingConstructor::TimerCallback_10MS(delta_t);
	IGGrowProduce::TimerCallback_10MS(delta_t);
}

// get the flag position of the object
GPOINT CGBuildingHouse::GetPositionFlag(void)
{
	GPOINT pos;

	GPOINT pos1 = GetPosition();
	pos.x = pos1.x + 100;
	pos.y = pos1.y;

	switch(m_ConstructPersent/25)
	{
	case 0:
		pos.y += 190;
		break;
	case 1:
		pos.y += 170;
		break;
	case 2:
		pos.y += 140;
		break;
	case 3:
		pos.y += 120;
		break;
	case 4:
		pos.y += 110;
		break;
	}
	return pos;
}

GPOINT CGBuildingHouse::GetPositionSmoke(void)
{
	GPOINT pos;

	GPOINT pos1 = GetPosition();
	pos.x = pos1.x + 40;
	pos.y = pos1.y;

	switch(m_ConstructPersent/25)
	{
	case 0:
		pos.y += 190;
		break;
	case 1:
		pos.y += 170;
		break;
	case 2:
		pos.y += 140;
		break;
	case 3:
		pos.y += 100;
		break;
	case 4:
		pos.y += 80;
		break;
	}
	return pos;
}


