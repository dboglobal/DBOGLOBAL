#include "precomp_navi.h"
#include "NtlNaviLoadingQueue.h"
#include <process.h>


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviLoadEntity
//
//////////////////////////////////////////////////////////////////////////


bool CNtlNaviLoadingEntity::IsError( void )
{
	m_csError.Lock();
	bool bError = m_bError;
	m_csError.Unlock();

	return bError;
}

void CNtlNaviLoadingEntity::SetError( bool bError )
{
	m_csError.Lock();
	m_bError = bError;
	m_csError.Unlock();
}


//////////////////////////////////////////////////////////////////////////
//
//	CNtlLoadingQueue
//
//////////////////////////////////////////////////////////////////////////


CNtlLoadingQueue* CNtlLoadingQueue::s_pLoadingQueue = NULL;


CNtlLoadingQueue* CNtlLoadingQueue::GetInstance( void )
{
	return s_pLoadingQueue;
}


CNtlLoadingQueue::CNtlLoadingQueue( void )
{
	s_pLoadingQueue = this;

	m_bCreated = false;

	m_bExit = false;

	m_hEvent = NULL;

	memset( m_arhThread, 0, sizeof( m_arhThread ) );
}

CNtlLoadingQueue::~CNtlLoadingQueue( void )
{
	Delete();

	s_pLoadingQueue = NULL;
}

bool CNtlLoadingQueue::Create( void )
{
	Delete();

	m_bCreated = false;

	m_bExit = false;

	m_hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	if ( NULL == m_hEvent )
	{
		goto error;
	}

	for ( int i = 0; i < eMAX_THREAD_CNT; ++i )
	{
		m_arhThread[i] = (HANDLE)_beginthreadex( NULL, 0, &ThreaFuncCB, this, 0, NULL );

		if ( NULL == m_arhThread[i] )
		{
			goto error;
		}
	}

	Sleep( 15 );

	m_bCreated = true;

	return true;

error:

	for ( int i = 0; i < eMAX_THREAD_CNT; ++i )
	{
		if ( m_arhThread[i] )
		{
			TerminateThread( m_arhThread[i], 0 );
			m_arhThread[i] = NULL;
		}
	}

	if ( m_hEvent )
	{
		CloseHandle( m_hEvent );
		m_hEvent = NULL;
	}

	m_bExit = false;

	m_bCreated = false;

	return false;
}

void CNtlLoadingQueue::Delete( void )
{
	if ( m_bCreated )
	{
		SetExit();

		WaitForMultipleObjectsEx( eMAX_THREAD_CNT, m_arhThread, TRUE, INFINITE, FALSE );

		Sleep( 15 );

		for ( int i = 0; i < eMAX_THREAD_CNT; ++i )
		{
			if ( m_arhThread[i] )
			{
				CloseHandle( m_arhThread[i] );
				m_arhThread[i] = NULL;
			}
		}

		if ( m_hEvent )
		{
			CloseHandle( m_hEvent );
			m_hEvent = NULL;
		}

		m_bExit = false;

		m_bCreated = false;
	}
}

bool CNtlLoadingQueue::IsExit( void )
{
	m_clExitCS.Lock();
	bool bExit = m_bExit;
	m_clExitCS.Unlock();

	return bExit;
}

void CNtlLoadingQueue::SetExit( void )
{
	m_clExitCS.Lock();
	m_bExit = true;
	m_clExitCS.Unlock();

	SetEvent( m_hEvent );
}

bool CNtlLoadingQueue::IsEmptyEntityToLoad( void )
{
	m_clEntityToLoadCS.Lock();
	bool bEmpty = m_defEntityToLoadList.empty();
	m_clEntityToLoadCS.Unlock();

	return bEmpty;
}

void CNtlLoadingQueue::AttachEntityToLoad( CNtlNaviLoadingEntity* pEntity )
{
	m_clEntityToLoadCS.Lock();
	m_defEntityToLoadList.push_back( pEntity );
	m_clEntityToLoadCS.Unlock();

	SetEvent( m_hEvent );
}

bool CNtlLoadingQueue::DetachEntityToLoad( CNtlNaviLoadingEntity* pEntity )
{
	m_clEntityToLoadCS.Lock();

	bool bRet = false;

	for ( vecdef_ENTITY_LIST::iterator it = m_defEntityToLoadList.begin(); it != m_defEntityToLoadList.end(); ++it )
	{
		CNtlNaviLoadingEntity* pRetEntity = *it;

		if ( pEntity == pRetEntity )
		{
			m_defEntityToLoadList.erase( it );

			bRet = true;

			break;
		}
	}

	m_clEntityToLoadCS.Unlock();

	return bRet;
}

CNtlNaviLoadingEntity* CNtlLoadingQueue::TakeEntityToLoad( void )
{
	m_clEntityToLoadCS.Lock();

	CNtlNaviLoadingEntity* pEntity = NULL;

	if ( !m_defEntityToLoadList.empty() )
	{
		vecdef_ENTITY_LIST::iterator it = m_defEntityToLoadList.begin();

		pEntity = *it;

		m_defEntityToLoadList.erase( it );
	}

	m_clEntityToLoadCS.Unlock();

	return pEntity;
}

void CNtlLoadingQueue::AttachEntityLoaded( CNtlNaviLoadingEntity* pEntity )
{
	m_clEntityLoadedCS.Lock();
	m_defEntityLoadedList.push_back( pEntity );
	m_clEntityLoadedCS.Unlock();
}

bool CNtlLoadingQueue::DetachEntityLoaded( CNtlNaviLoadingEntity* pEntity )
{
	m_clEntityLoadedCS.Lock();

	bool bRet = false;

	for ( vecdef_ENTITY_LIST::iterator it = m_defEntityLoadedList.begin(); it != m_defEntityLoadedList.end(); ++it )
	{
		CNtlNaviLoadingEntity* pRetEntity = *it;

		if ( pEntity == pRetEntity )
		{
			m_defEntityLoadedList.erase( it );

			bRet = true;

			break;
		}
	}

	m_clEntityLoadedCS.Unlock();

	return bRet;
}

unsigned int CNtlLoadingQueue::ThreadCallBackFunc( void )
{
	while ( !IsExit() )
	{
		WaitForSingleObjectEx( m_hEvent, INFINITE, FALSE );

		if ( IsExit() )
		{
			SetEvent( m_hEvent );
			break;
		}

		CNtlNaviLoadingEntity* pEntity = TakeEntityToLoad();

		if ( pEntity )
		{
			SetEvent( m_hEvent );

			pEntity->RunMultiThread();
			AttachEntityLoaded( pEntity );
		}
	}

	return 0;
}

unsigned int __stdcall CNtlLoadingQueue::ThreaFuncCB( void* pParam )
{
	return ((CNtlLoadingQueue*)pParam)->ThreadCallBackFunc();
}