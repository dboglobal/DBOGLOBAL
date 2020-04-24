#include "precomp_trigger.h"
#include "DboTSCheckPCRace.h"


/**
	Check pc race condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckPCRace, CNtlTSCond )


CDboTSCheckPCRace::CDboTSCheckPCRace( void )
: m_uiRaceFlags( 0 )
{
}

void CDboTSCheckPCRace::SetRaceFlags( unsigned int uiRaceFlags )
{
	m_uiRaceFlags = uiRaceFlags;
}

void CDboTSCheckPCRace::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "raceflg" ) )
	{
		SetRaceFlags( clProperty.GetValueAsInt( "raceflg" ) );
	}
}

void CDboTSCheckPCRace::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetRaceFlags() );
	clProperty.m_defProperty["raceflg"] = g_NtlTSString;
}
