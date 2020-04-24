#include "precomp_trigger.h"
#include "DboTSContUnifiedNarration.h"


/**
	Unified narration container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContUnifiedNarration, CNtlTSCont )


CDboTSContUnifiedNarration::CDboTSContUnifiedNarration( void )
{
	m_uiNarrationTblIdx = 0xffffffff;

	m_tcOkLink = NTL_TS_TC_ID_INVALID;
	m_tcCancelLink = NTL_TS_TC_ID_INVALID;
	m_tcLogInLink = NTL_TS_TC_ID_INVALID;
}

void CDboTSContUnifiedNarration::SetNarrationTblIdx( unsigned int uiNarrationTblIdx )
{
	m_uiNarrationTblIdx = uiNarrationTblIdx;
}

void CDboTSContUnifiedNarration::SetOkLink( NTL_TS_TC_ID tcId )
{
	m_tcOkLink = tcId;
}

void CDboTSContUnifiedNarration::SetCancelLink( NTL_TS_TC_ID tcId )
{
	m_tcCancelLink = tcId;
}

void CDboTSContUnifiedNarration::SetLogInLink( NTL_TS_TC_ID tcId )
{
	m_tcLogInLink = tcId;
}

void CDboTSContUnifiedNarration::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetNarrationTblIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "oklnk" ) )
	{
		SetOkLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "oklnk" ) );
	}

	if ( clProperty.IsExist( "cancellnk" ) )
	{
		SetCancelLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "cancellnk" ) );
	}

	if ( clProperty.IsExist( "lilnk" ) )
	{
		SetLogInLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "lilnk" ) );
	}
}

void CDboTSContUnifiedNarration::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetNarrationTblIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetOkLink() );
	clProperty.m_defProperty["oklnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetCancelLink() );
	clProperty.m_defProperty["cancellnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetLogInLink() );
	clProperty.m_defProperty["lilnk"] = g_NtlTSString;
}