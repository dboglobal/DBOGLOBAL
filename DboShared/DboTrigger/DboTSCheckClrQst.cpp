#include "precomp_trigger.h"
#include "DboTSCheckClrQst.h"


/**
	Check cleared quest condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckClrQst, CNtlTSCond )


CDboTSCheckClrQst::CDboTSCheckClrQst( void )
{
}


bool CDboTSCheckClrQst::SetAndIdList( const std::string& strIDList )
{
	std::string strNum;
	strNum.reserve( 10 );

	std::string::const_iterator cit = strIDList.begin();
	for ( ; cit != strIDList.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_vecAndIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_vecAndIdList.clear();
			CNtlTSLog::Log( "Only must have numbers. Info[%s]. [%s]", strIDList.c_str(), TS_CODE_TRACE() );
			return false;
		}
		else
		{
			strNum += *cit;
		}
	}

	if ( !strNum.empty() )
	{
		m_vecAndIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
	}

	return true;
}

void CDboTSCheckClrQst::ClearAndIdList( void )
{
	m_vecAndIdList.clear();
}

bool CDboTSCheckClrQst::SetOrIdList( const std::string& strIDList )
{
	std::string strNum;
	strNum.reserve( 10 );

	std::string::const_iterator cit = strIDList.begin();
	for ( ; cit != strIDList.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_vecOrIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_vecOrIdList.clear();
			CNtlTSLog::Log( "Only must have numbers. Info[%s]. [%s]", strIDList.c_str(), TS_CODE_TRACE() );
			return false;
		}
		else
		{
			strNum += *cit;
		}
	}

	if ( !strNum.empty() )
	{
		m_vecOrIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
	}

	return true;
}

void CDboTSCheckClrQst::ClearOrIdList( void )
{
	m_vecOrIdList.clear();
}



NTL_TS_T_ID CDboTSCheckClrQst::BeginNextQidLink( void )
{
	m_itNextQidLink = m_defNextQidLink.begin();
	if ( m_itNextQidLink == m_defNextQidLink.end() ) return 0;
	return *m_itNextQidLink;
}

NTL_TS_T_ID CDboTSCheckClrQst::NextNextQidLink( void )
{
	++m_itNextQidLink;
	if ( m_itNextQidLink == m_defNextQidLink.end() ) return 0;
	return *m_itNextQidLink;
}

bool CDboTSCheckClrQst::AttachNextQidLink( NTL_TS_T_ID tId )
{
	if ( NTL_TS_T_ID_INVALID == tId )
	{
		printf( "Can not attach the invalid quest id. ParamInfo[%d]. [%s]", tId, TS_CODE_TRACE() );
		return false;
	}

	m_defNextQidLink.push_back( tId );

	return true;
}

void CDboTSCheckClrQst::DetachNextQidLink( NTL_TS_T_ID tId )
{
	if ( NTL_TS_T_ID_INVALID == tId )
	{
		printf( "Can not detach the invalid quest id. ParamInfo[%d]. [%s]", tId, TS_CODE_TRACE() );
		return;
	}

	listdef_NextQidList::iterator it = std::find( m_defNextQidLink.begin(), m_defNextQidLink.end(), tId );
	if ( it != m_defNextQidLink.end() )
	{
		m_defNextQidLink.erase( it );
	}
	else
	{
		printf( "Can not find the quest id. ParamInfo[%d]. [%s]", tId, TS_CODE_TRACE() );
	}
}

bool CDboTSCheckClrQst::ParsingNextQidLink( const std::string& strContents )
{
	std::string strNum;

	std::string::const_iterator cit = strContents.begin();
	for ( ; cit != strContents.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_defNextQidLink.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( ' ' == *cit )
		{
			continue;
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_defNextQidLink.clear();

			printf( "The link info must have to be NUMERIC" );

			return false;
		}
		else
		{
			strNum += *cit;
		}
	}

	if ( !strNum.empty() )
	{
		m_defNextQidLink.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
	}

	return true;
}

void CDboTSCheckClrQst::SetFlink( int tFlink )
{
	m_tFlink = tFlink;
}
void CDboTSCheckClrQst::SetNOT( int tNOT )
{
	m_tNOT = tNOT;
}

void CDboTSCheckClrQst::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "and" ) )
	{
		SetAndIdList( clProperty.GetValue( "and" ) );
	}

	if ( clProperty.IsExist( "or" ) )
	{
		SetOrIdList( clProperty.GetValue( "or" ) );
	}

	if ( clProperty.IsExist( "flink" ) )
	{
		SetFlink( (BYTE)clProperty.GetValueAsInt( "flink" ) );
	}
	if ( clProperty.IsExist( "not" ) )
	{
		SetNOT( (BYTE)clProperty.GetValueAsInt( "not" ) );
	}
	if ( clProperty.IsExist( "flinknextqid" ) )
	{
		std::string strNextQLnk = clProperty.GetValue( "flinknextqid" );
		ParsingNextQidLink( strNextQLnk );
	}
}

void CDboTSCheckClrQst::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	std::string strIdList;
	std::vector<NTL_TS_T_ID>::iterator it;

	it = m_vecAndIdList.begin();
	for ( ; it != m_vecAndIdList.end(); ++it )
	{
		sprintf_s( g_NtlTSString, "%d", *it );
		strIdList += g_NtlTSString;
		strIdList += ";";
	}
	if ( !strIdList.empty() )
	{
		clProperty.m_defProperty["and"] = strIdList;
	}

	strIdList.clear();
	it = m_vecOrIdList.begin();
	for ( ; it != m_vecOrIdList.end(); ++it )
	{
		sprintf_s( g_NtlTSString, "%d", *it );
		strIdList += g_NtlTSString;
		strIdList += ";";
	}
	if ( !strIdList.empty() )
	{
		clProperty.m_defProperty["or"] = strIdList;
	}

	sprintf_s( g_NtlTSString, "%d", GetFlink() );
	clProperty.m_defProperty["flink"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNOT() );
	clProperty.m_defProperty["not"] = g_NtlTSString;

	if ( !m_defNextQidLink.empty() )
	{
		std::string strPreLnk;
		listdef_NextQidList::iterator it2 = m_defNextQidLink.begin();
		for ( ; it2 != m_defNextQidLink.end(); ++it2 )
		{
			sprintf_s( g_NtlTSString, "%d", *it2 );
			strPreLnk += g_NtlTSString;
			strPreLnk += ";";
		}

		clProperty.m_defProperty["flinknextqid"] = strPreLnk;
	}
}