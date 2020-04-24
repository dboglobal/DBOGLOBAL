#include "precomp_navi.h"
#include "NtlNaviWorldProp_WE.h"
#include "../NtlNaviDefine.h"
#include "NtlNaviUtility.h"
#include "NtlNaviFieldProp_WE.h"
#include "NtlNaviEntityFactory.h"


CNtlNaviWorldProp_WE::CNtlNaviWorldProp_WE( void )
: m_pParent( NULL )
{
}

CNtlNaviWorldProp_WE::~CNtlNaviWorldProp_WE( void )
{
	Destroy();
}

CNtlNaviWorld_WE* CNtlNaviWorldProp_WE::GetParent( void )
{
	return m_pParent;
}

void CNtlNaviWorldProp_WE::SetParent( CNtlNaviWorld_WE* pParent )
{
	m_pParent = pParent;
}

bool CNtlNaviWorldProp_WE::ImportWorldData( const char* pPath )
{
	std::string strPropertyFolder = pPath;
	AttachBackSlash( strPropertyFolder );

	strPropertyFolder += WE_FIELD_PROPERTY_FOLDER;
	AttachBackSlash( strPropertyFolder );

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile( (strPropertyFolder + "*.*").c_str(), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		Destroy();

		return false;
	}

	do
	{
		if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 (FindFileData.cFileName[0] != '.') )
		{
			std::string strPropertyFileName = strPropertyFolder;
			strPropertyFileName += FindFileData.cFileName;

			CNtlNaviFieldProp_WE* pNaviFieldProp = CNtlNaviEntityFactory::CreateEntity_FieldProp_WE();

			if ( NULL == pNaviFieldProp )
			{
				FindClose( hFind );

				Destroy();

				return false;
			}

			pNaviFieldProp->SetParent( this );
			m_defFieldPropList.push_back( pNaviFieldProp );

			if ( !pNaviFieldProp->ImportWorldData( strPropertyFileName.c_str() ) )
			{
				CNtlNaviEntityFactory::DeleteEntity_WE( pNaviFieldProp );

				FindClose( hFind );

				Destroy();

				return false;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

bool CNtlNaviWorldProp_WE::ExportPathData( const char* pPath )
{
	std::string strPropertyFile = pPath;
	AttachBackSlash( strPropertyFile );

	strPropertyFile += PE_WORLD_PROPERTY_FOLDER;
	AttachBackSlash( strPropertyFile );

	CreateDirectory( strPropertyFile.c_str(), NULL );

	strPropertyFile += PE_WORLD_PROPERTY_FILE;

	FILE* pFile;
	fopen_s( &pFile, strPropertyFile.c_str(), "wb" );

	if ( NULL == pFile )
	{
		return false;
	}

	// Field property count
	unsigned int uiFieldPropCnt = (unsigned int)m_defFieldPropList.size();
	fwrite( &uiFieldPropCnt, sizeof(uiFieldPropCnt), 1, pFile );

	fclose( pFile );

	listdef_FieldPropList::iterator it = m_defFieldPropList.begin();
	for ( ; it != m_defFieldPropList.end(); ++it )
	{
		if ( !(*it)->ExportPathData( pPath ) )
		{
			return false;
		}
	}

	return true;
}

void CNtlNaviWorldProp_WE::Destroy( void )
{
	listdef_FieldPropList::iterator it = m_defFieldPropList.begin();

	for ( ; it != m_defFieldPropList.end(); ++it )
	{
		CNtlNaviEntityFactory::DeleteEntity_WE( *it );
	}

	m_defFieldPropList.clear();
}
