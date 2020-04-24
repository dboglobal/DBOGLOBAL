// MainFrm.h : CMainFrame 클래스의 인터페이스
//
#pragma once

#include "Control/SizingTabCtrlBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성
public:
	BOOL m_bSplit;					// 창분할완료시 True;
	CSplitterWnd m_wndSplit1;		
	CSplitterWnd m_wndSplit2;	
		
	CSizingTabCtrlBar m_wndLayerBar;

// 작업
public:
	void ResizeSplitWnd();
	void InitCtrlBar();

// 재정의
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 메시지 맵 함수를 생성했습니다.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	virtual void ActivateFrame(int nCmdShow = -1);
};
