#include "precomp_trigger.h"
#include "DboTSCheckItem.h"


/**
	Check item condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckItem, CNtlTSCond )


CDboTSCheckItem::CDboTSCheckItem( void )
: m_uiItemIdx( 0xffffffff ),
  m_nItemCnt( 0 )
{
}

void CDboTSCheckItem::SetItemIdx( unsigned int uiItemIdx )
{
	m_uiItemIdx = uiItemIdx;
}

void CDboTSCheckItem::SetItemCnt( int nItemCnt )
{
	m_nItemCnt = nItemCnt;
}

void CDboTSCheckItem::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "iidx" ) )
	{
		SetItemIdx( clProperty.GetValueAsInt( "iidx" ) );
	}
	if ( clProperty.IsExist( "icnt" ) )
	{
		SetItemCnt( clProperty.GetValueAsInt( "icnt" ) );
	}
}

void CDboTSCheckItem::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetItemIdx() );
	clProperty.m_defProperty["iidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetItemCnt() );
	clProperty.m_defProperty["icnt"] = g_NtlTSString;
}
