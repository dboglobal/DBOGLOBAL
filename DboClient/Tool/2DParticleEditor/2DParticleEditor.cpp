// 2DParticleEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "2DParticleEditor.h"
#include "MainFrm.h"

#include "2DParticleEditorDoc.h"
#include "2DParticleEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2DParticleEditorApp

BEGIN_MESSAGE_MAP(CMy2DParticleEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMy2DParticleEditorApp::OnAppAbout)
	ON_COMMAND(ID_ATTR_NEW, &CMy2DParticleEditorApp::OnAttrNew)
	ON_COMMAND(ID_ATTR_OPEN, &CMy2DParticleEditorApp::OnAttrOpen)
	ON_COMMAND(ID_ATTR_SAVE, &CMy2DParticleEditorApp::OnAttrSave)
END_MESSAGE_MAP()


// CMy2DParticleEditorApp construction

CMy2DParticleEditorApp::CMy2DParticleEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMy2DParticleEditorApp object

CMy2DParticleEditorApp theApp;


// CMy2DParticleEditorApp initialization

BOOL CMy2DParticleEditorApp::InitInstance()
{
	// 디버깅용
	/*_CrtSetBreakAlloc( 73443 );*/

	/*AllocConsole();
	freopen(_T("CONOUT$"), _T("w"), stdout );*/

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMy2DParticleEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMy2DParticleEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	HWND hWnd = ((CMainFrame*)AfxGetMainWnd())->GetActiveView()->GetSafeHwnd();

	m_ParticleApp.Create( hWnd );

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX }; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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

// App command to run the dialog
void CMy2DParticleEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMy2DParticleEditorApp message handlers

int CMy2DParticleEditorApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_ParticleApp.Destroy();

	return CWinApp::ExitInstance();
}


BOOL CMy2DParticleEditorApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	// 윈도우가 최소화 되어 있을 경우는 업데이트 하지 않는다.
	if(m_pMainWnd->IsIconic())
		return FALSE;


	m_ParticleApp.OnIdle();

	// 매 프레임 갱신을 위한 코드 : 항상 TRUE를 리턴한다.
	__super::OnIdle(lCount);
	return TRUE;
}

BOOL CMy2DParticleEditorApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_ParticleApp.PreTranslateMessage( pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam );

	return CWinApp::PreTranslateMessage(pMsg);
}

void CMy2DParticleEditorApp::OnAttrNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	
}

void CMy2DParticleEditorApp::OnAttrOpen()
{
	// TODO: Add command handler code here.
	char szFilter[] = "DBO 2D Particle Script(*.2ps)|*.2ps| Full File (*.*)|*.*|";
	CFileDialog dlg( TRUE, "2ps", "*.2ps", 0, szFilter );

	if( dlg.DoModal() == IDOK )
	{
		g_Doc->LoadAttr( dlg.GetPathName().GetBuffer() );
	}

}

void CMy2DParticleEditorApp::OnAttrSave()
{
	// TODO: Add the command handler code here.
	char szFilter[] = "DBO 2D Particle Script(*.2ps)|*.2ps| Full File (*.*)|*.*|";
	CFileDialog dlg( FALSE, "2ps", "*.2ps", 0, szFilter );

	if( dlg.DoModal() == IDOK )
	{
		gui::GetParticleDoc()->Save( dlg.GetPathName().GetBuffer() );
	}
}
