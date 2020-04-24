#include "precomp_trigger.h"
#include "NtlTSUIFactory.h"
#include "NtlTSUIFactoryType.h"
#include "NtlTSUIObject.h"
#include "NtlTSLog.h"


/**
	UI factory
*/


CNtlTSUIFactory::~CNtlTSUIFactory( void )
{
	UnRegisterUIType();
}

CNtlTSUIObject* CNtlTSUIFactory::CreateObj( const std::string& strName )
{
	mapdef_UITypeList::iterator it = m_defUITypeList.find( strName );

	if ( it != m_defUITypeList.end() )
	{
		return it->second->CreateObj();
	}
	else
	{
		printf( "Can not find the UI object type. Info[%s]. [%s]", strName.c_str(), TS_CODE_TRACE() );
	}

	return 0;
}

void CNtlTSUIFactory::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		mapdef_UITypeList::iterator itType = m_defUITypeList.find( pObj->GetClassName() );

		if ( itType != m_defUITypeList.end() )
		{
			itType->second->DeleteObj( pObj );
			pObj = 0;
		}
		else
		{
			printf( "Don't find the UI object(%s).[%s]", pObj->GetClassName(), TS_CODE_TRACE() );
		}
	}
}

void CNtlTSUIFactory::UnRegisterUIType( void )
{
	CNtlTSUIFactoryType_CNtlTSUIObject* pType;
	mapdef_UITypeList::iterator it = m_defUITypeList.begin();
	for ( ; it != m_defUITypeList.end(); ++it )
	{
		pType = it->second;
		delete pType;
	}
	m_defUITypeList.clear();
}
