#include "stdAfx.h"
#include "common.h"
#include "..\wrapper\\wrapper.h"
#include <math.h>

bool InterSection(RECT &rect1, RECT &rect2)
{
	if(rect1.left < rect2.right
		&& rect1.right > rect2.left
		&& rect1.top < rect2.bottom
		&& rect1.bottom > rect2.top)
		return true;
	return false;
}

bool Overrange(RECT &rect1, RECT &rect2)
{
	if(rect1.left < rect2.left
		|| rect1.top < rect2.top
		|| rect1.right > rect2.right
		|| rect1.bottom > rect2.bottom)
		return true;
	return false;
}

short RandomValue(void)
{
//	G_TIME time;
//	GGetCurrentTime(time);
//	return time.second;
	return rand();
}

bool RandomValueBinary(void)
{
	int val = rand();
	return val & 0x01 ? true:false;
}

bool RandomValueBinaryWithProbability(int prob)
{
	int val = rand();
	return (val & 0x0f) > prob ? true:false;
}

int Distance2(RECT &rect1, RECT &rect2)
{
	int s1 = ABSMIN(rect1.left-rect2.left, rect1.left-rect2.right);
	int s2 = ABSMIN(rect1.right-rect2.right, rect1.right-rect2.left);
	int s3 = ABSMIN(s1, s2);
	int t1 = ABSMIN(rect1.top-rect2.top, rect1.top-rect2.bottom);
	int t2 = ABSMIN(rect1.bottom-rect2.bottom, rect1.bottom-rect2.top);
	int t3 = ABSMIN(t1, t2);

	return s3*s3 + t3*t3;
}

int Distance2(GPOINT &pos1, GPOINT &pos2)
{
	int s1 = pos1.x - pos2.x;
	int s2 = pos1.y - pos2.y;
	return s1*s1 + s2*s2;
}

void LimitWithinBoundary(int &x, int &y, RECT &bound)
{
	if(x < bound.left){
		x = bound.left;
	}
	if(y < bound.top){
		y = bound.top;
	}
	if(x > bound.right){
		x = bound.right;
	}
	if(y > bound.bottom){
		y = bound.bottom;
	}
}

int root2(int x)
{
	return (int)sqrtf((float)x);
}

int VectorDirection(GPOINT &pos1, GPOINT &pos2)
{
	int angle;
	int distX, distY, bdX, bdY;

	distX = (pos2.x - pos1.x)<<4;
	distY = (pos2.y - pos1.y)<<4;

	bdX = ABS(distX>>1);
	bdY = ABS(distY>>1);

	if(distX>=-bdY && distX<bdY){
		if(distY<0)
			angle = 0;
		else
			angle = 4;
	}else if(distY>=-bdX && distY<bdX){
		if(distX<0)
			angle = 6;
		else
			angle = 2;
	}else if(distX>0 && distY<0)
		angle = 1;
	else if(distX>0 && distY>0)
		angle = 3;
	else if(distX<0 && distY>0)
		angle = 5;
	else
		angle = 7;
	return angle;
}

RECT border_upon(RECT &rect, int dir)
{
	RECT rect1 = rect;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int delta = MIN(width, height);

	switch(dir)
	{
	case 0:
		rect1.top -= delta;
		rect1.bottom -= delta;
		break;
	case 1:
		rect1.left += delta;
		rect1.right += delta;
		rect1.top -= delta;
		rect1.bottom -= delta;
		break;
	case 2:
		rect1.left += delta;
		rect1.right += delta;
		break;
	case 3:
		rect1.left += delta;
		rect1.right += delta;
		rect1.top += delta;
		rect1.bottom += delta;
		break;
	case 4:
		rect1.top += delta;
		rect1.bottom += delta;
		break;
	case 5:
		rect1.left -= delta;
		rect1.right -= delta;
		rect1.top += delta;
		rect1.bottom += delta;
		break;
	case 6:
		rect1.left -= delta;
		rect1.right -= delta;
		break;
	case 7:
		rect1.left -= delta;
		rect1.right -= delta;
		rect1.top -= delta;
		rect1.bottom -= delta;
		break;
	default:
		;
	}
	return rect1;
}
