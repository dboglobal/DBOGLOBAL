//***********************************************************************************
//
//	File		:	NtlMiniDump.cpp
//
//	Begin		:	2007-03-05
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlMiniDump.h"

#include "NtlDebug.h"
#include "Dbghelp.h"

#include <stdio.h>
#include <tchar.h>

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess,
										 DWORD dwPid,
										 HANDLE hFile, 
										 MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);


LPTOP_LEVEL_EXCEPTION_FILTER s_previousExceptionFilter = NULL;

char CNtlMiniDump::m_szDumpPath[MAX_PATH + 1] = ".";
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS * pExceptionInfo)
{
	HMODULE	hDLL = LoadLibrary( TEXT( "DBGHELP.DLL") );
	if ( NULL == hDLL )
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}


	MINIDUMPWRITEDUMP pfnDump = (MINIDUMPWRITEDUMP) GetProcAddress( hDLL, "MiniDumpWriteDump" );
	if( NULL == pfnDump )
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}


	TCHAR		szDumpPath[MAX_PATH + 1] = { 0x00, };
	SYSTEMTIME	SystemTime;

	GetLocalTime( &SystemTime );
	_stprintf_s( szDumpPath, TEXT("%s\\%04d-%02d-%02d %02d_%02d_%02d.dmp"), CNtlMiniDump::GetDumpPath(), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond );

	HANDLE hFile = CreateFile(	szDumpPath,
								GENERIC_WRITE,
								FILE_SHARE_WRITE,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	if( hFile == INVALID_HANDLE_VALUE )
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}


	_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

	MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
	MiniDumpExceptionInfo.ExceptionPointers	= pExceptionInfo;
	MiniDumpExceptionInfo.ClientPointers	= NULL;

	if( FALSE == pfnDump(	GetCurrentProcess(),
							GetCurrentProcessId(), 
							hFile, 
#if defined(_DEBUG)
							MiniDumpWithFullMemory, 
#else
							MiniDumpNormal,
#endif
							&MiniDumpExceptionInfo, 
							NULL, 
							NULL ) )
	{
		CloseHandle( hFile );
		return EXCEPTION_EXECUTE_HANDLER;
	}


	CloseHandle( hFile );

	return EXCEPTION_CONTINUE_SEARCH;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlMiniDump::CNtlMiniDump(void)
{
	Start();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlMiniDump::~CNtlMiniDump(void)
{
	Stop();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlMiniDump::Start()
{
	SetErrorMode( SEM_FAILCRITICALERRORS );

	s_previousExceptionFilter = SetUnhandledExceptionFilter( UnHandledExceptionFilter );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMiniDump::Stop()
{
	SetUnhandledExceptionFilter( s_previousExceptionFilter );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlMiniDump::Snapshot()
{
	_EXCEPTION_POINTERS exceptionPointers;
	ZeroMemory( &exceptionPointers, sizeof( exceptionPointers ) );

	_EXCEPTION_RECORD exceptionRecord;
	ZeroMemory( &exceptionRecord, sizeof( exceptionRecord ) );

	CONTEXT contextRecord;
	ZeroMemory( &contextRecord, sizeof( contextRecord ) );
	contextRecord.ContextFlags = CONTEXT_ALL;//CONTEXT_FULL | CONTEXT_CONTROL | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS | CONTEXT_FLOATING_POINT;


	exceptionPointers.ContextRecord = &contextRecord;
	exceptionPointers.ExceptionRecord = &exceptionRecord;


	// Get Thread Context
	if( FALSE == GetThreadContext( GetCurrentThread(), &contextRecord ) )
	{
		NTL_ASSERT( 0 );
		return;
	}

	// Set Virtual Exception Record
	exceptionRecord.ExceptionCode;
	exceptionRecord.ExceptionFlags;
	exceptionRecord.ExceptionRecord;
#if defined(_AMD64_)
	exceptionRecord.ExceptionAddress = (PVOID) contextRecord.Rip;
#else
	exceptionRecord.ExceptionAddress = (PVOID)(DWORD_PTR)contextRecord.Eip;
#endif
	exceptionRecord.NumberParameters;
	exceptionRecord.ExceptionInformation;

	UnHandledExceptionFilter( &exceptionPointers );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlMiniDump::UnitTest()
{
	class CTest
	{
	public:
		int m_nNumber;
	};


	CNtlMiniDump::Start();

	
	CTest * pTest = (CTest*) NULL;

	pTest->m_nNumber = 1;
	

	CNtlMiniDump::Stop();

	return 0;
}

void CNtlMiniDump::SetDumpPath( const char * lptFilePath )
{
	_tcscpy_s( m_szDumpPath, MAX_PATH, lptFilePath );

	return;
}