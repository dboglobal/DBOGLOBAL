// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "2DParticleEditor.h"

#include "PathManager.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	m_pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINFRAME);
	if(m_pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	m_pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!m_pToolBar || !m_pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create main toolbar\n");
		return -1;
	}

	/*m_pOptionTool = (CXTPToolBar*)
		pCommandBars->Add(_T("Option"), xtpBarTop );
	if( !m_pOptionTool || !m_pOptionTool->LoadToolBar(IDR_OPTION))
	{
		TRACE0("Faield to create option toolbar\n");
		return -1;
	}

	DockRightOf( m_pOptionTool, m_pToolBar );*/

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeWhidbey);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// 개발 중에는 커맨드바의 위치를 로드하지 않는다.
	// Load the previous state for toolbars and menus.
	// LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005);
	m_paneManager.GetPaintManager()->GetTabPaintManager()->SetAppearance(xtpTabAppearanceVisualStudio2005);    
	m_paneManager.GetPaintManager()->GetTabPaintManager()->SetOneNoteColors(TRUE);
	m_paneManager.GetPaintManager()->GetTabPaintManager()->m_bBoldSelected = TRUE;
	m_paneManager.GetPaintManager()->GetTabPaintManager()->SetLayout(xtpTabLayoutMultiRow);
	m_paneManager.GetPaintManager()->GetTabPaintManager()->DisableLunaColors(FALSE);    
	m_paneManager.GetPaintManager()->GetTabPaintManager()->SetPosition(xtpTabPositionTop);

	// Create docking panes.
	m_pWndPaneSystem		= m_paneManager.CreatePane( IDR_PANE_SYSTEM, CRect(0, 0, 300, 120), xtpPaneDockRight );
	m_pWndPaneAttribute		= m_paneManager.CreatePane( IDR_PANE_ATTRIBUTE, CRect(0, 0, 300, 120), xtpPaneDockRight );
	m_paneManager.AttachPane( m_pWndPaneAttribute, m_pWndPaneSystem );
	m_pWndPaneProperties	= m_paneManager.CreatePane( IDR_PANE_PROPERTIES, CRect(0, 0, 300, 120), xtpPaneDockBottom, m_pWndPaneAttribute);
	m_pWndPaneInfomation	= m_paneManager.CreatePane( IDR_PANE_INFOMATION, CRect(0, 0, 300, 70), xtpPaneDockTop, m_pWndPaneAttribute );

	m_pWndPaneInfomation->SetOptions( xtpPaneNoHideable );
	m_pWndPaneAttribute->SetOptions( xtpPaneNoHideable );
	m_pWndPaneProperties->SetOptions( xtpPaneNoHideable );
	m_pWndPaneSystem->SetOptions( xtpPaneNoHideable );
		

	//// Set the icons for the docking pane tabs.
	//int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES};
	//	m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
	//	_countof(nIDIcons), RGB(0, 255, 0));

	// 이전 상태의 pane 들의 위치나 상태를 저장하여 로드한다. 
	/*CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
		m_paneManager.SetLayout(&layoutNormal);
	}*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return TRUE;
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


void CMainFrame::OnClose()
{

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CFrameWnd::OnClose();
}


void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the keyboard page to the customize dialog.
		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
		dlg.AddPage(&pageKeyboard);
		pageKeyboard.AddCategories(IDR_MAINFRAME);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_ATTRIBUTE:
				{
					if( m_AttrPane.GetSafeHwnd() == 0 )
					{
						m_AttrPane.Create( _T("STATIC"), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							CRect(0, 0, 0, 0), this, 0 );
					}
					pPane->Attach(&m_AttrPane);
				}
				break;
			case IDR_PANE_PROPERTIES:
				{
					if( m_PropertisPane.GetSafeHwnd() == 0 )
					{
						m_PropertisPane.Create( _T("STATIC"), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							CRect(0, 0, 0, 0), this, 0 );
					}
					pPane->Attach(&m_PropertisPane);
				}
				break;
			case IDR_PANE_SYSTEM:
				{
					pPane->AttachView( this, RUNTIME_CLASS( CSystemPane ) );
				}
				break;
			case IDR_PANE_INFOMATION:
				{
					pPane->AttachView( this, RUNTIME_CLASS( CInfomationPane ) );
				}
				break;
			default:
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}