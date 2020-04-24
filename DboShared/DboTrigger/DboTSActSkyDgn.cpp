#include "precomp_trigger.h"
#include "DboTSActSkyDgn.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActSkyDgn, CNtlTSAction )


CDboTSActSkyDgn::CDboTSActSkyDgn( void )
: m_eSkyDungeonType( ePARTY_DUNGEON_TYPE_ENTER )
, m_uiSkyDungeonTblIdx( 0xffffffff )
{
}

void CDboTSActSkyDgn::SetSkyDungeonType( ePARTY_DUNGEON_TYPE eDungeonType )
{
	m_eSkyDungeonType = eDungeonType;
}

void CDboTSActSkyDgn::SetSkyDungeonTblIdx( unsigned int uiTblIdx )
{
	m_uiSkyDungeonTblIdx = uiTblIdx;
}

void CDboTSActSkyDgn::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetSkyDungeonType( (ePARTY_DUNGEON_TYPE)atoi( clProperty.GetValue( "type" ).c_str() ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetSkyDungeonTblIdx( atoi( clProperty.GetValue( "idx" ).c_str() ) );
	}

}

void CDboTSActSkyDgn::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSkyDungeonType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSkyDungeonTblIdx() );
	clProperty.m_defProperty["nidx"] = g_NtlTSString;
}
