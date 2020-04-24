// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "CharacterEditor.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#include "AnimationView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
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
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::ControllBarInitialize(void)
{
	CSize maptoolbarsize;

	maptoolbarsize.cx = 300;
	HDC pDC = ::GetDC(NULL);

	maptoolbarsize.cy = GetDeviceCaps( pDC, VERTRES) - 350;
	::ReleaseDC( NULL, pDC );

	m_wndEditBar.Create(this, maptoolbarsize , IDW_PROP_BAR);	
	m_wndEditBar.ShowWindow( SW_HIDE );			
	
	m_wndEditBar.SetBarStyle( m_wndEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndEditBar.AddView("Animation", RUNTIME_CLASS(CAnimationView));
	
	// Add map toolbar palette...
	m_wndEditBar.EnableDocking( CBRS_ALIGN_RIGHT | CBRS_ALIGN_LEFT ); //| CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM );		
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

	
	CenterWindow();
	ShowWindow(SW_SHOW);
	UpdateWindow();
	
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

#endif //_DEBUG


// CMainFrame 메시지 처리기


void CMainFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	static BOOL bFirsttime = TRUE;
	if(bFirsttime)
	{
		bFirsttime = FALSE;

		RECT viewwndRect;
		GetActiveView()->GetWindowRect(&viewwndRect);

		SIZE maptoolbarsize;
		maptoolbarsize.cx = 350;
		maptoolbarsize.cy = viewwndRect.bottom - viewwndRect.top - 114;

		this->m_wndEditBar.SetFloatingSize( maptoolbarsize );
		DockControlBar(&m_wndEditBar, AFX_IDW_DOCKBAR_RIGHT);		
		m_wndEditBar.ShowWindow( SW_NORMAL );
	}

	CFrameWnd::ActivateFrame(nCmdShow);
}
