#pragma once
#include "gameload.h"
#include "typedefines.h"

class CGBomb :
	public CGameObject
{
public:
	CGBomb(void);
	CGBomb(CGBitmapTempletBomb *bomb);
	virtual ~CGBomb(void);

	void Load(CGAreaAct *pArea, GPOINT &pos);
	void Load(CGAreaAct *pArea, CGameObject *pObj, G_BOMB_TYPE type);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void UpdateView(bool show);
private:
	int *m_seq;
	G_BOMB_TYPE *m_type;
	GPOINT *m_pos;
	int index;
};
