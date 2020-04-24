// SizingControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "afxpriv.h"    // for CDockContext
#include "sizecbar.h"
#include "resource.h"


//#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizingControlBar

CSizingControlBar::CSizingControlBar()
{
    m_sizeMin = CSize(220, 470);
	m_sizeMax = CSize(400, 1280);
    m_sizeHorz = CSize(220, 470);
    m_sizeVert = CSize(220, 470);
    m_sizeFloat = CSize(220, 600);
    m_bTracking = FALSE;
    m_bInRecalcNC = FALSE;
    m_cxEdge = 5;
	m_WndClassBrush = NULL;
}

CSizingControlBar::~CSizingControlBar()
{
	if( m_WndClassBrush )
	{
		DeleteObject( m_WndClassBrush );
	}
}

BEGIN_MESSAGE_MAP(CSizingControlBar, CControlBar)
    //{{AFX_MSG_MAP(CSizingControlBar)
	ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_WM_WINDOWPOSCHANGED()
    ON_WM_NCPAINT()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCHITTEST()
    ON_WM_NCCALCSIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_CAPTURECHANGED()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_PAINT()
    ON_WM_SIZE()	
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSizingControlBar message handlers

void CSizingControlBar::OnUpdateCmdUI(class CFrameWnd *pTarget, int bDisableIfNoHndler)
{	
    //UpdateDialogControls(pTarget, bDisableIfNoHndler);
	UpdateDialogControls(pTarget, 0);
}

BOOL CSizingControlBar::Create(CWnd* pParentWnd, CSize sizeDefault, UINT nID, DWORD dwStyle) 
{

	ASSERT_VALID(pParentWnd); // must have a parent 
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC))); 
	// save the style 
	m_dwStyle = dwStyle;

	dwStyle &= ~CBRS_ALL; 

	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE; 

	if( sizeDefault.cy < m_sizeMin.cy ) sizeDefault.cy = m_sizeMin.cy;

	m_sizeHorz = sizeDefault; 
	m_sizeVert = sizeDefault; 
	m_sizeFloat = sizeDefault; 
	
	SetBarStyle(dwStyle & CBRS_ALL);

	m_WndClassBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	CString wndclass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), 
										   m_WndClassBrush, 0); 

	if (!CWnd::Create(wndclass, "Terrain palette", dwStyle , CRect(0,0,0,0), pParentWnd, nID)) 
		return FALSE; 

	this->EnableToolTips();

	return TRUE; 
}

CSize CSizingControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    CRect rc;

    m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_TOP)->GetWindowRect(rc);
    int nHorzDockBarWidth = bStretch ? 32767 : rc.Width() + 4;
    m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT)->GetWindowRect(rc);
    int nVertDockBarHeight = bStretch ? 32767 : rc.Height() + 4;

    if (bHorz)
        return CSize(nHorzDockBarWidth, m_sizeHorz.cy);
    else
        return CSize(m_sizeVert.cx, nVertDockBarHeight);
}

CSize CSizingControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK))
        return CControlBar::CalcDynamicLayout(nLength,dwMode);

    if (dwMode & LM_MRUWIDTH)
        return m_sizeFloat;

    if (dwMode & LM_COMMIT)
    {
        m_sizeFloat.cx = nLength;
        return m_sizeFloat;
    }

    if (dwMode & LM_LENGTHY)
        return CSize(m_sizeFloat.cx,
            m_sizeFloat.cy = max(m_sizeMin.cy, nLength));
    else
        return CSize(max(m_sizeMin.cx, nLength), m_sizeFloat.cy);
}

void CSizingControlBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
    CControlBar::OnWindowPosChanged(lpwndpos);

    if (m_bInRecalcNC) return;

    // Find on which side are we docked
    UINT nDockBarID = GetParent()->GetDlgCtrlID();

    // Return if dropped at same location
    if (nDockBarID == m_nDockBarID // no docking side change
        && (lpwndpos->flags & SWP_NOSIZE) // no size change
        && ((m_dwStyle & CBRS_BORDER_ANY) != CBRS_BORDER_ANY))
        return; 

    m_nDockBarID = nDockBarID;

    // Force recalc the non-client area
    m_bInRecalcNC = TRUE;
    SetWindowPos(NULL, 0, 0, 0, 0,
        SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER |
        SWP_NOACTIVATE | SWP_FRAMECHANGED);
    m_bInRecalcNC = FALSE;	
}

BOOL CSizingControlBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if ((nHitTest != HTSIZE) || m_bTracking)
        return CControlBar::OnSetCursor(pWnd, nHitTest, message);

    if (IsHorz())
        SetCursor(LoadCursor(NULL, IDC_SIZENS));
    else
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void CSizingControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (!m_bTracking)
        CControlBar::OnLButtonUp(nFlags, point);
    else
    {
        ClientToWnd(point);
        StopTracking(TRUE);
    }
}

void CSizingControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
    if (IsFloating() || !m_bTracking)
    {
        CControlBar::OnMouseMove(nFlags, point);
        return;
    }

    CPoint cpt = m_rectTracker.CenterPoint();

    ClientToWnd(point);

    if (IsHorz())
    {
        if (cpt.y != point.y)
        {
            OnInvertTracker(m_rectTracker);
            m_rectTracker.OffsetRect(0, point.y - cpt.y);
            OnInvertTracker(m_rectTracker);
        }
    }
    else 
    {
        if (cpt.x != point.x)
        {
            OnInvertTracker(m_rectTracker);
            m_rectTracker.OffsetRect(point.x - cpt.x, 0);
            OnInvertTracker(m_rectTracker);
        }
    }
}

void CSizingControlBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
    // Compute the rectangle of the mobile edge
    GetWindowRect(m_rectBorder);
    m_rectBorder = CRect(0, 0, m_rectBorder.Width(), m_rectBorder.Height());
    
    DWORD dwBorderStyle = m_dwStyle | CBRS_BORDER_ANY;

    switch(m_nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        dwBorderStyle &= ~CBRS_BORDER_BOTTOM;
        lpncsp->rgrc[0].bottom += -m_cxEdge;
        m_rectBorder.top = m_rectBorder.bottom - m_cxEdge;
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        dwBorderStyle &= ~CBRS_BORDER_TOP;
        lpncsp->rgrc[0].top += m_cxEdge;
        m_rectBorder.bottom = m_rectBorder.top + m_cxEdge;
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        dwBorderStyle &= ~CBRS_BORDER_RIGHT;
        lpncsp->rgrc[0].right += -m_cxEdge;
        m_rectBorder.left = m_rectBorder.right - m_cxEdge;
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        dwBorderStyle &= ~CBRS_BORDER_LEFT;
        lpncsp->rgrc[0].left += m_cxEdge;
        m_rectBorder.right = m_rectBorder.left + m_cxEdge;
        break;
    default:
        m_rectBorder.SetRectEmpty();
        break;
    }

    SetBarStyle(dwBorderStyle);
}

void CSizingControlBar::OnNcPaint() 
{
    EraseNonClient();

    CDC *pDC = GetWindowDC();
    pDC->Draw3dRect(m_rectBorder, GetSysColor(COLOR_BTNHIGHLIGHT),
                  GetSysColor(COLOR_BTNSHADOW));
	
    ReleaseDC(pDC);
}

void CSizingControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
    if (m_bTracking) return;

    if ((nHitTest == HTSIZE) && !IsFloating())
        StartTracking();
    else    
        CControlBar::OnNcLButtonDown(nHitTest, point);
}

LRESULT CSizingControlBar::OnNcHitTest(CPoint point) 
{
    if (IsFloating())
        return CControlBar::OnNcHitTest(point);

    CRect rc;
    GetWindowRect(rc);
    point.Offset(-rc.left, -rc.top);
    if (m_rectBorder.PtInRect(point))
        return HTSIZE;
    else
        return CControlBar::OnNcHitTest(point);
}

void CSizingControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // only start dragging if clicked in "void" space
	

    if (m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
		if( point.y > 0 && point.y < 15 )
		{
			ClientToScreen(&point);
			m_pDockContext->StartDrag(point);
		}
    }
    else
    {
        CWnd::OnLButtonDown(nFlags, point);
    }
/*	
	if (IsFloating()) //is toolbar floating
	{
		CWnd* pMiniFrame;
		CWnd* pDockBar;

		pDockBar = GetParent();
		pMiniFrame = pDockBar->GetParent();

		//get rid of window menu
		pMiniFrame->ModifyStyle(WS_SYSMENU, NULL);

		//extreme way of re-painting the toolbar
		pMiniFrame->ShowWindow(SW_HIDE);
		pMiniFrame->ShowWindow(SW_SHOW);
		//pMiniFrame->ShowControlBar((CControlBar*)this,TRUE,FALSE); 

	}	*/
}

void CSizingControlBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
    // only toggle docking if clicked in "void" space
    if (m_pDockBar != NULL)
    {
        // toggle docking
        ASSERT(m_pDockContext != NULL);		
			
        m_pDockContext->ToggleDocking();		
    }
    else
    {
        CWnd::OnLButtonDblClk(nFlags, point);
    }
	
/*	if (IsFloating()) //is toolbar floating
	{
		CWnd* pMiniFrame;
		CWnd* pDockBar;

		pDockBar = GetParent();
		pMiniFrame = pDockBar->GetParent();

		//get rid of window menu
		pMiniFrame->ModifyStyle(WS_SYSMENU, NULL);

		//extreme way of re-painting the toolbar
		pMiniFrame->ShowWindow(SW_HIDE);
		pMiniFrame->ShowWindow(SW_SHOW);
		//pMiniFrame->ShowControlBar((CControlBar*)this,TRUE,FALSE); 

	}	*/
}

void CSizingControlBar::ToggleDockingPalette( void )
{
	 if (m_pDockBar != NULL)
     {
        // toggle docking
        ASSERT(m_pDockContext != NULL);		
        m_pDockContext->ToggleDocking();
     }
}

void CSizingControlBar::StartTracking()
{
    SetCapture();

    // make sure no updates are pending
    RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
    m_pDockSite->LockWindowUpdate();

    m_ptOld = m_rectBorder.CenterPoint();
    m_bTracking = TRUE;
    
    m_rectTracker = m_rectBorder;
    if (!IsHorz()) m_rectTracker.bottom -= 4;

    OnInvertTracker(m_rectTracker);
}

void CSizingControlBar::OnCaptureChanged(CWnd *pWnd) 
{
    if (m_bTracking && pWnd != this)
        StopTracking(FALSE); // cancel tracking

    CControlBar::OnCaptureChanged(pWnd);
}

void CSizingControlBar::StopTracking(BOOL bAccept)
{
    OnInvertTracker(m_rectTracker);
    m_pDockSite->UnlockWindowUpdate();
    m_bTracking = FALSE;
    ReleaseCapture();
    
    if (!bAccept) return;

    int maxsize, minsize, newsize;
    CRect rcc;
    m_pDockSite->GetClientRect(rcc);

    newsize = IsHorz() ? m_sizeHorz.cy : m_sizeVert.cx;
    maxsize = newsize + (IsHorz() ? rcc.Height() : rcc.Width());
    minsize = IsHorz() ? m_sizeMin.cy : m_sizeMin.cx;

    CPoint point = m_rectTracker.CenterPoint();
    switch (m_nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        newsize += point.y - m_ptOld.y; break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        newsize += -point.y + m_ptOld.y; break;
    case AFX_IDW_DOCKBAR_LEFT:
        newsize += point.x - m_ptOld.x; break;
    case AFX_IDW_DOCKBAR_RIGHT:
        newsize += -point.x + m_ptOld.x; break;
    }

    newsize = max(minsize, min(maxsize, newsize));

    if (IsHorz())
        m_sizeHorz.cy = newsize;
    else
        m_sizeVert.cx = newsize;

    m_pDockSite->DelayRecalcLayout();
}

void CSizingControlBar::OnInvertTracker(const CRect& rect)
{
    ASSERT_VALID(this);
    ASSERT(!rect.IsRectEmpty());
    ASSERT(m_bTracking);

    CRect rct = rect, rcc, rcf;
    GetWindowRect(rcc);
    m_pDockSite->GetWindowRect(rcf);

    rct.OffsetRect(rcc.left - rcf.left, rcc.top - rcf.top);
    rct.DeflateRect(1, 1);

    CDC *pDC = m_pDockSite->GetDCEx(NULL,
        DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);

    CBrush* pBrush = CDC::GetHalftoneBrush();
    HBRUSH hOldBrush = NULL;
    if (pBrush != NULL)
        hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);

    pDC->PatBlt(rct.left, rct.top, rct.Width(), rct.Height(), PATINVERT);

    if (hOldBrush != NULL)
        SelectObject(pDC->m_hDC, hOldBrush);

    m_pDockSite->ReleaseDC(pDC);
}

BOOL CSizingControlBar::IsHorz() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP ||
        m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

CPoint& CSizingControlBar::ClientToWnd(CPoint& point)
{
    if (m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM)
        point.y += m_cxEdge;
    else if (m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT)
        point.x += m_cxEdge;

    return point;
}

void CSizingControlBar::OnPaint()
{	
	CControlBar::OnPaint();
	
	CDC *pDC = this->GetDC();

	RECT rect;
	GetClientRect( &rect );	
/*
	if( !IsFloating() )
	{
		// Drawing Docking dialog title line..
		pDC->Draw3dRect( rect.left+5, rect.top+9, rect.right - 30, rect.top+2,
						 GetSysColor(COLOR_BTNHIGHLIGHT),GetSysColor(COLOR_BTNSHADOW) );

		pDC->Draw3dRect( rect.left+5, rect.top+12, rect.right - 30, rect.top+2,
						 GetSysColor(COLOR_BTNHIGHLIGHT),GetSysColor(COLOR_BTNSHADOW) );					 
		
	}*/


	CRect gripper;
	GetWindowRect( gripper );
	ScreenToClient( gripper );
	gripper.OffsetRect( -gripper.left, -gripper.top );

    // Draw it accordingly to the orientation of the toolbar.
    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    COLORREF clrBtnHilight = ::GetSysColor(COLOR_BTNHILIGHT);
    COLORREF clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
    if( !IsFloating() && (m_dwStyle & CBRS_ORIENT_VERT) )
	{	
		// gripper at top
		gripper.DeflateRect( 2, 2 );
		gripper.bottom = gripper.top+2;
		pDC->Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );
		
		gripper.OffsetRect(0, 2);
        pDC->Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );		
	}	
/*	else
	{
		// gripper at left
		gripper.DeflateRect( 2, 2 );
		gripper.right = gripper.left+2;
        pDC->Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );
		
		gripper.OffsetRect(2, 0);
        pDC->Draw3dRect( gripper, clrBtnHilight, clrBtnShadow );		
	}
*/
	ReleaseDC(pDC);	
}



void CSizingControlBar::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
		
    //lpMMI->ptMinTrackSize = CPoint( m_sizeMin.cx, m_sizeMin.cy );
	//lpMMI->ptMaxTrackSize = CPoint( m_sizeMax.cx, m_sizeMax.cy );

	CControlBar::OnGetMinMaxInfo(lpMMI);
}
