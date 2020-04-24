// MainFrm.h : interface of the CMainFrame class
//

#pragma once



class CMainFrame : public CFrameWnd 
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:	
	CSplitterWnd m_wndColSplitter;
	CSplitterWnd m_wndRowSplitter;	
    CSplitterWnd m_wndRowSplitter2;
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;
    BOOL        m_bCreate;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	HWND GetViewWindow();				///< 중앙의 Viewer Window의 핸들을 반환한다.
	HWND GetTabWindow();				///< 왼쪽 Tab Window의 핸들을 반환한다.
    HWND GetRightWindow();              ///< 오른쪽 프로퍼티 Window의 핸들을 반환한다.
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


