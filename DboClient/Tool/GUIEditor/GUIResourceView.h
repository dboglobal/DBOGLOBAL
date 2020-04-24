// GUIResourceView.h : iCGUIResourceView 클래스의 인터페이스
//

#pragma once

#include "CDevILMFCMemDC.h"
#include "KMSelectBox.h"
#include "CoordManager.h"

class CGUIEditorDoc;

class CGUIResourceView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CGUIResourceView();
	DECLARE_DYNCREATE(CGUIResourceView)

// 특성
public:
	CGUIEditorDoc* GetDocument() const;
	
// 작업
public:
	//void SSetClientSizeFromImage();

// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현
public:
	virtual ~CGUIResourceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL	LoadImage( CString& strFullPath, CString& strFileName );
	void	SetSelectBox( CRect rcRect );							// 선택 박스 옮김.
	void	ResizeBuffer();
	void	UpdateRectInfo();					
	void	UpdateRsrInfo( CString& strFilename );
	CRect	GetSelectRect() { return m_selectBox.GetCurRect(); }
	void	ShowSelected();

// Variables
protected:
	CKMSelectBox	m_selectBox;	// RectTracker.
	CKMMemDC		m_dcBuffer;		// 더블버퍼링.
	CCoordManager	m_CoordMgr;		// 좌표관리.

	CPoint			m_ptRBtnDown;	// RButtonDown시 좌표.
	
	BOOL			m_bInitialized;	
			
// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // GUIResourceView.cpp의 디버그 버전
inline CGUIEditorDoc* CGUIResourceView::GetDocument() const
   { return reinterpret_cast<CGUIEditorDoc*>(m_pDocument); }
#endif

extern CGUIResourceView* g_pResourceView;

