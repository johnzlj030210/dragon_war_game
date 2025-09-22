#pragma once
#include "typedefines.h"
#include "GameArea.h"

//#pragma warning (disable:4250)
class CGameObject
{
	friend class CGAreaAct;
	friend class CGLogicMap;
	friend class IGWalkAuto;
	friend class IGWalkAutoAx;
public:
	CGameObject();
	virtual ~CGameObject();
	virtual void Load(CGAreaAct *pArea, GPOINT &pos);
	virtual void Unload(void);
	virtual G_SHIFT_RESULT Shift(int x, int y);
protected:
	RECT m_rect;
	RECT m_size; // the object's location area
	GPOINT m_origin;
	CGAreaAct *pHostArea;
	// where the object stands
	GPOINT m_pos_foot;
public:
	// Get the main surface of the host area for picture drawing
	GHANDLE GetObjectSurface(int number);
	// Change the object's status every delta_t clock, called by the timer automatically
	virtual void TimerCallback_10MS(int delta_t) = 0;
	// timer division
	virtual void TimerCallback_500MS(int delta_t);
	virtual void TimerCallback_100MS(int delta_t);
	virtual void TimerCallback_Test(int delta_t);
	virtual void TimerCallback_2S(int delta_t);  
	// Transform the coordinate in map into the active area
	GPOINT CoordinateTransformIntoArea(GPOINT &pos);
	// Get the object's position
	GPOINT GetPosition(void);
	void SetPosition(GPOINT & pos);
	void SetPositionFoot(GPOINT & pos);
	// get the smoke position of the object
	virtual GPOINT GetPositionSmoke(void);
	// get the bomb position of the object
	virtual GPOINT GetPositionBomb(void);
	// get the flag position of the object
	virtual GPOINT GetPositionFlag(void);
	// Get the object width
	int GetWidth(void);
	// Get the object height
	int GetHeight(void);
	// Redraw the object
	virtual void UpdateView(bool show) = 0;
protected:
	// The game object's type: map, building, car, etc
	G_TYPE_ID m_type_id;
	// The bmp template file
	CGBitmapTemplet *pTemplate;
protected:
	// direction in degree-0, 45, 90,..., 315
	int m_angle;
public:
	// Get the object's type id: map, building, car, etc
	G_TYPE_ID GetTypeId(void);
	// Process command from keyboard, mouse, or other devices
	virtual bool CommandProcessor(int cmd, int wParam, int lParam);
	// Get the object's location size
	RECT GetSize(void);
	// Get the object's angle, 0 if it's a building or map
	int GetAngle(void);
	// bombing an object
	virtual void Bombing(G_BOMB_TYPE type);
protected:
	// life value of the object, when is 0, the object will be ruined
	int m_blood;
	// constructing persent, 100 if finished
	int m_ConstructPersent;
public:
	int GetConstructPersent(void)	{		return m_ConstructPersent;	}
	// whether the object has already been unloaded
	bool Unloaded(void);
protected:
	// position in logic map
	GPOINT m_logic_pos;
public:
	RECT GetLocation(void);
protected:
	// the object's walk boundary inside the map
	RECT m_boundary;
	// group the object is from
	int m_team;
public:
	// the distance to the other object
	int DistanceToAnother(CGameObject * pObj);
protected:
	// power of defense
	int m_defense;
public:
	// being shoot by enemy by using arm
	virtual void BeingShoot(G_ARM_TYPE arm);
	// get the center position of the object
	GPOINT GetPositionCenter(void);
	// reduce m_blood
	virtual bool FlowBlood(int blood);
	// The gun position of the object
	virtual GPOINT GetPositionShoot(void);
	// set the object's blood
	void SetBlood(int blood);
	// get the blood of the object
	int GetBlood(void);
	bool CanBeDeletedSafely(void);
	// return m_team
	int GetTeamId(void)	{	return m_team;	}
public:

	// get the m_pos_foot
	GPOINT GetPositionFoot(void){	return m_pos_foot;	}
	// insert the object in the queue according to foot position
	void AddtoQueuebyFoot(CGQueue<CGameObject*> & queue);
	// update the position in the queue according to its foot position change
	void UpdateQueuebyFoot(CGQueue<CGameObject*> & queue);
protected:
	// count for safe deleting
	int m_del_count;
public:
	// get spare position near the pos
	GPOINT GetFreePositionNear(GPOINT & foot_pos);
	// transform foot position into origin position
	GPOINT GetOriginFromFootPos(GPOINT & foot_pos);
};

class CGMap : public CGameObject
{
	friend class CGAreaAct;
	friend class CGLogicMap;
public:
	CGMap(CGBitmapTempletMap *map);
	CGMap();
	virtual ~CGMap();
	void Load(CGAreaAct *pArea, GPOINT &pos);
	G_SHIFT_RESULT Shift(int x, int y);
	void ShiftTo(int x, int y);
public:
	void UpdateView(bool show);
protected:
	int m_blockNumX;
	int m_blockNumY;
	int m_blockSizeX;
	int m_blockSizeY;
public:
	// Change the map's status every delta_t clock, called by the timer automatically
	void TimerCallback_10MS(int delta_t);
	// Process command from keyboard, mouse, or other devices
	bool CommandProcessor(int cmd, int wParam, int lParam);
protected:
	// The roll speed of the map
	int m_roll_step;
public:
	// localize the object in the logic map
	void Localize(CGameObject * pObj);
	void Unlocalize(CGameObject * pObj);
	// detect if anything locates at the point
	CGameObject * WhoIsHere(GPOINT & pos);
	// Get the width of the map
	int GetWidth(void);
	// Get the height of the map
	int GetHeight(void);
};

class CGRoad : public CGameObject
{
};

