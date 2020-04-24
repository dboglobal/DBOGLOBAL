#include "precomp_trigger.h"
#include "NtlTSScrSaver.h"
#include "NtlTSScrObject.h"
#include "NtlTSScrFactory.h"
#include "NtlTSScrFactoryType.h"
#include "NtlTSLog.h"


/** 
	Script saver
*/


CNtlTSScrSaver::CNtlTSScrSaver( CNtlTSScrFactory* pObjFactory )
: m_pObjFactory( pObjFactory )
{
}

CNtlTSScrSaver::~CNtlTSScrSaver( void )
{
}

bool CNtlTSScrSaver::Save( const std::string& strScriptName, std::vector<CNtlTSScrObject*>& vecRootObjList )
{
	m_defObjInfoListMap.clear();
	m_defObjObjInfoMapList.clear();

	std::vector<CNtlTSScrObject*>::iterator itObj = vecRootObjList.begin();
	for ( ; itObj != vecRootObjList.end(); ++itObj )
	{
		if ( !MakeObjectInfoList( *itObj ) )
		{
			printf( "Saving is failed. Info[%s]. [%s]", strScriptName.c_str(), TS_CODE_TRACE() );
			return false;
		}
	}

	if ( !WriteToFile( strScriptName ) )
	{
		printf( "Can not write to the file. Info[%s]. [%s]", strScriptName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	return true;
}

bool CNtlTSScrSaver::MakeObjectInfoList( CNtlTSScrObject* pRootObj )
{
	std::vector<CNtlTSScrObject*> defObjList;
	defObjList.reserve( 20 );
	pRootObj->MakeAllObjectList( defObjList );

	mapdef_ObjInfoList& defObjInfoList = m_defObjInfoListMap.insert( mapdef_ObjInfoListMap::value_type( pRootObj, mapdef_ObjInfoList() ) ).first->second;

//	CNtlTSScrFactoryType::sPROPERTY_INFO* pPropInfo;
	std::vector<CNtlTSScrObject*>::iterator itObj = defObjList.begin();
	for ( ; itObj != defObjList.end(); ++itObj )
	{
		sOBJ_INFO& stObjInfo = defObjInfoList.insert( mapdef_ObjInfoList::value_type( *itObj, sOBJ_INFO() ) ).first->second;

		stObjInfo.pObj = *itObj;

		stObjInfo.pObjType = m_pObjFactory->GetFactoryType( stObjInfo.pObj->GetClassName() );
		if ( 0 == stObjInfo.pObjType )
		{
			printf( "Can not find the object type. Info[%s]. [%s]", stObjInfo.pObj->GetClassName(), TS_CODE_TRACE() );
			return false;
		}

/*		pPropInfo = stObjInfo.pObjType->BeginProperty();
		while ( pPropInfo )
		{
			stObjInfo.Property.AddProperty( *pPropInfo->pstrPropertyName, "" );
			pPropInfo = stObjInfo.pObjType->NextProperty();
		}
*/
		stObjInfo.pObj->TakeScriptDataForScript( stObjInfo.Property );
	}

	return BuildObjectInfoTree( pRootObj, defObjInfoList );
}

bool CNtlTSScrSaver::BuildObjectInfoTree( CNtlTSScrObject* pRootObj, mapdef_ObjInfoList& defObjInfoList )
{
	sOBJ_INFO* pRootObjInfo = 0;

	mapdef_ObjInfoList::iterator itObj = defObjInfoList.begin();
	for ( ; itObj != defObjInfoList.end(); ++itObj )
	{
		sOBJ_INFO& stObjInfo = itObj->second;

		if ( 0 == stObjInfo.pObj->GetParentForScript() )
		{
			if ( pRootObjInfo )
			{
				printf( "Only must have one parent. Info[%s]. [%s]", stObjInfo.pObj->GetClassName(), TS_CODE_TRACE() );
				return false;
			}

			pRootObjInfo = &itObj->second;
		}
		else
		{
			mapdef_ObjInfoList::iterator itFind = defObjInfoList.find( stObjInfo.pObj->GetParentForScript() );
			if ( itFind == defObjInfoList.end() )
			{
				printf( "Can not find the object. Info[%s]. [%s]", stObjInfo.pObj->GetClassName(), TS_CODE_TRACE() );
				return false;
			}

			itFind->second.listChildren.push_back( &itObj->second );
		}
	}

	if ( 0 == pRootObjInfo )
	{
		printf( "Can not find the root object. [%s]", TS_CODE_TRACE() );
		return false;
	}

	m_defObjObjInfoMapList[pRootObj] = pRootObjInfo;

	return true;
}

bool CNtlTSScrSaver::WriteToFile( const std::string& strScriptName )
{
	FILE* pFile;
	if ( 0 != fopen_s( &pFile, strScriptName.c_str(), "wb" ) )
	{
		printf( "Can not open the file. [%s]. [%s]", strScriptName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	mapdef_Object_ObjInfo_MapList::iterator it = m_defObjObjInfoMapList.begin();
	for ( ; it != m_defObjObjInfoMapList.end(); ++it )
	{
		sOBJ_INFO* pstObjInfo = it->second;

		if ( !WriteObj( pFile, pstObjInfo, 0 ) )
		{
			fclose( pFile );
			printf( "Writing the file is failed. [%s]. [%s]", strScriptName.c_str(), TS_CODE_TRACE() );
			return false;
		}

		fprintf_s( pFile, "\n" );
	}

	fclose( pFile );

	return true;
}

bool CNtlTSScrSaver::WriteObj( FILE* pFile, sOBJ_INFO* pstObjInfo, int nDepth )
{
	std::string strTab;
	for ( int i = 0; i < nDepth; ++i ) strTab += "\t";

	fprintf_s( pFile, "%s%s\n", strTab.c_str(), pstObjInfo->pObj->GetClassName() );
	fprintf_s( pFile, "%s{\n", strTab.c_str() );

	CNtlTSScrProperty::PROP_CIT cit = pstObjInfo->Property.Begin();
	CNtlTSScrProperty::PROP_CIT citEnd = pstObjInfo->Property.End();
	for ( ; cit != citEnd; ++cit )
	{
		switch ( pstObjInfo->pObjType->GetPropertyType( cit->first.c_str() ) )
		{
		case CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_BOOL:
			{
				fprintf_s( pFile, "%s\t%s = %s;\n", strTab.c_str(), cit->first.c_str(), cit->second.c_str() );
			}
			break;
		case CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_NUMBER:
			{
				unsigned int uiVal = atoi( cit->second.c_str() );
				fprintf_s( pFile, "%s\t%s = %d;\n", strTab.c_str(), cit->first.c_str(), uiVal );
			}
			break;
		case CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE_STRING:
			{
				fprintf_s( pFile, "%s\t%s = \"%s\";\n", strTab.c_str(), cit->first.c_str(), cit->second.c_str() );
			}
			break;
		default:
			printf( "Can not find the matching property type. Info[%s]. [%s]", cit->first.c_str(), TS_CODE_TRACE() );
			return false;
		}
	}

	if ( !pstObjInfo->listChildren.empty() )
	{
		fprintf_s( pFile, "\n" );
	}

	std::list< sOBJ_INFO* >::iterator itChild = pstObjInfo->listChildren.begin();
	for ( ; itChild != pstObjInfo->listChildren.end(); ++itChild )
	{
		WriteObj( pFile, *itChild, nDepth + 1 );
	}

	fprintf_s( pFile, "%s}\n", strTab.c_str() );

	return true;
}
