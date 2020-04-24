// VenusView.h : iCVenusView 클래스의 인터페이스
//


#pragma once


class CVenusView : public CView
{
protected: // serialization에서만 만들어집니다.
	CVenusView();
	DECLARE_DYNCREATE(CVenusView)

// 특성
public:
	CVenusDoc* GetDocument() const;

// 작업
public:

	POINT		m_PointButtonDown;

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
	virtual ~CVenusView();
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
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnInitCamera();
	afx_msg void OnFillmodePoint();
	afx_msg void OnUpdateFillmodePoint(CCmdUI *pCmdUI);
	afx_msg void OnFillmodeWireframe();
	afx_msg void OnUpdateFillmodeWireframe(CCmdUI *pCmdUI);
	afx_msg void OnFillmodeSolid();
	afx_msg void OnUpdateFillmodeSolid(CCmdUI *pCmdUI);
    afx_msg void OnCameraFrontcamera();
    afx_msg void OnCameraLeftcamera();
    afx_msg void OnCameraRightcamera();
    afx_msg void OnCameraviewpositionBackcamera();
    afx_msg void OnCameraviewpositionUpcamera();
    afx_msg void OnCameraviewpositionDowncamera();
    afx_msg void OnViewLowspeceffect();
    afx_msg void OnUpdateViewLowspeceffect(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // VenusView.cpp의 디버그 버전
inline CVenusDoc* CVenusView::GetDocument() const
   { return reinterpret_cast<CVenusDoc*>(m_pDocument); }
#endif

extern CVenusView* g_pMainView;

inline CVenusView* gMainView()
{
//	N3ASSERT(g_pMainView);
	return g_pMainView;
//	CHephaistosFrame* pMainfrm = dynamic_cast<CHephaistosFrame*>(AfxGetMainWnd());
//	N3ASSERT(pMainfrm);
//	CHephaistosView* pMainView = dynamic_cast<CHephaistosView*>(pMainfrm->GetV>GetActiveView());
//	N3ASSERT(pMainView);
//	return pMainView;
}

