// AutoPatchToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoPatchTool.h"
#include "AutoPatchToolDlg.h"
#include "PatchConfigManager.h"
#include "AutoPatchLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void OutLog(const char *pLog)
{
    CAutoPacthLog log;
    log.Out("AutoPatchLog.txt", pLog);
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


// CAutoPatchToolDlg dialog




CAutoPatchToolDlg::CAutoPatchToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoPatchToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoPatchToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoPatchToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CAutoPatchToolDlg message handlers

BOOL CAutoPatchToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//----------------------------------------------------------------------------
	// Mutex Check

	HANDLE hMutex = ::CreateMutex(NULL, FALSE, "AutoPatchBuilder");
	if(NULL == hMutex)
	{
		PostQuitMessage(0);
		return TRUE;
	}

	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		PostQuitMessage(0);
		return TRUE;
	}

	//----------------------------------------------------------------------------
	// command line 분석.
    // /fast - 파일 백업과 evtMaker 빌드를 제외한다.

	CString str = GetCommandLine();    
    int curPos = 0;
    BOOL bClient = FALSE;
    BOOL bServer = FALSE;
    BOOL bFastMode = FALSE;

    while(curPos != -1)
    {
        CString cmd = str.Tokenize(" ", curPos);
        if(cmd == "client")
        {
            bClient = TRUE;
        }
        else if(cmd == "server")
        {
            bServer = TRUE;
        }
        else if(cmd == "/fast")
        {
            bFastMode = TRUE;
        }        
    }
    
    if(bClient)
	{
		if(!GetPatchConfigManager()->LoadVersion("PatchVerList.XML"))
		{
            OutLog("Load PatchVerList.XML Fail!");

			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->LoadConfig("Auto Patch Config.xml"))
		{
            OutLog("Load Patch Config File Fail!");
			exit(1);
			return TRUE;
		}
		
		if(!GetPatchConfigManager()->CopySource(false))
		{
            OutLog("Copy Source Fail!");
			exit(1);
			return TRUE;
		}
		
        
		if(!GetPatchConfigManager()->TSEvtMakerBuild())
		{
            OutLog("TSEvtMakerBuild Fail!");
			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->SaveVersion("PatchVerList.XML"))
		{
            OutLog("SaveVersion Fail!");
			exit(1);
			return TRUE;
		}

		
		if(!GetPatchConfigManager()->SaveFTPUploadScript("FtpPatchUScript.txt"))
		{
            OutLog("SaveFTPUploadScript Fail!");
			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->SavePatchCommandScript("DBOPatch.cmd"))
		{
            OutLog("SavePatchCommandScript(DBOPatch.cmd) Fail!");            
			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->RtPatchBuild())
		{
            OutLog("RtPatchBuild Fail!");
			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->EndPatchBuild(bFastMode))
		{
            OutLog("EndPatchBuild Fail!");
			exit(1);
			return TRUE;
		}

		PostQuitMessage(0);
	}	
    else if(bServer)
	{
		if(!GetPatchConfigManager()->LoadServerConfig("Auto Patch Server Config.xml"))
		{
			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->MakeServerBuildFolder())
		{
			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->CopyServerSource(false))
		{
			exit(1);
			return TRUE;
		}

		if(!GetPatchConfigManager()->CopyServerSource(true))
		{
			exit(1);
			return TRUE;
		}
		
		PostQuitMessage(0);
	}
	
	return TRUE;  
}

void CAutoPatchToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoPatchToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoPatchToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

