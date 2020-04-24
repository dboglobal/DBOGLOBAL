// ExSliderCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ExSliderCtrl.h"

//-- 포인트 상세 설정시 사용할 다이얼로그


// 메뉴 처리를 위해 사용되는 아이디
#define ID_SET_RANGE	31244
#define ID_SET_START	31245
#define ID_SET_END		31246

// CExSliderCtrl

IMPLEMENT_DYNAMIC(CExSliderCtrl, CSliderCtrl)
CExSliderCtrl::CExSliderCtrl()
{
	m_nBarHeight = 12;
	m_nTickWidth = 10;
	m_nPntWidth	 = 10;
	m_nPntHeight = 12;
    m_nClickMarkerID = -1;

	m_bClickStart	= FALSE;
	m_bClickEnd		= FALSE;
	m_bClickTick	= FALSE;
    m_bClickMarker  = FALSE;

	m_nStartPnt	= 0;
	m_nEndPnt	= 0;

	m_bSetRange = FALSE;
    m_vMarker.clear();
}

CExSliderCtrl::~CExSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CExSliderCtrl, CSliderCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()

	ON_NOTIFY(TTN_NEEDTEXT, NULL, OnNeedText)	
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CExSliderCtrl 메시지 처리기입니다.

void CExSliderCtrl::SetRange(int nMin, int nMax, BOOL bRedraw /* = FALSE */)
{
	CSliderCtrl::SetRange(nMin, nMax, bRedraw);

	m_nStartPnt	= nMin;
	m_nEndPnt	= nMax;

	m_bSetRange = TRUE;
}

void CExSliderCtrl::SetRangeMin(int nMin, BOOL bRedraw /* = FALSE */)
{
	CSliderCtrl::SetRangeMin(nMin, bRedraw);
	m_nStartPnt	= nMin;

	m_bSetRange = TRUE;
}

void CExSliderCtrl::SetRangeMax(int nMax, BOOL bRedraw /* = FALSE */)
{
	CSliderCtrl::SetRangeMax(nMax, bRedraw);
	m_nEndPnt	= nMax;

	m_bSetRange = TRUE;
}

int CExSliderCtrl::PosToPixel(CRect rtBar, int nPos)
{
	int nMax = 0;
	int nMin = 0;
	GetRange(nMin, nMax);

	return (int)(rtBar.left + ((float)(rtBar.Width()) * ((float)nPos / (float)(nMax - nMin))));
}

int CExSliderCtrl::PixelToPos(CRect /*rtBar*/, int nPixel)
{
	int nMax = 0;
	int nMin = 0;
	GetRange(nMin, nMax);

	int nPos = 0;

	if(nPixel < m_rtBar.left)
		return nMin;

	if(nPixel > m_rtBar.right)
		return nMax;

	nPos = (int)(nMin + ((float)(nMax - nMin) * ((float)(nPixel - m_rtBar.left) / (float)m_rtBar.Width())));

	//TRACE("POS = %d\n", nPos);

	return nPos;
}

void CExSliderCtrl::CalcLayout()
{
	GetClientRect(&m_rtWnd);

	int nMax = 0;
	int nMin = 0;
	GetRange(nMin, nMax);

	int nPos = GetPos();

	// Bar 영역 구하기
	m_rtBar.left	= m_rtWnd.left + 7;
	m_rtBar.right	= m_rtWnd.right - 8;
	m_rtBar.top		= m_rtWnd.bottom - m_nBarHeight - 3;
	m_rtBar.bottom	= m_rtWnd.bottom - 3;

    // 마커 영역 구하기
    m_rtMarkerSpace.left = m_rtWnd.left + 7;
    m_rtMarkerSpace.right = m_rtWnd.right - 8;
    m_rtMarkerSpace.top = m_rtWnd.top + 3;
    m_rtMarkerSpace.bottom = m_rtBar.top;


	// Tick 영역 구하기
	m_rtTick.left	= PosToPixel(m_rtBar, nPos) - (m_nTickWidth / 2);
	m_rtTick.right	= m_rtTick.left + m_nTickWidth;
	m_rtTick.top	= m_rtBar.top - 2;
	m_rtTick.bottom	= m_rtBar.bottom + 2;

    // 마커들의 영역 구하기
    for(size_t i = 0; i < m_vMarker.size(); ++i)
    {
        m_vMarker[i].m_rt.left   = PosToPixel(m_rtBar, m_vMarker[i].m_nPoint) - (m_nPntWidth / 2);
        m_vMarker[i].m_rt.right  = m_vMarker[i].m_rt.left + m_nPntWidth;
        m_vMarker[i].m_rt.top    = m_rtBar.top - m_nPntHeight - 5;
        m_vMarker[i].m_rt.bottom = m_rtBar.top - 5;
    }


	//// 시작 점 영역 구하기
	//m_rtStart.left		= PosToPixel(m_rtBar, m_nStartPnt) - (m_nPntWidth / 2);
	//m_rtStart.right		= m_rtStart.left + m_nPntWidth;
	//m_rtStart.top		= m_rtBar.top - m_nPntHeight - 5;
	//m_rtStart.bottom	= m_rtBar.top - 5;

	//// 끝 점 영역 구하기
	//m_rtEnd.left	= PosToPixel(m_rtBar, m_nEndPnt) - (m_nPntWidth / 2);
	//m_rtEnd.right	= m_rtEnd.left + m_nPntWidth;
	//m_rtEnd.top		= m_rtBar.top - m_nPntHeight - 5;
	//m_rtEnd.bottom	= m_rtBar.top - 5;
}

void CExSliderCtrl::OnPaint()
{
	CPaintDC dc(this);

	// 더블 버퍼링 객체를 생성한다
	CGDIPlusDBuffer dbGraphics(this->GetSafeHwnd());
	// 더블 버퍼링 객체에서 Graphics 포인터를 얻는다
	Graphics* pGrp = dbGraphics.GetGraphics();

	CalcLayout();

	//---------------------------------------------------------
	// 전체 배경 그리기  
	//---------------------------------------------------------
	{
		LinearGradientBrush linerUp(Point(m_rtWnd.left, m_rtWnd.top - 1),
			Point(m_rtWnd.left, m_rtWnd.bottom - m_rtTick.Height()), 
			Color(255, 230, 238, 248), 
			Color(255, 184, 203, 230));
		pGrp->FillRectangle(&linerUp,
			m_rtWnd.left,
			m_rtWnd.top,
			m_rtWnd.Width(),
			m_rtWnd.bottom - m_rtTick.Height());

		LinearGradientBrush linerDown(Point(m_rtWnd.left, m_rtWnd.bottom - m_rtTick.Height() - 1),
			Point(m_rtWnd.left, m_rtWnd.bottom), 
			Color(255, 131, 166, 212),
			Color(255, 207, 220, 240));
		pGrp->FillRectangle(&linerDown,
			m_rtWnd.left,
			m_rtWnd.bottom - m_rtTick.Height() - 1,
			m_rtWnd.Width(),
			m_rtTick.Height());

		Pen penTickOut(Color(255, 183, 201, 230));
		pGrp->DrawRectangle(&penTickOut,
			m_rtWnd.left,
			m_rtWnd.top,
			m_rtWnd.Width() - 1,
			m_rtWnd.Height() - 1);
	}		

	//---------------------------------------------------------
	// 현재 진행된 영역 그리기: 초록색으로,,,  
	//---------------------------------------------------------
	{
		int nWidth = PosToPixel(m_rtBar, m_nStartPnt);

		CRect rt;
		rt.left		= nWidth + 1;
		rt.right	= m_rtTick.left - 1;
		rt.top		= m_rtBar.top + 2;
		rt.bottom	= m_rtBar.bottom - 2;

		// 와곽선 그리기
		Pen penOut(Color(255, 29, 168, 0));
		pGrp->DrawRectangle(&penOut, rt.left, rt.top, rt.Width(), rt.Height());

		// 안쪽 채우기
		LinearGradientBrush linerBr(Point(rt.left, rt.bottom),
			Point(rt.left, rt.top), 
			Color(255, 68, 184, 0),
			Color(255, 135, 210, 0));
		pGrp->FillRectangle(&linerBr, rt.left + 1, rt.top + 1, rt.Width() - 1, rt.Height() - 1);
	}

    //---------------------------------------------------------
    // 마커들 그리기
    //---------------------------------------------------------
    for(size_t i = 0; i < m_vMarker.size(); ++i)
    {
        Point pntLine[3] = {Point(m_vMarker[i].m_rt.left, m_vMarker[i].m_rt.top),
                            Point(m_vMarker[i].m_rt.right, m_vMarker[i].m_rt.top),
                            Point(m_vMarker[i].m_rt.left + m_vMarker[i].m_rt.Width() / 2, m_vMarker[i].m_rt.bottom)};

        if(i == m_nClickMarkerID) // 선택된 마커는 다르게 그려준다.
        {
            // 안쪽 칠하기
            SolidBrush brOut(Color(255, 50, 50, 255));
            pGrp->FillPolygon(&brOut, pntLine, 3);

            // 외곽선 그리기
            Pen penOut(Color(255, 128, 128, 255));
            pGrp->DrawPolygon(&penOut, pntLine, 3);
        }
        else
        {
            // 안쪽 칠하기
            SolidBrush brOut(Color(255, 255, 50, 50));
            pGrp->FillPolygon(&brOut, pntLine, 3);

            // 외곽선 그리기
            Pen penOut(Color(255, 255, 128, 128));
            pGrp->DrawPolygon(&penOut, pntLine, 3);
        }        
    }
	////---------------------------------------------------------
	//// 시작 점 그리기  
	////---------------------------------------------------------
	//{
	//	Point pntLine[3] = {Point(m_rtStart.left, m_rtStart.top), 
	//		Point(m_rtStart.right, m_rtStart.top),
	//		Point(m_rtStart.left + m_rtStart.Width() / 2, m_rtStart.bottom)};

	//	// 안쪽 칠하기
	//	SolidBrush brOut(Color(255, 255, 50, 50));
	//	pGrp->FillPolygon(&brOut, pntLine, 3);

	//	// 외곽선 그리기
	//	Pen penOut(Color(255, 255, 128, 128));
	//	pGrp->DrawPolygon(&penOut, pntLine, 3);	
	//}

 //   //---------------------------------------------------------
 //   // 종료 점 그리기  
 //   //---------------------------------------------------------
 //   {
 //       Point pntLine[3] = {Point(m_rtEnd.left, m_rtEnd.top), 
 //           Point(m_rtEnd.right, m_rtEnd.top),
 //           Point(m_rtEnd.left + m_rtEnd.Width() / 2, m_rtEnd.bottom)};

 //       // 안쪽 칠하기
 //       SolidBrush brOut(Color(255, 255, 50, 50));
 //       pGrp->FillPolygon(&brOut, pntLine, 3);

 //       // 외곽선 그리기
 //       Pen penOut(Color(255, 255, 128, 128));
 //       pGrp->DrawPolygon(&penOut, pntLine, 3);	
 //   }


	////---------------------------------------------------------
	//// Head 쪽 비활성 영역 그리기  
	////---------------------------------------------------------
	//{
	//	int nWidth = PosToPixel(m_rtBar, m_nStartPnt);

	//	CRect rt;
	//	rt.left		= m_rtBar.left + 1;
	//	rt.right	= nWidth - 1;
	//	rt.top		= m_rtBar.top + 2;
	//	rt.bottom	= m_rtBar.bottom - 2;

	//	// 외곽선 그리기
	//	Pen penOut(Color(255, 200, 200, 200));
	//	pGrp->DrawRectangle(&penOut, rt.left, rt.top, rt.Width(), rt.Height());

	//	// 안쪽 채우기
	//	LinearGradientBrush linerBr(Point(rt.left, rt.bottom),
	//		Point(rt.left, rt.top), 
	//		Color(255, 100, 100, 100),
	//		Color(255, 130, 130, 130));
	//	pGrp->FillRectangle(&linerBr, rt.left + 1, rt.top + 1, rt.Width() - 1, rt.Height() - 1);
	//}



	////---------------------------------------------------------
	//// Tail 쪽 비활성 영역 그리기  
	////---------------------------------------------------------
	//{
	//	int nLeft = PosToPixel(m_rtBar, m_nEndPnt);

	//	CRect rt;
	//	rt.left		= nLeft;
	//	rt.right	= m_rtBar.right - 1;
	//	rt.top		= m_rtBar.top + 2;
	//	rt.bottom	= m_rtBar.bottom - 2;

	//	// 외곽선 그리기
	//	Pen penOut(Color(255, 200, 200, 200));
	//	pGrp->DrawRectangle(&penOut, rt.left, rt.top, rt.Width(), rt.Height());

	//	// 안쪽 채우기
	//	LinearGradientBrush linerBr(Point(rt.left, rt.bottom),
	//		Point(rt.left, rt.top), 
	//		Color(255, 100, 100, 100),
	//		Color(255, 130, 130, 130));
	//	pGrp->FillRectangle(&linerBr, rt.left + 1, rt.top + 1, rt.Width() - 1, rt.Height() - 1);
	//}

	//---------------------------------------------------------
	// Tick 그리기. 가장 마지막에 그려져야 한다.
	//---------------------------------------------------------
	{
		// Tick 위쪽 칠하기
		LinearGradientBrush linerUp(Point(m_rtTick.left, m_rtTick.top - 1),
			Point(m_rtTick.left, m_rtTick.top + m_rtTick.Height() / 2), 
			Color(255, 246, 248, 252), 
			Color(255, 172, 184, 225));
		pGrp->FillRectangle(&linerUp,
			m_rtTick.left,
			m_rtTick.top,
			m_rtTick.Width(),
			m_rtTick.Height() / 2);

		// Tick 아래쪽 칠하기
		LinearGradientBrush linerDown(Point(m_rtTick.left, m_rtTick.top + m_rtTick.Height() / 2),
			Point(m_rtTick.left, m_rtTick.bottom), 
			Color(255, 172, 184, 225),
			Color(255, 246, 248, 252));
		pGrp->FillRectangle(&linerDown,
			m_rtTick.left,
			m_rtTick.top + m_rtTick.Height() / 2,
			m_rtTick.Width(),
			m_rtTick.Height() / 2);

		// Tick 외곽선 그리기
		Pen penTickOut(Color(255, 41, 92, 164));
		pGrp->DrawRectangle(&penTickOut,
			m_rtTick.left,
			m_rtTick.top,
			m_rtTick.Width(),
			m_rtTick.Height());
	}
}


void CExSliderCtrl::OnMouseMove(UINT /*nFlags*/, CPoint point)
{	
    if(m_bClickMarker && m_nClickMarkerID != -1)
    {
        int nPos = PixelToPos(m_rtBar, point.x);
        int nMin = 0, nMax = 0;
        GetRange(nMin, nMax);
        if(nPos < nMin)
        {
            nPos = nMin;
        }
        else if(nPos > nMax)
        {
            nPos = nMax;
        }

        m_vMarker[m_nClickMarkerID].m_nPoint = nPos;
        ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_MARKER_POS_CHANGED, NULL, NULL);
    }

	//if(m_bClickStart)
	//{
	//	int nPos = PixelToPos(m_rtBar, point.x);

	//	// Start는 End를 넘어갈 수 없다.
	//	if(nPos >= m_nEndPnt)
	//		nPos = m_nEndPnt - 1;

	//	// Start가 현재 Tick Pos보다 커지면 Tick도 같이 이동한다.
	//	if(nPos > GetPos())
	//		SetPos(nPos);

	//	m_nStartPnt = nPos;		
	//}
	//else if(m_bClickEnd)
	//{
	//	int nPos = PixelToPos(m_rtBar, point.x);

	//	// End는 Start를 넘어갈 수 없다.
	//	if(nPos <= m_nStartPnt)
	//		nPos = m_nStartPnt + 1;

	//	// End가 현재 Tick Pos보다 작아지면 Tick도 같이 이동한다.
	//	if(nPos < GetPos())
	//		SetPos(nPos);

	//	m_nEndPnt = nPos;
	//}

	else if(m_bClickTick)
	{
		int nPos = PixelToPos(m_rtTick, point.x);

		if(nPos == 100)
			TRACE("Pos = %d\n", nPos);

		// Pos 값은 Start Point ~ End Point 사이에만 존재해야 한다.
		if(nPos < m_nStartPnt)
			nPos = m_nStartPnt;

		if(nPos > m_nEndPnt)
			nPos = m_nEndPnt;

		SetPos(nPos);

        ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_POS_CHANGED, NULL, NULL);
	}
	else
		return;

	Invalidate();	

	
}

BOOL CExSliderCtrl::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
{
	int nPos = GetPos();

	//---------------------------------------------------------
	// zDelta: 휠을 위로 올리면 -120, 아래로 내리면 120이다.  
	//---------------------------------------------------------
	if(zDelta < 0)
	{
		++nPos;
		if(nPos > m_nEndPnt)
			nPos = m_nEndPnt;
	}
	else
	{
		--nPos;
		if(nPos < m_nStartPnt)
			nPos = m_nStartPnt;
	}

	SetPos(nPos);

	Invalidate();
	::SendMessage(GetParent()->GetSafeHwnd(), EXSM_POS_CHANGED, NULL, NULL);

	return TRUE;
}

void CExSliderCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	SetCapture();	// 마우스 이벤트 처리 위해
	SetFocus();		// 마우스 휠 처리를 위해	

	m_ptClick = point;

    m_bClickMarker = FALSE;

    // 마커를 클릭했을때 마커를 선택한다.
    for(size_t i = 0; i < m_vMarker.size(); ++i)
    {
        if(m_vMarker[i].m_rt.PtInRect(point))
        {
            m_bClickMarker = TRUE;
            m_nClickMarkerID = (int)i;
            ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_MARKER_SELECT, (int)i, NULL);            
            break;
        }
    }

	//if(m_rtStart.PtInRect(point))
	//{
	//	m_bClickStart = TRUE;
	//}
	//else if(m_rtEnd.PtInRect(point))
	//{
	//	m_bClickEnd = TRUE;
	//}

    if(!m_bClickMarker)
    {
	    if(m_rtTick.PtInRect(point))
	    {
		    m_bClickTick = TRUE;
	    }
	    else if(m_rtBar.PtInRect(point))
	    {
		    //---------------------------------------------------------
		    // Bar의 빈 부분을 클릭했을 경우, 그곳이 이동 가능한 곳이면
		    // 바로 이동을 시키고, 그렇지 않고 이동할 수 없는 영역이면
		    // 왕 무시한다.
		    //---------------------------------------------------------
		    int nWhere = PixelToPos(m_rtBar, point.x);
		    if(nWhere >= m_nStartPnt && nWhere <= m_nEndPnt)
		    {
			    m_bClickTick = TRUE;

			    SetPos(nWhere);
			    Invalidate();
			    ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_POS_CHANGED, NULL, NULL);
		    }
	    }
        else if(m_rtMarkerSpace.PtInRect(point))
        {
            // 마커 영역 부분을 클릭했을때 그자리에 마커를 추가한다.
            int nPos = PixelToPos(m_rtMarkerSpace, point.x);
            if(nPos >= m_nStartPnt && nPos <= m_nEndPnt)
            {
                AddMarker(nPos);
                Invalidate();                
            }
        }
    }
}

void CExSliderCtrl::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{	
	ReleaseCapture();

    if(m_bClickMarker)
    {
        ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_MARKER_POS_CHANGE_END, NULL, NULL);
    }

    m_bClickMarker = FALSE;    
	m_bClickStart = FALSE;	
	m_bClickEnd = FALSE;
	m_bClickTick = FALSE;
}

void CExSliderCtrl::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	CalcLayout();

	if(!m_bSetRange)
		return;

	//if(m_rtStart.PtInRect(point))
	//{
	//	//OnSetStart();
	//}
	//else if(m_rtEnd.PtInRect(point))
	//{
	//	//OnSetEnd();
	//}
}

void CExSliderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    //PopupMenu(point);

    // 우측 버튼을 클릭하면 마커를 지운다.
    VMarker::iterator it = m_vMarker.begin();
    for(; it != m_vMarker.end(); ++it)
    {
        SSliderMarker marker = *(it);
        if(marker.m_rt.PtInRect(point))
        {
            RemoveMarker(marker.m_nPoint);           
            break;
        }
    }    

    
    CSliderCtrl::OnRButtonDown(nFlags, point);
}

void CExSliderCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CSliderCtrl::OnKillFocus(pNewWnd);

	m_bClickStart = FALSE;	
	m_bClickEnd = FALSE;
	m_bClickTick = FALSE;
    m_bClickMarker = FALSE;    
}

void CExSliderCtrl::PreSubclassWindow()
{
	DWORD dwStyle = ::GetClassLong(GetSafeHwnd(), GCL_STYLE);
	dwStyle |= CS_DBLCLKS;
	::SetClassLong(GetSafeHwnd(), GCL_STYLE, dwStyle);

	CSliderCtrl::PreSubclassWindow();

	m_ctrTip.Create(this, TTS_ALWAYSTIP);
	m_ctrTip.AddTool(this);
	m_ctrTip.Activate(TRUE);
}



void CExSliderCtrl::PopupMenu(CPoint point)
{
	//CMenu menu;
	//menu.CreatePopupMenu();

	//menu.AppendMenu(MF_STRING, ID_SET_RANGE, _T("Set Frame Range"));
	//menu.AppendMenu(MF_SEPARATOR);

	//menu.AppendMenu(MF_STRING, ID_SET_START, _T("Set Start Frame"));
	//menu.AppendMenu(MF_STRING, ID_SET_END, _T("Set End Frame"));

	//ClientToScreen(&point);
	//menu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CExSliderCtrl::SetStartPnt(int nVal)
{
	int nMax = 0;
	int nMin = 0;
	GetRange(nMin, nMax);

	if(nVal < nMin)
		m_nStartPnt = nMin;
	else if(nVal >= m_nEndPnt)
		m_nStartPnt = m_nEndPnt - 1;
	else
		m_nStartPnt = nVal;
}

void CExSliderCtrl::SetEndPnt(int nVal)
{
	int nMax = 0;
	int nMin = 0;
	GetRange(nMin, nMax);

	if(nVal > nMax)
		m_nEndPnt = nMax;
	else if(nVal <= m_nStartPnt)
		m_nEndPnt = m_nStartPnt + 1;
	else
		m_nEndPnt = nVal;
}

BOOL CExSliderCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message== WM_LBUTTONDOWN ||
		pMsg->message== WM_LBUTTONUP ||
		pMsg->message== WM_MOUSEMOVE)
		m_ctrTip.RelayEvent(pMsg);

	return CSliderCtrl::PreTranslateMessage(pMsg);
}

void CExSliderCtrl::OnNeedText(NMHDR* pnmh, LRESULT* pResult)
{
	//TOOLTIPTEXT* pTip = (TOOLTIPTEXT*)pnmh;
	//{
	//	wsprintf(pTip->szText, "Pos: %d", GetPos());
	//}

	*pResult = 0;
}

BOOL CExSliderCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	//---------------------------------------------------------
	// 깜박거림을 없애기 위해서 배경을 지우지 않는다.  
	//---------------------------------------------------------
	return TRUE;
}

void CExSliderCtrl::OnKeyDown(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if(!m_bSetRange)
		return;

	if(::GetKeyState(VK_LEFT) < 0 || ::GetKeyState(VK_UP) < 0)
	{
		//---------------------------------------------------------
		// 감소  
		//---------------------------------------------------------
		int nPos = GetPos();
		--nPos;
		if(nPos < m_nStartPnt)
			nPos = m_nStartPnt;

		SetPos(nPos);
		Invalidate();
		::SendMessage(GetParent()->GetSafeHwnd(), EXSM_POS_CHANGED, NULL, NULL);
	}
	else if(::GetKeyState(VK_RIGHT) < 0 || ::GetKeyState(VK_DOWN) < 0)
	{
		//---------------------------------------------------------
		// 증가  
		//---------------------------------------------------------
		int nPos = GetPos();
		++nPos;
		if(nPos > m_nEndPnt)
			nPos = m_nEndPnt;

		SetPos(nPos);
		Invalidate();
		::SendMessage(GetParent()->GetSafeHwnd(), EXSM_POS_CHANGED, NULL, NULL);
	}
}

SSliderMarker* CExSliderCtrl::GetSelectMarker()
{
    if(m_nClickMarkerID == -1)
        return  NULL;

    return &m_vMarker[m_nClickMarkerID];        
}

void CExSliderCtrl::SelectMarker(int nPos)
{
    for(UINT i = 0; i < m_vMarker.size(); ++i)
    {
        if(m_vMarker[i].m_nPoint == nPos)
        {
            m_nClickMarkerID = i;
            ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_MARKER_SELECT, NULL, NULL);
            break;
        }
    }
}

void CExSliderCtrl::AddMarker(int nPos, SEventAnim* pEventAnim)
{
    if(nPos <= m_nStartPnt)
        return;

    if(m_vMarker.size() >= MAX_HIT_EVENT_COUNT)
    {
        MessageBox(L"Can't Insert Hit Event More");
        return;
    }

    SSliderMarker sliderMarker;    
    sliderMarker.m_nPoint = nPos;
    sliderMarker.m_pAnimEvent = pEventAnim;

    m_vMarker.push_back(sliderMarker);

    SelectMarker(nPos);

    ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_MARKER_ADD, NULL, NULL);
}

void CExSliderCtrl::RemoveMarker(int nPos)
{
    VMarker::iterator it;
    for(it = m_vMarker.begin(); it != m_vMarker.end(); ++it)
    {
        SSliderMarker marker = *it;
        if(marker.m_nPoint == nPos)
        {
            ::SendMessage(GetParent()->GetSafeHwnd(), EXSM_MARKER_REMOVE, NULL, (LPARAM)&marker);
            m_vMarker.erase(it);
            break;
        }
    }

    m_nClickMarkerID = -1;

    Invalidate();
    
}

void CExSliderCtrl::ClearMarker()
{
    m_vMarker.clear();
}

void CExSliderCtrl::SetSelectMarkerPos(int nPos)
{
    if(m_nClickMarkerID != -1)
    {
        m_vMarker[m_nClickMarkerID].m_nPoint = nPos;
    }
}