// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ModelTool.h"
#include "RightToolView.h"
#include "LeftTreeView.h"
#include "BottomToolView.h"
#include "AnimToolView.h"
#include "ViewerView.h"
#include "MTDef.h"
#include "AnimAttrView.h"

#include "MainFrm.h"

#define WIDTH_LEFTVIEW       200
#define WIDTH_RIGHTVIEW      345
#define HEIGHT_ANIMTOOLVIEW  55
#define HEIGHT_BOTTOMTOOLVIEW 220
#define HEIGHT_ANIMSETVIEW   470


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainFrame construction/destruction

// 미니 덤프 선언(전역변수로 선언만 하면 끝)
MiniDumper g_MiniDump(TRUE);

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
    m_bCreate = FALSE;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

    // 툴바 속성 
    m_wndToolBar.SetButtonStyle(0, TBBS_CHECKBOX);  
    m_wndToolBar.SetButtonStyle(1, TBBS_CHECKBOX);
    m_wndToolBar.SetButtonStyle(2, TBBS_CHECKBOX);

    // 버젼을 윈도우에 표시한다.
    SetWindowText(MT_WINDOW_NAME);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	RECT rc;
	GetClientRect(&rc);

    int nWidthViewer = rc.right - rc.left - WIDTH_LEFTVIEW - WIDTH_RIGHTVIEW;
    int nHeightViewer = rc.bottom - rc.top - HEIGHT_BOTTOMTOOLVIEW - HEIGHT_ANIMTOOLVIEW;

	m_wndColSplitter.CreateStatic(this, 1, 3);	
	m_wndColSplitter.SetColumnInfo(0, WIDTH_LEFTVIEW, 10);
	m_wndColSplitter.SetColumnInfo(1, nWidthViewer  , 10);
	m_wndColSplitter.SetColumnInfo(2, WIDTH_RIGHTVIEW, 10);

	m_wndColSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftTreeView), CSize(WIDTH_LEFTVIEW, rc.bottom), pContext);	

	m_wndRowSplitter.CreateStatic(&m_wndColSplitter, 3, 1, WS_CHILD | WS_VISIBLE, m_wndColSplitter.IdFromRowCol(0, 1));	
	m_wndRowSplitter.CreateView(0, 0, RUNTIME_CLASS(CViewerView), CSize(nWidthViewer, nHeightViewer), pContext);
	m_wndRowSplitter.CreateView(1, 0, RUNTIME_CLASS(CAnimToolView), CSize(nWidthViewer, HEIGHT_ANIMTOOLVIEW), pContext);
	m_wndRowSplitter.CreateView(2, 0, RUNTIME_CLASS(CBottomToolView), CSize(nWidthViewer, HEIGHT_BOTTOMTOOLVIEW), pContext);

	//m_wndColSplitter.CreateView(0, 2, RUNTIME_CLASS(CRightToolView), CSize(WIDTH_RIGHTVIEW, rc.bottom), pContext);	
    m_wndRowSplitter2.CreateStatic(&m_wndColSplitter, 2, 1, WS_CHILD + WS_VISIBLE, m_wndColSplitter.IdFromRowCol(0, 2));
    m_wndRowSplitter2.CreateView(0, 0, RUNTIME_CLASS(CRightToolView), CSize(WIDTH_RIGHTVIEW, HEIGHT_ANIMSETVIEW), pContext);
    m_wndRowSplitter2.CreateView(1, 0, RUNTIME_CLASS(CAnimAttrView), CSize(WIDTH_RIGHTVIEW, rc.bottom - rc.top - HEIGHT_ANIMSETVIEW), pContext);

    m_bCreate = TRUE;
	return TRUE;	
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// 윈도우 크기 설정 
	cs.cx = 1024;
	cs.cy = 768;	
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZE ;    

	return TRUE;
}

HWND CMainFrame::GetViewWindow()
{
	CWnd* pTempWind = m_wndRowSplitter.GetPane(0,0);
	
	return pTempWind->GetSafeHwnd();	
}

HWND CMainFrame::GetTabWindow()
{
	CWnd* pTempWind = m_wndColSplitter.GetPane(0, 0);
	return pTempWind->GetSafeHwnd();
}

HWND CMainFrame::GetRightWindow()
{
    CWnd* pTempWind = m_wndColSplitter.GetPane(0, 2);
    return pTempWind->GetSafeHwnd();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    if(m_bCreate && cx > 0 && cy > 0)
    {
        int nViewerWidth = cx - WIDTH_LEFTVIEW - WIDTH_RIGHTVIEW;
        int nViewerHeight = cy - HEIGHT_ANIMTOOLVIEW - HEIGHT_BOTTOMTOOLVIEW;

        m_wndColSplitter.SetColumnInfo(0, WIDTH_LEFTVIEW, 10);        
        m_wndColSplitter.SetColumnInfo(1, nViewerWidth, 10);
        m_wndColSplitter.SetColumnInfo(2, WIDTH_RIGHTVIEW, 10);

        m_wndRowSplitter.SetRowInfo(0, nViewerHeight, 10);
    }

    CFrameWnd::OnSize(nType, cx, cy);
}


