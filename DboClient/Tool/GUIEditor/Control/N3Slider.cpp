// N3Slider.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "Venus.h"

#include "N3Slider.h"
#include ".\n3slider.h"


// N3SlierFrame

IMPLEMENT_DYNAMIC(N3SlierFrame, CStatic)
N3SlierFrame::N3SlierFrame()
{
}

N3SlierFrame::~N3SlierFrame()
{
}


BEGIN_MESSAGE_MAP(N3SlierFrame, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// N3SlierFrame 메시지 처리기입니다.


void N3SlierFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
	CBrush brush(m_clrColor);
	CRect rc;
	GetClientRect(rc);
	dc.FillRect(rc,&brush);
	brush.DeleteObject();
}

void N3SlierFrame::SetColor(COLORREF color)
{
	m_clrColor = color;
	Invalidate();
}

COLORREF N3SlierFrame::GetColor(void)
{
	return m_clrColor;
}



// N3Slider

#define CX 6 // In fact it's a half of width
#define CY 6 // but it's a real height

IMPLEMENT_DYNAMIC(N3Slider, CWnd)
N3Slider::N3Slider()
{
	m_iPos		= 0;
	m_iMin		= 0;
	m_iMax		= 0;
	m_clrFrom	= 0;
	m_clrTo		= 0;
}

N3Slider::~N3Slider()
{
}


BEGIN_MESSAGE_MAP(N3Slider, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// N3Slider 메시지 처리기입니다.

// D:\WorkNet\Tool\Venus\N3Slider.cpp : 구현 파일입니다.
//


void N3Slider::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	CPaintDC dcPaint(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CBitmapDC dc;
	CRect rc;
	CRect rect;
	CRect rcFrame;
	int x ;
	int y ;

	GetClientRect(rc);
	dc.Create(&dcPaint,rc.Width(),rc.Height(),GetSysColor(COLOR_BTNFACE));
	
	rcFrame.SetRect(rc.left+CX,rc.top,rc.right-CX,rc.bottom-CY);
	dc.Draw3dRect( rcFrame,0x0,0xffffff);
	
	rect.top    = 1;
	rect.left   = CX;
	rect.right  = CX+1;;
	rect.bottom = rc.bottom-(CY+2);

	int iWidth = rc.Width()-CX*2 ;

									// ----> //
	int R1=GetRValue(m_clrFrom);						int R2=GetRValue(m_clrTo);
	int G1=GetGValue(m_clrFrom);						int G2=GetGValue(m_clrTo);
	int B1=GetBValue(m_clrFrom);						int B2=GetBValue(m_clrTo);

	for(int i = 1; i < iWidth; ++i) 
	{
		dc.FillSolidRect(&rect, 
			RGB(
					R2-MulDiv(i, R1-R2, iWidth),
					G2-MulDiv(i, G1-G2, iWidth), 
					B2-MulDiv(i, B1-B2, iWidth)
				)
			);	
		// MulDev(n1,n2,n3) = (n1*n2)/n3
		rect.OffsetRect(1,0);
	}

	x =CX+( (rc.Width()-CX*2)*GetPos())/GetRangeSize();
	y = rc.bottom-(CY+1) ;

	dc.MoveTo(x,y);
	dc.LineTo(x-CX,y+CY);
	dc.LineTo(x+CX,y+CY);
	dc.LineTo(x,y);/*
	

	CRect rect;
	rect.top    = CY;
	rect.left   = CX;
	rect.right  = CX+1;
	rect.bottom = rc.bottom-CY;

	int iWidth = rc.Width()-CX*2 ;

									// ----> //
	int R1=GetRValue(m_clrFrom);						int R2=GetRValue(m_clrTo);
	int G1=GetGValue(m_clrFrom);						int G2=GetGValue(m_clrTo);
	int B1=GetBValue(m_clrFrom);						int B2=GetBValue(m_clrTo);

	for(int i = 1; i < iWidth; ++i) 
	{
		dc.FillSolidRect(&rect, 
			RGB(
					R2-MulDiv(i, R1-R2, iWidth),
					G2-MulDiv(i, G1-G2, iWidth), 
					B2-MulDiv(i, B1-B2, iWidth)
				)
			);	
		// MulDev(n1,n2,n3) = (n1*n2)/n3
		rect.OffsetRect(1,0);
	}


	x = CX+( (rc.Width()-CX*2)*GetPos())/GetRangeSize();
	y = 0 ;

	int OldR2 = dc.SetROP2(R2_NOT);
	dc.MoveTo(x-1,y);
	dc.LineTo(x-1,y+rc.Height() );
	dc.MoveTo(x+1,y);
	dc.LineTo(x+1,y+rc.Height() );
	dc.SetROP2(OldR2);
	dc.Draw3dRect( CRect(rc.left+CX,rc.top+CY,rc.right-CX,rc.bottom-CY),0x0,0xffffff);*/
}

void N3Slider::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( GetCapture() == this )
	{
		CRect rc;
		GetClientRect(rc);
		if( point.x < CX )	point.x = CX;
		if( point.x > rc.Width()-CX ) point.x = rc.Width() - CX;
		point.x -= CX;
		m_iPos = ( GetRangeSize()*point.x ) / (rc.Width()-CX*2);
		GetParent()->SendMessage( WM_HSCROLL);
		Invalidate(0);
	}
	CWnd::OnMouseMove(nFlags, point);
}

void N3Slider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCapture();
	CRect rc;
	GetClientRect(rc);
	if( point.x < CX )	point.x = CX;
	if( point.x > rc.Width()-CX ) point.x = rc.Width() - CX;
	point.x -= CX;
	m_iPos = ( GetRangeSize()*point.x ) / (rc.Width()-(CX*2));

	GetParent()->SendMessage( WM_HSCROLL);
	Invalidate(0);
	CWnd::OnLButtonDown(nFlags, point);
}

void N3Slider::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void N3Slider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	bool b=false;
	if( nChar==37 )		 // Left
	{	m_iPos--;  b=true;	}
	else if( nChar==38 ) // Up
	{	m_iPos++;  b=true;	}
	else if( nChar==39 ) // Right
	{	m_iPos++;  b=true;	}
	else if( nChar==40 ) // Down
	{	m_iPos--;  b=true;	}
		
	if(b)
	{
		GetParent()->SendMessage( WM_HSCROLL);
		Invalidate(0);
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void N3Slider::SetRange(int iMin, int iMax)
{
	m_iMin = min(iMin,iMax);
	m_iMax = max(iMin,iMax);
}
void N3Slider::SetColors(COLORREF From, COLORREF To)
{
	m_clrFrom = From;
	m_clrTo   = To;
	Invalidate(0);
}