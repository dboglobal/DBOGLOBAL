// ModelTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ModelTool.h"
#include "MainFrm.h"

#include "ModelToolDoc.h"
#include "ModelToolView.h"
#include "LeftTreeView.h"
#include "ModelToolApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModelToolApp

BEGIN_MESSAGE_MAP(CModelToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CModelToolApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CModelToolApp::OnFileOpen)	
END_MESSAGE_MAP()


// CModelToolApp construction

CModelToolApp::CModelToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CModelToolApp object

CModelToolApp theApp;


// CModelToolApp initialization

BOOL CModelToolApp::InitInstance()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

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
	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CModelToolDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CModelToolView));
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
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();

	HWND hWnd = ((CMainFrame*)AfxGetMainWnd())->GetViewWindow();

	m_ModelToolApplication.Create(hWnd);

    m_hAccel = ::LoadAccelerators(
        AfxGetResourceHandle(), 
        MAKEINTRESOURCE(IDR_MAINFRAME));

    

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

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
void CModelToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CModelToolApp message handlers


BOOL CModelToolApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class

	// 윈도우가 최소화 되어있으면 갱신하지 않는다.
	if(m_pMainWnd->IsIconic())
		return FALSE;

	m_ModelToolApplication.OnIdle();
	
    // 매프레임 갱신을 위해서 필요하다.
    __super::OnIdle(lCount);
    return TRUE;
}


int CModelToolApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	m_ModelToolApplication.Destroy();
    GdiplusShutdown(m_gdiplusToken);
	return CWinApp::ExitInstance();
}

void CModelToolApp::OnFileOpen()
{
	
}

CDocument* CModelToolApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: Add your specialized code here and/or call the base class		

	//CModelToolApplication::GetInstance()->LoadClump(W2A(lpszFileName));
	
	return CWinApp::OpenDocumentFile(lpszFileName);
}

BOOL CModelToolApp::PreTranslateMessage(MSG* pMsg)
{
    if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
        if(m_hAccel && ::TranslateAccelerator(pMsg->hwnd, m_hAccel, pMsg))
            return TRUE; 

    return CWinApp::PreTranslateMessage(pMsg);
}
