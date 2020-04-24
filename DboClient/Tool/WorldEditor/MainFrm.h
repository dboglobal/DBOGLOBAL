// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "CEditorMainControlBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성
protected:

	CEditorMainControlBar			m_cEditControlBar;

// 작업
public:

	void ControllBarInitialize(void);

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

public:

	static void			SetActiveWnd( CWnd * pActiveWnd ) { CMainFrame::m_pActiveWnd = pActiveWnd; }
	static CWnd *		GetActiveWnd( void ) { return CMainFrame::m_pActiveWnd; }
	static CWnd *		m_pActiveWnd; 

	bool				SetStatusText( TCHAR * lptText );

	void				ShowPropertyDialogBarWindow( bool bShow );

	virtual void ActivateFrame(int nCmdShow = -1);
	afx_msg void OnDestroy();
};