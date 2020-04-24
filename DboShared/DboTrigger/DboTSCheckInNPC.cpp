#include "precomp_trigger.h"
#include "DboTSCheckInNPC.h"


/**
	Check in area
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckInNPC, CNtlTSCond )


CDboTSCheckInNPC::CDboTSCheckInNPC( void )
{
	m_uiNPCIdx = 0xffffffff;
	m_nRadius = 15;
}

void CDboTSCheckInNPC::SetNPCIndex( unsigned int uiNPCIdx )
{
	m_uiNPCIdx = uiNPCIdx;
}

void CDboTSCheckInNPC::SetRadius( int nRadius )
{
	m_nRadius = nRadius;
}

void CDboTSCheckInNPC::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetNPCIndex( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "rad" ) )
	{
		SetRadius( clProperty.GetValueAsInt( "rad" ) );
	}
}

void CDboTSCheckInNPC::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetNPCIndex() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetRadius() );
	clProperty.m_defProperty["rad"] = g_NtlTSString;
}
