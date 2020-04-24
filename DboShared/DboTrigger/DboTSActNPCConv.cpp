#include "precomp_trigger.h"
#include "DboTSActNPCConv.h"


/**
	NPC converation action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActNPCConv, CNtlTSAction )


CDboTSActNPCConv::CDboTSActNPCConv( void )
: m_eNPCConvType( eNPCCONV_TYPE_DIALOGBOX ),
  m_uiNPCIdx( 0xffffffff ),
  m_uiNPCConv( 0xffffffff )
{
}

void CDboTSActNPCConv::SetNPCConvType( eNPCCONV_TYPE eNPCConvType )
{
	m_eNPCConvType = eNPCConvType;
}

void CDboTSActNPCConv::SetNPCConv( unsigned int uiNPCConv )
{
	m_uiNPCConv = uiNPCConv;
}

void CDboTSActNPCConv::SetNPCIdx( unsigned int uiNPCIdx )
{
	m_uiNPCIdx = uiNPCIdx;
}

void CDboTSActNPCConv::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetNPCIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "ctype" ) )
	{
		SetNPCConvType( (eNPCCONV_TYPE) clProperty.GetValueAsInt( "ctype" ) );
	}

	if ( clProperty.IsExist( "conv" ) )
	{
		SetNPCConv( clProperty.GetValueAsInt( "conv" ) );
	}
}

void CDboTSActNPCConv::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetNPCIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNPCConvType() );
	clProperty.m_defProperty["ctype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNPCConv() );
	clProperty.m_defProperty["conv"] = g_NtlTSString;
}
