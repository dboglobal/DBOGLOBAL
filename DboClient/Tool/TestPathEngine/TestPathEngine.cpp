#include "stdafx.h"

// NtlPathEngine
#include "NtlPathEngine.h"
#include "NtlPathEngineLog.h"

#include "NtlMeshFederationBuilder.h"
#include "NtlMeshFederationContainer.h"

#include "NtlMesh.h"

// Test
#include "PerformanceChecker.h"
#include "TestTimer.h"
#include <conio.h>

int main( void )
{
	GetNtlPathEngine()->Create( "PathEngine.dll" );
	
	// Update
	CTBTimer::StartTimer();
	
	float m_fTime;
	float m_fElapsedTime;
	float m_fFps;

	int nX = 29000;
	int nY = 30000;
	int nZ = -12712;
	//int nX = 53490;
	//int nY = 53980;
	//int nZ = -70;*/
	
	int dX = 30000;
	int dY = 30000;
	GetNtlPathEngine()->SeamlessAddAgentSize( 100 );
	GetNtlPathEngine()->SeamlessAddAgentSize( 200 );
	GetNtlPathEngine()->SeamlessAddAgentSize( 300 );
	GetNtlPathEngine()->SeamlessAddAgentSize( 500 );
	GetNtlPathEngine()->SeamlessChangeWorld( 12 );
	GetNtlPathEngine()->SeamlessUpdatePosition( nX, nY, nZ );

	while( true )
	{
		CTBTimer::UpdateTimer();

		m_fTime			= CTBTimer::GetAppTime();
		m_fElapsedTime	= CTBTimer::GetElapsedTime();
		m_fFps			= CTBTimer::GetFps();

		GetNtlPathEngine()->Update( m_fElapsedTime );

		// Update ÈÄÀÇ Keyboard ÀÎÇ²
		if( _kbhit() )
		{
			switch(_getch())
			{
			case '8':
				{
					nY += dY;
					GetNtlPathEngine()->SeamlessUpdatePosition( nX, nY, nZ );
				}
				break;
			case '4':
				{
					nX -= dX;
					GetNtlPathEngine()->SeamlessUpdatePosition( nX, nY, nZ );
				}
				break;
			case '2':
				{
					nY -= dY;
					GetNtlPathEngine()->SeamlessUpdatePosition( nX, nY, nZ );
				}
				break;
			case '6':
				{
					nX += dX;
					GetNtlPathEngine()->SeamlessUpdatePosition( nX, nY, nZ );
				}
				break;
			case '5':
				{
					GetNtlPathEngine()->SeamlessPathFinding( 100, nX, nY, nZ,
						nX + 300, nY + 100, nZ);
				}
				break;
			default:
				break;
			}
		}
	}

	GetNtlPathEngine()->Destroy();
//	return 0;
//}
	/*std::vector <int> vecAgent;
	vecAgent.push_back( 100 );
	vecAgent.push_back( 200 );
	vecAgent.push_back( 300 );
	vecAgent.push_back( 500 );

	CNtlMeshFederationBuilder builder;
	CPerformanceChecker perf;
	
	perf.Run();
	builder.BuildFederation_FromWorldMesh( "12/0", 1000000, 500000, true, true, vecAgent );*/
	////builder.BuildFederation_FromWorldMesh( "0/10", 25600, 6400, true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/14", 25600, 6400, true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/15", 25600, 6400, true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/21", 25600, 6400, true, true, vecAgent  );
	////builder.BuildFederation_FromWorldMesh( "0/22", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/23", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/26", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/27", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/28", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/29", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/30", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/31", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/34", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/35", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/36", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/37", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/38", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/39", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/42", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/43", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/44", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/45", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/46", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/47", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/52", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/53", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/54", 25600, 6400 , true, true, vecAgent );
	////builder.BuildFederation_FromWorldMesh( "0/55", 25600, 6400 , true, true, vecAgent );
	//
	//perf.Stop();

	//CNtlPathEngineLog::GetInstance()->Log( "Perfomance Checker : TileSize 25600 Overlap 3200 build time : %f", perf.GetLastTime() );
	
	return 0;
}