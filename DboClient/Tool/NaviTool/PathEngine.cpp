#include "stdafx.h"
#include "PathEngine.h"


CPathEngine* CPathEngine::GetInstance( void )
{
	static CPathEngine clPathEngine;
	return &clPathEngine;
}


CPathEngine::CPathEngine( void )
{
}

CPathEngine::~CPathEngine( void )
{
}

bool CPathEngine::Create( const char* pPathDllName )
{
	return GetNaviEngine()->Create( this, pPathDllName, false );
}

void CPathEngine::Delete( void )
{
	GetNaviEngine()->Delete();
}

bool CPathEngine::Update( void )
{
	return GetNaviEngine()->UpdateToolData();
}

NAVI_INST_HANDLE CPathEngine::CreateInstanceHandler( unsigned int uiWorldID )
{
	return GetNaviEngine()->CreateInstanceHandler( uiWorldID );
}

void CPathEngine::DeleteInstanceHandler( NAVI_INST_HANDLE hHandle )
{
	return GetNaviEngine()->DeleteInstanceHandler( hHandle );
}

bool CPathEngine::ImportWorldData( const char* pPath )
{
	return GetNaviEngine()->ImportWorldToolData( pPath );
}

void CPathEngine::GetListImportedWorldIDList( vecdef_WorldIDList& vecOut )
{
	return GetNaviEngine()->GetListImportedWorldIDList( vecOut );
}

bool CPathEngine::ExportPathData( const char* pPath )
{
	return GetNaviEngine()->ExportPathEngineData( pPath );
}

bool CPathEngine::ExportPathDataSelect( const char* pPath, mapdef_ExportList& list )
{
	return GetNaviEngine()->ExportPathEngineDataSelect( pPath, list );
}

bool CPathEngine::LoadPathData( const char* pPath )
{
	vecdef_WorldIDList defWorldIDList;
	unsigned char byLoadFlags = ePATH_DATA_LOAD_FLAG_LOAD_ALL;

	return GetNaviEngine()->LoadPathEngineData( pPath, defWorldIDList, byLoadFlags );
}

int CPathEngine::GetWorldVertexCount( NAVI_INST_HANDLE hHandle )
{
	return GetNaviEngine()->GetWorldVertexCount( hHandle );
}

int CPathEngine::GetWorldVertexCount( unsigned int uiWorldID )
{
	return GetNaviEngine()->GetWorldVertexCount( uiWorldID );
}

bool CPathEngine::GetWorldVertexToBuffer( NAVI_INST_HANDLE hHandle, sNAVI_VERTEX* pBuffer )
{
	return GetNaviEngine()->GetWorldVertexToBuffer( hHandle, pBuffer );
}

bool CPathEngine::GetWorldVertexToBuffer( unsigned int uiWorldID, sNAVI_VERTEX* pBuffer )
{
	return GetNaviEngine()->GetWorldVertexToBuffer( uiWorldID, pBuffer );
}

iMesh* CPathEngine::GetNearstMesh( NAVI_INST_HANDLE hHandle, sNAVI_POS& vPos )
{
	return GetNaviEngine()->GetNearestMesh( hHandle, vPos );
}


unsigned long CPathEngine::GetTotalMemory( void )
{
	return GetNaviEngine()->GetTotalMemory();
}

void CPathEngine::Log( const char* pLog, ... )
{
	FILE* pFile;
	fopen_s( &pFile, "pelog.txt", "a" );
	if ( pFile )
	{
		fprintf_s( pFile, "%s\n", pLog );

		fclose( pFile );
	}
}

