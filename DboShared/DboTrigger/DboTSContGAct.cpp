#include "precomp_trigger.h"
#include "DboTSContGAct.h"


/**
	General action container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContGAct, CNtlTSCont )


CDboTSContGAct::CDboTSContGAct( void )
: m_tcNextLinkId( NTL_TS_TC_ID_INVALID ),
  m_tcErrorLinkId( NTL_TS_TC_ID_INVALID )
{
}

bool CDboTSContGAct::AttachNextLink( NTL_TS_TC_ID tcId )
{
	if ( NTL_TS_TC_ID_INVALID != m_tcNextLinkId )
	{
		CNtlTSLog::Log( "The next link count of the general action container have only one. [%s]", TS_CODE_TRACE() );
		return false;
	}

	m_tcNextLinkId = tcId;

	return true;
}

void CDboTSContGAct::DetachNextLink( void )
{
	m_tcNextLinkId = NTL_TS_TC_ID_INVALID;
}

bool CDboTSContGAct::AttachErrorLink( NTL_TS_TC_ID tcId )
{
	if ( NTL_TS_TC_ID_INVALID != m_tcErrorLinkId )
	{
		CNtlTSLog::Log( "The error link count of the general action container have only one. [%s]", TS_CODE_TRACE() );
		return false;
	}

	m_tcErrorLinkId = tcId;

	return true;
}

void CDboTSContGAct::DetachErrorLink( void )
{
	m_tcErrorLinkId = NTL_TS_TC_ID_INVALID;
}

void CDboTSContGAct::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "nextlnk" ) )
	{
		AttachNextLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "nextlnk" ) );
	}

	if ( clProperty.IsExist( "elnk" ) )
	{
		AttachErrorLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "elnk" ) );
	}
}

void CDboTSContGAct::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetNextLinkID() );
	clProperty.m_defProperty["nextlnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetErrorLinkID() );
	clProperty.m_defProperty["elnk"] = g_NtlTSString;
}
