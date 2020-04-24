//***********************************************************************************
//
//	File		:	NtlPerformance.cpp
//
//	Begin		:	2007-02-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "stdAfx.h"
#include "NtlPerformance.h"

#include "NtlError.h"

#include <stdlib.h>
#include <stdio.h>
#include <psapi.h>

#pragma comment(lib, "Psapi")


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPerformance::CNtlPerformance(void)
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPerformance::~CNtlPerformance(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPerformance::Init()
{
	m_hProcess = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPerformance::Create(HANDLE hProcess /* = NULL */)
{
	char szLogFilePath[16 + 1] = { 0x00, };
	char szLogFileNameFormat[25 + 1] = { 0x00, };

	char szProcessFullName[MAX_PATH + 1] = "<unknown>";
	char* lpszProcessName = NULL;

	m_hProcess = hProcess;

	if( NULL == m_hProcess )
	{
		m_hProcess = GetCurrentProcess();
		if( NULL == m_hProcess )
		{
			return GetLastError();
		}
	}


	::GetSystemInfo( &m_systemInfo );

	GlobalMemoryStatusEx( &m_memoryStatus );

	// Initializes 'szLogFileNameFormat'.
	{
		SYSTEMTIME localTime;
		GetLocalTime(&localTime);

		sprintf_s<sizeof(szLogFileNameFormat)>(
											szLogFileNameFormat,
											"%%s\\%04u%02u%02u_%02u%02u%02u_%%s.csv",
											localTime.wYear,
											localTime.wMonth,	
											localTime.wDay,
											localTime.wHour,
											localTime.wMinute,
											localTime.wSecond);
	}

	// Initializes 'szProcessFullName' and 'lpszProcessName'.
	{
		HMODULE hMod = NULL;
		DWORD cbNeeded = 0;

		int rc = EnumProcessModules( m_hProcess, &hMod, sizeof(hMod), &cbNeeded );
		if ( ERROR_SUCCESS == rc )
		{
			return 0;
		}


		DWORD dwSize = GetModuleBaseName( m_hProcess, hMod, szProcessFullName, MAX_PATH );
		if( 0 == dwSize )
		{
			return 0;
		}


		lpszProcessName = strtok_s( szProcessFullName, ".", &lpszProcessName );
		if( NULL == lpszProcessName )
		{
			return 0;
		}
	}

	if (FALSE == ::CreateDirectory("PerformanceLog", NULL))
	{
		DWORD dwLastError = ::GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			strcpy_s<_countof(szLogFilePath)>(szLogFilePath, ".");
		}
		else
		{
			strcpy_s<_countof(szLogFilePath)>(szLogFilePath, "PerformanceLog");
		}
	}
	else
	{
		strcpy_s<_countof(szLogFilePath)>(szLogFilePath, "PerformanceLog");
	}

	// Now lets PDH objects begin.

	CNtlString strLogFileName;

	{
		char szPdhPath[MAX_PATH + 1] = { 0x00, };
		sprintf_s(szPdhPath, MAX_PATH, "\\Process(%s)\\%% Processor Time", lpszProcessName );

		strLogFileName.Format(szLogFileNameFormat, szLogFilePath, "ProcessUsage");
		m_pdhProcessUsage.Prepare();
		m_pdhProcessUsage.RegisterCounter(szPdhPath);
		m_pdhProcessUsage.Start(strLogFileName.c_str());
	}

	{
		strLogFileName.Format(szLogFileNameFormat, szLogFilePath, "SystemProcessorUsage");
		m_pdhSystemProcessorUsage.Prepare();
		m_pdhSystemProcessorUsage.RegisterCounter( "\\Processor(_Total)\\% Processor Time" );
		m_pdhSystemProcessorUsage.Start(strLogFileName.c_str());
	}

	{
		char szPdhPath[MAX_PATH + 1] = { 0x00, };

		strLogFileName.Format(szLogFileNameFormat, szLogFilePath, "MemoryUsage");
		m_pdhMemoryUsage.Prepare();

		m_pdhMemoryUsage.RegisterCounter("\\Memory\\Available Bytes");

		sprintf_s(szPdhPath, MAX_PATH, "\\Process(%s)\\Page File Bytes", lpszProcessName);
		m_pdhMemoryUsage.RegisterCounter(szPdhPath);

		m_pdhMemoryUsage.RegisterCounter("\\Memory\\Page Faults/sec");

		sprintf_s(szPdhPath, MAX_PATH, "\\Process(%s)\\Page Faults/sec", lpszProcessName);
		m_pdhMemoryUsage.RegisterCounter(szPdhPath);

		sprintf_s(szPdhPath, MAX_PATH, "\\Process(%s)\\Virtual Bytes", lpszProcessName);
		m_pdhMemoryUsage.RegisterCounter(szPdhPath);

		sprintf_s(szPdhPath, MAX_PATH, "\\Process(%s)\\Private Bytes", lpszProcessName);
		m_pdhMemoryUsage.RegisterCounter(szPdhPath);

		sprintf_s(szPdhPath, MAX_PATH, "\\Process(%s)\\Working Bytes", lpszProcessName);
		m_pdhMemoryUsage.RegisterCounter(szPdhPath);


		m_pdhMemoryUsage.Start(strLogFileName.c_str());
	}


	return ERROR_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPerformance::Destroy()
{
	m_pdhProcessUsage.Stop();

	m_pdhSystemProcessorUsage.Stop();

	m_pdhMemoryUsage.Stop();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
MEMORYSTATUSEX * CNtlPerformance::GetMemoryStatus(bool bRefresh /* = true */)
{
	if( bRefresh )
	{
		GlobalMemoryStatusEx( &m_memoryStatus );
	}

	return &m_memoryStatus;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPerformance::UpdateLog()
{
	m_pdhProcessUsage.UpdateLog();

	m_pdhSystemProcessorUsage.UpdateLog();

	m_pdhMemoryUsage.UpdateLog();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
size_t CNtlPerformance::GetProcessMemoryUsage()
{
	if ( NULL == m_hProcess )
	{
		return 0;
	}


	PROCESS_MEMORY_COUNTERS pmc;
	if ( 0 == GetProcessMemoryInfo( m_hProcess, &pmc, sizeof(pmc)) )
	{
		return 0;
	}


	return pmc.WorkingSetSize;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD  CNtlPerformance::GetProcessProcessorLoad()
{
	if( NULL == m_hProcess )
	{
		return 0;
	}


	return m_pdhProcessUsage.GetValue();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD  CNtlPerformance::GetSystemProcessorLoad()
{
	if( NULL == m_hProcess )
	{
		return 0;
	}


	return m_pdhSystemProcessorUsage.GetValue();
}

