#include "precomp_trigger.h"
#include "DboTSCheckSToCDelivery.h"


/**
	Check SToC delivery condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckSToCDelivery, CNtlTSCond )


CDboTSCheckSToCDelivery::CDboTSCheckSToCDelivery( void )
{
	for ( int i = 0; i < MAX_CHECK_STOC_DELIVERTY_ITEM; ++i )
	{
		m_uiItemIdx[i] = 0xffffffff;
		m_nItemCnt[i] = 0;
	}
}

void CDboTSCheckSToCDelivery::SetItemIdx( int nIdx, unsigned int uiItemIdx )
{
	if ( nIdx < 0 || nIdx >= MAX_CHECK_STOC_DELIVERTY_ITEM )
	{
		CNtlTSLog::Log( "CDboTSCheckSToCDelivery::SetItemIdx parameter wrong [%d]", nIdx );
		return;
	}

	m_uiItemIdx[nIdx] = uiItemIdx;
}

void CDboTSCheckSToCDelivery::SetItemCnt( int nIdx, int nItemCnt )
{
	if ( nIdx < 0 || nIdx >= MAX_CHECK_STOC_DELIVERTY_ITEM )
	{
		CNtlTSLog::Log( "CDboTSCheckSToCDelivery::SetItemCnt parameter wrong [%d]", nIdx );
		return;
	}

	m_nItemCnt[nIdx] = nItemCnt;
}

void CDboTSCheckSToCDelivery::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "iidx0" ) )
	{
		m_uiItemIdx[0] = clProperty.GetValueAsInt( "iidx0" );
	}
	if ( clProperty.IsExist( "icnt0" ) )
	{
		m_nItemCnt[0] = clProperty.GetValueAsInt( "icnt0" );
	}

	if ( clProperty.IsExist( "iidx1" ) )
	{
		m_uiItemIdx[1] = clProperty.GetValueAsInt( "iidx1" );
	}
	if ( clProperty.IsExist( "icnt1" ) )
	{
		m_nItemCnt[1] = clProperty.GetValueAsInt( "icnt1" );
	}

	if ( clProperty.IsExist( "iidx2" ) )
	{
		m_uiItemIdx[2] = clProperty.GetValueAsInt( "iidx2" );
	}
	if ( clProperty.IsExist( "icnt2" ) )
	{
		m_nItemCnt[2] = clProperty.GetValueAsInt( "icnt2" );
	}
}

void CDboTSCheckSToCDelivery::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_uiItemIdx[0] );
	clProperty.m_defProperty["iidx0"] = g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", m_nItemCnt[0] );
	clProperty.m_defProperty["icnt0"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiItemIdx[1] );
	clProperty.m_defProperty["iidx1"] = g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", m_nItemCnt[1] );
	clProperty.m_defProperty["icnt1"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiItemIdx[2] );
	clProperty.m_defProperty["iidx2"] = g_NtlTSString;
	sprintf_s( g_NtlTSString, "%d", m_nItemCnt[2] );
	clProperty.m_defProperty["icnt2"] = g_NtlTSString;
}