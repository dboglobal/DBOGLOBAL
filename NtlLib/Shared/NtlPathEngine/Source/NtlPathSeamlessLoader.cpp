#include "Precomp_NtlPathEngine.h"
#include "NtlPathSeamlessLoader.h"

#include "NtlPathSeamlessLoadingManager.h"
#include "NtlSeamlessMesh.h"

// Debug
#include "PerformanceChecker.h"
#include "NtlPathEngineLog.h"

CNtlPathSeamlessLoader::CNtlPathSeamlessLoader()
: m_pManager( NULL )
{

}

CNtlPathSeamlessLoader::~CNtlPathSeamlessLoader()
{

}

bool CNtlPathSeamlessLoader::Create(  )
{
	return true;
}

unsigned int __stdcall CNtlPathSeamlessLoader::ThreadFunc( void* pParam )
{
	while( !GetNtlPathEngineLoadingManager()->IsExit() )
	{
		// WaitSingleObject
		WaitForSingleObjectEx( GetNtlPathEngineLoadingManager()->GetEvent(), INFINITE, FALSE );

		// 1. 로드 되어야 할 것들이 있는지 체크
		CNtlSeamlessMesh* pMesh = GetNtlPathEngineLoadingManager()->GetLoadReservation();
		if( !pMesh )
			continue;

		// 2. 로딩 완료 했으면 로딩 완료했다고 넣어줌

		static CPerformanceChecker perf;
		perf.Run();
		pMesh->BuildMeshFromIndexs();
		perf.Stop();
		CNtlPathEngineLog::GetInstance()->Log( "1 Tile Loaded Time %f", perf.GetLastTime() );
		
		GetNtlPathEngineLoadingManager()->Loaded( pMesh );

		// Test
		printf_s("Loading Complete! \n");
	}

	return 0;
}

void CNtlPathSeamlessLoader::Destroy()
{

}