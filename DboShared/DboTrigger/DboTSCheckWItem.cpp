#include "precomp_trigger.h"
#include "DboTSCheckWItem.h"


/**
	Check item condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckWItem, CNtlTSCond )


CDboTSCheckWItem::CDboTSCheckWItem( void )
: m_uiItemIdx( 0xffffffff )
{
}

void CDboTSCheckWItem::SetItemIdx( unsigned int uiItemIdx )
{
	m_uiItemIdx = uiItemIdx;
}

void CDboTSCheckWItem::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "iidx" ) )
	{
		SetItemIdx( clProperty.GetValueAsInt( "iidx" ) );
	}
}

void CDboTSCheckWItem::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetItemIdx() );
	clProperty.m_defProperty["iidx"] = g_NtlTSString;
}
