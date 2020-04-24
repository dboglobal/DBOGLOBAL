#include "Precomp_NtlPathEngine.h"
#include "NtlPathEngine.h"

// Log
#include "NtlPathEngineLog.h"

// Callback
#include "NtlMeshFederationTileGeneratedCallBack.h"

// Stream
#include "NtlFileOutputStream.h"

// STL
#include <sstream>

#include "PerformanceChecker.h"

CNtlPathEngine::CNtlPathEngine(void)
{
}

CNtlPathEngine::~CNtlPathEngine( void )
{
}

CNtlPathEngine* CNtlPathEngine::GetInstance()
{
	static CNtlPathEngine instacne;
	return &instacne;
}

bool CNtlPathEngine::Create( const char* pPathEngineDllName, BOOL bSeamless )
{
	m_bSeamless = bSeamless;

	if( NULL == pPathEngineDllName )
		return false;

	DWORD dwErrorValue;
	m_hInstance = LoadLibraryA( pPathEngineDllName );
	if( !m_hInstance )
	{
		dwErrorValue = GetLastError();

		CNtlPathEngineLog::GetInstance()->Log( "failed calling LoadLibrary() for %s", pPathEngineDllName );
		CNtlPathEngineLog::GetInstance()->Log( "GetLastError() : %ud", dwErrorValue );
				
		return false;
	}

	FARPROC procAddress;
	SetLastError(0);
	procAddress = GetProcAddress(m_hInstance, (LPCSTR)1);
	if(!procAddress)
	{
		dwErrorValue = GetLastError();

		CNtlPathEngineLog::GetInstance()->Log( "Failed to obtain PathEngine entrypoint in %s", pPathEngineDllName );
		CNtlPathEngineLog::GetInstance()->Log( "GetLastError() : %ud", dwErrorValue );
		
		return false;
	}

	// pLog가 NULL 이라면 PathEngine 내부에서 Log를 남기지 못한다.
	if( NULL == CNtlPathEngineLog::GetInstance() )
		CNtlPathEngineLog::GetInstance()->Log( "Failed to start ntl pathengine log system in %d", CNtlPathEngineLog::GetInstance() );

	// DLL 에서 PathEngine의 interface를 가지고 온다.
	tGetInterfaceFunction getInterfaceFunction = (tGetInterfaceFunction)procAddress;
	m_pPathEngine = getInterfaceFunction(CNtlPathEngineLog::GetInstance());
	if( NULL == m_pPathEngine )
	{
		CNtlPathEngineLog::GetInstance()->Log( "Failed to getting PathEngine interface in %d", procAddress );

		return false;
	}

	// DLL의 Version Check
	if( m_pPathEngine->getInterfaceMajorVersion() != PATHENGINE_INTERFACE_MAJOR_VERSION ||
		m_pPathEngine->getInterfaceMinorVersion() < PATHENGINE_INTERFACE_MINOR_VERSION )
	{
		CNtlPathEngineLog::GetInstance()->Log( "Pathengine version is incompatible with headers used for compilation." );

		m_pPathEngine = NULL;
		return false;
	}

	// Seamless Create

	if( bSeamless )
	{
		if( !m_seamManager.Create() )
			return false;
	}

	return true;
}

void CNtlPathEngine::Update( float fElapsed )
{
	m_seamManager.Update( fElapsed );
}

void CNtlPathEngine::Destroy()
{
	if( m_bSeamless )
		m_seamManager.Destroy();
}

iPathEngine* CNtlPathEngine::GetPathEngine()
{
	return m_pPathEngine;
}

void CNtlPathEngine::SeamlessAddAgentSize( int nSize )
{
	m_seamManager.AddAgentSize( nSize );
}

void CNtlPathEngine::SeamlessChangeWorld( int nWorldIndex )
{
	m_seamManager.ChangeWorld( nWorldIndex );
}

void CNtlPathEngine::SeamlessUpdatePosition( int nPositionX, int nPositionY, int nPositionZ )
{
	if( m_seamManager.UpdatePosition( nPositionX, nPositionY, nPositionZ ) )
	{
		m_nPositionX = nPositionX;
		m_nPositionY = nPositionY;
		m_nPositionZ = nPositionZ;
	}
}

void CNtlPathEngine::SeamlessPathFinding( int nAgentSize, 
										 int nSrcPositionX, int nSrcPositionY, int nSrcPositionZ,
										 int nDestPositionX, int nDestPositionY, int nDestPositionZ,
										 std::list< cPosition >& vec)
{
	static CPerformanceChecker perf;

	perf.Run();
	iPath* path = m_seamManager.FindShortestPath( nAgentSize, nSrcPositionX, nSrcPositionY, nSrcPositionZ, nDestPositionX, nDestPositionY, nDestPositionZ,
		vec );
	perf.Stop();

	if( path == NULL )
	{
		CNtlPathEngineLog::GetInstance()->Log("Can not find path");
	}
	else
	{
		if( path )
		{
			delete path;
			path = NULL;
		}
	}

	return;
}
