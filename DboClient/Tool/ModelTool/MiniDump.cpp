//========================================================================
// Minidump.cpp : This is a crash trapper - similar to a UNIX style core dump
//
// Part of the GameCode Application
//
// GameCode is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete" by Mike McShaffry, published by
// Paraglyph Press. 
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: http://www.amazon.com/exec/obidos/ASIN/1932111751/gamecodecompl-20/
//
// There's also a companion web site at http://www.mcshaffry.com/GameCode/portal.php
//
// (c) Copyright 2003 Michael L. McShaffry
//
// This work is licensed under the Creative Commons Attribution-ShareAlike License. 
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/1.0/ 
// or send a letter to:
//      Creative Commons
//      559 Nathan Abbott Way
//      Stanford, California 94305, USA.
//
//========================================================================

//========================================================================
//  Content References in Game Coding Complete
// 
//  MiniDumper					- Chapter 12, page 425-429
//========================================================================

#include "StdAfx.h"
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>
#include "MiniDump.h"

#pragma comment(lib, "version.lib")



// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
									CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
									CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
									CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
									);

MiniDumper *MiniDumper::gpDumper = NULL;

MiniDumper::MiniDumper(bool headless)
{
	// Detect if there is more than one MiniDumper.
	assert( !gpDumper );

	if (!gpDumper)
	{
		::SetUnhandledExceptionFilter( Handler );
		gpDumper = this;
		m_bHeadless = headless;						// doesn't throw up a dialog, just writes the file.
	}
}

LONG MiniDumper::Handler( _EXCEPTION_POINTERS *pExceptionInfo )
{
	LONG retval = EXCEPTION_CONTINUE_SEARCH;

	if (!gpDumper)
	{
		return retval;
	}

	return gpDumper->WriteMiniDump(pExceptionInfo);
}

LONG MiniDumper::WriteMiniDump(_EXCEPTION_POINTERS *pExceptionInfo )
{
    time( &m_lTime );

	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	m_pExceptionInfo = pExceptionInfo;

	// You have to find the right dbghelp.dll. 
	// Look next to the EXE first since the one in System32 might be old (Win2k)
	
	HMODULE hDll = NULL;
	TCHAR szDbgHelpPath[_MAX_PATH];

	if (GetModuleFileName( NULL, m_szAppPath, _MAX_PATH ))
	{
		TCHAR *pSlash = _tcsrchr( m_szAppPath, '\\' );
		if (pSlash)
		{
			_tcscpy( m_szAppBaseName, pSlash + 1);
			*(pSlash+1) = 0;
		}

		_tcscpy( szDbgHelpPath, m_szAppPath );
        _tcscat( szDbgHelpPath, _T("DBGHELP.DLL") );
		hDll = ::LoadLibrary( szDbgHelpPath );
	}

	if (hDll==NULL)
	{
		// If we haven't found it yet - try one more time.
		hDll = ::LoadLibrary( _T("DBGHELP.DLL") );
	}

	LPCTSTR szResult = NULL;

	if (hDll)
	{
		MINIDUMPWRITEDUMP pMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pMiniDumpWriteDump)
		{
			TCHAR szScratch [USER_DATA_BUFFER_SIZE];

			VSetDumpFileName();

			// ask the user if they want to save a dump file
			_stprintf(szScratch, _T("There was an unexpected error:\n\n%s\nWould you like to save a diagnostic file?\n\nFilename: %s"), VGetUserMessage(), m_szDumpPath);
			if (m_bHeadless || (::MessageBox( NULL, szScratch, NULL, MB_YESNO )==IDYES))
			{
				// create the file
				HANDLE hFile = ::CreateFile( m_szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
											FILE_ATTRIBUTE_NORMAL, NULL );

				if (hFile!=INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

					ExInfo.ThreadId = ::GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;

					// write the dump
					BOOL bOK = pMiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, VGetUserStreamArray(), NULL );
					if (bOK)
					{
						szResult = NULL;
						retval = EXCEPTION_EXECUTE_HANDLER;
					}
					else
					{
						_stprintf( szScratch, _T("Failed to save dump file to '%s' (error %d)"), m_szDumpPath, GetLastError() );
						szResult = szScratch;
					}
					::CloseHandle(hFile);
				}
				else
				{
					_stprintf( szScratch, _T("Failed to create dump file '%s' (error %d)"), m_szDumpPath, GetLastError() );
					szResult = szScratch;
				}
			}
		}
		else
		{
			szResult = _T("DBGHELP.DLL too old");
		}
	}
	else
	{
		szResult = _T("DBGHELP.DLL not found");
	}

	if (szResult && !m_bHeadless)
		::MessageBox( NULL, szResult, NULL, MB_OK );

	TerminateProcess(GetCurrentProcess(), 0);
	
	// MLM Note: ExitThread will work, and it allows the MiniDumper to kill a crashed thread
	// without affecting the rest of the application. The question of the day:
	//   Is That A Good Idea??? Answer: ABSOLUTELY NOT!!!!!!!
	//
	//ExitThread(0);

	return retval;
}


void MiniDumper::VSetDumpFileName(void)
{
	_stprintf(m_szDumpPath, _T("%s%s.%ld.dmp"), m_szAppPath, m_szAppBaseName, m_lTime);
}

