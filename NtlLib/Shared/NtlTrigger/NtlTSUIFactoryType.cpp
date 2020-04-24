#include "precomp_trigger.h"
#include "NtlTSUIFactoryType.h"
#include "NtlTSRecv.h"
#include "NtlTSAgency.h"
#include "NtlTSMain.h"


/** 
	UI 객체를 Runtime 시에 생성하기 위한 UI type class 들
*/


//////////////////////////////////////////////////////////////////////////


CNtlTSUIFactoryType_CNtlTSRecv::CNtlTSUIFactoryType_CNtlTSRecv( void )
{
}

CNtlTSUIObject* CNtlTSUIFactoryType_CNtlTSRecv::CreateObj( void )
{
	return new CNtlTSRecv;
}

void CNtlTSUIFactoryType_CNtlTSRecv::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CNtlTSUIFactoryType_CNtlTSRecv::GetKeyword( void ) const
{
	return CNtlTSAgency::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CNtlTSUIFactoryType_CNtlTSAgency::CNtlTSUIFactoryType_CNtlTSAgency( void )
{
}

CNtlTSUIObject* CNtlTSUIFactoryType_CNtlTSAgency::CreateObj( void )
{
	return new CNtlTSAgency;
}

void CNtlTSUIFactoryType_CNtlTSAgency::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CNtlTSUIFactoryType_CNtlTSAgency::GetKeyword( void ) const
{
	return CNtlTSAgency::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CNtlTSUIFactoryType_CNtlTSMain::CNtlTSUIFactoryType_CNtlTSMain( void )
{
}

CNtlTSUIObject* CNtlTSUIFactoryType_CNtlTSMain::CreateObj( void )
{
	return new CNtlTSMain;
}

void CNtlTSUIFactoryType_CNtlTSMain::DeleteObj( CNtlTSUIObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CNtlTSUIFactoryType_CNtlTSMain::GetKeyword( void ) const
{
	return CNtlTSAgency::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////
