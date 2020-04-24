// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MrPoPo.h"

#include "MainFrm.h"
#include "MPPDef.h"
#include "MPPConfigSetting.h"
#include "MPPLogView.h"
#include "MPPSetItemPane.h"
#include "MPPTestPane.h"
#include "MPPClientCheat.h"
#include "MPPGraphicPane.h"

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
    CString skinPath = L".\\WinXP.Royale.cjstyles";
    XTPSkinManager()->SetApplyOptions(xtpSkinApplyFrame | xtpSkinApplyColors | xtpSkinApplyMetrics);    
    XTPSkinManager()->LoadSkin(skinPath);	
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME,
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;      // fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	// Set Windows XP Theme
	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);

    // Get a pointer to the command bars object.
    //CXTPCommandBars* pCommandBars = GetCommandBars();
    //if(pCommandBars == NULL)
    //{
    //    TRACE0("Failed to create command bars object.\n");
    //    return -1;      // fail to create
    //}

	// Hide array of commands
	//pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	//// Set "Always Show Full Menus" option to the FALSE
	//pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	//pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Windows XP Theme
	m_paneManager.SetTheme(xtpPaneThemeNativeWinXP);
    m_paneManager.GetPaintManager()->GetTabPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);    
    m_paneManager.GetPaintManager()->GetTabPaintManager()->SetOneNoteColors(TRUE);
    m_paneManager.GetPaintManager()->GetTabPaintManager()->m_bBoldSelected = TRUE;
    m_paneManager.GetPaintManager()->GetTabPaintManager()->SetLayout(xtpTabLayoutMultiRow);
    m_paneManager.GetPaintManager()->GetTabPaintManager()->SetPosition(xtpTabPositionTop);
    m_paneManager.GetPaintManager()->GetTabPaintManager()->DisableLunaColors(FALSE);    

    // Create docking panes.	
    CXTPDockingPane* pwndConfig  = m_paneManager.CreatePane(IDR_PANE_CONFIG, CRect(0, 0, 200, 600), xtpPaneDockTop);
#ifndef  ONLY_GRAPHIC_PANE
    CXTPDockingPane* pwndTest    = m_paneManager.CreatePane(IDR_PANE_TEST, CRect(0, 0, 200, 600), xtpPaneDockTop);
    CXTPDockingPane* pwndCheat   = m_paneManager.CreatePane(IDR_PANE_CHEAT, CRect(0, 0, 200, 600), xtpPaneDockTop);    
    CXTPDockingPane* pwndLogView = m_paneManager.CreatePane(IDR_PANE_LOGVIEW, CRect(0, 0, 200, 300), xtpPaneDockBottom);        
    m_paneManager.AttachPane(pwndTest, pwndConfig);
    m_paneManager.AttachPane(pwndCheat, pwndConfig);

#ifdef DEBUG
    CXTPDockingPane* pwndSetItem = m_paneManager.CreatePane(IDR_PANE_SETITEM, CRect(0, 0, 200, 600), xtpPaneDockTop);
    m_paneManager.AttachPane(pwndSetItem, pwndConfig);
#endif    

#endif


    CXTPDockingPane* pwndGraphic = m_paneManager.CreatePane(IDR_PANE_GRAPHIC, CRect(0, 0, 200, 600), xtpPaneDockTop);
    m_paneManager.AttachPane(pwndGraphic, pwndConfig);
    
    pwndConfig->Select();

	// Set the icons for the docking pane tabs.
	int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_CONFIG};
	m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));

	// Load the previous state for docking panes.
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//if (layoutNormal.Load(_T("NormalLayout")))
	//{
	//	m_paneManager.SetLayout(&layoutNormal);
	//}

    //SetTitle(MPP_TITLE_VER);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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
            case IDR_PANE_CONFIG:
                pPane->AttachView(this, RUNTIME_CLASS(CMPPConfigSetting));
                break;
            case IDR_PANE_LOGVIEW:
                pPane->AttachView(this, RUNTIME_CLASS(CMPPLogView));
                pPane->Hide();                
                CMPPLogView::GetInstance()->SetOwnerPane(pPane);
                break;
            case IDR_PANE_SETITEM:
                pPane->AttachView(this, RUNTIME_CLASS(CMPPSetItemPane));
                break;
            case IDR_PANE_TEST:
                pPane->AttachView(this, RUNTIME_CLASS(CMPPTestPane));
                break;
            case IDR_PANE_CHEAT:
                pPane->AttachView(this, RUNTIME_CLASS(CMPPClientCheat));
                break;
            case IDR_PANE_GRAPHIC:
                pPane->AttachView(this, RUNTIME_CLASS(CMPPGraphicPane));
                break;
			}
		}
		return TRUE;
	}
	return FALSE;
}
