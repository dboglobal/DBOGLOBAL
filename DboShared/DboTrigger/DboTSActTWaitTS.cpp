#include "precomp_trigger.h"
#include "DboTSActTWaitTS.h"


/**
	Time wait TS action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActTWaitTS, CNtlTSAction )


CDboTSActTWaitTS::CDboTSActTWaitTS( void )
: m_uiWTime( 0xffffffff )
{
}

void CDboTSActTWaitTS::SetWaitTime( unsigned int uiWTime )
{
	m_uiWTime = uiWTime;
}

void CDboTSActTWaitTS::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "time" ) )
	{
		SetWaitTime( clProperty.GetValueAsInt( "time" ) );
	}
}

void CDboTSActTWaitTS::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetWaitTime() );
	clProperty.m_defProperty["time"] = g_NtlTSString;
}
