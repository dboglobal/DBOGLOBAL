#include "precomp_trigger.h"
#include "DboTSActQItem.h"


/**
	Create quest item action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActQItem, CNtlTSAction )


CDboTSActQItem::CDboTSActQItem( void )
: m_eQItemType( eQITEM_TYPE_CREATE )
{
	for ( int i = 0; i < eMAX_TS_QITEM_COUNT; ++i )
	{
		m_stQItemInfo[i].uiQItemIdx = 0xffffffff;
		m_stQItemInfo[i].nQItemCnt = 0;
		m_stQItemInfo[i].fProbability = 1.f;
	}
}

void CDboTSActQItem::SetQItemType( eQITEM_TYPE eQItemType )
{
	m_eQItemType = eQItemType;
}

void CDboTSActQItem::SetQItemInfo( int nIdx, const stTS_QITEM_INFO& stQItemInfo )
{
	memcpy( &m_stQItemInfo[nIdx], &stQItemInfo, sizeof(stTS_QITEM_INFO) );
}

void CDboTSActQItem::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "type" ) )
	{
		SetQItemType( (eQITEM_TYPE)clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "iidx0" ) )
	{
		m_stQItemInfo[0].uiQItemIdx = clProperty.GetValueAsInt( "iidx0" );
		m_stQItemInfo[0].nQItemCnt = clProperty.GetValueAsInt( "icnt0" );
		m_stQItemInfo[0].fProbability = clProperty.IsExist( "iprob0" ) ? (float)atof( clProperty.GetValue( "iprob0" ).c_str() ) : 1.f;
	}
	if ( clProperty.IsExist( "iidx1" ) )
	{
		m_stQItemInfo[1].uiQItemIdx = clProperty.GetValueAsInt( "iidx1" );
		m_stQItemInfo[1].nQItemCnt = clProperty.GetValueAsInt( "icnt1" );
		m_stQItemInfo[1].fProbability = clProperty.IsExist( "iprob1" ) ? (float)atof( clProperty.GetValue( "iprob1" ).c_str() ) : 1.f;
	}
	if ( clProperty.IsExist( "iidx2" ) )
	{
		m_stQItemInfo[2].uiQItemIdx = clProperty.GetValueAsInt( "iidx2" );
		m_stQItemInfo[2].nQItemCnt = clProperty.GetValueAsInt( "icnt2" );
		m_stQItemInfo[2].fProbability = clProperty.IsExist( "iprob2" ) ? (float)atof( clProperty.GetValue( "iprob2" ).c_str() ) : 1.f;
	}
}

void CDboTSActQItem::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetQItemType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	if ( 0xffffffff != m_stQItemInfo[0].uiQItemIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_stQItemInfo[0].uiQItemIdx );
		clProperty.m_defProperty["iidx0"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_stQItemInfo[0].nQItemCnt );
		clProperty.m_defProperty["icnt0"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_stQItemInfo[0].fProbability );
		clProperty.m_defProperty["iprob0"] = g_NtlTSString;
	}
	if ( 0xffffffff != m_stQItemInfo[1].uiQItemIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_stQItemInfo[1].uiQItemIdx );
		clProperty.m_defProperty["iidx1"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_stQItemInfo[1].nQItemCnt );
		clProperty.m_defProperty["icnt1"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_stQItemInfo[1].fProbability );
		clProperty.m_defProperty["iprob1"] = g_NtlTSString;
	}
	if ( 0xffffffff != m_stQItemInfo[2].uiQItemIdx )
	{
		sprintf_s( g_NtlTSString, "%d", m_stQItemInfo[2].uiQItemIdx );
		clProperty.m_defProperty["iidx2"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", m_stQItemInfo[2].nQItemCnt );
		clProperty.m_defProperty["icnt2"] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%f", m_stQItemInfo[2].fProbability );
		clProperty.m_defProperty["iprob2"] = g_NtlTSString;
	}
}
