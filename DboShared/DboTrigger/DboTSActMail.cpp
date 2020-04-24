#include "precomp_trigger.h"
#include "DboTSActMail.h"


/**
	Mail action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActMail, CNtlTSAction )


CDboTSActMail::CDboTSActMail( void )
{
	m_uiZeni = 0xffffffff;
	m_byItemCount = 0xff;
	m_uiItemTblidx = 0xffffffff;
	m_taId = NTL_TS_TA_ID_INVALID;
	m_dtIdx = 0xffffffff;
	m_stIdx = 0xffffffff;
}

void CDboTSActMail::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "dtidx" ) )
	{
		m_dtIdx = clProperty.GetValueAsInt( "dtidx" );
	}
	if ( clProperty.IsExist( "itidx" ) )
	{
		m_uiItemTblidx = clProperty.GetValueAsInt( "itidx" );
	}
	if ( clProperty.IsExist( "stidx" ) )
	{
		m_stIdx = clProperty.GetValueAsInt( "stidx" );
	}
	if ( clProperty.IsExist( "icnt" ) )
	{
		m_byItemCount = (BYTE)clProperty.GetValueAsInt( "icnt" );
	}
	if ( clProperty.IsExist( "taid" ) )
	{
		m_taId = (NTL_TS_TA_ID)clProperty.GetValueAsInt( "taid" );
	}
	if ( clProperty.IsExist( "zenny" ) )
	{
		m_uiZeni = clProperty.GetValueAsInt( "zenny" );
	}
}

void CDboTSActMail::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_dtIdx );
	clProperty.m_defProperty["dtidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiItemTblidx );
	clProperty.m_defProperty["itidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_stIdx );
	clProperty.m_defProperty["stidx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_byItemCount );
	clProperty.m_defProperty["icnt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_taId );
	clProperty.m_defProperty["taid"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiZeni );
	clProperty.m_defProperty["zenny"] = g_NtlTSString;
}
