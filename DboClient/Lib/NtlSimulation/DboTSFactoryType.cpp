#include "precomp_ntlsimulation.h"
#include "DboTSFactoryType.h"
#include "DboTSCQCtrl.h"
#include "DboTSCQRecv.h"
#include "DboTSCQAgency.h"
#include "DboTSCTCtrl.h"
#include "DboTSCTRecv.h"
#include "DboTSCTAgency.h"
#include "DboTSCObjAgency.h"
#include "DboTSCMain.h"
#include "NtlDebug.h"



/** 
	UI factory type
*/


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSCQCtrl::CDboTSControlFactoryType_CDboTSCQCtrl( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSCQCtrl::CreateObj( void )
{
	return NTL_NEW CDboTSCQCtrl;
}

void CDboTSControlFactoryType_CDboTSCQCtrl::DeleteObj( CNtlTSControlObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSControlFactoryType_CDboTSCQCtrl::GetKeyword( void ) const
{
	return CDboTSCQCtrl::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSControlFactoryType_CDboTSCTCtrl::CDboTSControlFactoryType_CDboTSCTCtrl( void )
{
}

CNtlTSControlObject* CDboTSControlFactoryType_CDboTSCTCtrl::CreateObj( void )
{
	return NTL_NEW CDboTSCTCtrl;
}

void CDboTSControlFactoryType_CDboTSCTCtrl::DeleteObj( CNtlTSControlObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSControlFactoryType_CDboTSCTCtrl::GetKeyword( void ) const
{
	return CDboTSCTCtrl::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSCQRecv::CDboTSUIFactoryType_CDboTSCQRecv( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSCQRecv::CreateObj( void )
{
	return NTL_NEW CDboTSCQRecv;
}

void CDboTSUIFactoryType_CDboTSCQRecv::DeleteObj( CNtlTSUIObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSUIFactoryType_CDboTSCQRecv::GetKeyword( void ) const
{
	return CDboTSCQRecv::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSCQAgency::CDboTSUIFactoryType_CDboTSCQAgency( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSCQAgency::CreateObj( void )
{
	return NTL_NEW CDboTSCQAgency;
}

void CDboTSUIFactoryType_CDboTSCQAgency::DeleteObj( CNtlTSUIObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSUIFactoryType_CDboTSCQAgency::GetKeyword( void ) const
{
	return CDboTSCQAgency::RTTI.GetClassName();
}



//////////////////////////////////////////////////////////////////////////
CDboTSUIFactoryType_CDboTSCTRecv::CDboTSUIFactoryType_CDboTSCTRecv( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSCTRecv::CreateObj( void )
{
	return NTL_NEW CDboTSCTRecv;
}

void CDboTSUIFactoryType_CDboTSCTRecv::DeleteObj( CNtlTSUIObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSUIFactoryType_CDboTSCTRecv::GetKeyword( void ) const
{
	return CDboTSCTRecv::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSCTAgency::CDboTSUIFactoryType_CDboTSCTAgency( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSCTAgency::CreateObj( void )
{
	return NTL_NEW CDboTSCTAgency;
}

void CDboTSUIFactoryType_CDboTSCTAgency::DeleteObj( CNtlTSUIObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSUIFactoryType_CDboTSCTAgency::GetKeyword( void ) const
{
	return CDboTSCTAgency::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSCObjAgency::CDboTSUIFactoryType_CDboTSCObjAgency( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSCObjAgency::CreateObj( void )
{
	return NTL_NEW CDboTSCObjAgency;
}

void CDboTSUIFactoryType_CDboTSCObjAgency::DeleteObj( CNtlTSUIObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSUIFactoryType_CDboTSCObjAgency::GetKeyword( void ) const
{
	return CDboTSCObjAgency::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////


CDboTSUIFactoryType_CDboTSCMain::CDboTSUIFactoryType_CDboTSCMain( void )
{
}

CNtlTSUIObject* CDboTSUIFactoryType_CDboTSCMain::CreateObj( void )
{
	return NTL_NEW CDboTSCMain;
}

void CDboTSUIFactoryType_CDboTSCMain::DeleteObj( CNtlTSUIObject*& pObj )
{
	NTL_DELETE( pObj );
}

const char* CDboTSUIFactoryType_CDboTSCMain::GetKeyword( void ) const
{
	return CDboTSCMain::RTTI.GetClassName();
}


//////////////////////////////////////////////////////////////////////////