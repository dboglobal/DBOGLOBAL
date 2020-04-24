#include "precomp_trigger.h"
#include "DboTSCheckQItem.h"


/**
	Check quest item condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckQItem, CNtlTSCond )


CDboTSCheckQItem::CDboTSCheckQItem( void )
: m_eCheckType( eQUEST_ITEM_CHECK_TYPE_MORE ),
  m_uiItemIdx( 0xffffffff ),
  m_nItemCnt( 0 )
{
	m_nItemCnt = 0;
	m_uiItemIdx = INVALID_TBLIDX;
}

void CDboTSCheckQItem::SetCheckType( eQUEST_ITEM_CHECK_TYPE eCheckType )
{
	m_eCheckType = eCheckType;
}

void CDboTSCheckQItem::SetItemIdx( unsigned int uiItemIdx )
{
	m_uiItemIdx = uiItemIdx;
}

void CDboTSCheckQItem::SetItemCnt( int nItemCnt )
{
	m_nItemCnt = nItemCnt;
}

void CDboTSCheckQItem::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "ct" ) )
	{
		SetCheckType( (eQUEST_ITEM_CHECK_TYPE)clProperty.GetValueAsInt( "ct" ) );
	}

	if ( clProperty.IsExist( "iidx" ) )
	{
		SetItemIdx( clProperty.GetValueAsInt( "iidx" ) );
	}

	if ( clProperty.IsExist( "icnt" ) )
	{
		SetItemCnt( clProperty.GetValueAsInt( "icnt" ) );
	}
}

void CDboTSCheckQItem::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetCheckType() );
	clProperty.m_defProperty["ct"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetItemIdx() );
	clProperty.m_defProperty["iidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetItemCnt() );
	clProperty.m_defProperty["icnt"] = g_NtlTSString;
}
