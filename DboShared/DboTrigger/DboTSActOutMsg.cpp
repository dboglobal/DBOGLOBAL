#include "precomp_trigger.h"
#include "DboTSActOutMsg.h"


/**
	Out msg action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActOutMsg, CNtlTSAction )


CDboTSActOutMsg::CDboTSActOutMsg( void )
: m_uiIdx( 0xffffffff ),
  m_TaIdx( NTL_TS_TA_ID_INVALID )
{
}

void CDboTSActOutMsg::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		m_uiIdx = clProperty.GetValueAsInt( "idx" );
	}
	if ( clProperty.IsExist( "taid" ) )
	{
		m_TaIdx = (NTL_TS_TA_ID)clProperty.GetValueAsInt( "taid" );
	}
}

void CDboTSActOutMsg::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_uiIdx );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_TaIdx );
	clProperty.m_defProperty["taid"] = g_NtlTSString;
}
