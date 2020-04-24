/*****************************************************************************
*
* File			: PSMngConfig.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "StdAfx.h"
#include "PSMngConfig.h"
#include "NtlXMLDoc.h"
#include "Utility.h"


//////////////////////////////////////////////////////////////////////////
//
// CPSMngConfig
//
//////////////////////////////////////////////////////////////////////////


#define	BUILD_ROOT_PATH							_T( "CONFIG/BUILD_ROOT_PATH" )
#define	BUILD_CLIENT_FULL_VER_PATH				_T( "CONFIG/BUILD_CLIENT_FULL_VER_PATH" )
#define	BUILD_HISTORY_DATA_PATH					_T( "CONFIG/BUILD_HISTORY_DATA_PATH" )
#define	BUILD_ROLLBACK_DATA_PATH				_T( "CONFIG/BUILD_ROLLBACK_DATA_PATH" )
#define	BUILD_UPLOAD_DATA_PATH					_T( "CONFIG/BUILD_UPLOAD_DATA_PATH" )
#define	BUILD_CUR_PATCH_VER_LIST_FILENAME		_T( "CONFIG/BUILD_CUR_PATCH_VER_LIST_FILENAME" )

#define	PATCH_ROOT_PATH							_T( "CONFIG/PATCH_ROOT_PATH" )
#define	PATCH_CLIENT_FULL_VER_PATH				_T( "CONFIG/PATCH_CLIENT_FULL_VER_PATH" )
#define	PATCH_LANGUAGE_PATH						_T( "CONFIG/PATCH_LANGUAGE_PATH" )
#define	PATCH_WEB_PATH							_T( "CONFIG/PATCH_WEB_PATH" )
#define	PATCH_PATCH_VER_LIST_FILENAME			_T( "CONFIG/PATCH_PATCH_VER_LIST_FILENAME" )

#define	INTEGRITY_BUILD_FILE_NAME				_T( "CONFIG/INTEGRITY_BUILD_FILE_NAME" )
#define	INTEGRITY_BUILD_SKIP_ENTITY				_T( "CONFIG/INTEGRITY_BUILD_SKIP_ENTITY" )
#define	INTEGRITY_BUILD_SKIP_ENTITY_ATTR_NAME	_T( "NAME" )
#define	INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE	_T( "TYPE" )

#define	INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE_FILE		_T( "FILE" )
#define	INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE_FOLDER	_T( "FOLDER" )


bool CPSMngConfig::Load( CString strPath )
{
	CNtlXMLDoc doc;
	if ( !doc.Create() )
	{
		return false;
	}

	LPCTSTR pPath = (LPCTSTR)strPath;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
		return false;
	}

	TCHAR szBuffer[4096] = { 0, };

	if ( !doc.GetDataWithXPath( BUILD_ROOT_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strBuildRootPath = szBuffer;

	if ( !doc.GetDataWithXPath( BUILD_CLIENT_FULL_VER_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strBuildClientFullVerPath = szBuffer;

	if ( !doc.GetDataWithXPath( BUILD_HISTORY_DATA_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strBuildHistoryDataPath = szBuffer;

	if ( !doc.GetDataWithXPath( BUILD_ROLLBACK_DATA_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strBuildRollbackPath = szBuffer;

	if ( !doc.GetDataWithXPath( BUILD_UPLOAD_DATA_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strBuildUploadDataPath = szBuffer;

	if ( !doc.GetDataWithXPath( BUILD_CUR_PATCH_VER_LIST_FILENAME, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strBuildCurPatchVerListFileName = szBuffer;

	if ( !doc.GetDataWithXPath( PATCH_ROOT_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchRootPath = szBuffer;

	if ( !doc.GetDataWithXPath( PATCH_CLIENT_FULL_VER_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchClientFullVerPath = szBuffer;

	if ( !doc.GetDataWithXPath( PATCH_LANGUAGE_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchLanguagePath = szBuffer;

	if ( !doc.GetDataWithXPath( PATCH_WEB_PATH, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchWebPath = szBuffer;

	if ( !doc.GetDataWithXPath( PATCH_PATCH_VER_LIST_FILENAME, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchPatchVerListFileName = szBuffer;

	if ( !doc.GetDataWithXPath( INTEGRITY_BUILD_FILE_NAME, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strIntegrityBuildFileName = szBuffer;

	IXMLDOMNodeList* pIBEntityList = doc.SelectNodeList( INTEGRITY_BUILD_SKIP_ENTITY );

	if ( pIBEntityList )
	{
		long lCnt;

		if ( S_OK != pIBEntityList->get_length( &lCnt ) )
		{
			pIBEntityList->Release();

			return false;
		}

		sINTEGRITY_SKIP_ENTITY_INFO sEntityInfo;

		for ( long i = 0; i < lCnt; ++i )
		{
			IXMLDOMNode* pEntityNode;

			if ( S_OK != pIBEntityList->get_item( i, &pEntityNode ) )
			{
				pIBEntityList->Release();

				return false;
			}

			if ( !doc.GetTextWithAttributeName( pEntityNode, INTEGRITY_BUILD_SKIP_ENTITY_ATTR_NAME, szBuffer, 4096 ) )
			{
				pEntityNode->Release();

				pIBEntityList->Release();

				return false;
			}

			sEntityInfo.strName = szBuffer;

			if ( !doc.GetTextWithAttributeName( pEntityNode, INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE, szBuffer, 4096 ) )
			{
				pEntityNode->Release();

				pIBEntityList->Release();

				return false;
			}

			sEntityInfo.strType = szBuffer;

			pEntityNode->Release();

			sEntityInfo.strName.MakeLower();
			sEntityInfo.strType.MakeLower();

			if ( sEntityInfo.strType == CString( INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE_FOLDER ).MakeLower() ||
				 sEntityInfo.strType == CString( INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE_FILE ).MakeLower() )
			{
				CString strKey = sEntityInfo.strName + sEntityInfo.strType;

				m_defIntegritySkipEntityList[strKey] = sEntityInfo;
			}
			else
			{
				pIBEntityList->Release();

				return false;
			}
		}

		pIBEntityList->Release();
	}

	{
		CString strCheckPath = m_strPatchRootPath;
		AttachBackSlash( strCheckPath );
		strCheckPath += m_strPatchClientFullVerPath;
		AttachBackSlash( strCheckPath );
		strCheckPath += _T("*");

		WIN32_FIND_DATA FindFileData;

		HANDLE hFind = FindFirstFile( strCheckPath, &FindFileData );

		if ( INVALID_HANDLE_VALUE != hFind )
		{
			int nCnt = 0;

			do
			{
				if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					if ( _T('.') != FindFileData.cFileName[0] )
					{
						nCnt++;
					}
				}
				else
				{
					nCnt++;
				}
			}
			while ( FindNextFile( hFind, &FindFileData ) != 0 );

			FindClose( hFind );

			if ( nCnt > 0 )
			{
				m_bIsInitClientFullVer = false;
			}
			else
			{
				m_bIsInitClientFullVer = true;
			}
		}
		else
		{
			m_bIsInitClientFullVer = true;
		}
	}

	return true;
}


bool CPSMngConfig::HasIntegrityEntity( TCHAR* pEntityName, bool bFolder )
{
	CString strKey = pEntityName;

	if ( bFolder )
	{
		strKey += INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE_FOLDER;
	}
	else
	{
		strKey += INTEGRITY_BUILD_SKIP_ENTITY_ATTR_TYPE_FILE;
	}

	strKey.MakeLower();

	return (m_defIntegritySkipEntityList.find( strKey ) == m_defIntegritySkipEntityList.end() ? false : true);
}


bool CPSMngConfig::IsInitClientFullVer( void )
{
	return m_bIsInitClientFullVer;
}
