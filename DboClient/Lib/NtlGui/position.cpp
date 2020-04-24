#include "gui_precomp.h"
#include "position.h"

VOID CPos::Offset(const int nX,const int nY)
{
	x += nX;
	y += nY;
}

BOOL CPos::operator == (const POINT& point) const
{
	if(x == point.x && y == point.y) return TRUE;
	return FALSE;
}

BOOL CPos::operator == (const CPos& point) const
{
	if(x == point.x && y == point.y) return TRUE;
	return FALSE;
}

BOOL CPos::operator != (const POINT& point)const
{
	if(x == point.x && y == point.y) return FALSE;
		return TRUE;
}

BOOL CPos::operator != (const CPos& point)const
{
	if(x == point.x && y == point.y) return FALSE;
		return TRUE;
}

VOID CPos::operator += (const POINT& point)
{
	x+=point.x;
	y+=point.y;
}

VOID CPos::operator += (const CPos& point)
{
	x+=point.x;
	y+=point.y;
}


VOID CPos::operator -= (const POINT& point)
{
	x-=point.x;
	y-=point.y;
}


VOID CPos::operator -= (const CPos& point)
{
	x-=point.x;
	y-=point.y;
}



const CPos CPos::operator + (const POINT& point)
{
	CPos ptTemp;
	ptTemp.SetPos(x+point.x,y+point.y);
	return ptTemp;
}


const CPos CPos::operator + (const CPos& point)
{
	CPos ptTemp;
	ptTemp.SetPos(x+point.x,y+point.y);
	return ptTemp;
}

const CPos CPos::operator - (const POINT& point)
{
	CPos ptTemp;
	ptTemp.SetPos(x-point.x,y-point.y);
	return ptTemp;
}


const CPos CPos::operator - (const CPos& point)
{
	CPos ptTemp;
	ptTemp.SetPos(x-point.x,y-point.y);
	return ptTemp;
}

