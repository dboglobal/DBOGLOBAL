// PropTreeItemSlider.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PropTreeItemSlider.h"

#include "proptree.h"


// CPropTreeItemSlider

#define CX 6 // In fact it's a half of width
#define CY 6 // but it's a real height

IMPLEMENT_DYNAMIC(CPropTreeItemSlider, CWnd)
CPropTreeItemSlider::CPropTreeItemSlider()
{
	m_FloatingPointMode = FALSE;
	m_FloatingData = 0;

	m_ValueString = _T("0");	

	m_iPos=0;
	m_iMin=0;
	m_iMax=255;
	m_clrFrom=0x00000000;
	m_clrTo=0xFFFFFFFF;

	m_ControlType = _XPTI_ControlType_Slider;
}

CPropTreeItemSlider::~CPropTreeItemSlider()
{
}


BEGIN_MESSAGE_MAP(CPropTreeItemSlider, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CLOSE()
END_MESSAGE_MAP()



// CPropTreeItemSlider 메시지 처리기입니다.


void CPropTreeItemSlider::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
//	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.
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
	rect.right  = CX+1;
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

	if( GetRangeSize() )
		x =CX+( (rc.Width()-CX*2)*GetPos())/GetRangeSize();
	else
		x = 0;

	y = rc.bottom-(CY+1) ;

	dc.MoveTo(x,y);
	dc.LineTo(x-CX,y+CY);
	dc.LineTo(x+CX,y+CY);
	dc.LineTo(x,y);

	dc.SelectObject(m_pProp->GetNormalFont());
	dc.SetTextColor(RGB(255,255,64));
	dc.SetBkMode(TRANSPARENT);	
	CRect r = rc;	
	r.bottom-=7;
	dc.DrawText(m_ValueString, r, DT_SINGLELINE|DT_VCENTER|DT_CENTER);	}

void CPropTreeItemSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( nFlags & MK_LBUTTON )
	{
		CRect rc;
		GetClientRect(rc);
		if( point.x < CX )	point.x = CX;
		if( point.x > rc.Width()-CX ) point.x = rc.Width() - CX;
		point.x -= CX;
		m_iPos = ( GetRangeSize()*point.x ) / (rc.Width()-CX*2);
		m_pProp->GetCtrlParent()->SendMessage( WM_HSCROLL);
	
		if( m_FloatingPointMode )
			m_ValueString.Format(_T("%4.4f"),((FLOAT)GetValue() / 1000.0f) );
		else
			m_ValueString.Format(_T("%d"), GetValue() );

		Invalidate(0);		
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CPropTreeItemSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCapture();

	CRect rc; 
	GetClientRect(rc);
	if( point.x < CX )	point.x = CX;
	if( point.x > rc.Width()-CX ) point.x = rc.Width() - CX;
	point.x -= CX;
	m_iPos = ( GetRangeSize()*point.x ) / (rc.Width()-(CX*2));

	m_pProp->GetCtrlParent()->SendMessage( WM_HSCROLL);
	Invalidate(0);	

	CWnd::OnLButtonDown(nFlags, point);
}

void CPropTreeItemSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);

	CommitChanges();

//	CWnd::OnLButtonUp(nFlags, point);
}

void CPropTreeItemSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
		m_pProp->GetCtrlParent()->SendMessage( WM_HSCROLL);
		Invalidate(0);
	}

	if (nChar==VK_ESCAPE) CommitChanges();
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPropTreeItemSlider::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CommitChanges();
}

void CPropTreeItemSlider::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CommitChanges();
//	CWnd::OnClose();
}

void CPropTreeItemSlider::DrawAttribute(CDC* pDC, const RECT& rc)
{
	ASSERT(m_pProp!=NULL);
	
	/*
	pDC->SelectObject(IsReadOnly() ? m_pProp->GetNormalFont() : m_pProp->GetBoldFont());
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	CRect r = rc;
	
	pDC->DrawText(m_ValueString, r, DT_SINGLELINE|DT_VCENTER);
	*/
		
	CRect crc(rc);
	CRect rect;
	CRect rcFrame;
	int x ;
	int y ;
	
	pDC->FillSolidRect(&crc, GetSysColor(COLOR_BTNFACE) );	
			
	rect.top    = 1;
	rect.left   = crc.left+CX;
	rect.right  = crc.left+CX+1;
	rect.bottom = crc.bottom-(CY+2);
	
	int iWidth = crc.Width()-CX*2 ;
	
	// ----> //
	int R1=GetRValue(m_clrFrom);						
	int R2=GetRValue(m_clrTo);
	int G1=GetGValue(m_clrFrom);						
	int G2=GetGValue(m_clrTo);
	int B1=GetBValue(m_clrFrom);						
	int B2=GetBValue(m_clrTo);
	
	for(int i = 1; i < iWidth; ++i) 
	{
		pDC->FillSolidRect(&rect, 
			RGB(
			R2-MulDiv(i, R1-R2, iWidth),
			G2-MulDiv(i, G1-G2, iWidth), 
			B2-MulDiv(i, B1-B2, iWidth)
			)
			);			
		rect.OffsetRect(1,0);
	}

	rcFrame.SetRect(crc.left+CX,crc.top,crc.right-CX,crc.bottom-CY);
	pDC->Draw3dRect( rcFrame,0x0,0xffffff);
	
	if( GetRangeSize() )
		x = crc.left+CX+( (crc.Width()-CX*2)*GetPos())/GetRangeSize();
	else
		x = crc.left;
	
	y = crc.top+(crc.bottom-crc.top)-(CY+1) ;

	CPen pn1(PS_SOLID, 1,RGB(0,0,0) );
	CPen* pOld;	
	pOld = pDC->SelectObject(&pn1);
	
	pDC->MoveTo(x,y);
	pDC->LineTo(x-CX,y+CY);
	pDC->LineTo(x+CX,y+CY);
	pDC->LineTo(x,y);
	
	pDC->SelectObject(pOld);

	pDC->SelectObject(m_pProp->GetNormalFont());
	pDC->SetTextColor(RGB(255,255,64));
	pDC->SetBkMode(TRANSPARENT);
	CRect r = crc;	
	r.bottom-=7;	
	
	pDC->DrawText(m_ValueString, r, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}


LPARAM CPropTreeItemSlider::GetItemValue()
{
	if( m_FloatingPointMode )	
	{
		 m_FloatingData = ((FLOAT)(m_iPos + m_iMin) / 1000.0f);
		 return (LPARAM)&m_FloatingData;
	}
	else return (LPARAM)(m_iPos + m_iMin);
}


void CPropTreeItemSlider::SetItemValue(LPARAM lParam)
{
	int value;
	if( m_FloatingPointMode )
	{	
		value = (int)((*(FLOAT*)lParam) * 1000.0f);
		m_ValueString.Format(_T("%4.4f"),*(FLOAT*)lParam);
	}
	else
	{
		value = (int)lParam;
		m_ValueString.Format(_T("%d"),value);
	}

	SetPos( value );	
}


void CPropTreeItemSlider::OnMove()
{
	if (IsWindow(m_hWnd))
		SetWindowPos(NULL, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
}


void CPropTreeItemSlider::OnRefresh()
{
	if (IsWindow(m_hWnd))
	{		
		//SetPos( m_Value );
	}
}


void CPropTreeItemSlider::OnCommit()
{
	POINT pt;
	CRect rc;
	GetCursorPos(  &pt );		
	GetWindowRect( &rc );
	
	
	//if( !rc.PtInRect(pt) )
	{
		ShowWindow(SW_HIDE);
	}
}


void CPropTreeItemSlider::OnActivate()
{
	if (!IsWindow(m_hWnd))
	{			
		LPCTSTR pszClassName;
		
		pszClassName = AfxRegisterWndClass(CS_VREDRAW|CS_HREDRAW, LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_BTNFACE + 1));
		
		DWORD dwStyle = WS_CHILD;
		
		CreateEx(0, pszClassName, _T(""), dwStyle, m_rc, m_pProp->GetCtrlParent(), 0);

		SendMessage(WM_SETFONT, (WPARAM)m_pProp->GetNormalFont()->m_hObject);
	}	
		
	SetWindowPos(NULL, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	SetFocus();
}


void CPropTreeItemSlider::SetRange(FLOAT fMin, FLOAT fMax)
{
	if( m_FloatingPointMode )
	{
		m_iMin = (int)(min(fMin,fMax) * 1000.0f);
		m_iMax = (int)(max(fMin,fMax) * 1000.0f);
	}
	else
	{
		m_iMin = (int)min(fMin,fMax);
		m_iMax = (int)max(fMin,fMax);
	}
}

void CPropTreeItemSlider::SetRange(int iMin, int iMax)
{
	if( m_FloatingPointMode )
	{
		m_iMin = (int)(min(iMin,iMax) * 1000.0f);
		m_iMax = (int)(max(iMin,iMax) * 1000.0f);
	}
	else
	{
		m_iMin = min(iMin,iMax);
		m_iMax = max(iMin,iMax);
	}
}


void CPropTreeItemSlider::SetColors(COLORREF From, COLORREF To)
{
	m_clrFrom = From;
	m_clrTo   = To;
//	Invalidate(0);
}


void CPropTreeItemSlider::SetPos(int nPos)
{
	if( nPos>= m_iMin && nPos<=m_iMax )
	{
		m_iPos = nPos;
		if (IsWindow(m_hWnd))
			Invalidate(FALSE);
	}
}

