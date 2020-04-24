#include "precomp_trigger.h"
#include "DboTSActPIDgn.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActPIDgn, CNtlTSAction )


CDboTSActPIDgn::CDboTSActPIDgn( void )
: m_eDungeonType( ePARTY_DUNGEON_TYPE_ENTER )
, m_uiNormalDungeonTblIdx( 0xffffffff )
, m_uiHardDungeonTblIdx( 0xffffffff )
, m_uiRDungeonTblIdx( 0xffffffff )
{
}

void CDboTSActPIDgn::SetDungeonType( ePARTY_DUNGEON_TYPE eDungeonType )
{
	m_eDungeonType = eDungeonType;
}

void CDboTSActPIDgn::SetNormalDungeonTblIdx( unsigned int uiTblIdx )
{
	m_uiNormalDungeonTblIdx = uiTblIdx;
}

void CDboTSActPIDgn::SetHardDungeonTblIdx( unsigned int uiTblIdx )
{
	m_uiHardDungeonTblIdx = uiTblIdx;
}

void CDboTSActPIDgn::SetRDungeonTblIdx( unsigned int uiTblIdx )
{
	m_uiRDungeonTblIdx = uiTblIdx;
}

void CDboTSActPIDgn::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetDungeonType( (ePARTY_DUNGEON_TYPE)atoi( clProperty.GetValue( "type" ).c_str() ) );
	}

	if ( clProperty.IsExist( "nidx" ) )
	{
		SetNormalDungeonTblIdx( atoi( clProperty.GetValue( "nidx" ).c_str() ) );
	}

	if ( clProperty.IsExist( "hidx" ) )
	{
		SetHardDungeonTblIdx( atoi( clProperty.GetValue( "hidx" ).c_str() ) );
	}

	if ( clProperty.IsExist( "ridx" ) )
	{
		SetRDungeonTblIdx( atoi( clProperty.GetValue( "ridx" ).c_str() ) );
	}
}

void CDboTSActPIDgn::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetDungeonType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNormalDungeonTblIdx() );
	clProperty.m_defProperty["nidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetHardDungeonTblIdx() );
	clProperty.m_defProperty["hidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetRDungeonTblIdx() );
	clProperty.m_defProperty["ridx"] = g_NtlTSString;
}
