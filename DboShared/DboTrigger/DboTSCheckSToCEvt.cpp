#include "precomp_trigger.h"
#include "DboTSCheckSToCEvt.h"


/**
	Check server to client condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckSToCEvt, CNtlTSCond )


CDboTSCheckSToCEvt::CDboTSCheckSToCEvt( void )
: m_eSToCEvtDataType( eSTOC_EVT_DATA_TYPE_INVALID )
{
}

void CDboTSCheckSToCEvt::SetSToCEvtDataType( eSTOC_EVT_DATA_TYPE eSToCEvtDataType )
{
	m_eSToCEvtDataType = eSToCEvtDataType;
}

void CDboTSCheckSToCEvt::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "itype" ) )
	{
		SetSToCEvtDataType( (eSTOC_EVT_DATA_TYPE)clProperty.GetValueAsInt( "itype" ) );
	}
}

void CDboTSCheckSToCEvt::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSToCEvtDataType() );
	clProperty.m_defProperty["itype"] = g_NtlTSString;
}
