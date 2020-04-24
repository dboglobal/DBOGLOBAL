// NtlWE.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEFrm.h"

#include "NtlWEDoc.h"
#include "NtlWEView.h"
#include ".\ntlwe.h"
#include "NtlWorldMouse.h"
#include "paletteterrain.h"
#include "UpdateEvent.h"
#include "SplashScreenfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNtlWEApp

BEGIN_MESSAGE_MAP(CNtlWEApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	//ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CNtlWEApp 생성

CNtlWEApp::CNtlWEApp()
{
	_CrtSetDbgFlag   ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

CNtlWEApp::~CNtlWEApp()
{
	
}

// 유일한 CNtlWEApp 개체입니다.

CNtlWEApp theApp;

BOOL CNtlWEApp::InitInstance()
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
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	// 문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.

	CRuntimeClass* pRuntimeCalss = RUNTIME_CLASS(CNtlWEDoc);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNtlWEDoc),
		RUNTIME_CLASS(CNtlWEFrm),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CNtlWEView));
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
	//m_pMainWnd->ShowWindow(SW_SHOW);
	//m_pMainWnd->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	// SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.

	// accessors
	m_pFrm = (CNtlWEFrm*)AfxGetMainWnd();
	m_pView = (CNtlWEView*)(m_pFrm->GetActiveView());
	m_pDoc = (CNtlWEDoc*)(m_pView->GetDocument());

	// 인도어
	m_menuIdle.LoadMenu(IDR_MENU_IDLE);
	m_menuOutdoor.LoadMenu(IDR_MENU_OUTDOOR);
	m_menuIndoor.LoadMenu(IDR_MENU_INDOOR);

	SetIdleMode();

	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	CRect rect;
	CDC* pDC = m_pView->GetDC();
	pDC->GetClipBox(&rect);
	pDC->FillSolidRect(rect.left,rect.top,rect.right,rect.bottom,RGB(150,150,150));
	m_pView->ReleaseDC(pDC);

	// SplashScreen
	CSplashScreenFx *pSplash = new CSplashScreenFx();
	pSplash->Create(AfxGetMainWnd(), "S  A  B  E  R", 0 ,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);

	char szOldDir[NTL_MAX_DIR_PATH] = {0,};
	char szNewDir[NTL_MAX_DIR_PATH] = {0,};
	char szNewBMP[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
	::strcpy(szNewDir, szOldDir);
	::strcpy(szNewBMP, szOldDir);
	::strcat(szNewDir, "\\tool\\saber\\texture\\splash\\");
	::strcat(szNewBMP, "\\tool\\saber\\texture\\splash\\*.bmp");
	
	
	CFileFind finder;
	int BMPCnt = 0;
	BOOL bWorking = finder.FindFile(szNewBMP);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		BMPCnt++;
	}

	::SetCurrentDirectory(szNewDir);
	CString BMPName;

	::srand( (unsigned )time(NULL));
	int BMPIdx = rand() % BMPCnt;
	if(BMPIdx < 10)
	{
		BMPName.Format("SABER_SPLASH0%d.BMP", BMPIdx);
	}
	else
	{
		BMPName.Format("SABER_SPLASH%d.BMP", BMPIdx);
	}

	pSplash->SetBitmap(BMPName.GetBuffer(0), 12, 41, 80);

	::SetCurrentDirectory(szOldDir);
	
	pSplash->SetTextFont("Arial",120,CSS_TEXT_NORMAL);
	pSplash->SetTextRect(CRect(160,220,390,260));
	pSplash->CreateProgressBar(CRect(200,288,450,295), CSize(0, 2), PBS_SMOOTH);
	//pSplash->SetTextColor(RGB(180, 180, 180));
	pSplash->SetTextColor(RGB(255, 255, 255));
	pSplash->SetTextFormat(DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	pSplash->Show();	
	pSplash->SetActiveWindow();

	//Sleep(500);

	// SplashScreen	
	pSplash->SetText("ARTRIA 召還...");
	{
		m_DI.Init(m_pFrm->GetSafeHwnd());
	//	Sleep(1000);
	}
	pSplash->ProgressStep();

	pSplash->SetText("ARTRIA 契約...");
	{
		m_NtlWEApplication.Create(((CNtlWEFrm*)AfxGetMainWnd())->GetActiveView()->GetSafeHwnd());
	}
	pSplash->ProgressStep();

	pSplash->SetText("ARTRIA 起動...");
	{
	//	Sleep(1000);
	}
	pSplash->ProgressStep();

	pSplash->Hide();

	return TRUE;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RwChar Info[NTL_MAX_DIR_PATH] = {0,};

	sprintf_s(Info, NTL_MAX_DIR_PATH, "SABER Version %s", dGET_WORLD_PARAM()->WorldVer);
	GetDlgItem(IDC_SABER_VER)->SetWindowText(Info);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

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
void CNtlWEApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CNtlWEApp 메시지 처리기
void CNtlWEApp::SetDiffusePickerClr(CString R, CString G, CString B)
{
	m_pFrm->m_pTerrainPalette->UpdateData(TRUE);
	m_pFrm->m_pTerrainPalette->m_ClrDiffusePickerR = R;
	m_pFrm->m_pTerrainPalette->m_ClrDiffusePickerG = G;
	m_pFrm->m_pTerrainPalette->m_ClrDiffusePickerB = B;
	m_pFrm->m_pTerrainPalette->UpdateData(FALSE);
}

BOOL CNtlWEApp::OnIdle(LONG lCount)
{
	// do not render if the app is minimized
	if(m_pMainWnd->IsIconic())
		return FALSE;

	// give app. some rest for other stuff
	//Sleep(1);

	lCount = 0;
	//return CWinApp::OnIdle(lCount);

	
	m_DI.Update();

	dGET_MOUSE()->Update();

	m_NtlWEApplication.OnIdle();

	dGET_MOUSE()->Reset();

	__super::OnIdle(lCount);

	return TRUE;
}

BOOL CNtlWEApp::PreTranslateMessage(MSG* pMsg)
{
	if((CNtlWEFrm*)AfxGetMainWnd())
	{
		HWND hWnd = ((CNtlWEFrm*)AfxGetMainWnd())->GetActiveView()->GetSafeHwnd();
		if(hWnd == pMsg->hwnd)
			m_NtlWEApplication.WndProc(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

int CNtlWEApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_DI.Free();
	m_NtlWEApplication.Destroy();

	return CWinApp::ExitInstance();
}

//void CNtlWEApp::OnFinalRelease()
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	//m_NtlWEApplication.Destroy();
//
//	CWinApp::OnFinalRelease();
//}

void CNtlWEApp::SetIndoorMode()
{
	m_pFrm->SetMenu(&m_menuIndoor);
}

void CNtlWEApp::SetOutdoorMode()
{
	m_pFrm->SetMenu(&m_menuOutdoor);
}

void CNtlWEApp::SetIdleMode()
{
	m_pFrm->SetMenu(&m_menuIdle);
}