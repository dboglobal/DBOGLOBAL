#include "precomp_trigger.h"
#include "DboTSClickNPC.h"


/**
	Click NPC event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSClickNPC, CNtlTSEvent )


CDboTSClickNPC::CDboTSClickNPC( void )
{
}

unsigned int CDboTSClickNPC::GetNPCIdx( void )
{
	m_itNPCIdx = m_defNPCIdx.begin();
	if ( m_itNPCIdx == m_defNPCIdx.end() ) return 0;
	return *m_itNPCIdx;
}

unsigned int CDboTSClickNPC::NextNPCIdx( void )
{
	++m_itNPCIdx;
	if ( m_itNPCIdx == m_defNPCIdx.end() ) return 0;
	return *m_itNPCIdx;
}

bool CDboTSClickNPC::AttachNPCIdx( unsigned int NPCId )
{
	if ( INVALID_TBLIDX == NPCId )
	{
		printf( "Can not attach the invalid NPC id. ParamInfo[%d]. [%s]", NPCId, TS_CODE_TRACE() );
		return false;
	}

	m_defNPCIdx.push_back( NPCId );

	return true;
}

void CDboTSClickNPC::DetachNPCIdx( unsigned int NPCId )
{
	if ( INVALID_TBLIDX == NPCId )
	{
		printf( "Can not detach the invalid NPC id. ParamInfo[%d]. [%s]", NPCId, TS_CODE_TRACE() );
		return;
	}

	listdef_NPCIdx::iterator it = std::find( m_defNPCIdx.begin(), m_defNPCIdx.end(), NPCId );
	if ( it != m_defNPCIdx.end() )
	{
		m_defNPCIdx.erase( it );
	}
	else
	{
		printf( "Can not find the NPC id. ParamInfo[%d]. [%s]", NPCId, TS_CODE_TRACE() );
	}
}

bool CDboTSClickNPC::ParsingNPCIdx( const std::string& strContents )
{
	std::string strNum;

	std::string::const_iterator cit = strContents.begin();
	for ( ; cit != strContents.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_defNPCIdx.push_back( (unsigned int)atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( ' ' == *cit )
		{
			continue;
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_defNPCIdx.clear();

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
		m_defNPCIdx.push_back( (unsigned int)atoi( strNum.c_str() ) );
	}

	return true;
}

void CDboTSClickNPC::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "npcidx" ) )
	{
		std::string strNextnpcid = clProperty.GetValue( "npcidx" );
		ParsingNPCIdx( strNextnpcid );
	}

}

void CDboTSClickNPC::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	if ( !m_defNPCIdx.empty() )
	{
		std::string strPreLnk;
		listdef_NPCIdx::iterator it = m_defNPCIdx.begin();
		for ( ; it != m_defNPCIdx.end(); ++it )
		{
			sprintf_s( g_NtlTSString, "%d", *it );
			strPreLnk += g_NtlTSString;
			strPreLnk += ";";
		}

		clProperty.m_defProperty["npcidx"] = strPreLnk;
	}


}
