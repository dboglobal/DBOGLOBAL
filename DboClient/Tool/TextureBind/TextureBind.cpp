// TextureBind.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "TextureBind.h"
#include "MainFrm.h"

#include "TextureBindDoc.h"
#include "TextureBindView.h"
#include "LoadDlg.h"
#include "SaveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TB
#include "TBApp.h"


// CTextureBindApp

BEGIN_MESSAGE_MAP(CTextureBindApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTextureBindApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_TB_OPEN, &CTextureBindApp::OnFileOpen)
	ON_COMMAND(ID_TB_SAVE, &CTextureBindApp::OnFileSave)
END_MESSAGE_MAP()


// CTextureBindApp 생성

CTextureBindApp::CTextureBindApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTextureBindApp 개체입니다.

CTextureBindApp theApp;


// CTextureBindApp 초기화

BOOL CTextureBindApp::InitInstance()
{
	// Debuging
	//_CrtSetBreakAlloc( 273 );

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	//if (!AfxOleInit())
	//{
	//	AfxMessageBox(IDP_OLE_INIT_FAILED);
	//	return FALSE;
	//}
	//AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("Texture Bind"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.
	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTextureBindDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CTextureBindView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.

	HWND hWnd = ((CMainFrame*)AfxGetMainWnd())->GetActiveView()->GetSafeHwnd();

	RECT rtRect;
	GetClientRect( hWnd, &rtRect );
	RwInt32 nWidth = rtRect.right - rtRect.left;
	RwInt32 nHeight = rtRect.bottom - rtRect.top;

	GetTBApp()->Create( hWnd, nWidth, nHeight );

	return TRUE;
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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
void CTextureBindApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CTextureBindApp 메시지 처리기

/**
* \brief 항상 호출하기 위하여 MFC의 OnIdle를 상속 받아 항상 TRUE를 리턴하도록 수정
*/
BOOL CTextureBindApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( m_pMainWnd->IsIconic() )
		return FALSE;

	GetTBApp()->OnIdle();

	__super::OnIdle(lCount);
	return TRUE;
}

int CTextureBindApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	GetTBApp()->Destroy();

	return CWinApp::ExitInstance();
}

void CTextureBindApp::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CLoadDlg dlg;

	if( dlg.DoModal() == IDOK )
	{
		CTBApp::GetInstance()->LoadTexFromPath( 
			(CBindingTexManager::eType)dlg.m_nTypeSel,
			(CBindingTexManager::eSize)dlg.m_nSizeSel,
			dlg.m_szPath );
	}
}

//void CTextureBindApp::OnFileSave()
//{
//	CSaveDlg dlg;
//
//	if( dlg.DoModal() == IDOK )
//	{
//		CTBApp::GetInstance()->SaveTexCurrentData(
//			(CBindingTexManager::eSize)dlg.m_nSaveType,
//			dlg.m_szSavePath );
//	}
//}

BOOL CTextureBindApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CTBApp::GetInstance()->PreTranslateMessage( pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam );

	return CWinApp::PreTranslateMessage(pMsg);
}
void CTextureBindApp::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSaveDlg dlg;
	
		if( dlg.DoModal() == IDOK )
		{
			CTBApp::GetInstance()->SaveTexCurrentData(
				(CBindingTexManager::eSize)dlg.m_nSaveType,
				dlg.m_szSavePath );
		}
}
