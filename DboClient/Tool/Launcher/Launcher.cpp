// Launcher.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "io.h"


#include "Launcher.h"
#include "LauncherDlg.h"
#include "DataType.h"
#include "NtlLauncherConfig.h"
#include "NtlPatchManager.h"
#include "NtlHttpDownLoader.h"
#include "NtlRtPatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MUTEX_DBO           "DragonBall"
#define MUTEX_DBO_LAUNCHER  "DBO Launcher"

// CLauncherApp

BEGIN_MESSAGE_MAP(CLauncherApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLauncherApp 생성

CLauncherApp::CLauncherApp()
: m_bPatchStart(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CLauncherApp 개체입니다.

CLauncherApp theApp;


BOOL IsMultiOpen(const char *pExeName, BOOL bCreateMutex)
{
    if(bCreateMutex)
    {
        HANDLE hMutex = ::CreateMutex(NULL, FALSE, pExeName);
        if(hMutex) 
        {
            if(GetLastError() == ERROR_ALREADY_EXISTS)
                return TRUE;
        }
        else // NULL이면 이미 열려있는것이다.
        {
            return TRUE;
        }
    }
    else
    {
	    HANDLE hMutex = ::OpenMutex(NULL, FALSE, pExeName);
	    if(hMutex) // NULL이 아니면 이미 열려 있는 것이다.
		    return TRUE;
    }

	return FALSE;
}

// CLauncherApp 초기화
BOOL CLauncherApp::InitInstance() 
{
    if(IsMultiOpen(MUTEX_DBO, FALSE))
    {
        ::MessageBox(NULL, "DragonBall Online is already opened !!!", "DragonBall Online", MB_OK);
        return FALSE;
    }

    if(IsMultiOpen(MUTEX_DBO_LAUNCHER, TRUE))
    {
        ::MessageBox(NULL, "DragonBall Online Launcher is already opened !!!", "DragonBall Online", MB_OK);
        return FALSE;
    }

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

	AfxEnableControlContainer();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	
	//m_bCompletePatch
	m_bPatchStart = false;
	CNtlLauncherConfig::GetInstance()->Create(LAUNCHER_CONFIG);
	CNtlPatchManager::GetInstance()->Create();

	//CmdLine Tokenize
	CString strCmdLine = m_lpCmdLine;
	if(strCmdLine == LAUNCHER_CMD_LAUNCHER_PATCH)
	{
		CNtlPatchManager::GetInstance()->SetPatchState(PATCH_STATE_LAUNCHER);
	}
	
	if(CNtlPatchManager::GetInstance()->GetPatchState() == PATCH_STATE_NONE)
	{
		if( _access(LAUNCHER_PATCH_EXE_FILE_NAME, 0) == 0)
		{
			while(1)
			{
				if( DeleteFile(LAUNCHER_PATCH_EXE_FILE_NAME) )
					break;
			}
		}

		CLauncherDlg dlg;
		m_pMainWnd = &dlg;

        INT_PTR nResponse = dlg.DoModal();		
	}
	else
	{
		CLauncherDlg dlg;
		m_pMainWnd = &dlg;

		INT_PTR nResponse = dlg.DoModal();        
	}
	
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

int CLauncherApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(CNtlPatchManager::GetInstance()->GetPatchState() == PATCH_STATE_NEW_LAUNCHER)
	{
		CopyFile(LAUNCHER_CUR_EXE_FILE_NAME, LAUNCHER_PATCH_EXE_FILE_NAME, FALSE);
		CString strLauncherPatchCmd = LAUNCHER_CMD_LAUNCHER_PATCH;
		ShellExecute(NULL, NULL, LAUNCHER_PATCH_EXE_FILE_NAME, strLauncherPatchCmd, NULL, 0);
	}
	else if(CNtlPatchManager::GetInstance()->GetPatchState() == PATCH_STATE_LAUNCHER)
	{
		ShellExecute(NULL, NULL, LAUNCHER_CUR_EXE_FILE_NAME, NULL, NULL, 1);
	}
	
	CNtlPatchManager::GetInstance()->DestroyInstance();
	CNtlLauncherConfig::GetInstance()->DestroyInstance();
	
	return CWinApp::ExitInstance();
}

BOOL CLauncherApp::PumpMessage()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//화면이 처음 표시 된 후가 여기 밖에 찾을수가 없었다.
	if(m_bPatchStart == false)
	{
		m_bPatchStart = true;
		CNtlPatchManager::GetInstance()->DoAction();
	}

	return CWinApp::PumpMessage();
}
