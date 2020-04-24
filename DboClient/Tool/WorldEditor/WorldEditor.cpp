// WorldEditor.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "MainFrm.h"

#include "CInstanceView.h"
#include "UserMsg.h"

#include "WorldEditorDoc.h"
#include "WorldEditorView.h"

#include "CEditorApplication.h"
#include "CSceneManager.h"
#include "CWorldSectorEntity.h"

#include "Resource.h"

#include "CAtomicEntity.h"
#include "WorldEditor.h"
#include ".\worldeditor.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CWorldEditorApp

BEGIN_MESSAGE_MAP(CWorldEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW, CWorldEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWorldEditorApp::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, CWorldEditorApp::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, CWorldEditorApp::OnFileSaveAs)
	ON_COMMAND(ID_MODE_OBJECT_MOVE, OnModeObjectMove)
	ON_COMMAND(ID_MODE_OBJECT_ROTATE, OnModeObjectRotate)
	ON_COMMAND(ID_MODE_OBJECT_SCALE, OnModeObjectScale)
	ON_COMMAND(ID_MODE_CAMERA_VIEW_FREE, OnModeCameraViewFree)
	ON_COMMAND(ID_MODE_CAMERA_VIEW_TOP, OnModeCameraViewTop)
	ON_COMMAND(ID_VIEW_PROPERTYDIALOGBAR, OnViewPropertydialogbar)
	ON_COMMAND(ID_VIEW_WIREFRAME_FOR_OBJECT, OnViewWireframeForObject)
	ON_COMMAND(ID_VIEW_WIREFRAME_FOR_TERRAIN, OnViewWireframeForTerrain)
	ON_COMMAND(ID_VIEW_SOLID_FOR_OBJECT, OnViewSolidForObject)
	ON_COMMAND(ID_VIEW_SOLID_FOR_TERRAIN, OnViewSolidForTerrain)
	ON_COMMAND(ID_VIEW_BOUNDING_FOR_OBJECT, OnViewBoundingForObject)
	ON_COMMAND(ID_VIEW_BOUNDING_FOR_TERRAIN, OnViewBoundingForTerrain)
END_MESSAGE_MAP()


// CWorldEditorApp 생성

CWorldEditorApp::CWorldEditorApp()
: m_pDocName(NULL)
{
	CEditorApplication::CreateInstance();

	m_pDocName = NTL_NEW( CFixedString );
	m_pDocName->Assign( _T( "default" ) );

	RwChar				szFilePath[256];
	GetCurrentDirectory( 256, szFilePath );
	CFixedString		FilePath( szFilePath );
	CEditorApplication::SetAppPath( FilePath );

	FilePath.Append( _T( "\\world\\structure" ) );
	CEditorApplication::SetFilePath( FilePath );
	CSceneManager::SetFilePath( FilePath );
	CWorldSectorEntity::SetFilePath( FilePath );
}


CWorldEditorApp::~CWorldEditorApp()
{
	CEditorApplication::DestroyInstance();

	NTL_DELETE( m_pDocName );
	m_pDocName = NULL;
}


// 유일한 CWorldEditorApp 개체입니다.

CWorldEditorApp theApp;

// CWorldEditorApp 초기화

BOOL CWorldEditorApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(RWSTRING("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.
	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	// 문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWorldEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CWorldEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 명령줄에 지정된 명령을 디스패치합니다. 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	// SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.

	HWND hWnd = ((CMainFrame*)AfxGetMainWnd())->GetActiveView()->GetSafeHwnd(); 
	g_pEngineApp->Create( hWnd, FALSE );

	::SendMessage(m_pAseetObjectView->GetSafeHwnd(), WM_USER_INIT_CUSTOM_WINDOW, 0, 0);

	CInstanceView::GetInstance()->GetInstanceDialog()->GetTreeCtrl()->InitializeData();

	return TRUE;
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CWorldEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


BOOL CWorldEditorApp::OnIdle( LONG nCount )
{
	BOOL bMore = CWinApp::OnIdle( nCount );

	g_pEngineApp->OnIdle();

	return TRUE;
}

int CWorldEditorApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_pEngineApp->Destroy();

	return CWinApp::ExitInstance();
}

void CWorldEditorApp::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CFileDialog		OpenFileDialog( TRUE, _T( "prj" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									_T( "Project Files (*.prj)|*.prj|All Files (*.*)|*.*||" ), NULL );

	if( OpenFileDialog.DoModal() == IDCANCEL )
	{
		return;
	}

	g_pEngineApp->OnFileLoad( OpenFileDialog.GetPathName().GetBuffer() );

	m_pDocName->Assign( OpenFileDialog.GetFileTitle().GetBuffer() );

	return;
}


void CWorldEditorApp::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if( m_pDocName->IsEquivalent( _T( "default" ) ) != FALSE )
	{
		OnFileSaveAs();
	}
	else
	{
		g_pEngineApp->OnFileSave( const_cast<RwChar *>(m_pDocName->GetBuffer()) );

		m_pDocName->Assign( m_pDocName->GetBuffer() );
	}

	CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	pFrame->SetWindowText( m_pDocName->GetBuffer() );

	return;
}


void CWorldEditorApp::OnFileSaveAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CFileDialog		OpenFileDialog( FALSE, _T( "prj" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									_T( "Project Files (*.prj)|*.prj|All Files (*.*)|*.*||" ), NULL );

	if( OpenFileDialog.DoModal() == IDCANCEL )
	{
		return;
	}

	g_pEngineApp->OnFileSave( OpenFileDialog.GetFileTitle().GetBuffer() );

	m_pDocName->Assign( OpenFileDialog.GetFileTitle().GetBuffer() );

	CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	pFrame->SetWindowText( m_pDocName->GetBuffer() );

	return;
}


void CWorldEditorApp::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	static bool	bFirst = true;

	if( bFirst == true )
	{
		CWinApp::OnFileNew();

		bFirst = false;
	}
	else
	{
		g_pEngineApp->OnFileNew();

		m_pDocName->Assign( _T( "default" ) );
	}

	CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	pFrame->SetWindowText( m_pDocName->GetBuffer() );

	return;
}


void CWorldEditorApp::OnModeObjectMove()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	g_pEngineApp->SetEditMode( E_EDIT_MODE_OBJECT_MOVE_XZ );

	CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());

	// Menu

	CMenu *			pMenu = pFrame->GetMenu();

	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 3 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_MOVE, &Info, FALSE );
	Info.fState = MFS_CHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_MOVE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_ROTATE, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_ROTATE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_SCALE, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_SCALE, &Info, FALSE );

	return;
}


void CWorldEditorApp::OnModeObjectRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	g_pEngineApp->SetEditMode( E_EDIT_MODE_OBJECT_ROTATE_Y );

	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());

	// Menu

	CMenu *			pMenu = pFrame->GetMenu();

	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 3 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_MOVE, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_MOVE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_ROTATE, &Info, FALSE );
	Info.fState = MFS_CHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_ROTATE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_SCALE, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_SCALE, &Info, FALSE );

	return;
}


void CWorldEditorApp::OnModeObjectScale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	g_pEngineApp->SetEditMode( E_EDIT_MODE_OBJECT_SCALE_Y );

	CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());

	// Menu

	CMenu *			pMenu = pFrame->GetMenu();

	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 3 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_MOVE, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_MOVE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_ROTATE, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_ROTATE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_OBJECT_SCALE, &Info, FALSE );
	Info.fState = MFS_CHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_OBJECT_SCALE, &Info, FALSE );

	return;
}


void CWorldEditorApp::OnModeCameraViewFree()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	g_pEngineApp->SetCameraMovementMode( E_CAMERA_MOVEMENT_MODE_FREE );

	CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();

	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 3 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_MODE_CAMERA_VIEW_FREE, &Info, FALSE );
	Info.fState = MFS_CHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_CAMERA_VIEW_FREE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_CAMERA_VIEW_TOP, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_CAMERA_VIEW_TOP, &Info, FALSE );

	return;
}


void CWorldEditorApp::OnModeCameraViewTop()
{
	g_pEngineApp->SetCameraMovementMode( E_CAMERA_MOVEMENT_MODE_TOP );

	CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());

	// Menu

	CMenu *			pMenu = pFrame->GetMenu();

	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 3 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_MODE_CAMERA_VIEW_FREE, &Info, FALSE );
	Info.fState = MFS_UNCHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_CAMERA_VIEW_FREE, &Info, FALSE );

	pMenu->GetMenuItemInfo( ID_MODE_CAMERA_VIEW_TOP, &Info, FALSE );
	Info.fState = MFS_CHECKED;
	pMenu->SetMenuItemInfo( ID_MODE_CAMERA_VIEW_TOP, &Info, FALSE );

	return;
}

void CWorldEditorApp::OnViewPropertydialogbar()
{
	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();
	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 2 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_VIEW_PROPERTYDIALOGBAR, &Info, FALSE );

	if( Info.fState == MFS_UNCHECKED )
	{
		Info.fState = MFS_CHECKED;

		pFrame->ShowPropertyDialogBarWindow( true );
	}
	else
	{
		Info.fState = MFS_UNCHECKED;

		pFrame->ShowPropertyDialogBarWindow( false );
	}

	pMenu->SetMenuItemInfo( ID_VIEW_PROPERTYDIALOGBAR, &Info, FALSE );
}


void CWorldEditorApp::OnViewSolidForObject()
{
	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();
	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 2 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_VIEW_SOLID_FOR_OBJECT, &Info, FALSE );

	if( Info.fState == MFS_UNCHECKED )
	{
		Info.fState = MFS_CHECKED;

		CAtomicEntity::SetRenderSolidForObject( TRUE );
		CAtomicEntity::SetRenderSolidForSelectedObject( TRUE );
	}
	else
	{
		Info.fState = MFS_UNCHECKED;

		CAtomicEntity::SetRenderSolidForObject( FALSE );
		CAtomicEntity::SetRenderSolidForSelectedObject( FALSE );
	}

	pMenu->SetMenuItemInfo( ID_VIEW_SOLID_FOR_OBJECT, &Info, FALSE );
}

void CWorldEditorApp::OnViewSolidForTerrain()
{
	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();
	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 2 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_VIEW_SOLID_FOR_TERRAIN, &Info, FALSE );

	if( Info.fState == MFS_UNCHECKED )
	{
		Info.fState = MFS_CHECKED;

		CAtomicEntity::SetRenderSolidForTerrain( TRUE );
		CAtomicEntity::SetRenderSolidForSelectedTerrain( TRUE );
	}
	else
	{
		Info.fState = MFS_UNCHECKED;

		CAtomicEntity::SetRenderSolidForTerrain( FALSE );
		CAtomicEntity::SetRenderSolidForSelectedTerrain( FALSE );
	}

	pMenu->SetMenuItemInfo( ID_VIEW_SOLID_FOR_TERRAIN, &Info, FALSE );
}


void CWorldEditorApp::OnViewWireframeForObject()
{
	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();
	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 2 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_VIEW_WIREFRAME_FOR_OBJECT, &Info, FALSE );

	if( Info.fState == MFS_UNCHECKED )
	{
		Info.fState = MFS_CHECKED;

		CAtomicEntity::SetRenderWireFrameForObject( TRUE );
		CAtomicEntity::SetRenderWireFrameForSelectedObject( TRUE );
	}
	else
	{
		Info.fState = MFS_UNCHECKED;

		CAtomicEntity::SetRenderWireFrameForObject( FALSE );
		CAtomicEntity::SetRenderWireFrameForSelectedObject( FALSE );
	}

	pMenu->SetMenuItemInfo( ID_VIEW_WIREFRAME_FOR_OBJECT, &Info, FALSE );
}


void CWorldEditorApp::OnViewWireframeForTerrain()
{
	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();
	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 2 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_VIEW_WIREFRAME_FOR_TERRAIN, &Info, FALSE );

	if( Info.fState == MFS_UNCHECKED )
	{
		Info.fState = MFS_CHECKED;

		CAtomicEntity::SetRenderWireFrameForTerrain( TRUE );
		CAtomicEntity::SetRenderWireFrameForSelectedTerrain( TRUE );
	}
	else
	{
		Info.fState = MFS_UNCHECKED;

		CAtomicEntity::SetRenderWireFrameForTerrain( FALSE );
		CAtomicEntity::SetRenderWireFrameForSelectedTerrain( FALSE );
	}

	pMenu->SetMenuItemInfo( ID_VIEW_WIREFRAME_FOR_TERRAIN, &Info, FALSE );
}

void CWorldEditorApp::OnViewBoundingForObject()
{
	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();
	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 2 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_VIEW_BOUNDING_FOR_OBJECT, &Info, FALSE );

	if( Info.fState == MFS_UNCHECKED )
	{
		Info.fState = MFS_CHECKED;

		CAtomicEntity::SetRenderBoundingForObject( TRUE );
		CAtomicEntity::SetRenderBoundingForSelectedObject( TRUE );
	}
	else
	{
		Info.fState = MFS_UNCHECKED;

		CAtomicEntity::SetRenderBoundingForObject( FALSE );
		CAtomicEntity::SetRenderBoundingForSelectedObject( TRUE );
	}

	pMenu->SetMenuItemInfo( ID_VIEW_BOUNDING_FOR_OBJECT, &Info, FALSE );
}


void CWorldEditorApp::OnViewBoundingForTerrain()
{
	CMainFrame *	pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());
	CMenu *			pMenu = pFrame->GetMenu();
	MENUITEMINFO	Info;

	pMenu = pMenu->GetSubMenu( 2 );

	Info.cbSize = sizeof( MENUITEMINFO );
	Info.fMask = MIIM_STATE;

	pMenu->GetMenuItemInfo( ID_VIEW_BOUNDING_FOR_TERRAIN, &Info, FALSE );

	if( Info.fState == MFS_UNCHECKED )
	{
		Info.fState = MFS_CHECKED;

		CAtomicEntity::SetRenderBoundingForTerrain( TRUE );
		CAtomicEntity::SetRenderBoundingForSelectedTerrain( TRUE );
	}
	else
	{
		Info.fState = MFS_UNCHECKED;

		CAtomicEntity::SetRenderBoundingForTerrain( FALSE );
		CAtomicEntity::SetRenderBoundingForSelectedTerrain( FALSE );
	}

	pMenu->SetMenuItemInfo( ID_VIEW_BOUNDING_FOR_TERRAIN, &Info, FALSE );
}
