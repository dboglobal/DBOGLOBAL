#include "precomp_trigger.h"
#include "DboTSItemGet.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSItemGet, CNtlTSEvent )


CDboTSItemGet::CDboTSItemGet( void )
: m_uiItemIdx( 0xffffffff )
{
}

void CDboTSItemGet::SetItemIdx( unsigned int uiItemIdx )
{
	m_uiItemIdx = uiItemIdx;
}

void CDboTSItemGet::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetItemIdx( clProperty.GetValueAsInt( "idx" ) );
	}
}

void CDboTSItemGet::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetItemIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;
}
