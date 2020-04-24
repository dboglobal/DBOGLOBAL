// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MainFrm.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlDTCinematicManager.h"
#include "NtlCameraController.h"
#include "NtlCameraManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLCENode.h"

// DTEdit
#include "DTEditor.h"
#include "DTEditorDoc.h"
#include "DockingPaneCustomTheme.h"
#include "DTEditorGlobal.h"

#include "ViewControlDlg.h"
#include "DirectionPane.h"
#include "DirectionPropController.h"
#include "DirectionPropEffect.h"
#include "DirectionPropSound.h"
#include "DirectionPropRefreeView.h"
#include "SoundOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_XTP_CREATECONTROL()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_CBN_SELCHANGE(ID_EDIT_MODE_COMBO, OnSelchangeEditMode)

	ON_COMMAND(ID_SPLINEMARK_CONTROLLINE, &CMainFrame::OnSplinemarkControlline)
	ON_COMMAND(ID_SPLINEMARK_CURVELINE, &CMainFrame::OnSplinemarkCurveline)
	ON_COMMAND(ID_SPLINEMARK_CURVEROTATELINE, &CMainFrame::OnSplinemarkCurverotateline)
	ON_UPDATE_COMMAND_UI(ID_SPLINEMARK_CONTROLLINE, &CMainFrame::OnUpdateSplinemarkControlline)
	ON_UPDATE_COMMAND_UI(ID_SPLINEMARK_CURVELINE, &CMainFrame::OnUpdateSplinemarkCurveline)
	ON_UPDATE_COMMAND_UI(ID_SPLINEMARK_CURVEROTATELINE, &CMainFrame::OnUpdateSplinemarkCurverotateline)
	ON_COMMAND(ID_RENDERVIEW_WORLDRENDER_ONOFF, &CMainFrame::OnRenderviewWorldrenderOnoff)
	ON_UPDATE_COMMAND_UI(ID_RENDERVIEW_WORLDRENDER_ONOFF, &CMainFrame::OnUpdateRenderviewWorldrenderOnoff)
	ON_COMMAND(ID_WORLD_CHANGE, &CMainFrame::OnWorldChange)
	ON_COMMAND(ID_AVATAR_RACE, &CMainFrame::OnAvatrRaceClassGender)
	ON_COMMAND(ID_AVATAR_TELEPORT, &CMainFrame::OnAvatarTeleport)
	ON_COMMAND(ID_AVATAR_SPEED, &CMainFrame::OnAvatarSpeed)
	ON_COMMAND(ID_OPTION_SOUND, &CMainFrame::OnOptionSound)
	ON_COMMAND(ID_SPLINEMARK_CURVE_CUSTOMDATA_VISIBLE, &CMainFrame::OnSplinemarkCurveCustomdataVisible)
	ON_UPDATE_COMMAND_UI(ID_SPLINEMARK_CURVE_CUSTOMDATA_VISIBLE, &CMainFrame::OnUpdateSplinemarkCurveCustomdataVisible)	
	
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
:m_pDirectionPane(NULL)
,m_pDirectionController(NULL)
,m_pDirectionPropEffect(NULL)
,m_pDirectionPropSound(NULL)
,m_pDirectionPropRefree(NULL)
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
	NTL_DELETE(m_pDirectionPane);
	NTL_DELETE(m_pDirectionController);
	NTL_DELETE(m_pDirectionPropEffect);
	NTL_DELETE(m_pDirectionPropSound);
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
	m_pMenuBar = (CXTPMenuBar*)pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	if(m_pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	XTPSkinManager()->LoadSkin(m_strStylesPath + _T(".\\Tool\\DTEditor\\styles\\WinXP.Royale.cjstyles"), _T("Normalroyale.INI"));	

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeNativeWinXP);

	m_paneManager.SetCustomTheme(new CDockingPaneAdobeTheme);
	m_paneManager.UseSplitterTracker(FALSE);
	m_paneManager.SetDockingContext(new CDockingPaneAdobeContext);
	m_paneManager.SetDefaultPaneOptions(xtpPaneNoCloseable);

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);

	XTPPaintManager()->SetCustomTheme(new CFlatCommandBarsTheme );
	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	CXTPImageManager* pImageManager = pCommandBars->GetImageManager();
	if (pImageManager->IsAlphaIconsSupported())
	{
		pImageManager->SetIcons(IDR_MAINFRAME, IDB_TOOLBAR_ALPHA);

		pCommandBars->GetCommandBarsOptions()->szIcons = CSize(20, 20);
		pCommandBars->GetCommandBarsOptions()->szPopupIcons = CSize(20, 20);

		XTPPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;

	}

	// Create the editmode toolbar
	m_pEditMode = (CXTPToolBar*)pCommandBars->Add(_T("EditMode Bar"), xtpBarTop);
	if (!m_pEditMode || !m_pEditMode->LoadToolBar(IDR_EDIT_MODE_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	// Dock the StaticBar to the right of the OptionsBar
	DockRightOf(m_pEditMode, pToolBar);

	// Create docking panes.
	m_pwndPaneControl		= m_paneManager.CreatePane(IDR_PANE_CONTROL, CRect(0, 0,300, 200), xtpPaneDockBottom);
	m_pwndPaneSpline		= m_paneManager.CreatePane(IDR_PANE_OPTIONS, CRect(0, 0,250, 120), xtpPaneDockRight);
	m_pwndPaneSkill			= m_paneManager.CreatePane(IDR_PANE_SKILL, CRect(0, 0,250, 120), xtpPaneDockRight);
	m_pwndPaneBot			= m_paneManager.CreatePane(IDR_PANE_BOT, CRect(0, 0,250, 120), xtpPaneDockRight);
	m_pwndPaneDirection		= m_paneManager.CreatePane(IDR_DIRECTION_PANE, CRect(0, 0,250, 120), xtpPaneDockRight);

	m_pwndPaneProperties	= m_paneManager.CreatePane(IDR_PANE_PROPERTIES, CRect(0, 0,250, 120), xtpPaneDockBottom, m_pwndPaneSpline);

	//m_pwndPaneActor			= m_paneManager.CreatePane(IDR_PANE_ACTOR, CRect(0, 0,250, 120), xtpPaneDockBottom, m_pwndPaneSpline);
		
	m_pwndPaneScript		= m_paneManager.CreatePane(IDR_PANE_SCRIPT, CRect(0, 0,250, 120), xtpPaneDockRight);

	m_paneManager.AttachPane(m_pwndPaneSkill, m_pwndPaneSpline);
	m_paneManager.AttachPane(m_pwndPaneBot, m_pwndPaneSpline);
	m_paneManager.AttachPane(m_pwndPaneDirection, m_pwndPaneSpline);
	m_pwndPaneSpline->Select();

	m_paneManager.AttachPane(m_pwndPaneScript, m_pwndPaneControl);
	m_pwndPaneControl->Select();
			
	/*
	// Set the icons for the docking pane tabs.
	int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES};
		m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));
	
	// Load the previous state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
		m_paneManager.SetLayout(&layoutNormal);
	}

	*/

	// 연출 관련 클래스
	m_pDirectionPane					= NTL_NEW CDirectionPane;
	m_pDirectionController				= NTL_NEW CDirectionPropController;
	m_pDirectionPropEffect				= NTL_NEW CDirectionPropEffect;
	m_pDirectionPropSound				= NTL_NEW CDirectionPropSound;
	
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

void CMainFrame::OnDestroy()
{
	CXTPFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.	
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

		switch (pPane->GetID())
		{
		case IDR_PANE_PROPERTIES:
			{
				break;
			}
		case IDR_PANE_ACTOR:
			{
				break;
				if(m_ActorPane.GetSafeHwnd() == 0)
				{
					m_ActorPane.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
				}
				pPane->Attach(&m_ActorPane);
				break;
			}
		case IDR_PANE_SCRIPT:
			{
				if(m_ScriptPane.GetSafeHwnd() == 0)
				{
					m_ScriptPane.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
				}
				pPane->Attach(&m_ScriptPane);
				break;
			}
		case IDR_PANE_OPTIONS:
			{
				if(m_ObjectPane.GetSafeHwnd() == 0)
				{
					m_ObjectPane.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
				}
				pPane->Attach(&m_ObjectPane);

				if(m_PropertyPane.GetSafeHwnd() == 0)
				{
					m_PropertyPane.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
				}
				m_pwndPaneProperties->Attach(&m_PropertyPane);

				break;
			}
		case IDR_PANE_SKILL:
			{
				if(m_SkillPane.GetSafeHwnd() == 0)
				{
					m_SkillPane.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
				}
				pPane->Attach(&m_SkillPane);
				break;
			}
		case IDR_PANE_BOT:
			{
				if(m_BotPane.GetSafeHwnd() == 0)
				{
					m_BotPane.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
				}
				pPane->Attach(&m_BotPane);
				break;
			}
		case IDR_DIRECTION_PANE:
			{
				if( m_pDirectionPane->GetSafeHwnd() == 0 )
				{
					m_pDirectionPane->Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
				}
				pPane->Attach(m_pDirectionPane);
				break;
			}
		case IDR_PANE_CONTROL:
			{
				if(m_ControlPane.GetSafeHwnd() == 0)
				{
					m_ControlPane.Create(IDD_CONTROL_PANE, this);
				}
				pPane->Attach(&m_ControlPane);				
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	// This function is called by the Extreme Tool Kit application framework.
	// This function handles the creation of the controls that are placed
	// on the CommandBars.  The LPCREATECONTROLSTRUCT structure contains information
	// about the control to create.

	if (lpCreateControl->nID == ID_EDIT_MODE_COMBO)
	{
		if (!m_comboEditMode.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_CLIPCHILDREN,
			CRect(0, 0, 150, 100), this, ID_EDIT_MODE_COMBO))
		{
			return FALSE;
		}

		int nIndex;

		nIndex = m_comboEditMode.AddString(_T("Third Camera"));
		m_comboEditMode.SetItemData(nIndex, (DWORD_PTR)EDIT_MODE_THIRD_CAMERA);

		nIndex = m_comboEditMode.AddString(_T("Free Camera"));
		m_comboEditMode.SetItemData(nIndex, (DWORD_PTR)EDIT_MODE_FREE_CAMERA);

		m_comboEditMode.SetCurSel(0);

		// Use m_monthCal to create a new CXTPControlCustom* control.
		CXTPControlCustom* pControlMonthCal = CXTPControlCustom::CreateControlCustom(&m_comboEditMode);

		// Set the xtpFlagManualUpdate flag.
		// Without this flag you must call the Enable member of
		// CCmdUI in the ON_UPDATE_COMMAND_UI handler.
		pControlMonthCal->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlMonthCal;
		return TRUE;
	}

	/*
	// Create the Option radio button
	if (lpCreateControl->nID >= ID_PROP_TRANS_POSITION && lpCreateControl->nID <= ID_CURVE_TRANS_ROTATE)
	{
		// Get the ID of the control currently being created
		UINT nID = lpCreateControl->nID;

		// Determine the index of the control
		CXTButton& wndOption = m_wndOptions[nID - ID_PROP_TRANS_POSITION];

		// Create the Options radio button
		if (!wndOption.Create(NULL, WS_CHILD | WS_VISIBLE  | BS_AUTORADIOBUTTON,
			CRect(0, 0, 120, 20), this, nID))
		{
			return FALSE;
		}

		UINT nLocalID = nID - ID_PROP_TRANS_POSITION;

//		wndOption.SetXButtonStyle(BS_XT_SEMIFLAT | BS_XT_SHOWFOCUS | BS_XT_TWOROWS);

//		wndOption.SetIcon(CSize(16,16), m_imageList.ExtractIcon(nLocalID));

		// Use the radio button to create a CXTPControlCustom* control
		CXTPControlCustom* pControlOption = CXTPControlCustom::CreateControlCustom(&wndOption);

		// Set the ID of the custom control to the ID of the radio button
		pControlOption->SetID(nID);

		// Set the window text to the string resource associated with the button ID
		wndOption.SetWindowText(pControlOption->GetCaption());

		// Set the font of the radio button
		wndOption.SetFont(&m_fontIcon);

		// Check the first radio button on the Options toolbar.
		wndOption.SetCheck(nID == ID_PROP_TRANS_POSITION);

		// Set the vertical draw option to hide when the Option toolbar is drawn vertically.
		pControlOption->SetVerticalPositionOption(xtpVerticalButton);

		// Set the xtpFlagManualUpdate flag.
		// Without this flag you must call the Enable member of
		// CCmdUI in the ON_UPDATE_COMMAND_UI handler.
		pControlOption->SetFlags(xtpFlagManualUpdate);

		lpCreateControl->pControl = pControlOption;

		return TRUE;
	}
	
	*/

	return FALSE;
}


BEGIN_MESSAGE_MAP(CButtonOptions, CButton)
	//{{AFX_MSG_MAP(CButtonOptions)
	ON_CONTROL_REFLECT(BN_CLICKED, OnOptionShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CButtonOptions::OnOptionShow()
{
	// This function sets the vertical draw behavior for the Option
	// buttons on the Options toolbar if the toolbar is docked vertically.
	// The three possible options are to hide the control, show the control,
	// or to draw the control as a button.  These options are defined as an
	// enum in the Extreme Tool Kit as follows:
	//
	//  enum XTPControlCustomVerticalOptions
	//  {
	//      xtpVerticalHide,  // To hide control if parent toolbar vertically docked.
	//      xtpVerticalShow,  // To show control if parent toolbar vertically docked.
	//      xtpVerticalButton // To show control as a button.
	//  };

	// Get a pointer to the CommandBar.
	CXTPCommandBar* pCommandBar = DYNAMIC_DOWNCAST(CXTPCommandBar, GetParent());
	if (pCommandBar)
	{
		// Get the ID of the currently selected Options button on the Options toolbar.
		int nID = ::GetDlgCtrlID(m_hWnd);

		switch(nID)
		{
		case ID_PROP_TRANS_POSITION:  // Option "Hide Vertically" button is selected.
			break;
		case ID_PROP_TRANS_ROTATE: 
			break;
		case ID_PROP_TRANS_SCALE: 
			break;
		case ID_POINT_TRANS_POSITION: 
			break;
		case ID_CURVE_TRANS_ROTATE: 
			break;
		};
	}
}

void CMainFrame::OnSplinemarkControlline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	BYTE byMarkFlags = CNtlDTRender::GetRenderMarkFlags();
	if(byMarkFlags & DT_RENDER_MARK_CONTROL_POINT)
		CNtlDTRender::ExcludeRenderMarkFlag(DT_RENDER_MARK_CONTROL_POINT);
	else
		CNtlDTRender::IncludeRenderMarkFlag(DT_RENDER_MARK_CONTROL_POINT);
}

void CMainFrame::OnSplinemarkCurveline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE byMarkFlags = CNtlDTRender::GetRenderMarkFlags();
	if(byMarkFlags & DT_RENDER_MARK_CURVE_POINT)
		CNtlDTRender::ExcludeRenderMarkFlag(DT_RENDER_MARK_CURVE_POINT);
	else
		CNtlDTRender::IncludeRenderMarkFlag(DT_RENDER_MARK_CURVE_POINT);
}

void CMainFrame::OnSplinemarkCurverotateline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE byMarkFlags = CNtlDTRender::GetRenderMarkFlags();
	if(byMarkFlags & DT_RENDER_MARK_CURVE_ROTATE)
		CNtlDTRender::ExcludeRenderMarkFlag(DT_RENDER_MARK_CURVE_ROTATE);
	else
		CNtlDTRender::IncludeRenderMarkFlag(DT_RENDER_MARK_CURVE_ROTATE);
}

void CMainFrame::OnUpdateSplinemarkControlline(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	BYTE byMarkFlags = CNtlDTRender::GetRenderMarkFlags();
	if(byMarkFlags & DT_RENDER_MARK_CONTROL_POINT)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnSplinemarkCurveCustomdataVisible()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(IsCustomDataRender())
		EnableCustomDataRender(FALSE);
	else
		EnableCustomDataRender(TRUE);
}

void CMainFrame::OnUpdateSplinemarkCurveCustomdataVisible(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(IsCustomDataRender())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}


void CMainFrame::OnUpdateSplinemarkCurveline(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	BYTE byMarkFlags = CNtlDTRender::GetRenderMarkFlags();
	if(byMarkFlags & DT_RENDER_MARK_CURVE_POINT)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnUpdateSplinemarkCurverotateline(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	BYTE byMarkFlags = CNtlDTRender::GetRenderMarkFlags();
	if(byMarkFlags & DT_RENDER_MARK_CURVE_ROTATE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnRenderviewWorldrenderOnoff()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CNtlPLGlobal::m_bWorldTerrainVisible = !CNtlPLGlobal::m_bWorldTerrainVisible;
}

void CMainFrame::OnUpdateRenderviewWorldrenderOnoff(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(CNtlPLGlobal::m_bWorldTerrainVisible)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}


void CMainFrame::OnSelchangeEditMode() 
{
	int nCurSel = m_comboEditMode.GetCurSel();

	SetEditModeType( (EEditModeType)nCurSel );

	if(GetEditModeType() == EDIT_MODE_FREE_CAMERA)
	{
		CNtlSLEventGenerator::CameraFree();
	}
	else
	{
		CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_FREE);
	}
}

void CMainFrame::OnWorldChange()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	EEditModeType eEditMode = GetEditModeType();

	if(eEditMode != EDIT_MODE_THIRD_CAMERA)
	{
		AfxMessageBox("Current mode is not third camera !!!");
		return;
	}

	CViewControlDlg dlg;
	dlg.m_eType = EWORLD_CHANGE;
	dlg.DoModal();
}

void CMainFrame::OnAvatrRaceClassGender()
{
	EEditModeType eEditMode = GetEditModeType();

	if(eEditMode != EDIT_MODE_THIRD_CAMERA)
	{
		AfxMessageBox("Current mode is not third camera !!!");
		return;
	}

	CViewControlDlg dlg;
	dlg.m_eType = EAVATAR_RACE;
	dlg.DoModal();
}

void CMainFrame::OnAvatarTeleport()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	EEditModeType eEditMode = GetEditModeType();

	if(eEditMode != EDIT_MODE_THIRD_CAMERA)
	{
		AfxMessageBox("Current mode is not third camera !!!");
		return;
	}

	CViewControlDlg dlg;
	dlg.m_eType = EAVATAR_TELEPORT;
	dlg.DoModal();
}

void CMainFrame::OnAvatarSpeed()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	EEditModeType eEditMode = GetEditModeType();

	if(eEditMode != EDIT_MODE_THIRD_CAMERA)
	{
		AfxMessageBox("Current mode is not third camera !!!");
		return;
	}

	CViewControlDlg dlg;
	dlg.m_eType = EAVATAR_SPEED;
	dlg.DoModal();
}

void CMainFrame::OnOptionSound()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSoundOption dlgSoundOption;
	dlgSoundOption.DoModal();
}

void CMainFrame::SetDirectionProperty(CNtlSLTBCrowdController* pController)
{
	if( !m_pDirectionController )
		return;

	if( m_pDirectionController->GetSafeHwnd() == 0)
	{
		m_pDirectionController->Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
	}

	m_pDirectionController->SetProperty(pController);
	m_pwndPaneProperties->Attach( m_pDirectionController );
}

void CMainFrame::SetDirectionProperty(BYTE byNodeType, CNtlSLCENode* pNode)
{
	switch( byNodeType )
	{
	case CNtlSLCENode::E_CE_EFFECT:
		{
			if( !m_pDirectionPropEffect )
				break;

			if( m_pDirectionPropEffect->GetSafeHwnd() == 0)
			{
				m_pDirectionPropEffect->Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
			}

			m_pDirectionPropEffect->SetProperty(pNode);
			m_pwndPaneProperties->Attach( m_pDirectionPropEffect );
			break;
		}
	case CNtlSLCENode::E_CE_SOUND:
		{
			if( !m_pDirectionPropSound )
				break;

			if( m_pDirectionPropSound->GetSafeHwnd() == 0)
			{
				m_pDirectionPropSound->Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
			}

			m_pDirectionPropSound->SetProperty(pNode);
			m_pwndPaneProperties->Attach( m_pDirectionPropSound );
			break;
		}
	case CNtlSLCENode::E_CE_JUDGE:
		{
			m_pDirectionPropRefree = (CDirectionPropRefreeView*)m_pwndPaneProperties->AttachView(this, RUNTIME_CLASS(CDirectionPropRefreeView));
			m_pDirectionPropRefree->SetProperty(pNode);
			break;
		}
	default:
		{
			NTL_ASSERT(false, "CMainFrame::SetDirectionProperty, Invalid direction item type");
			break;
		}
	}
}
