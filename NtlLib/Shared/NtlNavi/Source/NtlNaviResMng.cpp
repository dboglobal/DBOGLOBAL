#include "precomp_navi.h"
#include "NtlNaviResMng.h"
#include "NtlNaviLog.h"
#include "NtlNaviUtility.h"


CNtlNaviResMng::CNtlNaviResMng( void )
{
}

CNtlNaviResMng::~CNtlNaviResMng( void )
{
}

bool CNtlNaviResMng::Load( const char* pPath )
{
	std::string strFileName, strModelName;

	std::string strFolderName = pPath;
	AttachBackSlash( strFolderName );

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile( (strFolderName + "*.*").c_str(), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		CNtlNaviLog::GetInstance()->Log( "Can not load navigation resource manager. [%s]", pPath );

		return false;
	}

	do
	{
		if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 (FindFileData.cFileName[0] != '.') )
		{
			strFileName = MakeUpperCase( FindFileData.cFileName );

			std::string strFullPath = strFolderName + strFileName;
			strFullPath = MakeUpperCase( strFullPath );

			switch ( SplitResName( strFileName, strModelName ) )
			{
			case eNAVI_MESH_TYPE_NAVI_MESH:
				{
					m_defNaviResList.insert( mmapdef_RES_NAME_LIST::value_type( strModelName, strFullPath ) );
				}
				break;

			case eNAVI_MESH_TYPE_OBS_MESH:
				{
					m_defObsResList.insert( mmapdef_RES_NAME_LIST::value_type( strModelName, strFullPath ) );
				}
				break;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

const char* CNtlNaviResMng::BeginNaviRes( const char* pModelName )
{
	std::string strModelName = MakeUpperCase( pModelName );
	m_pairResNameList = m_defNaviResList.equal_range( strModelName );
	m_defResNameListIT = m_pairResNameList.first;
	if ( m_defResNameListIT == m_pairResNameList.second ) return NULL;
	return m_defResNameListIT->second.c_str();
}

const char* CNtlNaviResMng::NextNaviRes( void )
{
	++m_defResNameListIT;
	if ( m_defResNameListIT == m_pairResNameList.second ) return NULL;
	return m_defResNameListIT->second.c_str();
}

const char* CNtlNaviResMng::BeginObsRes( const char* pModelName )
{
	std::string strModelName = MakeUpperCase( pModelName );
	m_pairResNameList = m_defObsResList.equal_range( strModelName );
	m_defResNameListIT = m_pairResNameList.first;
	if ( m_defResNameListIT == m_pairResNameList.second ) return NULL;
	return m_defResNameListIT->second.c_str();
}

const char* CNtlNaviResMng::NextObsRes( void )
{
	++m_defResNameListIT;
	if ( m_defResNameListIT == m_pairResNameList.second ) return NULL;
	return m_defResNameListIT->second.c_str();
}

CNtlNaviResMng::eNAVI_MESH_TYPE	CNtlNaviResMng::SplitResName( const std::string& strFileName, std::string& strModelName )
{
	if ( std::string::npos == strFileName.find( MakeUpperCase( PE_DATA_MESH_EXT ) ) )
	{
		return eNAVI_MESH_TYPE_INVALID;
	}

	std::string::size_type pos;

	pos = strFileName.find( MakeUpperCase( PE_DATA_NAVI_MESH_KEY ) );

	if ( std::string::npos != pos )
	{
		strModelName = strFileName.substr( 0, pos );
		return eNAVI_MESH_TYPE_NAVI_MESH;
	}

	pos = strFileName.find( MakeUpperCase( PE_DATA_OBS_MESH_KEY ) );

	if ( std::string::npos != pos )
	{
		strModelName = strFileName.substr( 0, pos );
		return eNAVI_MESH_TYPE_OBS_MESH;
	}

	return eNAVI_MESH_TYPE_INVALID;
}
