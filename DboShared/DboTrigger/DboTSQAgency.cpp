#include "precomp_trigger.h"
#include "DboTSQAgency.h"
#include "DboTSCompleteQInfo.h"
#include "DboTSQRecv.h"
#include "DboTSQCtrl.h"
#include "DboTSMain.h"
#include "DboTSCtrlFactory.h"


/** 
	Client quest agency
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSQAgency, CDboTSTAgency )


CDboTSQAgency::CDboTSQAgency( void )
{
	m_pCompleteQInfo = new CDboTSCompleteQInfo;
}

CDboTSQAgency::~CDboTSQAgency( void )
{
	if ( m_pCompleteQInfo )
	{
		delete m_pCompleteQInfo;
	}
}

void CDboTSQAgency::SaveCompletedQInfo( sCOMPLETE_QUEST_INFO& sInfo )
{
	memcpy( &sInfo, &m_pCompleteQInfo->GetCompletedQInfo(), sizeof(sCOMPLETE_QUEST_INFO) );
}

void CDboTSQAgency::SaveProgressInfo( unsigned char& byCount, sPROGRESS_QUEST_INFO* pQuestInfoList )
{
	byCount = 0;

	mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
	for ( ; it != m_defCurTList.end(); ++it )
	{
		CDboTSQCtrl* pQTrig = (CDboTSQCtrl*) it->second;

		pQuestInfoList[byCount].byVer = sPROGRESS_QUEST_INFO::eCUR_VERSION;
		pQuestInfoList[byCount].tId = pQTrig->GetTrigger()->GetID();

		pQTrig->SaveQuestProgressInfo_V0( pQuestInfoList[byCount].uData );

		++byCount;
	}
}

bool CDboTSQAgency::SaveProgressInfo( NTL_TS_T_ID tId, sPROGRESS_QUEST_INFO& sQuestInfoList )
{
	CDboTSQCtrl* pQTrig = (CDboTSQCtrl*)FindProgressTrigger( tId );
	if ( 0 == pQTrig ) return false;

	sQuestInfoList.byVer = sPROGRESS_QUEST_INFO::eCUR_VERSION;
	sQuestInfoList.tId = pQTrig->GetTrigger()->GetID();
	
	pQTrig->SaveQuestProgressInfo_V0( sQuestInfoList.uData );

	return true;
}

void CDboTSQAgency::LoadCompletedQInfo( const sCOMPLETE_QUEST_INFO& sInfo )
{
	if ( m_pCompleteQInfo )
	{
		m_pCompleteQInfo->SetCompletedQInfo( sInfo );
	}
	else
	{
		printf( "Can not save the completed quest info. [%s]\n", TS_CODE_TRACE() );
	}
}

void CDboTSQAgency::LoadQuestProgressInfo_V0( CNtlTSTrigger* pTrigger, const sPROGRESS_QUEST_INFO& stQInfo )
{
	if ( 0 == pTrigger )
	{
		printf( "The parameter \"pTrigger\" Must not be NULL. Info[%d]. [%s]\n", stQInfo.tId, TS_CODE_TRACE() );
		return;
	}

	// Quest controller 를 생성, 등록, 정보 설정
	CDboTSTCtrl* pTCtrl = MakeTriggerController( pTrigger );
	if ( pTCtrl && pTCtrl->IsDerivedClass( "CDboTSQCtrl" ) )
	{
		// 등록
		AttachProgressTrigger( pTrigger->GetID(), pTCtrl );

		// 퀘스트 진행 정보 설정
		((CDboTSQCtrl*)pTCtrl)->LoadQuestProgressInfo_V0( stQInfo.uData );
	}
	else
	{
		printf( "Trigger info mismatch. Info[%d,%s]. [%s]\n", pTrigger->GetID(), pTCtrl ? pTCtrl->GetClassName() : "NULL", TS_CODE_TRACE() );
	}
}

CDboTSTCtrl* CDboTSQAgency::MakeTriggerController( CNtlTSTrigger* pTrig )
{
	// Quest controller 를 생성한다
	CNtlTSControlObject* pCtrlObj = GetParent()->GetControlFactory()->CreateObj( "CDboTSQCtrl" );
	if ( !pCtrlObj->IsDerivedClass( "CDboTSQCtrl" ) )
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSQCtrl. Info[%s]. [%s]", pCtrlObj->GetClassName(), TS_CODE_TRACE() );
		GetParent()->GetControlFactory()->DeleteObj( pCtrlObj );
		return 0;
	}

	// TS의 trigger 와 Quest trigger 객체를 연결 및 저장한다
	((CDboTSQCtrl*)pCtrlObj)->SetTrigger( pTrig );
	((CDboTSQCtrl*)pCtrlObj)->SetParent( this );

	return (CDboTSQCtrl*)pCtrlObj;
}
