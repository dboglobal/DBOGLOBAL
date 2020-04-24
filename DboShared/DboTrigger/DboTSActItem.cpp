#include "precomp_trigger.h"
#include "DboTSActItem.h"


/**
	Create quest item action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActItem, CNtlTSAction )


CDboTSActItem::CDboTSActItem( void )
: m_eItemType( eTSITEM_TYPE_CREATE )
{
	for ( int i = 0; i < eMAX_TS_ITEM_COUNT; ++i )
	{
		m_stItemInfo[i].eItemSlotType = eTSITEM_SLOT_TYPE_BAG;
		m_stItemInfo[i].uiItemIdx = 0xffffffff;
	}
}

void CDboTSActItem::SetItemType( eTSITEM_TYPE eItemType )
{
	m_eItemType = eItemType;
}

void CDboTSActItem::SetItemInfo( int nIdx, const stTS_ITEM_INFO& stItemInfo )
{
	memcpy( &m_stItemInfo[nIdx], &stItemInfo, sizeof(stTS_ITEM_INFO) );
}

void CDboTSActItem::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetItemType( (eTSITEM_TYPE)clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "iidx0" ) )
	{
		m_stItemInfo[0].eItemSlotType = (eTSITEM_SLOT_TYPE)clProperty.GetValueAsInt( "stype0" );
		m_stItemInfo[0].uiItemIdx = clProperty.GetValueAsInt( "iidx0" );
	}
	if ( clProperty.IsExist( "iidx1" ) )
	{
		m_stItemInfo[1].eItemSlotType = (eTSITEM_SLOT_TYPE)clProperty.GetValueAsInt( "stype1" );
		m_stItemInfo[1].uiItemIdx = clProperty.GetValueAsInt( "iidx1" );
	}
	if ( clProperty.IsExist( "iidx2" ) )
	{
		m_stItemInfo[2].eItemSlotType = (eTSITEM_SLOT_TYPE)clProperty.GetValueAsInt( "stype2" );
		m_stItemInfo[2].uiItemIdx = clProperty.GetValueAsInt( "iidx2" );
	}
}

void CDboTSActItem::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetItemType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	if ( 0xffffffff != m_stItemInfo[0].uiItemIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_stItemInfo[0].eItemSlotType );
		clProperty.m_defProperty["stype0"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_stItemInfo[0].uiItemIdx );
		clProperty.m_defProperty["iidx0"] = g_NtlTSString;
	}
	if ( 0xffffffff != m_stItemInfo[1].uiItemIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_stItemInfo[1].eItemSlotType );
		clProperty.m_defProperty["stype1"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_stItemInfo[1].uiItemIdx );
		clProperty.m_defProperty["iidx1"] = g_NtlTSString;
	}
	if ( 0xffffffff != m_stItemInfo[2].uiItemIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_stItemInfo[2].eItemSlotType );
		clProperty.m_defProperty["stype2"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_stItemInfo[2].uiItemIdx );
		clProperty.m_defProperty["iidx2"] = g_NtlTSString;
	}
}
