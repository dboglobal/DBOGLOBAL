#include "precomp_trigger.h"
#include "DboTSActObjConv.h"


/**
	NPC converation action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActObjConv, CNtlTSAction )


CDboTSActObjConv::CDboTSActObjConv( void )
: m_eObjConvType( eOBJCONV_TYPE_DIALOGBOX ),
  m_uiWorldTblIdx( 0xffffffff ),
  m_uiObjIdx( 0xffffffff ),
  m_uiObjConv( 0xffffffff )
{
}

void CDboTSActObjConv::SetWorldTblIdx( unsigned int uiWorldTblIdx )
{
	m_uiWorldTblIdx = uiWorldTblIdx;
}

void CDboTSActObjConv::SetObjIdx( unsigned int uiObjIdx )
{
	m_uiObjIdx = uiObjIdx;
}

void CDboTSActObjConv::SetObjConvType( eOBJCONV_TYPE eObjConvType )
{
	m_eObjConvType = eObjConvType;
}

void CDboTSActObjConv::SetObjConv( unsigned int uiObjConv )
{
	m_uiObjConv = uiObjConv;
}

void CDboTSActObjConv::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "widx" ) )
	{
		SetWorldTblIdx( clProperty.GetValueAsInt( "widx" ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetObjIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "ctype" ) )
	{
		SetObjConvType( (eOBJCONV_TYPE) clProperty.GetValueAsInt( "ctype" ) );
	}

	if ( clProperty.IsExist( "conv" ) )
	{
		SetObjConv( clProperty.GetValueAsInt( "conv" ) );
	}
}

void CDboTSActObjConv::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetWorldTblIdx() );
	clProperty.m_defProperty["widx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjConvType() );
	clProperty.m_defProperty["ctype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetObjConv() );
	clProperty.m_defProperty["conv"] = g_NtlTSString;
}
