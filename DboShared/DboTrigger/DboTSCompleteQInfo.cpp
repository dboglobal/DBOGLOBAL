#include "precomp_trigger.h"
#include "DboTSCompleteQInfo.h"


/** 
	Completion quest info
*/



CDboTSCompleteQInfo::CDboTSCompleteQInfo( void )
{
	memset( &m_stCQInfo, 0, sizeof( sCOMPLETE_QUEST_INFO ) );
}

CDboTSCompleteQInfo::~CDboTSCompleteQInfo( void )
{
}

void CDboTSCompleteQInfo::SetCompletedQInfo( const sCOMPLETE_QUEST_INFO& stCQInfo )
{
	memcpy( &m_stCQInfo, &stCQInfo, sizeof( sCOMPLETE_QUEST_INFO ) );
}

bool CDboTSCompleteQInfo::HasBeenClearQuest( NTL_TS_T_ID tId )
{
	if ( tId >= eQUEST_ID_RANGE_QUEST_MAX )
	{
		printf( "Over the max quest id. Info[%d]. [%s]\n", tId, TS_CODE_TRACE() );
		return false;
	}

	unsigned char c = m_stCQInfo.abyQCInfo[tId / eCOMPLETE_QUEST_QUEST_PER_BYTE];
	int nShift = (tId % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

	if ( c & (eCOMPLETE_QUEST_STATE_CLEAR << nShift) )
	{
		return true;
	}

	return false;
}

void CDboTSCompleteQInfo::SetClearQuest( NTL_TS_T_ID tId, bool bOn )
{
	if ( tId >= eQUEST_ID_RANGE_QUEST_MAX )
	{
		printf( "Over the max quest id. Info[%d]. [%s]\n", tId, TS_CODE_TRACE() );
		return;
	}

	unsigned char& c = m_stCQInfo.abyQCInfo[tId / eCOMPLETE_QUEST_QUEST_PER_BYTE];
	int nShift = (tId % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

	if ( bOn )	c |= (eCOMPLETE_QUEST_STATE_CLEAR << nShift);
	else		c &= ~(eCOMPLETE_QUEST_STATE_CLEAR << nShift);
}

void CDboTSCompleteQInfo::SetAllFlagsClear( NTL_TS_T_ID tId )
{
	if ( tId >= eQUEST_ID_RANGE_QUEST_MAX )
	{
		CNtlTSLog::Log( "Over the max quest id. Info[%d]. [%s]", tId, TS_CODE_TRACE() );
		return;
	}

	unsigned char& c = m_stCQInfo.abyQCInfo[tId / eCOMPLETE_QUEST_QUEST_PER_BYTE];
	int nShift = (tId % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

	c &= ~(eCOMPLETE_QUEST_STATE_CLEAR << nShift);
}

void CDboTSCompleteQInfo::InitQuest( NTL_TS_T_ID tStartId, NTL_TS_T_ID tEndId )
{
	for ( NTL_TS_T_ID i = tStartId; i < tEndId; ++i )
	{
		SetAllFlagsClear( i );
	}
}


sCOMPLETE_QUEST_BIT_INFO* CDboTSCompleteQInfo::GetCompletedQBitInfo( int nPos )
{
	//sCOMPLETE_QUEST_BIT_INFO* pabyQCBitInfo;
	//::CopyMemory( pabyQCBitInfo, &GetCompletedQInfo().abyQCInfo[ nPos * eCOMPLETE_QUEST_QUEST_PER_STRUCT ], sizeof(sCOMPLETE_QUEST_BIT_INFO) );
	return (sCOMPLETE_QUEST_BIT_INFO*)&m_stCQInfo.abyQCInfo[ nPos * eCOMPLETE_QUEST_QUEST_PER_STRUCT/eCOMPLETE_QUEST_QUEST_PER_BYTE ];
}