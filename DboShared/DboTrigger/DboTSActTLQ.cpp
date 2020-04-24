#include "precomp_trigger.h"
#include "DboTSActTLQ.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActTLQ, CNtlTSAction )


CDboTSActTLQ::CDboTSActTLQ( void )
: m_eDungeonType( eTLQ_DUNGEON_TYPE_ENTER )
, m_uiDungeonTblIdx( 0xffffffff )
{
}

void CDboTSActTLQ::SetDungeonType( eTLQ_DUNGEON_TYPE eDungeonType )
{
	m_eDungeonType = eDungeonType;
}

void CDboTSActTLQ::SetDungeonTblIdx( unsigned int uiTblIdx )
{
	m_uiDungeonTblIdx = uiTblIdx;
}

void CDboTSActTLQ::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetDungeonType( (eTLQ_DUNGEON_TYPE)atoi( clProperty.GetValue( "type" ).c_str() ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetDungeonTblIdx( atoi( clProperty.GetValue( "idx" ).c_str() ) );
	}
}

void CDboTSActTLQ::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetDungeonType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDungeonTblIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;
}
