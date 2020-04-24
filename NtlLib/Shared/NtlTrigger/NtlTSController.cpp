#include "precomp_trigger.h"
#include "NtlTSController.h"
#include "NtlTSRecv.h"
#include "NtlTSTrigger.h"


/**
	Controller
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSController, CNtlTSControlObject )


CNtlTSController::CNtlTSController( void )
: m_pTrigger( 0 )
{
}

CNtlTSController::~CNtlTSController( void )
{
}

CNtlTSTrigger* CNtlTSController::SetTrigger( CNtlTSTrigger* pTrigger )
{
	CNtlTSTrigger* pOld = m_pTrigger;
	m_pTrigger = pTrigger;
	return pOld;
}

NTL_TSRESULT CNtlTSController::SearchAll( CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	if ( 0 != m_pTrigger )
	{
		tsResult |= m_pTrigger->SearchAll( pTSRecv, pParam );
	}
	else
	{
		tsResult |= NTL_TSRESULT_TYPE_ERROR;
	}

	return tsResult;
}

NTL_TSRESULT CNtlTSController::SearchTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	if ( 0 != m_pTrigger )
	{
		tsResult |= m_pTrigger->SearchTarget( tgID, tcID, pTSRecv, pParam );
	}
	else
	{
		tsResult |= NTL_TSRESULT_TYPE_ERROR;
	}

	return tsResult;
}

NTL_TSRESULT CNtlTSController::RunTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	if ( 0 != m_pTrigger )
	{
		tsResult |= m_pTrigger->RunTarget( tgID, tcID, pTSRecv, pParam );
	}
	else
	{
		tsResult |= NTL_TSRESULT_TYPE_ERROR;
	}

	return tsResult;
}
