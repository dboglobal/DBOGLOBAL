// NtlWEView.h : iCNtlWEView 클래스의 인터페이스
//


#pragma once


class CNtlWEView : public CView
{
protected: // serialization에서만 만들어집니다.
	CNtlWEView();
	DECLARE_DYNCREATE(CNtlWEView)

// 특성
public:
	CNtlWEDoc* GetDocument() const;

// 작업
public:

// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현
public:
	virtual ~CNtlWEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

public:
	RwBool IsNMapPropEditingEnable();
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // NtlWEView.cpp의 디버그 버전
inline CNtlWEDoc* CNtlWEView::GetDocument() const
   { return reinterpret_cast<CNtlWEDoc*>(m_pDocument); }
#endif