#include "precomp_trigger.h"
#include "DboTSClickMob.h"


/**
	Click Mob event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSClickMob, CNtlTSEvent )


CDboTSClickMob::CDboTSClickMob( void )
: m_uiMobGroupIdx( 0xffffffff )
{
}

void CDboTSClickMob::SetMobGroupIdx( unsigned int uiMobGroupIdx )
{
	m_uiMobGroupIdx = uiMobGroupIdx;
}

void CDboTSClickMob::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "mgidx" ) )
	{
		SetMobGroupIdx( clProperty.GetValueAsInt( "mgidx" ) );
	}
}

void CDboTSClickMob::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetMobGroupIdx() );
	clProperty.m_defProperty["mgidx"] = g_NtlTSString;
}
