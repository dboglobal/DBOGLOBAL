// DTEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "MainFrm.h"

#include "DTEditorDoc.h"
#include "DTEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDTEditorApp

BEGIN_MESSAGE_MAP(CDTEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDTEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDTEditorApp construction

CDTEditorApp::CDTEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDTEditorApp object

CDTEditorApp theApp;


// CDTEditorApp initialization

BOOL CDTEditorApp::InitInstance()
{
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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
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
		RUNTIME_CLASS(CDTEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDTEditorView));
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
    m_DirectApp.Create(hWnd);

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
void CDTEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CDTEditorApp message handlers


BOOL CDTEditorApp::OnIdle(LONG lCount)
{
	// 윈도우가 최소화 되어있으면 갱신하지 않는다.
   if(m_pMainWnd->IsIconic())
      return FALSE;

    m_DirectApp.OnIdle();

    // 매프레임 갱신을 위해서 필요하다.
    __super::OnIdle(lCount);
    return TRUE;
}

BOOL CDTEditorApp::PreTranslateMessage(MSG* pMsg)
{
	m_DirectApp.PreTranslateMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::PreTranslateMessage(pMsg);
}

int CDTEditorApp::ExitInstance()
{
	m_DirectApp.Destroy();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::ExitInstance();
}
