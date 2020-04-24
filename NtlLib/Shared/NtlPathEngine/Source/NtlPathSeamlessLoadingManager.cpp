#include "Precomp_NtlPathEngine.h"
#include "NtlPathSeamlessLoadingManager.h"

#include <process.h>
#include <sstream>
#include "NtlSeamlessMesh.h"
#include "NtlPathEngineHelper.h"

// Test
#include "NtlPathEngineLog.h"

CNtlPathSeamlessLoadingManager::CNtlPathSeamlessLoadingManager()
{
	// Thread가 생성되었는지 확인하기 위한 부울
	m_bCreatedThread = false;
	m_hEventForThread = NULL;
	m_bExit = false;
	memset( m_ahThread, 0, sizeof( m_ahThread ) );
	memset( m_dwThreadID, 0, sizeof( m_dwThreadID ) );
}

CNtlPathSeamlessLoadingManager::~CNtlPathSeamlessLoadingManager()
{
	
}

bool CNtlPathSeamlessLoadingManager::Create()
{
	// 이벤트 생성
	m_hEventForThread = CreateEvent( NULL, FALSE, FALSE, NULL );
	if( NULL == m_hEventForThread )
		return false;

	// 쓰레드로더 클래스 생성
	if( !m_ThreadLoader.Create() )
		return false;

	// 쓰레드 생성
	for( int i = 0; i < eMAX_THREAD_CNT; ++i )
	{
		m_ahThread[i] = (HANDLE)_beginthreadex( 
			NULL,	// 핸들의 상속 여부 ( NULL은 상속 대상에서 제외 )
			0,		// 쓰레드 생성 시 생성될 스택사이즈 ( 0 은 Default 1MByte )
			&CNtlPathSeamlessLoader::ThreadFunc,	// 쓰레드로 동작할 함수
			NULL,	// 쓰레드로 전달되는 매개변수의 포인터
			0,		// 쓰레드의 생성 및 실행을 조절하기 위한 Flag
			NULL);	// 쓰레드 ID를 전달받기 위한 변수의 주소값
	}

	m_bCreatedThread = true;
		
	return true;
}

void CNtlPathSeamlessLoadingManager::Update( float fElapsed )
{
	static float fTimeCheck = 0.f;
	fTimeCheck += fElapsed;

	if( fTimeCheck > 5.0f )
	{
		fTimeCheck = 0.0f;

		CNtlPathEngineLog::GetInstance()->Log("Reservation %d", m_listLoadReservation.size() );
	}

	// Reservation이 있는지 확인한다.
	// 있다면 Thread Event를 Signaled 로 변경한다.
	// 없다면 Thread Event를 Reset하여 UnSignaled 로 돌아가게 한다.
	if( !m_listLoadReservation.empty() )
		SetThreadEvent();
	else
		ResetThreadEvent();

}

void CNtlPathSeamlessLoadingManager::Destroy()
{
	if( m_bCreatedThread )
	{
		// 종료를 선언해주고
		SetExit();
		
		// Event를 통과시킨다. ( 그래야지 ThreadLoader가 종료되었는지 검사할 수 있다. )
		SetThreadEvent();

		// 모든 스레드가 멈출 때까지 기다린다.
		WaitForMultipleObjectsEx( eMAX_THREAD_CNT, m_ahThread, TRUE, INFINITE, FALSE );

		for( int i=0; i < eMAX_THREAD_CNT; ++i )
		{
			if( m_ahThread[i] )
			{
				CloseHandle( m_ahThread[i] );
				m_ahThread[i] = NULL;
			}
		}

		if( m_hEventForThread )
		{
			CloseHandle( m_hEventForThread );
			m_hEventForThread = NULL;
		}

		m_bExit = false;
		m_bCreatedThread = false;
	}
}

void CNtlPathSeamlessLoadingManager::LoadReservation( CNtlSeamlessMesh* pMesh )
{
	m_ReservationCS.Lock();

	// Make key name
	std::ostringstream keyname;
	keyname << pMesh->GetWorldIndex();
	keyname << "_";
	keyname << pMesh->GetFieldIndex();
	keyname << "_";
	keyname << pMesh->GetTileIndex();
	
	// 예약 되어 있거나 로딩이 완료 되어 있으면 추가하지 않는다.
	mapdef_seamMesh::iterator reserIt = m_mapLoadReservation.find( keyname.str().c_str() );
	mapdef_seamMesh::iterator loadedIt = m_mapLoaded.find( keyname.str().c_str() );
	if( reserIt == m_mapLoadReservation.end() &&
		loadedIt == m_mapLoaded.end() )
	{
		m_mapLoadReservation.insert( std::make_pair( keyname.str(), pMesh ) );
		m_listLoadReservation.push_back( pMesh );
	}

	m_ReservationCS.Unlock();
}

CNtlSeamlessMesh* CNtlPathSeamlessLoadingManager::GetLoadReservation()
{
	m_ReservationCS.Lock();
	if( m_listLoadReservation.empty() )
	{
		m_ReservationCS.Unlock();
		return NULL;
	}

	

	CNtlSeamlessMesh* pMesh = (CNtlSeamlessMesh*)m_listLoadReservation.front();
	m_listLoadReservation.pop_front();

	std::ostringstream keyname;
	keyname << pMesh->GetWorldIndex();
	keyname << "_";
	keyname << pMesh->GetFieldIndex();
	keyname << "_";
	keyname << pMesh->GetTileIndex();
	mapdef_seamMesh::iterator reserIt = m_mapLoadReservation.find( keyname.str().c_str() );
	if( reserIt != m_mapLoadReservation.end() )
		m_mapLoadReservation.erase( reserIt );

	m_ReservationCS.Unlock();

	return pMesh;
}

void CNtlPathSeamlessLoadingManager::Loaded( CNtlSeamlessMesh* pMesh )
{
	m_LoadedCS.Lock();
	
	// Make key name
	std::ostringstream keyname;
	keyname << pMesh->GetWorldIndex();
	keyname << "_";
	keyname << pMesh->GetFieldIndex();
	keyname << "_";
	keyname << pMesh->GetTileIndex();

	// 이미 로드되어 있다면 삭제하고 로드되어 있지 않다면 추가한다.
	mapdef_seamMesh::iterator loadedIt = m_mapLoaded.find( keyname.str().c_str() );
	if( loadedIt == m_mapLoaded.end() )
	{
		m_mapLoaded.insert( std::make_pair( keyname.str(), pMesh ) );
		m_listLoaded.push_back( pMesh );
	}
	else
	{
		delete pMesh;
		pMesh = NULL;
	}

	m_LoadedCS.Unlock();
}

CNtlSeamlessMesh* CNtlPathSeamlessLoadingManager::GetLoadedMesh()
{
	m_LoadedCS.Lock();

	if( m_listLoaded.empty() )
	{
		m_LoadedCS.Unlock();
		return NULL;
	}

	CNtlSeamlessMesh* pMesh = (CNtlSeamlessMesh*)m_listLoaded.front();
	m_listLoaded.pop_front();

	std::ostringstream keyname;
	keyname << pMesh->GetWorldIndex();
	keyname << "_";
	keyname << pMesh->GetFieldIndex();
	keyname << "_";
	keyname << pMesh->GetTileIndex();
	mapdef_seamMesh::iterator loadedIt = m_mapLoaded.find( keyname.str().c_str() );
	if( loadedIt != m_mapLoaded.end() )
		m_mapLoaded.erase( loadedIt );


	m_LoadedCS.Unlock();

	return pMesh;
}

HANDLE CNtlPathSeamlessLoadingManager::GetEvent()
{
	m_EventCS.Lock();
	HANDLE hEvent = m_hEventForThread;
	m_EventCS.Unlock();

	return hEvent;
}

void CNtlPathSeamlessLoadingManager::SetExit( void )
{
	m_ExitCS.Lock();
	m_bExit = true;
	m_ExitCS.Unlock();
}

bool CNtlPathSeamlessLoadingManager::IsExit( void )
{
	m_ExitCS.Lock();
	bool bExit = m_bExit;
	m_ExitCS.Unlock();

	return bExit;
}

void CNtlPathSeamlessLoadingManager::ResetThreadEvent()
{
	m_EventCS.Lock();
	ResetEvent( m_hEventForThread );
	m_EventCS.Unlock();
}

void CNtlPathSeamlessLoadingManager::SetThreadEvent()
{
	m_EventCS.Lock();
	SetEvent( m_hEventForThread );
	m_EventCS.Unlock();
}

void CNtlPathSeamlessLoadingManager::RemoveReservationByRangeOver( cHorizontalRange& range )
{
	m_ReservationCS.Lock();
	listdef_seamMesh::iterator it = m_listLoadReservation.begin();
	while( it != m_listLoadReservation.end() )
	{
		CNtlSeamlessMesh* pMesh = (*it);

		// 벗어났다.
		if( !NTLPE_RangesOverlap( pMesh->GetRangeAtWorld(), range ) )
		{
			// KeyName으로 Map에서도 제거
			std::ostringstream keyname;
			keyname << pMesh->GetWorldIndex();
			keyname << "_";
			keyname << pMesh->GetFieldIndex();
			keyname << "_";
			keyname << pMesh->GetTileIndex();
			mapdef_seamMesh::iterator reserIt = m_mapLoadReservation.find( keyname.str().c_str() );
			if( reserIt != m_mapLoadReservation.end() )
				m_mapLoadReservation.erase( reserIt );

			// 포인터 삭제
			delete pMesh;
			pMesh = NULL;

			it = m_listLoadReservation.erase( it );
		}
		else
		{
			it++;
		}
	}
	
	m_ReservationCS.Unlock();
}