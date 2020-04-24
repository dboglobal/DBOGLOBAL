#include "precomp_trigger.h"
#include "DboTSActRegCTiming.h"


/**
	Register client timing
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActRegCTiming, CNtlTSAction )


CDboTSActRegCTiming::CDboTSActRegCTiming( void )
{
	m_bReg = true;

	m_uiTimingFlags = 0;
	memset( &m_sMobClick, 0xffffffff, sizeof( m_sMobClick ) );
	memset( &m_sNPCClick, 0xffffffff, sizeof( m_sNPCClick ) );
	memset( &m_sObjClick, 0xffffffff, sizeof( m_sObjClick ) );
	memset( &m_sEnterRgn, 0xffffffff, sizeof( m_sEnterRgn ) );
	memset( &m_sGetQItem, 0xffffffff, sizeof( m_sGetQItem ) );

	m_eExcuteType = eREG_C_TIMING_EXCUTE_TYPE_MINI_NARRATION;
	memset( &m_uExcuteData, 0xffffffff, sizeof( m_uExcuteData ) );
}

void CDboTSActRegCTiming::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	m_bReg = clProperty.GetValueAsInt( "rtype" ) ? true : false;

	if ( m_bReg )
	{
		if ( clProperty.IsExist( "flags" ) )
		{
			m_uiTimingFlags = clProperty.GetValueAsInt( "flags" );
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_MOB_CLICK_FLAG )
		{
			m_sMobClick.uiMobGroupIdx = clProperty.GetValueAsInt( "0_midx" );
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_NPC_CLICK_FLAG )
		{
			m_sNPCClick.uiNpcIdx = clProperty.GetValueAsInt( "1_nidx" );
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_OBJ_CLICK_FLAG )
		{
			m_sObjClick.uiWorldIdx = clProperty.GetValueAsInt( "2_widx" );
			m_sObjClick.uiObjIdx = clProperty.GetValueAsInt( "2_oidx" );
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_ENTER_RGN_FLAG )
		{
			m_sEnterRgn.uiWorldIdx  = clProperty.GetValueAsInt( "3_widx" );
			m_sEnterRgn.fMinX = (float)atof( clProperty.GetValue( "3_minx" ).c_str() );
			m_sEnterRgn.fMinZ = (float)atof( clProperty.GetValue( "3_minz" ).c_str() );
			m_sEnterRgn.fMaxX = (float)atof( clProperty.GetValue( "3_maxx" ).c_str() );
			m_sEnterRgn.fMaxZ = (float)atof( clProperty.GetValue( "3_maxz" ).c_str() );
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_GET_QITEM_FLAG )
		{
			m_sGetQItem.uiQItemIdx = clProperty.GetValueAsInt( "4_qidx" );
		}

		if ( clProperty.IsExist( "etype" ) )
		{
			m_eExcuteType = (eREG_C_TIMING_EXCUTE_TYPE)clProperty.GetValueAsInt( "etype" );
		}
		if ( m_eExcuteType == eREG_C_TIMING_EXCUTE_TYPE_MINI_NARRATION )
		{
			m_uExcuteData.sMiniNarration.uiTblIdx = clProperty.GetValueAsInt( "ntidx" );
		}
	}
}

void CDboTSActRegCTiming::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_bReg ? 1 : 0 );
	clProperty.m_defProperty["rtype"] = g_NtlTSString;

	if ( m_bReg )
	{
		sprintf_s( g_NtlTSString, "%d", m_uiTimingFlags );
		clProperty.m_defProperty["flags"] = g_NtlTSString;

		if ( m_uiTimingFlags & eREG_C_TIMING_MOB_CLICK_FLAG )
		{
			sprintf_s( g_NtlTSString, "%d", m_sMobClick.uiMobGroupIdx );
			clProperty.m_defProperty["0_midx"] = g_NtlTSString;
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_NPC_CLICK_FLAG )
		{
			sprintf_s( g_NtlTSString, "%d", m_sNPCClick.uiNpcIdx );
			clProperty.m_defProperty["1_nidx"] = g_NtlTSString;
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_OBJ_CLICK_FLAG )
		{
			sprintf_s( g_NtlTSString, "%d", m_sObjClick.uiWorldIdx );
			clProperty.m_defProperty["2_widx"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%d", m_sObjClick.uiObjIdx );
			clProperty.m_defProperty["2_oidx"] = g_NtlTSString;
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_ENTER_RGN_FLAG )
		{
			sprintf_s( g_NtlTSString, "%d", m_sEnterRgn.uiWorldIdx );
			clProperty.m_defProperty["3_widx"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_sEnterRgn.fMinX );
			clProperty.m_defProperty["3_minx"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%f", m_sEnterRgn.fMinZ );
			clProperty.m_defProperty["3_minz"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_sEnterRgn.fMaxX );
			clProperty.m_defProperty["3_maxx"] = g_NtlTSString;
			sprintf_s( g_NtlTSString, "%f", m_sEnterRgn.fMaxZ );
			clProperty.m_defProperty["3_maxz"] = g_NtlTSString;
		}
		if ( m_uiTimingFlags & eREG_C_TIMING_GET_QITEM_FLAG )
		{
			sprintf_s( g_NtlTSString, "%d", m_sGetQItem.uiQItemIdx );
			clProperty.m_defProperty["4_qidx"] = g_NtlTSString;
		}

		sprintf_s( g_NtlTSString, "%d", m_eExcuteType );
		clProperty.m_defProperty["etype"] = g_NtlTSString;

		if ( m_eExcuteType == eREG_C_TIMING_EXCUTE_TYPE_MINI_NARRATION )
		{
			sprintf_s( g_NtlTSString, "%d", m_uExcuteData.sMiniNarration.uiTblIdx );
			clProperty.m_defProperty["ntidx"] = g_NtlTSString;
		}
	}
}
