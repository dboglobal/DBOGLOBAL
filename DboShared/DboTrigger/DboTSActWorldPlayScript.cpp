#include "precomp_trigger.h"
#include "DboTSActWorldPlayScript.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActWorldPlayScript, CNtlTSAction )


CDboTSActWorldPlayScript::CDboTSActWorldPlayScript( void )
: m_bStart( true ),
  m_bUserOutFailed( false ),
  m_uiScriptID( 0xffffffff )
{
}

void CDboTSActWorldPlayScript::SetStart( bool bStart )
{
	m_bStart = bStart;
}

void CDboTSActWorldPlayScript::SetUserOutFailed( bool bUserOutFailed )
{
	m_bUserOutFailed = bUserOutFailed;
}

void CDboTSActWorldPlayScript::SetScriptID( unsigned int uiScriptID )
{
	m_uiScriptID = uiScriptID;
}

void CDboTSActWorldPlayScript::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "start" ) )
	{
		SetStart( clProperty.GetValueAsInt( "start" ) ? true : false );
	}

	if ( clProperty.IsExist( "uof" ) )
	{
		SetUserOutFailed( clProperty.GetValueAsInt( "uof" ) ? true : false );
	}

	if ( clProperty.IsExist( "sid" ) )
	{
		SetScriptID( clProperty.GetValueAsInt( "sid" ) );
	}
}

void CDboTSActWorldPlayScript::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", IsStart() ? 1 : 0 );
	clProperty.m_defProperty["start"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", IsUserOutFailed() ? 1 : 0 );
	clProperty.m_defProperty["uof"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetScriptID() );
	clProperty.m_defProperty["sid"] = g_NtlTSString;
}
