#include "StdAfx.h"
#include "ERSMCore.h"
#include "ERSMConfig.h"
#include "ERSMWorker.h"
#include "ERSMDef.h"

bool ERSMCore::m_bEndFlag = FALSE;
CTime ERSMCore::m_prevRunTime = CTime(2007,01,01,01,01,01);

ERSMCore::ERSMCore(void)
{
}

ERSMCore::~ERSMCore(void)
{
	Destroy();
}

bool ERSMCore::Init() 
{
	NCDefInitSGT(ERSMConfig);

	std::string strConfigFile = GetModulePath();
	strConfigFile += CONFIG_FILE_NAME;
		
	if(NCGetSGT(ERSMConfig)->ReadConfig(strConfigFile))
	{
		cout<<"# ERSM Config File Read Success...\n";
		cout<<"# Error Report Storage = "<<NCGetSGT(ERSMConfig)->GetERSFolder()<<"\n";
		cout<<"# Error Report RunTime Term = "<<NCGetSGT(ERSMConfig)->GetRunTime()<<"\n";
	}
	else
	{
		cout<<"# ERSM Config File Read Fail - " << strConfigFile <<"\n";
		return FALSE;
	}
	
	return TRUE;
}
void ERSMCore::Destroy() 
{
	m_bEndFlag = TRUE;
	NCDestroySGT(ERSMConfig);
}

bool ERSMCore::Run() 
{
	DWORD dwDummy = 0;
	
	HANDLE hThread = CreateThread(NULL, 0, WorkThread, NULL, NULL, &dwDummy);
	if(hThread)
	{
		cout<<"<< ERSM Run Thread Success!! >>\n";		
	}
	else
	{
		cout<<"<< ERSM Run Thread Fail!! >>\n";
		return FALSE;
	}

	WaitForSingleObject(hThread, INFINITE);

	return TRUE;
}

DWORD WINAPI ERSMCore::WorkThread( LPVOID pParam ) 
{
	pParam;

	while(!m_bEndFlag)
	{
		CTime currentTime = CTime::GetCurrentTime();
		CTime tempTime = m_prevRunTime + CTimeSpan(0, 0, NCGetSGT(ERSMConfig)->GetRunTime(), 0);
		if(currentTime >= tempTime)
		{
			m_prevRunTime = currentTime;

			CERSMWorker ersmWorker;
			ersmWorker.ProcessErrReports();
		}

		Sleep(1000);
	}

	return 0;
}

std::string ERSMCore::GetModulePath() 
{
	// 서비스 프로그램에서 GetCurrentDirectory를 하면 SYSTEM폴더를 반환한다
	// 이 함수는 서비스 프로그램이 실행된 경로를 반환한다.
	char szFilePath[_MAX_PATH] = {0,};
	char szDrive[_MAX_PATH] = {0,};
	char szDir[_MAX_PATH] = {0,};


	GetModuleFileName(NULL, szFilePath, _MAX_PATH);
	_splitpath_s(szFilePath, 
				 szDrive, _MAX_PATH,
				 szDir, _MAX_PATH,
				 NULL, NULL,
				 NULL, NULL);
	

	std::string strPath = szDrive;
	strPath += szDir;

	return strPath;
}