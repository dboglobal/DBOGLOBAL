#include "precomp_navi.h"
#include "NtlNaviPEImporter.h"
#include "NtlNaviLog.h"
#include "NtlNaviUtility.h"
#include "NtlNaviPathEngine.h"


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_ODProp
//
//////////////////////////////////////////////////////////////////////////


void CNtlNaviPE_ODProp::RunMultiThread( void )
{
	m_csPropInfo.Lock();

	m_pPropInfo = m_pNaviDataMng->Load_PropOutDoor( m_uiFieldID, m_strPath.c_str() );

	if ( NULL == m_pPropInfo )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world outdoor property data. [%d, %s]", m_uiFieldID, m_strPath.c_str() );

		SetError( true );
	}

	m_csPropInfo.Unlock();
}


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_IDProp
//
//////////////////////////////////////////////////////////////////////////


void CNtlNaviPE_IDProp::RunMultiThread( void )
{
	m_csPropInfo.Lock();

	m_pPropInfo = m_pNaviDataMng->Load_PropInDoor( m_uiBlockID, m_strPath.c_str() );

	if ( NULL == m_pPropInfo )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world indoor property data. [%d, %s]", m_uiBlockID, m_strPath.c_str() );

		SetError( true );
	}

	m_csPropInfo.Unlock();
}


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_ODGroup
//
//////////////////////////////////////////////////////////////////////////


void CNtlNaviPE_ODGroup::AttachAgent( float fRadius, iShape* pShape )
{
	sAGENT_DATA sAgent;
	sAgent.fRadius = fRadius;
	sAgent.pShape = pShape;

	m_defAgentList.push_back( sAgent );
}

void CNtlNaviPE_ODGroup::RunMultiThread( void )
{
	CNtlNaviAutoCS clAuto( &m_csODGroup );

	iMesh* pGroundMesh = NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Import ground mesh
	//
	//////////////////////////////////////////////////////////////////////////

	{
		std::string strMeshFileName = m_strImportPath;
		AttachBackSlash( strMeshFileName );

		char szTempBuffer[128];
		sprintf_s( szTempBuffer, 128, "%d.%s", m_uiGroupID, PE_FIELD_GROUP_MESH_EXT );
		strMeshFileName += szTempBuffer;

		sFILE_CHUNK_INFO sMeshChunk = LoadFileChunk( strMeshFileName.c_str() );

		if ( sMeshChunk.lChunkSize == 0xffffffff ||
			 sMeshChunk.lChunkSize == 0 )
		{
			return;
		}

		pGroundMesh = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->loadMeshFromBuffer( "tok", sMeshChunk.pChunk, sMeshChunk.lChunkSize, NULL );

		UnloadFileChunk( sMeshChunk );

		if ( NULL == pGroundMesh )
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating ground mesh failed. [%s]", strMeshFileName.c_str() );

			SetError( true );

			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Import agent
	//
	//////////////////////////////////////////////////////////////////////////

	{
		char szTempBuff[256];

		std::vector< sAGENT_DATA >::iterator it = m_defAgentList.begin();
		for ( ; it != m_defAgentList.end(); ++it )
		{
			//////////////////////////////////////////////////////////////////////////
			// Collision preprocess import
			//////////////////////////////////////////////////////////////////////////

			std::string strColFile = m_strImportPath;
			AttachBackSlash( strColFile );

			sprintf_s( szTempBuff, 256, "%d_%d.%s", m_uiGroupID, (tSigned32)WORLD_COORD_TO_PATH_COORD(it->fRadius), PE_FIELD_GROUP_COL_PRE_EXT );
			strColFile += szTempBuff;

			sFILE_CHUNK_INFO sColChunk = LoadFileChunk( strColFile.c_str() );

			if ( sColChunk.lChunkSize == 0xffffffff ||
				 sColChunk.lChunkSize == 0 )
			{
				CNtlNaviLog::GetInstance()->Log( "[IMPORT] Importing collision process failed. [%s]", strColFile.c_str() );

				SetError( true );

				return;
			}

			pGroundMesh->loadCollisionPreprocessFor( it->pShape, sColChunk.pChunk, sColChunk.lChunkSize );

			UnloadFileChunk( sColChunk );

			//////////////////////////////////////////////////////////////////////////
			// Path find preprocess import
			//////////////////////////////////////////////////////////////////////////

			std::string strPFFile = m_strImportPath;
			AttachBackSlash( strPFFile );

			sprintf_s( szTempBuff, 256, "%d_%d.%s", m_uiGroupID, (tSigned32)WORLD_COORD_TO_PATH_COORD(it->fRadius), PE_FIELD_GROUP_PATH_FIND_PRE_EXT );
			strPFFile += szTempBuff;

			sFILE_CHUNK_INFO sPFChunk = LoadFileChunk( strPFFile.c_str() );

			if ( sPFChunk.lChunkSize == 0xffffffff ||
				 sPFChunk.lChunkSize == 0 )
			{
				CNtlNaviLog::GetInstance()->Log( "[IMPORT] Importing path preprocess failed. [%s]", strPFFile.c_str() );

				SetError( true );

				return;
			}

			pGroundMesh->loadPathfindPreprocessFor( it->pShape, sPFChunk.pChunk, sPFChunk.lChunkSize );

			UnloadFileChunk( sPFChunk );
		}
	}

	m_pMesh = pGroundMesh;
}


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPE_IDGroup
//
//////////////////////////////////////////////////////////////////////////


void CNtlNaviPE_IDGroup::AttachAgent( float fRadius, iShape* pShape )
{
	sAGENT_DATA sAgent;
	sAgent.fRadius = fRadius;
	sAgent.pShape = pShape;

	m_defAgentList.push_back( sAgent );
}

void CNtlNaviPE_IDGroup::RunMultiThread( void )
{
	CNtlNaviAutoCS clAuto( &m_csIDGroup );

	iMesh* pGroundMesh = NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Import ground mesh
	//
	//////////////////////////////////////////////////////////////////////////

	{
		std::string strMeshFileName = m_strImportPath;
		AttachBackSlash( strMeshFileName );

		char szTempBuffer[128];
		sprintf_s( szTempBuffer, 128, "%d.%s", 0, PE_FIELD_GROUP_MESH_EXT );
		strMeshFileName += szTempBuffer;

		sFILE_CHUNK_INFO sMeshChunk = LoadFileChunk( strMeshFileName.c_str() );

		if ( sMeshChunk.lChunkSize == 0xffffffff ||
			 sMeshChunk.lChunkSize == 0 )
		{
			return;
		}

		pGroundMesh = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->loadMeshFromBuffer( "tok", sMeshChunk.pChunk, sMeshChunk.lChunkSize, NULL );

		UnloadFileChunk( sMeshChunk );

		if ( NULL == pGroundMesh )
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating ground mesh failed. [%s]", strMeshFileName.c_str() );

			SetError( true );

			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Import agent
	//
	//////////////////////////////////////////////////////////////////////////

	{
		char szTempBuff[256];

		std::vector< sAGENT_DATA >::iterator it = m_defAgentList.begin();
		for ( ; it != m_defAgentList.end(); ++it )
		{
			//////////////////////////////////////////////////////////////////////////
			// Collision preprocess import
			//////////////////////////////////////////////////////////////////////////

			std::string strColFile = m_strImportPath;
			AttachBackSlash( strColFile );

			sprintf_s( szTempBuff, 256, "%d_%d.%s", 0, (tSigned32)WORLD_COORD_TO_PATH_COORD(it->fRadius), PE_FIELD_GROUP_COL_PRE_EXT );
			strColFile += szTempBuff;

			sFILE_CHUNK_INFO sColChunk = LoadFileChunk( strColFile.c_str() );

			if ( sColChunk.lChunkSize == 0xffffffff ||
				 sColChunk.lChunkSize == 0 )
			{
				CNtlNaviLog::GetInstance()->Log( "[IMPORT] Importing collision preprocess failed. [%s]", strColFile.c_str() );

				SetError( true );

				return;
			}

			pGroundMesh->loadCollisionPreprocessFor( it->pShape, sColChunk.pChunk, sColChunk.lChunkSize );

			UnloadFileChunk( sColChunk );

			//////////////////////////////////////////////////////////////////////////
			// Path find preprocess import
			//////////////////////////////////////////////////////////////////////////

			std::string strPFFile = m_strImportPath;
			AttachBackSlash( strPFFile );

			sprintf_s( szTempBuff, 256, "%d_%d.%s", 0, (tSigned32)WORLD_COORD_TO_PATH_COORD(it->fRadius), PE_FIELD_GROUP_PATH_FIND_PRE_EXT );
			strPFFile += szTempBuff;

			sFILE_CHUNK_INFO sPFChunk = LoadFileChunk( strPFFile.c_str() );

			if ( sPFChunk.lChunkSize == 0xffffffff ||
				 sPFChunk.lChunkSize == 0 )
			{
				CNtlNaviLog::GetInstance()->Log( "[IMPORT] Importing path preprocess failed. [%s]", strPFFile.c_str() );

				SetError( true );

				return;
			}

			pGroundMesh->loadPathfindPreprocessFor( it->pShape, sPFChunk.pChunk, sPFChunk.lChunkSize );

			UnloadFileChunk( sPFChunk );
		}
	}

	m_pMesh = pGroundMesh;
}
