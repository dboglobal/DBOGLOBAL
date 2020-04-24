#include "precomp_trigger.h"
#include "DboTSCheckInWorld.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckInWorld, CNtlTSCond )


CDboTSCheckInWorld::CDboTSCheckInWorld( void )
: m_uiWorldIdx( 0xffffffff )
{
}

void CDboTSCheckInWorld::SetWorldTblIdx( unsigned int uiWorldTblIdx )
{
	m_uiWorldIdx = uiWorldTblIdx;
}

void CDboTSCheckInWorld::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "widx" ) )
	{
		SetWorldTblIdx( clProperty.GetValueAsInt( "widx" ) );
	}
}

void CDboTSCheckInWorld::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetWorldTblIdx() );
	clProperty.m_defProperty["widx"] = g_NtlTSString;
}
