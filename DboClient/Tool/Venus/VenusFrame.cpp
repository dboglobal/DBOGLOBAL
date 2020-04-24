// VenusFrame.cpp : CVenusFrame 클래스의 구현
//

#include "stdafx.h"
#include "Venus.h"

#include "VenusFrame.h"

#include "StatusBarDialog.h"
#include "OptionDialog.h"
#include "LightDialog.h"
#include "ProgressDialog.h"
#include "PostEffectDialog.h"

#include "VenusConfig.h"

#include "EffectEditLayer.h"
#include "TextureLayer.h"
#include "MeshLayer.h"
#include "PathLayer.h"
#include "EventLayer.h"

#include "InstanceEffectDialog.h"

#include ".\venuspropertycontainer.h"
#include ".\venusvisualmanager.h"

#include "StringManager.h"

#include "libgfl.h"


#include "VenusFramework.h"
#include ".\venusframe.h"

#include "Splash.h"
#include "VenusCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVenusFrame

IMPLEMENT_DYNCREATE(CVenusFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CVenusFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_EFFECT_LIST, OnViewEffectList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EFFECT_LIST, OnUpdateViewEffectList)
//	ON_COMMAND(ID_FILE_NEW, OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
//	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
//	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_VIEW_OPTION, OnViewOption)
	ON_COMMAND(ID_EDIT_MODE_MOVE, OnEditModeMove)
	ON_COMMAND(ID_EDIT_MODE_ROTATE, OnEditModeRotate)
	ON_COMMAND(ID_EDIT_MODE_SCALE, OnEditModeScale)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODE_MOVE, OnUpdateEditModeMove)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODE_ROTATE, OnUpdateEditModeRotate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODE_SCALE, OnUpdateEditModeScale)
	ON_COMMAND(ID_VIEW_EDIT_DIALOG, OnViewEditDialog)
	ON_COMMAND(ID_FILE_EFFECT_NEW, OnFileEffectNew)
	ON_COMMAND(ID_FILE_EFFECT_OPEN, OnFileEffectOpen)
	ON_COMMAND(ID_FILE_EFFECT_SAVE, OnFileEffectSave)
	ON_COMMAND(ID_FILE_EFFECT_SAVE_AS, OnFileEffectSaveAs)
	ON_COMMAND(ID_VIEW_UPDATE_NEWS, OnViewUpdateNews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_UPDATE_NEWS, OnUpdateViewUpdateNews)
	ON_COMMAND(ID_VIEW_LIGHT, OnViewLight)
	ON_COMMAND(ID_FILE_EFFECT_MERGE, &CVenusFrame::OnFileEffectMerge)
	ON_COMMAND(ID_VIEW_POST_EFFECT, &CVenusFrame::OnViewPostEffect)
    ON_COMMAND(ID_VIEW_BLURFILTER, &CVenusFrame::OnViewBlurfilter)
    ON_UPDATE_COMMAND_UI(ID_VIEW_BLURFILTER, &CVenusFrame::OnUpdateViewBlurfilter)
    ON_COMMAND(ID_VIEW_FRAMEFIX, &CVenusFrame::OnViewFramefix)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FRAMEFIX, &CVenusFrame::OnUpdateViewFramefix)
    ON_COMMAND(ID_VIEW_VIEWBOUNDINGSPHERE, &CVenusFrame::OnViewBoundingSphere)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWBOUNDINGSPHERE, &CVenusFrame::OnUpdateViewBoundingSphere)
    ON_COMMAND(ID_TOOL_GENARATEBOUNDINGSPHEREALL, &CVenusFrame::OnToolGenarateBoundingSphereAll)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CVenusFrame 생성/소멸

CVenusFrame::CVenusFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CVenusFrame::~CVenusFrame()
{
}


int CVenusFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// Gfl Lib Load
	gflLibraryInit();
	gflEnableLZW(GFL_TRUE);


	// Insert
	this->CenterWindow();
	this->ShowWindow(SW_SHOWMAXIMIZED);
//	this->UpdateWindow();

	InitializeToolbar();

	InitializeEditLayer();

	InitializeStatusbar();

	if (!CInstanceEffectDialog::GetInstance().Create(CInstanceEffectDialog::IDD, this))
	{
		TRACE0("Failed to create CInstanceEffectDialog.");
		return -1;
	}
	if (!COptionDialog::GetInstance().Create(COptionDialog::IDD, this))
	{
		TRACE0("Failed to create COptionDialog.");
		return -1;
	}
	if (!CEditDialog::GetInstance().Create(CEditDialog::IDD, this))
	{
		TRACE0("Failed to create CEditDialog.");
		return -1;
	}
	if (!CLightDialog::GetInstance().Create(CLightDialog::IDD, this))
	{
		TRACE0("Failed to create CLightDialog.");
		return -1;
	}
	if (!CProgressDialog::GetInstance().Create(CProgressDialog::IDD, this))
	{
		TRACE0("Failed to create CProgressDialog.");
		return -1;
	}
	if (!CPostEffectDialog::GetInstance().Create(CPostEffectDialog::IDD, this))
	{
		TRACE0("Failed to create CLightDialog.");
		return -1;
	}

	CSplashWnd::ShowSplashScreen(this, "TOOL\\VenusData\\splash.bmp");

	return 0;
}

BOOL CVenusFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CVenusFrame 진단

#ifdef _DEBUG
void CVenusFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CVenusFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CVenusFrame 메시지 처리기

void CVenusFrame::InitializeEditLayer()
{
	CSize maptoolbarsize;

	maptoolbarsize.cx = 320;
	HDC pDC = ::GetDC(NULL);

	maptoolbarsize.cy = GetDeviceCaps( pDC, VERTRES) - 240;
	::ReleaseDC( NULL, pDC );

	m_wndEditLayer.Create(this, maptoolbarsize , IDW_PROP_BAR);	
	m_wndEditLayer.ShowWindow( SW_HIDE );			
	
	m_wndEditLayer.SetBarStyle( m_wndEditLayer.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	m_wndEditLayer.AddView("EffectEditLayer",		RUNTIME_CLASS(CEffectEditLayer));
	m_wndEditLayer.AddView("TextureLayer",			RUNTIME_CLASS(CTextureLayer));
	m_wndEditLayer.AddView("MeshLayer",				RUNTIME_CLASS(CMeshLayer));
	m_wndEditLayer.AddView("PathLayer",				RUNTIME_CLASS(CPathLayer));
	m_wndEditLayer.AddView("EventLayer",			RUNTIME_CLASS(CEventLayer));

	// Add map toolbar palette...
	m_wndEditLayer.EnableDocking( CBRS_ALIGN_RIGHT | CBRS_ALIGN_LEFT ); //| CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM );		
}


BOOL CVenusFrame::InitializeStatusbar()
{
	if (!CStatusBarDialog::GetInstance().Create(CStatusBarDialog::IDD, this))
	{
		TRACE0("Failed to create CStatusbarDialog.");
		return FALSE;
	}

	if (!m_wndStatusReBar.Create(this, RBS_BANDBORDERS , 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM, AFX_IDW_REBAR ) ||		
		!m_wndStatusReBar.AddBar(&CStatusBarDialog::GetInstance()))
	{
		TRACE0("Failed to create rebar\n");
		return FALSE;      // fail to create
	}

	return TRUE;
}

BOOL CVenusFrame::InitializeToolbar()
{
	m_ToolBar.SetBorders(1, 1, 1, 1);
	if (!m_ToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_DRAW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_ToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH,
									   IDB_TOOLBAR_DRAW,
		                               IDB_TOOLBAR_DRAW_HOT);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_ToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_ToolBar);

	return TRUE;
}

void CVenusFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	static BOOL bFirsttime = TRUE;
	if(bFirsttime)
	{
		bFirsttime = FALSE;

		RECT viewwndRect;
		GetActiveView()->GetWindowRect(&viewwndRect);

		SIZE maptoolbarsize;
		maptoolbarsize.cx = 300;
		maptoolbarsize.cy = viewwndRect.bottom - viewwndRect.top - 114;

		m_wndEditLayer.SetFloatingSize( maptoolbarsize );
		DockControlBar(&m_wndEditLayer, AFX_IDW_DOCKBAR_RIGHT);		
		m_wndEditLayer.ShowWindow(SW_NORMAL);
	}

	CFrameWnd::ActivateFrame(nCmdShow);
}

void CVenusFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CVenusPropertyContainer::GetInstance().m_bSave)
	{
		if (AfxMessageBox(CStringManager::GetInstance().GetString(STRING_SAVE), MB_OKCANCEL) == IDOK)
		{
			OnFileEffectSave();
		}
	}

	CVenusConfig::GetInstance().Save();

	gflLibraryExit(); 

	//gMainApp()->DeleteVenus();

	CVenusFramework::GetInstance().Destroy();

	CFrameWnd::OnClose();
}

void CVenusFrame::OnViewEffectList()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CInstanceEffectDialog::GetInstance().IsWindowVisible())
		CInstanceEffectDialog::GetInstance().ShowWindow(SW_HIDE);
	else
		CInstanceEffectDialog::GetInstance().ShowWindow(SW_SHOW);
}


void CVenusFrame::OnViewEditDialog()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CEditDialog::GetInstance().IsWindowVisible())
		CEditDialog::GetInstance().ShowWindow(SW_HIDE);
	else
		CEditDialog::GetInstance().ShowWindow(SW_SHOW);
}

void CVenusFrame::OnViewOption()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (COptionDialog::GetInstance().IsWindowVisible())
		COptionDialog::GetInstance().ShowWindow(SW_HIDE);
	else
		COptionDialog::GetInstance().ShowWindow(SW_SHOW);
}


void CVenusFrame::OnViewPostEffect()
{
	// TODO: Add your command handler code here
	if (CPostEffectDialog::GetInstance().IsWindowVisible())
		CPostEffectDialog::GetInstance().ShowWindow(SW_HIDE);
	else
		CPostEffectDialog::GetInstance().ShowWindow(SW_SHOW);
}


void CVenusFrame::OnViewUpdateNews()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CVenusConfig::GetInstance().ShowUpdateNews();
}

void CVenusFrame::OnUpdateViewUpdateNews(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}
void CVenusFrame::OnUpdateViewEffectList(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(CInstanceEffectDialog::GetInstance().IsWindowVisible());
}


void CVenusFrame::OnViewLight()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CLightDialog::GetInstance().IsWindowVisible())
		CLightDialog::GetInstance().ShowWindow(SW_HIDE);
	else
		CLightDialog::GetInstance().ShowWindow(SW_SHOW);
}


void CVenusFrame::OnEditModeMove()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CVenusConfig::GetInstance().m_nEditMode = CVenusConfig::EDIT_MODE_MOVE;
	CEditDialog::GetInstance().Reset();
}
void CVenusFrame::OnUpdateEditModeMove(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(CVenusConfig::GetInstance().m_nEditMode == CVenusConfig::EDIT_MODE_MOVE);
}

void CVenusFrame::OnEditModeRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CVenusConfig::GetInstance().m_nEditMode = CVenusConfig::EDIT_MODE_ROTATE;
	CEditDialog::GetInstance().Reset();
}
void CVenusFrame::OnUpdateEditModeRotate(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(CVenusConfig::GetInstance().m_nEditMode == CVenusConfig::EDIT_MODE_ROTATE);
}

void CVenusFrame::OnEditModeScale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CVenusConfig::GetInstance().m_nEditMode = CVenusConfig::EDIT_MODE_SCALE;
	CEditDialog::GetInstance().Reset();
}
void CVenusFrame::OnUpdateEditModeScale(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(CVenusConfig::GetInstance().m_nEditMode == CVenusConfig::EDIT_MODE_SCALE);
}

void CVenusFrame::OnFileEffectNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CVenusPropertyContainer::GetInstance().m_bSave)
	{
		if (AfxMessageBox(CStringManager::GetInstance().GetString(STRING_SAVE), MB_OKCANCEL) == IDOK)
		{
			OnFileEffectSave();
		}
	}

	CVenusConfig::GetInstance().m_strFileName = "";
	CVenusConfig::GetInstance().m_strPathName = "";

	CVenusVisualManager::GetInstance().Clear();
	CVenusPropertyContainer::GetInstance().Clear();

	CVenusPlayerEventManager::GetInstance().DeleteEffectEntity();

	//CVenusPropertyContainer::GetInstance().Create();
	//CVenusVisualManager::GetInstance().Create();

	CVenusPropertyContainer::GetInstance().m_bSave = FALSE;

	CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
	pView->RebuildResourceEffect();

	CPathLayer* pPathView = (CPathLayer*)gGetEditLayer(EDIT_LAYER_PATH);
	pPathView->RebuildSplinePath();

	// Event layer 에서 사용하고 있는 이펙트의 리스가 달라 지므로 갱신을 해 줘야 한다.
	CEventLayer* pEventView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
	pEventView->ResetProperties();

	CString strTitleName;
	strTitleName.Format( "%s [ %s ]", CVenusConfig::GetInstance().m_strAppName, "Noname");
	SetWindowText(strTitleName);	
}

void CVenusFrame::OnFileEffectOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnFileEffectNew();

	OnFileEffectMerge();
}


void CVenusFrame::OnFileEffectMerge()
{
	// TODO: Add your command handler code here
	CString strFilter;
	strFilter.Format("%s File(*.%s)|*.%s\0", EFFECT_FILE_EXE, EFFECT_FILE_EXE, EFFECT_FILE_EXE);

	CFileDialog dlg(TRUE, NULL, "Venus", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());

		CString strExtName;
		strExtName.Format(".%s", EFFECT_FILE_EXE);

		CVenusConfig::GetInstance().m_strPathName = dlg.GetPathName();
        CVenusConfig::GetInstance().m_strFileName = dlg.GetFileName();

		if (CVenusConfig::GetInstance().m_strPathName.Find(strExtName) < 0)
		{
			CVenusConfig::GetInstance().m_strPathName += strExtName;
		}

		if (CVenusPropertyContainer::GetInstance().Load(CVenusConfig::GetInstance().m_strPathName.GetBuffer(0)))
		{
			CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
			pView->RebuildResourceEffect();

			CPathLayer* pPathView = (CPathLayer*)gGetEditLayer(EDIT_LAYER_PATH);
			pPathView->RebuildSplinePath();

			// Event layer 에서 사용하고 있는 이펙트의 리스가 달라 지므로 갱신을 해 줘야 한다.
			CEventLayer* pEventView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
			pEventView->ResetProperties();


			CVenusConfig::GetInstance().m_strFileName = dlg.GetFileName();
			if (CVenusConfig::GetInstance().m_strFileName.Find(strExtName) < 0)
			{
				CVenusConfig::GetInstance().m_strFileName += strExtName;
			}

			CString strTitleName;
			strTitleName.Format( "%s [ %s ]", CVenusConfig::GetInstance().m_strAppName, CVenusConfig::GetInstance().m_strFileName);
			SetWindowText(strTitleName);
		}
	}
	SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());
}

void CVenusFrame::OnFileEffectSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CVenusConfig::GetInstance().m_strFileName.GetLength() <= 0)
	{
		OnFileEffectSaveAs();
		return;
	}
	if (CVenusConfig::GetInstance().m_strPathName.GetLength() > 0)
	{
		CProgressDialog::GetInstance().Start(CVenusConfig::GetInstance().m_strFileName.GetBuffer(), 1000);
		CVenusPropertyContainer::GetInstance().Save(CVenusConfig::GetInstance().m_strPathName.GetBuffer(0));
	}
}

void CVenusFrame::OnFileEffectSaveAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString strFileName = CVenusConfig::GetInstance().m_strFileName;
	if (strFileName.GetLength() <= 0)
	{
		strFileName = "Venus";
	}

	CString strFilter;
	strFilter.Format("%s File(*.%s)|*.%s\0", EFFECT_FILE_EXE, EFFECT_FILE_EXE, EFFECT_FILE_EXE);

	CFileDialog dlg(FALSE, NULL, strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());

		CString strExtName;
		strExtName.Format(".%s", EFFECT_FILE_EXE);

		CVenusConfig::GetInstance().m_strPathName = dlg.GetPathName();
		if (CVenusConfig::GetInstance().m_strPathName.Find(strExtName) < 0)
		{
			CVenusConfig::GetInstance().m_strPathName += strExtName;
		}

		CProgressDialog::GetInstance().Start(dlg.GetFileName().GetBuffer(0), 1000);
		if (CVenusPropertyContainer::GetInstance().Save(CVenusConfig::GetInstance().m_strPathName.GetBuffer(0)))
		{
			CVenusConfig::GetInstance().m_strFileName = dlg.GetFileName();
			if (CVenusConfig::GetInstance().m_strFileName.Find(strExtName) < 0)
			{
				CVenusConfig::GetInstance().m_strFileName += strExtName;
			}

			CString strTitleName;
			strTitleName.Format( "%s [ %s ]", CVenusConfig::GetInstance().m_strAppName, CVenusConfig::GetInstance().m_strFileName);
			SetWindowText(strTitleName);	
		}
	}
	SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());
}

void CVenusFrame::OnViewBlurfilter()
{
    RwUInt8 byFilters = CVenusCamera::GetInstance().GetPostEffectFilters();
    if(byFilters == POST_EFFECT_FILTER_NONE)
    {
        CVenusCamera::GetInstance().SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
    }
    else
    {
        CVenusCamera::GetInstance().SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
    }
}

void CVenusFrame::OnUpdateViewBlurfilter(CCmdUI *pCmdUI)
{
    RwUInt8 byFilters = CVenusCamera::GetInstance().GetPostEffectFilters();
    if(byFilters == POST_EFFECT_FILTER_NONE)
    {
        pCmdUI->SetCheck(FALSE);
    }
    else
    {
        pCmdUI->SetCheck(TRUE);
    }

}

void CVenusFrame::OnViewFramefix()
{
    BOOL bFrameFixEnable = CVenusFramework::GetInstance().GetFrameFix();
    CVenusFramework::GetInstance().SetFrameFix(!bFrameFixEnable);
}

void CVenusFrame::OnUpdateViewFramefix(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(CVenusFramework::GetInstance().GetFrameFix());
}

void CVenusFrame::OnViewBoundingSphere() 
{
    RwBool bEnable = CVenusConfig::GetInstance().m_bRenderBoundingSphere;
    CVenusConfig::GetInstance().m_bRenderBoundingSphere = !bEnable;
}

void CVenusFrame::OnUpdateViewBoundingSphere( CCmdUI *pCmdUI ) 
{
    pCmdUI->SetCheck(CVenusConfig::GetInstance().m_bRenderBoundingSphere);
}

void CVenusFrame::OnToolGenarateBoundingSphereAll() 
{
    CVenusVisualManager::GetInstance().OnGenerateBoundingBoxAll();
}