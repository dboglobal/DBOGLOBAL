#include "precomp_trigger.h"
#include "DboTSActETimerE.h"


/**
	Timer action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActETimerE, CNtlTSAction )


CDboTSActETimerE::CDboTSActETimerE( void )
: m_tcId( NTL_TS_TC_ID_INVALID ),
  m_taId( NTL_TS_TA_ID_INVALID )
{
}

void CDboTSActETimerE::SetTCId( NTL_TS_TC_ID tcId )
{
	m_tcId = tcId;
}

void CDboTSActETimerE::SetTAId( NTL_TS_TA_ID taId )
{
	m_taId = taId;
}

void CDboTSActETimerE::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "tceid" ) )
	{
		SetTCId( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "tceid" ) );
	}

	if ( clProperty.IsExist( "taeid" ) )
	{
		SetTAId( (NTL_TS_TA_ID)clProperty.GetValueAsInt( "taeid" ) );
	}
}

void CDboTSActETimerE::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetTCId() );
	clProperty.m_defProperty["tceid"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTAId() );
	clProperty.m_defProperty["taeid"] = g_NtlTSString;
}
