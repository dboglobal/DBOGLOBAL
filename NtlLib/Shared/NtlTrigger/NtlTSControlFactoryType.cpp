#include "precomp_trigger.h"
#include "NtlTSControlFactoryType.h"
#include "NtlTSController.h"
#include "NtlTSEvtMapper.h"


/** 
	Control 객체를 Runtime 시에 생성하기 위한 control type class 들
*/


//////////////////////////////////////////////////////////////////////////


CNtlTSControlFactoryType_CNtlTSController::CNtlTSControlFactoryType_CNtlTSController( void )
{
}

CNtlTSControlObject* CNtlTSControlFactoryType_CNtlTSController::CreateObj( void )
{
	return new CNtlTSController;
}

void CNtlTSControlFactoryType_CNtlTSController::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CNtlTSControlFactoryType_CNtlTSController::GetKeyword( void ) const
{
	return CNtlTSController::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CNtlTSControlFactoryType_CNtlTSEvtMapper::CNtlTSControlFactoryType_CNtlTSEvtMapper( void )
{
}

CNtlTSControlObject* CNtlTSControlFactoryType_CNtlTSEvtMapper::CreateObj( void )
{
	return new CNtlTSEvtMapper;
}

void CNtlTSControlFactoryType_CNtlTSEvtMapper::DeleteObj( CNtlTSControlObject*& pObj )
{
	if ( pObj )
	{
		delete pObj;
		pObj = 0;
	}
}

const char* CNtlTSControlFactoryType_CNtlTSEvtMapper::GetKeyword( void ) const
{
	return CNtlTSEvtMapper::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////
