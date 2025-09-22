#pragma once
#include "GQueue.h"
#include "gbitmaptemplet.h"

#define TINY_AREA_WIDTH		200
#define TINY_AREA_HEIGHT	200

class CGArea 
{
public:
	CGArea();
	virtual ~CGArea();
public:
	virtual void GetRect(RECT &rect1) = 0;
	virtual void OnRedraw(void) = 0;
	// Change the area's status every delta_t clock, called by the timer automatically
	virtual void TimerCallback_10MS(int delta_t) = 0;
	// active area command processor
	virtual bool CommandProcessor(int cmd, int wParam, int lParam) = 0;
protected:
	// positon on the display window
	GPOINT m_pos;
	// rect in the surface
	RECT m_rect;
	// main surface for drawing
	GHANDLE main_Surface;
	GHANDLE second_Surface;
};

class CGAreaAct : public CGArea
{
	friend class CGameObject;
	friend class CGAreaTiny;
	friend class CGAreaMenu;
	friend class IGGrowProduce;
	friend class IGWalkAuto;
public:
	CGAreaAct();
	virtual ~CGAreaAct();
	void GetRect(RECT &rect1);
	void ShiftGameMapTo(int x, int y);
	// Load all game objects stored in the queue
	void LoadAllObjects(void);
	// Unload all game objects from the area
	void UnloadAllObjects(void);
	int GetWidth(){ return m_rect.right - m_rect.left + 1;};
	int GetHeight(){ return m_rect.bottom - m_rect.top + 1;};
public:
	void ShiftGampMap(int x, int y);
	// on message that window is redrawed
	void OnRedraw(void);
protected:
	// The current in used map object in the area
	class CGMap *pMapCurrent;
	CGQueue<CGameObject *> m_queue;
public:
	// Change the area's status every delta_t clock, called by the timer automatically
	void TimerCallback_10MS(int delta_t);
	void TimerCallback_500MS(int delta_t);  // timer division
	void TimerCallback_100MS(int delta_t);  // timer division
	void TimerCallback_Test(int delta_t);
	void TimerCallback_2S(int delta_t);  // timer division
	// Get the origin of the object
	GPOINT GetObjectPosition(CGameObject * obj);
	// Get the origin of the map in area
	GPOINT GetMapPosition(void);
	// Get the focused car object in the area
	CGameObject * GetFocusObject(void);
	// Get the m_rect of the map in the area
	void GetMapRect(RECT & rect1);
protected:
	// The focused object in the active area
	CGameObject *pFocusObj;
//	CGQueue<CGameObject *> m_queue_smoke;
public:
	// Set an object as the focus
	void SetFocusObject(CGameObject * pObj);
	// Set the next object in the area as focus
	bool SetFocusObjectNext(void);
protected:
	// Template of the object
	CGBitmapTempletBuild * pBuildTemplate;
	CGBitmapTempletMap * pMapTemplate;
	CGBitmapTempletCar * pCarTemplate[4];
	CGBitmapTempletConstructor * pConstructTemplate;
	CGBitmapTempletFlag * pFlagTemplate;
	// smoking template
	CGBitmapTempletSmoke *pSmokeTemplate;
	CGBitmapTempletBomb *pBombTemplate;
	CGBitmapTempletBullet *pBulletTemplate;
	// warrior templet
	CGBitmapTempletWarrior *pWarriorTemplate;
	CGBitmapTempletCavalry *pCavalryTemplate;
	CGBitmapTempletBowman *pBowmanTemplate;
	CGBitmapTempletChariot *pChariotTemplate;
public:
	// Set focus on the object at the position, or on the map
	void SetFocusObjectAt(GPOINT & pos);
private:
	// decide is the area's view tracking the focus object
	bool m_IsTrackFocus;
public:
	// enable/disable tracking with focus object
	void SetTrackFocus(bool track);
	// decide if tracking with the focus object
	bool IfTrackFocus(void);
private:
	// cursor's position
	GPOINT m_cursor_pos;
public:
	// Notify the area the new cursor position when mouse is moving
	void NotifyCursorPositionChange(GPOINT & pos);
	// get the cursor position
	void GetCursorPostion(GPOINT & pos);
	// active area command processor
	bool CommandProcessor(int cmd, int wParam, int lParam);
protected:
	// When cursor reach the edge, the map should move along with it
	void ShiftGameMapWithCursor(void);
public:
	// bombing the object in the area, return the bomb id
	void StartBombing(CGameObject * pObj, G_BOMB_TYPE type);
private:
	class CGBomb *m_bombs;
public:
	CGMap * GetCurrentMap(void);
	// Which object locates at the position
	CGameObject * WhoIsHere(GPOINT & pos, CGameObject *notObject);
	// 100MS timer callback with delta diferrence
	void TimerCallback_100MS_delta1(int delta_t);
	// limit rect within the area boundary
	void LimitWithin(RECT & rect);
protected:
	G_MENU_OPERATE m_operate;
public:
	// if the rect intersect with other objects, return true
	bool InterSectWithOthers(RECT & rect, CGameObject *pObjEx = NULL);
	// if the rect over-range the current map's boundary, return true
	bool OverrangeMapBoundary(RECT & rect);
};

class CGAreaTiny :
	public CGArea
{
public:
	CGAreaTiny(CGAreaAct *pArea);
	virtual ~CGAreaTiny(void);

	void GetRect(RECT &rect1);
	void OnRedraw(void);
	// Change the area's status every delta_t clock, called by the timer automatically
	void TimerCallback_10MS(int delta_t);
	// active area command processor
	bool CommandProcessor(int cmd, int wParam, int lParam);
private:
	CGAreaAct* pHostArea;
	// draw frame line outside the tiny area
	void DrawOutFrame(void);
	void DrawOutFrame_16bit(void);
	// show the map and all objects in a small global map
	void ShowGlobalMap(void);
	void ShowGlobalMap_16bit(void);
	// mapwidth / tinywidth
	int m_map_ratio_x;
	// mapheight / tinyheight
	int m_map_ratio_y;
	// photo frame
	CGBitmapTempletPhoto *pFrameTemplet;
	// show a rectangle shape for the current viewible active area
	void ShowCurrentView(void);
	void ShowCurrentView_16bit(void);
};

class CGAreaMenu :
	public CGArea
{
	friend class CGAreaButton;
public:
	CGAreaMenu(CGAreaAct *pArea);
	virtual ~CGAreaMenu(void);

public:
	void GetRect(RECT &rect1);
	void OnRedraw(void);
	// Change the area's status every delta_t clock, called by the timer automatically
	void TimerCallback_10MS(int delta_t);
	// active area command processor
	bool CommandProcessor(int cmd, int wParam, int lParam);
	CGAreaButton *WhichAreaIsMouse(GPOINT & pos);
private:
	CGAreaAct* pHostArea;
	// draw frame line outside the tiny area
	void DrawOutFrame(void);
	void DrawOutFrame_16bit(void);
	// photo frame
	CGBitmapTempletPhoto *pFrameTemplet;
	CGBitmapTempletLibs *pButtonTemplet;

public:
	// show buildings and cars in the menu
	void LoadAllButtons(void);
	void RemoveAllButtons(void);
private:
	int m_icon_width;
	int m_icon_height;
protected:
	CGQueue<CGAreaButton *> m_button_queue[3];//according to m_page_id 
public:
	void SetOperateType(G_MENU_OPERATE_TYPE typeId, G_CONSTRUCT_ITEM itemId);
	void GetOperateType(G_MENU_OPERATE_TYPE & typeId, G_CONSTRUCT_ITEM &itemId);
private:
	int m_page_id;
protected:
	void ShowButtonOnPage(void);
};

class CGAreaButton :
	public CGArea
{
public:
	CGAreaButton(CGAreaMenu *area, RECT &rect);
	virtual ~CGAreaButton(void);
	void GetRect(RECT &rect1);
	void OnRedraw(void);
	// Change the area's status every delta_t clock, called by the timer automatically
	void TimerCallback_10MS(int delta_t);
	// active area command processor
	bool CommandProcessor(int cmd, int wParam, int lParam);
protected:
	// g_button_up, g_btton_down
	G_BUTTON_STATUS m_status;
public:
	// load the button with a string
	void Load(char * topic);
public:
	// show the button
	void Show(void);
	// Hide the button
	void Hide(void);
protected:
	// host menu area
	CGAreaMenu *pHostArea;
	char *m_topic;
	// whether active or not
	bool m_active;
	// draw the out shape
	void DrawOutFrame(void);
	void DrawOutFrame_16bit(void);
public:
	// load a picture into the button
	void Load(CGBitmapTempletLibs * picture, int index);
protected:
	CGBitmapTempletLibs *m_picture;
public:
	void OnClick(void);
private:
	int m_IconId;
};

