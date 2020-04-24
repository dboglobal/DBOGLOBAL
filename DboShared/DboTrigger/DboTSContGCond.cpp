#include "precomp_trigger.h"
#include "DboTSContGCond.h"


/**
	General condition container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContGCond, CNtlTSCont )


CDboTSContGCond::CDboTSContGCond( void )
: m_tcYesId( NTL_TS_TC_ID_INVALID ),
  m_tcNoId( NTL_TS_TC_ID_INVALID ),
  m_bRewardMark( false )
{
}

void CDboTSContGCond::SetRewardMark( bool bRewardMark )
{
	m_bRewardMark = bRewardMark;
}

bool CDboTSContGCond::AttachYesLinkID( NTL_TS_TC_ID tcId )
{
	if ( NTL_TS_TC_ID_INVALID != m_tcYesId )
	{
		CNtlTSLog::Log( "the yes container have only one. Info[%d]. [%s]", tcId, TS_CODE_TRACE() );
		return false;
	}

	m_tcYesId = tcId;

	return true;
}

void CDboTSContGCond::DetachYesLinkID( void )
{
	m_tcYesId = NTL_TS_TC_ID_INVALID;
}

bool CDboTSContGCond::AttachNoLinkID( NTL_TS_TC_ID tcId )
{
	if ( NTL_TS_TC_ID_INVALID != m_tcNoId )
	{
		CNtlTSLog::Log( "The no container have only one. Info[%d]. [%s]", tcId, TS_CODE_TRACE() );
		return false;
	}

	m_tcNoId = tcId;

	return true;
}

void CDboTSContGCond::DetachNoLinkID( void )
{
	m_tcNoId = NTL_TS_TC_ID_INVALID;
}

void CDboTSContGCond::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "yeslnk" ) )
	{
		AttachYesLinkID( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "yeslnk" ) );
	}

	if ( clProperty.IsExist( "nolnk" ) )
	{
		AttachNoLinkID( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "nolnk" ) );
	}

	if ( clProperty.IsExist( "rm" ) )
	{
		SetRewardMark( clProperty.GetValueAsInt( "rm" ) ? true : false );
	}
}

void CDboTSContGCond::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetYesLinkID() );
	clProperty.m_defProperty["yeslnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNoLinkID() );
	clProperty.m_defProperty["nolnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", IsRewardMark() ? 1 : 0 );
	clProperty.m_defProperty["rm"] = g_NtlTSString;
}