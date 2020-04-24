#include "precomp_trigger.h"
#include "DboTSContSwitch.h"


/**
	Switch container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContSwitch, CNtlTSCont )


CDboTSContSwitch::CDboTSContSwitch( void )
{
}

bool CDboTSContSwitch::AttachNextLink( NTL_TS_TC_ID tcId, unsigned int uiBranchID )
{
	m_defNextLink.push_back( sBranchInfo( tcId, uiBranchID ) );

	return true;
}

void CDboTSContSwitch::DetachNextLink( NTL_TS_TC_ID tcId )
{
	listdef_NEXT_LINK::iterator it = m_defNextLink.begin();
	for ( ; it != m_defNextLink.end(); )
	{
		sBranchInfo& sInfo = *it;

		if ( sInfo.tcNextId == tcId )
		{
			it = m_defNextLink.erase( it );
		}
		else
		{
			++it;
		}
	}
}

void CDboTSContSwitch::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	const std::string strContID = "cid";
	const std::string strBranchID = "bid";

	std::string strCId, strBId ;

	for ( int i = 0; i < MAX_NEXT_LINK_COUNT; ++i )
	{
		sprintf_s( g_NtlTSString, "%d", i ); strCId = strContID + g_NtlTSString;
		sprintf_s( g_NtlTSString, "%d", i ); strBId = strBranchID + g_NtlTSString;

		if ( clProperty.IsExist( strCId ) )
		{
			NTL_TS_TC_ID tcCId = (NTL_TS_TC_ID)clProperty.GetValueAsInt( strCId );
			unsigned int uiBId = (unsigned int)clProperty.GetValueAsInt( strBId );

			if ( !AttachNextLink( tcCId, uiBId ) )
			{
				CNtlTSLog::Log( "The container id already exist. Info[%d,%d]. [%s]", tcCId, uiBId, TS_CODE_TRACE() );
			}
		}
	}
}

void CDboTSContSwitch::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	const std::string strContID = "cid";
	const std::string strBranchID = "bid";

	std::string strCId, strBId ;

	listdef_NEXT_LINK_CIT cit = m_defNextLink.begin();
	for ( int i = 0; cit != m_defNextLink.end(); ++i, ++cit )
	{
		const sBranchInfo& sInfo = *cit;

		sprintf_s( g_NtlTSString, "%d", i ); strCId = strContID + g_NtlTSString;
		sprintf_s( g_NtlTSString, "%d", sInfo.tcNextId );
		clProperty.m_defProperty[strCId] = g_NtlTSString;

		sprintf_s( g_NtlTSString, "%d", i ); strBId = strBranchID + g_NtlTSString;
		sprintf_s( g_NtlTSString, "%d", sInfo.uiBranchId );
		clProperty.m_defProperty[strBId] = g_NtlTSString;
	}
}
