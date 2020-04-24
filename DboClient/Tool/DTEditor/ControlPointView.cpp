// ControlPointView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "ControlPointView.h"

#include "resource.h"
#include "DTEditorGlobal.h"
#include "PointEditDlg.h"
#include "PointAttrDlg.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlCameraManager.h"

#define RULERSCALE			15

#define RULERS_LINE_Y		5

#define RULERSCALE_START_X	5
#define RULERSCALE_START_Y	3

#define RULERNUMBER_START_Y	10

// CControlPointView

IMPLEMENT_DYNCREATE(CControlPointView, CFormView)

CControlPointView::CControlPointView()
	: CFormView(CControlPointView::IDD)
{
	m_hbm		= NULL;
	m_hbmMask	= NULL;
}

CControlPointView::~CControlPointView()
{
}

void CControlPointView::LoadSequencdBitmap()
{
	m_hbmMask = ::LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_RULER_DOM), RT_BITMAP),
							MAKEINTRESOURCE(IDB_RULER_DOM));
	ASSERT(m_hbmMask != NULL);

	LPCTSTR lpszResourceName = MAKEINTRESOURCE(IDB_RULER_DO);

	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_BITMAP);

	m_hbm = AfxLoadSysColorBitmap(hInst, hRsrc);

	BITMAP bm;
	::GetObject(m_hbm, sizeof(BITMAP), &bm);
	m_bmSize = CSize(bm.bmWidth, bm.bmHeight);
}

void CControlPointView::DrawFace(CDC& dc)
{
	dc.SaveDC();

	CRect rt3D = m_rtPointRuler;
	dc.FillSolidRect(rt3D, RGB(210, 210, 210));
	dc.Draw3dRect(rt3D, XTPPaintManager()->GetXtremeColor(XPCOLOR_HIGHLIGHT), RGB(30, 30, 30));
	
	dc.SelectObject(&m_penShadow);
	
	for(int i = 0; i < m_nRulerScaleNum; ++i)
	{
		dc.MoveTo(RULERSCALE_START_X+i*RULERSCALE, RULERSCALE_START_Y);
		dc.LineTo(RULERSCALE_START_X+i*RULERSCALE, RULERSCALE_START_Y + RULERS_LINE_Y);

		dc.MoveTo(RULERSCALE_START_X+i*RULERSCALE, rt3D.bottom-RULERSCALE_START_Y);
		dc.LineTo(RULERSCALE_START_X+i*RULERSCALE, rt3D.bottom-RULERSCALE_START_Y - RULERS_LINE_Y);
	}

	// 숫자 찍기
	CString str;
	dc.SelectObject(m_penRulerNumber);
	dc.SetBkMode(TRANSPARENT);
	for(int i = 0; i < m_nRulerScaleNum; ++i)
	{
		if(i+1 == 1 || (i+1)%5 == 0)
		{
			str.Format("%d", i+1);
			
			dc.TextOut(RULERSCALE_START_X+i*RULERSCALE, RULERNUMBER_START_Y, str);
		}
	}

	// point sel

	rt3D = m_rtPointRulerSel;
	dc.FillSolidRect(rt3D, RGB(228, 228, 228));
	if(m_nRulerSelNum < m_nRulerScaleNum)
	{
		rt3D.right = m_nRulerSelNum*RULERSCALE+RULERSCALE_START_X;
		dc.FillSolidRect(rt3D, RGB(237, 209, 235));
	}

	rt3D = m_rtPointRulerSel;
	dc.Draw3dRect(rt3D, RGB(201, 201, 201), RGB(201, 201, 201));

	dc.SelectObject(m_penRulerSel);

	for(int i = 0; i < m_nRulerScaleNum; ++i)
	{
		dc.MoveTo(RULERSCALE_START_X+i*RULERSCALE, rt3D.top);
		dc.LineTo(RULERSCALE_START_X+i*RULERSCALE, rt3D.bottom);
	}

	dc.RestoreDC(-1);
}

void CControlPointView::DrawSel(CDC& dc)
{
	if(m_nSelSx < 0 || m_nSelEx < 0)
		return;

	CRect rt3D;
	rt3D.left	= m_rtPointRulerSel.left+RULERSCALE_START_X+m_nSelSx*RULERSCALE+2;
	rt3D.top	= m_rtPointRulerSel.top+2;
	rt3D.right	= m_rtPointRulerSel.left+RULERSCALE_START_X+m_nSelEx*RULERSCALE-2;
	rt3D.bottom = m_rtPointRulerSel.bottom-2;

	dc.FillSolidRect(rt3D, RGB(122, 194, 146));
	dc.Draw3dRect(rt3D, RGB(30, 30, 30), RGB(30, 30, 30));
}

void CControlPointView::DrawSequenceMark(CDC& dc, int nPointIdx)
{
	CDC dcBitmap;
	dcBitmap.CreateCompatibleDC(&dc);

	CPoint ptPos = GetRulerSequenceMarkPosition(nPointIdx);

	HGDIOBJ hbm = ::SelectObject(dcBitmap.m_hDC, m_hbmMask);
	dc.BitBlt(ptPos.x - m_bmSize.cx/2, ptPos.y, m_bmSize.cx, m_bmSize.cy, &dcBitmap, 0, 0, SRCAND);
	
	::SelectObject(dcBitmap.m_hDC, m_hbm);
	dc.BitBlt(ptPos.x - m_bmSize.cx/2, ptPos.y, m_bmSize.cx, m_bmSize.cy, &dcBitmap, 0, 0, SRCINVERT);

	::SelectObject(dcBitmap.m_hDC, hbm);
}

void CControlPointView::DrawSequenceMark(CDC& dc)
{
	if(m_pSplineProp != NULL)
	{
		int nCtrlNum = GetSequenceMarkNum();
		const SSplineCtrlPt *pCtrlPt = m_pDTSpilne->GetSplineControlPoint();

		RwReal fTime = pCtrlPt[0].fTime;
		
		for(int i = 1; i < nCtrlNum; ++i)
		{
			if(fabs(pCtrlPt[i].fTime - fTime) > 0.0001f)
			{
				fTime = pCtrlPt[i].fTime;
				DrawSequenceMark(dc, i);
			}
		}
	}

	DrawSequenceMark(dc, m_nRulerSelNum-1);
}

void CControlPointView::SetSelectPointStart(int nSx)
{
	m_nSelSx = nSx;

	SetActiveSelectControlPoint(m_nSelSx, m_nSelEx);
}

void CControlPointView::SetSelectPointEnd(int nEx)
{
	m_nSelEx = nEx;

	SetActiveSelectControlPoint(m_nSelSx, m_nSelEx);
}

void CControlPointView::SetSelectPoint(int nSx, int nEx)
{
	m_nSelSx = nSx;
	m_nSelEx = nEx;

	SetActiveSelectControlPoint(m_nSelSx, m_nSelEx);
}

int CControlPointView::GetSequenceMarkNum(void)
{
	if(m_pSplineProp == NULL)
		return 0;

	return m_pSplineProp->iCtrlNum;
}

CPoint CControlPointView::GetRulerSequenceMarkPosition(int nPointIdx)
{
	CPoint ptPos(RULERSCALE_START_X+nPointIdx*RULERSCALE, m_rtSeqMark.top);

	return ptPos;
}

BOOL CControlPointView::PossibleControlPointTime()
{
	if(m_pDTSpilne == NULL)
		return FALSE;

	if(m_nSelSx == -1 || m_nSelEx == -1)
		return FALSE;

	return TRUE;
}

void CControlPointView::ChangeControlPointTime(void)
{
	if(!PossibleControlPointTime())
		return;

	RwReal fSumTime = m_pDTSpilne->GetSumPointTime(m_nSelSx, m_nSelEx);
	RwReal fTime = fSumTime/(m_nSelEx-m_nSelSx)/NtlRandomNumber(1.0f, 3.0f);

	m_pDTSpilne->ChangeControlPointTime(m_nSelSx, m_nSelEx, fTime);

	Invalidate(TRUE);
}

void CControlPointView::PointClick(int nSx, int nEx)
{
	SetSelectPoint(nSx, nEx);
	InvalidateRect(m_rtPointRulerSel, FALSE);
}

void CControlPointView::ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData)
{
	m_pDTSpilne		= (CNtlDTSpline*)pDTProp;
	m_pSplineProp	= (SSplineProp*)pPropData;

	if(m_pDTSpilne)
	{
		m_nRulerSelNum = GetSequenceMarkNum();
	}

	SetSelectPoint(-1, -1);

	Invalidate(TRUE);
}

void CControlPointView::DeActiveSplineProp(void)
{
	m_pDTSpilne		= NULL;
	m_pSplineProp	= NULL;
	m_nRulerSelNum  = m_nRulerScaleNum;
	SetSelectPoint(-1, -1);
	Invalidate(TRUE);
}

void CControlPointView::DeleteSplineProp(CNtlDTProp *pDTProp)
{
	if(m_pDTSpilne == pDTProp)
	{
		m_pDTSpilne		= NULL;
		m_pSplineProp	= NULL;
		m_nRulerSelNum  = m_nRulerScaleNum;

		SetSelectPoint(-1, -1);

		Invalidate(TRUE);
	}
}

void CControlPointView::RebuildProp()
{
	if(m_pDTSpilne)
	{
		m_nRulerSelNum = GetSequenceMarkNum();
	}

	SetSelectPoint(-1, -1);

	Invalidate(TRUE);
}

void CControlPointView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlPointView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CControlPointView 진단입니다.

#ifdef _DEBUG
void CControlPointView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlPointView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlPointView 메시지 처리기입니다.

int CControlPointView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	SetSelectPoint(-1, -1);
	m_bLBtnDown = FALSE;

	m_rtPointRuler.SetRect(0, 0, 100, 40);
	m_rtSeqMark.SetRect(0, 50, 100, 20);
	m_rtPointRulerSel.SetRect(0, 60, 100, 90);

	m_penShadow.CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));
	m_penRulerNumber.CreatePen(PS_SOLID, 0, RGB(148, 166, 250));
	m_penRulerSel.CreatePen(PS_SOLID, 0, RGB(201, 201, 201));
	LoadSequencdBitmap();

	m_pDTSpilne		= NULL;
	m_pSplineProp	= NULL;	
	
	return 0;
}

void CControlPointView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	m_rtPointRulerSel.right = cx;
	m_rtPointRuler.right	= cx;

	m_nRulerScaleNum	= m_rtPointRuler.Width() / RULERSCALE;
	m_nRulerSelNum		= m_nRulerScaleNum;

	if(m_pSplineProp)
		m_nRulerSelNum = GetSequenceMarkNum();
}

void CControlPointView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_rtPointRulerSel.PtInRect(point))
	{
		SetSelectPointStart((point.x - RULERSCALE_START_X)/RULERSCALE);
		
		if(m_pSplineProp)
		{
			if(m_nSelSx < GetSequenceMarkNum())
			{
				SetSelectPointEnd(m_nSelSx+1);
				m_bLBtnDown = TRUE;
				SetCapture();
			}
			else
			{
				SetSelectPoint(-1, -1);
			}
		}
		else
		{
			SetSelectPointEnd(m_nSelSx+1);
			m_bLBtnDown = TRUE;
			SetCapture();
		}
	}
	else
	{
		SetSelectPoint(-1, -1);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnLButtonDown(nFlags, point);
}

void CControlPointView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_nSelSx > m_nSelEx)
	{
		int nTemp = m_nSelSx;
		SetSelectPoint(m_nSelEx, nTemp);
	}

	if(m_bLBtnDown)
	{
		ReleaseCapture();
	}

	m_bLBtnDown = FALSE;
	InvalidateRect(m_rtPointRulerSel, FALSE);

	int nSelSx, nSelEx;
	GetActiveSelectControlPoint(nSelSx, nSelEx);

	// camera setting.
	if(GetEditModeType() == EDIT_MODE_FREE_CAMERA && nSelSx >= 0) 
	{
		CNtlDTProp *pDTProp = GetActiveDTProp();
		if(pDTProp && IsCameraMovePointClick())
		{
			CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;

			RwV3d vCamPos		= pDTSpline->GetSplineControlPointWorldPosition(nSelSx);
			RwV3d vDir			= pDTSpline->GetSplineControlPointRotate(nSelSx);

			RwReal fAngleX, fAngleY;
			CNtlMath::LineToAngleXY(&vDir, fAngleX, fAngleY);

			RwBool bUseWorldPos = pDTSpline->IsUseWorldPosition();
			if(!bUseWorldPos)
			{
				RwV3d vPos = GetSplineStandardPosition();
				RwV3dAdd(&vCamPos, &vCamPos, &vPos); 
			}

			GetNtlGameCameraManager()->SetCameraPos(&vCamPos);   
			GetNtlGameCameraManager()->SetPitch(fAngleX);
			GetNtlGameCameraManager()->SetYaw(fAngleY);
		}
	}

	m_pEditDlg->OnPropSelectUpdate(nSelSx, nSelEx);
	m_pAttrtDlg->OnPropSelectUpdate(nSelSx, nSelEx);
	
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnLButtonUp(nFlags, point);
}

void CControlPointView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bLBtnDown)
	{
		if(point.x >= m_rtPointRulerSel.left && point.x <= m_rtPointRulerSel.right)
		{
			SetSelectPointEnd((point.x - RULERSCALE_START_X)/RULERSCALE+1);
			
			if(m_pSplineProp)
			{
				if(m_nSelEx >= GetSequenceMarkNum())
					SetSelectPointEnd(GetSequenceMarkNum());
			}
		}

		InvalidateRect(m_rtPointRulerSel, FALSE);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnMouseMove(nFlags, point);
}

void CControlPointView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.

	DrawFace(dc);
	DrawSel(dc);
	DrawSequenceMark(dc);
}

void CControlPointView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(m_hbm != NULL)
		::DeleteObject(m_hbm);
	if(m_hbmMask != NULL)
		::DeleteObject(m_hbmMask);

	m_penShadow.DeleteObject();
	m_penRulerNumber.DeleteObject();
	m_penRulerSel.DeleteObject();
}


void CControlPointView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CControlPointView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnKeyUp(nChar, nRepCnt, nFlags);
}
