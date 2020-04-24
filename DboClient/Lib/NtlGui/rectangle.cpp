#include "gui_precomp.h"
#include "rectangle.h"

CRectangle::operator LPRECT()
{
	return this;
}

CRectangle::operator LPCRECT() const
{
	return this;
}

int  CRectangle::GetWidth(VOID) const
{
	return abs(right-left);
}


int  CRectangle::GetHeight(VOID) const
{
	return abs(bottom-top);
}


BOOL CRectangle::IsRectEmpty(VOID)
{
	if(GetWidth() == 0 && GetHeight() == 0) 
		return TRUE;
	return FALSE;
}


VOID CRectangle::NormalizeRect(VOID)
{
	int temp;
	if(right < left)
	{
		temp = left;
		left = right;
		right = temp;
	}

	if(bottom < top)
	{
		temp = top;
		top = bottom;
		bottom = temp;
	}
}


BOOL CRectangle::PtInRect(const int x,const int y)
{
	if(x >= left && x < right && y >= top && y < bottom)
		return TRUE;
	return FALSE;
}


BOOL CRectangle::PtInRect(const CPos& ptPos)
{
	if(ptPos.x >= left && ptPos.x < right && ptPos.y >= top && ptPos.y < bottom)
		return TRUE;
	return FALSE;
}

VOID CRectangle::GetCenterPosition( int* pX, int* pY )
{
	*pX = (INT)( (FLOAT)( left + right ) / 2 + 0.5f );
	*pY = (INT)( (FLOAT)( top + bottom ) / 2 + 0.5f );
}

VOID CRectangle::Offset(const int nX,const int nY)
{
	left += nX;
	top += nY;
	right += nX;
	bottom += nY;
}

BOOL CRectangle::Collision(const RECT& rect) const
{
	if(left < rect.left)
		if(rect.left > right) return FALSE;
	else
		if(left > rect.right) return FALSE;

	if(top < rect.top) 
		if(rect.top > top) return FALSE;
	else
		if(top > rect.top) return FALSE;

	return TRUE;

}



VOID CRectangle::operator = (const CRectangle& rect)
{
	SetRect(rect.left,rect.top,rect.right,rect.bottom); 
}


VOID CRectangle::operator = (const RECT& rect)
{
	SetRect(rect.left,rect.top,rect.right,rect.bottom); 
}


BOOL CRectangle::operator == (LPRECT rect) const
{
	if(left == rect->left && top == rect->top &&
		right == rect->right && bottom == rect->bottom)
		return TRUE;
	return FALSE;
}


BOOL CRectangle::operator != (LPRECT rect ) const
{
	if(left == rect->left && top == rect->top &&
		right == rect->right && bottom == rect->bottom)
		return FALSE;
	return TRUE;
}


VOID CRectangle::operator += (LPRECT rect)
{
	left += rect->left;
	top += rect->top; 
	right += rect->right;
	bottom += rect->bottom; 
}


VOID CRectangle::operator += (const CRectangle& rect)
{
	left += rect.left;
	top += rect.top; 
	right += rect.right;
	bottom += rect.bottom; 
}


VOID CRectangle::operator -= (LPRECT rect)
{
	left -= rect->left;
	top -= rect->top; 
	right -= rect->right;
	bottom -= rect->bottom; 
}



const CRectangle& CRectangle::operator + (LPRECT rect)
{
	left += rect->left;
	top += rect->top; 
	right += rect->right;
	bottom += rect->bottom; 

	return *this;

}


const CRectangle& CRectangle::operator - (LPRECT rect)
{
	left -= rect->left;
	top -= rect->top; 
	right -= rect->right;
	bottom -= rect->bottom; 

	return *this;
}



