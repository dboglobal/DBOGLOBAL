// SequenceGraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "SequenceGraphDlg.h"
#include "MainFrm.h"
#include "DTEditorGlobal.h"


// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"


#define DEF_RULER_GRID		20
#define DEF_RULER_TIME		0.5f



BEGIN_MESSAGE_MAP(CSequenceGraphDlg::CSequenceEditView, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

int	CSequenceGraphDlg::CSequenceEditView::GetRulerGrid(void)
{
	return DEF_RULER_GRID;
}

float CSequenceGraphDlg::CSequenceEditView::GetRulerTime(void)
{
	return DEF_RULER_TIME;
}

void CSequenceGraphDlg::CSequenceEditView::CalcSeqCtrl(void)
{
#define SEQ_RECT_OFFSET		5

	DeleteSeqCtrl();

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)GetActiveDTProp();	
	if(pDTSpline == NULL)
		return;

	const SSplineProp *pSplineProp = pDTSpline->GetSplineProp();
	const SSplineCtrlPt *pCtrlPt = pDTSpline->GetSplineControlPoint();

	int nPointNum = pSplineProp->iCtrlNum;
	
	float fSumTime = pCtrlPt[0].fTime;
	float fGroupTime = pCtrlPt[0].fTime;
	
	SSeqCtrlInfo *pCtrlInfo;

	for(int i = 1; i < nPointNum; i++)
	{
		if(fabs(fGroupTime - pCtrlPt[i].fTime) > 0.001f)
		{
			pCtrlInfo = new SSeqCtrlInfo;
			pCtrlInfo->nIdx = i;
			pCtrlInfo->pt = GetSeqPosition(i, fSumTime);
			pCtrlInfo->rt.SetRect(pCtrlInfo->pt.x-SEQ_RECT_OFFSET, pCtrlInfo->pt.y-SEQ_RECT_OFFSET,
								pCtrlInfo->pt.x+SEQ_RECT_OFFSET, pCtrlInfo->pt.y+SEQ_RECT_OFFSET);

			m_listSeqCtrl.push_back(pCtrlInfo);

			fGroupTime = pCtrlPt[i].fTime;
		}

		fSumTime += pCtrlPt[i].fTime;
	}

	// end point는 무조건 추가한다.
	pCtrlInfo = new SSeqCtrlInfo;
	pCtrlInfo->nIdx = nPointNum;
	pCtrlInfo->pt = GetSeqPosition(pCtrlInfo->nIdx, fSumTime);
	pCtrlInfo->rt.SetRect(pCtrlInfo->pt.x-SEQ_RECT_OFFSET, pCtrlInfo->pt.y-SEQ_RECT_OFFSET, 
						pCtrlInfo->pt.x+SEQ_RECT_OFFSET, pCtrlInfo->pt.y+SEQ_RECT_OFFSET);

	m_listSeqCtrl.push_back(pCtrlInfo);
}


void CSequenceGraphDlg::CSequenceEditView::DeleteSeqCtrl(void)
{
	SSeqCtrlInfo *pCtrlInfo;

	ListSeqCtrl::iterator it;
	for(it = m_listSeqCtrl.begin(); it != m_listSeqCtrl.end(); ++it)
	{
		pCtrlInfo = (*it);
		delete pCtrlInfo;
	}

	m_listSeqCtrl.clear();
}

SSeqCtrlInfo* CSequenceGraphDlg::CSequenceEditView::PrevSeqCtrl(SSeqCtrlInfo *pCtrlInfo)
{
	SSeqCtrlInfo *pPrevCtrlInfo = NULL;

	ListSeqCtrl::iterator it;
	for(it = m_listSeqCtrl.begin(); it != m_listSeqCtrl.end(); ++it)
	{
		if(pCtrlInfo == (*it))
			return pPrevCtrlInfo;

		pPrevCtrlInfo = (*it);
	}

	return NULL;
}

SSeqCtrlInfo* CSequenceGraphDlg::CSequenceEditView::NextSeqCtrl(SSeqCtrlInfo *pCtrlInfo)
{
	ListSeqCtrl::iterator it;
	for(it = m_listSeqCtrl.begin(); it != m_listSeqCtrl.end(); ++it)
	{
		if(pCtrlInfo == (*it))
		{
			++it;

			if(it == m_listSeqCtrl.end())
				return NULL;

			return (*it);
		}
	}

	return NULL;
}


SSeqCtrlInfo* CSequenceGraphDlg::CSequenceEditView::GetSeqCtrl(CPoint pt)
{
	pt += m_ptScrollOffset;

	ListSeqCtrl::iterator it;
	for(it = m_listSeqCtrl.begin(); it != m_listSeqCtrl.end(); ++it)
	{
		if((*it)->rt.PtInRect(pt))
			return (*it);
	}

	return NULL;
}


float CSequenceGraphDlg::CSequenceEditView::GetSeqTime(CPoint pt)
{
	CRect rt;
	GetClientRect(rt);

	return (float)(rt.bottom - pt.y - m_ptScrollOffset.y)/(float)GetRulerGrid()*GetRulerTime();
}

float CSequenceGraphDlg::CSequenceEditView::GetSeqTime(SSeqCtrlInfo *pCtrlInfo)
{
	CRect rt;
	GetClientRect(rt);

	return (float)(rt.bottom - pCtrlInfo->pt.y)/(float)GetRulerGrid()*GetRulerTime();
}


CPoint CSequenceGraphDlg::CSequenceEditView::GetSeqPosition(int nIdx, float fTime)
{
	CRect rt;
	GetClientRect(rt);

	CPoint pt;
	pt.x = rt.left+nIdx*GetRulerGrid();
	pt.y = rt.bottom-(int)((fTime/GetRulerTime())*(float)GetRulerGrid());

	return pt;
}

SSeqCtrlInfo* CSequenceGraphDlg::CSequenceEditView::OnSelPropPoint(CPoint point)
{
	m_pCurrCtrlSel	= NULL;
	m_pPrevCtrlSel	= NULL;
	m_pNextCtrlSel	= NULL;

	CRect rt;

	ListSeqCtrl::iterator it;
	for(it = m_listSeqCtrl.begin(); it != m_listSeqCtrl.end(); ++it)
	{
		rt = (*it)->rt;
		rt.OffsetRect(-m_ptScrollOffset);

		if(rt.PtInRect(point))
		{
			m_pCurrCtrlSel	= (*it);
			m_pPrevCtrlSel	= PrevSeqCtrl(m_pCurrCtrlSel);
			m_pNextCtrlSel	= NextSeqCtrl(m_pCurrCtrlSel);
		}
	}

	return m_pCurrCtrlSel;
}

void CSequenceGraphDlg::CSequenceEditView::OnSelPropPointClear(void)
{
	m_pCurrCtrlSel	= NULL;
	m_pPrevCtrlSel	= NULL;
	m_pNextCtrlSel	= NULL;
}

void CSequenceGraphDlg::CSequenceEditView::OnScrollPositionClear(void)
{
	m_ptScrollOffset.SetPoint(0, 0);
}

void CSequenceGraphDlg::CSequenceEditView::OnGridPaint(CPaintDC& dc)
{
	CRect rt;
	GetClientRect(rt);

	int nGridCx = rt.Width()/GetRulerGrid() + 1;
	int nGridCy = rt.Width()/GetRulerGrid() + 1;

	int nOffsetX = (m_ptScrollOffset.x % GetRulerGrid());
	int nOffsetY = m_ptScrollOffset.y % GetRulerGrid();

	int nStartX = m_ptScrollOffset.x/GetRulerGrid();
	int nStartY = abs(m_ptScrollOffset.y)/GetRulerGrid();

	// 가로
	for(int i = 0; i < nGridCy; i++)
	{
		if( nStartY % 10)
			dc.SelectObject(&m_penGrid);
		else
			dc.SelectObject(&m_penBlockGrid);

		nStartY++;

		dc.MoveTo(rt.left, rt.bottom - nOffsetY - i*GetRulerGrid());
		dc.LineTo(rt.right,rt.bottom - nOffsetY - i*GetRulerGrid());
	}

	// 세로.

	for(int i = 0; i < nGridCx; i++)
	{
		if(nStartX % 10)
			dc.SelectObject(&m_penGrid);
		else
			dc.SelectObject(&m_penBlockGrid);

		nStartX++;

		dc.MoveTo(rt.left - nOffsetX + i*GetRulerGrid(), rt.top);
		dc.LineTo(rt.left - nOffsetX + i*GetRulerGrid(), rt.bottom);
	}
}


void CSequenceGraphDlg::CSequenceEditView::OnProPointPaint(CPaintDC& dc)
{
	if(GetActiveDTProp() == NULL)
		return;

	dc.SelectObject(&m_penPropPoint);

	SSeqCtrlInfo *pCtrlInfo;

	CRect rtClient;
	GetClientRect(rtClient);

	CPoint ptPrev = -m_ptScrollOffset;
	ptPrev.y += rtClient.bottom;

	CPoint ptCurr;
	CRect rt;

	ListSeqCtrl::iterator it;
	for(it = m_listSeqCtrl.begin(); it != m_listSeqCtrl.end(); ++it)
	{
		pCtrlInfo = (*it);

		ptCurr = pCtrlInfo->pt - m_ptScrollOffset;

		rt = pCtrlInfo->rt;
		rt.OffsetRect(-m_ptScrollOffset);

		dc.MoveTo(ptPrev.x, ptPrev.y);
		dc.LineTo(ptCurr.x, ptCurr.y);
		
		dc.FillSolidRect(rt, RGB(170, 13, 60));
		dc.Draw3dRect(rt, RGB(232, 17, 82), RGB(87, 6, 31));

		ptPrev = pCtrlInfo->pt - m_ptScrollOffset;
	}

	dc.RestoreDC(-1);
}

void CSequenceGraphDlg::CSequenceEditView::OnPaneMove(int nDeltaX, int nDeltaY)
{
	m_ptScrollOffset.x += nDeltaX;
	m_ptScrollOffset.y += nDeltaY;

	if(m_ptScrollOffset.x < 0)
		m_ptScrollOffset.x = 0;

	if(m_ptScrollOffset.y > 0)
		m_ptScrollOffset.y = 0;
}

void CSequenceGraphDlg::CSequenceEditView::OnPropPointMove(CPoint point, CNtlDTSpline *pDTSpilne)
{
	// position 구하기.
	CPoint ptPrev, ptNext;

	if(m_pPrevCtrlSel == NULL)
		ptPrev = GetSeqPosition(0, 0.0f);
	else
		ptPrev = m_pPrevCtrlSel->pt;

	if(m_pNextCtrlSel != NULL)
		ptNext = m_pNextCtrlSel->pt;

	ptPrev -= m_ptScrollOffset;
	ptNext -= m_ptScrollOffset;

	if(point.y >= ptPrev.y)
		return;

	if(m_pNextCtrlSel &&  point.y <= ptNext.y)
		return;

	// time data change
	int nSx, nEx;
	float fSumTime, fNextSumTime, fCurrSumTime, fTime;
	if(m_pNextCtrlSel)
	{
		if(m_pPrevCtrlSel == NULL)
			nSx = 0;
		else
			nSx = m_pPrevCtrlSel->nIdx;

		nEx = m_pCurrCtrlSel->nIdx;

		fSumTime = pDTSpilne->GetSumPointTime(0, nSx);
		fNextSumTime = pDTSpilne->GetSumPointTime(0, m_pNextCtrlSel->nIdx);
		fCurrSumTime = GetSeqTime(point);

		fTime = (fCurrSumTime - fSumTime)/(nEx-nSx);
		if(fTime < 0.0001f)
			fTime = 0.0f;

		pDTSpilne->ChangeControlPointTime(nSx, nEx, fTime);

		nSx = m_pCurrCtrlSel->nIdx;
		nEx = m_pNextCtrlSel->nIdx;

		fTime = (fNextSumTime - fCurrSumTime)/(nEx-nSx);
		if(fTime < 0.0001f)
			fTime = 0.0f;

		pDTSpilne->ChangeControlPointTime(nSx, nEx, fTime);
	}
	else // 맨끝점일 경우.
	{
		if(m_pPrevCtrlSel == NULL)
		{
			nSx = 0;
			fSumTime = 0.0f;
		}
		else
		{
			nSx = m_pPrevCtrlSel->nIdx;
			fSumTime = pDTSpilne->GetSumPointTime(0, nSx);
		}

		nEx = m_pCurrCtrlSel->nIdx;

		float fEndTime = GetSeqTime(point);
//		fTime = (fEndTime - fSumTime)/(nEx-nSx);
//		pDTSpilne->ChangeControlPointTime(nSx, nEx, fTime);

		pDTSpilne->ChangeControlPointTime(fEndTime);
	}
	
	// graphic 위치 다시 계산.
	fSumTime = pDTSpilne->GetSumPointTime(0, m_pCurrCtrlSel->nIdx);
	CPoint ptNewPosition = GetSeqPosition(m_pCurrCtrlSel->nIdx, fSumTime);
	m_pCurrCtrlSel->pt.y = ptNewPosition.y;
	m_pCurrCtrlSel->rt.SetRect(m_pCurrCtrlSel->pt.x-SEQ_RECT_OFFSET, m_pCurrCtrlSel->pt.y-SEQ_RECT_OFFSET, 
								m_pCurrCtrlSel->pt.x+SEQ_RECT_OFFSET, m_pCurrCtrlSel->pt.y+SEQ_RECT_OFFSET);
}

void CSequenceGraphDlg::CSequenceEditView::OnProPointUpdate(void)
{
	DeleteSeqCtrl();
	CalcSeqCtrl();
}

void CSequenceGraphDlg::CSequenceEditView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.

	dc.SaveDC();

	CRect rt;
	GetClientRect(rt);
	dc.FillSolidRect(rt, RGB(10, 10, 10));

	OnGridPaint(dc);
	OnProPointPaint(dc);

	dc.RestoreDC(-1);
}




void CSequenceGraphDlg::CSequenceEditView::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	OnProPointUpdate();
}

int CSequenceGraphDlg::CSequenceEditView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_pPrevCtrlSel	= NULL;
	m_pNextCtrlSel	= NULL;
	m_pCurrCtrlSel	= NULL;

	m_ptScrollOffset.SetPoint(0, 0);

	m_penGrid.CreatePen(PS_SOLID, 0, RGB(70, 70, 70));
	m_penBlockGrid.CreatePen(PS_SOLID, 0, RGB(120, 120, 120));
	m_penPropPoint.CreatePen(PS_SOLID, 0, RGB(240, 47, 105));

	return 0;
}

void CSequenceGraphDlg::CSequenceEditView::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	DeleteSeqCtrl();

	m_penGrid.DeleteObject();
	m_penBlockGrid.DeleteObject();
	m_penPropPoint.DeleteObject();
}

// CSequenceGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSequenceGraphDlg, CDialog)

CSequenceGraphDlg::CSequenceGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSequenceGraphDlg::IDD, pParent)
{
	m_pDTSpilne		= NULL;
	m_pSplineProp	= NULL;
}

CSequenceGraphDlg::~CSequenceGraphDlg()
{
}


void CSequenceGraphDlg::ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData)
{
	m_pDTSpilne		= (CNtlDTSpline*)pDTProp;
	m_pSplineProp	= (SSplineProp*)pPropData;
	
	m_nCurrGridX = 0;
	m_nCurrGridY = 0;
	m_SeqEditView.OnScrollPositionClear();
	m_SeqEditView.OnProPointUpdate();
	m_SeqEditView.Invalidate(FALSE);
	Invalidate(TRUE);
}

void CSequenceGraphDlg::DeActiveSplineProp(void)
{
	m_pDTSpilne		= NULL;
	m_pSplineProp	= NULL;

	m_bSelPoint		= FALSE;

	m_nCurrGridX = 0;
	m_nCurrGridY = 0;
	m_SeqEditView.OnScrollPositionClear();
	m_SeqEditView.OnProPointUpdate();
	m_SeqEditView.Invalidate(FALSE);
	Invalidate(TRUE);
}

void CSequenceGraphDlg::DeleteSplineProp(CNtlDTProp *pDTProp)
{
	if(m_pDTSpilne == pDTProp)
	{
		m_pDTSpilne		= NULL;
		m_pSplineProp	= NULL;

		m_nCurrGridX = 0;
		m_nCurrGridY = 0;
		m_SeqEditView.OnScrollPositionClear();
		m_SeqEditView.OnProPointUpdate();
		m_SeqEditView.Invalidate(FALSE);
		Invalidate(TRUE);
	}
}

void CSequenceGraphDlg::RebuildProp()
{
	m_nCurrGridX = 0;
	m_nCurrGridY = 0;
	m_SeqEditView.OnScrollPositionClear();
	m_SeqEditView.OnProPointUpdate();
	m_SeqEditView.Invalidate(FALSE);
	Invalidate(TRUE);
}

void CSequenceGraphDlg::ChangeControlPointTime(void)
{
	m_SeqEditView.OnProPointUpdate();
	m_SeqEditView.Invalidate(FALSE);
}



void CSequenceGraphDlg::CalcSeqEditView(void)
{
	CRect rt;
	GetClientRect(rt);

	m_rtSeqEdit.SetRect(rt.left + 40, rt.top + 25, rt.right - 30, rt.bottom - 30);

	m_rtWndZoomInOut.SetRect(rt.right-20, rt.bottom-20, rt.right, rt.bottom);
}

CPoint CSequenceGraphDlg::GetLocalEditView(CPoint point)
{
	POINT ptMouseScreen;
	ptMouseScreen.x = point.x;
	ptMouseScreen.y = point.y;

	POINT ptViewScreen;
	ptViewScreen.x = m_rtSeqEdit.left;
	ptViewScreen.y = m_rtSeqEdit.top;
	
	ClientToScreen(&ptMouseScreen);
	ClientToScreen(&ptViewScreen);

	CPoint ptViewPoint;
	ptViewPoint.SetPoint(ptMouseScreen.x - ptViewScreen.x, ptMouseScreen.y - ptViewScreen.y);

	return ptViewPoint;
}



void CSequenceGraphDlg::DrawAxis(CPaintDC& dc)
{
	// ruler time
	int nTimeNum = m_rtSeqEdit.Height()/m_nRulerSize;
	int nPointNum = m_rtSeqEdit.Width()/m_nRulerSize;

	dc.SelectObject(&m_penRuler);

	for(int i = 1; i < nTimeNum; i++)
	{
		if(i%2)
		{
			dc.MoveTo(m_rtSeqEdit.left-6, m_rtSeqEdit.bottom-i*m_nRulerSize);
			dc.LineTo(m_rtSeqEdit.left, m_rtSeqEdit.bottom-i*m_nRulerSize);
		}
		else
		{
			dc.MoveTo(m_rtSeqEdit.left-10, m_rtSeqEdit.bottom-i*m_nRulerSize);
			dc.LineTo(m_rtSeqEdit.left, m_rtSeqEdit.bottom-i*m_nRulerSize);
		}
	}
	
	for(int i = 1; i < nPointNum; i++)
	{
		if(i%5 == 0)
		{
			dc.MoveTo(m_rtSeqEdit.left+i*m_nRulerSize, m_rtSeqEdit.bottom);
			dc.LineTo(m_rtSeqEdit.left+i*m_nRulerSize, m_rtSeqEdit.bottom+10);
		}
		else
		{
			dc.MoveTo(m_rtSeqEdit.left+i*m_nRulerSize, m_rtSeqEdit.bottom);
			dc.LineTo(m_rtSeqEdit.left+i*m_nRulerSize, m_rtSeqEdit.bottom+5);
		}
	}

	
	// ruler time number

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(50, 50, 200));

	CString str;
		
	for(int i = 1; i < nTimeNum; i++)
	{
		if(i%2 == 0)
		{
			str.Format("%.1f", m_fRulderTime*(i+m_nCurrGridY));
			dc.TextOut(m_rtSeqEdit.left-25, m_rtSeqEdit.bottom-i*m_nRulerSize -15, str);
		}
	}

	for(int i = 1; i < nPointNum; i++)
	{
		if(i%5 == 0)
		{
			str.Format("%d", (i+m_nCurrGridX));
			dc.TextOut(m_rtSeqEdit.left+i*m_nRulerSize, m_rtSeqEdit.bottom+5, str);
		}
	}
}

void CSequenceGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTROL_POINT_TIME_INFO, m_ctrlTimeInfo);
}


BEGIN_MESSAGE_MAP(CSequenceGraphDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_SEQUENCE_GRAPH_CENTER, &CSequenceGraphDlg::OnBnClickedSequenceGraphCenter)
END_MESSAGE_MAP()


// CSequenceGraphDlg 메시지 처리기입니다.

void CSequenceGraphDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bSelPoint		= FALSE;
	m_bWndZoomInOut	= FALSE;
	
	if(m_rtSeqEdit.PtInRect(point))
	{
		CPoint ptViewPoint = GetLocalEditView(point);
		if(m_SeqEditView.OnSelPropPoint(ptViewPoint))
		{
			m_bSelPoint = TRUE;
			SetCapture();
		}
	}

	else if(m_rtWndZoomInOut.PtInRect(point))
	{
		m_ptOldWndZoomInOut = point;
		m_bWndZoomInOut = TRUE;
		SetCapture();
	}
	
	
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}

void CSequenceGraphDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bSelPoint)
	{
		m_SeqEditView.OnSelPropPointClear();
		ReleaseCapture();
	}

	if(m_bWndZoomInOut)
	{
		ReleaseCapture();
	}
	
	m_bSelPoint		= FALSE;
	m_bWndZoomInOut = FALSE;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonUp(nFlags, point);
}

void CSequenceGraphDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_rtSeqEdit.PtInRect(point))
	{
		m_bPanMove = TRUE;
		m_ptOldPane	= point;
		SetCapture();
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnRButtonDown(nFlags, point);
}

void CSequenceGraphDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(m_bPanMove)
		ReleaseCapture();

	m_bPanMove = FALSE;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnRButtonUp(nFlags, point);
}


void CSequenceGraphDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptViewPoint = GetLocalEditView(point);

	if(m_bSelPoint)
	{
		m_SeqEditView.OnPropPointMove(ptViewPoint, m_pDTSpilne);
		m_SeqEditView.InvalidateRect(FALSE);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
		pPropertyPane->RefreshSplineProp();
	}
	
	/*
	if(m_bSelPoint)
	{
		SSeqCtrlInfo *pPrevCtrlSel = m_SeqEditView.GetPrevSeqCtrl();
		SSeqCtrlInfo *pCurrCtrlSel = m_SeqEditView.GetCurrSeqCtrl();
		SSeqCtrlInfo *pNextCtrlSel = m_SeqEditView.GetNextSeqCtrl();
		// position 구하기.
		CPoint ptPrev, ptNext;

		if(pPrevCtrlSel == NULL)
			ptPrev = GetSeqPosition(0, 0.0f);
		else
			ptPrev = pPrevCtrlSel->pt;

		if(pNextCtrlSel != NULL)
			ptNext = pNextCtrlSel->pt;

		if(point.y >= ptPrev.y)
			return CDialog::OnMouseMove(nFlags, point);

		if(pNextCtrlSel &&  point.y <= ptNext.y)
			return CDialog::OnMouseMove(nFlags, point);


		// time data change

		int nSx, nEx;
		float fSumTime, fNextSumTime, fCurrSumTime, fTime;
		if(pNextCtrlSel)
		{
			if(pPrevCtrlSel == NULL)
				nSx = 0;
			else
				nSx = pPrevCtrlSel->nIdx;

			nEx = pCurrCtrlSel->nIdx;
			
			fSumTime = m_pDTSpilne->GetSumPointTime(0, nSx);
			fNextSumTime = m_pDTSpilne->GetSumPointTime(0, pNextCtrlSel->nIdx);
			fCurrSumTime = GetSeqTime(point);

			fTime = (fCurrSumTime - fSumTime)/(nEx-nSx);
			if(fTime < 0.0001f)
				fTime = 0.0f;

			m_pDTSpilne->ChangeControlPointTime(nSx, nEx, fTime);

			nSx = pCurrCtrlSel->nIdx;
			nEx = pNextCtrlSel->nIdx;

			fTime = (fNextSumTime - fCurrSumTime)/(nEx-nSx);
			if(fTime < 0.0001f)
				fTime = 0.0f;

			m_pDTSpilne->ChangeControlPointTime(nSx, nEx, fTime);
		}
		else // 맨끝점일 경우.
		{
			if(pPrevCtrlSel == NULL)
			{
				nSx = 0;
				fSumTime = 0.0f;
			}
			else
			{
				nSx = pPrevCtrlSel->nIdx;
				fSumTime = m_pDTSpilne->GetSumPointTime(0, nSx);
			}

			nEx = pCurrCtrlSel->nIdx;

			float fEndTime = GetSeqTime(point);
			fTime = (fEndTime - fSumTime)/(nEx-nSx);
			m_pDTSpilne->ChangeControlPointTime(nSx, nEx, fTime);
		}

		CalcRulerTime();
		
		// graphic 위치 다시 계산.
		pCurrCtrlSel->pt.y = point.y;
		pCurrCtrlSel->rt.SetRect(pCurrCtrlSel->pt.x-SEQ_RECT_OFFSET, pCurrCtrlSel->pt.y-SEQ_RECT_OFFSET, 
								pCurrCtrlSel->pt.x+SEQ_RECT_OFFSET, pCurrCtrlSel->pt.y+SEQ_RECT_OFFSET);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
		pPropertyPane->RefreshSplineProp();

		m_SeqEditView.InvalidateRect(FALSE);
	}
	*/

	if(m_bWndZoomInOut)
	{
		CPoint ptDelta = point - m_ptOldWndZoomInOut;

		CRect rtWindow;
		GetWindowRect(rtWindow);

		rtWindow.right += ptDelta.x;
		rtWindow.bottom += ptDelta.y;

		MoveWindow(rtWindow.left, rtWindow.top, rtWindow.Width(), rtWindow.Height());

		m_ptOldWndZoomInOut = point;

		Invalidate(TRUE);
	}
	else if(m_bPanMove)
	{
		CPoint ptDelta = m_ptOldPane - point;
		m_SeqEditView.OnPaneMove(ptDelta.x, ptDelta.y);
		m_SeqEditView.Invalidate(FALSE);

		m_ptOldPane = point;

		CPoint ptScroll = m_SeqEditView.GetScrollPosition();
		int nGridX = ptScroll.x / m_nRulerSize;
		int nGridY = abs(ptScroll.y) / m_nRulerSize;

		if(m_nCurrGridX != nGridX || m_nCurrGridY != nGridY)
		{
			m_nCurrGridX = nGridX;
			m_nCurrGridY = nGridY;
			Invalidate(TRUE);
		}
	}
	else
	{
		if(m_rtSeqEdit.PtInRect(ptViewPoint))
		{
			// 정보 윈도우.
			SSeqCtrlInfo *pSeqCtrl = m_SeqEditView.GetSeqCtrl(ptViewPoint);
			if(pSeqCtrl)
			{
				float fTime = m_SeqEditView.GetSeqTime(pSeqCtrl);
				CString str;
				str.Format("%.2f",fTime); 
				SetDlgItemText(IDC_CONTROL_POINT_TIME_INFO, str);
				CRect rt;
				m_ctrlTimeInfo.GetClientRect(&rt);
				m_ctrlTimeInfo.MoveWindow(point.x+20, point.y+5, rt.Width(), rt.Height());
				m_ctrlTimeInfo.ShowWindow(SW_SHOW);
			}
			else
			{
				m_ctrlTimeInfo.ShowWindow(SW_HIDE);
			}
		}
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CSequenceGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_bSelPoint		= FALSE;
	m_bWndZoomInOut = FALSE;
	m_bPanMove		= FALSE;

	m_nRulerSize	=	DEF_RULER_GRID;
	m_fRulderTime	= DEF_RULER_TIME;

	m_nCurrGridX	= 0;
	m_nCurrGridY	= 0;

	CalcSeqEditView();

	m_ctrlTimeInfo.ShowWindow(SW_HIDE);

	m_SeqEditView.Create("", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, m_rtSeqEdit, this, IDC_SEQUENCEEDIT_VIEW);
	m_SeqEditView.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_SeqEditView.MoveWindow(m_rtSeqEdit);
	m_SeqEditView.Invalidate(TRUE);

	m_penRuler.CreatePen(PS_SOLID, 0, RGB(100, 100, 255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSequenceGraphDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(m_SeqEditView.GetSafeHwnd())
	{
		CalcSeqEditView();

		m_SeqEditView.MoveWindow(m_rtSeqEdit);
		m_SeqEditView.Invalidate(TRUE);
	}
}

void CSequenceGraphDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CSequenceGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	dc.SaveDC();

	DrawAxis(dc);

	dc.FillSolidRect(m_rtWndZoomInOut, RGB(170, 60, 60));

	dc.RestoreDC(-1);
}

void CSequenceGraphDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	m_penRuler.DeleteObject();
}

void CSequenceGraphDlg::OnBnClickedSequenceGraphCenter()
{
	m_SeqEditView.OnScrollPositionClear();
	m_nCurrGridX = 0;
	m_nCurrGridY = 0;

	Invalidate(TRUE);
}
