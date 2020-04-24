// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "ModelToolApplication.h"
#include "ClumpPane.h"
#include "MaterialPane.h"
#include "AnimSetPane.h"
#include "AttackTypePane.h"
#include "BoneEditPane.h"
#include "AnimPlayPane.h"
#include "PropertyPane.h"
#include "TreePCPane.h"
#include "TreeMob.h"
#include "TreeObjectPane.h"
#include "TreeItemPane.h"
#include "BBoxPane.h"
#include "EnvMapPane.h"
#include "DummyModelPane.h"
#include "LinkEffectPane.h"
#include "ItemViewPane.h"
#include "TreeNPCPane.h"
#include "TreeVehiclePane.h"
#include "NaviMeshPane.h"
#include "VehicleViewPane.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CXTPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
    ON_XTP_CREATECONTROL()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)    
    ON_COMMAND(ID_PANE_ANIMATIONPLAY, &CMainFrame::OnPaneAnimationplay)
    ON_COMMAND(ID_PANE_ANIMATIONSET, &CMainFrame::OnPaneAnimationset)
    ON_UPDATE_COMMAND_UI(ID_PANE_ANIMATIONPLAY, &CMainFrame::OnUpdatePaneAnimationplay)
    ON_UPDATE_COMMAND_UI(ID_PANE_ANIMATIONSET, &CMainFrame::OnUpdatePaneAnimationset)
    ON_COMMAND(ID_PANE_ATTACKTYPE, &CMainFrame::OnPaneAttacktype)
    ON_UPDATE_COMMAND_UI(ID_PANE_ATTACKTYPE, &CMainFrame::OnUpdatePaneAttacktype)
    ON_COMMAND(ID_PANE_BBOXEDIT, &CMainFrame::OnPaneBboxedit)
    ON_UPDATE_COMMAND_UI(ID_PANE_BBOXEDIT, &CMainFrame::OnUpdatePaneBboxedit)
    ON_COMMAND(ID_PANE_BONEEDIT, &CMainFrame::OnPaneBoneedit)
    ON_UPDATE_COMMAND_UI(ID_PANE_BONEEDIT, &CMainFrame::OnUpdatePaneBoneedit)
    ON_COMMAND(ID_PANE_CLUMPEDIT, &CMainFrame::OnPaneClumpedit)
    ON_UPDATE_COMMAND_UI(ID_PANE_CLUMPEDIT, &CMainFrame::OnUpdatePaneClumpedit)
    ON_COMMAND(ID_PANE_EVENTPROPERTIES, &CMainFrame::OnPaneEventproperties)
    ON_UPDATE_COMMAND_UI(ID_PANE_EVENTPROPERTIES, &CMainFrame::OnUpdatePaneEventproperties)
    ON_COMMAND(ID_PANE_MATERIALEDIT, &CMainFrame::OnPaneMaterialedit)
    ON_UPDATE_COMMAND_UI(ID_PANE_MATERIALEDIT, &CMainFrame::OnUpdatePaneMaterialedit)
    ON_COMMAND(ID_PANE_PCTREE, &CMainFrame::OnPanePctree)
    ON_UPDATE_COMMAND_UI(ID_PANE_PCTREE, &CMainFrame::OnUpdatePanePctree)
    ON_COMMAND(ID_PANE_MOBTREE, &CMainFrame::OnPaneMobTree)
    ON_UPDATE_COMMAND_UI(ID_PANE_MOBTREE, &CMainFrame::OnUpdatePaneMobTree)
    ON_COMMAND(ID_PANE_OBJECTTREE, &CMainFrame::OnPaneObjecttree)
    ON_UPDATE_COMMAND_UI(ID_PANE_OBJECTTREE, &CMainFrame::OnUpdatePaneObjecttree)
    ON_COMMAND(ID_PANE_DUMMYMODEL, &CMainFrame::OnPaneDummymodel)
    ON_UPDATE_COMMAND_UI(ID_PANE_DUMMYMODEL, &CMainFrame::OnUpdatePaneDummymodel)
    ON_COMMAND(ID_TOOL_CAMERA_SENSE, &CMainFrame::OnCameraSense)    
    ON_COMMAND(ID_TOOL_THICKNESS, OnToonThickness)
    ON_UPDATE_COMMAND_UI(ID_TOOL_TOONCOLOR, &CMainFrame::OnUpdateBack)
    ON_UPDATE_COMMAND_UI(ID_TOON_SELECTOR, &CMainFrame::OnUpdateSelectorBack)    
    ON_XTP_EXECUTE(ID_TOON_SELECTOR, &CMainFrame::OnSelectorBack)

    ON_UPDATE_COMMAND_UI(ID_TOOLBAR_HEADSKINCOLOR, &CMainFrame::OnUpdateHeadSkinColor)
    ON_UPDATE_COMMAND_UI(ID_HEAD_SKIN_SELECT, &CMainFrame::OnUpdateSelectorBack)
    ON_XTP_EXECUTE(ID_HEAD_SKIN_SELECT, &CMainFrame::OnSelectHeadSkinColor)
    ON_COMMAND(ID_HEADSKIN_MORE, OnHeadSkinMore)

    ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BODYSKINCOLOR, &CMainFrame::OnUpdateBodySkinColor)
    ON_UPDATE_COMMAND_UI(ID_BODY_SKIN_SELECT, &CMainFrame::OnUpdateSelectorBack)
    ON_XTP_EXECUTE(ID_BODY_SKIN_SELECT, &CMainFrame::OnSelectBodySkinColor)
    ON_COMMAND(ID_BODYSKIN_MORE, OnBodySkinMore)
    
    ON_COMMAND(ID_PANE_ITEMTREEE, &CMainFrame::OnPaneItemtreee)
    ON_UPDATE_COMMAND_UI(ID_PANE_ITEMTREEE, &CMainFrame::OnUpdatePaneItemtreee)
    ON_COMMAND(ID_PANE_ENVIROMENTMAP, &CMainFrame::OnPaneEnviromentmap)
    ON_COMMAND(ID_PANE_ITEMVIEW, &CMainFrame::OnPaneItemview)
    ON_UPDATE_COMMAND_UI(ID_PANE_ITEMVIEW, &CMainFrame::OnUpdatePaneItemview)
    ON_COMMAND(ID_PANE_NPCTREE, &CMainFrame::OnPaneNpctree)
    ON_UPDATE_COMMAND_UI(ID_PANE_NPCTREE, &CMainFrame::OnUpdatePaneNpctree)
    ON_COMMAND(ID_PANE_VEHICLEVIEW, &CMainFrame::OnPaneVehicleview)
    ON_UPDATE_COMMAND_UI(ID_PANE_VEHICLEVIEW, &CMainFrame::OnUpdatePaneVehicleview)
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
    //CString skinPath = ".\\Tool\\ModelTool\\Styles\\WinXP.Royale.cjstyles";
    CString skinPath =	".\\Vista.cjstyles";
    XTPSkinManager()->SetApplyOptions(xtpSkinApplyFrame | xtpSkinApplyColors | xtpSkinApplyMetrics);
    //XTPSkinManager()->LoadSkin(skinPath, _T("Normalroyale.INI"));	
    //XTPSkinManager()->LoadSkin(skinPath, _T("Black_Normal.ini"));	
	XTPSkinManager()->LoadSkin(skinPath);	

    m_clrToonColor = RGB(0, 0, 0);


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

    // Set Office 2003 Theme
    CXTPPaintManager::SetTheme(xtpThemeOffice2003);

    XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
    pIconsInfo->bUseDisabledIcons = TRUE;
    pIconsInfo->bIconsWithShadow = TRUE;    
    

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}    

    CXTPToolBar* pToolBar2 = pCommandBars->Add(_T("Toolbar1"), xtpBarTop);
    if(!pToolBar2 || !pToolBar2->LoadToolBar(IDR_TOOLBAR1))
    {
        return -1;
    }
    CXTPToolBar* pToolBar3 = pCommandBars->Add(_T("Toolbar2"), xtpBarTop);
    if(!pToolBar3 || !pToolBar3->LoadToolBar(IDR_TOOLBAR2))
    {
        return -1;
    }

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

    CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
    pOptions->szLargeIcons = CSize(24, 24);
    pOptions->bLargeIcons = TRUE;

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);
    m_paneManager.GetPaintManager()->GetTabPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);    
    m_paneManager.GetPaintManager()->GetTabPaintManager()->SetOneNoteColors(TRUE);
    m_paneManager.GetPaintManager()->GetTabPaintManager()->m_bBoldSelected = TRUE;
    m_paneManager.GetPaintManager()->GetTabPaintManager()->SetLayout(xtpTabLayoutMultiRow);
    m_paneManager.GetPaintManager()->GetTabPaintManager()->DisableLunaColors(FALSE);    

	// Create docking panes.    
    CXTPDockingPane* pwndAnimPlay = m_paneManager.CreatePane(
        IDR_PANE_ANIMPLAY, CRect(0, 0, 300,80), xtpPaneDockBottom);
    CXTPDockingPane* pwndBoneEdit = m_paneManager.CreatePane(
        IDR_PANE_BONEEDIT, CRect(0, 0, 200, 220), xtpPaneDockBottom, pwndAnimPlay);
    CXTPDockingPane* pwndLinkEffect = m_paneManager.CreatePane(
        IDR_PANE_LINKEFFECT, CRect(0, 0, 200, 220), xtpPaneDockRight, pwndBoneEdit);
    CXTPDockingPane* pwndBBox = m_paneManager.CreatePane(
        IDR_PANE_BBOX, CRect(0, 0, 200, 220), xtpPaneDockBottom, pwndLinkEffect);
	CXTPDockingPane* pwndPaneClump = m_paneManager.CreatePane(
		IDR_PANE_CLUMP, CRect(0, 0,200, 400), xtpPaneDockLeft);
    CXTPDockingPane* pwndPaneMaterial = m_paneManager.CreatePane(
        IDR_PANE_MATERIAL, CRect(0, 0,200, 150), xtpPaneDockBottom, pwndPaneClump);
    CXTPDockingPane* pwndPaneDummy = m_paneManager.CreatePane(
        IDR_PANE_DUMMY, CRect(0, 0, 200, 200), xtpPaneDockBottom, pwndPaneMaterial);
    CXTPDockingPane* pwndPaneAnimSet = m_paneManager.CreatePane(
        IDR_PANE_ANIMSET, CRect(0, 0,330, 400), xtpPaneDockRight);
    CXTPDockingPane* pwndAttackType = m_paneManager.CreatePane(
        IDR_PANE_ATTACKTYPE, CRect(0, 0, 330, 100), xtpPaneDockBottom, pwndPaneAnimSet);
    CXTPDockingPane* pwndProperty = m_paneManager.CreatePane(
        IDR_PANE_PROPERTIES, CRect(0, 0, 330, 300), xtpPaneDockBottom, pwndAttackType);
    CXTPDockingPane* pwndItemView = m_paneManager.CreatePane(
        IDR_PANE_ITEMVIEW, CRect(0, 0, 250, 400), xtpPaneDockBottom, pwndPaneAnimSet);
    CXTPDockingPane* pwndVehicleView = m_paneManager.CreatePane(IDR_PANE_VEHICLEVIEW, CRect(0, 0, 250, 400), xtpPaneDockBottom, pwndPaneAnimSet);
    CXTPDockingPane* pwndTreePC = m_paneManager.CreatePane(
        IDR_PANE_TREEPC, CRect(0, 0, 200, 300), xtpPaneDockLeft);
    CXTPDockingPane* pwndTreeMob = m_paneManager.CreatePane(
        IDR_PANE_TREEMOB, CRect(0, 0, 200, 300), xtpPaneDockLeft);
    CXTPDockingPane* pwndTreeObject = m_paneManager.CreatePane(
        IDR_PANE_TREEOBJECT, CRect(0, 0, 200, 300), xtpPaneDockLeft);
    CXTPDockingPane* pwndTreeNPC    = m_paneManager.CreatePane(
        IDR_PANE_TREENPC, CRect(0, 0, 200, 300), xtpPaneDockLeft);
    CXTPDockingPane* pwndTreeItem = m_paneManager.CreatePane(
        IDR_PANE_TREEITEM, CRect(0, 0, 200, 300), xtpPaneDockLeft);
    CXTPDockingPane* pwndTreeVehicle = m_paneManager.CreatePane(
        IDR_PANE_TREEVEHICLE, CRect(0, 0, 200, 300), xtpPaneDockLeft);
    CXTPDockingPane* pwndEnvMap = m_paneManager.CreatePane(
        IDR_PANE_ENVMAP, CRect(0, 0, 200, 200), xtpPaneDockBottom, pwndPaneMaterial);
    CXTPDockingPane* pwndNaviMesh = m_paneManager.CreatePane(
        IDR_PANE_NAVI, CRect(0, 0, 200, 200), xtpPaneDockBottom, pwndPaneMaterial);
    
    
    pwndAnimPlay->SetOptions(xtpPaneNoHideable);
    pwndBoneEdit->SetOptions(xtpPaneNoHideable);
    pwndBBox->SetOptions(xtpPaneNoHideable);
    pwndPaneClump->SetOptions(xtpPaneNoHideable);
    pwndPaneMaterial->SetOptions(xtpPaneNoHideable);
    pwndPaneAnimSet->SetOptions(xtpPaneNoHideable);
    pwndAttackType->SetOptions(xtpPaneNoHideable);
    pwndProperty->SetOptions(xtpPaneNoHideable);
    pwndTreePC->SetOptions(xtpPaneNoHideable);
    pwndTreeMob->SetOptions(xtpPaneNoHideable);
    pwndTreeObject->SetOptions(xtpPaneNoHideable);
    pwndPaneDummy->SetOptions(xtpPaneNoHideable);
    pwndTreeItem->SetOptions(xtpPaneNoHideable);
    pwndEnvMap->SetOptions(xtpPaneNoHideable);
    pwndLinkEffect->SetOptions(xtpPaneNoHideable);
    pwndItemView->SetOptions(xtpPaneNoHideable);
    pwndVehicleView->SetOptions(xtpPaneNoHideable);
    pwndTreeNPC->SetOptions(xtpPaneNoHideable);
    pwndTreeVehicle->SetOptions(xtpPaneNoHideable);

    m_paneManager.ToggleDocking(pwndEnvMap);
    m_paneManager.ToggleDocking(pwndItemView);
    m_paneManager.ToggleDocking(pwndVehicleView);
    pwndVehicleView->Select();
    pwndEnvMap->Close();
    pwndItemView->Close();
    pwndVehicleView->Close();
    
    
    m_paneManager.AttachPane(pwndTreeVehicle, pwndTreeMob);
    m_paneManager.AttachPane(pwndTreeObject, pwndTreeMob);
    m_paneManager.AttachPane(pwndTreeItem, pwndTreeMob);    
    m_paneManager.AttachPane(pwndTreeNPC, pwndTreeMob);
    m_paneManager.AttachPane(pwndTreePC, pwndTreeMob); 
    
    pwndNaviMesh->Select();    
    m_paneManager.AttachPane(pwndNaviMesh, pwndBoneEdit);        
    m_paneManager.AttachPane(pwndPaneMaterial, pwndPaneDummy);   
    
	// Set the icons for the docking pane tabs.
	/*int nIDIcons[] = {IDR_PANE_CLUMP, IDR_PANE_PROPERTIES};
		m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));*/

    // TODO: 테스트를 위해서 레이아웃 저장 기능을 끈다.
	//// Load the previous state for docking panes.
	//CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	//if (layoutNormal.Load(_T("NormalLayout")))
	//{
	//	m_paneManager.SetLayout(&layoutNormal);
	//}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPFrameWnd::PreCreateWindow(cs) )
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
	CXTPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
    if (lpCreateControl->bToolBar)
    {
        CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
        if (!pToolBar)
            return FALSE;

        if (lpCreateControl->nID == ID_TOOL_CAMERA_SENSE)
        {
            m_pEditCamraSense = (CXTPControlEdit*)CXTPControlEdit::CreateObject();
            m_pEditCamraSense->ShowLabel(FALSE);
            m_pEditCamraSense->SetEnabled(TRUE);
            m_pEditCamraSense->SetReadOnly(FALSE);
            m_pEditCamraSense->SetFlags(xtpFlagManualUpdate);
            m_pEditCamraSense->SetWidth(50);            
            m_pEditCamraSense->SetEditText("2.0");            
            
            lpCreateControl->pControl = m_pEditCamraSense;            
            return TRUE;
        }
        else if(lpCreateControl->nID == ID_TOOL_THICKNESS)
        {
            m_pEditToonThickness = (CXTPControlEdit*)CXTPControlEdit::CreateObject();
            m_pEditToonThickness->ShowLabel(FALSE);
            m_pEditToonThickness->SetEnabled(TRUE);
            m_pEditToonThickness->SetReadOnly(FALSE);
            m_pEditToonThickness->SetFlags(xtpFlagManualUpdate);
            m_pEditToonThickness->SetWidth(50);
            m_pEditToonThickness->SetEditText("1.2");
            

            lpCreateControl->pControl = m_pEditToonThickness;
            return TRUE;
        }
        else if(lpCreateControl->nID == ID_TOOL_TOONCOLOR)
        {
            m_pCSToonColor =  new CXTPControlPopupColor();
            m_pCSToonColor->SetEnabled(TRUE);
            m_pCSToonColor->SetFlags(xtpFlagManualUpdate);

            CXTPPopupBar* pColorBar = (CXTPPopupBar*)CXTPPopupToolBar::CreateObject();
            
            pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_TOON_SELECTOR);            

            m_pCSToonColor->SetCommandBar(pColorBar);
            pColorBar->InternalRelease();

            pColorBar->EnableCustomization(FALSE);

            lpCreateControl->pControl = m_pCSToonColor;
            return TRUE;
        }
        else if(lpCreateControl->nID == ID_TOOLBAR_HEADSKINCOLOR)
        {
            m_pCSHeadSkinColor = new CXTPControlPopupColor();
            m_pCSHeadSkinColor->SetEnabled(TRUE);
            m_pCSHeadSkinColor->SetFlags(xtpFlagManualUpdate);

            CXTPPopupBar* pColorBar = (CXTPPopupBar*)CXTPPopupToolBar::CreateObject();
            pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_HEAD_SKIN_SELECT);
            pColorBar->GetControls()->Add(new CXTPControlButtonColor(), ID_HEADSKIN_MORE);

           m_pCSHeadSkinColor->SetCommandBar(pColorBar);
           pColorBar->InternalRelease();

           pColorBar->EnableCustomization(FALSE);                      

           lpCreateControl->pControl = m_pCSHeadSkinColor;
           return TRUE;

        }
        else if(lpCreateControl->nID == ID_TOOLBAR_BODYSKINCOLOR)
        {
            m_pCSBodySkinColor = new CXTPControlPopupColor();
            m_pCSBodySkinColor->SetEnabled(TRUE);
            m_pCSBodySkinColor->SetFlags(xtpFlagManualUpdate);

            CXTPPopupBar* pColorBar = (CXTPPopupBar*)CXTPPopupToolBar::CreateObject();
            pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_BODY_SKIN_SELECT);            
            pColorBar->GetControls()->Add(new CXTPControlButtonColor(), ID_BODYSKIN_MORE);

            m_pCSBodySkinColor->SetCommandBar(pColorBar);
            pColorBar->InternalRelease();

            pColorBar->EnableCustomization(TRUE);

            lpCreateControl->pControl = m_pCSBodySkinColor;
            return TRUE;
        }
    }
    return FALSE;
}

void CMainFrame::OnClose()
{

    // 종료시 저장할것인지 확인한다.
    GetSafeInstance(CTreePCPane)->OnShowSaveForChanges();
    GetSafeInstance(CTreeItemPane)->OnShowSaveForChanges();
    GetSafeInstance(CTreeObjectPane)->OnShowSaveForChanges();

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));    

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CXTPFrameWnd::OnClose();
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
            case IDR_PANE_CLUMP:
                pPane->AttachView(this, RUNTIME_CLASS(CClumpPane));
                break;
            case IDR_PANE_MATERIAL:
                pPane->AttachView(this, RUNTIME_CLASS(CMaterialPane));
                break;
            case IDR_PANE_ANIMSET:
                pPane->AttachView(this, RUNTIME_CLASS(CAnimSetPane));
                break;
            case IDR_PANE_ATTACKTYPE:
                pPane->AttachView(this, RUNTIME_CLASS(CAttackTypePane));
                break;
            case IDR_PANE_BONEEDIT:
                pPane->AttachView(this, RUNTIME_CLASS(CBoneEditPane));
                break;
            case IDR_PANE_ANIMPLAY:
                pPane->AttachView(this, RUNTIME_CLASS(CAnimPlayPane));
                break;
            case IDR_PANE_PROPERTIES:
                pPane->AttachView(this, RUNTIME_CLASS(CPropertyPane));
                break;
            case IDR_PANE_TREEPC:
                pPane->AttachView(this, RUNTIME_CLASS(CTreePCPane));
                break;
            case IDR_PANE_TREEMOB:
                pPane->AttachView(this, RUNTIME_CLASS(CTreeMobPane));
                break;
            case IDR_PANE_BBOX:
                pPane->AttachView(this, RUNTIME_CLASS(CBBoxPane));
                break;
            case IDR_PANE_TREEOBJECT:
                pPane->AttachView(this, RUNTIME_CLASS(CTreeObjectPane));
                break;
            case IDR_PANE_TREEITEM:
                pPane->AttachView(this, RUNTIME_CLASS(CTreeItemPane));
                break;
            case IDR_PANE_DUMMY:
                pPane->AttachView(this, RUNTIME_CLASS(CDummyModelPane));
                break;         
            case IDR_PANE_ENVMAP:
                pPane->AttachView(this, RUNTIME_CLASS(CEnvMapPane));
                break;            
            case IDR_PANE_LINKEFFECT:
                pPane->AttachView(this, RUNTIME_CLASS(CLinkEffectPane));
                break;
            case IDR_PANE_ITEMVIEW:
                pPane->AttachView(this, RUNTIME_CLASS(CItemViewPane));
                break;
            case IDR_PANE_TREENPC:
                pPane->AttachView(this, RUNTIME_CLASS(CTreeNPCPane));
                break;
            case IDR_PANE_TREEVEHICLE:
                pPane->AttachView(this, RUNTIME_CLASS(CTreeVehiclePane));
                break;
            case IDR_PANE_NAVI:
                pPane->AttachView(this, RUNTIME_CLASS(CNaviMeshPane));
                SetVisibleNaviPane(FALSE);
                break;
            case IDR_PANE_VEHICLEVIEW:
                pPane->AttachView(this, RUNTIME_CLASS(CVehicleViewPane));
                break;
			}
		}
		return TRUE;
	}
	return FALSE;
}


void CMainFrame::OnPaneAnimationplay()
{
    m_paneManager.ShowPane(IDR_PANE_ANIMPLAY);
}

void CMainFrame::OnUpdatePaneAnimationplay(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_ANIMPLAY)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}


void CMainFrame::OnPaneAnimationset()
{
    m_paneManager.ShowPane(IDR_PANE_ANIMSET);
}


void CMainFrame::OnUpdatePaneAnimationset(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_ANIMSET)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}

void CMainFrame::OnPaneAttacktype()
{
    m_paneManager.ShowPane(IDR_PANE_ATTACKTYPE);
}

void CMainFrame::OnUpdatePaneAttacktype(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_ATTACKTYPE)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}


void CMainFrame::OnPaneBboxedit()
{
    m_paneManager.ShowPane(IDR_PANE_BBOX);
}

void CMainFrame::OnUpdatePaneBboxedit(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_BBOX)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}


void CMainFrame::OnPaneBoneedit()
{
    m_paneManager.ShowPane(IDR_PANE_BONEEDIT);
}


void CMainFrame::OnUpdatePaneBoneedit(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_BONEEDIT)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}


void CMainFrame::OnPaneClumpedit()
{
    m_paneManager.ShowPane(IDR_PANE_CLUMP);
}

void CMainFrame::OnUpdatePaneClumpedit(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_CLUMP)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}



void CMainFrame::OnPaneEventproperties()
{
    m_paneManager.ShowPane(IDR_PANE_PROPERTIES);
}



void CMainFrame::OnUpdatePaneEventproperties(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_PROPERTIES)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}


void CMainFrame::OnPaneMaterialedit()
{
    m_paneManager.ShowPane(IDR_PANE_MATERIAL);
}


void CMainFrame::OnUpdatePaneMaterialedit(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_MATERIAL)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}


void CMainFrame::OnPanePctree()
{
    m_paneManager.ShowPane(IDR_PANE_TREEPC);
}

void CMainFrame::OnUpdatePanePctree(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_TREEPC)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}

void CMainFrame::OnPaneMobTree()
{
    m_paneManager.ShowPane(IDR_PANE_TREEMOB);
}

void CMainFrame::OnUpdatePaneMobTree(CCmdUI* pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_TREEMOB)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}

void CMainFrame::OnPaneObjecttree()
{
    m_paneManager.ShowPane(IDR_PANE_TREEOBJECT);
}


void CMainFrame::OnUpdatePaneObjecttree(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_TREEOBJECT)->IsClosed();
    pCmdUI->SetCheck(!bClosed);    
}

void CMainFrame::OnPaneDummymodel()
{
    m_paneManager.ShowPane(IDR_PANE_DUMMY);
}

void CMainFrame::OnUpdatePaneDummymodel(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_DUMMY)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}

void CMainFrame::OnPaneItemtreee()
{
    m_paneManager.ShowPane(IDR_PANE_TREEITEM);
}

void CMainFrame::OnUpdatePaneItemtreee(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_TREEITEM)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}

void CMainFrame::OnCameraSense()
{
    if(CModelToolApplication::GetInstance())
    {
        CString strCameraSense = m_pEditCamraSense->GetEditText();
        RwReal fCameraSense = (RwReal)atof(strCameraSense);

        CNtlGameCamera* pCamera = (CNtlGameCamera*)CModelToolApplication::GetInstance()->GetCamera();
        pCamera->SetRatioDistance(fCameraSense);
    }
}

void CMainFrame::OnUpdateSelectorBack( CCmdUI* pCmdUI ) 
{
    CXTPControlColorSelector* pSelector = DYNAMIC_DOWNCAST(CXTPControlColorSelector, CXTPControl::FromUI(pCmdUI));

    if (pSelector)
    {
        pSelector->SetColor(-1);
    }
    pCmdUI->Enable(TRUE);
}

void CMainFrame::OnSelectorBack( NMHDR* pNMHDR, LRESULT* pResult ) 
{
    NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

    CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
    m_clrToonColor = pControl->GetColor();    
    
    OnUpdateBack(NULL);

    *pResult = 1;
}

void CMainFrame::OnUpdateBack( CCmdUI* pCmdUI ) 
{
    m_pCSToonColor->SetColor(m_clrToonColor);
    
    BYTE r = GetRValue(m_clrToonColor);
    BYTE g = GetGValue(m_clrToonColor);
    BYTE b = GetBValue(m_clrToonColor);

    RwRGBA color = {r, g, b, 255};
    CMTClump::SetToonInkColor(color);
}

void CMainFrame::OnToonThickness() 
{
    if(CModelToolApplication::GetInstance())
    {
        CString strThickness = m_pEditToonThickness->GetEditText();
        RwReal fThickness = (RwReal)atof(strThickness);

        CMTClump::SetToonThickness(fThickness);
    }
}

void CMainFrame::OnPaneEnviromentmap()
{
    m_paneManager.ShowPane(IDR_PANE_ENVMAP);        
}

void CMainFrame::OnPaneItemview()
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_ITEMVIEW)->IsClosed();
    if(bClosed)
    {
        m_paneManager.ShowPane(IDR_PANE_ITEMVIEW);        
    }
    else
    {
        m_paneManager.ClosePane(IDR_PANE_ITEMVIEW);
    }
}

void CMainFrame::OnUpdatePaneItemview(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_ITEMVIEW)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}

void CMainFrame::OnPaneVehicleview()
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_VEHICLEVIEW)->IsClosed();
    if(bClosed)
    {
        m_paneManager.ShowPane(IDR_PANE_VEHICLEVIEW);        
    }
    else
    {
        m_paneManager.ClosePane(IDR_PANE_VEHICLEVIEW);
    }    
}

void CMainFrame::OnUpdatePaneVehicleview(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_VEHICLEVIEW)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}


void CMainFrame::OnPaneNpctree()
{
    m_paneManager.ShowPane(IDR_PANE_TREENPC);
}

void CMainFrame::OnUpdatePaneNpctree(CCmdUI *pCmdUI)
{
    BOOL bClosed = m_paneManager.FindPane(IDR_PANE_TREENPC)->IsClosed();
    pCmdUI->SetCheck(!bClosed);
}

void CMainFrame::OnUpdateHeadSkinColor( CCmdUI* pCmdUI ) 
{
    m_pCSHeadSkinColor->SetColor(m_clrHeadSkinColor);

    BYTE r = GetRValue(m_clrHeadSkinColor);
    BYTE g = GetGValue(m_clrHeadSkinColor);
    BYTE b = GetBValue(m_clrHeadSkinColor);

    if(CModelToolApplication::GetInstance()->GetEditChar())
    {
        GetSafeInstance(CModelToolApplication)->GetEditChar()->SetHeadColor(r, g, b);
    }
}

void CMainFrame::OnUpdateBodySkinColor( CCmdUI* pCmdUI ) 
{
    m_pCSBodySkinColor->SetColor(m_clrBodySkinColor);

    BYTE r = GetRValue(m_clrBodySkinColor);
    BYTE g = GetGValue(m_clrBodySkinColor);
    BYTE b = GetBValue(m_clrBodySkinColor);

    if(CModelToolApplication::GetInstance()->GetEditChar())
    {
        GetSafeInstance(CModelToolApplication)->GetEditChar()->SetSkinColor(r, g, b);
    }

}

void CMainFrame::OnSelectHeadSkinColor( NMHDR* pNMHDR, LRESULT* pResult ) 
{
    NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

    CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
    m_clrHeadSkinColor = pControl->GetColor();    

    OnUpdateHeadSkinColor(NULL);

    *pResult = 1;

}

void CMainFrame::OnSelectBodySkinColor( NMHDR* pNMHDR, LRESULT* pResult ) 
{
    NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

    CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
    m_clrBodySkinColor = pControl->GetColor();    

    OnUpdateBodySkinColor(NULL);

    *pResult = 1;
}

void CMainFrame::OnHeadSkinMore() 
{
    CColorDialog cd;
    if(cd.DoModal())
    {
        m_clrHeadSkinColor = cd.GetColor();
        OnUpdateHeadSkinColor(NULL);
    }
}

void CMainFrame::OnBodySkinMore() 
{
    CColorDialog cd;
    if(cd.DoModal())
    {
        m_clrBodySkinColor = cd.GetColor();
        OnUpdateBodySkinColor(NULL);
    }
}

void CMainFrame::SetVisibleNaviPane( BOOL bVisible ) 
{
    if(bVisible)
    {
        m_paneManager.ShowPane(IDR_PANE_NAVI);
        m_paneManager.ClosePane(IDR_PANE_BONEEDIT);
    }    
    else
    {
        m_paneManager.ShowPane(IDR_PANE_BONEEDIT);
        m_paneManager.ClosePane(IDR_PANE_NAVI);
    }
}
