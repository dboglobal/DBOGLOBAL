#include "precomp_trigger.h"
#include "DboTSUIFactoryType.h"
#include "DboTSTRecv.h"
#include "DboTSQRecv.h"
#include "DboTSTAgency.h"
#include "DboTSQAgency.h"
#include "DboTSMain.h"


/** 
	UI 객체를 Runtime 시에 생성하기 위한 ui type class 들
*/


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSTRecv::CDboTSUIFactoryType_CDboTSTRecv( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSTRecv::CreateObj( void )
{
	return new CDboTSTRecv;
}

void CDboTSUIFactoryType_CDboTSTRecv::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSUIFactoryType_CDboTSTRecv::GetKeyword( void ) const
{
	return CDboTSTRecv::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSQRecv::CDboTSUIFactoryType_CDboTSQRecv( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSQRecv::CreateObj( void )
{
	return new CDboTSQRecv;
}

void CDboTSUIFactoryType_CDboTSQRecv::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSUIFactoryType_CDboTSQRecv::GetKeyword( void ) const
{
	return CDboTSQRecv::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSTAgency::CDboTSUIFactoryType_CDboTSTAgency( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSTAgency::CreateObj( void )
{
	return new CDboTSTAgency;
}

void CDboTSUIFactoryType_CDboTSTAgency::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSUIFactoryType_CDboTSTAgency::GetKeyword( void ) const
{
	return CDboTSTAgency::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSQAgency::CDboTSUIFactoryType_CDboTSQAgency( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSQAgency::CreateObj( void )
{
	return new CDboTSQAgency;
}

void CDboTSUIFactoryType_CDboTSQAgency::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSUIFactoryType_CDboTSQAgency::GetKeyword( void ) const
{
	return CDboTSQAgency::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSMain::CDboTSUIFactoryType_CDboTSMain( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSMain::CreateObj( void )
{
	return new CDboTSMain;
}

void CDboTSUIFactoryType_CDboTSMain::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CDboTSUIFactoryType_CDboTSMain::GetKeyword( void ) const
{
	return CDboTSMain::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////
