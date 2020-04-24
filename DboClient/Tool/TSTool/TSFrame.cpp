#include "stdafx.h"
#include "TSTool.h"

#include "TSFrame.h"
#include "TSProjectMainMenuForm.h"
#include "TSQuestTextForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
//	CTSFrame
//
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CTSFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CTSFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_PROJECT_MAIN_MENU, &CTSFrame::OnViewProjectMainMenu)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROJECT_MAIN_MENU, &CTSFrame::OnUpdateViewProjectMainMenu)
	ON_COMMAND(ID_VIEW_QUEST_TEXT, &CTSFrame::OnViewQuestText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_QUEST_TEXT, &CTSFrame::OnUpdateViewQuestText)
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
};


CTSFrame::CTSFrame( void )
{
}

CTSFrame::~CTSFrame( void )
{
}

BOOL CTSFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CFrameWnd::PreCreateWindow( cs ) )
		return FALSE;

	return TRUE;
}

void CTSFrame::UpdateStatusBar( CString& strTriggerID, CString& strTitle, CString& strRepeat, CString& strShare )
{
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_ID, strTriggerID );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_TITLE, strTitle );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_REPEAT, strRepeat );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_SHARE, strShare );
}

int CTSFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 )
		return -1;

	// 기본 툴바
	if ( !m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		 | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		 !m_wndToolBar.LoadToolBar(IDR_MAINFRAME) )
	{
		TRACE0( "도구 모음을 만들지 못했습니다.\n" );
		return -1;
	}

	// 컨테이너 툴바
	if ( !m_wndContToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		 | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		 !m_wndContToolBar.LoadToolBar(IDR_CONTAINERBAR) )
	{
		TRACE0( "도구 모음을 만들지 못했습니다.\n" );
		return -1;
	}

	// Project main menu 생성
	if ( !m_wndProjMainMenuBar.Create( this,
									   RUNTIME_CLASS(CTSProjectMainMenuForm),
									   (CCreateContext*)(lpCreateStruct->lpCreateParams),
									   _T("Project main menu"),
									   WS_CHILD|WS_VISIBLE|CBRS_TOP,
									   ID_VIEW_PROJECT_MAIN_MENU ) )
	{
		TRACE0( "프로젝트 메인 메뉴 생성 실패.\n" );
		return -1;
	}
	m_wndProjMainMenuBar.SetBarStyle( m_wndProjMainMenuBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC );

	// Quest text bar 생성
	if ( !m_wndQuestTextBar.Create( this,
									RUNTIME_CLASS(CTSQuestTextForm),
									(CCreateContext*)(lpCreateStruct->lpCreateParams),
									_T("Quest text"),
									WS_CHILD|CBRS_TOP,
									ID_VIEW_QUEST_TEXT ) )
	{
		TRACE0( "프로젝트 메인 메뉴 생성 실패.\n" );
		return -1;
	}
	m_wndQuestTextBar.SetBarStyle( m_wndQuestTextBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC );

	if ( !m_wndStatusBar.Create( this ) ||
		 !m_wndStatusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT)) )
	{
		TRACE0( "상태 표시줄을 만들지 못했습니다.\n" );
		return -1;
	}

	// Trigger id
	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_ID_STR, 100, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_ID_STR, "Trigger ID" );
	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_ID, 101, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_ID, "Invalid" );

	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_TITLE_STR, 102, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_TITLE_STR, "Trigger title" );
	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_TITLE, 103, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_TITLE, "Invalid" );

	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_REPEAT_STR, 104, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_REPEAT_STR, "Repeat quest" );
	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_REPEAT, 105, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_REPEAT, "Invalid" );

	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_SHARE_STR, 106, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_SHARE_STR, "Share quest" );
	m_wndStatusBar.SetPaneInfo( ePAN_INFO_TRIGGER_SHARE, 107, 0, 100 );
	m_wndStatusBar.SetPaneText( ePAN_INFO_TRIGGER_SHARE, "Invalid" );

	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndContToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndProjMainMenuBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndQuestTextBar.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &m_wndToolBar );
	DockControlBar( &m_wndProjMainMenuBar, AFX_IDW_DOCKBAR_LEFT );
	DockControlBar( &m_wndContToolBar, AFX_IDW_DOCKBAR_LEFT );
	DockControlBar( &m_wndQuestTextBar, AFX_IDW_DOCKBAR_LEFT );

	return 0;
}

void CTSFrame::OnViewProjectMainMenu()
{
	OnBarCheck( ID_VIEW_PROJECT_MAIN_MENU );
}

void CTSFrame::OnUpdateViewProjectMainMenu(CCmdUI *pCmdUI)
{
	CControlBar* pBar = GetControlBar( ID_VIEW_PROJECT_MAIN_MENU );
	if ( NULL == pBar )
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		pCmdUI->SetCheck( pBar->IsVisible() ? 1 : 0 );
	}
}

void CTSFrame::OnViewQuestText()
{
	OnBarCheck( ID_VIEW_QUEST_TEXT );
}

void CTSFrame::OnUpdateViewQuestText(CCmdUI *pCmdUI)
{
	CControlBar* pBar = GetControlBar( ID_VIEW_QUEST_TEXT );
	if ( NULL == pBar )
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		pCmdUI->SetCheck( pBar->IsVisible() ? 1 : 0 );
	}
}

#ifdef _DEBUG
void CTSFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CTSFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
