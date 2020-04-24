#include "precomp_trigger.h"
#include "NtlTSEntityFactory.h"
#include "NtlTSEntityFactoryType.h"
#include "NtlTSEntity.h"


/**
	Entity factory
*/


CNtlTSEntityFactory::CNtlTSEntityFactory( void )
{
}

CNtlTSEntityFactory::~CNtlTSEntityFactory( void )
{
	UnRegisterEntityType();
}

CNtlTSScrFactoryType* CNtlTSEntityFactory::GetFactoryType( const std::string& strClassName )
{
	mapdef_EntityTypeList::iterator it = m_defEntityTypeList.find( strClassName );
	if ( it == m_defEntityTypeList.end() ) return 0;
	return it->second;
}

CNtlTSScrObject* CNtlTSEntityFactory::CreateObj( const std::string& strName )
{
	mapdef_EntityTypeList::iterator it = m_defEntityTypeList.find( strName );
	if ( it != m_defEntityTypeList.end() )
	{
		return it->second->CreateObj();
	}
	else
	{
		printf( "Can not find the class type. ParamInfo[%s]. [%s]", strName.c_str(), TS_CODE_TRACE() );
	}

	return 0;
}

void CNtlTSEntityFactory::DeleteObj( CNtlTSScrObject*& pObj )
{
	if ( pObj )
	{
		std::vector<CNtlTSScrObject*> defObjList;
		pObj->MakeAllObjectList( defObjList );

		mapdef_EntityTypeList::iterator itType;
		std::vector<CNtlTSScrObject*>::iterator itObj = defObjList.begin();
		for ( ; itObj != defObjList.end(); ++itObj )
		{
			itType = m_defEntityTypeList.find( (*itObj)->GetClassName() );
			if ( itType != m_defEntityTypeList.end() )
			{
				itType->second->DeleteObj( (*itObj) );
			}
			else
			{
				printf( "The class type is not exist. ParamInfo[%s]. [%s]", (*itObj)->GetClassName(), TS_CODE_TRACE() );
			}
		}

		pObj = 0;
	}
}

void CNtlTSEntityFactory::RegisterEntityType( void )
{
	m_defEntityTypeList[CNtlTSEntityFactoryType_CNtlTSEvent::OBJ_CLASS_NAME()]		= new CNtlTSEntityFactoryType_CNtlTSEvent;
	m_defEntityTypeList[CNtlTSEntityFactoryType_CNtlTSCond::OBJ_CLASS_NAME()]		= new CNtlTSEntityFactoryType_CNtlTSCond;
	m_defEntityTypeList[CNtlTSEntityFactoryType_CNtlTSAction::OBJ_CLASS_NAME()]		= new CNtlTSEntityFactoryType_CNtlTSAction;
	m_defEntityTypeList[CNtlTSEntityFactoryType_CNtlTSCont::OBJ_CLASS_NAME()]		= new CNtlTSEntityFactoryType_CNtlTSCont;
	m_defEntityTypeList[CNtlTSEntityFactoryType_CNtlTSGroup::OBJ_CLASS_NAME()]		= new CNtlTSEntityFactoryType_CNtlTSGroup;
	m_defEntityTypeList[CNtlTSEntityFactoryType_CNtlTSTrigger::OBJ_CLASS_NAME()]	= new CNtlTSEntityFactoryType_CNtlTSTrigger;
}

void CNtlTSEntityFactory::UnRegisterEntityType( void )
{
	CNtlTSEntityFactoryType* pType;
	mapdef_EntityTypeList::iterator it = m_defEntityTypeList.begin();
	for ( ; it != m_defEntityTypeList.end(); ++it )
	{
		pType = it->second;
		delete pType;
	}
	m_defEntityTypeList.clear();
}
