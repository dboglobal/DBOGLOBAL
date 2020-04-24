#include "precomp_trigger.h"
#include "NtlTSControlFactory.h"
#include "NtlTSControlFactoryType.h"
#include "NtlTSControlObject.h"
#include "NtlTSLog.h"


/**
	Control factory
*/


CNtlTSControlFactory::~CNtlTSControlFactory( void )
{
	UnRegisterCtrlType();
}

CNtlTSControlObject* CNtlTSControlFactory::CreateObj( const std::string& strName )
{
	mapdef_CtrlTypeList::iterator it = m_defCtrlTypeList.find( strName );

	if ( it != m_defCtrlTypeList.end() )
	{
		return it->second->CreateObj();
	}
	else
	{
		CNtlTSLog::Log( "Can not find the control object type. Info[%s]. [%s]", strName.c_str(), TS_CODE_TRACE() );
	}

	return 0;
}

void CNtlTSControlFactory::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		mapdef_CtrlTypeList::iterator itType = m_defCtrlTypeList.find( pObj->GetClassName() );

		if ( itType != m_defCtrlTypeList.end() )
		{
			itType->second->DeleteObj( pObj );
			pObj = 0;
		}
		else
		{
			printf( "Don't find the control object(%s).[%s]", pObj->GetClassName(), TS_CODE_TRACE() );
		}
	}
}

void CNtlTSControlFactory::UnRegisterCtrlType( void )
{
	CNtlTSControlFactoryType_CNtlTSControlObject* pType;
	mapdef_CtrlTypeList::iterator it = m_defCtrlTypeList.begin();
	for ( ; it != m_defCtrlTypeList.end(); ++it )
	{
		pType = it->second;
		delete pType;
	}
	m_defCtrlTypeList.clear();
}
