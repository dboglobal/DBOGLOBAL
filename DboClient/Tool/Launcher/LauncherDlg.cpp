// LauncherDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"


#include "DataType.h"
#include "NtlPatchManager.h"
#include "NtlHttpDownLoader.h"
#include "NtlRtPatcher.h"
#include "NtlLauncherConfig.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_PROGRESS_RANGE 1000

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CBitmapDlg
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

CAboutDlg::CAboutDlg() : CBitmapDlg(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBitmapDlg)
END_MESSAGE_MAP()


// CLauncherDlg 대화 상자




CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CBitmapDlg(CLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_nDownValue = m_nPatchValue = 0;
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
    CBitmapDlg::DoDataExchange(pDX);    
    DDX_Control(pDX, IDC_STATIC_FILE_DOWN, m_StaticFileDown);    
    DDX_Control(pDX, IDC_EXPLORER_INFORMATION, m_ExplorerInformation);
    DDX_Control(pDX, IDC_BUTTON_GAME_START, m_ButtonGameStart);
    DDX_Control(pDX, IDC_BUTTON_CANCEL, m_ButtonCancel);
    DDX_Control(pDX, IDC_STATIC_ERROR, m_StaticInfoOutput);    
    DDX_Control(pDX, IDC_STATIC_LOG, m_StaticLog);
    DDX_Control(pDX, IDC_STATIC_PATCH, m_StaticFIlePatch);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CBitmapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_GAME_START, &CLauncherDlg::OnBnClickedButtonGameStart)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CLauncherDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CLauncherDlg 메시지 처리기

BOOL CLauncherDlg::OnInitDialog()
{
    CBitmapDlg::OnInitDialog();

    MoveWindow(0, 0, 700, 500);

    // background image setting
    EnableEasyMove();                    // enable moving of the dialog by clicking anywhere in the dialog
    SetBitmap(IDB_BACK);           // set background bitmap
    SetStyle(LO_RESIZE);                 // resize dialog to the size of the bitmap
    SetTransparentColor(RGB(0, 255, 0)); // set green as the transparent color

    int nScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - 350;
    int nScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - 250;

    MoveWindow(nScreenX, nScreenY, 700, 500);
   
    m_ButtonCancel.SetButtonImg(IDB_CLOSE_UP, IDB_CLOSE_DOWN, IDB_CLOSE_FOC);
    m_ButtonGameStart.SetButtonImg(IDB_START_UP, IDB_START_DOWN, IDB_START_FOC, IDB_START_DISABLE);

    m_ButtonCancel.MoveWindow(681, 5, 14, 14);
    m_ButtonGameStart.MoveWindow(570, 371, 124, 124);

    m_nIDProgress1 = AddBitmap(IDB_PROGRESS,85, 420, 483 ,7);
    m_nIDProgress2 = AddBitmap(IDB_PROGRESS,85, 448, 483 ,7);

    SetBltRateX(m_nIDProgress1, 0, MAX_PROGRESS_RANGE);
    SetBltRateX(m_nIDProgress2, 0, MAX_PROGRESS_RANGE);
    
    m_StaticFileDown.MoveWindow(85, 428, 483, 14);    
    m_StaticFIlePatch.MoveWindow(85, 459, 483, 14);
    m_StaticInfoOutput.MoveWindow(0, 2, 640, 14);        
    m_StaticLog.MoveWindow(85, 459, 483, 14);    
    m_StaticLog.m_bIsLog = TRUE;

    m_StaticFileDown.SetWindowText("");
    m_StaticInfoOutput.SetWindowText("");
    m_StaticLog.SetWindowText("");
    m_StaticFIlePatch.SetWindowText("");
    
    m_ExplorerInformation.MoveWindow(17, 34, 407, 370);    

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CString strHtmlInformation;
	strHtmlInformation = CNtlLauncherConfig::GetInstance()->GetHtmlInfoFullPathFileName();
	m_ExplorerInformation.Navigate(strHtmlInformation, NULL, NULL, NULL, NULL);
	
	CNtlPatchManager::GetInstance()->LinkEvent(this, &CLauncherDlg::PatchManagerCallBack);

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CBitmapDlg::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBitmapDlg::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CLauncherDlg::PatchManagerCallBack(unsigned int uiEventID, unsigned int pParam)
{
	static int s_nNumberOfFiles = 0;
	static int s_nPatchedFiles = 0;
	
	if(uiEventID == EVENT_PM_PENDING_DESTROY)
	{
		CBitmapDlg::DestroyWindow();
	}
	else if( uiEventID == EVENT_PM_START)
	{
        SetBltRateX(m_nIDProgress2, 0, MAX_PROGRESS_RANGE, TRUE);
		m_ButtonGameStart.EnableWindow(FALSE);
	}
	else if( uiEventID == EVENT_PM_COMPLETE)
	{
        SetBltRateX(m_nIDProgress2, MAX_PROGRESS_RANGE, MAX_PROGRESS_RANGE);

        // 임시 폴더 삭제        
        SHFILEOPSTRUCT FileOp = {0};        
        FileOp.hwnd = NULL;
        FileOp.wFunc = FO_DELETE;       // 삭제 속성 설정
        FileOp.pFrom = NULL;
        FileOp.pTo = NULL;
        FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI; // 확인메시지가 안뜨도록 설정
        FileOp.fAnyOperationsAborted = false;
        FileOp.hNameMappings = NULL;
        FileOp.lpszProgressTitle = NULL;
        FileOp.pFrom = PATCH_CLIENT_FILE_TEMP_PATH;
        SHFileOperation(&FileOp);       // 삭제 작업        
        
		m_ButtonGameStart.EnableWindow(TRUE);
	}
	else if( uiEventID == EVENT_PM_LAST_UPDATE_INFO)
	{
		EventPMLastUpdateInfoMsg *pLastUpdateInfoMsg = (EventPMLastUpdateInfoMsg *)pParam;

		CString strLastUpdate;
        strLastUpdate.Format("CurrentVersion: Client: %s(%s), Launcher: %s", pLastUpdateInfoMsg->pClientVersion,
                                                                             CNtlPatchHistory::GetInstance()->GetPatchFileDate(),
                                                                             pLastUpdateInfoMsg->pLauncherVersion);

        m_StaticInfoOutput.EraseBack(this);
		m_StaticInfoOutput.SetWindowTextA(strLastUpdate);
	}
	else if( uiEventID == EVENT_DL_COMPLETE_THIS_FILE )
	{
		EventDLCompleteThisFileMsg *pDLCompleteThisFileMsg = (EventDLCompleteThisFileMsg *)pParam;
		float fRatio = float(pDLCompleteThisFileMsg->uiComplete) / float(pDLCompleteThisFileMsg->uiFileSize);
		float fcurPos = (float)MAX_PROGRESS_RANGE * fRatio;
		//m_ProgressFileDown.SetPos((int)fcurPos);        
        SetBltRateX(m_nIDProgress1, (int)fcurPos, MAX_PROGRESS_RANGE);
		
		static int nRenderCount = 0;
		if(nRenderCount >= 180)
		{
			nRenderCount = 0;
			
			CString strBuffer;
			strBuffer.Format("DownLoadingFile:%s    %4.2fMb%s%4.2fMb", pDLCompleteThisFileMsg->pszFileName, 
			float(pDLCompleteThisFileMsg->uiComplete)/(1024.f * 1024.f), "/", 
			float(pDLCompleteThisFileMsg->uiFileSize)/(1024.f * 1024.f) );
            m_StaticFileDown.EraseBack(this);    
			m_StaticFileDown.SetWindowTextA(strBuffer);
		}
		nRenderCount++;

		if(pDLCompleteThisFileMsg->uiComplete == pDLCompleteThisFileMsg->uiFileSize)
		{
            m_StaticFileDown.EraseBack(this);
            m_StaticFileDown.SetWindowTextA("DownLoad Complete");
		}        
	}
	else if( uiEventID == EVENT_DL_ERROR_SERVER_CONNECT)
	{
        m_StaticLog.EraseBack(this);
        m_StaticLog.SetWindowText(ERROR_PATCH_SERVER_CONNECT);
	}
	else if( uiEventID == EVENT_DL_ERROR_SERVER_FILE_DOWN)
	{
		EventDLTextMsg *pTextMsg = (EventDLTextMsg *)pParam;

		CString strError = pTextMsg->pszMessage;
		strError += ERROR_PATCH_SERVER_FILE_DOWN;
		
        m_StaticLog.EraseBack(this);
        m_StaticLog.SetWindowText(strError);
	}
	else if( uiEventID == EVENT_PH_RT_WARNING_MESSAGE_HEADER ||
		uiEventID == EVENT_PH_RT_WARNING_MESSAGE_TEXT ||
		uiEventID == EVENT_PH_RT_ERROR_MESSAGE_HEADER ||
		uiEventID == EVENT_PH_RT_ERROR_MESSAGE_TEXT )
	{
		EventPHTextMsg *pTextMsg = (EventPHTextMsg *)pParam;
		CString strBuffer;
		strBuffer = pTextMsg->pszMessage;

        m_StaticLog.EraseBack(this);
		m_StaticLog.SetWindowTextA(strBuffer);
	}
	else if(uiEventID == EVENT_PH_RT_FILE_PATCH_START )
	{
		EventPHTextMsg *pTextMsg = (EventPHTextMsg *)pParam;
		CString strBuffer;

		strBuffer.Format("%s  %d%s", pTextMsg->pszMessage, int((float)s_nPatchedFiles/(float)s_nNumberOfFiles*100.f),"%");		
        m_StaticLog.EraseBack(this);
        m_StaticLog.SetWindowTextA(strBuffer);
		s_nPatchedFiles++;

		float fRatio = (float)s_nPatchedFiles/(float)s_nNumberOfFiles;
		
		float fcurPos = (float)MAX_PROGRESS_RANGE * fRatio;	
        SetBltRateX(m_nIDProgress2, (int)fcurPos, MAX_PROGRESS_RANGE);
	}
	else if( uiEventID == EVENT_PH_RT_NUMBER_OF_FILES)
	{
		EventPHNumberOfFilesMsg *pPHNumberOfFilesMsg = (EventPHNumberOfFilesMsg *)pParam;

		s_nNumberOfFiles = pPHNumberOfFilesMsg->nNumberOfFiles;
		s_nPatchedFiles = 0;
	}
	else if(uiEventID == EVENT_PH_RT_COMPLETE_THIS_FILE)
	{
		//EventPHCompleteThisFileMsg *pCompleteThisFileMsg = (EventPHCompleteThisFileMsg *)pParam;
		//if(pCompleteThisFileMsg->uiComplete >= 100)
			//s_nPatchedFiles++;
	}
	else if(uiEventID == EVENT_PH_RT_CURRENT_FILE_PATCH_COMPLETE)
	{
        m_StaticLog.EraseBack(this);
        m_StaticLog.SetWindowTextA("DBO Patch Complete!!");

        CString strLastUpdate;
        strLastUpdate.Format("CurrentVersion: Client: %s(%s), Launcher: %s", CNtlPatchHistory::GetInstance()->GetCurVerClient()->Get(), 
                                                                             CNtlPatchHistory::GetInstance()->GetPatchFileDate(),
                                                                             CNtlPatchHistory::GetInstance()->GetCurVerLauncher()->Get());
        m_StaticInfoOutput.EraseBack(this);
        m_StaticInfoOutput.SetWindowTextA(strLastUpdate);
	}

	if( uiEventID == EVENT_PH_DEFAULT_MSG_PUMP ||
		uiEventID == EVENT_DL_DEAFULT_MSG_PUMP)
	{
		MSG msg;
		for( int MsgCount = 10;
			 MsgCount && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
			 MsgCount--)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	return 0;
}

BOOL CLauncherDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	
	return CBitmapDlg::DestroyWindow();
}

void CLauncherDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( CNtlPatchManager::GetInstance()->GetActive() == TRUE &&
		CNtlPatchManager::GetInstance()->GetPendingDestory() == FALSE)
		CNtlPatchManager::GetInstance()->SetAbort(TRUE);
	else
		CBitmapDlg::OnClose();
}

void CLauncherDlg::OnBnClickedButtonGameStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(CNtlPatchManager::GetInstance()->GetActive() == FALSE)
	{
        CBitmapDlg::DestroyWindow();

		//; Client 실행
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			
			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );			

			TCHAR szCurDir[4096];
			GetCurrentDirectory( 4096, szCurDir );

			CString strFullPath = szCurDir;
			AttachBackSlash( strFullPath );
			strFullPath += LAUNCHER_MAIN_EXE_FILE_NAME;

			if( !CreateProcess( NULL, strFullPath.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
			{				
                DWORD dwErr = GetLastError();

                char* lpMsgBuf;
                FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR) (void**)&lpMsgBuf, 0, NULL );

                // Process any inserts in lpMsgBuf.
                // ...
                // Display the string.

                CString strMsg;
                CString strNum;
                strNum.Format( "%d", dwErr );                
                strMsg = "DBO.exe Run File Error\r\n";
                strMsg += "Error Message.\r\n\r\n";
                strMsg += lpMsgBuf;
                strMsg += "Error Number : ";
                strMsg += strNum;

                AfxMessageBox( strMsg, MB_OK | MB_ICONINFORMATION );
                // Free the buffer.
                LocalFree( (void*)lpMsgBuf );
			}

			CloseHandle( pi.hProcess );
			pi.hProcess = NULL;

		//		ShellExecute(NULL, NULL, LAUNCHER_MAIN_EXE_FILE_NAME, LAUNCHER_CMD_MAIN, NULL, 1);
	}
}

void CLauncherDlg::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( CNtlPatchManager::GetInstance()->GetActive() == TRUE &&
		CNtlPatchManager::GetInstance()->GetPendingDestory() == FALSE)
		CNtlPatchManager::GetInstance()->SetAbort(TRUE);
	else
		CBitmapDlg::DestroyWindow();
}


void CLauncherDlg::AttachBackSlash( CString& strString )
{
	if ( strString.IsEmpty() )
	{
		strString += _T( "\\" );
	}
	else
	{
		if ( strString[strString.GetLength() - 1] != _T( '\\' ) )
		{
			strString += _T( "\\" );
		}
	}
}
