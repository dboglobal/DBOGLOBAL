#include "precomp_trigger.h"
#include "DboTSCheckDistWithNPC.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckDistWithNPC, CNtlTSCond )


CDboTSCheckDistWithNPC::CDboTSCheckDistWithNPC( void )
: m_fDistance( 0.f )
{
}

void CDboTSCheckDistWithNPC::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "dist" ) )
	{
		SetDistance((float)atof( clProperty.GetValue( "dist" ).c_str() ) );
	}

	if ( clProperty.IsExist( "idxlist" ) )
	{
		std::string strIndexList = clProperty.GetValue( "idxlist" );
		SetNPCIndexList( strIndexList );
	}
}

void CDboTSCheckDistWithNPC::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%f", GetDistance() );
	clProperty.m_defProperty["dist"] = g_NtlTSString;

	std::string strNPCIndexList;

	vecdef_NPC_INDEX_LIST::iterator it = m_defNPCIndexList.begin();
	for ( ; it != m_defNPCIndexList.end(); ++it )
	{
		sprintf_s( g_NtlTSString, "%d", *it );
		strNPCIndexList += g_NtlTSString;
		strNPCIndexList += ";";
	}
	if ( !strNPCIndexList.empty() )
	{
		clProperty.m_defProperty["idxlist"] = strNPCIndexList;
	}
}


bool CDboTSCheckDistWithNPC::SetNPCIndexList( const std::string& strNPCIndexList )
{
	m_defNPCIndexList.clear();

	std::string strNum;
	strNum.reserve( 10 );

	std::string::const_iterator cit = strNPCIndexList.begin();
	for ( ; cit != strNPCIndexList.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_defNPCIndexList.push_back( atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_defNPCIndexList.clear();
			CNtlTSLog::Log( "Only must have numbers. Info[%s]. [%s]", strNPCIndexList.c_str(), TS_CODE_TRACE() );
			return false;
		}
		else
		{
			strNum += *cit;
		}
	}

	if ( !strNum.empty() )
	{
		m_defNPCIndexList.push_back( atoi( strNum.c_str() ) );
	}

	return true;
}