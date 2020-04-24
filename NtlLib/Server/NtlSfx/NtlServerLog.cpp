//***********************************************************************************
//
//	File		:	NtlServerLog.cpp
//
//	Begin		:	2007-03-07
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlServerLog.h"

#include <tchar.h>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const unsigned int SERVER_LOGDATA_RESERVED = 100;
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
DEFINE_DYNAMIC_MEMORYPOOL_THREADSAFE( CNtlServerLogData, SERVER_LOGDATA_RESERVED );
//-----------------------------------------------------------------------------------
#ifdef _WIN64
	DWORD64 CNtlServerLogData::curSequence = 0;
#else
	DWORD CNtlServerLogData::curSequence = 0;
#endif
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlServerLog::CNtlServerLog(void)
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlServerLog::~CNtlServerLog(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerLog::Init()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerLog::Destroy()
{
	CNtlServerLogData * pLogData = PopLog();
	while( pLogData )
	{
		SAFE_DELETE( pLogData );

		pLogData = PopLog();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlServerLog::Log(BYTE byLogChannel, bool bDate, LPCTSTR lpszFile, int nLine, LPCTSTR lpszFunc, LPCTSTR lpszText, ...)
{
	CNtlServerLogData * pLogData = new CNtlServerLogData;
	if( NULL == pLogData )
	{
		// 예외처리
		return;
	}


	int nBuffSize = pLogData->GetBufSize();
	int nWriteSize = 0;

	pLogData->dwSource = GetDefaultLogSource();
	pLogData->byChannel = byLogChannel;


	if( bDate )
	{
		SYSTEMTIME	systemTime;
		GetLocalTime( &systemTime );
		nWriteSize += _stprintf_s( pLogData->achLogText + nWriteSize, nBuffSize - nWriteSize, TEXT("%d-%02d-%02d %02d:%02d:%02d,"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond );
	}


	va_list args;
	va_start( args, lpszText );
	nWriteSize += _vstprintf_s( pLogData->achLogText + nWriteSize, nBuffSize - nWriteSize, lpszText, args );
	va_end( args );


	if( lpszFile )
	{
		nWriteSize += _stprintf_s( pLogData->achLogText + nWriteSize, nBuffSize - nWriteSize, TEXT(" file:%s\t(line:%d)"), lpszFile, nLine );
	}


	if( lpszFunc )
	{
		nWriteSize += _stprintf_s( pLogData->achLogText + nWriteSize, nBuffSize - nWriteSize, TEXT(" function[%s]\t"), lpszFunc );
	}


	pLogData->wStrLen = (WORD) nWriteSize;


	if( false == PushLog( pLogData ) )
	{
		SAFE_DELETE( pLogData );
	}

}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlServerLog::PushLog(CNtlServerLogData* pLogData)
{
	m_mutex.Lock();

	pLogData->sequence = ++(pLogData->curSequence);
	m_logQueue.push_back( pLogData );

	m_mutex.Unlock();

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlServerLogData* CNtlServerLog::PopLog()
{
	CNtlServerLogData * pLogData = NULL;

	m_mutex.Lock();

	if( false == m_logQueue.empty() )
	{
		pLogData = m_logQueue.front();
		m_logQueue.pop_front();
	}

	m_mutex.Unlock();


	return pLogData;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlServerLogData* CNtlServerLog::PeekLog()
{
	CNtlServerLogData * pLogData = NULL;

	m_mutex.Lock();

	if( false == m_logQueue.empty() )
	{
		pLogData = m_logQueue.front();
	}

	m_mutex.Unlock();


	return pLogData;
}
