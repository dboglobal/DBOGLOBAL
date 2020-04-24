#include "precomp_ntlsimulation.h"
#include "DboTSCObjAgency.h"
#include "ObjectTable.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "TableContainer.h"
#include "NtlObject.h"


/** 
	Client object agency
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCObjAgency, CDboTSTAgency )


CDboTSCObjAgency::CDboTSCObjAgency( void )
{
	m_QuestInfo.QuestInfoList.reserve( 10 );
}

CDboTSCObjAgency::~CDboTSCObjAgency( void )
{
}

SGET_QUEST_INFO* CDboTSCObjAgency::GetQuestInfoList( eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId )
{
	m_QuestInfo.dwType = 0;
	m_QuestInfo.QuestInfoList.clear();

	return &m_QuestInfo;
}

void CDboTSCObjAgency::HandleEvents( RWS::CMsg& Msg )
{
}
