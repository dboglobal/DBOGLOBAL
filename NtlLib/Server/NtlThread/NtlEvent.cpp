//***********************************************************************************
//
//	File		:	NtlEvent.cpp
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Event 동기화 오브젝트 클래스
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlEvent.h"
#include "NtlThreadException.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlEvent::CNtlEvent()
:
m_hEvent( INVALID_HANDLE_VALUE )
{
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if( NULL == m_hEvent )
	{
		DWORD rc = GetLastError();
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_EVENT_CREATE, rc);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlEvent::~CNtlEvent(void)
{
	if( false == CloseHandle( m_hEvent ) )
	{
		DWORD rc = GetLastError();
		THROW_THREAD_EXCEPTION((int)eTHREAD_ERR_EVENT_DESTROY, (int)rc);
	}

	m_hEvent = INVALID_HANDLE_VALUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlEvent::Reset()
{
	if( false == ResetEvent( m_hEvent ) )
	{
		DWORD rc = GetLastError();
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_EVENT_RESET, rc);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlEvent::Wait()
{
	if( WAIT_FAILED == WaitForSingleObject( m_hEvent, INFINITE ) )
	{
		DWORD rc = GetLastError();
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_EVENT_WAIT, rc);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlEvent::Wait( unsigned int millisecs )
{
	DWORD rc = NO_ERROR;
	DWORD status = WaitForSingleObject( m_hEvent, millisecs );

	if( WAIT_TIMEOUT == status  )
	{
		rc = ERROR_TIMEOUT;
	}
	else if( WAIT_FAILED == status )
	{
		rc = GetLastError();
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_EVENT_WAIT, rc);
	}

	return rc;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlEvent::Notify()
{
	if( !SetEvent( m_hEvent ) )
	{
		DWORD rc = GetLastError();
		THROW_THREAD_EXCEPTION(eTHREAD_ERR_EVENT_NOTIFY, rc);
	}
}
