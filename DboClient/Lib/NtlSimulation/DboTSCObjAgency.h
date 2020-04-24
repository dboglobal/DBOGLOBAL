#ifndef _DBO_TSCOBJAGENCY_H_
#define _DBO_TSCOBJAGENCY_H_


#include "DboTSCore.h"
#include "ceventhandler.h"


/** 
	Client object agency
*/


class CDboTSCObjAgency : public CDboTSTAgency, public RWS::CEventHandler
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	SGET_QUEST_INFO						m_QuestInfo;

// Constructions and Destructions
public:
	CDboTSCObjAgency( void );
	virtual ~CDboTSCObjAgency( void );

// Methods
public:
	SGET_QUEST_INFO*					GetQuestInfoList( eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId );

	virtual void						HandleEvents( RWS::CMsg& Msg );
};


#endif