#include "precomp_trigger.h"
#include "DboTSActHint.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActHint, CNtlTSAction )


CDboTSActHint::CDboTSActHint( void )
: m_uiHintType( 0 ),
  m_uiTableIdx( 0xffffffff )
{
}

void CDboTSActHint::SetHintType( unsigned int uiHintType )
{
	m_uiHintType = uiHintType;
}

void CDboTSActHint::SetTableIndex( unsigned int uiTblIdx )
{
	m_uiTableIdx = uiTblIdx;
}

void CDboTSActHint::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetHintType( clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetTableIndex( clProperty.GetValueAsInt( "idx" ) );
	}
}

void CDboTSActHint::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetHintType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTableIndex() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;
}
