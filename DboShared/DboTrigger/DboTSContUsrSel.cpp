#include "precomp_trigger.h"
#include "DboTSContUsrSel.h"


/**
	User selection container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContUsrSel, CNtlTSCont )


CDboTSContUsrSel::CDboTSContUsrSel( void )
: m_eUSType( eUSER_SEL_TYPE_GENERAL ),
  m_eUSProgState( eUSER_SEL_PROGRESS_STATE_INVALID ),
  m_eTargetType( eUSER_SEL_TARGET_TYPE_INVALID ),
  m_uiTargetTblIdx( 0xffffffff ),
  m_uiDesc( 0xffffffff )
{
	m_tcCancelLink = NTL_TS_TC_ID_INVALID;
	m_tcLogInLink = NTL_TS_TC_ID_INVALID;
}

void CDboTSContUsrSel::SetUserSelType( eUSER_SEL_TYPE eUSType )
{
	m_eUSType = eUSType;
}

void CDboTSContUsrSel::SetUserSelProgState( eUSER_SEL_PROGRESS_STATE eUSProgState )
{
	m_eUSProgState = eUSProgState;
}

void CDboTSContUsrSel::SetTargetType( eUSER_SEL_TARGET_TYPE eTargetType )
{
	m_eTargetType = eTargetType;
}

void CDboTSContUsrSel::SetTargetTblIdx( unsigned int uiTargetTblIdx )
{
	m_uiTargetTblIdx = uiTargetTblIdx;
}

void CDboTSContUsrSel::SetDesc( unsigned int uiDesc )
{
	m_uiDesc = uiDesc;
}

bool CDboTSContUsrSel::IsNextLink( NTL_TS_TC_ID tcId ) const
{
	mapdef_NEXT_LINK::const_iterator cit = m_defNextLink.find( tcId );
	if ( cit != m_defNextLink.end() ) return true;
	return false;
}

unsigned int CDboTSContUsrSel::GetNextLink( NTL_TS_TC_ID tcId ) const
{
	mapdef_NEXT_LINK::const_iterator cit = m_defNextLink.find( tcId );
	if ( cit != m_defNextLink.end() )
	{
		return cit->second;
	}

	return 0xffffffff;
}

CDboTSContUsrSel::mapdef_NEXT_LINK_CIT CDboTSContUsrSel::GetNextLinkBegin( void ) const
{
	return m_defNextLink.begin();
}

CDboTSContUsrSel::mapdef_NEXT_LINK_CIT CDboTSContUsrSel::GetNextLinkEnd( void ) const
{
	return m_defNextLink.end();
}

bool CDboTSContUsrSel::AttachNextLink( NTL_TS_TC_ID tcId, unsigned int uiName )
{
	if ( NTL_TS_TC_ID_INVALID == tcId )
	{
		CNtlTSLog::Log( "Invalid trigger container id. Info[%d]. [%s]", tcId, TS_CODE_TRACE() );
		return false;
	}

	if ( m_defNextLink.size() >= MAX_NEXT_LINK_COUNT )
	{
		CNtlTSLog::Log( "The next link count can not more than %d. [%s]", MAX_NEXT_LINK_COUNT, TS_CODE_TRACE() );
		return false;
	}

	m_defNextLink[tcId] = uiName;

	return true;
}

void CDboTSContUsrSel::DetachNextLink( NTL_TS_TC_ID tcId )
{
	mapdef_NEXT_LINK::iterator it = m_defNextLink.find( tcId );
	if ( it != m_defNextLink.end() )
	{
		m_defNextLink.erase( it );
	}
}

void CDboTSContUsrSel::SetCancelLink( NTL_TS_TC_ID tcId )
{
	m_tcCancelLink = tcId;
}

void CDboTSContUsrSel::SetLogInLink( NTL_TS_TC_ID tcId )
{
	m_tcLogInLink = tcId;
}

void CDboTSContUsrSel::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "ust" ) )
	{
		SetUserSelType( (eUSER_SEL_TYPE)clProperty.GetValueAsInt( "ust" ) );
	}

	if ( clProperty.IsExist( "uspt" ) )
	{
		SetUserSelProgState( (eUSER_SEL_PROGRESS_STATE)clProperty.GetValueAsInt( "uspt" ) );
	}

	if ( clProperty.IsExist( "type" ) )
	{
		SetTargetType( (eUSER_SEL_TARGET_TYPE)clProperty.GetValueAsInt( "type" ) );
	}

	if ( clProperty.IsExist( "idx" ) )
	{
		SetTargetTblIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "desc" ) )
	{
		SetDesc( clProperty.GetValueAsInt( "desc" ) );
	}

	const std::string strId = "nid";
	const std::string strDesc = "ndesc";

	std::string strConcId, strConcDesc;

	for ( int i = 0; i < MAX_NEXT_LINK_COUNT; ++i )
	{
		sprintf_s( g_NtlTSString, "%d", i ); strConcId = strId + g_NtlTSString;
		sprintf_s( g_NtlTSString, "%d", i ); strConcDesc = strDesc + g_NtlTSString;

		if ( clProperty.IsExist( strConcId ) )
		{
			AttachNextLink( (NTL_TS_TC_ID)clProperty.GetValueAsInt( strConcId ), clProperty.GetValueAsInt( strConcDesc ) );
		}
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

void CDboTSContUsrSel::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", (int)GetUserSelType() );
	clProperty.m_defProperty["ust"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", (int)GetUserSelProgState() );
	clProperty.m_defProperty["uspt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", (int)GetTargetType() );
	clProperty.m_defProperty["type"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTargetTblIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDesc() );
	clProperty.m_defProperty["desc"] = g_NtlTSString;

	const std::string strId = "nid";
	const std::string strDesc = "ndesc";

	std::string strConcId, strConcDesc;

	mapdef_NEXT_LINK::iterator cit = m_defNextLink.begin();
	for ( int i = 0; cit != m_defNextLink.end(); ++i, ++cit )
	{
		sprintf_s( g_NtlTSString, "%d", i ); strConcId = strId + g_NtlTSString;
		sprintf_s( g_NtlTSString, "%d", cit->first );
		clProperty.m_defProperty[strConcId] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", i ); strConcDesc = strDesc + g_NtlTSString;
		sprintf_s( g_NtlTSString, "%d", cit->second );
		clProperty.m_defProperty[strConcDesc] = g_NtlTSString;
	}

	sprintf_s( g_NtlTSString, "%d", GetCancelLink() );
	clProperty.m_defProperty["cancellnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetLogInLink() );
	clProperty.m_defProperty["lilnk"] = g_NtlTSString;
}
