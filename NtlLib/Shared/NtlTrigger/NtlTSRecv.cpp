#include "precomp_trigger.h"
#include "NtlTSRecv.h"


/** 
	Receiver
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSRecv, CNtlTSUIObject )


CNtlTSRecv::CNtlTSRecv( void )
{
}

CNtlTSRecv::~CNtlTSRecv( void )
{
}

NTL_TSRESULT CNtlTSRecv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	UNREFERENCED_PARAMETER( pEntity );
	UNREFERENCED_PARAMETER( pParam );

	return 0;
}

NTL_TSRESULT CNtlTSRecv::Run( CNtlTSEntity* pEntity, void* pParam )
{
	UNREFERENCED_PARAMETER( pEntity );
	UNREFERENCED_PARAMETER( pParam );

	return 0;
}
