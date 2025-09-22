#pragma once
#include "gameload.h"

class CGFlag :
	public CGameObject
{
public:
	CGFlag(CGBitmapTempletFlag *flag);
	virtual ~CGFlag(void);
public:
	void Load(CGAreaAct *pArea, CGameObject *pObj);
	void Unload(void);
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_100MS(int delta_t);
	void UpdateView(bool show);
private:
	G_FLAG_TYPE m_type;
	// Whose flag
	CGameObject *pHostObject;
	// wave the flag
	int m_seq;
};
