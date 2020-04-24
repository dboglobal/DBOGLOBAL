// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "MainFrm.h"

#include "CEditorApplication.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


CWnd *	CMainFrame::m_pActiveWnd = NULL;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.

	SetActiveWnd( this );
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
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	this->CenterWindow();
	this->ShowWindow(SW_SHOWMAXIMIZED);
	this->UpdateWindow();

    ControllBarInitialize();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

void CMainFrame::ControllBarInitialize(void)
{
	CSize maptoolbarsize;

	maptoolbarsize.cx = 300;
	HDC pDC = ::GetDC(NULL);

	maptoolbarsize.cy = (GetDeviceCaps( pDC, VERTRES) - 240 );
	::ReleaseDC( NULL, pDC );

	m_cEditControlBar.Create( this, maptoolbarsize , IDW_PROP_BAR );	
	m_cEditControlBar.ShowWindow( SW_HIDE );
	m_cEditControlBar.SetBarStyle( m_cEditControlBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_cEditControlBar.AddViewes();
	m_cEditControlBar.EnableDocking( CBRS_ALIGN_RIGHT | CBRS_ALIGN_LEFT ); //| CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM );		
}


#endif //_DEBUG


// CMainFrame 메시지 처리기


void CMainFrame::ActivateFrame(int nCmdShow)
{
	static BOOL bFirsttime = TRUE;
	if(bFirsttime)
	{
		bFirsttime = FALSE;

		RECT viewwndRect;
		GetActiveView()->GetWindowRect(&viewwndRect);

		SIZE maptoolbarsize;
		maptoolbarsize.cx = 300;
		maptoolbarsize.cy = viewwndRect.bottom - viewwndRect.top - 114;

		this->m_cEditControlBar.SetFloatingSize( maptoolbarsize );
		DockControlBar(&m_cEditControlBar, AFX_IDW_DOCKBAR_RIGHT);
		m_cEditControlBar.ShowWindow( SW_NORMAL );

		RecalcLayout( TRUE );
	}

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


bool CMainFrame::SetStatusText( TCHAR * lptText )
{
	return !!m_wndStatusBar.SetPaneText( 0, lptText, true );
}


void CMainFrame::ShowPropertyDialogBarWindow( bool bShow )
{
//	m_cPropertyDialog.Show( bShow );

	RecalcLayout( TRUE );
}


