// Chaoz.cpp : Defines the class behavior for the application.
//

#include "stdafx.h"
#include "Chaoz.h"
#include "ChaozDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChaozApp

BEGIN_MESSAGE_MAP(CChaozApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChaozApp 생성

CChaozApp::CChaozApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CChaozApp 개체입니다.

CChaozApp theApp;


// CChaozApp 초기화

BOOL CChaozApp::InitInstance()
{
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

	// standard initialization
		// To reduce the size of the final executable file without using these features
		// specific initialization not needed below
		// routines must be removed.
		// Change the registry key where those settings are stored.
		// TODO: This string is the same as the name of your company or organization.
		// You need to modify it with the appropriate content.
	//SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	CChaozDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Click OK here to process when the dialog box is gone
		// Place the code.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Click [Cancel] here to process when dialog disappears
		// Place the code.
	}

	// FALSE to allow the application to exit without starting the application's message pump because the dialog is closed
	// returns.
	return FALSE;
}
