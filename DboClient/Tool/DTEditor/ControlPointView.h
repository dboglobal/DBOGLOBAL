#pragma once

#include "afxwin.h"
#include "afxcmn.h"

#include "resource.h"
// CControlPointView 폼 뷰입니다.

class CPointEditDlg;
class CPointAttrDlg;

struct SSplineProp;
class CNtlDTProp;
class CNtlDTSpline;

class CControlPointView : public CFormView
{
	DECLARE_DYNCREATE(CControlPointView)

protected:

	CControlPointView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CControlPointView();

	int		m_nRulerScaleNum;
	int		m_nRulerSelNum;
	int		m_nSelSx;
	int		m_nSelEx;

	BOOL	m_bLBtnDown;
	BOOL	m_bCaptureMouse;

	CRect	m_rtPointRuler;
	CRect	m_rtSeqMark;
	CRect	m_rtPointRulerSel;
	
	CPen	m_penShadow;
	CPen	m_penRulerNumber;
	CPen	m_penRulerSel;
	
	HBITMAP m_hbm;
	HBITMAP m_hbmMask;
	CSize	m_bmSize;			 // size of item in pixels

protected:

	void	LoadSequencdBitmap();
	
	void	DrawFace(CDC& dc);
	void	DrawSel(CDC& dc);
	void	DrawSequenceMark(CDC& dc);
	void	DrawSequenceMark(CDC& dc, int nPointIdx);
	
	int		GetSequenceMarkNum(void);
	CPoint	GetRulerSequenceMarkPosition(int nPointIdx);

public:

	void	SetSelectPointStart(int nSx);
	void	SetSelectPointEnd(int nEx);
	void	SetSelectPoint(int nSx, int nEx);

	BOOL	PossibleControlPointTime();
	void	ChangeControlPointTime(void);
	void	PointClick(int nSx, int nEx);

public:

	CPointEditDlg	*m_pEditDlg;
	CPointAttrDlg	*m_pAttrtDlg;

	CNtlDTSpline	*m_pDTSpilne;
	SSplineProp		*m_pSplineProp;

	void			ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData);
	void			DeActiveSplineProp(void);
	void			DeleteSplineProp(CNtlDTProp *pDTProp);
	void			RebuildProp();

public:
	enum { IDD = IDD_CONTROL_POINT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


