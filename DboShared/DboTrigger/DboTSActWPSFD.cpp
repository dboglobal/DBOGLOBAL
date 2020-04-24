#include "precomp_trigger.h"
#include "DboTSActWPSFD.h"


/**
	WPSFD
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActWPSFD, CNtlTSAction )


CDboTSActWPSFD::CDboTSActWPSFD( void )
: m_tSidx( NTL_TS_T_ID_INVALID ),
  m_TaIdx( NTL_TS_TA_ID_INVALID )
{
}

void CDboTSActWPSFD::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "sid" ) )
	{
		m_tSidx = (NTL_TS_T_ID)clProperty.GetValueAsInt( "sid" );
	}
	if ( clProperty.IsExist( "taid" ) )
	{
		m_TaIdx = (NTL_TS_TA_ID)clProperty.GetValueAsInt( "taid" );
	}
}

void CDboTSActWPSFD::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_tSidx );
	clProperty.m_defProperty["sid"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_TaIdx );
	clProperty.m_defProperty["taid"] = g_NtlTSString;
}
