#include "precomp_trigger.h"
#include "DboTSActAvatarDead.h"


/**
	avatar dead action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActAvatarDead, CNtlTSAction )


CDboTSActAvatarDead::CDboTSActAvatarDead( void )
: m_ucStart( 0xff ),
  m_TaIdx( NTL_TS_TA_ID_INVALID )
{
}

void CDboTSActAvatarDead::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "start" ) )
	{
		m_ucStart = (unsigned char)clProperty.GetValueAsInt( "start" );
	}
	if ( clProperty.IsExist( "taid" ) )
	{
		m_TaIdx = (NTL_TS_TA_ID)clProperty.GetValueAsInt( "taid" );
	}
}

void CDboTSActAvatarDead::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_ucStart );
	clProperty.m_defProperty["start"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_TaIdx );
	clProperty.m_defProperty["taid"] = g_NtlTSString;
}
