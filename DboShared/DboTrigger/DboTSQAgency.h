#ifndef _DBO_TSQAGENCY_H_
#define _DBO_TSQAGENCY_H_


#include "DboTSTAgency.h"


class CDboTSQCtrl;


/** 
	Quest agency
*/


class CDboTSQAgency : public CDboTSTAgency
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	CDboTSCompleteQInfo*				m_pCompleteQInfo;	// 퀘스트 완료 정보

// Constructions and Destructions
public:
	CDboTSQAgency( void );
	virtual ~CDboTSQAgency( void );

// Methods
public:
	CDboTSCompleteQInfo*				GetCompletedQInfo( void ) const;

	void								SaveCompletedQInfo( sCOMPLETE_QUEST_INFO& sInfo );
	void								SaveProgressInfo( unsigned char& byCount, sPROGRESS_QUEST_INFO* pQuestInfoList );
	bool								SaveProgressInfo( NTL_TS_T_ID tId, sPROGRESS_QUEST_INFO& sQuestInfoList );

// Implementations
protected:
	virtual CDboTSTCtrl*				MakeTriggerController( CNtlTSTrigger* pTrig );

	void								LoadCompletedQInfo( const sCOMPLETE_QUEST_INFO& sInfo );
	void								LoadQuestProgressInfo_V0( CNtlTSTrigger* pTrigger, const sPROGRESS_QUEST_INFO& stQInfo );
};


inline CDboTSCompleteQInfo* CDboTSQAgency::GetCompletedQInfo( void ) const
{
	return m_pCompleteQInfo;
}


#endif
