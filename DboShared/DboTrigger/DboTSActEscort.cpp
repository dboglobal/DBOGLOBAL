#include "precomp_trigger.h"
#include "DboTSActEscort.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActEscort, CNtlTSAction )


CDboTSActEscort::CDboTSActEscort( void )
: m_bStart( true ),
  m_eEscortType( ESCORT_TYPE_UNDER_ESCORT ),
  m_bTSShare( true ),
  m_uiNPCTblIdx( 0xffffffff )
{
}

void CDboTSActEscort::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "start" ) )
	{
        SetEscortStart( clProperty.GetValueAsInt( "start" ) ? true : false);
	}

	if ( clProperty.IsExist( "etype" ) )
	{
		SetEscortType( (eESCORT_TYPE)clProperty.GetValueAsInt( "etype" ) );
	}

	if ( clProperty.IsExist( "tsshare" ) )
	{
        SetTSShare(clProperty.GetValueAsInt( "tsshare" ) ? true : false);
	}

	if ( clProperty.IsExist( "tblidx" ) )
	{
		SetNPCTableIndex( clProperty.GetValueAsInt( "tblidx" ) );
	}
}

void CDboTSActEscort::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", IsEscortStart() ? 1 : 0 );
	clProperty.m_defProperty["start"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetEscortType() );
	clProperty.m_defProperty["etype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", IsTSShare() ? 1 : 0 );
	clProperty.m_defProperty["tsshare"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNPCTableIndex() );
	clProperty.m_defProperty["tblidx"] = g_NtlTSString;
}
