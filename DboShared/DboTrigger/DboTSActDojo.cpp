#include "precomp_trigger.h"
#include "DboTSActDojo.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActDojo, CNtlTSAction )


CDboTSActDojo::CDboTSActDojo( void )
: m_eDungeonType( eDOJO_DUNGEON_TYPE_ENTER )
, m_uiDojoTblIdx( 0xffffffff )
{
}

void CDboTSActDojo::SetDungeonType( eDOJO_DUNGEON_TYPE eDungeonType )
{
	m_eDungeonType = eDungeonType;
}

void CDboTSActDojo::SetDojoTblIdx( unsigned int uiTblIdx )
{
	m_uiDojoTblIdx = uiTblIdx;
}

void CDboTSActDojo::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetDungeonType( (eDOJO_DUNGEON_TYPE)atoi( clProperty.GetValue( "type" ).c_str() ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetDojoTblIdx( atoi( clProperty.GetValue( "idx" ).c_str() ) );
	}
}

void CDboTSActDojo::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetDungeonType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDojoTblIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;
}
