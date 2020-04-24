#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "position.h"

class CRectangle : public RECT
{
	//int left, top, right, bottom;

public:

	CRectangle () {left=0, top=0, right=0, bottom=0;}
	CRectangle( const int l, const int t, const int r, const int b )
	{ left=l,top=t,right=r,bottom=b; }
	CRectangle(LPRECT rect)
	{ left=rect->left,top=rect->top,right=rect->right,bottom=rect->bottom; }
	CRectangle(const CPos& LeftTop,const CPos& RightBottom)
	{ left = LeftTop.x,top = LeftTop.y,right = RightBottom.x,bottom = RightBottom.y;}
	
	VOID SetRect(const int l,const int t,const int r,const int b)
	{ left = l,top = t,right = r,bottom = b; }
	VOID SetRectWH( const int l, const int t, const int w, const int h )
	{ left = l; top = t; right = left + w; bottom = top + h; }
	VOID SetCenterRect( const int x, const int y, const int w, const int h )
	{ left = x - w / 2; top = y - h / 2; right = left + w; bottom = top + h; }
	
	int  GetWidth(VOID) const;
	int  GetHeight(VOID) const;
	BOOL IsRectEmpty(VOID);
	VOID NormalizeRect(VOID);
	BOOL PtInRect(const int nX,const int nY);
	BOOL PtInRect(const CPos& ptPos);
	VOID GetCenterPosition( int* pX, int* pY );

	// left+=nX,top+=nY,right+=nX,bottom+=nY
	VOID Offset(const int nX,const int nY);

	BOOL Collision(const RECT& rect) const;

	operator LPRECT();
	operator LPCRECT() const;
		
	VOID operator = (const RECT& rect); 
	VOID operator = (const CRectangle& rect);
	BOOL operator == (LPRECT rect) const;
	BOOL operator != (LPRECT rect )const;
	VOID operator += (LPRECT rect);
	VOID operator += (const CRectangle& rect);
	VOID operator -= (LPRECT rect);
	const CRectangle& operator + (LPRECT rect);
	const CRectangle& operator - (LPRECT rect);
};


#endif
