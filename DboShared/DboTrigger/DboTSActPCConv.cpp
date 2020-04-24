#include "precomp_trigger.h"
#include "DboTSActPCConv.h"


/**
	PC conversation action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActPCConv, CNtlTSAction )


CDboTSActPCConv::CDboTSActPCConv( void )
: m_uiConvTblIdx( 0xffffffff )
{
}

void CDboTSActPCConv::SetConvTblIdx( unsigned int uiConvTblIdx )
{
	m_uiConvTblIdx = uiConvTblIdx;
}

void CDboTSActPCConv::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "tblidx" ) )
	{
		SetConvTblIdx( atoi( clProperty.GetValue( "tblidx" ).c_str() ) );
	}
}

void CDboTSActPCConv::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetConvTblIdx() );
	clProperty.m_defProperty["tblidx"] = g_NtlTSString;
}
