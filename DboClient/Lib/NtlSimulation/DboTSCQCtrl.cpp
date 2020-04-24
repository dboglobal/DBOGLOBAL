#include "precomp_ntlsimulation.h"
#include "DboTSCQCtrl.h"
#include "DboTSCQRecv.h"
#include "DboTSCQAgency.h"
#include "NtlSLApi.h"
#include "NtlClock.h"
#include "NTLResultCode.h"
#include "NtlSLGlobal.h"
#include "DboTSCMain.h"
#include "TableContainer.h"
#include "ObjectTable.h"
#include "NtlSLEventFunc.h"
#include "DboTSCQAgency.h"
#include "NtlCameraController.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlInventory.h"
#include "NtlSobItem.h"
#include "NtlSobManager.h"
#include "NtlSobItemAttr.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "QuestItemTable.h"
#include "DboTSDefine.h"
#include "NtlSLLogic.h"
#include "NtlSLPacketGenerator.h"


/** 
	Search receiver
*/

class CEventSearch : public CNtlTSRecv
{
	virtual	NTL_TSRESULT Search( CNtlTSEntity* pEntity, void* pParam )
	{
		if ( DBO_EVENT_TYPE_ID_RCV_SVR_EVT == pEntity->GetEntityType() )
		{
			CDboTSCQCtrl::mmapdef_SVR_EVENT_ID_LIST* pParamData = (CDboTSCQCtrl::mmapdef_SVR_EVENT_ID_LIST*) pParam;

			CDboTSRcvSvrEvt* pObj = (CDboTSRcvSvrEvt*) pEntity;

			sTS_KEY sKey;
			sKey.Init();
			sKey.tcID = ((CNtlTSCont*)pEntity->GetParent())->GetID();
			sKey.tgID = ((CNtlTSGroup*)pEntity->GetParent()->GetParent())->GetID();
			sKey.tID = ((CNtlTSTrigger*)pEntity->GetParent()->GetParent()->GetParent())->GetID();

			if ( sKey.tgID == NTL_TS_MAIN_GROUP_ID )
			{
				pParamData->insert( CDboTSCQCtrl::mmapdef_SVR_EVENT_ID_LIST::value_type( pObj->GetEvtID(), sKey ) );
			}
		}

		return NTL_TSRESULT_TYPE_SUCCESS;
	}

	virtual	NTL_TSRESULT Run( CNtlTSEntity* pEntity, void* pParam )
	{
		return NTL_TSRESULT_TYPE_SUCCESS;
	}
};


/**
	Client quest controller
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCQCtrl, CDboTSQCtrl )


CDboTSCQCtrl::CDboTSCQCtrl()
{
	m_uiTitle = 0xffffffff;
	m_uiArea = 0xffffffff;
	m_uiGoal = 0xffffffff;
	m_uiGrade = 0xffffffff;
	m_eGradeType = eGRADE_TYPE_INVALID;
	m_eSort = eQUEST_SORT_TYPE_INVALID;
	m_uiSort = 0xffffffff;
	m_uiContents = 0xffffffff;
	m_pDefRwd = 0;
	m_pSelRwd = 0;

	m_uiUpdateTimeCnt = 0;

	m_bQuestShare = false;

	m_eCTimingExcType = eREG_C_TIMING_EXCUTE_TYPE_INVALID;
	memset( &m_uCTimingExcData, 0xffffffff, sizeof(uREG_C_TIMING_EXCUTE_DATA) );

	m_bTransCameraCtrlRight = FALSE;
}

CDboTSCQCtrl::~CDboTSCQCtrl()
{
	m_mapNPCIdxList.clear();

	std::vector<QM_KEY> vecRmvList; vecRmvList.reserve( 10 );

	mapdef_QUESTMARKINFO_LIST::iterator itRmvQMIList = m_defQuestMarkInfoList.begin();
	for ( ; itRmvQMIList != m_defQuestMarkInfoList.end(); ++itRmvQMIList )
	{
		vecRmvList.push_back( itRmvQMIList->first );
	}

	std::vector<QM_KEY>::iterator itRmvList = vecRmvList.begin();
	for ( ; itRmvList != vecRmvList.end(); ++itRmvList )
	{
		UnregisterQuestMark( *itRmvList );
	}

	ClearAllClientTimingData();
}

void CDboTSCQCtrl::Update( void )
{
	// 종료 상태인 경우
	if ( IsExitState() ) return;

	// 예외 타이머 업데이트
	UpdateExceptionTimer();

	// 제한 시간 타입 업데이트
	UpdateLimitTime();

	// 대기 타이머 업데이트
	UpdateTimeWait();

	// 대기 타이머 동작
	if ( !IsTimeWait() )
	{
		// 동기화 큐가 동작 중이면 다음 단계로 진행하지 않는다
		if ( IsContSyncQueueEmpty() )
		{
			// Server event ID update
			UpdateSvrEventID();

			// 퀘스트를 진행한다
			UpdateTSStep();
		}
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if ( pAvatar )
	{
		RwV3d vAvatarPos = pAvatar->GetPosition();
		MatchTimingData_EnterRgn( GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx, vAvatarPos.x, vAvatarPos.z );
	}
}

RwUInt32 CDboTSCQCtrl::GetTitle()
{
	if ( 0 == m_pTrigger )
	{
		return m_uiTitle;
	}
	else
	{
		if ( 0xffffffff == m_uiTitle )
		{
			return m_pTrigger->GetTitle();
		}
		else
		{
			return m_uiTitle;
		}
	}
}

unsigned int CDboTSCQCtrl::GetCurTime( void )
{
	if ( API_GetGameClock() ) return API_GetGameClock()->GetTime_AvatarTime();
	return 0;
}

bool CDboTSCQCtrl::IsCompletedServerEvt( void )
{
	if ( NTL_TS_TC_ID_INVALID == m_sSToCEvtData.tcId ||
		 NTL_TS_TA_ID_INVALID == m_sSToCEvtData.taId )
	{
		return false;
	}

	switch ( m_sSToCEvtData.eSToCEvtDataType )
	{
	case eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT:
		{
			bool bRet = true;
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_MOB_KILL; ++i )
			{
				if ( 0xffffffff == m_sSToCEvtData.uSToCEvtData.sMobKillCnt[i].uiMobIdx ) continue;

				bRet &= ( m_sSToCEvtData.uSToCEvtData.sMobKillCnt[i].nMobCnt <= m_sSToCEvtData.uSToCEvtData.sMobKillCnt[i].nCurMobCnt ) ? true : false;
			}

			if ( bRet )
			{
				return true;
			}
		}
		break;
	case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
		{
			bool bRet = true;
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM; ++i )
			{
				if ( 0xffffffff == m_sSToCEvtData.uSToCEvtData.sMobKillItemCnt[i].uiMobLIIdx ) continue;

				bRet &= ( m_sSToCEvtData.uSToCEvtData.sMobKillItemCnt[i].nMobLICnt <= m_sSToCEvtData.uSToCEvtData.sMobKillItemCnt[i].nCurMobLICnt ) ? true : false;
			}

			if ( bRet )
			{
				return true;
			}
		}
		break;
	case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
		{
			bool bRet = true;
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM; ++i )
			{
				if ( 0xffffffff == m_sSToCEvtData.uSToCEvtData.sDeliveryItemCnt[i].uiItemIdx ) continue;

				bRet &= ( 0 >= m_sSToCEvtData.uSToCEvtData.sDeliveryItemCnt[i].nCurItemCnt ) ? true : false;
			}

			if ( bRet )
			{
				return true;
			}
		}
		break;
	case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
		{
			bool bRet = true;
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_OBJECT_ITEM; ++i )
			{
				if ( 0xffffffff == m_sSToCEvtData.uSToCEvtData.sObjectItemCnt[i].uiItemIdx ) continue;

				bRet &= ( m_sSToCEvtData.uSToCEvtData.sObjectItemCnt[i].nItemCnt <= m_sSToCEvtData.uSToCEvtData.sObjectItemCnt[i].nCurItemCnt ) ? true : false;
			}

			if ( bRet )
			{
				return true;
			}
		}
		break;
	case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT:
		{
			CNtlQuestInventory* pQInventory = GetNtlSLGlobal()->GetSobAvatar()->GetQuestInventory();
			if ( NULL == pQInventory )
			{
				return false;
			}

			bool bRet = true;
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM; ++i )
			{
				if ( !bRet ) break;

				if ( 0xffffffff == m_sSToCEvtData.uSToCEvtData.sPublicMobItemCnt[i].uiItemIdx ) continue;

				bRet &= ( m_sSToCEvtData.uSToCEvtData.sPublicMobItemCnt[i].nItemCnt <= m_sSToCEvtData.uSToCEvtData.sPublicMobItemCnt[i].nCurItemCnt ) ? true : false;

				if ( !bRet ) break;

				// 필요한 아이템이 Invalid 인 경우 필요 아이템이 없는 경우로 간주
				unsigned int uiNeedItemIdx = m_sSToCEvtData.uSToCEvtData.sPublicMobItemCnt[i].uiRequireItemIdx;
				if ( 0xffffffff == uiNeedItemIdx ) continue;

				// 해당 퀘스트 아이템을 유저가 가지고 있는지 검사한다
				{
					int nSumCnt = 0;

					// Quest inventory를 검색
					for ( int j = 0; j < MAX_QUEST_INVENTORY_SLOT; ++j )
					{
						CNtlSobQuestItem* pQItem = pQInventory->GetQuestItemFromIdx( j );
						if ( 0 == pQItem ) continue;

						CNtlSobQuestItemAttr* pQItemAttr = reinterpret_cast< CNtlSobQuestItemAttr* > ( pQItem->GetSobAttr() );

						if ( pQItemAttr->GetQuestItemTbl()->tblidx == uiNeedItemIdx )
						{
							nSumCnt += pQItemAttr->GetStackNum();
						}
					}

					bRet &= (nSumCnt > 0 ? true : false);

					if ( !bRet )
					{
						break;
					}
				}
			}

			if ( bRet )
			{
				return true;
			}
		}
		break;

	case eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT:
		{
			bool bRet = true;
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT; ++i )
			{
				if ( 0xffffffff == m_sSToCEvtData.uSToCEvtData.sCustomEvtCnt[i].uiCustomEvtID ) continue;

				bRet &= ( m_sSToCEvtData.uSToCEvtData.sCustomEvtCnt[i].nMaxCnt <= m_sSToCEvtData.uSToCEvtData.sCustomEvtCnt[i].nCurCnt ) ? true : false;
			}

			if ( bRet )
			{
				return true;
			}
		}
		break;

	case eSTOC_EVT_DATA_TYPE_VISIT:
		{
			bool bRet = true;
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; ++i )
			{
				if ( 0xff == m_sSToCEvtData.uSToCEvtData.sVisitEvt[i].byObjType ) continue;

				bRet &= m_sSToCEvtData.uSToCEvtData.sVisitEvt[i].bCompleted;
			}

			if ( bRet )
			{
				return true;
			}
		}
		break;
	}

	return false;
}

bool CDboTSCQCtrl::HasEventNPCInProgressQuest( unsigned int uiNPCIdx )
{
	std::map< unsigned int, unsigned int >::iterator it = m_mapNPCIdxList.find( uiNPCIdx );
	if ( it == m_mapNPCIdxList.end() ) return false;
	return true;
}

void CDboTSCQCtrl::RegNPCCameraStopEvt( void )
{
	m_bNPCCameraStopEvt = true;
}

void CDboTSCQCtrl::UnregNPCCameraStopEvt( void )
{
	if ( m_bNPCCameraStopEvt )
	{
		if ( m_bTransCameraCtrlRight )
		{
			Logic_CancelNpcFacing();
		}
		m_bNPCCameraStopEvt = false;
	}
}

void CDboTSCQCtrl::MatchTimingData_MobClick( unsigned int uiMobGroupIdx )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_MOB_CLICK );
	if ( it == m_defCTimingDataList.end() ) return;

	sREG_C_TIMING_DATA_MOB_CLICK* pData = (sREG_C_TIMING_DATA_MOB_CLICK*) it->second;
	if ( NULL == pData )
	{
		m_defCTimingDataList.erase( it );
	}
	else
	{
		if ( pData->uiMobGroupIdx == uiMobGroupIdx )
		{
			NTL_DELETE( pData );

			m_defCTimingDataList.erase( it );

			DoClientTimingAction();
		}
	}
}

void CDboTSCQCtrl::MatchTimingData_NpcClick( unsigned int uiNPCIdx )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_NPC_CLICK );
	if ( it == m_defCTimingDataList.end() ) return;

	sREG_C_TIMING_DATA_NPC_CLICK* pData = (sREG_C_TIMING_DATA_NPC_CLICK*) it->second;
	if ( NULL == pData )
	{
		m_defCTimingDataList.erase( it );
	}
	else
	{
		if ( pData->uiNpcIdx == uiNPCIdx )
		{
			NTL_DELETE( pData );

			m_defCTimingDataList.erase( it );

			DoClientTimingAction();
		}
	}
}

void CDboTSCQCtrl::MatchTimingData_ObjClick( unsigned int uiWorldIdx, unsigned int uiObjIdx )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_OBJ_CLICK );
	if ( it == m_defCTimingDataList.end() ) return;

	sREG_C_TIMING_DATA_OBJ_CLICK* pData = (sREG_C_TIMING_DATA_OBJ_CLICK*) it->second;
	if ( NULL == pData )
	{
		m_defCTimingDataList.erase( it );
	}
	else
	{
		if ( pData->uiWorldIdx == uiWorldIdx &&
			 pData->uiObjIdx == uiObjIdx )
		{
			NTL_DELETE( pData );

			m_defCTimingDataList.erase( it );

			DoClientTimingAction();
		}
	}
}

void CDboTSCQCtrl::MatchTimingData_EnterRgn( unsigned int uiWorldIdx, float fAvatarPosX, float fAvatarPosZ )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_ENTER_RGN );
	if ( it == m_defCTimingDataList.end() ) return;

	sREG_C_TIMING_DATA_ENTER_RGN* pData = (sREG_C_TIMING_DATA_ENTER_RGN*) it->second;
	if ( NULL == pData )
	{
		m_defCTimingDataList.erase( it );
	}
	else
	{
		if ( pData->uiWorldIdx == uiWorldIdx &&
			 pData->fMinX <= fAvatarPosX &&
			 pData->fMaxX >= fAvatarPosX &&
			 pData->fMinZ <= fAvatarPosZ &&
			 pData->fMaxZ >= fAvatarPosZ )
		{
			NTL_DELETE( pData );

			m_defCTimingDataList.erase( it );

			DoClientTimingAction();
		}
	}
}

void CDboTSCQCtrl::MatchTimingData_GetQItem( unsigned int uiQItemIdx )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_GET_QITEM );
	if ( it == m_defCTimingDataList.end() ) return;

	sREG_C_TIMING_DATA_GET_QITEM* pData = (sREG_C_TIMING_DATA_GET_QITEM*) it->second;
	if ( NULL == pData )
	{
		m_defCTimingDataList.erase( it );
	}
	else
	{
		if ( pData->uiQItemIdx == uiQItemIdx )
		{
			NTL_DELETE( pData );

			m_defCTimingDataList.erase( it );

			DoClientTimingAction();
		}
	}
}

void CDboTSCQCtrl::MatchTimingData_QTitleClick( void )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_QLISTWND_QTITLE_CLICK );
	if ( it == m_defCTimingDataList.end() ) return;

	m_defCTimingDataList.erase( it );

	DoClientTimingAction();
}

void CDboTSCQCtrl::MatchTimingData_AcceptClick( void )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_ACCEPT_QUEST );
	if ( it == m_defCTimingDataList.end() ) return;

	m_defCTimingDataList.erase( it );

	DoClientTimingAction();
}

void CDboTSCQCtrl::MatchTimingData_SuccessQuest( void )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_SUCCESS_QUEST );
	if ( it == m_defCTimingDataList.end() ) return;

	m_defCTimingDataList.erase( it );

	DoClientTimingAction();
}

void CDboTSCQCtrl::MatchTimingData_PreReward( void )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_PRE_REWARD );
	if ( it == m_defCTimingDataList.end() ) return;

	m_defCTimingDataList.erase( it );

	DoClientTimingAction();
}

void CDboTSCQCtrl::MatchTimingData_PostReward( void )
{
	if ( IsError() || IsFailed() )
	{
		ClearAllClientTimingData();
		return;
	}

	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eREG_C_TIMING_POST_REWARD );
	if ( it == m_defCTimingDataList.end() ) return;

	m_defCTimingDataList.erase( it );

	DoClientTimingAction();
}

void CDboTSCQCtrl::SkipContainer( NTL_TS_TC_ID tcID )
{
	if ( m_pCurTSP->GetID() == tcID )
	{
		switch ( m_pCurTSP->GetEntityType() )
		{
		case DBO_CONT_TYPE_ID_CONT_GCOND:
			{
				SetEventData( eEVENT_GEN_TYPE_SKIP_CONT, 0xffffffff );

				CDboTSContGCond* pCondCont = (CDboTSContGCond*) m_pCurTSP;

				unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

				UG_Avatar_TS_Confirm_Step( pCondCont->GetID(), pCondCont->GetYesLinkID(), uiParam, GetEventType(), GetEventData() );

				InitEventData();
			}
			break;
		}
	}
}

void CDboTSCQCtrl::Build( void )
{
	// Server event id 빌드
	m_defSvrEventIDList.clear();

	CEventSearch clReceive;
	m_pTrigger->SearchAll( &clReceive, &m_defSvrEventIDList );
}

void CDboTSCQCtrl::AttachSvrEventID( NTL_TS_EVENT_ID eID )
{
	sSERVER_EVENT_ID_DATA sData;
	sData.uiEventID = eID;

	std::pair< mmapdef_SVR_EVENT_ID_LIST::iterator, mmapdef_SVR_EVENT_ID_LIST::iterator > pairRange;
	pairRange = m_defSvrEventIDList.equal_range( eID );

	mmapdef_SVR_EVENT_ID_LIST::iterator it = pairRange.first;
	for ( ; it != pairRange.second; ++it )
	{
		sData.sTSKey = it->second;

		m_defRegSvrEvtIDList[sData.sTSKey.tcID] = sData;
	}
}

void CDboTSCQCtrl::UpdateSvrEventID( void )
{
	if ( IsError() ||
		 IsFailed() )
	{
		return;
	}

	if ( IsCSComunication() ||
		 IsClientWait() ||
		 IsSvrComAfterClientWait() )
	{
		return;
	}

	if ( NULL == m_pCurTSP )
	{
		return;
	}

	NTL_TS_TC_ID tcID = m_pCurTSP->GetID();

	mapdef_REGISTED_SVR_EVENT_ID_LIST::iterator it = m_defRegSvrEvtIDList.find( tcID );

	if ( it != m_defRegSvrEvtIDList.end() )
	{
		sSERVER_EVENT_ID_DATA& sData = it->second;

		UT_RcvSvrEvt( sData.uiEventID );
	}
}

void CDboTSCQCtrl::SetTransCameraCtrlRight( RwBool bTransCameraCtrlRight )
{
	m_bTransCameraCtrlRight = bTransCameraCtrlRight;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_GCond( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContGCond* pContGCond = (CDboTSContGCond*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContGCond->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContGCond->GetID();

	sCQRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );

	// 일반적인 조건 컨테이너는 클라이언트에서 먼저 실행하고 문제 없으면 서버로 검증을 요구한다
	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( tgID, tcID, GetParent()->GetRecv(), &sParam );
	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		CNtlTSLog::Log( "Can not progress the quest container. Info[%d,%d]. [%s]", tID, tcID, TS_CODE_TRACE() );
		return tsResult;
	}

	// 조건을 만족함
	if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContGCond->GetYesLinkID() );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", tID, tgID, tcID, pContGCond->GetYesLinkID(), TS_CODE_TRACE() );
			return NTL_TSRESULT_TYPE_ERROR;
		}

		// 서버에게 다음 단계로 진행해도 되는지 물어본다
		if (bDoAction)
		{
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step(tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData());
		}
	}
	// 현재는 조건을 만족하지는 못하나 다음에는 조건을 만족할 수 있음
	// 현재도 조건을 만족하지는 못하고 다음에도 절대 조건을 만족할 수 없는 경우
	else if ( (tsResult & NTL_TSRESULT_TYPE_COND_CAN_PROGRESS) || (tsResult & NTL_TSRESULT_TYPE_COND_CANT_PROGRESS) )
	{
		// 만약 No에 연결된 링크가 있다면 실행한다
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContGCond->GetNoLinkID() );
		if ( pNextCont )
		{
			// 서버에게 다음 단계로 진행해도 되는지 물어본다
			if (bDoAction)
			{
				unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

				UG_Avatar_TS_Confirm_Step(tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData());
			}
		}
	}

	return tsResult;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_GAct( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContGAct* pContGAct = (CDboTSContGAct*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContGAct->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContGAct->GetID();

	// 일반적인 액션 컨테이너는 서버에게 실행을 요청하고 응답을 받은 시점에서 실행을 한다
	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContGAct->GetNextLinkID() );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", tID, tgID, tcID, ((CDboTSContGAct*)m_pCurTSP)->GetNextLinkID(), TS_CODE_TRACE() );
		return NTL_TSRESULT_TYPE_ERROR;
	}

	// 서버에게 다음 단계로 진행해도 되는지 물어본다
	if (bDoAction)
	{
		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step(tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData());
	}

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_UserSel( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContUsrSel* pContUsr = (CDboTSContUsrSel*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContUsr->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContUsr->GetID();

	// 유저에게 유저 선택창 출력
	sTS_KEY sKey; sKey.Init();
	sKey.tID = tID;
	sKey.tgID = tgID;
	sKey.tcID = tcID;

	if ( bDoAction ) TU_ShowUserSelectionDialog( sKey, pContUsr );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_Reward( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContReward* pContRwd = (CDboTSContReward*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContRwd->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContRwd->GetID();

	sCQRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );

	// 보상 컨테이너는 클라이언트에서 먼저 실행하고 문제 없으면 서버로 검증을 요구한다
	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( tgID, tcID, GetParent()->GetRecv(), &sParam );
	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		CNtlTSLog::Log( "Can not progress the quest container. Info[%d,%d,%d]. [%s]", tID, tgID, tcID, TS_CODE_TRACE() );
		return tsResult;
	}

	// 조건을 만족함
	if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
	{
		// 유저에게 대화 출력.
		// 그리고, 유저가 주어진 보상중 하나를 선택해서 알려줘야 함
		sTS_KEY sKey; sKey.Init();
		sKey.tID = tID;
		sKey.tgID = tgID;
		sKey.tcID = tcID;

		if ( bDoAction )
		{
			MatchTimingData_PreReward();

			TU_ShowRewardDialog( sKey, pContRwd );
		}
	}

	return tsResult;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_Start( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContStart* pContStart = (CDboTSContStart*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContStart->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContStart->GetID();

	sCQRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );
	sParam.SetQuestShare( IsQuestShareMode() ? true : false );

	// 시작 컨테이너는 클라이언트에서 먼저 실행하고 문제 없으면 서버로 검증을 요구한다
	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( tgID, tcID, GetParent()->GetRecv(), &sParam );
	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		CNtlTSLog::Log( "Can not progress the quest container. Info[%d,%d,%d]. [%s]", tID, tgID, tcID, TS_CODE_TRACE() );
		return tsResult;
	}

	// 조건을 만족함
	if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContStart->GetYesLinkID() );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", tID, tgID, tcID, pContStart->GetYesLinkID(), TS_CODE_TRACE() );
			return NTL_TSRESULT_TYPE_ERROR;
		}

		// 서버에게 다음 단계로 진행해도 되는지 물어본다
		if (bDoAction)
		{
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { IsQuestShareMode() ? 0 : 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };
			UG_Avatar_TS_Confirm_Step(tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData());
		}
	}
	// 시작 컨테이너에서 시작을 다음으로 진행 할 수 없는 경우는 무조건 No를 실행한다
	else if ( (tsResult & NTL_TSRESULT_TYPE_COND_CAN_PROGRESS) || (tsResult & NTL_TSRESULT_TYPE_COND_CANT_PROGRESS) )
	{
		// 만약 No에 연결된 링크가 있다면 실행한다
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( tgID )->GetChildCont( pContStart->GetNoLinkID() );

		if ( pNextCont )
		{
			// 서버에게 다음 단계로 진행해도 되는지 물어본다
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { IsQuestShareMode() ? 0 : 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };
			if (bDoAction)
			{
				UG_Avatar_TS_Confirm_Step(tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData());
			}
		}
		else
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", tID, tgID, tcID, pContStart->GetNoLinkID(), TS_CODE_TRACE() );
			return NTL_TSRESULT_TYPE_ERROR;
		}
	}

	return tsResult;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_End( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContEnd* pContEnd = (CDboTSContEnd*)pCont;
	NTL_TS_T_ID tID = GetTrigger()->GetID();
	NTL_TS_TG_ID tgID = ((CNtlTSGroup*)pContEnd->GetParent())->GetID();
	NTL_TS_TG_ID tcID = pContEnd->GetID();

	// 서버에게 다음 단계로 진행해도 되는지 물어본다
	if (bDoAction)
	{
		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step(tcID, NTL_TS_TC_ID_INVALID, uiParam, GetEventType(), GetEventData());
	}

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_Narration( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContNarration* pContNarration = (CDboTSContNarration*)pCont;

	sTS_KEY sKey; sKey.Init();
	sKey.tID = GetTrigger()->GetID();
	sKey.tgID = ((CNtlTSGroup*)pContNarration->GetParent())->GetID();
	sKey.tcID = pContNarration->GetID();

	// 유저에게 나래이션 대화 출력.
	if ( bDoAction ) TU_ShowNarrationDialog( sKey, pContNarration );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_Proposal( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContProposal* pContProposal = (CDboTSContProposal*)pCont;

	sTS_KEY sKey; sKey.Init();
	sKey.tID = GetTrigger()->GetID();
	sKey.tgID = ((CNtlTSGroup*)pContProposal->GetParent())->GetID();
	sKey.tcID = pContProposal->GetID();

	CNtlTSCont* pRwd = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( pContProposal->GetReward() );

	if ( pRwd && pRwd->IsDerivedClass( "CDboTSContReward" ) )
	{
		if ( bDoAction ) TU_ShowProposalDialog( sKey, pContProposal, (CDboTSContReward*)pRwd );
	}
	else
	{
		if ( bDoAction ) TU_ShowProposalDialog( sKey, pContProposal, 0 );
	}

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_Switch( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContSwitch* pContSwitch = (CDboTSContSwitch*)pCont;

	NTL_TS_TG_ID tcID = pContSwitch->GetID();

	// 서버에게 다음 단계로 진행해도 되는지 물어본다
	if (bDoAction)
	{
		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step(tcID, NTL_TS_TC_ID_INVALID, uiParam, GetEventType(), GetEventData());
	}

	return NTL_TSRESULT_TYPE_SUCCESS;
}

NTL_TSRESULT CDboTSCQCtrl::Cont_UnifiedNarration( CNtlTSCont* pCont, bool bDoAction /*= true*/ )
{
	CDboTSContUnifiedNarration* pContNarration = (CDboTSContUnifiedNarration*)pCont;

	sTS_KEY sKey; sKey.Init();
	sKey.tID = GetTrigger()->GetID();
	sKey.tgID = ((CNtlTSGroup*)pContNarration->GetParent())->GetID();
	sKey.tcID = pContNarration->GetID();

	// 유저에게 나래이션 대화 출력.
	if ( bDoAction ) TU_ShowUnifiedNarrationDialog( sKey, pContNarration );

	return NTL_TSRESULT_TYPE_SUCCESS;
}

void CDboTSCQCtrl::UpdateLimitTime( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;

	sEXCEPTION_TIMER* pTimer = GetTimeLimitInfo();
	if ( pTimer )
	{
		if ( m_uiUpdateTimeCnt == pTimer->uiRemainTime / 1000 ) return;
		m_uiUpdateTimeCnt = pTimer->uiRemainTime / 1000;

		// 클라이언트 UI 업데이트
		((CDboTSCQAgency*)GetParent())->TU_UpdateQuestProgressInfoNfy( GetTrigger()->GetID(),
																	   GetServerEvtDataType(),
																	   GetServerEvtData(),
																	   GetLimitTime() );
	}
	else
	{
		m_uiUpdateTimeCnt = 0;
	}
}

void CDboTSCQCtrl::UpdateSuccessState( void )
{
	bool bRet = IsCompletedServerEvt();

	if ( bRet )
	{
		if ( !IsSuccess() ) SetSuccess( true );
	}
	else
	{
		if ( IsSuccess() ) SetSuccess( false );
	}
}

void CDboTSCQCtrl::UpdateClearState( void )
{
	if ( m_pCurTSP )
	{
		switch ( m_pCurTSP->GetEntityType() )
		{
		case DBO_CONT_TYPE_ID_CONT_GCOND:
			{
				CDboTSContGCond* pCondCont = (CDboTSContGCond*)m_pCurTSP;

				if ( pCondCont->IsRewardMark() )
				{
					sCQRUN_PARAM sParam;
					sParam.SetControl( this );
					sParam.SetAgency( GetParent() );
					sParam.SetQuestShare( true );

					NTL_TSRESULT tsResult = GetTrigger()->RunTarget( ((CNtlTSGroup*)m_pCurTSP->GetParent())->GetID(), m_pCurTSP->GetID(), GetParent()->GetRecv(), &sParam );

					if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
					{
						if ( !IsCleared() ) SetCleared( true );
					}
					else
					{
						if ( IsCleared() ) SetCleared( false );
					}
				}
				else
				{
					if ( IsCleared() ) SetCleared( false );
				}
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_START:
		case DBO_CONT_TYPE_ID_CONT_GACT:
			{
				if ( IsCleared() ) SetCleared( false );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_REWARD:
			{
				sCQRUN_PARAM sParam;
				sParam.SetControl( this );
				sParam.SetAgency( GetParent() );
				sParam.SetQuestShare( true );

				NTL_TSRESULT tsResult = GetTrigger()->RunTarget( ((CNtlTSGroup*)m_pCurTSP->GetParent())->GetID(), m_pCurTSP->GetID(), GetParent()->GetRecv(), &sParam );

				if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
				{
					if ( !IsCleared() ) SetCleared( true );
				}
				else
				{
					if ( IsCleared() ) SetCleared( false );
				}
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_END:
		case DBO_CONT_TYPE_ID_CONT_USERSEL:
		case DBO_CONT_TYPE_ID_CONT_NARRATION:
		case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
		case DBO_CONT_TYPE_ID_CONT_SWITCH:
		case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
			{
				if ( IsCleared() ) SetCleared( false );
			}
			break;

		default:
			{
				if ( IsCleared() ) SetCleared( false );
			}
			break;
		}
	}
	else
	{
		if ( !IsCleared() ) SetCleared( true );
	}
}

void CDboTSCQCtrl::UpdateTSStep( void )
{
	if ( IsError() ||
		 IsFailed() )
	{
		UnregNPCCameraStopEvt();
		return;
	}

	if ( IsCSComunication() ||
		 IsClientWait() ||
		 IsSvrComAfterClientWait() )
	{
		return;
	}

	// 현재 진행 중인 컨테이너가 없다는 것은 시작을 의미함
	if ( 0 == m_pCurTSP )
	{
		m_pCurTSP = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( START_CONTAINER_ID );
		if ( 0 == m_pCurTSP )
		{
			SetError();
			CNtlTSLog::Log( "Can not find the start container id. Info[%d]. [%s]", GetTrigger()->GetID(), TS_CODE_TRACE() );
			return;
		}
	}

	switch ( m_pCurTSP->GetEntityType() )
	{
	case DBO_CONT_TYPE_ID_CONT_GCOND:
		{
			if ( Cont_GCond( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_GACT:
		{
			if ( Cont_GAct( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_USERSEL:
		{
			if ( Cont_UserSel( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_REWARD:
		{
			if ( Cont_Reward( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_START:
		{
			if ( Cont_Start( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_END:
		{
			if ( Cont_End( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_NARRATION:
		{
			if ( Cont_Narration( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
		{
			if ( Cont_Proposal( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;

	case DBO_CONT_TYPE_ID_CONT_SWITCH:
		{
			if ( Cont_Switch( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;

	case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
		{
			if ( Cont_UnifiedNarration( m_pCurTSP ) & NTL_TSRESULT_TYPE_ERROR )
			{
				SetError();
				CNtlTSLog::Log( "The trigger running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), m_pCurTSP->GetID(), TS_CODE_TRACE() );
				return;
			}
		}
		break;
	}

	UpdateSuccessState();

	UpdateClearState();

	UpdateQuestMark();
}

void CDboTSCQCtrl::ChangeTSState( unsigned int uiChangFlag )
{
	// 클라이언트 UI 업데이트
	((CDboTSCQAgency*)GetParent())->TU_UpdateQuestStateNfy( GetTrigger()->GetID(), GetTrigger()->IsOutStateMsg(), uiChangFlag, GetCurState(), GetTitle(), GetServerEvtDataType(), GetServerEvtData() );

	if ( eTS_PROG_STATE_PROGRESS_FAILED == uiChangFlag )
	{
		if ( IsFailed() )
		{
			UnregNPCCameraStopEvt();

			// 서버에게 Failed 설정 상태를 전송한다
			UG_TS_Update_State( eTSSTATE_TYPE_ADD, eTS_SVR_STATE_FAILED );

			// 실패 상태에서 퀘스트의 등록 정보가 없으면
			// 퀘스트를 제거한다
			if ( NTL_TS_TC_ID_INVALID == m_tcQuestInfo || NTL_TS_TA_ID_INVALID == m_taQuestInfo )
			{
				SetExitState();
			}
		}
		else
		{
			// 서버에게 Failed 설정 상태를 전송한다
			UG_TS_Update_State( eTSSTATE_TYPE_REMOVE, eTS_SVR_STATE_FAILED );
		}
	}
	else if ( eTS_PROG_STATE_ERROR_BLOCK == uiChangFlag )
	{
		if ( IsError() )
		{
			UnregNPCCameraStopEvt();

			// 서버에게 에러 설정 상태를 전송한다
			UG_TS_Update_State( eTSSTATE_TYPE_ADD, eTS_SVR_STATE_ERROR );

			// 에러 상태에서 퀘스트의 등록 정보가 없으면
			// 퀘스트를 제거한다
			if ( NTL_TS_TC_ID_INVALID == m_tcQuestInfo || NTL_TS_TA_ID_INVALID == m_taQuestInfo )
			{
				SetExitState();
			}
		}
		else
		{
			// 서버에게 에러 설정 해재 상태를 전송한다
			UG_TS_Update_State( eTSSTATE_TYPE_REMOVE, eTS_SVR_STATE_ERROR );
		}
	}
	else if ( eTS_PROG_STATE_EXIT == uiChangFlag )
	{
		if ( IsExitState() )
		{
			CDboTSCQAgency* pAgency = (CDboTSCQAgency*)GetParent();

			bool bUnregCam = true;

			CDboTSEMFLink* pEvtMapper = (CDboTSEMFLink*)GetParent()->GetParent()->FindEventMapper("CDboTSEMFLink");
			if (pEvtMapper)
			{
				WORD wNextQuestID = pEvtMapper->GetNextQuestID(GetTrigger()->GetID());
				if (wNextQuestID != INVALID_WORD)
				{
					bool bHasBeenCleared = pAgency->GetCompletedQInfo()->HasBeenClearQuest(GetTrigger()->GetID());
					
					if (bHasBeenCleared)
						bUnregCam = false;
				}
			}

			if(bUnregCam)
				UnregNPCCameraStopEvt();

			// 클라이언트 UI 업데이트
			pAgency->TU_UnregQuestInfoNfy( GetTrigger()->GetID() );

			// 클라이언트에게 퀘스트 종료를 알림
			pAgency->TU_FinishQuest( TS_TYPE_QUEST_CS, GetTrigger()->GetID() );
		}
	}
	else if ( eTS_PROG_STATE_SUCCESS == uiChangFlag )
	{
		if ( IsSuccess() )
		{
			MatchTimingData_SuccessQuest();
		}
	}
}

void CDboTSCQCtrl::LoadQuestProgressInfo_V0( const sPROGRESS_QUEST_INFO::uDATA& uData )
{
	CDboTSQCtrl::LoadQuestProgressInfo_V0( uData );

	// At the time of loading,
	// Eliminate quests in case of error or failure
	if ( (NTL_TS_TC_ID_INVALID == m_tcQuestInfo || NTL_TS_TA_ID_INVALID == m_taQuestInfo) &&
		 (IsError() || IsFailed()) )
	{
		SetExitState();
		return;
	}

	SetCleared( false );

	NTL_TS_TG_ID tgExcCGroup = GetExcCGroup();

	if ( tgExcCGroup >= NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN &&
		 tgExcCGroup <= NTL_TS_EXCEPT_CLIENT_GROUP_ID_END )
	{
		AttachUpdateExceptionGroup( tgExcCGroup );
	}
	else
	{
		SetExcCGroup( NTL_TS_TG_ID_INVALID );
	}

	if ( NTL_TS_TC_ID_INVALID != m_tcQuestInfo && NTL_TS_TA_ID_INVALID != m_taQuestInfo )
	{
		CNtlTSCont* pCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( m_tcQuestInfo );
		if ( pCont )
		{
			CNtlTSEntity* pEntity = pCont->FindChildEntity( m_taQuestInfo );
			if ( pEntity && pEntity->IsDerivedClass( "CDboTSActRegQInfo" ) )
			{
				CDboTSActRegQInfo* pActQInfo = (CDboTSActRegQInfo*)pEntity;

				m_uiTitle = pActQInfo->GetTitle();
				m_uiArea = pActQInfo->GetArea();
				m_uiGoal = pActQInfo->GetGoal();
				m_uiGrade = pActQInfo->GetGrade();
				m_eGradeType = pActQInfo->GetGradeType();
				m_eSort = pActQInfo->GetSortType();
				m_uiSort = pActQInfo->GetSort();
				m_uiContents = pActQInfo->GetContents();

				CNtlTSCont* pContNew = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( pActQInfo->GetReward() );
				if (pContNew && pContNew->IsDerivedClass( "CDboTSContReward" ) )
				{
					CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
					NTL_ASSERT(pSobAvatar != NULL, "Not exist avatar instance");

					BYTE byAvatarClass = Logic_GetPlayerClass(pSobAvatar);

					((CDboTSContReward*)pContNew)->GetContRewardFinal(API_GetTableContainer(), byAvatarClass);

					m_pDefRwd = (sREWARD_INFO*)((CDboTSContReward*)pContNew)->GetDefRewardInfo();
					m_pSelRwd = (sREWARD_INFO*)((CDboTSContReward*)pContNew)->GetSelRewardInfo();
					
					m_uiRewardExp = ((CDboTSContReward*)pContNew)->GetRewardExp();
					m_uiRewardZenny = ((CDboTSContReward*)pContNew)->GetRewardZeny();
				}
				else
				{

					m_uiRewardExp = 0;
					m_uiRewardZenny = 0;
					m_pDefRwd = 0;
					m_pSelRwd = 0;
				}

				// 클라이언트 UI 업데이트
				((CDboTSCQAgency*)GetParent())->TU_RegQuestInfoNfy( GetTrigger()->GetID(), m_tcQuestInfo, m_taQuestInfo, false, GetTrigger()->IsShareQuest(), GetArea(), GetCurState(), m_uiTitle, GetGoal(), m_eSort );
			}
		}
	}

	// 클라이언트 UI 업데이트
	((CDboTSCQAgency*)GetParent())->TU_UpdateQuestStateNfy( GetTrigger()->GetID(), GetTrigger()->IsOutStateMsg(), GetCurState(), GetCurState(), GetTitle(), GetServerEvtDataType(), GetServerEvtData() );

	// 클라이언트 UI 업데이트
	((CDboTSCQAgency*)GetParent())->TU_UpdateQuestProgressInfoNfy( GetTrigger()->GetID(), GetServerEvtDataType(), GetServerEvtData(), GetLimitTime() );

	CNtlTSCont* pCurTSP = GetCurTSP();

	if ( pCurTSP && pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_NARRATION )
	{
		CDboTSContNarration* pNarration = (CDboTSContNarration*)pCurTSP;

		NTL_TS_TC_ID tcNextID;
		
		tcNextID = pNarration->GetLogInLink();
		if ( NTL_TS_TC_ID_INVALID != tcNextID )
		{
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step( pCurTSP->GetID(), tcNextID, uiParam, GetEventType(), GetEventData() );
		}
		else
		{
			tcNextID = pNarration->GetCancelLink();
			if ( NTL_TS_TC_ID_INVALID != tcNextID )
			{
				unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

				UG_Avatar_TS_Confirm_Step( pCurTSP->GetID(), tcNextID, uiParam, GetEventType(), GetEventData() );
			}
		}
	}

	if ( pCurTSP && pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_USERSEL )
	{
		CDboTSContUsrSel* pUS = (CDboTSContUsrSel*)pCurTSP;

		NTL_TS_TC_ID tcNextID;

		tcNextID = pUS->GetLogInLink();
		if ( NTL_TS_TC_ID_INVALID != tcNextID )
		{
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step( pCurTSP->GetID(), tcNextID, uiParam, GetEventType(), GetEventData() );
		}
		else
		{
			tcNextID = pUS->GetCancelLink();
			if ( NTL_TS_TC_ID_INVALID != tcNextID )
			{
				unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

				UG_Avatar_TS_Confirm_Step( pCurTSP->GetID(), tcNextID, uiParam, GetEventType(), GetEventData() );
			}
		}
	}

	if ( pCurTSP && pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION )
	{
		CDboTSContUnifiedNarration* pNarration = (CDboTSContUnifiedNarration*)pCurTSP;

		NTL_TS_TC_ID tcNextID;

		tcNextID = pNarration->GetLogInLink();
		if ( NTL_TS_TC_ID_INVALID != tcNextID )
		{
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step( pCurTSP->GetID(), tcNextID, uiParam, GetEventType(), GetEventData() );
		}
		else
		{
			tcNextID = pNarration->GetCancelLink();
			if ( NTL_TS_TC_ID_INVALID != tcNextID )
			{
				unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

				UG_Avatar_TS_Confirm_Step( pCurTSP->GetID(), tcNextID, uiParam, GetEventType(), GetEventData() );
			}
		}
	}
}

void CDboTSCQCtrl::UpdateQuestMark( void )
{
	if ( NULL == m_pCurTSP ) return;

	//////////////////////////////////////////////////////////////////////////
	//
	// 이벤터 등록
	//
	//////////////////////////////////////////////////////////////////////////

	// 1. 이벤트 엔티티를 가지고 있으면서

	CNtlTSEntity* pEntity = m_pCurTSP->GetEventEntity();

	if ( pEntity )
	{
		// 2. 퀘스트 마크 리스트에 등록이 안되어 있으면서

		QM_KEY Key = ((CDboTSCQAgency*)GetParent())->MakeQuestMarkKey( QMI_PROG_TYPE_EVENTER, GetTrigger()->GetID(), m_pCurTSP->GetID() );

		if ( !HasQuestMark( Key ) )
		{
			// 3. 이벤트와 액션 실행을 제외한 현재 컨테이너 실행이 참인 경우

			NTL_TS_TG_ID tgID = ((CNtlTSGroup*)m_pCurTSP->GetParent())->GetID();
			NTL_TS_TC_ID tcID = m_pCurTSP->GetID();

			sCQRUN_PARAM sParam;
			sParam.SetControl( this );
			sParam.SetAgency( GetParent() );
			sParam.SetForCheckQuestMark( true );

			NTL_TSRESULT tsResult = GetTrigger()->RunTarget( tgID, tcID, GetParent()->GetRecv(), &sParam );

			if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
			{
				// 4. 퀘스트 마크 리스트에 등록한다

				switch ( pEntity->GetEntityType() )
				{
				case DBO_EVENT_TYPE_ID_CLICK_NPC:
					{
						CDboTSClickNPC* pClickNPC = (CDboTSClickNPC*)pEntity;

						RwV3d* pPos = ((CDboTSCMain*)GetParent()->GetParent())->FindNPCPosition( GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx, pClickNPC->GetNPCIdx() );

						if ( pPos )
						{
							RegisterQuestMark_Eventer( Key,
													   eQMI_TARGET_TYPE_NPC,
													   GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx,
													   pClickNPC->GetNPCIdx(),
													   pPos->x,
													   pPos->y,
													   pPos->z,
													   GetTitle() );
						}
					}
					break;

				case DBO_EVENT_TYPE_ID_CLICK_OBJECT:
					{
						CDboTSClickObject* pClickObj = (CDboTSClickObject*)pEntity;

						unsigned int uiObjectIdx = pClickObj->BeginObjectIdx();
						while ( 0xffffffff != uiObjectIdx )
						{
							CObjectTable* pObjTbl = API_GetTableContainer()->GetObjectTable( pClickObj->GetWorldIdx() );

							sOBJECT_TBLDAT* pObjTblData = (sOBJECT_TBLDAT*)pObjTbl->FindData( uiObjectIdx );

							if ( pObjTblData )
							{
								RegisterQuestMark_Eventer( Key,
														   eQMI_TARGET_TYPE_OBJECT,
														   pClickObj->GetWorldIdx(),
														   uiObjectIdx,
														   pObjTblData->vLoc.x,
														   pObjTblData->vLoc.y,
														   pObjTblData->vLoc.z,
														   GetTitle() );
							}

							uiObjectIdx = pClickObj->NextObjectIdx();
						}
					}
					break;

				case DBO_EVENT_TYPE_ID_COL_OBJECT:
					{
						CDboTSColObject* pColObj = (CDboTSColObject*)pEntity;

						unsigned int uiObjectIdx = pColObj->BeginObjectIdx();
						while ( 0xffffffff != uiObjectIdx )
						{
							CObjectTable* pObjTbl = API_GetTableContainer()->GetObjectTable( pColObj->GetWorldIdx() );

							sOBJECT_TBLDAT* pObjTblData = (sOBJECT_TBLDAT*)pObjTbl->FindData( uiObjectIdx );

							if ( pObjTblData )
							{
								RegisterQuestMark_Eventer( Key,
														   eQMI_TARGET_TYPE_OBJECT,
														   pColObj->GetWorldIdx(),
														   uiObjectIdx,
														   pObjTblData->vLoc.x,
														   pObjTblData->vLoc.y,
														   pObjTblData->vLoc.z,
														   GetTitle() );
							}

							uiObjectIdx = pColObj->NextObjectIdx();
						}
					}
					break;

				case DBO_EVENT_TYPE_ID_ITEM_USE:
				case DBO_EVENT_TYPE_ID_ITEM_GET:
				case DBO_EVENT_TYPE_ID_ITEM_EQUIP:
				case DBO_EVENT_TYPE_ID_SCOUT_USE:
				case DBO_EVENT_TYPE_ID_RCV_SVR_EVT:
				case DBO_EVENT_TYPE_ID_SKILL_USE:
				case DBO_EVENT_TYPE_ID_COL_RGN:
				case DBO_EVENT_TYPE_ID_RB:
					{
						// 이 이벤트는 미니맵에 마크를 출력하지 않는다
					}
					break;
				}
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//
	// 마크 포지션 등록
	//
	//////////////////////////////////////////////////////////////////////////

	// 1. 퀘스트 정보가 등록 되어 있고

	if ( NTL_TS_TC_ID_INVALID != m_tcQuestInfo &&
		 NTL_TS_TA_ID_INVALID != m_taQuestInfo )
	{
		CNtlTSEntity* pEntityInfo = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( m_tcQuestInfo )->FindChildEntity( m_taQuestInfo );

		if ( pEntityInfo && pEntityInfo->GetEntityType() == DBO_ACT_TYPE_ID_ACT_REGQINFO )
		{
			QM_KEY Key = ((CDboTSCQAgency*)GetParent())->MakeQuestMarkKey( QMI_PROG_TYPE_MARK_POSITION, GetTrigger()->GetID(), m_tcQuestInfo );

			// 2. 퀘스트 마크 리스트에 등록이 안되어 있으면서 서버 이벤트가 진행 중인 경우

			if ( !HasQuestMark( Key ) && !IsCleared() )
			{
				// 3. 액션에 등록된 마크 포지션 정보를 등록한다

				CDboTSActRegQInfo* pQInfo = (CDboTSActRegQInfo*)pEntityInfo;

				for ( int i = 0; i < pQInfo->GetMaxNumOfQuestMarkInfo(); ++i )
				{
					const CDboTSActRegQInfo::SQUEST_MARK_INFO& sQMarkInfo = pQInfo->GetQuestMarkInfo( i );

					if ( 0xffffffff != sQMarkInfo.uiWorldTblIdx )
					{
						RegisterQuestMark_Position( Key,
													eQMI_TARGET_TYPE_POSITION,
													sQMarkInfo.uiWorldTblIdx,
													sQMarkInfo.fX,
													sQMarkInfo.fY,
													sQMarkInfo.fZ,
													sQMarkInfo.uiTooltipIdx );
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Visit 서버 이벤트 등록
	//
	//////////////////////////////////////////////////////////////////////////

	// 퀘스트 마크 업데이트
	if ( NTL_TS_TC_ID_INVALID != GetServerEvtContainerID() &&
		 NTL_TS_TA_ID_INVALID != GetServerEvtActionID() &&
		 eSTOC_EVT_DATA_TYPE_VISIT == GetServerEvtDataType() )
	{
		uSTOC_EVT_DATA& uEvtData = GetServerEvtData();

		for ( int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; ++i )
		{
			if ( 0xff == uEvtData.sVisitEvt[i].byObjType ) continue;

			if ( !uEvtData.sVisitEvt[i].bCompleted )
			{
				QM_KEY Key = ((CDboTSCQAgency*)GetParent())->MakeQuestMarkKey( QMI_PROG_TYPE_VISIT_SVR_EVENTER, GetTrigger()->GetID(), GetServerEvtContainerID(), uEvtData.sVisitEvt[i].byObjType, uEvtData.sVisitEvt[i].uiWorldTblIdx, uEvtData.sVisitEvt[i].uiObjTblIdx );

				if ( HasQuestMark( Key ) )
				{
					continue;
				}

				switch ( uEvtData.sVisitEvt[i].byObjType )
				{
				case OBJTYPE_NPC:
					{
						RwV3d* pPos = ((CDboTSCMain*)GetParent()->GetParent())->FindNPCPosition( uEvtData.sVisitEvt[i].uiWorldTblIdx, uEvtData.sVisitEvt[i].uiObjTblIdx );

						if ( NULL == pPos )
						{
							continue;
						}

						RegisterQuestMark_Eventer( Key,
												   eQMI_TARGET_TYPE_NPC,
												   uEvtData.sVisitEvt[i].uiWorldTblIdx,
												   uEvtData.sVisitEvt[i].uiObjTblIdx,
												   pPos->x,
												   pPos->y,
												   pPos->z,
												   GetTitle() );
					}
					break;

				case OBJTYPE_TOBJECT:
					{
						CObjectTable* pObjTbl = API_GetTableContainer()->GetObjectTable( uEvtData.sVisitEvt[i].uiWorldTblIdx );

						if ( NULL == pObjTbl )
						{
							continue;
						}

						sOBJECT_TBLDAT* pObjTblData = (sOBJECT_TBLDAT*)pObjTbl->FindData( uEvtData.sVisitEvt[i].uiObjTblIdx );

						if ( NULL == pObjTblData )
						{
							continue;
						}

						RegisterQuestMark_Eventer( Key,
												   eQMI_TARGET_TYPE_OBJECT,
												   uEvtData.sVisitEvt[i].uiWorldTblIdx,
												   uEvtData.sVisitEvt[i].uiObjTblIdx,
												   pObjTblData->vLoc.x,
												   pObjTblData->vLoc.y,
												   pObjTblData->vLoc.z,
												   GetTitle() );
					}
					break;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// 마크 리스트에서 등록된 엔티티 제거
	//
	//////////////////////////////////////////////////////////////////////////

	unsigned char byRmvQProgType;
	NTL_TS_T_ID rmvTId;
	NTL_TS_TC_ID rmvTCId;
	unsigned char byType;
	unsigned int uiUserData1;
	unsigned int uiUserData2;

	std::vector<QM_KEY> vecRmvList; vecRmvList.reserve( 10 );

	mapdef_QUESTMARKINFO_LIST::iterator itRmvQMIList = m_defQuestMarkInfoList.begin();
	for ( ; itRmvQMIList != m_defQuestMarkInfoList.end(); ++itRmvQMIList )
	{
		((CDboTSCQAgency*)GetParent())->SplitQuestMarkKey( itRmvQMIList->first, byRmvQProgType, rmvTId, rmvTCId, byType, uiUserData1, uiUserData2 );

		// 1. 이벤터 제거 - 이벤터의 경우 현재 진행중인 컨테이너 아이디와 저장되어 있는 컨테이너 아이디와
		//					같지 않은 경우 제거한다
		if ( QMI_PROG_TYPE_EVENTER == byRmvQProgType )
		{
			if ( m_pCurTSP->GetID() != rmvTCId )
			{
				vecRmvList.push_back( itRmvQMIList->first );
			}
		}

		// 2. 마크 포지션 제거
		else if ( QMI_PROG_TYPE_MARK_POSITION == byRmvQProgType )
		{
			// 1. 퀘스트 등록 정보가 존재하지 않는 경우는 무조건 제거한다

			if ( NTL_TS_TC_ID_INVALID == m_tcQuestInfo || NTL_TS_TA_ID_INVALID == m_taQuestInfo )
			{
				vecRmvList.push_back( itRmvQMIList->first );
			}

			// 2. 퀘스트 등록 정보를 담고 있는 컨테이너 아이디가 다르면 제거한다
			if ( m_tcQuestInfo != rmvTCId )
			{
				vecRmvList.push_back( itRmvQMIList->first );
			}

			// 3. 서버 이벤트가 완료 되었거나 존재하지 않는경우 제거한다

			if ( IsCleared() )
			{
				vecRmvList.push_back( itRmvQMIList->first );
			}
		}

		// 3. Visit 서버 이벤트 제거
		else if ( QMI_PROG_TYPE_VISIT_SVR_EVENTER == byRmvQProgType )
		{
			if ( eSTOC_EVT_DATA_TYPE_VISIT == GetServerEvtDataType() )
			{
				const QM_KEY& RmvQMKey = itRmvQMIList->first;

				uSTOC_EVT_DATA& uEvtData = GetServerEvtData();

				for ( int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; ++i )
				{
					if ( uEvtData.sVisitEvt[i].bCompleted &&
						 (uEvtData.sVisitEvt[i].byObjType == RmvQMKey.byType) &&
						 (uEvtData.sVisitEvt[i].uiWorldTblIdx == RmvQMKey.uiUserData1) &&
						 (uEvtData.sVisitEvt[i].uiObjTblIdx == RmvQMKey.uiUserData2) )
					{
						vecRmvList.push_back( itRmvQMIList->first );
						break;
					}
				}
			}
			else
			{
				vecRmvList.push_back( itRmvQMIList->first );
			}
		}
	}

	std::vector<QM_KEY>::iterator itRmvList = vecRmvList.begin();
	for ( ; itRmvList != vecRmvList.end(); ++itRmvList )
	{
		UnregisterQuestMark( *itRmvList );
	}
}

bool CDboTSCQCtrl::HasQuestMark( QM_KEY Key )
{
	return m_defQuestMarkInfoList.find( Key ) != m_defQuestMarkInfoList.end() ? true : false;
}

void CDboTSCQCtrl::RegisterQuestMark_Eventer( QM_KEY Key,
											  eQMI_TARGET_TYPE eTargetType,
											  unsigned int uiTargetWorldTbldx,
											  unsigned int uiTargetTableIdx,
											  float fTargetPosX,
											  float fTargetPosY,
											  float fTargetPosZ,
											  unsigned int uiTooltipTblIdx )
{
	SQuestMarkInfo sInfo;

	sInfo.eTargetType = eTargetType;
	sInfo.uiTargetWorldTblIdx = uiTargetWorldTbldx;
	sInfo.uiTargetTableIdx = uiTargetTableIdx;
	sInfo.fTargetPosX = fTargetPosX;
	sInfo.fTargetPosY = fTargetPosY;
	sInfo.fTargetPosZ = fTargetPosZ;
	sInfo.uiTooltipTblIdx = uiTooltipTblIdx;

	m_defQuestMarkInfoList.insert( mapdef_QUESTMARKINFO_LIST::value_type( Key, sInfo ) );

	CNtlSLEventGenerator::RegisterQuestPosMark( Key, eTargetType, uiTargetWorldTbldx, uiTargetTableIdx, fTargetPosX, fTargetPosY, fTargetPosZ, uiTooltipTblIdx );
}

void CDboTSCQCtrl::RegisterQuestMark_Position( QM_KEY Key,
											   eQMI_TARGET_TYPE eTargetType,
											   unsigned int uiTargetWorldTbldx,
											   float fTargetPosX,
											   float fTargetPosY,
											   float fTargetPosZ,
											   unsigned int uiTooltipTblIdx )
{
	SQuestMarkInfo sInfo;

	sInfo.eTargetType = eTargetType;
	sInfo.uiTargetWorldTblIdx = uiTargetWorldTbldx;
	sInfo.uiTargetTableIdx = 0xffffffff;
	sInfo.fTargetPosX = fTargetPosX;
	sInfo.fTargetPosY = fTargetPosY;
	sInfo.fTargetPosZ = fTargetPosZ;
	sInfo.uiTooltipTblIdx = uiTooltipTblIdx;

	m_defQuestMarkInfoList.insert( mapdef_QUESTMARKINFO_LIST::value_type( Key, sInfo ) );

	CNtlSLEventGenerator::RegisterQuestPosMark( Key, eTargetType, uiTargetWorldTbldx, 0xffffffff, fTargetPosX, fTargetPosY, fTargetPosZ, uiTooltipTblIdx );
}

void CDboTSCQCtrl::UnregisterQuestMark( QM_KEY Key )
{
	mapdef_QUESTMARKINFO_LIST::iterator it = m_defQuestMarkInfoList.find( Key );

	if ( it != m_defQuestMarkInfoList.end() )
	{
		m_defQuestMarkInfoList.erase( it );

		CNtlSLEventGenerator::UnregisterQuestPosMark( Key );
	}
}

unsigned char CDboTSCQCtrl::CheckUnregNPCCameraStop( CNtlTSCont* pNextCont, bool bFirstCall )
{
	unsigned char byNPCCamera = eNPC_CAMERA_DIR_TYPE_STOP;

	if ( NULL == pNextCont )
	{
		// Canceled camera production
		return eNPC_CAMERA_DIR_TYPE_STOP;
	}

	switch ( pNextCont->GetEntityType() )
	{
		case DBO_CONT_TYPE_ID_CONT_GCOND:
			{
				// GCond는 처음 호출 시점에서만 진행 가부를 판단하며
				// 이후 호출에서는 처음 호출이 되었을때 다시 판단한다
				if ( !bFirstCall ) return eNPC_CAMERA_DIR_TYPE_CONTINUE;

				NTL_TSRESULT tsResult = Cont_GCond( pNextCont, false );

				if ( tsResult & NTL_TSRESULT_TYPE_ERROR ) return eNPC_CAMERA_DIR_TYPE_STOP;

				if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
				{
					CNtlTSCont* pCont = GetTrigger()->GetGroup( ((CNtlTSGroup*)pNextCont->GetParent())->GetID() )->GetChildCont( ((CDboTSContGCond*)pNextCont)->GetYesLinkID() );
					if ( 0 == pCont )
					{
						// Canceled camera production
						return eNPC_CAMERA_DIR_TYPE_STOP;
					}
					else
					{
						byNPCCamera = CheckUnregNPCCameraStop( pCont, false );
						return byNPCCamera;
					}
				}
				else if ( (tsResult & NTL_TSRESULT_TYPE_COND_CAN_PROGRESS) || (tsResult & NTL_TSRESULT_TYPE_COND_CANT_PROGRESS) )
				{
					CNtlTSCont* pCont = GetTrigger()->GetGroup( ((CNtlTSGroup*)pNextCont->GetParent())->GetID() )->GetChildCont( ((CDboTSContGCond*)pNextCont)->GetNoLinkID() );
					if ( 0 == pCont )
					{
						// Canceled camera production
						return eNPC_CAMERA_DIR_TYPE_STOP;
					}
					else
					{
						byNPCCamera = CheckUnregNPCCameraStop( pCont, false );
						return byNPCCamera;
					}
				}
				else
				{
					// Canceled camera production
					return eNPC_CAMERA_DIR_TYPE_STOP;
				}
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_START:
			{
				// GStart는 처음 호출 시점에서만 진행 가부를 판단하며
				// 이후 호출에서는 처음 호출이 되었을때 다시 판단한다
				if ( !bFirstCall ) return eNPC_CAMERA_DIR_TYPE_CONTINUE;

				NTL_TSRESULT tsResult = Cont_Start( pNextCont, false );

				if ( tsResult & NTL_TSRESULT_TYPE_ERROR ) return eNPC_CAMERA_DIR_TYPE_STOP;

				if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
				{
					CNtlTSCont* pCont = GetTrigger()->GetGroup( ((CNtlTSGroup*)pNextCont->GetParent())->GetID() )->GetChildCont( ((CDboTSContStart*)pNextCont)->GetYesLinkID() );
					if ( 0 == pCont )
					{
						// Canceled camera production
						return eNPC_CAMERA_DIR_TYPE_STOP;
					}
					else
					{
						byNPCCamera = CheckUnregNPCCameraStop( pCont, false );
						return byNPCCamera;
					}
				}
				else if ( (tsResult & NTL_TSRESULT_TYPE_COND_CAN_PROGRESS) || (tsResult & NTL_TSRESULT_TYPE_COND_CANT_PROGRESS) )
				{
					CNtlTSCont* pCont = GetTrigger()->GetGroup( ((CNtlTSGroup*)pNextCont->GetParent())->GetID() )->GetChildCont( ((CDboTSContStart*)pNextCont)->GetNoLinkID() );
					if ( 0 == pCont )
					{
						// Canceled camera production
						return eNPC_CAMERA_DIR_TYPE_STOP;
					}
					else
					{
						byNPCCamera = CheckUnregNPCCameraStop( pCont, false );
						return byNPCCamera;
					}
				}
				else
				{
					// Canceled camera production
					return eNPC_CAMERA_DIR_TYPE_STOP;
				}
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_GACT:
			{
				bool bFind = false;
				int nSize = pNextCont->GetNumOfChildEntity();
				for ( int i = 0; i < nSize; ++i )
				{
					CNtlTSEntity* pEntity = pNextCont->GetChildEntity( i );

					if ( pEntity->GetEntityType() == DBO_ACT_TYPE_ID_ACT_NPCCONV ||
						 pEntity->GetEntityType() == DBO_ACT_TYPE_ID_ACT_OBJCONV )
					{
						bFind = true;
						break;
					}
				}

				if ( bFind ) return eNPC_CAMERA_DIR_TYPE_CONTINUE;

				CNtlTSCont* pCont = GetTrigger()->GetGroup( ((CNtlTSGroup*)pNextCont->GetParent())->GetID() )->GetChildCont( ((CDboTSContGAct*)pNextCont)->GetNextLinkID() );
				if ( 0 == pCont )
				{
					// Canceled camera production
					return eNPC_CAMERA_DIR_TYPE_STOP;
				}
				else
				{
					byNPCCamera = CheckUnregNPCCameraStop( pCont, false );
					return byNPCCamera;
				}
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_REWARD:
			{
				// Reward는 처음 호출 시점에서만 진행 가부를 판단하며
				// 이후 호출에서는 처음 호출이 되었을때 다시 판단한다
				if ( !bFirstCall ) return eNPC_CAMERA_DIR_TYPE_CONTINUE;

				NTL_TSRESULT tsResult = Cont_Reward( pNextCont, false );

				if ( tsResult & NTL_TSRESULT_TYPE_ERROR ) return eNPC_CAMERA_DIR_TYPE_STOP;

				if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
				{
					return eNPC_CAMERA_DIR_TYPE_CONTINUE;
				}
				else
				{
					// Canceled camera production
					return eNPC_CAMERA_DIR_TYPE_STOP;
				}
			}
			break;
			case DBO_CONT_TYPE_ID_CONT_END:
			{
				if (((CDboTSContEnd*)pNextCont)->GetEndType() == eEND_TYPE_NOT_PROGRESS)
					return eNPC_CAMERA_DIR_TYPE_STOP;

				bool bContinueCamera = false;

				CDboTSEMFLink* pEvtMapper = (CDboTSEMFLink*)GetParent()->GetParent()->FindEventMapper("CDboTSEMFLink");
				if (pEvtMapper)
				{
					WORD wNextQuestID = pEvtMapper->GetNextQuestID(GetTrigger()->GetID());
					if (wNextQuestID != INVALID_WORD)
					{
						CDboTSCQAgency* pAgency = (CDboTSCQAgency*)GetParent();

						bool bHasBeenCleared = pAgency->GetCompletedQInfo()->HasBeenClearQuest(GetTrigger()->GetID());

						pAgency->GetCompletedQInfo()->SetClearQuest(GetTrigger()->GetID(), true);

						// check if next quest can be accepted
						HOBJECT uiTargetSerial = Logic_GetAvatarTargetHandle();
						CNtlSob* pSobNPC = GetNtlSobManager()->GetSobObject(uiTargetSerial);
						if (pSobNPC)
						{
							TBLIDX npcIdx = Logic_GetNPCTriggerId(pSobNPC);
							if (npcIdx != INVALID_TBLIDX)
							{
								SGET_QUEST_INFO* pGetQuestList = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GetQuestInfoList(eEVENT_GEN_TYPE_CLICK_NPC, npcIdx);
								vecdef_QUEST_INFO_LIST* vQuestList = &pGetQuestList->QuestInfoList;
								vecdef_QUEST_INFO_LIST::iterator it;
									
								for (it = vQuestList->begin(); it != vQuestList->end(); ++it)
								{
									if ((*it).sKey.tID == wNextQuestID)
									{
										bContinueCamera = true;
										break;
									}
								}
							}
						}
							
						pAgency->GetCompletedQInfo()->SetClearQuest(GetTrigger()->GetID(), bHasBeenCleared);

						if (bContinueCamera)
							return eNPC_CAMERA_DIR_TYPE_CONTINUE;
					}
				}

				// Canceled camera production
				return eNPC_CAMERA_DIR_TYPE_STOP;
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_USERSEL:
		case DBO_CONT_TYPE_ID_CONT_NARRATION:
		case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
		case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
			{
				// Canceled camera production
				return eNPC_CAMERA_DIR_TYPE_CONTINUE;
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_SWITCH:
			{
				// Canceled camera production
				return eNPC_CAMERA_DIR_TYPE_STOP;
			}
			break;
		default:
			return eNPC_CAMERA_DIR_TYPE_STOP;
	}

	return byNPCCamera;
}

void CDboTSCQCtrl::BuildEventNPCInProgressQuest( std::map< unsigned int, unsigned int >& mapNPCIdxList )
{
	mapNPCIdxList.clear();

	CNtlTSGroup* pGroup = m_pTrigger->GetGroup( NTL_TS_MAIN_GROUP_ID );
	if ( NULL == pGroup ) return;

	CNtlTSCont* pCont = pGroup->GetBeginChildCont();

	while ( pCont )
	{
		if ( pCont->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
			 pCont->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND )
		{
			int nCnt = pCont->GetNumOfChildEntity();
			for ( int i = 0; i < nCnt; ++i )
			{
				CNtlTSEntity* pEntity = pCont->GetChildEntity( i );
				if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_NPC )
				{
					unsigned int uiNPCIdx = ((CDboTSClickNPC*)pEntity)->GetNPCIdx();
					mapNPCIdxList[uiNPCIdx] = uiNPCIdx;
				}
			}
		}

		pCont = pGroup->GetNextChildCont();
	}
}

void CDboTSCQCtrl::AddClientTimingData( eREG_C_TIMING eCTimingType, void* pData )
{
	mapdef_CLIENT_TIMING_DATA_LIST::iterator it = m_defCTimingDataList.find( eCTimingType );

	if ( it == m_defCTimingDataList.end() )
	{
		m_defCTimingDataList[eCTimingType] = pData;
	}
	else
	{
		void* pPtr = it->second;

		NTL_DELETE( pPtr );

		m_defCTimingDataList[eCTimingType] = pData;
	}
}

void CDboTSCQCtrl::ClearAllClientTimingData( void )
{
	mapdef_CLIENT_TIMING_DATA_LIST::iterator itCTData = m_defCTimingDataList.begin();
	for ( ; itCTData != m_defCTimingDataList.end(); ++itCTData )
	{
		void* pPtr = itCTData->second;

		NTL_DELETE( pPtr );
	}
	m_defCTimingDataList.clear();

	m_eCTimingExcType = eREG_C_TIMING_EXCUTE_TYPE_INVALID;
	memset( &m_uCTimingExcData, 0xffffffff, sizeof(uREG_C_TIMING_EXCUTE_DATA) );
}

void CDboTSCQCtrl::SetTimingExecutionData( eREG_C_TIMING_EXCUTE_TYPE eType, uREG_C_TIMING_EXCUTE_DATA& uData )
{
	m_eCTimingExcType = eType;
	memcpy( &m_uCTimingExcData, &uData, sizeof( uREG_C_TIMING_EXCUTE_DATA ) );
}

void CDboTSCQCtrl::DoClientTimingAction( void )
{
	switch ( m_eCTimingExcType )
	{
	case eREG_C_TIMING_EXCUTE_TYPE_MINI_NARRATION:
		{
			CNtlSLEventGenerator::BroadMsgNfy_MiniNarration( m_uCTimingExcData.sMiniNarration.uiTblIdx );
		}
		break;
	}
}

bool CDboTSCQCtrl::IfInterestingDoExceptionGroup( NTL_TS_TG_ID tgId ) const
{
	if ( NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN <= tgId && tgId <= NTL_TS_EXCEPT_CLIENT_GROUP_ID_END ) return true;
	return false;
}

void CDboTSCQCtrl::UG_Avatar_TS_Confirm_Step( NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, unsigned int* uiParam, unsigned char byEventType, unsigned int uiEventData )
{
	SetCSComunication();

	((CDboTSCQAgency*)GetParent())->UG_Avatar_TS_Confirm_Step( GetTrigger()->GetID(), tcCurId, tcNextId, uiParam, byEventType, uiEventData );
}

void CDboTSCQCtrl::GU_Avatar_TS_Confirm_Step( WORD wResultCode, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId )
{
	SetCSComunication( false );

	if (wResultCode >= GAME_TS_ERROR_RUN_ERROR && wResultCode <= GAME_TS_ERROR_TRIGGER_OBJECT_INVALID_FUNCFLAG)
	{
		SetError();
		CNtlTSLog::Log( "Quest confirm step error. Info[%d,%d,%d]. [%s]", wResultCode, tcCurId, tcNextId, TS_CODE_TRACE() );
		return;
	}
	else if (wResultCode >= GAME_TS_WARNING_NOT_SATISFY_REWARD_CONDITIONS && wResultCode <= GAME_TS_WARNING_NOT_YET_PROCESSED)
	{
		CNtlTSCont* pCurCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcCurId );

		if ( pCurCont->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GACT )
		{
			CDboTSContGAct* pCont = (CDboTSContGAct*)pCurCont;
			CNtlTSCont* pErrorCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( pCont->GetErrorLinkID() );

			if ( pErrorCont )
			{
				CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, "DST_QUEST_LINK_ACTION_ERROR", FALSE );

				unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

				UG_Avatar_TS_Confirm_Step( tcCurId, pErrorCont->GetID(), uiParam, GetEventType(), GetEventData() );

				return;
			}
			else
			{
				SetError();

				CNtlTSLog::Log( "Quest confirm step error. Info[%d,%d,%d]. [%s]", wResultCode, tcCurId, tcNextId, TS_CODE_TRACE() );

				return;
			}
		}
		else
		{
			UnregNPCCameraStopEvt();

			return;
		}
	}
	else if ( RESULT_SUCCESS != wResultCode )
	{
		SetError();
		CNtlTSLog::Log( "Wrong result code. Info[%d,%d,%d]. [%s]", wResultCode, tcCurId, tcNextId, TS_CODE_TRACE() );
		return;
	}

	CNtlTSCont* pCurCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcCurId );
	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcNextId );

	if ( 0 == pCurCont )
	{
		SetError();
		CNtlTSLog::Log( "Can not progress the quest. Info[%d,%d]. [%s]", GetTrigger()->GetID(), pCurCont, TS_CODE_TRACE() );
		return;
	}

	// 현재 컨테이너 실행
	sCQRUN_PARAM sParam;
	sParam.SetControl( this );
	sParam.SetAgency( GetParent() );

	NTL_TSRESULT tsResult = 0;

	switch ( pCurCont->GetEntityType() )
	{
	case DBO_CONT_TYPE_ID_CONT_GCOND:
		{
			// 선 검사를 하므로 이곳에서 트리거를 Run 할 필요 없음
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_GACT:
		{
			// 에러 상태로 분기시에는 액션을 실행하지 않는다
			if ( pNextCont->GetID() != ((CDboTSContGAct*)pCurCont)->GetErrorLinkID() )
			{
				tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, pCurCont->GetID(), GetParent()->GetRecv(), &sParam );
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_USERSEL:
		{
			// 실행 시킬 엔티티가 존재하지 않음
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_REWARD:
		{
			// 선 검사를 하므로 이곳에서 트리거를 Run 할 필요 없음

			// 유저가 확인을 누른 경우...
			if ( ((CDboTSContReward*)pCurCont)->GetNextLinkID() == pNextCont->GetID() )
			{
				// 사용자에게 유저가 보상을 받았음을 알려줌
				TU_AcceptReward( NTL_TS_MAIN_GROUP_ID, tcCurId, GetTitle(), GetTrigger()->IsOutStateMsg() );

				MatchTimingData_PostReward();
			}
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_START:
		{
			// 선 검사를 하므로 이곳에서 트리거를 Run 할 필요 없음
			SetCleared( false );

			BuildEventNPCInProgressQuest( m_mapNPCIdxList );
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_END:
		{
			// no entity to execute

			// Record quest completion status
			if ( eEND_TYPE_COMPLETE == ((CDboTSContEnd*)pCurCont)->GetEndType() )
			{
				CDboTSCQAgency* pAgency = (CDboTSCQAgency*)GetParent();
				pAgency->GetCompletedQInfo()->SetClearQuest( GetTrigger()->GetID(), true );

				// Start next quest.. Im not sure if this is well done. Might need to be updated ! BY DANEOS
				CDboTSEMFLink* pEvtMapper = (CDboTSEMFLink*)GetParent()->GetParent()->FindEventMapper("CDboTSEMFLink");
				if (pEvtMapper)
				{
					WORD wNextQuestID = pEvtMapper->GetNextQuestID(GetTrigger()->GetID());
					if (wNextQuestID != INVALID_WORD)
					{
						HOBJECT uiTargetSerial = Logic_GetAvatarTargetHandle();
						CNtlSob* pSobNPC = GetNtlSobManager()->GetSobObject(uiTargetSerial);
						if (pSobNPC)
						{
							TBLIDX npcIdx = Logic_GetNPCTriggerId(pSobNPC);
							if (npcIdx != INVALID_TBLIDX)
							{
								SGET_QUEST_INFO* pGetQuestList = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GetQuestInfoList(eEVENT_GEN_TYPE_CLICK_NPC, npcIdx);
								vecdef_QUEST_INFO_LIST* vQuestList = &pGetQuestList->QuestInfoList;
								vecdef_QUEST_INFO_LIST::iterator it;

								for (it = vQuestList->begin(); it != vQuestList->end(); ++it)
								{
									if ((*it).sKey.tID == wNextQuestID)
									{
										CNtlSLEventGenerator::DoQuest(*it, eEVENT_GEN_TYPE_CLICK_NPC, npcIdx, TRUE);
										break;
									}
								}
							}
						}
					}
				}
			}

			SetExitState();
			Logic_SendTutorialCondition( ETL_CONDITION_TYPE_QUEST_FINISH, GetTrigger()->GetID() );
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_NARRATION:
		{
			// 실행 시킬 엔티티가 존재하지 않음
		}
		break;
	case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
		{
			// 실행 시킬 엔티티가 존재하지 않음

			// 사용자에게 유저가 퀘스트를 정상적으로 수락했음을 알려줌
			CDboTSContProposal* pCurCont = (CDboTSContProposal*)GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcCurId );
			if ( NTL_TS_TC_ID_INVALID != tcNextId && pCurCont && pCurCont->GetOkLink() == tcNextId )
			{
				TU_AcceptProposal( NTL_TS_MAIN_GROUP_ID, tcCurId );

				MatchTimingData_AcceptClick();
			}
		}
		break;

	case DBO_CONT_TYPE_ID_CONT_SWITCH:
		{
			// 실행 시킬 엔티티가 존재하지 않음
		}
		break;

	case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
		{
			// 실행 시킬 엔티티가 존재하지 않음
		}
		break;
	}

	if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
	{
		SetError();
		CNtlTSLog::Log( "The quest running error. Info[%d,%d]. [%s]", GetTrigger()->GetID(), tsResult, TS_CODE_TRACE() );
		return;
	}

	// Processing for NPC Forced Camera Movement
	if ( pNextCont && !IsSvrComAfterClientWait() )
	{
		unsigned char byResult = CheckUnregNPCCameraStop( pNextCont, true );

		if ( byResult == eNPC_CAMERA_DIR_TYPE_STOP )
		{
			UnregNPCCameraStopEvt();
		}
	}

	// 다음 컨테이너로 TSP를 이동한다
	MoveTSP( pCurCont, pNextCont, true );
	
	// disable by daneos: to avoid "mob hunt complete" and other quests shown multiple times.
	//((CDboTSCQAgency*)GetParent())->TU_UpdateQuestStateNfy( GetTrigger()->GetID(), GetTrigger()->IsOutStateMsg(), GetCurState(), GetCurState(), GetTitle(), GetServerEvtDataType(), GetServerEvtData() );
}

void CDboTSCQCtrl::UG_Avatar_TS_GiveUp_Quest( void )
{
	((CDboTSCQAgency*)GetParent())->UG_Avatar_TS_GiveUp_Quest( GetTrigger()->GetID() );

	SetCSComunication();
}

void CDboTSCQCtrl::GU_Avatar_TS_GiveUp_Quest( WORD wResultCode )
{
	SetCSComunication( false );

	SetExitState();

	if ( RESULT_SUCCESS == wResultCode )
	{
		TU_AcceptGiveUp( NTL_TS_MAIN_GROUP_ID, GetTitle() );
	}
}

void CDboTSCQCtrl::GU_Avatar_TS_SToC_Event_Start_NFY( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId )
{
	// Local SToC event 시작
	sSTOC_EVT_DB_DATA sSvrEvtData; sSvrEvtData.Init();
	sSvrEvtData.tcId = tcId;
	sSvrEvtData.taId = taId;
	SetSToCEvtDBData( sSvrEvtData, true );

	// 클라이언트 UI 업데이트
	((CDboTSCQAgency*)GetParent())->TU_UpdateQuestProgressInfoNfy( GetTrigger()->GetID(), GetServerEvtDataType(), GetServerEvtData(), GetLimitTime() );
}

void CDboTSCQCtrl::GU_Avatar_TS_SToC_Event_End_NFY( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId )
{
	// Local SToC event 종료
	sSTOC_EVT_DB_DATA sSvrEvtData; sSvrEvtData.Init();
	SetSToCEvtDBData( sSvrEvtData, false );

	// 클라이언트 UI 업데이트
	((CDboTSCQAgency*)GetParent())->TU_UpdateQuestProgressInfoNfy( GetTrigger()->GetID(), GetServerEvtDataType(), GetServerEvtData(), GetLimitTime() );
}

void CDboTSCQCtrl::GU_Avatar_TS_SToC_Event_Update_NFY( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, BYTE bySvrEvtType, BYTE bySlot, uSTOC_EVT_UPDATE_DATA& uEvtData )
{
	if ( m_sSToCEvtData.tcId != tcId ||
		 m_sSToCEvtData.taId != taId )
	{
		CNtlTSLog::Log( "Wrong SToC event tc or ta id. Info[%d, %d, %d, %d, %d]. [%s]", GetTrigger()->GetID(), m_sSToCEvtData.tcId, tcId, m_sSToCEvtData.taId, taId, TS_CODE_TRACE() );
		SetError( true );
		return;
	}

	if ( GetServerEvtDataType() != bySvrEvtType )
	{
		CNtlTSLog::Log( "Wrong SToC event type. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), GetServerEvtDataType(), bySvrEvtType, TS_CODE_TRACE() );
		SetError( true );
		return;
	}

	uSTOC_EVT_DATA& uSToCEvtData = GetServerEvtData();

	switch ( bySvrEvtType )
	{
	case eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT:
		{
			if ( bySlot >= uSTOC_EVT_DATA::MAX_MOB_KILL )
			{
				CNtlTSLog::Log( "Wrong SToC event slot id. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, bySlot, TS_CODE_TRACE() );
				SetError( true );
				return;
			}

			uSToCEvtData.sMobKillCnt[bySlot].nCurMobCnt = uEvtData.sMobKillCnt.nCurMobCnt;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
		{
			if ( bySlot >= uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM )
			{
				CNtlTSLog::Log( "Wrong SToC event slot id. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, bySlot, TS_CODE_TRACE() );
				SetError( true );
				return;
			}

			uSToCEvtData.sMobKillItemCnt[bySlot].nCurMobLICnt = uEvtData.sMobKillItemCnt.nCurMobLICnt;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
		{
			if ( bySlot >= uSTOC_EVT_DATA::MAX_DELIVERY_ITEM )
			{
				CNtlTSLog::Log( "Wrong SToC event slot id. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, bySlot, TS_CODE_TRACE() );
				SetError( true );
				return;
			}

			uSToCEvtData.sDeliveryItemCnt[bySlot].nCurItemCnt = uEvtData.sDeliveryItemCnt.nCurItemCnt;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
		{
			if ( bySlot >= uSTOC_EVT_DATA::MAX_OBJECT_ITEM )
			{
				CNtlTSLog::Log( "Wrong SToC event slot id. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, bySlot, TS_CODE_TRACE() );
				SetError( true );
				return;
			}

			uSToCEvtData.sObjectItemCnt[bySlot].nCurItemCnt = uEvtData.sObjectItemCnt.nCurItemCnt;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT:
		{
			if ( bySlot >= uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM )
			{
				CNtlTSLog::Log( "Wrong SToC event slot id. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, bySlot, TS_CODE_TRACE() );
				SetError( true );
				return;
			}

			uSToCEvtData.sPublicMobItemCnt[bySlot].nCurItemCnt = uEvtData.sPublicMobItemCnt.nCurItemCnt;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT:
		{
			if ( bySlot >= uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT )
			{
				CNtlTSLog::Log( "Wrong SToC event slot id. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, bySlot, TS_CODE_TRACE() );
				SetError( true );
				return;
			}

			uSToCEvtData.sCustomEvtCnt[bySlot].nCurCnt = uEvtData.sCustomEvtCnt.nCurCnt;
		}
		break;
	case eSTOC_EVT_DATA_TYPE_VISIT:
		{
			if ( bySlot >= uSTOC_EVT_DATA::MAX_VISIT_EVT )
			{
				CNtlTSLog::Log( "Wrong SToC event slot id. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, bySlot, TS_CODE_TRACE() );
				SetError( true );
				return;
			}

			uSToCEvtData.sVisitEvt[bySlot].bCompleted = uEvtData.sVisitEvt.bCompleted;
		}
		break;

	default:
		CNtlTSLog::Log( "Wrong SToC event type. Info[%d, %d]. [%s]", GetTrigger()->GetID(), bySvrEvtType, TS_CODE_TRACE() );
		return;
	}

	// 클라이언트 UI 업데이트
	((CDboTSCQAgency*)GetParent())->TU_UpdateQuestProgressInfoNfy( GetTrigger()->GetID(), GetServerEvtDataType(), GetServerEvtData(), GetLimitTime() );
}

void CDboTSCQCtrl::GU_TS_Update_State( unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	unsigned int uiUpdatedQuestFlag = 0;

	switch ( byType )
	{
	case eTSSTATE_TYPE_ADD:
		{
			if ( wTSState & eTS_SVR_STATE_ERROR ) { UnregNPCCameraStopEvt(); SetError( true, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_ERROR_BLOCK; }
			if ( wTSState & eTS_SVR_STATE_FAILED ) { UnregNPCCameraStopEvt(); SetFailed( true, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_PROGRESS_FAILED; }
			if ( wTSState & eTS_SVR_STATE_ESCORT ) { SetEscort( true, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_ESCORT; }
			if ( wTSState & eTS_SVR_STATE_SVR_EVT_WORLD_RANGE ) { SetSvrEvtWorldRange( true, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_SVR_EVT_WORLD_RANGE; }
			if ( wTSState & eTS_SVR_STATE_WPS_MODE ) { SetWPSMode( true, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_WPS_MODE; }
			if ( wTSState & eTS_SVR_STATE_USER_OUT_FAILED_WPS ) { SetUserOutFailedWPS( true, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_USER_OUT_FAILED_WPS; }
		}
		break;
	case eTSSTATE_TYPE_REMOVE:
		{
			if ( wTSState & eTS_SVR_STATE_ERROR ) { SetError( false, false );  uiUpdatedQuestFlag |= eTS_PROG_STATE_ERROR_BLOCK; }
			if ( wTSState & eTS_SVR_STATE_FAILED ) { SetFailed( false, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_PROGRESS_FAILED; }
			if ( wTSState & eTS_SVR_STATE_ESCORT ) { SetEscort( false, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_ESCORT; }
			if ( wTSState & eTS_SVR_STATE_SVR_EVT_WORLD_RANGE ) { SetSvrEvtWorldRange( false, false );  uiUpdatedQuestFlag |= eTS_PROG_STATE_SVR_EVT_WORLD_RANGE; }
			if ( wTSState & eTS_SVR_STATE_WPS_MODE ) { SetWPSMode( false, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_WPS_MODE; }
			if ( wTSState & eTS_SVR_STATE_USER_OUT_FAILED_WPS ) { SetUserOutFailedWPS( false, false ); uiUpdatedQuestFlag |= eTS_PROG_STATE_USER_OUT_FAILED_WPS; }
		}
		break;
	}

	// 에러 상태에서 퀘스트의 등록 정보가 없으면
	// 퀘스트를 제거한다
	if ( (NTL_TS_TC_ID_INVALID == m_tcQuestInfo || NTL_TS_TA_ID_INVALID == m_taQuestInfo) &&
		 (IsError() || IsFailed() ) )
	{
		SetExitState();
	}
	else
	{
		// 클라이언트 UI 업데이트
		((CDboTSCQAgency*)GetParent())->TU_UpdateQuestStateNfy( GetTrigger()->GetID(), GetTrigger()->IsOutStateMsg(), uiUpdatedQuestFlag, GetCurState(), GetTitle(), GetServerEvtDataType(), GetServerEvtData() );
	}
}

void CDboTSCQCtrl::UG_TS_Update_State( unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	((CDboTSCQAgency*)GetParent())->UG_TS_Update_State( GetTrigger()->GetID(), byType, wTSState, uiParam );
}

void CDboTSCQCtrl::TU_AcceptProposal( NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId )
{
	((CDboTSCQAgency*)GetParent())->TU_AcceptProposal( GetTrigger()->GetID(), tgId, tcId );
}

void CDboTSCQCtrl::TU_AcceptReward( NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId, unsigned int uiQTitle, bool bOutStateMsg )
{
	((CDboTSCQAgency*)GetParent())->TU_AcceptReward( GetTrigger()->GetID(), tgId, tcId, uiQTitle, bOutStateMsg );

	SetComplete( true );
}

void CDboTSCQCtrl::TU_AcceptGiveUp( NTL_TS_TG_ID tgId, unsigned int uiQTitle )
{
	((CDboTSCQAgency*)GetParent())->TU_AcceptGiveUp( GetTrigger()->GetID(), tgId, uiQTitle );
}

void CDboTSCQCtrl::UT_GiveUpQuest( void )
{
	if ( !IsCSComunication() )
	{
		UG_Avatar_TS_GiveUp_Quest();
	}
}

void CDboTSCQCtrl::UT_ClickObject( unsigned int uiObjId )
{
	RegNPCCameraStopEvt();

	if (IsError() ||
		IsFailed() ||
		IsCSComunication() ||
		IsClientWait() ||
		IsSvrComAfterClientWait())
	{
		UnregNPCCameraStopEvt();
		return;
	}

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_OBJECT );
	SetEventGenId( uiObjId );

	SetEventData( eEVENT_GEN_TYPE_CLICK_OBJECT, uiObjId );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_CollisionObject( unsigned int uiObjId )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_COL_OBJECT );
	SetEventGenId( uiObjId );

	SetEventData( eEVENT_GEN_TYPE_COL_OBJECT, uiObjId );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ClickNPC( unsigned int uiNPCId )
{
	RegNPCCameraStopEvt();

	if ( IsError() ||
		 IsFailed() ||
		 IsCSComunication() ||
		 IsClientWait() ||
		 IsSvrComAfterClientWait() )
	{
		UnregNPCCameraStopEvt();
		return;
	}

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_NPC );
	SetEventGenId( uiNPCId );

	SetEventData( eEVENT_GEN_TYPE_CLICK_NPC, uiNPCId );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ItemUse( unsigned int uiItemIdx, vecdef_ItemTargetList* pdefItemTargetList )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_USE );
	SetEventGenId( uiItemIdx );
	SetEventGenItemTargetInfo( pdefItemTargetList );

	SetEventData( eEVENT_GEN_TYPE_ITEM_USE, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ItemGet( unsigned int uiItemIdx )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_GET );
	SetEventGenId( uiItemIdx );

	SetEventData( eEVENT_GEN_TYPE_ITEM_GET, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ItemEquip( unsigned int uiItemIdx )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_EQUIP );
	SetEventGenId( uiItemIdx );

	SetEventData( eEVENT_GEN_TYPE_ITEM_EQUIP, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ScoutUse( unsigned int uiItemIdx, SScouterUseTargetInfo* pTargetInfo )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SCOUT_USE );
	SetEventGenId( uiItemIdx );
	SetEventGenScouterUseTargetInfo( pTargetInfo );

	SetEventData( eEVENT_GEN_TYPE_SCOUT_USE, uiItemIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_RcvSvrEvt( unsigned int uiEvtID )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_RCV_SVR_EVT );
	SetEventGenId( uiEvtID );

	SetEventData( eEVENT_GEN_TYPE_RCV_SVR_EVT, uiEvtID );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_SkillUse( unsigned int uiSkillIdx, SSkillUseInfo* psSkillUseInfo )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SKILL_USE );
	SetEventGenId( uiSkillIdx );
	SetEventGenSkillUseInfo( psSkillUseInfo );

	SetEventData( eEVENT_GEN_TYPE_SKILL_USE, uiSkillIdx );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ColRgn( sCOL_RGN_DATA* pColRgnData )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_COL_REGION );
	SetEventGenColRgnInfo( pColRgnData );

	SetEventData( eEVENT_GEN_TYPE_COL_REGION, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_RB( unsigned int uiWorldIdx, SRB_DATA* pRBData )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_RB );
	SetEventGenId( uiWorldIdx );
	SetEventGenRBData( pRBData );

	SetEventData( eEVENT_GEN_TYPE_RB, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ClickMob( unsigned int uiMobGroupID, SMobTargetInfo* pTargetInfo )
{
	RegNPCCameraStopEvt();

	if ( IsError() ||
		 IsFailed() ||
		 IsCSComunication() ||
		 IsClientWait() ||
		 IsSvrComAfterClientWait() )
	{
		UnregNPCCameraStopEvt();
		return;
	}

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_MOB );
	SetEventGenId( uiMobGroupID );
	SetEventGenMobTargetData( pTargetInfo );

	SetEventData( eEVENT_GEN_TYPE_CLICK_MOB, pTargetInfo->hTarget );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_BindStone( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_BIND_STONE );

	SetEventData( eEVENT_GEN_TYPE_BIND_STONE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_SearchQuest( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SEARCH_QUEST );

	SetEventData( eEVENT_GEN_TYPE_SEARCH_QUEST, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ItemUpgrade( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_UPGRADE );

	SetEventData( eEVENT_GEN_TYPE_ITEM_UPGRADE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_Teleport( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_TELEPORT );

	SetEventData( eEVENT_GEN_TYPE_TELEPORT, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_Budokai( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_BUDOKAI );

	SetEventData( eEVENT_GEN_TYPE_BUDOKAI, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_SlotMachine( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_SLOTMACHINE );

	SetEventData( eEVENT_GEN_TYPE_SLOTMACHINE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_HoipoiMix( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_HOIPOIMIX );

	SetEventData( eEVENT_GEN_TYPE_HOIPOIMIX, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_PrivateShop( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_PRIVATESHOP );

	SetEventData( eEVENT_GEN_TYPE_PRIVATESHOP, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_FreeBattle( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_FREEBATTLE );

	SetEventData( eEVENT_GEN_TYPE_FREEBATTLE, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ItemIdentity( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_ITEMIDENTITY );

	SetEventData( eEVENT_GEN_TYPE_ITEMIDENTITY, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_UseMail( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_USEMAIL );

	SetEventData( eEVENT_GEN_TYPE_USEMAIL, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_Party()
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;
	if ( IsCSComunication() ) return;
	if ( IsClientWait() ) return;
	if ( IsSvrComAfterClientWait() ) return;

	SetEventGenType( eEVENT_GEN_TYPE_PARTY );

	SetEventData( eEVENT_GEN_TYPE_PARTY, 0xffffffff );

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_ClickSideIcon()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_CLICK_SIDEICON);

	SetEventData(eEVENT_GEN_TYPE_CLICK_SIDEICON, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_LevelCheck()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_LEVELCHECK);

	SetEventData(eEVENT_GEN_TYPE_LEVELCHECK, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_Quest()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_QUEST);

	SetEventData(eEVENT_GEN_TYPE_QUEST, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::UT_DialogOpen()
{
	if (IsError()) return;
	if (IsFailed()) return;
	if (IsCSComunication()) return;
	if (IsClientWait()) return;
	if (IsSvrComAfterClientWait()) return;

	SetEventGenType(eEVENT_GEN_TYPE_DIALOGOPEN);

	SetEventData(eEVENT_GEN_TYPE_DIALOGOPEN, 0xffffffff);

	UpdateTSStep();

	ClearAllEventGenInfo();
}

void CDboTSCQCtrl::TU_ShowProposalDialog( sTS_KEY& sKey, CDboTSContProposal* pCont, CDboTSContReward* pContRwd )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SetClientWait();

	((CDboTSCQAgency*)GetParent())->TU_ShowProposalDialog( sKey, pCont, pContRwd );
}

void CDboTSCQCtrl::UT_ShowProposalDialog( sTS_KEY& sKey, bool bAccept )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_PROPOSAL )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	NTL_TS_TC_ID tcNextID = bAccept ? ((CDboTSContProposal*) pCont)->GetOkLink() : ((CDboTSContProposal*) pCont)->GetCancelLink();

	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcNextID );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcNextID, TS_CODE_TRACE() );
		SetError();
		return;
	}

	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( sKey.tcID, tcNextID, uiParam, GetEventType(), GetEventData() );
}

void CDboTSCQCtrl::TU_ShowRewardDialog( sTS_KEY& sKey, CDboTSContReward* pCont )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SetClientWait();

	((CDboTSCQAgency*)GetParent())->TU_ShowRewardDialog( sKey, this, pCont );
}

void CDboTSCQCtrl::UT_ShowRewardDialog( sTS_KEY& sKey, int nSelRwdIdx, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_REWARD )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	if ( bCancel )
	{
		CNtlTSCont* pCancCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( ((CDboTSContReward*)pCont)->GetCancelLinkID() );
		if ( pCancCont )
		{
			unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

			UG_Avatar_TS_Confirm_Step( m_pCurTSP->GetID(), pCancCont->GetID(), uiParam, GetEventType(), GetEventData() );
		}
		else
		{
			// NPC 강제 카메라 이동에 대한 처리
			UnregNPCCameraStopEvt();
		}
	}
	else
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( ((CDboTSContReward*)pCont)->GetNextLinkID() );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, ((CDboTSContReward*)m_pCurTSP)->GetNextLinkID(), TS_CODE_TRACE() );
			SetError();
			return;
		}

		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { nSelRwdIdx , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( m_pCurTSP->GetID(), pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
	}
}

void CDboTSCQCtrl::TU_ShowUserSelectionDialog( sTS_KEY& sKey, CDboTSContUsrSel* pCont )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SetClientWait();

	((CDboTSCQAgency*)GetParent())->TU_ShowUserSelectionDialog( sKey, this, pCont );
}

void CDboTSCQCtrl::UT_ShowUserSelectionDialog( sTS_KEY& sKey, NTL_TS_TC_ID tcSelId, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_USERSEL )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	if ( bCancel )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( ((CDboTSContUsrSel*)pCont)->GetCancelLink() );
		if ( NULL == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcSelId, TS_CODE_TRACE() );
			SetError();
			return;
		}

		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( sKey.tcID, pNextCont->GetID(), uiParam, GetEventType(), GetEventData() );
	}
	else if ( ((CDboTSContUsrSel*)m_pCurTSP)->IsNextLink( tcSelId ) )
	{
		CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcSelId );
		if ( 0 == pNextCont )
		{
			CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcSelId, TS_CODE_TRACE() );
			SetError();
			return;
		}

		unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

		UG_Avatar_TS_Confirm_Step( sKey.tcID, tcSelId, uiParam, GetEventType(), GetEventData() );
	}
	else
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcSelId, TS_CODE_TRACE() );
		SetError();
	}
}

void CDboTSCQCtrl::TU_ShowNarrationDialog( sTS_KEY& sKey, CDboTSContNarration* pCont )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SetClientWait();

	((CDboTSCQAgency*)GetParent())->TU_ShowNarrationDialog( sKey, pCont );
}

void CDboTSCQCtrl::UT_ShowNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_NARRATION )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	NTL_TS_TC_ID tcNextID = bCancel ? ((CDboTSContNarration*) pCont)->GetCancelLink() : ((CDboTSContNarration*) pCont)->GetOkLink();

	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcNextID );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcNextID, TS_CODE_TRACE() );
		SetError();
		return;
	}

	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( sKey.tcID, tcNextID, uiParam, GetEventType(), GetEventData() );
}

void CDboTSCQCtrl::TU_ShowUnifiedNarrationDialog( sTS_KEY& sKey, CDboTSContUnifiedNarration* pCont )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SetClientWait();

	((CDboTSCQAgency*)GetParent())->TU_ShowUnifiedNarrationDialog( sKey, pCont );
}

void CDboTSCQCtrl::UT_ShowUnifiedNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	SetClientWait( false );

	if ( 0 == m_pCurTSP || m_pCurTSP->GetID() != sKey.tcID ) return;

	CNtlTSCont* pCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( sKey.tcID );
	if ( 0 == pCont || pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION )
	{
		CNtlTSLog::Log( "Wrong container. Info[%s]. [%s]", pCont ? pCont->GetClassName() : "NULL", TS_CODE_TRACE() );
		SetError();
		return;
	}

	NTL_TS_TC_ID tcNextID = bCancel ? ((CDboTSContUnifiedNarration*) pCont)->GetCancelLink() : ((CDboTSContUnifiedNarration*) pCont)->GetOkLink();

	CNtlTSCont* pNextCont = GetTrigger()->GetGroup( sKey.tgID )->GetChildCont( tcNextID );
	if ( 0 == pNextCont )
	{
		CNtlTSLog::Log( "Must have the next container. Info[%d,%d,%d,%d]. [%s]", sKey.tID, sKey.tgID, sKey.tcID, tcNextID, TS_CODE_TRACE() );
		SetError();
		return;
	}

	unsigned int uiParam[QUEST_REWARD_SEL_MAX_CNT] = { 0xffffffff , 0xffffffff , 0xffffffff , 0xffffffff };

	UG_Avatar_TS_Confirm_Step( sKey.tcID, tcNextID, uiParam, GetEventType(), GetEventData() );
}

void CDboTSCQCtrl::TG_Avatar_TS_ExceptTimerStartNfy( sTS_KEY& sKey, CDboTSActETimerS* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	AttachExceptionTimer( sKey.tcID, sKey.taID, GetCurTime(), pAct->GetTime() );
}

void CDboTSCQCtrl::TG_Avatar_TS_ExceptTimerEndNfy( sTS_KEY& sKey, CDboTSActETimerE* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	DetachExceptionTimer( pAct->GetTCId(), pAct->GetTAId() );
}

void CDboTSCQCtrl::TU_ServerEvent( sTS_KEY& sKey, CDboTSActSToCEvt* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCQAgency*)GetParent())->TU_ServerEvent( sKey, this );
}

void CDboTSCQCtrl::UT_ServerEvent( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCQCtrl::TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( true );

	((CDboTSCQAgency*)GetParent())->TU_ShowNPCConv( sKey, pAct );
}

void CDboTSCQCtrl::UT_ShowNPCConv( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( false );

	// NPC 강제 카메라 이동에 대한 처리
	unsigned char byResult = CheckUnregNPCCameraStop( m_pCurTSP, true );

	if ( byResult & eNPC_CAMERA_DIR_TYPE_STOP )
	{
		UnregNPCCameraStopEvt();
	}
}

void CDboTSCQCtrl::TU_RegisterQuestInfo( sTS_KEY& sKey, CDboTSActRegQInfo* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	// 기존에 등록된 Quest info 가 존재하면 클라이언트 UI 업데이트
	if ( NTL_TS_TC_ID_INVALID != m_tcQuestInfo && NTL_TS_TA_ID_INVALID != m_taQuestInfo )
	{
		((CDboTSCQAgency*)GetParent())->TU_UnregQuestInfoNfy( sKey.tID );
	}

	m_uiTitle = pAct->GetTitle();
	m_uiArea = pAct->GetArea();
	m_uiGoal = pAct->GetGoal();
	m_uiGrade = pAct->GetGrade();
	m_eGradeType = pAct->GetGradeType();
	m_eSort = pAct->GetSortType();
	m_uiSort = pAct->GetSort();
	m_uiContents = pAct->GetContents();

	CNtlTSCont* pCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( pAct->GetReward() );
	if ( pCont && pCont->IsDerivedClass( "CDboTSContReward" ) )
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		NTL_ASSERT(pSobAvatar != NULL, "Not exist avatar instance");

		BYTE byAvatarClass = Logic_GetPlayerClass(pSobAvatar);

		((CDboTSContReward*)pCont)->GetContRewardFinal(API_GetTableContainer(), byAvatarClass);

		m_pDefRwd = (sREWARD_INFO*)((CDboTSContReward*)pCont)->GetDefRewardInfo( );
		m_pSelRwd = (sREWARD_INFO*)((CDboTSContReward*)pCont)->GetSelRewardInfo( );

		m_uiRewardExp = ((CDboTSContReward*)pCont)->GetRewardExp();
		m_uiRewardZenny = ((CDboTSContReward*)pCont)->GetRewardZeny();
	}
	else
	{
		m_pDefRwd = 0;
		m_pSelRwd = 0;

		m_uiRewardExp = 0;
		m_uiRewardZenny = 0;
	}

	m_tcQuestInfo = sKey.tcID;
	m_taQuestInfo = sKey.taID;

	// 클라이언트 UI 업데이트
	((CDboTSCQAgency*)GetParent())->TU_RegQuestInfoNfy( sKey.tID, sKey.tcID, sKey.taID, true, GetTrigger()->IsShareQuest(), GetArea(), GetCurState(), GetTitle(), GetGoal(), GetSortType() );

	((CDboTSCQAgency*)GetParent())->TU_RegisterQuestInfo( sKey, this );
}

void CDboTSCQCtrl::UT_RegisterQuestInfo( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCQCtrl::TU_CameraDirection( sTS_KEY& sKey, CDboTSActOPCam* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCQAgency*)GetParent())->TU_CameraDirection( sKey, pAct );
}

void CDboTSCQCtrl::UT_CameraDirection( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCQCtrl::TG_WaitTSNfy( sTS_KEY& sKey, CDboTSActTWaitTS* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	StartTimeWait( pAct->GetWaitTime() );
}

void CDboTSCQCtrl::TG_InSSMNfy( sTS_KEY& sKey, CDboTSActInSSM* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SetSSM( pAct->GetSSMId(), pAct->GetValue() );
}

void CDboTSCQCtrl::TG_TSStateNfy( sTS_KEY& sKey, CDboTSActTSState* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	switch ( pAct->GetType() )
	{
	case eTSSTATE_TYPE_ADD:
		if ( pAct->GetTSState() & eTS_SVR_STATE_FAILED ) SetFailed();
		break;
	case eTSSTATE_TYPE_REMOVE:
		if ( pAct->GetTSState() & eTS_SVR_STATE_FAILED ) SetFailed( false );
		break;
	}
}

void CDboTSCQCtrl::TU_ShowObjConv( sTS_KEY& sKey, CDboTSActObjConv* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( true );

	((CDboTSCQAgency*)GetParent())->TU_ShowObjConv( sKey, pAct );
}

void CDboTSCQCtrl::UT_ShowObjConv( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );

	SetSvrComAfterClientWait( false );

	// NPC 강제 카메라 이동에 대한 처리
	unsigned char byResult = CheckUnregNPCCameraStop( m_pCurTSP, true );

	if ( byResult & eNPC_CAMERA_DIR_TYPE_STOP )
	{
		UnregNPCCameraStopEvt();
	}
}

void CDboTSCQCtrl::TU_Hint( sTS_KEY& sKey, CDboTSActHint* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCQAgency*)GetParent())->TU_Hint( sKey, pAct );
}

void CDboTSCQCtrl::UT_Hint( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCQCtrl::TU_Open_Window( sTS_KEY& sKey, CDboTSActOpenWindow* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCQAgency*)GetParent())->TU_Open_Window( sKey, pAct );
}

void CDboTSCQCtrl::UT_Open_Window( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCQCtrl::TU_PC_ConvNfy( sTS_KEY& sKey, CDboTSActPCConv* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	((CDboTSCQAgency*)GetParent())->TU_PC_ConvNfy( sKey, pAct );
}

void CDboTSCQCtrl::TU_TelMudosa( sTS_KEY& sKey, CDboTSActTelMudosa* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCQAgency*)GetParent())->TU_TelMudosa( sKey, pAct );
}

void CDboTSCQCtrl::UT_TelMudosa( sTS_KEY& sKey, RwUInt8 byMudosaIndex, RwBool bCancel )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );

	if ( !bCancel )
	{
		API_GetSLPacketGenerator()->SendBudokaiMudosaTeleport( byMudosaIndex );
	}
}

void CDboTSCQCtrl::TU_RegCTiming( sTS_KEY& sKey, CDboTSActRegCTiming* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCQAgency*)GetParent())->TU_RegCTiming( sKey, pAct );
}

void CDboTSCQCtrl::UT_RegCTiming( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}

void CDboTSCQCtrl::TU_ExcCGroup( sTS_KEY& sKey, CDboTSActExcCGroup* pAct )
{
	sKey.byTSType = TS_TYPE_QUEST_CS;

	SYNC_WITH_MIDDLE_BEGIN( sKey.tgID, sKey.tcID, sKey.taID );

	((CDboTSCQAgency*)GetParent())->TU_ExcCGroup( sKey, pAct );
}

void CDboTSCQCtrl::UT_ExcCGroup( sTS_KEY& sKey )
{
	SYNC_WITH_MIDDLE_END( sKey.tgID, sKey.tcID, sKey.taID );
}
