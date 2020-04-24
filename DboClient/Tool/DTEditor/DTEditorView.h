// DTEditorView.h : interface of the CDTEditorView class
//


#pragma once

#include "NtlSLDef.h"

#define NTL_ZOOM_INOUT_SENSITIVITY		0.03f
#define NTL_MOUSE_SENSITIVITY_SCALE		0.5f
#define NTL_HEADING_UPDATE_TIME			1.0f

enum ECameraRotateFlag
{
	CAMERA_ROTATE_FLAG_NONE,	
	CAMERA_ROTATE_FLAG_ADJUST,	
	CAMERA_ROTATE_FLAG_ACCORD,	
};

class CDTEditorView : public CView
{
protected: // create from serialization only
	CDTEditorView();
	DECLARE_DYNCREATE(CDTEditorView)

// Attributes
public:
	CDTEditorDoc* GetDocument() const;

// Operations
private:

	void	ViewerModeMouseLBtnDown(CPoint point);

	void	ViewerModeMouseLBtnUp(UINT nFlags, CPoint point);
	void	SplineModeMouseLBtnUp(CPoint point);

	void	ViewerModeMouseRBtnDown(CPoint point);

	void	ViewerModeMouseRBtnUp(CPoint point);
	void	SplineModeMouseRBtnUp(CPoint point);

	void	ViewerModeMouseMove(UINT nFlags, CPoint point);
	void	SplineModeMouseMove(UINT nFlags, CPoint point);

	void	ViewerModeMouseMoveLBtn(CPoint point);
	void	ViewerModeMouseMoveRBtn(CPoint point);
	void	ViewerModeMouseMoveNotBtn(CPoint point);


	void	Picking(CPoint point);

	int		GetFocusControlPointIndex(CPoint point, FLOAT& fDistance);

	void	CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY);
	void	CalcCameraRotateToCharBack(void);
	void	UpdateCameraRotateToCharBack(RwReal fElapsed);
	void	CreateMoveMark(const RwV3d *pPos);
	void	DeleteMoveMark(void);
	void	UpdateMoveMark(RwReal fElapsed);

public:

	

	RwReal			m_fMoveMarkTime;
	CNtlPLEntity	*m_pMoveMark;

	CPoint			m_ptPrev;

	DWORD			m_dwLBtnDownTime;
	DWORD			m_dwRBtnDownTime;

	ECameraRotateFlag m_eRotFlag;

	RwBool			m_bLButtonMoveFlag;
	RwInt32			m_iOldLButtonMouseX;
	RwInt32			m_iOldLButtonMouseY;
	
	RwBool			m_bRButtonMoveFlag;
	RwInt32			m_iOldRButtonMouseX;
	RwInt32			m_iOldRButtonMouseY;

	RwBool			m_bChangeHeadingFlag;

	RwInt32			m_iMouseX;
	RwInt32			m_iMouseY;

public:

	
	void			Update(RwReal fElapsed);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDTEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in DTEditorView.cpp
inline CDTEditorDoc* CDTEditorView::GetDocument() const
   { return reinterpret_cast<CDTEditorDoc*>(m_pDocument); }
#endif
