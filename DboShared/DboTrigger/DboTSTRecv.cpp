#include "precomp_trigger.h"
#include "DboTSTRecv.h"


/** 
	Trigger receiver
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSTRecv, CNtlTSRecv )


NTL_TSRESULT CDboTSTRecv::Search( CNtlTSEntity* , void* )
{
	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSTRecv::Run( CNtlTSEntity* , void* )
{
	return NTL_TSRESULT_TYPE_SUCCESS;
}
