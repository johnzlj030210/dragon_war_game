#pragma once
#include "gameload.h"
#include "typedefines.h"

class CGSmoke :
	public CGameObject
{
public:
	CGSmoke(void);
	CGSmoke(CGBitmapTempletSmoke *smoke);
	virtual ~CGSmoke(void);

	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Load(CGAreaAct *pArea, CGameObject *pObj, G_SMOKE_TYPE type);
	void Load(CGAreaAct *pArea, CGameObject *pObj);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void UpdateView(bool show);
private:
	// the smoke moving sequence
	int m_seq;
	// who is smoking
	CGameObject *pHostObject;
	// Smoking like water flow
	GPOINT *m_pos_array;
	// The smoke's m_scale
	int m_scale;
	G_SMOKE_TYPE m_type;
public:
	void ShowingUp(G_SMOKE_TYPE type);
};
