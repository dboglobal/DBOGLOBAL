#include "stdafx.h"
#include "NtlNaviData.h"
#include <float.h>


void Util_Add_Inv_Slash( std::string& strString )
{
	if ( strString.empty() )
	{
		strString += "\\";
	}
	else
	{
		if ( strString[strString.size()-1] != '\\' )
		{
			strString += "\\";
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi info
//
//////////////////////////////////////////////////////////////////////////


void CNtlNaviInfo::SetParent( CNtlNaviInfo* pParent )
{
	m_pParent = pParent;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi world info
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviWorldInfo::CNtlNaviWorldInfo( eNAVI_WORLD_TYPE eWorldType )
: m_eWorldType( eWorldType )
{
	m_fWorldMinPosX = m_fWorldMinPosZ = FLT_MAX;
	m_fWorldMaxPosX = m_fWorldMaxPosZ = FLT_MAX;
}

CNtlNaviWorldInfo::~CNtlNaviWorldInfo( void )
{
}

void CNtlNaviWorldInfo::SetWorldType( eNAVI_WORLD_TYPE eWorldType )
{
	m_eWorldType = eWorldType;
}

void CNtlNaviWorldInfo::SetWorldID( unsigned int uiWorldID )
{
	m_uiWorldID = uiWorldID;
}

void CNtlNaviWorldInfo::SetWorldName( const char* pWorldName )
{
	m_strWorldName = pWorldName;
}

void CNtlNaviWorldInfo::SetWorldMinPos( float fWorldMinPosX, float fWorldMinPosZ )
{
	m_fWorldMinPosX = fWorldMinPosX;
	m_fWorldMinPosZ = fWorldMinPosZ;
}

void CNtlNaviWorldInfo::SetWorldMaxPos( float fWorldMaxPosX, float fWorldMaxPosZ )
{
	m_fWorldMaxPosX = fWorldMaxPosX;
	m_fWorldMaxPosZ = fWorldMaxPosZ;
}

int CNtlNaviWorldInfo::GetAgencyCnt( void )
{
	return (int)m_defAgencyList.size();
}

float CNtlNaviWorldInfo::GetAgency( int nIdx )
{
	return m_defAgencyList[nIdx];
}

void CNtlNaviWorldInfo::AttachAgency( float fAgencyRadius )
{
	m_defAgencyList.push_back( fAgencyRadius );
}

bool CNtlNaviWorldInfo::Import( const char* pPathName )
{
	return true;
}

bool CNtlNaviWorldInfo::Export( const char* pPathName )
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi world out-door info
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviWorldOutDoorInfo::CNtlNaviWorldOutDoorInfo( void )
: CNtlNaviWorldInfo( eNAVI_WORLD_TYPE_OUTDOOR )
{
	m_fTileSize = FLT_MAX;
	m_fFieldSize = FLT_MAX;
	m_uiCrossFieldCntOfGroup = 0;
	m_fOverlapSize = 0.f;
}

CNtlNaviWorldOutDoorInfo::~CNtlNaviWorldOutDoorInfo( void )
{
}

void CNtlNaviWorldOutDoorInfo::SetTileSize( float fTileSize )
{
	m_fTileSize = fTileSize;
}

void CNtlNaviWorldOutDoorInfo::SetFieldSize( float fFieldSize )
{
	m_fFieldSize = fFieldSize;
}

void CNtlNaviWorldOutDoorInfo::SetCrossFieldCntOfGroup( unsigned int uiCrossFieldCntOfGroup )
{
	m_uiCrossFieldCntOfGroup = uiCrossFieldCntOfGroup;
}

void CNtlNaviWorldOutDoorInfo::SetOverlapSize( float fOverlapSize )
{
	m_fOverlapSize = fOverlapSize;
}

bool CNtlNaviWorldOutDoorInfo::Import( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	strPathName += NAVI_WORLD_NAME;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "rb" );

	if ( pFile )
	{
		char szIndentity[64]; memset( szIndentity, 0, sizeof(szIndentity) );
		fread( szIndentity, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );
		if ( stricmp( szIndentity, IDENTITY_NAVI_WORLD ) != 0 )
		{
			fclose( pFile );
			return false;
		}

		unsigned int uiVersion;
		fread( &uiVersion, sizeof(uiVersion), 1, pFile );
		if ( uiVersion != VERSION_NAVI_WORLD )
		{
			fclose( pFile );
			return false;
		}

		unsigned char byType;
		fread( &byType, sizeof(byType), 1, pFile );
		m_eWorldType = (eNAVI_WORLD_TYPE)byType;
		if ( m_eWorldType != eNAVI_WORLD_TYPE_OUTDOOR )
		{
			fclose( pFile );
			return false;
		}

		fread( &m_uiWorldID, sizeof(m_uiWorldID), 1, pFile );

		unsigned int uiNameLen;
		fread( &uiNameLen, sizeof(uiNameLen), 1, pFile );

		char* pszName = new char[uiNameLen+1];
		fread( pszName, sizeof(char) * uiNameLen, 1, pFile );
		pszName[uiNameLen] = '\0';
		m_strWorldName = pszName;
		delete pszName;

		fread( &m_fWorldMinPosX, sizeof(m_fWorldMinPosX), 1, pFile );
		fread( &m_fWorldMinPosZ, sizeof(m_fWorldMinPosZ), 1, pFile );

		fread( &m_fWorldMaxPosX, sizeof(m_fWorldMaxPosX), 1, pFile );
		fread( &m_fWorldMaxPosZ, sizeof(m_fWorldMaxPosZ), 1, pFile );

		unsigned int uiAgencyListSize;
		float fAgencyRadius;

		fread( &uiAgencyListSize, sizeof(uiAgencyListSize), 1, pFile );

		for ( unsigned int i = 0; i < uiAgencyListSize; ++i )
		{
			fread( &fAgencyRadius, sizeof(fAgencyRadius), 1, pFile );
			m_defAgencyList.push_back( fAgencyRadius );
		}

		fread( &m_fTileSize, sizeof(m_fTileSize), 1, pFile );

		fread( &m_fFieldSize, sizeof(m_fFieldSize), 1, pFile );

		fread( &m_uiCrossFieldCntOfGroup, sizeof(m_uiCrossFieldCntOfGroup), 1, pFile );

		fread( &m_fOverlapSize, sizeof(m_fOverlapSize), 1, pFile );

		fclose( pFile );
	}

	return true;
}

bool CNtlNaviWorldOutDoorInfo::Export( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	strPathName += NAVI_WORLD_NAME;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "wb" );

	if ( pFile )
	{
		fwrite( IDENTITY_NAVI_WORLD, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );

		unsigned int uiVersion = VERSION_NAVI_WORLD;
		fwrite( &uiVersion, sizeof(uiVersion), 1, pFile );

		unsigned char byType = (unsigned char)m_eWorldType;
		fwrite( &byType, sizeof(byType), 1, pFile );

		fwrite( &m_uiWorldID, sizeof(m_uiWorldID), 1, pFile );

		unsigned int uiNameLen = (unsigned int)m_strWorldName.size();
		fwrite( &uiNameLen, sizeof(uiNameLen), 1, pFile );

		fwrite( m_strWorldName.c_str(), uiNameLen, 1, pFile );

		fwrite( &m_fWorldMinPosX, sizeof(m_fWorldMinPosX), 1, pFile );
		fwrite( &m_fWorldMinPosZ, sizeof(m_fWorldMinPosZ), 1, pFile );

		fwrite( &m_fWorldMaxPosX, sizeof(m_fWorldMaxPosX), 1, pFile );
		fwrite( &m_fWorldMaxPosZ, sizeof(m_fWorldMaxPosZ), 1, pFile );

		unsigned int uiAgencyListSize = (unsigned int)m_defAgencyList.size();
		fwrite( &uiAgencyListSize, sizeof(uiAgencyListSize), 1, pFile );

		for ( unsigned int i = 0; i < uiAgencyListSize; ++i )
		{
			fwrite( &m_defAgencyList[i], sizeof(float), 1, pFile );
		}

		fwrite( &m_fTileSize, sizeof(m_fTileSize), 1, pFile );

		fwrite( &m_fFieldSize, sizeof(m_fFieldSize), 1, pFile );

		fwrite( &m_uiCrossFieldCntOfGroup, sizeof(m_uiCrossFieldCntOfGroup), 1, pFile );

		fwrite( &m_fOverlapSize, sizeof(m_fOverlapSize), 1, pFile );

		fclose( pFile );
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi world in-door info
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviWorldInDoorInfo::CNtlNaviWorldInDoorInfo( void )
: CNtlNaviWorldInfo( eNAVI_WORLD_TYPE_INDOOR )
{
	m_fBlockSize = 0;
}

CNtlNaviWorldInDoorInfo::~CNtlNaviWorldInDoorInfo( void )
{
}

void CNtlNaviWorldInDoorInfo::SetBlockSize( float fBlockSize )
{
	m_fBlockSize = fBlockSize;
}

bool CNtlNaviWorldInDoorInfo::Import( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	strPathName += NAVI_WORLD_NAME;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "rb" );

	if ( pFile )
	{
		char szIndentity[64]; memset( szIndentity, 0, sizeof(szIndentity) );
		fread( szIndentity, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );
		if ( stricmp( szIndentity, IDENTITY_NAVI_WORLD ) != 0 )
		{
			fclose( pFile );
			return false;
		}

		unsigned int uiVersion;
		fread( &uiVersion, sizeof(uiVersion), 1, pFile );
		if ( uiVersion != VERSION_NAVI_WORLD )
		{
			fclose( pFile );
			return false;
		}

		unsigned char byType;
		fread( &byType, sizeof(byType), 1, pFile );
		m_eWorldType = (eNAVI_WORLD_TYPE)byType;
		if ( m_eWorldType != eNAVI_WORLD_TYPE_INDOOR )
		{
			fclose( pFile );
			return false;
		}

		fread( &m_uiWorldID, sizeof(m_uiWorldID), 1, pFile );

		unsigned int uiNameLen;
		fread( &uiNameLen, sizeof(uiNameLen), 1, pFile );

		char* pszName = new char[uiNameLen+1];
		fread( pszName, sizeof(char) * uiNameLen, 1, pFile );
		pszName[uiNameLen] = '\0';
		m_strWorldName = pszName;
		delete pszName;

		fread( &m_fWorldMinPosX, sizeof(m_fWorldMinPosX), 1, pFile );
		fread( &m_fWorldMinPosZ, sizeof(m_fWorldMinPosZ), 1, pFile );

		fread( &m_fWorldMaxPosX, sizeof(m_fWorldMaxPosX), 1, pFile );
		fread( &m_fWorldMaxPosZ, sizeof(m_fWorldMaxPosZ), 1, pFile );

		unsigned int uiAgencyListSize;
		float fAgencyRadius;

		fread( &uiAgencyListSize, sizeof(uiAgencyListSize), 1, pFile );

		for ( unsigned int i = 0; i < uiAgencyListSize; ++i )
		{
			fread( &fAgencyRadius, sizeof(fAgencyRadius), 1, pFile );
			m_defAgencyList.push_back( fAgencyRadius );
		}

		fread( &m_fBlockSize, sizeof(m_fBlockSize), 1, pFile );

		fclose( pFile );
	}

	return true;
}

bool CNtlNaviWorldInDoorInfo::Export( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	strPathName += NAVI_WORLD_NAME;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "wb" );

	if ( pFile )
	{
		fwrite( IDENTITY_NAVI_WORLD, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );

		unsigned int uiVersion = VERSION_NAVI_WORLD;
		fwrite( &uiVersion, sizeof(uiVersion), 1, pFile );

		unsigned char byType = (unsigned char)m_eWorldType;
		fwrite( &byType, sizeof(byType), 1, pFile );

		fwrite( &m_uiWorldID, sizeof(m_uiWorldID), 1, pFile );

		unsigned int uiNameLen = (unsigned int)m_strWorldName.size();
		fwrite( &uiNameLen, sizeof(uiNameLen), 1, pFile );

		fwrite( m_strWorldName.c_str(), uiNameLen, 1, pFile );

		fwrite( &m_fWorldMinPosX, sizeof(m_fWorldMinPosX), 1, pFile );
		fwrite( &m_fWorldMinPosZ, sizeof(m_fWorldMinPosZ), 1, pFile );

		fwrite( &m_fWorldMaxPosX, sizeof(m_fWorldMaxPosX), 1, pFile );
		fwrite( &m_fWorldMaxPosZ, sizeof(m_fWorldMaxPosZ), 1, pFile );

		unsigned int uiAgencyListSize = (unsigned int)m_defAgencyList.size();
		fwrite( &uiAgencyListSize, sizeof(uiAgencyListSize), 1, pFile );

		for ( unsigned int i = 0; i < uiAgencyListSize; ++i )
		{
			fwrite( &m_defAgencyList[i], sizeof(float), 1, pFile );
		}

		fwrite( &m_fBlockSize, sizeof(m_fBlockSize), 1, pFile );

		fclose( pFile );
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi group out-door info
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviGroupOutDoorInfo::CNtlNaviGroupOutDoorInfo( void )
{
	m_uiGroupID = 0xffffffff;
	m_fInvalidHeightVal = FLT_MAX;
	m_uiTileCntOfSector = 0;
	m_uiFieldCntOfGroup = 0;
}

CNtlNaviGroupOutDoorInfo::~CNtlNaviGroupOutDoorInfo( void )
{
	hashdef_FIELD_HEIGHT_LIST::iterator itFHList = m_defFieldHeightList.begin();
	for ( ; itFHList != m_defFieldHeightList.end(); ++itFHList )
	{
		sFIELD_HEIGHT_LIST& sFieldHeightList = itFHList->second;

		for ( int i = 0; i < eSECTOR_MAX; ++i )
		{
			if ( sFieldHeightList.parSectorHeightList[i] )
			{
				delete [] sFieldHeightList.parSectorHeightList[i];
				sFieldHeightList.parSectorHeightList[i] = NULL;
			}
		}
	}
	m_defFieldHeightList.clear();

	vecdef_NAVI_OBJECT_LSIT::iterator itNaviMesh = m_defNaviMeshList.begin();
	for ( ; itNaviMesh != m_defNaviMeshList.end(); ++itNaviMesh )
	{
		delete *itNaviMesh;
	}
	m_defNaviMeshList.clear();

	vecdef_NAVI_OBJECT_LSIT::iterator itObsMesh = m_defObsMeshList.begin();
	for ( ; itObsMesh != m_defObsMeshList.end(); ++itObsMesh )
	{
		delete *itObsMesh;
	}
	m_defObsMeshList.clear();
}

void CNtlNaviGroupOutDoorInfo::SetGroupID( unsigned int uiGroupID )
{
	m_uiGroupID = uiGroupID;
}

void CNtlNaviGroupOutDoorInfo::SetInvalidHeightVal( float fInvalidHeightVal )
{
	m_fInvalidHeightVal = fInvalidHeightVal;
}

void CNtlNaviGroupOutDoorInfo::SetTileCntOfSector( unsigned int uiTileCntOfSector )
{
	m_uiTileCntOfSector = uiTileCntOfSector;
}

void CNtlNaviGroupOutDoorInfo::SetFieldCntOfGroup( unsigned int uiFieldCntOfGroup )
{
	m_uiFieldCntOfGroup = uiFieldCntOfGroup;
}

void CNtlNaviGroupOutDoorInfo::AttachSectorHeightList( unsigned int uiFieldID, int nIdx, float* pSectorHeightList )
{
	hashdef_FIELD_HEIGHT_LIST::iterator it = m_defFieldHeightList.find( uiFieldID );
	if ( it == m_defFieldHeightList.end() )
	{
		sFIELD_HEIGHT_LIST sFieldHeightList;

		sFieldHeightList.parSectorHeightList[nIdx] = pSectorHeightList;

		m_defFieldHeightList[uiFieldID] = sFieldHeightList;
	}
	else
	{
		sFIELD_HEIGHT_LIST& sFieldHeightList = it->second;

		sFieldHeightList.parSectorHeightList[nIdx] = pSectorHeightList;
	}
}

int CNtlNaviGroupOutDoorInfo::GetNaviMeshCnt( void )
{
	return (int)m_defNaviMeshList.size();
}

CNtlNaviGroupOutDoorInfo::sNAVI_OBJECT_LIST* CNtlNaviGroupOutDoorInfo::GetNaviMesh( int nIdx )
{
	return m_defNaviMeshList[nIdx];
}

void CNtlNaviGroupOutDoorInfo::AttachNaviMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal )
{
	sNAVI_OBJECT_LIST* pObjInfo = new sNAVI_OBJECT_LIST;

	pObjInfo->strModelName = pModelName;
	pObjInfo->fPosX = fPosX;
	pObjInfo->fPosY = fPosY;
	pObjInfo->fPosZ = fPosZ;
	pObjInfo->fScaleX = fScaleX;
	pObjInfo->fScaleY = fScaleY;
	pObjInfo->fScaleZ = fScaleZ;
	pObjInfo->fYRotVal = fYRotVal;

	m_defNaviMeshList.push_back( pObjInfo );
}

int CNtlNaviGroupOutDoorInfo::GetObsMeshCnt( void )
{
	return (int)m_defObsMeshList.size();
}

CNtlNaviGroupOutDoorInfo::sNAVI_OBJECT_LIST* CNtlNaviGroupOutDoorInfo::GetObsMesh( int nIdx )
{
	return m_defObsMeshList[nIdx];
}

void CNtlNaviGroupOutDoorInfo::AttachObsMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal )
{
	sNAVI_OBJECT_LIST* pObjInfo = new sNAVI_OBJECT_LIST;

	pObjInfo->strModelName = pModelName;
	pObjInfo->fPosX = fPosX;
	pObjInfo->fPosY = fPosY;
	pObjInfo->fPosZ = fPosZ;
	pObjInfo->fScaleX = fScaleX;
	pObjInfo->fScaleY = fScaleY;
	pObjInfo->fScaleZ = fScaleZ;
	pObjInfo->fYRotVal = fYRotVal;

	m_defObsMeshList.push_back( pObjInfo );
}

bool CNtlNaviGroupOutDoorInfo::Import( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_GROUP_OUTDOOR_NAME( buf, m_uiGroupID );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "rb" );

	if ( pFile )
	{
		char szIndentity[64]; memset( szIndentity, 0, sizeof(szIndentity) );
		fread( szIndentity, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );
		if ( stricmp( szIndentity, IDENTITY_NAVI_GROUP ) != 0 )
		{
			fclose( pFile );
			return false;
		}

		unsigned int uiVersion;
		fread( &uiVersion, sizeof(uiVersion), 1, pFile );
		if ( uiVersion != VERSION_NAVI_GROUP )
		{
			fclose( pFile );
			return false;
		}

		fread( &m_uiGroupID, sizeof(m_uiGroupID), 1, pFile );

		fread( &m_fInvalidHeightVal, sizeof(m_fInvalidHeightVal), 1, pFile );

		fread( &m_uiTileCntOfSector, sizeof(m_uiTileCntOfSector), 1, pFile );

		fread( &m_uiFieldCntOfGroup, sizeof(m_uiFieldCntOfGroup), 1, pFile );

		unsigned int i;

		for ( i = 0; i < m_uiFieldCntOfGroup; ++i )
		{
			unsigned int uiFieldID;
			fread( &uiFieldID, sizeof(uiFieldID), 1, pFile );

			sFIELD_HEIGHT_LIST sFieldHeightInfo;

			for ( int ii = 0; ii < eSECTOR_MAX; ++ii )
			{
				sFieldHeightInfo.parSectorHeightList[ii] = new float[m_uiTileCntOfSector];
				fread( sFieldHeightInfo.parSectorHeightList[ii], sizeof( float ) * m_uiTileCntOfSector, 1, pFile );
			}

			m_defFieldHeightList[uiFieldID] = sFieldHeightInfo;
		}

		unsigned int uiNaviCnt;
		fread( &uiNaviCnt, sizeof(uiNaviCnt), 1, pFile );

		for ( i = 0; i < uiNaviCnt; ++i )
		{
			unsigned int uiNameLen;
			fread( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			char* pszName = new char[uiNameLen+1];
			fread( pszName, sizeof(char) * uiNameLen, 1, pFile );
			pszName[uiNameLen] = '\0';

			float fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal;

			fread( &fPosX, sizeof(fPosX), 1, pFile );
			fread( &fPosY, sizeof(fPosY), 1, pFile );
			fread( &fPosZ, sizeof(fPosZ), 1, pFile );
			fread( &fScaleX, sizeof(fScaleX), 1, pFile );
			fread( &fScaleY, sizeof(fScaleY), 1, pFile );
			fread( &fScaleZ, sizeof(fScaleZ), 1, pFile );
			fread( &fYRotVal, sizeof(fYRotVal), 1, pFile );

			AttachNaviMesh( pszName, fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal );

			delete pszName;
		}

		unsigned int uiObsCnt;
		fread( &uiObsCnt, sizeof(uiObsCnt), 1, pFile );

		for ( i = 0; i < uiObsCnt; ++i )
		{
			unsigned int uiNameLen;
			fread( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			char* pszName = new char[uiNameLen+1];
			fread( pszName, sizeof(char) * uiNameLen, 1, pFile );
			pszName[uiNameLen] = '\0';

			float fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal;

			fread( &fPosX, sizeof(fPosX), 1, pFile );
			fread( &fPosY, sizeof(fPosY), 1, pFile );
			fread( &fPosZ, sizeof(fPosZ), 1, pFile );
			fread( &fScaleX, sizeof(fScaleX), 1, pFile );
			fread( &fScaleY, sizeof(fScaleY), 1, pFile );
			fread( &fScaleZ, sizeof(fScaleZ), 1, pFile );
			fread( &fYRotVal, sizeof(fYRotVal), 1, pFile );

			AttachObsMesh( pszName, fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal );

			delete pszName;
		}

		fclose( pFile );
	}

	return true;
}

bool CNtlNaviGroupOutDoorInfo::Export( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_GROUP_OUTDOOR_NAME( buf, m_uiGroupID );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "wb" );

	if ( pFile )
	{
		fwrite( IDENTITY_NAVI_GROUP, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );

		unsigned int uiVersion = VERSION_NAVI_GROUP;
		fwrite( &uiVersion, sizeof(uiVersion), 1, pFile );

		fwrite( &m_uiGroupID, sizeof(m_uiGroupID), 1, pFile );

		fwrite( &m_fInvalidHeightVal, sizeof(m_fInvalidHeightVal), 1, pFile );

		fwrite( &m_uiTileCntOfSector, sizeof(m_uiTileCntOfSector), 1, pFile );

		fwrite( &m_uiFieldCntOfGroup, sizeof(m_uiFieldCntOfGroup), 1, pFile );

		_ASSERT( m_uiFieldCntOfGroup == m_defFieldHeightList.size() );

		hashdef_FIELD_HEIGHT_LIST::iterator itField = m_defFieldHeightList.begin();
		for ( ; itField != m_defFieldHeightList.end(); ++itField )
		{
			unsigned int uiFieldID = itField->first;
			fwrite( &uiFieldID, sizeof(uiFieldID), 1, pFile );

			sFIELD_HEIGHT_LIST& sField = itField->second;

			for ( int i = 0; i < eSECTOR_MAX; ++i )
			{
				fwrite( sField.parSectorHeightList[i], sizeof( float ) * m_uiTileCntOfSector, 1, pFile );
			}
		}

		unsigned int uiNaviCnt = (unsigned int)m_defNaviMeshList.size();
		fwrite( &uiNaviCnt, sizeof(uiNaviCnt), 1, pFile );

		vecdef_NAVI_OBJECT_LSIT::iterator itNavi = m_defNaviMeshList.begin();
		for ( ; itNavi != m_defNaviMeshList.end(); ++itNavi )
		{
			sNAVI_OBJECT_LIST* pNaviObj = *itNavi;

			unsigned int uiNameLen = (unsigned int)pNaviObj->strModelName.size();
			fwrite( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			fwrite( pNaviObj->strModelName.c_str(), uiNameLen, 1, pFile );

			fwrite( &pNaviObj->fPosX, sizeof(pNaviObj->fPosX), 1, pFile );
			fwrite( &pNaviObj->fPosY, sizeof(pNaviObj->fPosY), 1, pFile );
			fwrite( &pNaviObj->fPosZ, sizeof(pNaviObj->fPosZ), 1, pFile );
			fwrite( &pNaviObj->fScaleX, sizeof(pNaviObj->fScaleX), 1, pFile );
			fwrite( &pNaviObj->fScaleY, sizeof(pNaviObj->fScaleY), 1, pFile );
			fwrite( &pNaviObj->fScaleZ, sizeof(pNaviObj->fScaleZ), 1, pFile );

			fwrite( &pNaviObj->fYRotVal, sizeof(pNaviObj->fYRotVal), 1, pFile );
		}

		unsigned int uiObsCnt = (unsigned int)m_defObsMeshList.size();
		fwrite( &uiObsCnt, sizeof(uiObsCnt), 1, pFile );

		vecdef_NAVI_OBJECT_LSIT::iterator itObs = m_defObsMeshList.begin();
		for ( ; itObs != m_defObsMeshList.end(); ++itObs )
		{
			sNAVI_OBJECT_LIST* pObsObj = *itObs;

			unsigned int uiNameLen = (unsigned int)pObsObj->strModelName.size();
			fwrite( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			fwrite( pObsObj->strModelName.c_str(), uiNameLen, 1, pFile );

			fwrite( &pObsObj->fPosX, sizeof(pObsObj->fPosX), 1, pFile );
			fwrite( &pObsObj->fPosY, sizeof(pObsObj->fPosY), 1, pFile );
			fwrite( &pObsObj->fPosZ, sizeof(pObsObj->fPosZ), 1, pFile );
			fwrite( &pObsObj->fScaleX, sizeof(pObsObj->fScaleX), 1, pFile );
			fwrite( &pObsObj->fScaleY, sizeof(pObsObj->fScaleY), 1, pFile );
			fwrite( &pObsObj->fScaleZ, sizeof(pObsObj->fScaleZ), 1, pFile );

			fwrite( &pObsObj->fYRotVal, sizeof(pObsObj->fYRotVal), 1, pFile );
		}

		fclose( pFile );
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi group in-door info
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviGroupInDoorInfo::CNtlNaviGroupInDoorInfo( void )
{
}

CNtlNaviGroupInDoorInfo::~CNtlNaviGroupInDoorInfo( void )
{
	vecdef_NAVI_OBJECT_LSIT::iterator itNaviMesh = m_defNaviMeshList.begin();
	for ( ; itNaviMesh != m_defNaviMeshList.end(); ++itNaviMesh )
	{
		delete *itNaviMesh;
	}
	m_defNaviMeshList.clear();

	vecdef_NAVI_OBJECT_LSIT::iterator itObsMesh = m_defObsMeshList.begin();
	for ( ; itObsMesh != m_defObsMeshList.end(); ++itObsMesh )
	{
		delete *itObsMesh;
	}
	m_defObsMeshList.clear();
}

void CNtlNaviGroupInDoorInfo::SetModelName( const char* pModelName )
{
	m_strModelName = pModelName;
}

int CNtlNaviGroupInDoorInfo::GetNaviMeshCnt( void )
{
	return (int)m_defNaviMeshList.size();
}

CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* CNtlNaviGroupInDoorInfo::GetNaviMesh( int nIdx )
{
	return m_defNaviMeshList[nIdx];
}

void CNtlNaviGroupInDoorInfo::AttachNaviMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal )
{
	sNAVI_OBJECT_LIST* pObjInfo = new sNAVI_OBJECT_LIST;

	pObjInfo->strModelName = pModelName;
	pObjInfo->fPosX = fPosX;
	pObjInfo->fPosY = fPosY;
	pObjInfo->fPosZ = fPosZ;
	pObjInfo->fScaleX = fScaleX;
	pObjInfo->fScaleY = fScaleY;
	pObjInfo->fScaleZ = fScaleZ;
	pObjInfo->fYRotVal = fYRotVal;

	m_defNaviMeshList.push_back( pObjInfo );
}

int CNtlNaviGroupInDoorInfo::GetObsMeshCnt( void )
{
	return (int)m_defObsMeshList.size();
}

CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* CNtlNaviGroupInDoorInfo::GetObsMesh( int nIdx )
{
	return m_defObsMeshList[nIdx];
}

void CNtlNaviGroupInDoorInfo::AttachObsMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal )
{
	sNAVI_OBJECT_LIST* pObjInfo = new sNAVI_OBJECT_LIST;

	pObjInfo->strModelName = pModelName;
	pObjInfo->fPosX = fPosX;
	pObjInfo->fPosY = fPosY;
	pObjInfo->fPosZ = fPosZ;
	pObjInfo->fScaleX = fScaleX;
	pObjInfo->fScaleY = fScaleY;
	pObjInfo->fScaleZ = fScaleZ;
	pObjInfo->fYRotVal = fYRotVal;

	m_defObsMeshList.push_back( pObjInfo );
}

bool CNtlNaviGroupInDoorInfo::Import( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_GROUP_INDOOR_NAME( buf );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "rb" );

	if ( pFile )
	{
		char szIndentity[64]; memset( szIndentity, 0, sizeof(szIndentity) );
		fread( szIndentity, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );
		if ( stricmp( szIndentity, IDENTITY_NAVI_GROUP ) != 0 )
		{
			fclose( pFile );
			return false;
		}

		unsigned int uiVersion;
		fread( &uiVersion, sizeof(uiVersion), 1, pFile );
		if ( uiVersion != VERSION_NAVI_GROUP )
		{
			fclose( pFile );
			return false;
		}

		unsigned int uiModelNameLen;
		fread( &uiModelNameLen, sizeof(uiModelNameLen), 1, pFile );

		char* pszModelName = new char[uiModelNameLen+1];
		fread( pszModelName, sizeof(char) * uiModelNameLen, 1, pFile );
		pszModelName[uiModelNameLen] = '\0';
		m_strModelName = pszModelName;
		delete pszModelName;

		unsigned int i;

		unsigned int uiNaviCnt;
		fread( &uiNaviCnt, sizeof(uiNaviCnt), 1, pFile );

		for ( i = 0; i < uiNaviCnt; ++i )
		{
			unsigned int uiNameLen;
			fread( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			char* pszName = new char[uiNameLen+1];
			fread( pszName, sizeof(char) * uiNameLen, 1, pFile );
			pszName[uiNameLen] = '\0';

			float fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal;

			fread( &fPosX, sizeof(fPosX), 1, pFile );
			fread( &fPosY, sizeof(fPosY), 1, pFile );
			fread( &fPosZ, sizeof(fPosZ), 1, pFile );
			fread( &fScaleX, sizeof(fScaleX), 1, pFile );
			fread( &fScaleY, sizeof(fScaleY), 1, pFile );
			fread( &fScaleZ, sizeof(fScaleZ), 1, pFile );
			fread( &fYRotVal, sizeof(fYRotVal), 1, pFile );

			AttachNaviMesh( pszName, fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal );

			delete pszName;
		}

		unsigned int uiObsCnt;
		fread( &uiObsCnt, sizeof(uiObsCnt), 1, pFile );

		for ( i = 0; i < uiObsCnt; ++i )
		{
			unsigned int uiNameLen;
			fread( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			char* pszName = new char[uiNameLen+1];
			fread( pszName, sizeof(char) * uiNameLen, 1, pFile );
			pszName[uiNameLen] = '\0';

			float fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal;

			fread( &fPosX, sizeof(fPosX), 1, pFile );
			fread( &fPosY, sizeof(fPosY), 1, pFile );
			fread( &fPosZ, sizeof(fPosZ), 1, pFile );
			fread( &fScaleX, sizeof(fScaleX), 1, pFile );
			fread( &fScaleY, sizeof(fScaleY), 1, pFile );
			fread( &fScaleZ, sizeof(fScaleZ), 1, pFile );
			fread( &fYRotVal, sizeof(fYRotVal), 1, pFile );

			AttachObsMesh( pszName, fPosX, fPosY, fPosZ, fScaleX, fScaleY, fScaleZ, fYRotVal );

			delete pszName;
		}

		fclose( pFile );
	}

	return true;
}

bool CNtlNaviGroupInDoorInfo::Export( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_GROUP_INDOOR_NAME( buf );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "wb" );

	if ( pFile )
	{
		fwrite( IDENTITY_NAVI_GROUP, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );

		unsigned int uiVersion = VERSION_NAVI_GROUP;
		fwrite( &uiVersion, sizeof(uiVersion), 1, pFile );

		unsigned int uiNameLen = (unsigned int)m_strModelName.size();
		fwrite( &uiNameLen, sizeof(uiNameLen), 1, pFile );

		fwrite( m_strModelName.c_str(), sizeof(char) * uiNameLen, 1, pFile );

		unsigned int uiNaviCnt = (unsigned int)m_defNaviMeshList.size();
		fwrite( &uiNaviCnt, sizeof(uiNaviCnt), 1, pFile );

		vecdef_NAVI_OBJECT_LSIT::iterator itNavi = m_defNaviMeshList.begin();
		for ( ; itNavi != m_defNaviMeshList.end(); ++itNavi )
		{
			sNAVI_OBJECT_LIST* pNaviObj = *itNavi;

			uiNameLen = (unsigned int)pNaviObj->strModelName.size();
			fwrite( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			fwrite( pNaviObj->strModelName.c_str(), uiNameLen, 1, pFile );

			fwrite( &pNaviObj->fPosX, sizeof(pNaviObj->fPosX), 1, pFile );
			fwrite( &pNaviObj->fPosY, sizeof(pNaviObj->fPosY), 1, pFile );
			fwrite( &pNaviObj->fPosZ, sizeof(pNaviObj->fPosZ), 1, pFile );
			fwrite( &pNaviObj->fScaleX, sizeof(pNaviObj->fScaleX), 1, pFile );
			fwrite( &pNaviObj->fScaleY, sizeof(pNaviObj->fScaleY), 1, pFile );
			fwrite( &pNaviObj->fScaleZ, sizeof(pNaviObj->fScaleZ), 1, pFile );

			fwrite( &pNaviObj->fYRotVal, sizeof(pNaviObj->fYRotVal), 1, pFile );
		}

		unsigned int uiObsCnt = (unsigned int)m_defObsMeshList.size();
		fwrite( &uiObsCnt, sizeof(uiObsCnt), 1, pFile );

		vecdef_NAVI_OBJECT_LSIT::iterator itObs = m_defObsMeshList.begin();
		for ( ; itObs != m_defObsMeshList.end(); ++itObs )
		{
			sNAVI_OBJECT_LIST* pObsObj = *itObs;

			uiNameLen = (unsigned int)pObsObj->strModelName.size();
			fwrite( &uiNameLen, sizeof(uiNameLen), 1, pFile );

			fwrite( pObsObj->strModelName.c_str(), uiNameLen, 1, pFile );

			fwrite( &pObsObj->fPosX, sizeof(pObsObj->fPosX), 1, pFile );
			fwrite( &pObsObj->fPosY, sizeof(pObsObj->fPosY), 1, pFile );
			fwrite( &pObsObj->fPosZ, sizeof(pObsObj->fPosZ), 1, pFile );
			fwrite( &pObsObj->fScaleX, sizeof(pObsObj->fScaleX), 1, pFile );
			fwrite( &pObsObj->fScaleY, sizeof(pObsObj->fScaleY), 1, pFile );
			fwrite( &pObsObj->fScaleZ, sizeof(pObsObj->fScaleZ), 1, pFile );

			fwrite( &pObsObj->fYRotVal, sizeof(pObsObj->fYRotVal), 1, pFile );
		}

		fclose( pFile );
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi property out-door info
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviPropOutDoorInfo::CNtlNaviPropOutDoorInfo( void )
{
	m_uiFieldID = 0xffffffff;
	m_uiFieldProp = 0;
	m_uiTileCntOfField = 0;
	m_pTileProp = NULL;
}

CNtlNaviPropOutDoorInfo::~CNtlNaviPropOutDoorInfo( void )
{
	if ( m_pTileProp )
	{
		delete m_pTileProp;
		m_pTileProp = NULL;
	}
}

void CNtlNaviPropOutDoorInfo::SetFieldID( unsigned int uiFieldID )
{
	m_uiFieldID = uiFieldID;
}

void CNtlNaviPropOutDoorInfo::SetFieldProp( unsigned int uiFieldProp )
{
	m_uiFieldProp = uiFieldProp;
}

void CNtlNaviPropOutDoorInfo::SetTileCntOfField( unsigned int uiTileCntOfField )
{
	m_uiTileCntOfField = uiTileCntOfField;
}

void CNtlNaviPropOutDoorInfo::SetTileProp( unsigned int* pTileProp )
{
	m_pTileProp = pTileProp;
}

bool CNtlNaviPropOutDoorInfo::Import( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_PROINFO_OUTDOOR_NAME( buf, m_uiFieldID );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "rb" );

	if ( pFile )
	{
		char szIndentity[64]; memset( szIndentity, 0, sizeof(szIndentity) );
		fread( szIndentity, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );
		if ( stricmp( szIndentity, IDENTITY_NAVI_PROP ) != 0 )
		{
			fclose( pFile );
			return false;
		}

		unsigned int uiVersion;
		fread( &uiVersion, sizeof(uiVersion), 1, pFile );
		if ( uiVersion != VERSION_NAVI_PROP )
		{
			fclose( pFile );
			return false;
		}

		fread( &m_uiFieldID, sizeof(m_uiFieldID), 1, pFile );

		fread( &m_uiFieldProp, sizeof(m_uiFieldProp), 1, pFile );

		fread( &m_uiTileCntOfField, sizeof(m_uiTileCntOfField), 1, pFile );

		m_pTileProp = new unsigned int[m_uiTileCntOfField];
		fread( m_pTileProp, sizeof(unsigned int) * m_uiTileCntOfField, 1, pFile );

		fclose( pFile );
	}

	return true;
}

bool CNtlNaviPropOutDoorInfo::Export( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_PROINFO_OUTDOOR_NAME( buf, m_uiFieldID );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "wb" );

	if ( pFile )
	{
		fwrite( IDENTITY_NAVI_PROP, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );

		unsigned int uiVersion = VERSION_NAVI_PROP;
		fwrite( &uiVersion, sizeof(uiVersion), 1, pFile );

		fwrite( &m_uiFieldID, sizeof(m_uiFieldID), 1, pFile );

		fwrite( &m_uiFieldProp, sizeof(m_uiFieldProp), 1, pFile );

		fwrite( &m_uiTileCntOfField, sizeof(m_uiTileCntOfField), 1, pFile );

		fwrite( m_pTileProp, sizeof(unsigned int) * m_uiTileCntOfField, 1, pFile );

		fclose( pFile );
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi property in-door info
//
//////////////////////////////////////////////////////////////////////////


CNtlNaviPropInDoorInfo::CNtlNaviPropInDoorInfo( void )
{
	m_uiBlockID = 0xffffffff;
	m_uiBlockProp = 0;
	m_uiGameProp = 0;
}

CNtlNaviPropInDoorInfo::~CNtlNaviPropInDoorInfo( void )
{
	vecdef_NAVI_ENTITY_PROP_LIST::iterator it = m_defEntityPropList.begin();
	for ( ; it != m_defEntityPropList.end(); ++it )
	{
		delete *it;
	}
	m_defEntityPropList.clear();
}

void CNtlNaviPropInDoorInfo::SetBlockID( unsigned int uiBlockID )
{
	m_uiBlockID = uiBlockID;
}

void CNtlNaviPropInDoorInfo::SetBlockProp( unsigned int uiBlockProp )
{
	m_uiBlockProp = uiBlockProp;
}

void CNtlNaviPropInDoorInfo::SetGameProp( unsigned int uiGameProp )
{
	m_uiGameProp = uiGameProp;
}

int CNtlNaviPropInDoorInfo::GetEntityInfoCnt( void )
{
	return (int)m_defEntityPropList.size();
}

CNtlNaviPropInDoorInfo::sENTITY_INFO* CNtlNaviPropInDoorInfo::GetEntityInfo( int nIdx )
{
	return m_defEntityPropList[nIdx];
}

void CNtlNaviPropInDoorInfo::AttachEntityInfo_Sphere( float fPosX, float fPosY, float fPosZ, float fRadius )
{
	sENTITY_INFO* pEntityInfo = new sENTITY_INFO;

	pEntityInfo->eEntityType = eENTITY_TYPE_SPHERE;
	pEntityInfo->uEntityData.sSphere.fX = fPosX;
	pEntityInfo->uEntityData.sSphere.fY = fPosY;
	pEntityInfo->uEntityData.sSphere.fZ = fPosZ;
	pEntityInfo->uEntityData.sSphere.fRadius = fRadius;

	m_defEntityPropList.push_back( pEntityInfo );
}

void CNtlNaviPropInDoorInfo::AttachEntityInfo_Plane( float fPosX, float fPosY, float fPosZ, float fWidth, float fDepth )
{
	sENTITY_INFO* pEntityInfo = new sENTITY_INFO;

	pEntityInfo->eEntityType = eENTITY_TYPE_PLANE;
	pEntityInfo->uEntityData.sPlane.fX = fPosX;
	pEntityInfo->uEntityData.sPlane.fY = fPosY;
	pEntityInfo->uEntityData.sPlane.fZ = fPosZ;
	pEntityInfo->uEntityData.sPlane.fWidth = fWidth;
	pEntityInfo->uEntityData.sPlane.fDepth = fDepth;

	m_defEntityPropList.push_back( pEntityInfo );
}

bool CNtlNaviPropInDoorInfo::Import( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_PROINFO_INDOOR_NAME( buf, m_uiBlockID );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "rb" );

	if ( pFile )
	{
		char szIndentity[64]; memset( szIndentity, 0, sizeof(szIndentity) );
		fread( szIndentity, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );
		if ( stricmp( szIndentity, IDENTITY_NAVI_PROP ) != 0 )
		{
			fclose( pFile );
			return false;
		}

		unsigned int uiVersion;
		fread( &uiVersion, sizeof(uiVersion), 1, pFile );
		if ( uiVersion != VERSION_NAVI_PROP )
		{
			fclose( pFile );
			return false;
		}

		fread( &m_uiBlockID, sizeof(m_uiBlockID), 1, pFile );

		fread( &m_uiBlockProp, sizeof(m_uiBlockProp), 1, pFile );

		fread( &m_uiGameProp, sizeof(m_uiGameProp), 1, pFile );

		unsigned int i;

		unsigned int uiEntityCnt;
		fread( &uiEntityCnt, sizeof(uiEntityCnt), 1, pFile );

		for ( i = 0; i < uiEntityCnt; ++i )
		{
			unsigned char byEntityType;
			fread( &byEntityType, sizeof(byEntityType), 1, pFile );

			switch ( byEntityType )
			{
			case eENTITY_TYPE_SPHERE:
				{
					float fX, fY, fZ, fRadius;

					fread( &fX, sizeof(fX), 1, pFile );
					fread( &fY, sizeof(fY), 1, pFile );
					fread( &fZ, sizeof(fZ), 1, pFile );
					fread( &fRadius, sizeof(fRadius), 1, pFile );

					AttachEntityInfo_Sphere( fX, fY, fZ, fRadius );
				}
				break;
			case eENTITY_TYPE_PLANE:
				{
					float fX, fY, fZ, fWidth, fDepth;

					fread( &fX, sizeof(fX), 1, pFile );
					fread( &fY, sizeof(fY), 1, pFile );
					fread( &fZ, sizeof(fZ), 1, pFile );
					fread( &fWidth, sizeof(fWidth), 1, pFile );
					fread( &fDepth, sizeof(fDepth), 1, pFile );

					AttachEntityInfo_Plane( fX, fY, fZ, fWidth, fDepth );
				}
				break;
			}
		}

		fclose( pFile );
	}

	return true;
}

bool CNtlNaviPropInDoorInfo::Export( const char* pPathName )
{
	std::string strPathName = pPathName;
	Util_Add_Inv_Slash( strPathName );

	char buf[1024];
	NAVI_PROINFO_INDOOR_NAME( buf, m_uiBlockID );
	strPathName += buf;

	FILE* pFile;
	fopen_s( &pFile, strPathName.c_str(), "wb" );

	if ( pFile )
	{
		fwrite( IDENTITY_NAVI_PROP, sizeof(char) * IDENTITY_SIZE_BY_BYTE, 1, pFile );

		unsigned int uiVersion = VERSION_NAVI_PROP;
		fwrite( &uiVersion, sizeof(uiVersion), 1, pFile );

		fwrite( &m_uiBlockID, sizeof(m_uiBlockID), 1, pFile );

		fwrite( &m_uiBlockProp, sizeof(m_uiBlockProp), 1, pFile );

		fwrite( &m_uiGameProp, sizeof(m_uiGameProp), 1, pFile );

		unsigned int uiEntityCnt = (unsigned int)m_defEntityPropList.size();
		fwrite( &uiEntityCnt, sizeof(uiEntityCnt), 1, pFile );

		vecdef_NAVI_ENTITY_PROP_LIST::iterator it = m_defEntityPropList.begin();
		for ( ; it != m_defEntityPropList.end(); ++it )
		{
			sENTITY_INFO* pEntityInfo = *it;

			unsigned char byEntityType = (unsigned char)pEntityInfo->eEntityType;
			fwrite( &byEntityType, sizeof(unsigned char), 1, pFile );

			switch ( pEntityInfo->eEntityType )
			{
			case eENTITY_TYPE_SPHERE:
				{
					fwrite( &pEntityInfo->uEntityData.sSphere.fX, sizeof(pEntityInfo->uEntityData.sSphere.fX), 1, pFile );
					fwrite( &pEntityInfo->uEntityData.sSphere.fY, sizeof(pEntityInfo->uEntityData.sSphere.fY), 1, pFile );
					fwrite( &pEntityInfo->uEntityData.sSphere.fZ, sizeof(pEntityInfo->uEntityData.sSphere.fZ), 1, pFile );
					fwrite( &pEntityInfo->uEntityData.sSphere.fRadius, sizeof(pEntityInfo->uEntityData.sSphere.fRadius), 1, pFile );
				}
				break;
			case eENTITY_TYPE_PLANE:
				{
					fwrite( &pEntityInfo->uEntityData.sPlane.fX, sizeof(pEntityInfo->uEntityData.sPlane.fX), 1, pFile );
					fwrite( &pEntityInfo->uEntityData.sPlane.fY, sizeof(pEntityInfo->uEntityData.sPlane.fY), 1, pFile );
					fwrite( &pEntityInfo->uEntityData.sPlane.fZ, sizeof(pEntityInfo->uEntityData.sPlane.fZ), 1, pFile );
					fwrite( &pEntityInfo->uEntityData.sPlane.fWidth, sizeof(pEntityInfo->uEntityData.sPlane.fWidth), 1, pFile );
					fwrite( &pEntityInfo->uEntityData.sPlane.fDepth, sizeof(pEntityInfo->uEntityData.sPlane.fDepth), 1, pFile );
				}
				break;
			}
		}

		fclose( pFile );
	}

	return true;
}
