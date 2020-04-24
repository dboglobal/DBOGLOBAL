#ifndef _DBO_TSCOMPLETEQINFO_H_
#define _DBO_TSCOMPLETEQINFO_H_


#include "DboTSCoreDefine.h"


/** 
	Completion quest info
*/


class CDboTSCompleteQInfo
{
// Member variables
protected:
	sCOMPLETE_QUEST_INFO				m_stCQInfo;

// Constructions and Destructions
public:
	CDboTSCompleteQInfo( void );
	~CDboTSCompleteQInfo( void );

// Methods
public:
	sCOMPLETE_QUEST_INFO&				GetCompletedQInfo( void );

	void								SetCompletedQInfo( const sCOMPLETE_QUEST_INFO& stCQInfo );

	sCOMPLETE_QUEST_BIT_INFO*			GetCompletedQBitInfo( int nPos );

	BYTE								GetCompletedQBitPosition( NTL_TS_T_ID tId ) { return (BYTE)(tId / eCOMPLETE_QUEST_QUEST_PER_STRUCT); }

	bool								HasBeenClearQuest( NTL_TS_T_ID tId );
	void								SetClearQuest( NTL_TS_T_ID tId, bool bOn );

	void								SetAllFlagsClear( NTL_TS_T_ID tId );

	// Start Id <= ??? < End Id : 반계 범위 사용
	void								InitQuest( NTL_TS_T_ID tStartId, NTL_TS_T_ID tEndId );
};


inline sCOMPLETE_QUEST_INFO& CDboTSCompleteQInfo::GetCompletedQInfo( void )
{
	return m_stCQInfo;
}


#endif