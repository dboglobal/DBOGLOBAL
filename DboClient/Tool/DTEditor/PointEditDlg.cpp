// PointEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "PointEditDlg.h"
#include "MainFrm.h"
#include "ControlPointView.h"
#include "DTEditorGlobal.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"

#define POINT_EDIT_VIEW_TYPE_FRONT		0
#define POINT_EDIT_VIEW_TYPE_TOP		1
#define POINT_EDIT_VIEW_TYPE_SIDE		2
#define POINT_EDIT_VIEW_TYPE_RESERVED	3

#define POINT_EDIT_VIEW_SPACE				4
#define POINT_EDIT_GRID_SIZE				20
#define POINT_EDIT_GRID_BLOCK_NUM			10
#define POINT_EDIT_GRID_RULER_SHAPE_NUM		10



BEGIN_MESSAGE_MAP(CPointEditDlg::CPointEditView, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CPointEditDlg::CPointEditView::CalcBackgourndSize(void)
{
	GetClientRect(m_rtBackground);
	m_rtBackground.left		= POINT_EDIT_VIEW_SPACE/2;
	m_rtBackground.top		= POINT_EDIT_VIEW_SPACE/2;
	m_rtBackground.right	= m_rtBackground.right - POINT_EDIT_VIEW_SPACE/2;
	m_rtBackground.bottom	= m_rtBackground.bottom - POINT_EDIT_VIEW_SPACE/2;
}

void CPointEditDlg::CPointEditView::CalcGridNum(void)
{
	m_nGridXNum = m_rtBackground.Width()/POINT_EDIT_GRID_SIZE/2 +1;
	m_nGridYNum = m_rtBackground.Height()/POINT_EDIT_GRID_SIZE/2 +1;

	m_ptGridCenter.x = m_rtBackground.left+m_rtBackground.Width()/2;
	m_ptGridCenter.y = m_rtBackground.top+m_rtBackground.Height()/2;
}

RwReal CPointEditDlg::CPointEditView::CaleScale(void)
{
	if(m_fScale < 0.0f)
		return 1.0f / fabs(m_fScale - 1.0f);

	return m_fScale;
}

void CPointEditDlg::CPointEditView::CalePropPoint(void)
{
	m_listPropPoint.clear();

	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;

	const SSplineProp *pProp		= pDTSplineProp->GetSplineProp();
	const SSplineCtrlPt *pCtrlPt	= pDTSplineProp->GetSplineControlPoint();

	const RwV3d *pPos;
	int nViewX, nViewY;

	if(m_nViewType == POINT_EDIT_VIEW_TYPE_FRONT)
	{
		int nScrollOffsetX = m_nGridScrollOffsetX%m_nGridRulerSize;
		int nScrollOffsetY = m_nGridScrollOffsetY%m_nGridRulerSize;

		CPoint ptCenter = m_ptGridCenter;
		ptCenter.x += nScrollOffsetX;
		ptCenter.y += nScrollOffsetY;

		RwReal fScaleRate = CaleScale();

		for(int i = 0; i < pProp->iCtrlNum; i++)
		{
			pPos = &pCtrlPt[i].vPos;
			nViewX = -(int)(pPos->x * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
			nViewY = -(int)(pPos->y * (float)POINT_EDIT_GRID_SIZE*fScaleRate);

			nViewX += m_ptGridCenter.x + m_nGridScrollOffsetX;
			nViewY += m_ptGridCenter.y + m_nGridScrollOffsetY;

			SPropPointInfo sPoint;
			sPoint.nIndex = i;
			sPoint.bSelect = FALSE;
			sPoint.pt.SetPoint(nViewX, nViewY);
			m_listPropPoint.push_back(sPoint);
		}
	}
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_TOP)
	{
		int nScrollOffsetX = m_nGridScrollOffsetX%m_nGridRulerSize;
		int nScrollOffsetY = m_nGridScrollOffsetY%m_nGridRulerSize;

		CPoint ptCenter = m_ptGridCenter;
		ptCenter.x += nScrollOffsetX;
		ptCenter.y += nScrollOffsetY;

		RwReal fScaleRate = CaleScale();

		for(int i = 0; i < pProp->iCtrlNum; i++)
		{
			pPos = &pCtrlPt[i].vPos;
			nViewX = -(int)(pPos->x * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
			nViewY = -(int)(pPos->z * (float)POINT_EDIT_GRID_SIZE*fScaleRate);

			nViewX += m_ptGridCenter.x + m_nGridScrollOffsetX;
			nViewY += m_ptGridCenter.y + m_nGridScrollOffsetY;

			SPropPointInfo sPoint;
			sPoint.nIndex = i;
			sPoint.bSelect = FALSE;
			sPoint.pt.SetPoint(nViewX, nViewY);
			m_listPropPoint.push_back(sPoint);
		}
	}

	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_SIDE)
	{
		int nScrollOffsetX = m_nGridScrollOffsetX%m_nGridRulerSize;
		int nScrollOffsetY = m_nGridScrollOffsetY%m_nGridRulerSize;

		CPoint ptCenter = m_ptGridCenter;
		ptCenter.x += nScrollOffsetX;
		ptCenter.y += nScrollOffsetY;

		RwReal fScaleRate = CaleScale();

		for(int i = 0; i < pProp->iCtrlNum; i++)
		{
			pPos = &pCtrlPt[i].vPos;
			nViewX = (int)(pPos->z * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
			nViewY = -(int)(pPos->y * (float)POINT_EDIT_GRID_SIZE*fScaleRate);

			nViewX += m_ptGridCenter.x + m_nGridScrollOffsetX;
			nViewY += m_ptGridCenter.y + m_nGridScrollOffsetY;

			SPropPointInfo sPoint;
			sPoint.nIndex = i;
			sPoint.bSelect = FALSE;
			sPoint.pt.SetPoint(nViewX, nViewY);
			m_listPropPoint.push_back(sPoint);
		}
	}
}

CPoint CPointEditDlg::CPointEditView::CalcPropPointOne(int iIdx)
{
	CPoint pt;
	pt.SetPoint(0, 0);

	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return pt;

	const SSplineProp *pProp		= pDTSplineProp->GetSplineProp();
	const SSplineCtrlPt *pCtrlPt	= pDTSplineProp->GetSplineControlPoint();

	if(m_nViewType == POINT_EDIT_VIEW_TYPE_FRONT)
	{
		int nScrollOffsetX = m_nGridScrollOffsetX%m_nGridRulerSize;
		int nScrollOffsetY = m_nGridScrollOffsetY%m_nGridRulerSize;

		CPoint ptCenter = m_ptGridCenter;
		ptCenter.x += nScrollOffsetX;
		ptCenter.y += nScrollOffsetY;
		
		RwReal fScaleRate = CaleScale();

		const RwV3d *pPos = &pCtrlPt[iIdx].vPos;
		pt.x = -(int)(pPos->x * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
		pt.y = -(int)(pPos->y * (float)POINT_EDIT_GRID_SIZE*fScaleRate);

		pt.x += m_ptGridCenter.x + m_nGridScrollOffsetX;
		pt.y += m_ptGridCenter.y + m_nGridScrollOffsetY;

		return pt;
	}

	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_TOP)
	{
		int nScrollOffsetX = m_nGridScrollOffsetX%m_nGridRulerSize;
		int nScrollOffsetY = m_nGridScrollOffsetY%m_nGridRulerSize;

		CPoint ptCenter = m_ptGridCenter;
		ptCenter.x += nScrollOffsetX;
		ptCenter.y += nScrollOffsetY;
		
		RwReal fScaleRate = CaleScale();

		const RwV3d *pPos = &pCtrlPt[iIdx].vPos;
		pt.x = -(int)(pPos->x * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
		pt.y = -(int)(pPos->z * (float)POINT_EDIT_GRID_SIZE*fScaleRate);

		pt.x += m_ptGridCenter.x + m_nGridScrollOffsetX;
		pt.y += m_ptGridCenter.y + m_nGridScrollOffsetY;

		return pt;
	}

	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_SIDE)
	{
		int nScrollOffsetX = m_nGridScrollOffsetX%m_nGridRulerSize;
		int nScrollOffsetY = m_nGridScrollOffsetY%m_nGridRulerSize;

		CPoint ptCenter = m_ptGridCenter;
		ptCenter.x += nScrollOffsetX;
		ptCenter.y += nScrollOffsetY;
		
		RwReal fScaleRate = CaleScale();

		const RwV3d *pPos = &pCtrlPt[iIdx].vPos;
		pt.x = (int)(pPos->z * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
		pt.y = -(int)(pPos->y * (float)POINT_EDIT_GRID_SIZE*fScaleRate);

		pt.x += m_ptGridCenter.x + m_nGridScrollOffsetX;
		pt.y += m_ptGridCenter.y + m_nGridScrollOffsetY;

		return pt;
	}
}

void CPointEditDlg::CPointEditView::OnBackgroundPaint(CPaintDC& dc)
{
	if(m_bActive)
		dc.FillSolidRect(m_rtBackground, RGB(40, 40, 40));
	else
		dc.FillSolidRect(m_rtBackground, RGB(10, 10, 10));
}

void CPointEditDlg::CPointEditView::OnGridPaint(CPaintDC& dc)
{
	int nScrollOffsetX = m_nGridScrollOffsetX%m_nGridRulerSize;
	int nScrollOffsetY = m_nGridScrollOffsetY%m_nGridRulerSize;

	int nCenterOffsetX = -(m_nGridScrollOffsetX/m_nGridRulerSize);
	int nCenterOffsetY = -m_nGridScrollOffsetY/m_nGridRulerSize;

	CPoint ptCenter = m_ptGridCenter;
	ptCenter.x += nScrollOffsetX;
	ptCenter.y += nScrollOffsetY;

	// 가로
	int nStartGridY = ptCenter.y - (m_nGridYNum + 1)*m_nGridRulerSize;
	nCenterOffsetY -= (m_nGridYNum + 1);

	for(int i = 0; i < (m_nGridYNum + 1)*2; i++)
	{
		if(nCenterOffsetY == 0)
		{
			dc.SelectObject(&m_penCenter);
		}
		else
		{
			if(nCenterOffsetY % POINT_EDIT_GRID_BLOCK_NUM == 0)
				dc.SelectObject(&m_penBlock);
			else
				dc.SelectObject(&m_penElement);
		}
		
		dc.MoveTo(m_rtBackground.left, nStartGridY + i*m_nGridRulerSize);
		dc.LineTo(m_rtBackground.right, nStartGridY + i*m_nGridRulerSize);

		nCenterOffsetY++;
	}

	// 세로.

	int nStartGridX = ptCenter.x - (m_nGridXNum + 1)*m_nGridRulerSize;
	nCenterOffsetX -= (m_nGridXNum + 1);
	
	for(int i = 0; i < (m_nGridXNum + 1)*2; i++)
	{
		if(nCenterOffsetX == 0)
		{
			dc.SelectObject(&m_penCenter);
		}
		else
		{
			if(nCenterOffsetX % POINT_EDIT_GRID_BLOCK_NUM == 0)
				dc.SelectObject(&m_penBlock);
			else
				dc.SelectObject(&m_penElement);
		}

		dc.MoveTo(nStartGridX + i*m_nGridRulerSize, m_rtBackground.top);
		dc.LineTo(nStartGridX + i*m_nGridRulerSize, m_rtBackground.bottom);

		nCenterOffsetX++;
	}


	// 


}

void CPointEditDlg::CPointEditView::OnNumberPaint(CPaintDC& dc)
{
	// name 출력.
	dc.SetBkMode(TRANSPARENT);
	if(m_bActive)
		dc.SetTextColor(RGB(50, 50, 200));
	else
		dc.SetTextColor(RGB(200, 200, 200));

	RwReal fScaleRate = CaleScale();

	CString strPos;
	strPos.Format(" - ( %f , %f )", 
		(float)m_nGridScrollOffsetX /(float)POINT_EDIT_GRID_SIZE*fScaleRate , 
		(float)m_nGridScrollOffsetY /(float)POINT_EDIT_GRID_SIZE*fScaleRate);

	CString strText = m_strViewName + strPos;

	dc.TextOut(5, 5, strText);

	if(m_nViewType == POINT_EDIT_VIEW_TYPE_RESERVED)
	{
		dc.SetTextColor(RGB(200, 200, 10));
		dc.TextOut(5, 25, "(By Jang, Hyung Suk)");
	}
}

void CPointEditDlg::CPointEditView::OnPropPointPaint(CPaintDC& dc)
{
	CRect rtFill;
	CPoint ptLineStart;

	dc.SelectObject(&m_penPropPoint);

	ListPropPoint::iterator it;
	for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
	{
		if((*it).nIndex > 0)
		{
			dc.MoveTo(ptLineStart.x, ptLineStart.y);
			dc.LineTo((*it).pt.x, (*it).pt.y);
		}

		ptLineStart = (*it).pt;

		if((*it).bSelect)
		{
			rtFill.SetRect(ptLineStart.x-5, ptLineStart.y-5, ptLineStart.x+5, ptLineStart.y+5);
			dc.FillSolidRect(rtFill, RGB(170, 60, 60));
			dc.Draw3dRect(rtFill, RGB(232, 82, 82), RGB(87, 31, 31));
		}
		else
		{
			rtFill.SetRect(ptLineStart.x-3, ptLineStart.y-3, ptLineStart.x+3, ptLineStart.y+3);

			if((*it).nIndex == 0)
			{
				dc.FillSolidRect(rtFill, RGB(90, 90, 170));
				dc.Draw3dRect(rtFill, RGB(140, 140, 232), RGB(50, 50, 87));
			}
			else
			{
				dc.FillSolidRect(rtFill, RGB(170, 170, 60));
				dc.Draw3dRect(rtFill, RGB(232, 232, 82), RGB(87, 87, 31));
			}
		}
	}
}

void CPointEditDlg::CPointEditView::OnFocusPropPointPaint(CPaintDC& dc)
{
	if(m_nFocusPropPoint < 0)
		return;
	
	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;
	
	const SSplineCtrlPt *pCtrlPt	= pDTSplineProp->GetSplineControlPoint();

	CString str;

	if(m_nViewType == POINT_EDIT_VIEW_TYPE_FRONT)
		str.Format("(%f , %f)", pCtrlPt[m_nFocusPropPoint].vPos.x, pCtrlPt[m_nFocusPropPoint].vPos.y);
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_TOP)
		str.Format("(%f , %f)", pCtrlPt[m_nFocusPropPoint].vPos.x, pCtrlPt[m_nFocusPropPoint].vPos.z);
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_SIDE)
		str.Format("(%f , %f)", pCtrlPt[m_nFocusPropPoint].vPos.y, pCtrlPt[m_nFocusPropPoint].vPos.z);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(200, 200, 250));
	dc.TextOut(m_ptFocusPropPoint.x + 10, m_ptFocusPropPoint.y - 20, str);
}

void CPointEditDlg::CPointEditView::OnSelectCursorPaint(CPaintDC& dc)
{
	dc.Draw3dRect(m_rtSelectCursor, RGB(10, 255, 10), RGB(10, 255, 10));
}

void CPointEditDlg::CPointEditView::IncScale(void)
{
	m_fScale += 1.0f;

	if(m_fScale == 0.0f)
		m_fScale = 1.0f;

	m_nGridRulerIndex--;

	int nValue = abs(m_nGridRulerIndex)%POINT_EDIT_GRID_RULER_SHAPE_NUM;
	m_nGridRulerSize = POINT_EDIT_GRID_SIZE*(nValue+1);

	CalePropPoint();
}

void CPointEditDlg::CPointEditView::DecScale(void)
{
	m_fScale -= 1.0f;

	if(m_fScale ==  0.0f)
		m_fScale = -1.0f;
	
	m_nGridRulerIndex++;

	int nValue = abs(m_nGridRulerIndex)%POINT_EDIT_GRID_RULER_SHAPE_NUM;
	m_nGridRulerSize = POINT_EDIT_GRID_SIZE*(nValue+1);

	CalePropPoint();
}

float CPointEditDlg::CPointEditView::GetScale(void)
{
	return m_fScale;
}

void CPointEditDlg::CPointEditView::OnPaneMove(int nDeltaX, int nDeltaY)
{
	/*
#define POINT_EDIT_VIEW_TYPE_FRONT		0
#define POINT_EDIT_VIEW_TYPE_TOP		1
#define POINT_EDIT_VIEW_TYPE_SIDE		2
#define POINT_EDIT_VIEW_TYPE_RESERVED	3
	*/

	m_nGridScrollOffsetX += nDeltaX;
	m_nGridScrollOffsetY += nDeltaY;

	float fScale = m_fScale;;
	if(fScale < 0.0f)
		fScale = 1.0f / fabs(fScale - 1.0f);
	
	if(m_nViewType == POINT_EDIT_VIEW_TYPE_FRONT)
	{
		m_fCameraCenterX = (fScale*(float)nDeltaX)/(float)POINT_EDIT_GRID_SIZE;
		m_fCameraCenterY = (fScale*(float)nDeltaY)/(float)POINT_EDIT_GRID_SIZE;
	}
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_TOP)
	{
		m_fCameraCenterX = (fScale*(float)nDeltaX)/(float)POINT_EDIT_GRID_SIZE;
		m_fCameraCenterY = (fScale*(float)nDeltaY)/(float)POINT_EDIT_GRID_SIZE;
	}
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_SIDE)
	{
		m_fCameraCenterX = (fScale*(float)nDeltaX)/(float)POINT_EDIT_GRID_SIZE;
		m_fCameraCenterY = (fScale*(float)nDeltaY)/(float)POINT_EDIT_GRID_SIZE;
	}

	CalePropPoint();
	OnSelectCursorClear();
}

void CPointEditDlg::CPointEditView::OnPaneCenter(void)
{
	m_nGridScrollOffsetX = 0;
	m_nGridScrollOffsetY = 0;

	m_fCameraCenterX = 0.0f;
	m_fCameraCenterY = 0.0f;

	CalePropPoint();
}

void CPointEditDlg::CPointEditView::OnPaneSerachPoint(int nIdx)
{
	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;

	if(m_nViewType == POINT_EDIT_VIEW_TYPE_RESERVED)
		return;

	const SSplineProp *pProp		= pDTSplineProp->GetSplineProp();
	const SSplineCtrlPt *pCtrlPt	= pDTSplineProp->GetSplineControlPoint();

	const RwV3d *pPos = &pCtrlPt[nIdx].vPos;

	RwReal fScaleRate = CaleScale();

	if(m_nViewType == POINT_EDIT_VIEW_TYPE_FRONT)
	{
		m_nGridScrollOffsetX = -(int)(pPos->x * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
		m_nGridScrollOffsetY = -(int)(pPos->y * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
	}
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_TOP)
	{
		m_nGridScrollOffsetX = -(int)(pPos->x * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
		m_nGridScrollOffsetY = -(int)(pPos->z * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
	}
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_SIDE)
	{
		m_nGridScrollOffsetX = (int)(pPos->z * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
		m_nGridScrollOffsetY = -(int)(pPos->y * (float)POINT_EDIT_GRID_SIZE*fScaleRate);
	}

	m_nGridScrollOffsetX = -m_nGridScrollOffsetX;
	m_nGridScrollOffsetY = -m_nGridScrollOffsetY;

	CalePropPoint();
}

void CPointEditDlg::CPointEditView::OnFocusPropPoint(CPoint point)
{
	int nFocusProPoint = -1;
	CRect rt;
	ListPropPoint::iterator it;
	for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
	{
		rt.SetRect((*it).pt.x - 3, (*it).pt.y - 3, (*it).pt.x + 3, (*it).pt.y + 3);
		if(rt.PtInRect(point))
		{
			if(m_nFocusPropPoint != (*it).nIndex)
			{
				m_nFocusPropPoint = (*it).nIndex;
				m_ptFocusPropPoint = (*it).pt;
				Invalidate(FALSE);
			}

			nFocusProPoint = (*it).nIndex;

			return;
		}
	}

	if(nFocusProPoint < 0 && m_nFocusPropPoint >= 0)
	{
		m_nFocusPropPoint = -1;
		Invalidate(FALSE);
	}
}

void CPointEditDlg::CPointEditView::OnSelectPoint(CRect rt, std::list<int>& listSelPoint)
{
	int nSx = 10000;
	int nEx = -1;

	SPropPointInfo sPoint;
	ListPropPoint::iterator it;
	for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
	{
		CPoint pt = (*it).pt;

		if(rt.PtInRect(pt))
		{
			if((*it).nIndex < nSx)
			{
				nSx = (*it).nIndex;
			}

			if((*it).nIndex > nEx)
			{
				nEx = (*it).nIndex;
			}

			listSelPoint.push_back((*it).nIndex);
		}
	}

	for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
	{
		(*it).bSelect = FALSE;
		if((*it).nIndex >= nSx && (*it).nIndex <= nEx)
			(*it).bSelect = TRUE;
	}
}

void CPointEditDlg::CPointEditView::OnSelectPoint(int nSx, int nEx)
{
	SPropPointInfo sPoint;
	ListPropPoint::iterator it;
	for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
	{
		(*it).bSelect = FALSE;
	}

	if(nSx < 0 || nEx < 0)
		return;

	for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
	{
		if((*it).nIndex >= nSx && (*it).nIndex < nEx)
			(*it).bSelect = TRUE;
	}
}

void CPointEditDlg::CPointEditView::OnSelectCursor(CRect rt)
{
	m_rtSelectCursor = rt;
}

void CPointEditDlg::CPointEditView::OnSelectCursorClear(void)
{
	m_rtSelectCursor.SetRect(0, 0, 0, 0);
}

void CPointEditDlg::CPointEditView::OnSelectPointClear(void)
{
	ListPropPoint::iterator it;
	for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
	{
		(*it).bSelect = FALSE;
	}
}

void CPointEditDlg::CPointEditView::OnPropPointMove(int nDeltaX, int nDeltaY)
{
	CNtlDTSpline *pDTSplineProp = (CNtlDTSpline*)GetActiveDTProp();
	if(pDTSplineProp == NULL)
		return;

	if(m_nViewType == POINT_EDIT_VIEW_TYPE_FRONT)
	{
		RwReal fScaleRate = CaleScale();
		ListPropPoint::iterator it;
		for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
		{
			if( (*it).bSelect )
			{
				RwV3d vAddPos;

				vAddPos.x = (float)nDeltaX/(float)(POINT_EDIT_GRID_SIZE*fScaleRate);
				vAddPos.y = (float)nDeltaY/(float)(POINT_EDIT_GRID_SIZE*fScaleRate);
				vAddPos.z = 0.0f;

				pDTSplineProp->AddControlPointPosition((*it).nIndex, (*it).nIndex+1, vAddPos);

				(*it).pt = CalcPropPointOne((*it).nIndex);
			}
		}
	}
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_TOP)
	{
		RwReal fScaleRate = CaleScale();
		ListPropPoint::iterator it;
		for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
		{
			if( (*it).bSelect )
			{
				RwV3d vAddPos;

				vAddPos.x = (float)nDeltaX/(float)(POINT_EDIT_GRID_SIZE*fScaleRate);
				vAddPos.y = 0.0f;
				vAddPos.z = (float)nDeltaY/(float)(POINT_EDIT_GRID_SIZE*fScaleRate);

				pDTSplineProp->AddControlPointPosition((*it).nIndex, (*it).nIndex+1, vAddPos);

				(*it).pt = CalcPropPointOne((*it).nIndex);
			}
		}
	}
	else if(m_nViewType == POINT_EDIT_VIEW_TYPE_SIDE)
	{
		RwReal fScaleRate = CaleScale();
		ListPropPoint::iterator it;
		for(it = m_listPropPoint.begin(); it != m_listPropPoint.end(); it++)
		{
			if( (*it).bSelect )
			{
				RwV3d vAddPos;

				vAddPos.x = 0.0f;
				vAddPos.y = (float)nDeltaY/(float)(POINT_EDIT_GRID_SIZE*fScaleRate);
				vAddPos.z = -(float)nDeltaX/(float)(POINT_EDIT_GRID_SIZE*fScaleRate);

				pDTSplineProp->AddControlPointPosition((*it).nIndex, (*it).nIndex+1, vAddPos);

				(*it).pt = CalcPropPointOne((*it).nIndex);
			}
		}
	}
}

void CPointEditDlg::CPointEditView::OnRecalcPropPoint(void)
{
	CalePropPoint();
}

void CPointEditDlg::CPointEditView::SetViewType(int nType)
{
	m_nViewType = nType;
}

void CPointEditDlg::CPointEditView::SetViewName(const char *pViewName)
{
	m_strViewName = pViewName;
}

void CPointEditDlg::CPointEditView::SetActive(BOOL bActive)
{
	m_bActive = bActive;
}

int CPointEditDlg::CPointEditView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_bActive			= FALSE;
	m_fScale			= 1.0f;
	m_nGridRulerIndex	= 0;
	m_nGridRulerSize	= POINT_EDIT_GRID_SIZE;

	m_nGridScrollOffsetX	= 0;
	m_nGridScrollOffsetY	= 0;
	m_fCameraCenterX		= 0.0f;
	m_fCameraCenterY		= 0.0f;

	m_nFocusPropPoint		= -1;
	m_rtSelectCursor.SetRect(0, 0, 0, 0);

	m_penCenter.CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	m_penBlock.CreatePen(PS_SOLID, 0, RGB(120, 120, 120));
	m_penElement.CreatePen(PS_SOLID, 0, RGB(70, 70, 70));
	m_penPropPoint.CreatePen(PS_SOLID, 0, RGB(80, 80, 30));

	return 0;
}


void CPointEditDlg::CPointEditView::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	m_penCenter.DeleteObject();
	m_penBlock.DeleteObject();
	m_penElement.DeleteObject();
	m_penPropPoint.DeleteObject();
}

void CPointEditDlg::CPointEditView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.

	dc.SaveDC();

	CalcBackgourndSize();
	OnBackgroundPaint(dc);
	OnGridPaint(dc);
	OnNumberPaint(dc);
	OnPropPointPaint(dc);
	OnFocusPropPointPaint(dc);
	OnSelectCursorPaint(dc);

	dc.RestoreDC(-1);
}

void CPointEditDlg::CPointEditView::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	CalcBackgourndSize();
	CalcGridNum();
	CalePropPoint();
}

BOOL CPointEditDlg::CPointEditView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


// CPointEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPointEditDlg, CDialog)

CPointEditDlg::CPointEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointEditDlg::IDD, pParent)
{

}

CPointEditDlg::~CPointEditDlg()
{
}

void CPointEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPointEditDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_POINTEDIT_CENTER_MOVE, &CPointEditDlg::OnBnClickedPointeditCenterMove)
	ON_WM_LBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_POINTEDIT_SEARCH_MOVE, &CPointEditDlg::OnBnClickedPointeditSearchMove)
END_MESSAGE_MAP()


void CPointEditDlg::CalcViewSize(void)
{
	GetClientRect(m_rtFullView);

	CRect rtToolBar;
	GetDlgItem(IDC_POINTEDIT_CENTER_MOVE)->GetClientRect(rtToolBar);
	
	m_rtFullView.top    = rtToolBar.Height();
	m_rtFullView.bottom -= rtToolBar.Height();

	m_rtView[0].SetRect(m_rtFullView.left,		m_rtFullView.top,			m_rtFullView.right/2,	m_rtFullView.bottom/2);
	m_rtView[1].SetRect(m_rtFullView.right/2,	m_rtFullView.top,			m_rtFullView.right,		m_rtFullView.bottom/2);
	m_rtView[2].SetRect(m_rtFullView.left,		m_rtFullView.bottom/2,		m_rtFullView.right/2,	m_rtFullView.bottom);
	m_rtView[3].SetRect(m_rtFullView.right/2,	m_rtFullView.bottom/2,		m_rtFullView.right,		m_rtFullView.bottom);
}

CRect CPointEditDlg::CalcSelectDrag(CPoint point)
{
	CRect rt;
	rt.SetRect(m_ptSelectPoint.x, m_ptSelectPoint.y, point.x, point.y);
	rt.NormalizeRect();

	CRect rtView;
	if(m_bQuaterView)
		rtView = m_rtView[m_nActiveView];
	else
		rtView = m_rtFullView;

	ClientToScreen(&rt);
	ClientToScreen(&rtView);

	rtView.left		= rt.left - rtView.left;
	rtView.top		= rt.top - rtView.top;
	rtView.right	= rtView.left + rt.Width();
	rtView.bottom	= rtView.top + rt.Height();

	return rtView;

}

int CPointEditDlg::SelectView(CPoint point)
{
	int nSelIdx = -1;

	if(m_bQuaterView)
	{
		for(int i = 0; i < 4; i++)
		{
			if(m_rtView[i].PtInRect(point))
			{
				nSelIdx			= i;
				m_nActiveView	= i;

				RwReal fScale = m_EditView[i].GetScale();
				VisibleScale(fScale);

				break;
			}
		}
	}
	else
	{
		if(m_rtFullView.PtInRect(point))
		{
			return m_nActiveView;
		}
	}

	if(nSelIdx >= 0)
	{
		for(int i = 0; i < 4; i++)
		{
			if(i == nSelIdx)
				m_EditView[i].SetActive(TRUE);
			else
				m_EditView[i].SetActive(FALSE);

			m_EditView[i].Invalidate(TRUE);
		}
	}
	
	return nSelIdx;
}

void CPointEditDlg::VisibleScale(float fScale)
{
	CString str;
	if(fScale < 0)
	{
		fScale -= 1.0f;
		fScale = 1.0f / fabs(fScale);
	}

	str.Format("%.1f (m)", fScale);
	SetDlgItemText(IDC_POINTEDIT_SCALE_INFO, str);
}

void CPointEditDlg::VisibleWorldCenter(float f1, float f2)
{
}

void CPointEditDlg::CalcControlPosition(int nCx, int nCy)
{
	m_rtWndZoomInOut.SetRect(nCx-20, nCy-20, nCx, nCy);

	GetDlgItem(IDC_POINTEDIT_SCALE_INFO)->MoveWindow(nCx-110, nCy-20, 80, 20); 
	GetDlgItem(IDC_STATIC_SCALE_INFO)->MoveWindow(nCx-150, nCy-20, 40, 20);
}

void CPointEditDlg::OnPropUpdate(void)
{
	for(int i = 0; i < 4; i++)
	{
		m_EditView[i].OnRecalcPropPoint();
		m_EditView[i].Invalidate(FALSE);
	}
}

void CPointEditDlg::OnPropSelectUpdate(int nSx, int nEx)
{
	for(int i = 0; i < 4; i++)
	{
		m_EditView[i].OnSelectPoint(nSx, nEx);
		m_EditView[i].Invalidate(FALSE);
	}
}


// CPointEditDlg 메시지 처리기입니다.

BOOL CPointEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_bQuaterView	= TRUE;
	m_nActiveView	= POINT_EDIT_VIEW_TYPE_FRONT;

	m_bPaneActive	= FALSE;
	m_bSelectActive	= FALSE;
	m_bPointMove	= FALSE;
	m_bWndZoomInOut	= FALSE;

	CalcViewSize();

	m_EditView[0].Create("", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, m_rtView[0], this, IDC_POINTEDIT_EDITVIEW0);
	m_EditView[0].ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_EditView[0].SetViewType(POINT_EDIT_VIEW_TYPE_FRONT);
	m_EditView[0].SetViewName("Front( X - Y )");
	m_EditView[0].SetActive(TRUE);
	m_EditView[0].MoveWindow(m_rtView[0]);
	m_EditView[0].Invalidate(TRUE);

	m_EditView[1].Create("", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, m_rtView[1], this, IDC_POINTEDIT_EDITVIEW1);
	m_EditView[1].ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_EditView[1].SetViewType(POINT_EDIT_VIEW_TYPE_TOP);
	m_EditView[1].SetViewName("Top( X - Z )");
	m_EditView[1].MoveWindow(m_rtView[1]);
	m_EditView[1].Invalidate(TRUE);

	m_EditView[2].Create("", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, m_rtView[2], this, IDC_POINTEDIT_EDITVIEW2);
	m_EditView[2].ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_EditView[2].SetViewType(POINT_EDIT_VIEW_TYPE_SIDE);
	m_EditView[2].SetViewName("Side( Z - Y )");
	m_EditView[2].MoveWindow(m_rtView[2]);
	m_EditView[2].Invalidate(TRUE);

	m_EditView[3].Create("", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, m_rtView[3], this, IDC_POINTEDIT_EDITVIEW3);
	m_EditView[3].ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_EditView[3].SetViewType(POINT_EDIT_VIEW_TYPE_RESERVED);
	m_EditView[3].SetViewName("Reserved");
	m_EditView[3].MoveWindow(m_rtView[3]);
	m_EditView[3].Invalidate(TRUE);

	CRect rtClient;
	GetClientRect(rtClient);
	CalcControlPosition(rtClient.right, rtClient.bottom);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPointEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (m_EditView[0].GetSafeHwnd())
	{
		CalcViewSize();

		if(m_bQuaterView)
		{
			m_EditView[0].MoveWindow(m_rtView[0]);
			m_EditView[1].MoveWindow(m_rtView[1]);
			m_EditView[2].MoveWindow(m_rtView[2]);
			m_EditView[3].MoveWindow(m_rtView[3]);
		}
		else
		{
			m_EditView[m_nActiveView].MoveWindow(m_rtFullView);
		}

		CalcControlPosition(cx, cy);
	}
}


void CPointEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( !(nFlags & MK_CONTROL) )
	{
		int nSelectView = SelectView(point);
		if(nSelectView >= 0)
		{
			m_bSelectActive = TRUE;
			m_ptSelectPoint = point;
		}
		else
		{
			if(m_rtWndZoomInOut.PtInRect(point))
			{
				m_bWndZoomInOut		= TRUE;
				m_ptOldWndZoomInOut = point;
				SetCapture();
			}
		}
	}

	if(nFlags & MK_CONTROL)
	{
		m_bPointMove = TRUE;
		m_ptOldPointMove = point;
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}

void CPointEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bSelectActive)
	{
		std::list<int> listSelPoint;
		CRect rtSelect = CalcSelectDrag(point);
		m_EditView[m_nActiveView].OnSelectPoint(rtSelect, listSelPoint);
		m_EditView[m_nActiveView].OnSelectCursor(rtSelect);
		m_EditView[m_nActiveView].OnSelectCursorClear();

		int nSx = -1, nEx = -1;

		if(listSelPoint.size() > 0)
		{
			std::list<int>::iterator itbegin = listSelPoint.begin();
			std::list<int>::iterator itend = listSelPoint.end();
			--itend;

			nSx = (*itbegin);
			nEx = (*itend);
		}

		for(int i = 0; i < 4; i++)
		{
			if(i != m_nActiveView)
				m_EditView[i]. OnSelectPoint(nSx, nEx+1);

			m_EditView[i].Invalidate(FALSE);
		}

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CControlPane *pControlPane = pMainFrame->GetControlPane();

		CControlPointView *pCtrlPointView = pControlPane->GetControlPointView();
		pCtrlPointView->SetSelectPoint(nSx, nEx+1);
		pCtrlPointView->Invalidate(TRUE);
	}

	if(m_bWndZoomInOut)
		ReleaseCapture();

	m_bSelectActive = FALSE;
	m_bPointMove	= FALSE;
	m_bWndZoomInOut	= FALSE;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonUp(nFlags, point);
}


void CPointEditDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_bQuaterView)
	{
		for(int i = 0; i < 4; i++)
		{
			if(i == m_nActiveView)
				m_EditView[i].SetActive(TRUE);

			m_EditView[i].MoveWindow(m_rtView[i]);
			m_EditView[i].ShowWindow(SW_SHOW);
			m_EditView[i].Invalidate(TRUE);
		}

		m_bQuaterView	= TRUE;
		m_nActiveView	= 0;

		Invalidate(TRUE);
		return;
	}

	int nSelIdx = -1;
	for(int i = 0; i < 4; i++)
	{
		if(m_rtView[i].PtInRect(point))
		{
			nSelIdx			= i;
			m_bQuaterView	= FALSE;
			m_nActiveView	= i;
			break;
		}
	}

	if(nSelIdx >= 0)
	{
		for(int i = 0; i < 4; i++)
		{
			if(i == nSelIdx)
			{
				m_EditView[i].SetActive(TRUE);
				m_EditView[i].MoveWindow(m_rtFullView);
				m_EditView[i].ShowWindow(SW_SHOW);
				m_EditView[i].Invalidate(TRUE);
			}
			else
			{
				m_EditView[i].SetActive(FALSE);
				m_EditView[i].ShowWindow(SW_HIDE);
			}
		}

		Invalidate(TRUE);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDblClk(nFlags, point);
}


BOOL CPointEditDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta > 0)
		m_EditView[m_nActiveView].DecScale();
	else
		m_EditView[m_nActiveView].IncScale();

	m_EditView[m_nActiveView].Invalidate(FALSE);

	float fScale = m_EditView[m_nActiveView].GetScale();
	VisibleScale(fScale);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CPointEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bPaneActive)
	{
		CPoint ptDelta = point - m_ptOldPane;

		m_EditView[m_nActiveView].OnPaneMove(ptDelta.x, ptDelta.y);

		m_ptOldPane = point;

		m_EditView[m_nActiveView].Invalidate(FALSE);
	}

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
	else
	{
		if(m_bSelectActive)
		{
			CRect rtSelect = CalcSelectDrag(point);
			m_EditView[m_nActiveView].OnSelectCursor(rtSelect);
			m_EditView[m_nActiveView].Invalidate(FALSE);
		}
		else
		{
			if(nFlags & MK_CONTROL && m_bPointMove)
			{
				CPoint ptDelta = m_ptOldPointMove - point;
				m_EditView[m_nActiveView].OnPropPointMove(ptDelta.x, ptDelta.y);
				for(int i = 0; i < 4; i++)
				{
					if(i != m_nActiveView)
						m_EditView[i].OnRecalcPropPoint();

					m_EditView[i].Invalidate(FALSE);
				}
				
				m_ptOldPointMove = point;
			}
			else
			{
				if(m_bQuaterView)
				{
					for(int i = 0; i < 4; i++)
					{
						if(m_rtView[i].PtInRect(point))
						{
							POINT ptMouseScreen;
							ptMouseScreen.x = point.x;
							ptMouseScreen.y = point.y;

							POINT ptViewScreen;
							ptViewScreen.x = m_rtView[i].left;
							ptViewScreen.y = m_rtView[i].top;
							
							ClientToScreen(&ptMouseScreen);
							ClientToScreen(&ptViewScreen);

							m_EditView[i].OnFocusPropPoint(CPoint(ptMouseScreen.x - ptViewScreen.x, ptMouseScreen.y - ptViewScreen.y));
						}
					}
				}
				else
				{
					if(m_rtFullView.PtInRect(point))
					{
						POINT ptMouseScreen;
						ptMouseScreen.x = point.x;
						ptMouseScreen.y = point.y;

						POINT ptViewScreen;
						ptViewScreen.x = m_rtFullView.left;
						ptViewScreen.y = m_rtFullView.top;

						ClientToScreen(&ptMouseScreen);
						ClientToScreen(&ptViewScreen);

						m_EditView[m_nActiveView].OnFocusPropPoint(CPoint(ptMouseScreen.x - ptViewScreen.x, ptMouseScreen.y - ptViewScreen.y));
					}
				}
			}
		}
	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnMouseMove(nFlags, point);
}

void CPointEditDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	int nSelIdx = SelectView(point);

	if(nSelIdx >= 0)
	{
		m_bPaneActive	= TRUE;
		m_ptOldPane		= point;

		SetCapture();
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnRButtonDown(nFlags, point);
}

void CPointEditDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(m_bPaneActive)
		ReleaseCapture();

	m_bPaneActive = FALSE;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnRButtonUp(nFlags, point);
}

void CPointEditDlg::OnBnClickedPointeditCenterMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_EditView[m_nActiveView].OnPaneCenter();
	m_EditView[m_nActiveView].Invalidate(FALSE);
}

void CPointEditDlg::OnBnClickedPointeditSearchMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_EditView[m_nActiveView].OnPaneSerachPoint(0);
	m_EditView[m_nActiveView].Invalidate(FALSE);
}

void CPointEditDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(bShow)
	{
		OnPropUpdate();
	}
}

void CPointEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	dc.FillSolidRect(m_rtWndZoomInOut, RGB(170, 60, 60));
}

