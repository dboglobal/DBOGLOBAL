#include "precomp_navi.h"
#include "NtlNaviLoadFinishCheckThread.h"
#include <process.h>
#include "NtlNaviLog.h"


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviLoadFinishCheckThread
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviLoadFinishCheckThread::CNtlNaviLoadFinishCheckThread( void )
{
	m_hLoadingFinish = NULL;

	m_bExit = false;
}

CNtlNaviLoadFinishCheckThread::~CNtlNaviLoadFinishCheckThread( void )
{
}

bool CNtlNaviLoadFinishCheckThread::Create( void )
{
	m_hLoadingFinish = CreateEvent( NULL, TRUE, FALSE, NULL );

	if ( NULL == m_hLoadingFinish )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating the loading finish check thread failed." );

		return false;
	}

	SetExit( false );

	return true;
}

void CNtlNaviLoadFinishCheckThread::Delete( void )
{
	SetExit( true );

	if ( m_hLoadingFinish )
	{
		CloseHandle( m_hLoadingFinish );
		m_hLoadingFinish = NULL;
	}
}

bool CNtlNaviLoadFinishCheckThread::Begin( void )
{
	ResetEvent( m_hLoadingFinish );

	SetExit( false );

	HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreaFuncCB, this, 0, NULL );

	if ( NULL == hThread )
	{
		return false;
	}

	Sleep( 15 );

	return true;
}

void CNtlNaviLoadFinishCheckThread::End( void )
{
	SetExit( true );

	SetEvent( m_hLoadingFinish );
}

HANDLE CNtlNaviLoadFinishCheckThread::GetLoadingFinishEvent( void )
{
	return m_hLoadingFinish;
}

bool CNtlNaviLoadFinishCheckThread::IsExit( void )
{
	m_clExitCS.Lock();
	bool bExit = m_bExit;
	m_clExitCS.Unlock();

	return bExit;
}

void CNtlNaviLoadFinishCheckThread::SetExit( bool bExit )
{
	m_clExitCS.Lock();
	m_bExit = bExit;
	m_clExitCS.Unlock();
}

unsigned int __stdcall CNtlNaviLoadFinishCheckThread::ThreaFuncCB( void* pParam )
{
	CNtlNaviLoadFinishCheckThread* pThis = (CNtlNaviLoadFinishCheckThread*)pParam;

	while ( !pThis->IsExit() )
	{
		pThis->ThreadCallBackFunc();

		Sleep( 15 );
	}

	_endthread();

	return 0;
}