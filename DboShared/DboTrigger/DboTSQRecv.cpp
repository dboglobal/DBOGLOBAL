#include "precomp_trigger.h"
#include "DboTSQRecv.h"


/** 
	Quest receiver
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSQRecv, CDboTSTRecv )


NTL_TSRESULT CDboTSQRecv::Search( CNtlTSEntity* , void* )
{
	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSQRecv::Run( CNtlTSEntity* , void* )
{
	return NTL_TSRESULT_TYPE_SUCCESS;
}
