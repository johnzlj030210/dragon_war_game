#include "StdAfx.h"
#include ".\igwalk.h"
#include "common.h"
#include "GLogicMap.h"

IGWalkAuto::IGWalkAuto(void)
{
	m_area = NULL;
	m_walker = NULL;
	m_speed = 0;
	m_driving_mode = g_drive_manual;
	m_default_speed = 20;
	m_dest_pos.x = -1;
	m_dest_pos.y = -1;
	m_next_stop.x = -1;
	m_next_stop.y = -1;
}

IGWalkAuto::~IGWalkAuto(void)
{
}

void IGWalkAuto::Load(CGAreaAct * pArea, CGameObject *pObj)
{
	m_walker = pObj;
	m_area = pArea;
}

void IGWalkAuto::Unload(void)
{
	m_area = NULL;
	m_walker = NULL;
}

// return false if the destination is out of map
bool IGWalkAuto::SetDestinePosition(GPOINT & pos)
{
	RECT rect1;
	GPOINT pos1 = m_walker->GetFreePositionNear(pos); // in case of unreachable dest

	m_area->GetMapRect(rect1);

	m_track_foot.clear();

	if(pos1.x>=rect1.left
		&& pos1.x<rect1.right
		&& pos1.y>=m_walker->m_rect.top
		&& pos1.y<rect1.bottom)
	{
		m_dest_pos = pos1;
		m_next_stop = m_walker->GetPositionFoot();
		m_speed = m_default_speed;		
		m_driving_mode = g_drive_automatic;
	
		return true;
	}
	return false;
}

// Check if the car collide with other objectsin the area
bool IGWalkAuto::CheckCollideOthers(void)
{
	CGameObject *pObj;
	RECT rect, rect1;
	bool retVal = false;

	m_area->m_queue.entry2();
	for(bool b = m_area->m_queue.head(pObj);
		b == true;
		b = m_area->m_queue.next(pObj))
	{
		if(m_walker == pObj ||
			pObj->m_type_id == g_type_map)
			continue;
		rect = m_walker->GetLocation();
		rect1 = pObj->GetLocation();
		if(InterSection(rect, rect1)){

			retVal = true;
			break;
		}
	}
	m_area->m_queue.leave2();
	return retVal;
}

// find path automatically
void IGWalkAuto::WalkAutoFindPath(double & dx, double & dy)
{
	G_NEXTSTEP_RESULT step_status;
	G_NEXTSTOP_RESULT stop_status;

	if(m_driving_mode == g_drive_automatic)
	{
		stop_status = g_stop_ok;
		// flag for FindNextStop
		if(m_next_stop.x == -1 && m_next_stop.y == -1)
		{
			GPOINT pos = m_walker->GetPositionFoot();
			m_track_foot.push(pos);
			stop_status = FindNextStop();
		}
		if(g_stop_ok == stop_status)
		{
			step_status = FindNextStep(dx, dy);
			if(step_status == g_reach_destination){
				m_speed = 0;
				m_driving_mode = g_drive_manual;
			}else if(step_status == g_reach_nextstop){
				m_next_stop.x = -1;
				m_next_stop.y = -1;
			}
		}else // stop_status == g_stop_error
		{
			m_speed = 0;
			m_driving_mode = g_drive_manual;
			m_next_stop.x = -1;
			m_next_stop.y = -1;
		}
	}else // g_drive_manual
	{
		m_dest_pos.x = -1;
		m_dest_pos.y = -1;
		FindNextStep(dx, dy);
	}
}

// return true if the postion has ever been reached
bool IGWalkAuto::IsFootPrint(GPOINT pos)
{
	GPOINT pos1;

	for(bool b = m_track_foot.head(pos1);
		b == true;
		b = m_track_foot.next(pos1))
	{
		if(pos1.x == pos.x && pos1.y == pos.y)
			return true;
	}

	return false;
}

//return true if reach the dest or nextstop.
G_NEXTSTEP_RESULT IGWalkAutoAx::FindNextStep(double &dx, double &dy)
{
	double dx1, dy1;

	int distX, distY;
	double step = m_speed/5.0;

	if(m_dest_pos.x != -1 && m_dest_pos.y != -1)
	{
		GPOINT pos1 = m_walker->GetPositionFoot();

		distX = m_dest_pos.x - pos1.x;
		distY = m_dest_pos.y - pos1.y;

		if((distX>-step && distX<step)
			&& (distY>-step && distY<step))
		{
			dx = distX; dy = distY;
			return g_reach_destination;
		}

		distX = m_next_stop.x - pos1.x;
		distY = m_next_stop.y - pos1.y;

		if((distX>-step && distX<step)
			&& (distY>-step && distY<step))
		{
			dx = distX; dy = distY;
			return g_reach_nextstop;
		}
		m_walker->m_angle = VectorDirection(pos1, m_next_stop);
	}
	switch(m_walker->m_angle)
	{
	case 0:		dx1 = 0;			dy1 = -step;	break;
	case 1:		dx1 = step*0.707;	dy1 = -dx1;		break;
	case 2:		dx1 = step;			dy1 = 0;		break;
	case 3:		dx1 = step*0.707;	dy1 = dx1;		break;
	case 4:		dx1 = 0;			dy1 = step;		break;
	case 5:		dx1 = -step*0.707;	dy1 = -dx1;		break;
	case 6:		dx1 = -step;		dy1 = 0;		break;
	case 7:		dx1 = -step*0.707;	dy1 = dx1;		break;
	default:	dx1 = 0;			dy1 = 0;
	}
	dx += dx1; // cumulative for fractional  step
	dy += dy1;
	return g_reach_none;
}

// Find where to go next stop by the path selection algorithm
G_NEXTSTOP_RESULT IGWalkAutoAx::FindNextStop(void)
{
	G_NEXTSTOP_RESULT ret;
	int merit = 0;
	GPOINT pos = m_walker->GetPositionFoot();
	RECT rect = m_walker->GetLocation();
	int dist2 = Distance2(pos, m_dest_pos); 
	int delta = MIN(rect.right-rect.left, rect.bottom-rect.top);

	if(dist2 <= delta*delta)// close enough to destination
	{
		m_next_stop = m_dest_pos;
		m_speed = m_default_speed;
		return g_stop_ok;
	}

	m_speed = 0;
	m_next_stop = pos;
	ret = g_stop_error;

	for(int dir=0; dir<8; dir++)
	{
		int merit1;
		GPOINT pos1;
		RECT rect_stop = border_upon(rect, dir);
		// for non-regulate area
		RECT rect_com;
		rect_com.left = MIN(rect_stop.left, rect.left);
		rect_com.right = MAX(rect_stop.right, rect.right);
		rect_com.top = MIN(rect_stop.top, rect.top);
		rect_com.bottom = MAX(rect_stop.bottom, rect.bottom);
		if(m_area->InterSectWithOthers(rect_com, m_walker))
			continue;
		pos1.x = rect_stop.left;
		pos1.y = rect_stop.bottom;
		if(IsFootPrint(pos1))
			continue;
		merit1 = CalculateMerit(pos, m_dest_pos, dir);
		if(merit1 >= merit)
		{
			merit = merit1;
			m_next_stop = pos1;
			m_speed = m_default_speed;
			ret = g_stop_ok;
		}
	}
	return ret;
}

// get the merit of the next point
int IGWalkAutoAx::CalculateMerit(GPOINT pos, GPOINT pos_dest, int dir)
{
	int angle = VectorDirection(pos, pos_dest);
	int merit = ABS(dir - angle);
	if(merit>4)
		merit = 8-merit;

	return 5-merit;
}
