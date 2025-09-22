#include "StdAfx.h"
#include "GameArea.h"
#include "GameLoad.h"
#include "typedefines.h"
#include "..\wrapper\\wrapper.h"
#include ".\gamearea.h"
#include "common.h"
#include "GBuilding.h"

#define FIRST_BUTTON_POS_X	20
#define FIRST_BUTTON_POS_Y	65
#define BUTTON_INTER_X		55
#define BUTTON_INTER_Y		70
#define BUTTON_WIDTH		50
#define BUTTON_HEIGHT		60

CGAreaTiny::CGAreaTiny(CGAreaAct *pArea)
: pHostArea(NULL)
, m_map_ratio_x(0)
, m_map_ratio_y(0)
{
	int x,y;
	RECT mapRect;
	pHostArea = pArea;
	GGetWindowRect(m_rect);
	x = m_rect.left;
	y = m_rect.top;
	m_rect.left = m_rect.right - TINY_AREA_WIDTH;
	m_rect.top = m_rect.bottom - TINY_AREA_HEIGHT;
	main_Surface = pArea->main_Surface;
	m_pos.x = m_rect.left - x;
	m_pos.y = m_rect.top - y;
	pHostArea->GetMapRect(mapRect);
	m_map_ratio_x = (mapRect.right-mapRect.left)/TINY_AREA_WIDTH;
	m_map_ratio_y = (mapRect.bottom-mapRect.top)/TINY_AREA_HEIGHT;

	pFrameTemplet = new CGBitmapTempletPhoto(IDB_BITMAP_FRAME, TINY_AREA_WIDTH, TINY_AREA_HEIGHT);
}

CGAreaTiny::~CGAreaTiny(void)
{
	delete pFrameTemplet;
}

void CGAreaTiny::GetRect(RECT &rect1)
{
	rect1 = m_rect;
}

// on message that window is redrawed
void CGAreaTiny::OnRedraw(void)
{
	int x,y;

	GGetWindowRect(m_rect);
	x = m_rect.left;
	y = m_rect.top;
	m_rect.left = m_rect.right - TINY_AREA_WIDTH;
	m_rect.top = m_rect.bottom - TINY_AREA_HEIGHT;

	m_pos.x = m_rect.left - x;
	m_pos.y = m_rect.top - y;

	SHOW_GLOBAL_MAP();
	DRAW_OUT_FRAME();
}

// Change the area's status every delta_t clock, called by the timer automatically
void CGAreaTiny::TimerCallback_10MS(int delta_t)
{
	SHOW_GLOBAL_MAP();
	SHOW_CURRENT_VIEW();
	DRAW_OUT_FRAME();
}
// draw frame line outside the tiny area
void CGAreaTiny::DrawOutFrame(void)
{
	pFrameTemplet->BitmapBlockLayout(main_Surface, m_rect);
}
void CGAreaTiny::DrawOutFrame_16bit(void)
{
	pFrameTemplet->BitmapBlockLayout(main_Surface, m_rect);
}

// show the map and all objects in a small global map
void CGAreaTiny::ShowGlobalMap(void)
{
	char *buffer = 0;
	int stride;
	int *ptr;
	CGameObject *pObj;
	RECT rect1 = m_rect;
	int color;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;

	for(int i=0; i<rect1.bottom-rect1.top; i++)
		for(int j=0; j<rect1.right-rect1.left; j++)
	{
		ptr = (int*)buffer + i*stride/4 + j;
		*ptr = GLOBAL_MAP_COLOR1;
	}

	pHostArea->m_queue.entry2();
	for(bool b = pHostArea->m_queue.head(pObj); 
		b==true; 
		b = pHostArea->m_queue.next(pObj))
	{
		if(pObj->GetTypeId() == g_type_map)
			continue;
		if(pObj->GetBlood()<=0)
			continue;
		if(pObj == pHostArea->GetFocusObject())
			color = GLOBAL_MAP_COLOR2;
		else
			color = GLOBAL_MAP_COLOR3;
		RECT locate = pObj->GetLocation();
		locate.left /= m_map_ratio_x;
		locate.right /= m_map_ratio_x;
		locate.top /= m_map_ratio_y;
		locate.bottom /= m_map_ratio_y;
		if(locate.right==locate.left) locate.right++;
		if(locate.bottom==locate.top) locate.bottom++;

		//in case of over-range of locked memory
		locate.left = MAX(locate.left, 0);
		locate.right = MAX(locate.right, 0);
		locate.top = MAX(locate.top, 0);
		locate.bottom = MAX(locate.bottom, 0);
		locate.left = MIN(locate.left, rect1.right - rect1.left);
		locate.right = MIN(locate.right, rect1.right - rect1.left);
		locate.top = MIN(locate.top, rect1.bottom - rect1.top);
		locate.bottom = MIN(locate.bottom, rect1.bottom - rect1.top);

		for(int x=locate.left; x<locate.right; x++)
			for(int y=locate.top; y<locate.bottom; y++)
			{
			ptr = (int*)buffer + y*stride/4 + x;
			*ptr = color;
			}
	}
	pHostArea->m_queue.leave2();

	GUnlockDisplayBuffer(main_Surface, rect1);
}


// show the map and all objects in a small global map
void CGAreaTiny::ShowGlobalMap_16bit(void)
{
	char *buffer = 0;
	int stride;
	short *ptr;
	CGameObject *pObj;
	RECT rect1 = m_rect;
	int color;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;

	for(int i=0; i<rect1.bottom-rect1.top; i++)
		for(int j=0; j<rect1.right-rect1.left; j++)
	{
		ptr = (short*)buffer + i*stride/2 + j;
		*ptr = GLOBAL_MAP_COLOR1_16B;
	}

	pHostArea->m_queue.entry2();
	for(bool b = pHostArea->m_queue.head(pObj); 
		b==true; 
		b = pHostArea->m_queue.next(pObj))
	{
		if(pObj->GetTypeId() == g_type_map)
			continue;
		if(pObj->GetBlood()<=0)
			continue;
		if(pObj == pHostArea->GetFocusObject())
			color = GLOBAL_MAP_COLOR2_16B;
		else
			color = GLOBAL_MAP_COLOR3_16B;
		RECT locate = pObj->GetLocation();
		locate.left /= m_map_ratio_x;
		locate.right /= m_map_ratio_x;
		locate.top /= m_map_ratio_y;
		locate.bottom /= m_map_ratio_y;
		if(locate.right==locate.left) locate.right++;
		if(locate.bottom==locate.top) locate.bottom++;

		//in case of over-range of locked memory
		locate.left = MAX(locate.left, 0);
		locate.right = MAX(locate.right, 0);
		locate.top = MAX(locate.top, 0);
		locate.bottom = MAX(locate.bottom, 0);
		locate.left = MIN(locate.left, rect1.right - rect1.left);
		locate.right = MIN(locate.right, rect1.right - rect1.left);
		locate.top = MIN(locate.top, rect1.bottom - rect1.top);
		locate.bottom = MIN(locate.bottom, rect1.bottom - rect1.top);

		for(int x=locate.left; x<locate.right; x++)
			for(int y=locate.top; y<locate.bottom; y++)
			{
			ptr = (short*)buffer + y*stride/2 + x;
			*ptr = color;
			}
	}
	pHostArea->m_queue.leave2();

	GUnlockDisplayBuffer(main_Surface, rect1);
}

// show a rectangle shape for the current viewible active area
void CGAreaTiny::ShowCurrentView(void)
{
	char *buffer = 0;
	int stride;
	int *ptr;

	GPOINT pos = pHostArea->GetMapPosition();
	RECT rect1;
	rect1.left = pos.x/m_map_ratio_x + m_rect.left;
	rect1.top = pos.y/m_map_ratio_y + m_rect.top;
	rect1.right = rect1.left + pHostArea->GetWidth()/m_map_ratio_x;
	rect1.bottom = rect1.top + pHostArea->GetHeight()/m_map_ratio_y;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;

	for(int i=0; i<rect1.bottom-rect1.top; i++)
	{
		ptr = (int*)buffer + i*stride/4;
		*ptr = VIEW_RECTANG_VALUE;
		ptr = (int*)buffer + i*stride/4 + (rect1.right-rect1.left -1);
		*ptr = VIEW_RECTANG_VALUE;
	}
	for(int j=0; j<rect1.right-rect1.left; j++)
	{
		ptr = (int*)buffer + j;
		*ptr = VIEW_RECTANG_VALUE;
		ptr = (int*)buffer + j + (rect1.bottom-rect1.top - 1)*stride/4;
		*ptr = VIEW_RECTANG_VALUE;
	}

	GUnlockDisplayBuffer(main_Surface, rect1);
}

// show a rectangle shape for the current viewible active area
void CGAreaTiny::ShowCurrentView_16bit(void)
{
	char *buffer = 0;
	int stride;
	short *ptr;

	GPOINT pos = pHostArea->GetMapPosition();
	RECT rect1;
	rect1.left = pos.x/m_map_ratio_x + m_rect.left;
	rect1.top = pos.y/m_map_ratio_y + m_rect.top;
	rect1.right = rect1.left + pHostArea->GetWidth()/m_map_ratio_x;
	rect1.bottom = rect1.top + pHostArea->GetHeight()/m_map_ratio_y;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;

	for(int i=0; i<rect1.bottom-rect1.top; i++)
	{
		ptr = (short*)buffer + i*stride/2;
		*ptr = VIEW_RECTANG_VALUE_16B;
		ptr = (short*)buffer + i*stride/2 + (rect1.right-rect1.left -1);
		*ptr = VIEW_RECTANG_VALUE_16B;
	}
	for(int j=0; j<rect1.right-rect1.left; j++)
	{
		ptr = (short*)buffer + j;
		*ptr = VIEW_RECTANG_VALUE_16B;
		ptr = (short*)buffer + j + (rect1.bottom-rect1.top - 1)*stride/2;
		*ptr = VIEW_RECTANG_VALUE_16B;
	}

	GUnlockDisplayBuffer(main_Surface, rect1);
}

// active area command processor
bool CGAreaTiny::CommandProcessor(int cmd, int wParam, int lParam)
{
	GPOINT pos;
	RECT rect1;
	pHostArea->GetRect(rect1);
	pos.x = (wParam-m_pos.x)*m_map_ratio_x;
	pos.y = (lParam-m_pos.y)*m_map_ratio_y;
	switch (cmd)
	{
		case G_MOUSE_LEFT:
			pHostArea->SetTrackFocus(false);
			//pHostArea->SetFocusObjectAt(pos);
			pHostArea->ShiftGameMapTo(pos.x, pos.y);
			break;
		case G_MOUSE_RIGHT:
		if(pHostArea->pFocusObj)
			pHostArea->pFocusObj->CommandProcessor(cmd, pos.x, pos.y);
			break;
		default:
			;
	}
	return true;
}

CGAreaMenu::CGAreaMenu(CGAreaAct *pArea)
: m_icon_width(0)
, m_icon_height(0)
, m_page_id(0)
{
	int x,y;

	pHostArea = pArea;
	GGetWindowRect(m_rect);
	x = m_rect.left;
	y = m_rect.top;
	m_rect.left = m_rect.right - TINY_AREA_WIDTH;
	m_rect.bottom = m_rect.bottom - TINY_AREA_HEIGHT;

	main_Surface = pArea->main_Surface;
	m_pos.x = m_rect.left - x;
	m_pos.y = m_rect.top - y;
	m_icon_width = 50;
	m_icon_height = 50;
	pFrameTemplet = new CGBitmapTempletPhoto(IDB_BITMAP_MENU, 200, 360);
	pButtonTemplet = new CGBitmapTempletLibs(IDB_BITMAP_LIBS);

	LoadAllButtons();
	ShowButtonOnPage();
}

CGAreaMenu::~CGAreaMenu(void)
{
	delete pFrameTemplet;
	delete pButtonTemplet;
	RemoveAllButtons();
}


void CGAreaMenu::GetRect(RECT &rect1)
{
	rect1 = m_rect;
}

// on message that window is redrawed
void CGAreaMenu::OnRedraw(void)
{
	int x,y;

	GGetWindowRect(m_rect);
	x = m_rect.left;
	y = m_rect.top;
	m_rect.left = m_rect.right - TINY_AREA_WIDTH;
	m_rect.bottom = m_rect.bottom - TINY_AREA_HEIGHT;

	m_pos.x = m_rect.left - x;
	m_pos.y = m_rect.top - y;

//	ShowGlobalMap();
	DRAW_OUT_FRAME();
}

// draw frame line outside the menu area
void CGAreaMenu::DrawOutFrame(void)
{
	char *buffer = 0;
	int stride;
	int *ptr;
	RECT rect1 = m_rect;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;
	for(int i=0; i<rect1.bottom-rect1.top; i++)
		for(int j=0; j<rect1.right-rect1.left; j++)
		{
		ptr = (int*)buffer + i*stride/4 + j;
		*ptr = 0x00000000;
		}

	for(int i=0; i<rect1.bottom-rect1.top; i++)
		for(int j=0; j<5; j++)
	{// MAX/MIN makes memory safe
		ptr = (int*)buffer + i*stride/4 + MIN(rect1.bottom-rect1.top, j);
		*ptr = EDGE_COLOR_VALUE;
		ptr = (int*)buffer + i*stride/4 + MAX((rect1.right-rect1.left -1 - j), 0);
		*ptr = EDGE_COLOR_VALUE;
	}
	for(int j=0; j<rect1.right-rect1.left; j++)
		for(int i=0; i<5; i++)
	{// MAX/MIN makes memory safe
		ptr = (int*)buffer + j + MIN(rect1.bottom-rect1.top, i)*stride/4;
		*ptr = EDGE_COLOR_VALUE;
		ptr = (int*)buffer + j + MAX((rect1.bottom-rect1.top - 1 - i), 0)*stride/4;
		*ptr = EDGE_COLOR_VALUE;
	}

	GUnlockDisplayBuffer(main_Surface, rect1);
	pFrameTemplet->BitmapBlockLayout(main_Surface, m_rect);
}


// draw frame line outside the menu area
void CGAreaMenu::DrawOutFrame_16bit(void)
{
	char *buffer = 0;
	int stride;
	short *ptr;
	RECT rect1 = m_rect;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;
	for(int i=0; i<rect1.bottom-rect1.top; i++)
		for(int j=0; j<rect1.right-rect1.left; j++)
		{
		ptr = (short*)buffer + i*stride/2 + j;
		*ptr = 0x0000;
		}

	for(int i=0; i<rect1.bottom-rect1.top; i++)
		for(int j=0; j<5; j++)
	{// MAX/MIN makes memory safe
		ptr = (short*)buffer + i*stride/2 + MIN(rect1.bottom-rect1.top, j);
		*ptr = EDGE_COLOR_VALUE_16B;
		ptr = (short*)buffer + i*stride/2 + MAX((rect1.right-rect1.left -1 - j), 0);
		*ptr = EDGE_COLOR_VALUE_16B;
	}
	for(int j=0; j<rect1.right-rect1.left; j++)
		for(int i=0; i<5; i++)
	{// MAX/MIN makes memory safe
		ptr = (short*)buffer + j + MIN(rect1.bottom-rect1.top, i)*stride/2;
		*ptr = EDGE_COLOR_VALUE_16B;
		ptr = (short*)buffer + j + MAX((rect1.bottom-rect1.top - 1 - i), 0)*stride/2;
		*ptr = EDGE_COLOR_VALUE_16B;
	}

	GUnlockDisplayBuffer(main_Surface, rect1);
	pFrameTemplet->BitmapBlockLayout(main_Surface, m_rect);
}


// Change the area's status every delta_t clock, called by the timer automatically
void CGAreaMenu::TimerCallback_10MS(int delta_t)
{
	int temp;
	CGAreaButton *button;

	DRAW_OUT_FRAME();

	CGameObject *pObj = pHostArea->GetFocusObject();
	if(NULL != dynamic_cast<CGBuildingHouse*>(pObj))
		temp = 1;
	else if(NULL != dynamic_cast<CGBuildingFactory*>(pObj))
		temp = 2;
	else
		temp = 0;
	if(temp != m_page_id)
	{
		m_page_id = temp;
		ShowButtonOnPage();
	}
	for(bool b = m_button_queue[m_page_id].head(button);
		b == true;
		b = m_button_queue[m_page_id].next(button))
	{
		button->TimerCallback_10MS(delta_t);
	}
}

// active area command processor
bool CGAreaMenu::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch (cmd)
	{
	case G_MOUSE_LEFT:
		{
			GPOINT pos;
			CGAreaButton *button;
			pos.x = wParam;
			pos.y = lParam;
			button = WhichAreaIsMouse(pos);
			if(button)
				button->OnClick();
		}
		break;
	default:
		;
	}
	return true;
}

// decide which area the mouse locates
CGAreaButton * CGAreaMenu::WhichAreaIsMouse(GPOINT & pos)
{
	RECT rect;
	CGAreaButton *button;

	for(bool b = m_button_queue[m_page_id].head(button);
		b == true;
		b = m_button_queue[m_page_id].next(button))
	{
		button->GetRect(rect);
		rect.left += (m_pos.x - m_rect.left);
		rect.top += (m_pos.y - m_rect.top);
		rect.right += (m_pos.x - m_rect.left);
		rect.bottom += (m_pos.y - m_rect.top);

		if(pos.x>=rect.left && pos.x<rect.right
			&& pos.y>=rect.top && pos.y<rect.bottom)
			return button;
	}
	return NULL;
}

// show buildings and cars in the menu
void CGAreaMenu::LoadAllButtons(void)
{
	CGAreaButton *button;
	RECT rect;
	// building button with focusing on map
	for(int i=0; i<3; i++)
	{
		rect.top = FIRST_BUTTON_POS_Y + (i/3)*BUTTON_INTER_Y;
		rect.left = FIRST_BUTTON_POS_X + (i%3)*BUTTON_INTER_X;
		rect.right = rect.left + BUTTON_WIDTH;
		rect.bottom = rect.top + BUTTON_HEIGHT;

		button = new CGAreaButton(this, rect);
		button->Load(pButtonTemplet, i);
		m_button_queue[0].push(button);
	}
	// person with focus on building house
	for(int i=0; i<4; i++)
	{
		rect.top = FIRST_BUTTON_POS_Y + (i/3)*BUTTON_INTER_Y;
		rect.left = FIRST_BUTTON_POS_X + (i%3)*BUTTON_INTER_X;
		rect.right = rect.left + BUTTON_WIDTH;
		rect.bottom = rect.top + BUTTON_HEIGHT;

		button = new CGAreaButton(this, rect);
		button->Load(pButtonTemplet, i+8);
		m_button_queue[1].push(button);
	}
	// viechles with focus on building factory
	for(int i=0; i<4; i++)
	{
		rect.top = FIRST_BUTTON_POS_Y + (i/3)*BUTTON_INTER_Y;
		rect.left = FIRST_BUTTON_POS_X + (i%3)*BUTTON_INTER_X;
		rect.right = rect.left + BUTTON_WIDTH;
		rect.bottom = rect.top + BUTTON_HEIGHT;

		button = new CGAreaButton(this, rect);
		button->Load(pButtonTemplet, i+4);
		m_button_queue[2].push(button);
	}
}

void CGAreaMenu::RemoveAllButtons(void)
{
	CGAreaButton *button;
	for(int i=0; i<3; i++)
	{
		for(bool b = m_button_queue[i].head(button);
			b == true;
			b = m_button_queue[i].pop(button))
		{
			delete button;
		}
	}
}

void CGAreaMenu::ShowButtonOnPage(void)
{
	CGAreaButton *button;
	for(int i=0; i<3; i++)
	{
		for(bool b = m_button_queue[i].head(button);
			b == true;
			b = m_button_queue[i].next(button))
		{
			if(i==m_page_id)
				button->Show();
			else
				button->Hide();
		}
	}
}

// after a button is left clicked
void CGAreaMenu::SetOperateType(G_MENU_OPERATE_TYPE typeId, G_CONSTRUCT_ITEM itemId)
{
	pHostArea->m_operate.typeId = typeId;
	pHostArea->m_operate.itemId = itemId;
	if(itemId>=4 && itemId<8)
	{//produce viechles
		CGBuildingFactory *factory;
		factory = dynamic_cast<CGBuildingFactory*>(pHostArea->GetFocusObject());
		if(factory)
			factory->Produce((G_PRODUCE_ITEM)(itemId-3));
	}else if(itemId>=8 && itemId<12)
	{//produce persons
		CGBuildingHouse *house;
		house = dynamic_cast<CGBuildingHouse*>(pHostArea->GetFocusObject());
		if(house)
			house->Produce((G_PRODUCE_ITEM)(itemId-3));
	}
}

void CGAreaMenu::GetOperateType(G_MENU_OPERATE_TYPE & typeId, G_CONSTRUCT_ITEM &itemId)
{
	typeId = pHostArea->m_operate.typeId;
	itemId = pHostArea->m_operate.itemId;
}


CGAreaButton::CGAreaButton(CGAreaMenu *area, RECT &rect)
: pHostArea(NULL)
, m_topic(NULL)
, m_active(false)
, m_picture(NULL)
, m_IconId(0)
{
	m_status = g_button_up;
	pHostArea = area;
	main_Surface = pHostArea->main_Surface;
	m_pos.x = rect.left; // position in the menu area
	m_pos.y = rect.top;
	m_rect.left = pHostArea->m_rect.left + m_pos.x;
	m_rect.top = pHostArea->m_rect.top + m_pos.y;
	m_rect.right = m_rect.left + (rect.right - rect.left);
	m_rect.bottom = m_rect.top + (rect.bottom - rect.top);
	m_topic = new char[20];
}

CGAreaButton::~CGAreaButton(void)
{
	delete m_topic;
}

void CGAreaButton::GetRect(RECT &rect1)
{
	rect1 = m_rect;
}

// on message that window is redrawed
void CGAreaButton::OnRedraw(void)
{
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;

	m_rect.left = pHostArea->m_rect.left + m_pos.x;
	m_rect.top = pHostArea->m_rect.top + m_pos.y;
	m_rect.right = m_rect.left + width;
	m_rect.bottom = m_rect.top + height;
}

// active area command processor
bool CGAreaButton::CommandProcessor(int cmd, int wParam, int lParam)
{
	switch (cmd)
	{
	case G_MOUSE_LEFT:
		{
		}
		break;
	default:
		;
	}
	return true;
}

// load the button with a string
void CGAreaButton::Load(char * topic)
{
	STR_COPY(m_topic, topic);
}


// load a picture into the button
void CGAreaButton::Load(CGBitmapTempletLibs * picture, int index)
{
	m_picture = picture;
	m_IconId = index;
}

// show the button
void CGAreaButton::Show(void)
{
	m_active = true;
}

// Hide the button
void CGAreaButton::Hide(void)
{
	m_active = false;
}

void CGAreaButton::TimerCallback_10MS(int delta_t)
{
	char *buffer = NULL;
	int stride;
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;
	
	m_rect.left = pHostArea->m_rect.left + m_pos.x;
	m_rect.top = pHostArea->m_rect.top + m_pos.y;
	m_rect.right = m_rect.left + width;
	m_rect.bottom = m_rect.top + height;
	RECT rect1 = m_rect;

	if(m_active)
	{
		if(m_picture)
		{
			m_picture->BitmapBlockLayout(main_Surface, m_rect, m_IconId);
			DRAW_OUT_FRAME(); 
		}
		else if(m_topic)
		{
			GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
			if(!buffer)
				return;
			PRINT_STRING(buffer, stride, m_topic);
			GUnlockDisplayBuffer(main_Surface, rect1);
		}
	}else
	{
		GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
		if(!buffer)
			return;
		PRINT_STRING(buffer, stride, " ");
		GUnlockDisplayBuffer(main_Surface, rect1);
	}
}

// draw the out shape
void CGAreaButton::DrawOutFrame(void)
{
	int *ptr;
	char *buffer = NULL;
	int stride;
	RECT rect1 = m_rect;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;
	for(int i=0; i<rect1.bottom-rect1.top; i++)
	{
		ptr = (int*)buffer + i*stride/4;
		*ptr = EDGE_COLOR_VALUE;
		ptr = (int*)buffer + i*stride/4 + (rect1.right-rect1.left -1);
		*ptr = EDGE_COLOR_VALUE;
	}
	for(int j=0; j<rect1.right-rect1.left; j++)
	{
		ptr = (int*)buffer + j;
		*ptr = EDGE_COLOR_VALUE;
		ptr = (int*)buffer + j + (rect1.bottom-rect1.top - 1)*stride/4;
		*ptr = EDGE_COLOR_VALUE;
	}
	GUnlockDisplayBuffer(main_Surface, rect1);
}


// draw the out shape
void CGAreaButton::DrawOutFrame_16bit(void)
{
	short *ptr;
	char *buffer = NULL;
	int stride;
	RECT rect1 = m_rect;

	GLockDisplayBuffer(main_Surface, rect1, &buffer, stride);
	if(!buffer)
		return;
	for(int i=0; i<rect1.bottom-rect1.top; i++)
	{
		ptr = (short*)buffer + i*stride/2;
		*ptr = EDGE_COLOR_VALUE_16B;
		ptr = (short*)buffer + i*stride/2 + (rect1.right-rect1.left -1);
		*ptr = EDGE_COLOR_VALUE_16B;
	}
	for(int j=0; j<rect1.right-rect1.left; j++)
	{
		ptr = (short*)buffer + j;
		*ptr = EDGE_COLOR_VALUE_16B;
		ptr = (short*)buffer + j + (rect1.bottom-rect1.top - 1)*stride/2;
		*ptr = EDGE_COLOR_VALUE_16B;
	}
	GUnlockDisplayBuffer(main_Surface, rect1);
}

void CGAreaButton::OnClick(void)
{
	if(m_IconId<4)//buildings
		pHostArea->SetOperateType(g_menu_op_type_1, (G_CONSTRUCT_ITEM)m_IconId);
	else if(m_IconId<12)//viechles, persons
	{
		pHostArea->SetOperateType(g_menu_op_type_2, (G_CONSTRUCT_ITEM)m_IconId);
	}
}
