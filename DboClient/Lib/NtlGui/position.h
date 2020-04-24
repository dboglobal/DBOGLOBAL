#ifndef __POSITION_H__
#define __POSITION_H__

class CPos 
{
public:

	int x,y;

public:

	CPos() {x = 0, y = 0;}
	CPos( const int ptX, const int ptY ) {x=ptX,y=ptY;}
	CPos( const POINT& point ) { x=point.x,y=point.y;}


	VOID SetPos(const int nX,const int nY)
	{ x = nX,y = nY; }

	VOID Offset(const int nX,const int nY);

	BOOL operator == (const POINT& point) const;
	BOOL operator == (const CPos& point) const;
	BOOL operator != (const POINT& point)const;
	BOOL operator != (const CPos& point)const;
	VOID operator += (const POINT& point);
	VOID operator += (const CPos& point);
	VOID operator -= (const POINT& point);
	VOID operator -= (const CPos& point);
	const CPos operator + (const POINT& point);
	const CPos operator + (const CPos& point);
	const CPos operator - (const POINT& point);
	const CPos operator - (const CPos& point);
};


#endif
