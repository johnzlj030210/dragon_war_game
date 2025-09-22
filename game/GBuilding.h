#pragma once
#include "gameload.h"
#include "IGBurn.h"
#include "IGArmed.h"
#include "IGGrow.h"
#include "IGFlag.h"

class CGBuilding :
	public CGameObject,
	public IGBurnSmoke,
	public IGFlagWave
{
public:
	CGBuilding(void);
	CGBuilding(CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		BUILD_ID id);
	virtual ~CGBuilding(void);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	// Fresh the building's display
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
	// Show buildings
	void UpdateView(bool show);
	// Process command from keyboard, mouse, or other devices
	bool CommandProcessor(int cmd, int wParam, int lParam);
	// reduce m_blood
	bool FlowBlood(int blood);
protected:
	// show a high light bar under the focus building object
	void ShowFocusBar(RECT &rect);
	void ShowFocusBar_16bit(RECT &rect);
	// which of the building in the template is used
	BUILD_ID m_build_id;
};


class CGBuildingConstructor :
	public CGBuilding,
	public IGGrowConstruct
{
public:
	CGBuildingConstructor(void);
	~CGBuildingConstructor(void);
	CGBuildingConstructor(CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		BUILD_ID id);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	// Fresh the building's display
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
	// get the smoke position of the object
	GPOINT GetPositionSmoke(void);
	GPOINT GetPositionBomb(void);
};

class CGFort :
	public CGBuildingConstructor,
	public IGArmedCannon
{
public:
	CGFort(void);
	~CGFort(void);
	CGFort(CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		CGBitmapTempletBullet *bullet,
		BUILD_ID id);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	// Fresh the building's display
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void TimerCallback_500MS(int delta_t);
	void TimerCallback_Test(int delta_t);
	// Process command from keyboard, mouse, or other devices
	bool CommandProcessor(int cmd, int wParam, int lParam);

	GPOINT GetPositionShoot(void);
	// get the flag position of the object
	GPOINT GetPositionFlag(void);

	void SetAttack(CGameObject *pObj);
};

class CGBuildingFactory :
	public CGBuildingConstructor,
	public IGGrowProduce
{
public:
	CGBuildingFactory(void);
	CGBuildingFactory(CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		BUILD_ID id);
	~CGBuildingFactory(void);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);

	void TimerCallback_10MS(int delta_t);
	// get the flag position of the object
	GPOINT GetPositionFlag(void);
};

class CGBuildingHouse :
	public CGBuildingConstructor,
	public IGGrowProduce
{
public:
	CGBuildingHouse(void);
	CGBuildingHouse(CGBitmapTempletBuild *build, 
		CGBitmapTempletSmoke *smoke, 
		CGBitmapTempletFlag *flag,
		BUILD_ID id);
	~CGBuildingHouse(void);
public:
	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	// get the flag position of the object
	GPOINT GetPositionFlag(void);
	GPOINT GetPositionSmoke(void);
};

