#include "precomp_ntlsimulation.h"
#include "DboTSCQAgency.h"
#include "DboTSCQCtrl.h"
#include "DboTSCQRecv.h"
#include "DboTSCMain.h"
#include "NtlSLEventFunc.h"
#include "NtlResultCode.h"
#include "TableContainer.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "DboTSDefine.h"
#include "NtlCameraController.h"
#include "TableContainer.h"
#include "QuestTextDataTable.h"
#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "GuiUtil.h"
#include "DboTSEntityFastFinder.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"
#include "MobTable.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NPCTable.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "ObjectTable.h"
#include "QuestNarrationTable.h"
#include "DboUnifiedQuestNarration.h"
#include "NtlSLLogic.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "QuestItemTable.h"
#include "ResultCodeString.h"

/** 
	Client quest agency
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCQAgency, CDboTSQAgency )


CDboTSCQAgency::CDboTSCQAgency( void )
{
	m_QuestInfo.QuestInfoList.reserve( 10 );

	m_pUnifiedQuestNarration = NULL;

	LinkMsg( g_EventShowQuestWindow );

	LinkMsg( g_EventShowIndicator );
	LinkMsg( g_EventGiveUpQuest );

	LinkMsg( g_EventDoQuest );
	LinkMsg( g_EventQuestProposalDialog_Res );
	LinkMsg( g_EventQuestUserSelectDialog_Res );
	LinkMsg( g_EventQuestNarrationUserSelectDialog_Res );
	LinkMsg( g_EventQuestRewardDialog_Res );
	LinkMsg( g_EventQuestNarrationDialog_Res );

	LinkMsg( g_EventQuestShare_Nfy );

	LinkMsg( g_EventQuestNPCDialog_Res );
	LinkMsg( g_EventQuestObjDialog_Res );

	LinkMsg( g_EventTSItemUse );
	LinkMsg( g_EventTSItemGet );
	LinkMsg( g_EventTSItemEquip );
	LinkMsg( g_EventTSScoutUse );
	LinkMsg( g_EventTSSkillUse );
	LinkMsg( g_EventTSRankBattle );
	LinkMsg( g_EventTSColTriggerObject );

	LinkMsg( g_EventTLExcuteQuest );

	LinkMsg( g_EventTSMudosaTeleport_Res );

	LinkMsg( g_EventSobTargetSelect );
	LinkMsg( g_EventSobAttackSelect );

	LinkMsg( g_EventTSSkipContainer );

	LinkMsg( g_EventTSBindStone );
	LinkMsg( g_EventTSSearchQuest );
	LinkMsg( g_EventTSItemUpgrade );
	LinkMsg( g_EventTSTeleport );
	LinkMsg( g_EventTSBudokai );
	LinkMsg( g_EventTSSlotMachine );
	LinkMsg( g_EventTSHoipoiMix );
	LinkMsg( g_EventTSPrivateShop );
	LinkMsg( g_EventTSFreeBattle );
	LinkMsg( g_EventTSItemIdentity );
	LinkMsg( g_EventTSUseMail );
	LinkMsg( g_EventTSParty );
}

CDboTSCQAgency::~CDboTSCQAgency( void )
{
	mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
	for ( ; it != m_defCurTList.end(); ++it )
	{
		CNtlTSControlObject* pObj = it->second;
		m_pParent->GetControlFactory()->DeleteObj( pObj );
	}
	m_defCurTList.clear();

	UnLinkMsg( g_EventShowQuestWindow );
	UnLinkMsg( g_EventShowIndicator );
	UnLinkMsg( g_EventGiveUpQuest );

	UnLinkMsg( g_EventDoQuest );
	UnLinkMsg( g_EventQuestProposalDialog_Res );
	UnLinkMsg( g_EventQuestUserSelectDialog_Res );
	UnLinkMsg( g_EventQuestNarrationUserSelectDialog_Res );
	UnLinkMsg( g_EventQuestRewardDialog_Res );
	UnLinkMsg( g_EventQuestNarrationDialog_Res );

	UnLinkMsg( g_EventQuestShare_Nfy );

	UnLinkMsg( g_EventQuestNPCDialog_Res );
	UnLinkMsg( g_EventQuestObjDialog_Res );

	UnLinkMsg( g_EventTSItemUse );
	UnLinkMsg( g_EventTSItemGet );
	UnLinkMsg( g_EventTSItemEquip );
	UnLinkMsg( g_EventTSScoutUse );
	UnLinkMsg( g_EventTSSkillUse );
	UnLinkMsg( g_EventTSRankBattle );
	UnLinkMsg( g_EventTSColTriggerObject );

	UnLinkMsg( g_EventTLExcuteQuest );

	UnLinkMsg( g_EventTSMudosaTeleport_Res );

	UnLinkMsg( g_EventSobTargetSelect );
	UnLinkMsg( g_EventSobAttackSelect );

	UnLinkMsg( g_EventTSSkipContainer );

	UnLinkMsg( g_EventTSBindStone );
	UnLinkMsg( g_EventTSSearchQuest );
	UnLinkMsg( g_EventTSItemUpgrade );
	UnLinkMsg( g_EventTSTeleport );
	UnLinkMsg( g_EventTSBudokai );
	UnLinkMsg( g_EventTSSlotMachine );
	UnLinkMsg( g_EventTSHoipoiMix );
	UnLinkMsg( g_EventTSPrivateShop );
	UnLinkMsg( g_EventTSFreeBattle );
	UnLinkMsg( g_EventTSItemIdentity );
	UnLinkMsg( g_EventTSUseMail );
	UnLinkMsg( g_EventTSParty );

}

SGET_QUEST_INFO* CDboTSCQAgency::GetQuestInfoList( eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter /*= NULL*/ )
{
	m_QuestInfo.dwType = 0;
	m_QuestInfo.QuestInfoList.clear();

	sQUEST_INFO sQuestInfo;

	// 1. 현재 진행 중인 퀘스트가 해당 오브젝트 아이디를 진행 이벤트로 사용할 수 있는지 검사한다
	mapdef_TRIGGER_LIST::iterator itCPQ = m_defCurTList.begin();
	for ( ; itCPQ != m_defCurTList.end(); ++itCPQ )
	{
		bool bCan = false;
		bool bVisit = false;
		unsigned int uiVisitTitleText = 0xffffffff;
		unsigned int uiVisitDLGText = 0xffffffff;

		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)itCPQ->second;

		if ( eSTOC_EVT_DATA_TYPE_VISIT == pQCtrl->GetServerEvtDataType() &&
			 !pQCtrl->IsCompletedServerEvt() )
		{
			if ( pQCtrl->IsCSComunication() ||
				 pQCtrl->IsClientWait() ||
				 pQCtrl->IsSvrWait() ||
				 pQCtrl->IsError() ||
				 pQCtrl->IsExitState() ||
				 pQCtrl->IsTimeWait() ||
				 pQCtrl->IsSvrComAfterClientWait() ||
				 !pQCtrl->IsContSyncQueueEmpty() ||
				 (NULL == pQCtrl->GetCurTSP() ) ||
				 !( pQCtrl->GetCurTSP()->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
					pQCtrl->GetCurTSP()->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
					pQCtrl->GetCurTSP()->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) )
			{
				continue;
			}

			uSTOC_EVT_DATA& uEvtData = pQCtrl->GetServerEvtData();
			for ( int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; ++i )
			{
				if ( 0xff == uEvtData.sVisitEvt[i].byObjType ) continue;

				if ( uEvtData.sVisitEvt[i].bCompleted ) continue;

				int nSum = 0;

				if ( 0xffffffff != uEvtData.sVisitEvt[i].uiItemTblIdx )
				{
					CNtlQuestInventory* pQuestInventory = GetNtlSLGlobal()->GetSobAvatar()->GetQuestInventory();
					for ( RwUInt8 j = 0; j < MAX_QUEST_INVENTORY_SLOT; ++j )
					{
						CNtlSobQuestItem* pSobQItem = pQuestInventory->GetQuestItemFromIdx( j );
						if ( pSobQItem )
						{
							CNtlSobQuestItemAttr* pSobQItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>( pSobQItem->GetSobAttr() );
							if ( pSobQItemAttr && (pSobQItemAttr->GetQuestItemTbl()->tblidx == uEvtData.sVisitEvt[i].uiItemTblIdx) )
							{
								nSum += pSobQItemAttr->GetStackNum();
							}
						}
					}
				}
				else
				{
					nSum = 1;
				}

				switch ( eEvtGenType )
				{
				case eEVENT_GEN_TYPE_CLICK_NPC:
					{
						if ( Logic_GetActiveWorldTableId() == uEvtData.sVisitEvt[i].uiWorldTblIdx &&
							 uEvtData.sVisitEvt[i].byObjType == OBJTYPE_NPC &&
							 uEvtData.sVisitEvt[i].uiObjTblIdx == uiOwnerId &&
							 nSum > 0 )
						{
							bCan = true;
							bVisit = true;
							uiVisitTitleText = uEvtData.sVisitEvt[i].uiIndicatorQText;
							uiVisitDLGText = uEvtData.sVisitEvt[i].uiDialogText;
						}
					}
					break;

				case eEVENT_GEN_TYPE_CLICK_OBJECT:
					{
						if ( Logic_GetActiveWorldTableId() == uEvtData.sVisitEvt[i].uiWorldTblIdx &&
							 uEvtData.sVisitEvt[i].byObjType == OBJTYPE_TOBJECT &&
							 uEvtData.sVisitEvt[i].uiObjTblIdx == uiOwnerId &&
							 nSum > 0 )
						{
							bCan = true;
							bVisit = true;
							uiVisitTitleText = uEvtData.sVisitEvt[i].uiIndicatorQText;
							uiVisitDLGText = uEvtData.sVisitEvt[i].uiDialogText;
						}
					}
					break;
				}
			}
		}

		if ( !bCan )
		{
			if ( eEVENT_GEN_TYPE_CLICK_NPC == eEvtGenType )			bCan = pQCtrl->CanProgressClickNPC( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_CLICK_OBJECT == eEvtGenType )	bCan = pQCtrl->CanProgressClickObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_COL_OBJECT == eEvtGenType )	bCan = pQCtrl->CanProgressCollisionObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		bCan = pQCtrl->CanProgressItemUse( uiOwnerId, (vecdef_ItemTargetList*) pParameter );
			else if ( eEVENT_GEN_TYPE_ITEM_GET == eEvtGenType )		bCan = pQCtrl->CanProgressItemGet( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_EQUIP == eEvtGenType )	bCan = pQCtrl->CanProgressItemEquip( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	bCan = pQCtrl->CanProgressScoutUse( uiOwnerId, (SScouterUseTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_RCV_SVR_EVT == eEvtGenType )	bCan = pQCtrl->CanProgressRcvSvrEvt( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )	bCan = pQCtrl->CanProgressSkillUse( uiOwnerId, (SSkillUseInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	bCan = pQCtrl->CanProgressColRgn( (sCOL_RGN_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			bCan = pQCtrl->CanProgressRB( uiOwnerId, (SRB_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_CLICK_MOB == eEvtGenType )	bCan = pQCtrl->CanProgressClickMob( uiOwnerId, (SMobTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_BIND_STONE == eEvtGenType )	bCan = pQCtrl->CanProgressBindStone();
			else if ( eEVENT_GEN_TYPE_SEARCH_QUEST == eEvtGenType )	bCan = pQCtrl->CanProgressSearchQuest();
			else if ( eEVENT_GEN_TYPE_ITEM_UPGRADE == eEvtGenType )	bCan = pQCtrl->CanProgressItemUpgrade();
			else if ( eEVENT_GEN_TYPE_TELEPORT == eEvtGenType )		bCan = pQCtrl->CanProgressTeleport();
			else if ( eEVENT_GEN_TYPE_BUDOKAI == eEvtGenType )		bCan = pQCtrl->CanProgressBudokai();
			else if ( eEVENT_GEN_TYPE_SLOTMACHINE == eEvtGenType )	bCan = pQCtrl->CanProgressSlotMachine();
			else if ( eEVENT_GEN_TYPE_HOIPOIMIX == eEvtGenType )	bCan = pQCtrl->CanProgressHoipoiMix();
			else if ( eEVENT_GEN_TYPE_PRIVATESHOP == eEvtGenType )	bCan = pQCtrl->CanProgressPrivateShop();
			else if ( eEVENT_GEN_TYPE_FREEBATTLE == eEvtGenType )	bCan = pQCtrl->CanProgressFreeBattle();
			else if ( eEVENT_GEN_TYPE_ITEMIDENTITY == eEvtGenType )	bCan = pQCtrl->CanProgressItemIdentity();
			else if ( eEVENT_GEN_TYPE_USEMAIL == eEvtGenType )		bCan = pQCtrl->CanProgressUseMail();
			else if ( eEVENT_GEN_TYPE_PARTY == eEvtGenType )		bCan = pQCtrl->CanProgressParty();

			else if (eEVENT_GEN_TYPE_FLINK == eEvtGenType)		bCan = pQCtrl->CanProgressFLink();
			else if (eEVENT_GEN_TYPE_CLICK_SIDEICON == eEvtGenType)		bCan = pQCtrl->CanProgressClickSideIcon();
			else if (eEVENT_GEN_TYPE_LEVELCHECK == eEvtGenType)		bCan = pQCtrl->CanProgressLevelCheck();
			else if (eEVENT_GEN_TYPE_QUEST == eEvtGenType)		bCan = pQCtrl->CanProgressQuest();
			else if (eEVENT_GEN_TYPE_DIALOGOPEN == eEvtGenType)		bCan = pQCtrl->CanProgressDialogOpen();

			else return &m_QuestInfo;
		}

		if ( bCan )
		{
			sQuestInfo.sKey.Init();

			sQuestInfo.sKey.byTSType = TS_TYPE_QUEST_CS;
			sQuestInfo.sKey.tID = pQCtrl->GetTrigger()->GetID();
			sQuestInfo.sKey.tcID = pQCtrl->GetCurTSP()->GetID();
			sQuestInfo.bVisitOP = bVisit;
			sQuestInfo.uiVisitTitle = uiVisitTitleText;
			sQuestInfo.uiVisitDiag = uiVisitDLGText;
			sQuestInfo.eProgType = pQCtrl->IsCleared() ? eQUEST_PROGRESS_TYPE_REWARD : eQUEST_PROGRESS_TYPE_PROGRESS;
			sQuestInfo.dwQuestTitle = pQCtrl->GetTitle();
			sQuestInfo.dwQuestStartDialog = 0xffffffff;

			m_QuestInfo.QuestInfoList.push_back( sQuestInfo );
		}
	}

	// 2. 해당 오브젝트 아이디로 시작 할 수 있는 퀘스트가 존재하면 시작한다
	CNtlTSEvtMapper* pEvtMapper = 0;
	
	if ( eEVENT_GEN_TYPE_CLICK_NPC == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMNPC" );
	}
	else if ( eEVENT_GEN_TYPE_CLICK_OBJECT == eEvtGenType || eEVENT_GEN_TYPE_COL_OBJECT == eEvtGenType )
	{
		sprintf_s( g_NtlTSString, "CDboTSEMObject_%d", GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx );
		pEvtMapper = GetParent()->FindEventMapper( g_NtlTSString );
	}
	else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType ||
			  eEVENT_GEN_TYPE_ITEM_GET == eEvtGenType ||
			  eEVENT_GEN_TYPE_ITEM_EQUIP == eEvtGenType ||
			  eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMItem" );
	}
	else if ( eEVENT_GEN_TYPE_RCV_SVR_EVT == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMSvrEvt" );
	}
	else if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMSkill" );
	}
	else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMColRgn" );
	}
	else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMRB" );
	}
	else if ( eEVENT_GEN_TYPE_CLICK_MOB == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMMob" );
	}
	else if ( eEVENT_GEN_TYPE_BIND_STONE == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMBindStone" );
	}
	else if ( eEVENT_GEN_TYPE_SEARCH_QUEST == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMSearchQuest" );
	}
	else if ( eEVENT_GEN_TYPE_ITEM_UPGRADE == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMItemUpgrade" );
	}
	else if ( eEVENT_GEN_TYPE_TELEPORT == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMTeleport" );
	}
	else if ( eEVENT_GEN_TYPE_BUDOKAI == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMBudokai" );
	}
	else if ( eEVENT_GEN_TYPE_SLOTMACHINE == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMSlotMachine" );
	}
	else if ( eEVENT_GEN_TYPE_HOIPOIMIX == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMHoipoiMix" );
	}
	else if ( eEVENT_GEN_TYPE_PRIVATESHOP == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMPrivateShop" );
	}
	else if ( eEVENT_GEN_TYPE_FREEBATTLE == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMFreeBattle" );
	}
	else if ( eEVENT_GEN_TYPE_ITEMIDENTITY == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMItemIdentity" );
	}
	else if ( eEVENT_GEN_TYPE_USEMAIL == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMUseMail" );
	}
	else if ( eEVENT_GEN_TYPE_PARTY == eEvtGenType )
	{
		pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMParty" );
	}

	else if (eEVENT_GEN_TYPE_FLINK == eEvtGenType)
	{
		pEvtMapper = GetParent()->FindEventMapper("CDboTSEMFLink");
	}
	else if (eEVENT_GEN_TYPE_CLICK_SIDEICON == eEvtGenType)
	{
		pEvtMapper = GetParent()->FindEventMapper("CDboTSEMSideIcon");
	}
	else if (eEVENT_GEN_TYPE_LEVELCHECK == eEvtGenType)
	{
		pEvtMapper = GetParent()->FindEventMapper("CDboTSEMLevelCheck");
	}
	else if (eEVENT_GEN_TYPE_QUEST == eEvtGenType)
	{
		pEvtMapper = GetParent()->FindEventMapper("CDboTSEMQuest");
	}
	else if (eEVENT_GEN_TYPE_DIALOGOPEN == eEvtGenType)
	{
		pEvtMapper = GetParent()->FindEventMapper("CDboTSEMDialogOpen");
	}

	else
	{
		return &m_QuestInfo;
	}

	const std::vector<NTL_TS_T_ID>* pSTrigList = pEvtMapper->FindTSList( uiOwnerId, "quest" );
	if ( NULL == pSTrigList || pSTrigList->empty() ) return &m_QuestInfo;

	sCQRUN_PARAM sRunParam;
	sRunParam.SetAgency( this );

	std::vector<NTL_TS_T_ID>::const_iterator itSQ;

	for ( itSQ = pSTrigList->begin(); itSQ != pSTrigList->end(); ++itSQ )
	{
		// 현재 진행중인 퀘스트이면 더 이상 진행 할 수 없다
		if ( m_defCurTList.find( *itSQ ) != m_defCurTList.end() ) continue;

		CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindQuestFromTS( *itSQ );
		if ( 0 == pTrig ) continue;

		// 반복 가능 퀘스트인지 검사한다
		if ( !pTrig->IsRepeatQuest() && GetCompletedQInfo()->HasBeenClearQuest( *itSQ ) ) continue;

		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)MakeTriggerController( pTrig );

		// Main group의 start container(시작 컨테이너)를 만족하면 퀘스트 진행

		sRunParam.SetControl( pQCtrl );

		pQCtrl->SetEventGenType( eEvtGenType );
		pQCtrl->SetEventGenId( uiOwnerId );
		if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )			pQCtrl->SetEventGenSkillUseInfo( (SSkillUseInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pQCtrl->SetEventGenItemTargetInfo( (vecdef_ItemTargetList*) pParameter );
		else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pQCtrl->SetEventGenScouterUseTargetInfo( (SScouterUseTargetInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pQCtrl->SetEventGenColRgnInfo( (sCOL_RGN_DATA*) pParameter );
		else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pQCtrl->SetEventGenRBData( (SRB_DATA*) pParameter );

		NTL_TSRESULT tsResult = pQCtrl->RunTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, m_pRecv, &sRunParam );

		pQCtrl->ClearAllEventGenInfo();

		if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
		{
			sQuestInfo.sKey.Init();

			sQuestInfo.sKey.byTSType = TS_TYPE_QUEST_CS;
			sQuestInfo.sKey.tID = pTrig->GetID();
			sQuestInfo.bVisitOP = false;
			sQuestInfo.uiVisitTitle = 0xffffffff;
			sQuestInfo.uiVisitDiag = 0xffffffff;
			sQuestInfo.eProgType = eQUEST_PROGRESS_TYPE_NEW;
			sQuestInfo.dwQuestTitle = pTrig->GetTitle();
			CDboTSContStart* pStartCont = (CDboTSContStart*)pTrig->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( START_CONTAINER_ID );
			sQuestInfo.dwQuestStartDialog = pStartCont ? pStartCont->GetSTDiag() : 0xffffffff;

			m_QuestInfo.QuestInfoList.push_back( sQuestInfo );
		}

		GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pQCtrl );
	}

	return &m_QuestInfo;
}


SGET_QUEST_INFO* CDboTSCQAgency::GetQuestInfoList_ForQuestSerarch( unsigned int uiWorldTblIdx, RwReal fX, RwReal fZ, RwReal fRadius )
{
	m_QuestInfo.dwType = 0;
	m_QuestInfo.QuestInfoList.clear();

	CDboTSEntityFastFinder* pEntityFastFinder = ((CDboTSCMain*)m_pParent)->GetEntityFastFinder();

	if ( NULL == pEntityFastFinder )
	{
		return &m_QuestInfo;
	}

	CDboTSWorldMng* pWorldMng = pEntityFastFinder->FindWorldMng( uiWorldTblIdx );

	if ( NULL == pWorldMng )
	{
		return &m_QuestInfo;
	}

	std::list< sTSEntity* > FindEntityList;
	pWorldMng->FindEntity( fX, fZ, fRadius, FindEntityList );

	if ( FindEntityList.empty() )
	{
		return &m_QuestInfo;
	}

	CNtlTSEvtMapper* pEvtMapper = GetParent()->FindEventMapper( "CDboTSEMNPC" );
	if ( NULL == pEvtMapper )
	{
		return &m_QuestInfo;
	}

	sCQRUN_PARAM sRunParam;
	sRunParam.SetAgency( this );

	sQUEST_INFO sQuestInfo;

	sTSEntity* pTSEntity;
	std::list< sTSEntity* >::iterator it = FindEntityList.begin();
	for ( ; it != FindEntityList.end(); ++it )
	{
		pTSEntity = *it;

		const std::vector<NTL_TS_T_ID>* pSTrigList = pEvtMapper->FindTSList( pTSEntity->uiTSEntityID, "quest" );
		if ( NULL == pSTrigList || pSTrigList->empty() ) continue;
	
		std::vector<NTL_TS_T_ID>::const_iterator itSQ = pSTrigList->begin();
		for ( ; itSQ != pSTrigList->end(); ++itSQ )
		{
			if ( m_defCurTList.find( *itSQ ) != m_defCurTList.end() ) continue;

			CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindQuestFromTS( *itSQ );
			if ( 0 == pTrig ) continue;

			if ( !pTrig->IsRepeatQuest() && GetCompletedQInfo()->HasBeenClearQuest( *itSQ ) ) continue;

			CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)MakeTriggerController( pTrig );

			sRunParam.SetControl( pQCtrl );

			pQCtrl->SetEventGenType( eEVENT_GEN_TYPE_CLICK_NPC );
			pQCtrl->SetEventGenId( pTSEntity->uiTSEntityID );

			NTL_TSRESULT tsResult = pQCtrl->RunTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, m_pRecv, &sRunParam );

			pQCtrl->ClearAllEventGenInfo();

			if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
			{
				sQuestInfo.sKey.Init();

				sQuestInfo.sKey.byTSType = TS_TYPE_QUEST_CS;
				sQuestInfo.sKey.tID = pTrig->GetID();
				sQuestInfo.bVisitOP = false;
				sQuestInfo.uiVisitTitle = 0xffffffff;
				sQuestInfo.uiVisitDiag = 0xffffffff;
				sQuestInfo.eProgType = eQUEST_PROGRESS_TYPE_NEW;
				sQuestInfo.dwQuestTitle = pTrig->GetTitle();
				CDboTSContStart* pStartCont = (CDboTSContStart*)pTrig->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( START_CONTAINER_ID );
				sQuestInfo.dwQuestStartDialog = pStartCont ? pStartCont->GetSTDiag() : 0xffffffff;
				sQuestInfo.uiNPCTblIdx = pTSEntity->uiTSEntityID;
				sQuestInfo.fXPos = pTSEntity->fX;
				sQuestInfo.fZPos = pTSEntity->fZ;

				m_QuestInfo.QuestInfoList.push_back( sQuestInfo );
			}

			GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pQCtrl );
		}
	}

	return &m_QuestInfo;
}


bool CDboTSCQAgency::HasEventNPCInProgressQuest( unsigned int uiNPCIdx )
{
	mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
	for ( ; it != m_defCurTList.end(); ++it )
	{
		CDboTSCQCtrl* pQuest = (CDboTSCQCtrl*) it->second;

		if ( pQuest->HasEventNPCInProgressQuest( uiNPCIdx ) )
		{
			return true;
		}
	}

	return false;
}

void CDboTSCQAgency::HandleEvents( RWS::CMsg& Msg )
{
	if ( Msg.Id == g_EventQuestShare_Nfy )
	{
		SNtlEventQuestShare_Nfy* pEventData = (SNtlEventQuestShare_Nfy*) Msg.pData;
		UG_Quest_Share( pEventData->sTSKey.tID );
	}
	else if ( Msg.Id == g_EventSobCreate )
	{
		GU_Avatar_TS_Completed_Info( (sQUEST_COMPLETE_INFO&)GetNtlSLGlobal()->GetAvatarInfo()->sQuestCompleteInfo );

		int nCnt = (int)GetNtlSLGlobal()->GetAvatarInfo()->byQuestProgressInfoCount;
		for ( int i = 0; i < nCnt; ++i )
		{
			GU_Avatar_TS_Progress_Info( (sQUEST_PROGRESS_INFO&)GetNtlSLGlobal()->GetAvatarInfo()->sQuestProgressInfo[i] );
		}
	}
	else if ( Msg.Id == g_EventShowQuestWindow )
	{
		SNtlEventShowQuestWindow* pEventData = (SNtlEventShowQuestWindow*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		TU_ShowQuestWindowNfy( pEventData->sTSKey.tID );

		mapdef_TRIGGER_LIST::iterator it = m_defCurTList.find( pEventData->sTSKey.tID );
		if ( it != m_defCurTList.end() )
		{
			((CDboTSCQCtrl*)it->second)->MatchTimingData_QTitleClick();
		}
	}
	else if ( Msg.Id == g_EventShowIndicator )
	{
		SNtlEventShowIndicator* pEventData = (SNtlEventShowIndicator*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		TU_ShowQuestIndicatorNfy( pEventData->sTSKey.tID, pEventData->bAuto ? true : false );
	}
	else if ( Msg.Id == g_EventGiveUpQuest )
	{
		SNtlEventGiveUpQuest* pEventData = (SNtlEventGiveUpQuest*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_GiveUpQuest( pEventData->sTSKey.tID );
	}
	else if ( Msg.Id == g_EventDoQuest )
	{
		SNtlEventDoQuest* pEventData = (SNtlEventDoQuest*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sQuestInfo.sKey.byTSType ) return;

		UT_EventDoQuest( pEventData->sQuestInfo, pEventData->eEventGenType, pEventData->uiOwnerID, NULL, pEventData->bTransCameraCtrlRight );
	}
	else if ( Msg.Id == g_EventQuestProposalDialog_Res )
	{
		SNtlEventQuestProposalDialog_Res* pEventData = (SNtlEventQuestProposalDialog_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowProposalDialog( pEventData->sTSKey, pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestUserSelectDialog_Res )
	{
		SNtlEventQuestUserSelectDialog_Res* pEventData = (SNtlEventQuestUserSelectDialog_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowUserSelectionDialog( pEventData->sTSKey, pEventData->tcSelID, !pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestNarrationUserSelectDialog_Res )
	{
		SNtlEventQuestNarrationUserSelectDialog_Res* pEventData = (SNtlEventQuestNarrationUserSelectDialog_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowUserSelectionDialog( pEventData->sTSKey, pEventData->tcSelID, !pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestRewardDialog_Res )
	{
		SNtlEventQuestRewardDialog_Res* pEventData = (SNtlEventQuestRewardDialog_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowRewardDialog( pEventData->sTSKey, pEventData->nSelRwdIdx, !pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestNarrationDialog_Res )
	{
		SNtlEventQuestNarrationDialog_Res* pEventData = (SNtlEventQuestNarrationDialog_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( pEventData->sTSKey.tID );

		if ( pQCtrl && pQCtrl->GetCurTSP()->GetEntityType() == DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION )
		{
			UT_ShowUnifiedNarrationDialog( pEventData->sTSKey, !pEventData->bResult );
		}
		else
		{
			UT_ShowNarrationDialog( pEventData->sTSKey, !pEventData->bResult );
		}
	}
	else if ( Msg.Id == g_EventQuestNPCDialog_Res )
	{
		SNtlEventQuestNPCDialog_Res* pEventData = (SNtlEventQuestNPCDialog_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowNPCConv( pEventData->sTSKey );
	}
	else if ( Msg.Id == g_EventQuestObjDialog_Res )
	{
		SNtlEventQuestObjDialog_Res* pEventData = (SNtlEventQuestObjDialog_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowObjConv( pEventData->sTSKey );
	}
	else if ( Msg.Id == g_EventTSItemUse )
	{
		SNtlEventTSItemUse* pEventData = (SNtlEventTSItemUse*) Msg.pData;
		sGU_CHAR_ACTION_ITEM* pServerData = (sGU_CHAR_ACTION_ITEM*)pEventData->pItemAction;

		SItemUseTargetInfo sItemUseTargetInfo;
		vecdef_ItemTargetList defItemTargetList;

		for ( int i = 0; i < pServerData->bySkillResultCount; ++i )
		{
			if ( ConvertItemUseTargetHandleToInfo( pServerData->aSkillResult[i].hTarget, sItemUseTargetInfo ) )
			{
				defItemTargetList.push_back( sItemUseTargetInfo );
			}
		}

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEM_USE, pServerData->itemTblidx, &defItemTargetList );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_ITEM_USE, pServerData->itemTblidx, &defItemTargetList );
		}
	}
	else if ( Msg.Id == g_EventTSItemGet )
	{
		SNtlEventTSItemGet* pEventData = (SNtlEventTSItemGet*) Msg.pData;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEM_GET, pEventData->uiItemIdx );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_ITEM_GET, pEventData->uiItemIdx );
		}
	}
	else if ( Msg.Id == g_EventTSItemEquip )
	{
		SNtlEventTSItemEquip* pEventData = (SNtlEventTSItemEquip*) Msg.pData;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEM_EQUIP, pEventData->uiItemIdx );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_ITEM_EQUIP, pEventData->uiItemIdx );
		}
	}
	else if ( Msg.Id == g_EventTSScoutUse )
	{
		SNtlEventTSScoutUse* pEventData = (SNtlEventTSScoutUse*) Msg.pData;

		SScouterUseTargetInfo sScoutTargetInfo;
		sScoutTargetInfo.uiClassID = pEventData->uiTargetClassID;
		sScoutTargetInfo.tblIdx = pEventData->uiTargetIdx;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_SCOUT_USE, pEventData->uiItemIdx, &sScoutTargetInfo );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			SScouterUseTargetInfo sScoutTargetInfo2;
			sScoutTargetInfo2.uiClassID = pEventData->uiTargetClassID;
			sScoutTargetInfo2.tblIdx = pEventData->uiTargetIdx;

			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_SCOUT_USE, pEventData->uiItemIdx, &sScoutTargetInfo2 );
		}
	}
	else if ( Msg.Id == g_EventTSSkillUse )
	{
		SNtlEventTSSkillUse* pEventData = (SNtlEventTSSkillUse*) Msg.pData;

		SSkillUseInfo sSkillUseInfo;
		sSkillUseInfo.eSkillType = (eEVENT_SKILL_TYPE)pEventData->uiSkillType;
		sSkillUseInfo.byRpBonusType = pEventData->byRpBonusType;

		SSkillUseTargetInfo sSkillUseTargetInfo;
		vecdef_SkillTargetList defSkillTargetList;
		for ( int i = 0; i < NTL_MAX_NUMBER_OF_SKILL_TARGET; ++i )
		{
			if ( ConvertSkillUseTargetHandleToInfo( pEventData->arhTargetHandle[i], sSkillUseTargetInfo ) )
			{
				defSkillTargetList.push_back( sSkillUseTargetInfo );
			}
		}
		sSkillUseInfo.defSkillTargetList = defSkillTargetList;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_SKILL_USE, pEventData->uiSkillIdx, &sSkillUseInfo );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_SKILL_USE, pEventData->uiSkillIdx, &sSkillUseInfo );
		}
	}
	else if ( Msg.Id == g_EventTSRankBattle )
	{
		SNtlEventTSRankBattle* pEventData = (SNtlEventTSRankBattle*) Msg.pData;

		SRB_DATA sRBData;
		sRBData.eRBCondFlag = pEventData->eRBCondFlag;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_RB, pEventData->uiRBTblIdx, &sRBData );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_RB, pEventData->uiRBTblIdx, &sRBData );
		}
	}
	else if ( Msg.Id == g_EventTSColTriggerObject )
	{
		SNtlEventTSColTriggerObject* pEventData = (SNtlEventTSColTriggerObject*) Msg.pData;

		if ( NULL == FindColTrigObj( pEventData->hTriggerObject ) )
		{
			AttachColTrigObj( pEventData->hTriggerObject );

			CNtlSob* pColliSobObj = GetNtlSobManager()->GetSobObject( pEventData->hTriggerObject );
			if ( pColliSobObj && pColliSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT )
			{
				TBLIDX tblIdx = ((CNtlSobTriggerObjectAttr*)((CNtlSobTriggerObject*)pColliSobObj)->GetSobAttr())->GetTriggerObjectTbl()->tblidx;

				SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_COL_OBJECT, tblIdx );

				if ( !pQuestInfo->QuestInfoList.empty() )
				{
					sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
					UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_COL_OBJECT, pEventData->hTriggerObject );
				}
			}
		}
	}
	else if ( Msg.Id == g_EventTLExcuteQuest )
	{
		SNtlEventTLExcuteQuest* pData = (SNtlEventTLExcuteQuest*) Msg.pData;

		UT_DoTutorialQuest( pData->tQuestID );
	}
	else if ( Msg.Id == g_EventTSMudosaTeleport_Res )
	{
		SNtlEventTSMudosaTeleport_Res* pEventData = (SNtlEventTSMudosaTeleport_Res*) Msg.pData;
		if ( TS_TYPE_QUEST_CS != pEventData->sTSKey.byTSType ) return;

		UT_TelMudosa( pEventData->sTSKey, pEventData->byMudosaIndex, pEventData->bCancel );
	}
	else if ( Msg.Id == g_EventSobTargetSelect )
	{
		SNtlEventSobTargetSelect* pEventData = (SNtlEventSobTargetSelect*) Msg.pData;

		CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pEventData->hSerialId );
		if ( NULL == pSobObj ) return;

		RwUInt32 uiClassID = pSobObj->GetClassID();
		if ( SLCLASS_MONSTER == uiClassID )
		{
			DWORD dwMobGroup = ((CNtlSobMonsterAttr*)((CNtlSobMonster*)pSobObj)->GetSobAttr())->GetMobTbl()->dwMobGroup;

			SMobTargetInfo sMobTargetInfo;
			sMobTargetInfo.hTarget = pEventData->hSerialId;

			SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );

			if ( !pQuestInfo->QuestInfoList.empty() )
			{
				sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
				UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );
			}

			mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
			for ( ; it != m_defCurTList.end(); ++it )
			{
				((CDboTSCQCtrl*)it->second)->MatchTimingData_MobClick( dwMobGroup );
			}
		}
		else if ( SLCLASS_NPC == uiClassID )
		{
			DWORD dwNPCIdx = ((CNtlSobNpcAttr*)((CNtlSobNpc*)pSobObj)->GetSobAttr())->GetNpcTbl()->tblidx;

			mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
			for ( ; it != m_defCurTList.end(); ++it )
			{
				((CDboTSCQCtrl*)it->second)->MatchTimingData_NpcClick( dwNPCIdx );
			}
		}
		else if ( SLCLASS_TRIGGER_OBJECT == uiClassID )
		{
			DWORD dwTObjIdx = ((CNtlSobTriggerObjectAttr*)((CNtlSobTriggerObject*)pSobObj)->GetSobAttr())->GetTriggerObjectTbl()->tblidx;

			mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
			for ( ; it != m_defCurTList.end(); ++it )
			{
				((CDboTSCQCtrl*)it->second)->MatchTimingData_ObjClick( GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx, dwTObjIdx );
			}
		}
	}
	else if ( Msg.Id == g_EventSobAttackSelect )
	{
		SNtlEventSobAttackSelect* pEventData = (SNtlEventSobAttackSelect*) Msg.pData;

		CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pEventData->hSerialId );
		if ( NULL == pSobObj ) return;

		RwUInt32 uiClassID = pSobObj->GetClassID();
		if ( SLCLASS_MONSTER == uiClassID )
		{
			DWORD dwMobGroup = ((CNtlSobMonsterAttr*)((CNtlSobMonster*)pSobObj)->GetSobAttr())->GetMobTbl()->dwMobGroup;

			SMobTargetInfo sMobTargetInfo;
			sMobTargetInfo.hTarget = pEventData->hSerialId;

			SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );

			if ( !pQuestInfo->QuestInfoList.empty() )
			{
				sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
				UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );
			}

			mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
			for ( ; it != m_defCurTList.end(); ++it )
			{
				((CDboTSCQCtrl*)it->second)->MatchTimingData_MobClick( dwMobGroup );
			}
		}
		else if ( SLCLASS_NPC == uiClassID )
		{
			DWORD dwNPCIdx = ((CNtlSobNpcAttr*)((CNtlSobNpc*)pSobObj)->GetSobAttr())->GetNpcTbl()->tblidx;

			mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
			for ( ; it != m_defCurTList.end(); ++it )
			{
				((CDboTSCQCtrl*)it->second)->MatchTimingData_NpcClick( dwNPCIdx );
			}
		}
		else if ( SLCLASS_TRIGGER_OBJECT == uiClassID )
		{
			DWORD dwTObjIdx = ((CNtlSobTriggerObjectAttr*)((CNtlSobTriggerObject*)pSobObj)->GetSobAttr())->GetTriggerObjectTbl()->tblidx;

			mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
			for ( ; it != m_defCurTList.end(); ++it )
			{
				((CDboTSCQCtrl*)it->second)->MatchTimingData_ObjClick( GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx, dwTObjIdx );
			}
		}
	}
	else if ( Msg.Id == g_EventSobQuestItemAdd )
	{
		SNtlEventSobQuestItemAdd* pEventData = (SNtlEventSobQuestItemAdd*) Msg.pData;

		mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
		for ( ; it != m_defCurTList.end(); ++it )
		{
			((CDboTSCQCtrl*)it->second)->MatchTimingData_GetQItem( pEventData->uiQuestItemTblId );
		}
	}
	else if ( Msg.Id == g_EventTSSkipContainer)
	{
		SNtlEventTSSkipContainer* pEventData = (SNtlEventTSSkipContainer*) Msg.pData;

		CDboTSCQCtrl* pCurQuest = (CDboTSCQCtrl*)FindProgressTrigger( pEventData->tID );

		if ( pCurQuest )
		{
			pCurQuest->SkipContainer( pEventData->tcID );
		}
	}
	else if ( Msg.Id == g_EventTSBindStone)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_BIND_STONE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_BIND_STONE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSSearchQuest)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_SEARCH_QUEST, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_SEARCH_QUEST, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSItemUpgrade)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEM_UPGRADE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_ITEM_UPGRADE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSTeleport)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_TELEPORT, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_TELEPORT, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSBudokai)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_BUDOKAI, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_BUDOKAI, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSSlotMachine)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_SLOTMACHINE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_SLOTMACHINE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSHoipoiMix)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_HOIPOIMIX, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_HOIPOIMIX, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSPrivateShop)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_PRIVATESHOP, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_PRIVATESHOP, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSFreeBattle)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_FREEBATTLE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_FREEBATTLE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSItemIdentity)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEMIDENTITY, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_ITEMIDENTITY, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSUseMail)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_USEMAIL, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_USEMAIL, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSParty)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_PARTY, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_PARTY, 0 );
		}
	}

	else if (Msg.Id == g_EventTSClickSideIcon)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_CLICK_SIDEICON, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest(sQuestInfo, eEVENT_GEN_TYPE_CLICK_SIDEICON, 0);
		}
	}
	else if (Msg.Id == g_EventTSLevelCheck)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_LEVELCHECK, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest(sQuestInfo, eEVENT_GEN_TYPE_LEVELCHECK, 0);
		}
	}
	else if (Msg.Id == g_EventTSQuest)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_QUEST, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest(sQuestInfo, eEVENT_GEN_TYPE_QUEST, 0);
		}
	}
	else if (Msg.Id == g_EventTSDialogOpen)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_DIALOGOPEN, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoQuest(sQuestInfo, eEVENT_GEN_TYPE_DIALOGOPEN, 0);
		}
	}
}

void CDboTSCQAgency::Update( void )
{
	CDboTSQAgency::Update();

	if ( m_pUnifiedQuestNarration )
	{
		m_pUnifiedQuestNarration->Update();
	}

	unsigned int uiCurTime = GetTickCount();

	mapdef_COL_TRIG_OBJ_LIST::iterator it = m_defColTrigObjCache.begin();
	for ( ; it != m_defColTrigObjCache.end(); )
	{
		sCOL_TRIG_OBJ_INFO& sInfo = it->second;

		if ( (int)(uiCurTime - sInfo.uiStandTime) > (int)sInfo.uiEraseTime )
		{
			it = m_defColTrigObjCache.erase( it );
		}
		else
		{
			++it;
		}
	}
}

bool CDboTSCQAgency::IsSameKeyTSKeyWithQMKey( const sTS_KEY& sKey, const QM_KEY& QMKey )
{
	return (sKey.tID == QMKey.tId ? true : false);
}

NTL_TS_T_ID CDboTSCQAgency::GetTIDFromQuestMarkKey( const QM_KEY& Key )
{
	return Key.tId;
}

const QM_KEY& CDboTSCQAgency::MakeQuestMarkKey( unsigned char byQProgType, NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, unsigned char byType /*= 0xff*/, unsigned int uiUserData1 /*= 0xffffffff*/, unsigned int uiUserData2 /*= 0xffffffff*/)
{
	static QM_KEY QMKey;

	QMKey.byQProgType	= byQProgType;
	QMKey.tId			= tId;
	QMKey.tcId			= tcId;
	QMKey.byType		= byType;
	QMKey.uiUserData1	= uiUserData1;
	QMKey.uiUserData2	= uiUserData2;

	return QMKey;
}

void CDboTSCQAgency::SplitQuestMarkKey( const QM_KEY& Key, unsigned char& byQProgType, NTL_TS_T_ID& tId, NTL_TS_TC_ID& tcId, unsigned char& byType, unsigned int& uiUserData1, unsigned int& uiUserData2 )
{
	byQProgType = Key.byQProgType;
	tId			= Key.tId;
	tcId		= Key.tcId;
	byType		= Key.byType;
	uiUserData1	= Key.uiUserData1;
	uiUserData2	= Key.uiUserData2;
}

const wchar_t* CDboTSCQAgency::GetQuestText( NTL_TS_T_ID tID )
{
	static std::wstring strQuestTitle;

	strQuestTitle.clear();

	CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindQuestFromTS( tID );

	if ( pTrig )
	{
		unsigned int uiQTextTblIdx = pTrig->GetTitle();
		sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( API_GetTableContainer()->GetQuestTextDataTable()->FindData( uiQTextTblIdx ) );
		if ( pQuestText )
		{
			strQuestTitle = gui::GetHtmlFromMemoryString( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
		}
	}

	return strQuestTitle.c_str();
}

CDboTSTCtrl* CDboTSCQAgency::MakeTriggerController( CNtlTSTrigger* pTrig )
{
	// Quest controller 를 생성한다
	CNtlTSControlObject* pCtrlObj = GetParent()->GetControlFactory()->CreateObj( "CDboTSCQCtrl" );
	if ( !pCtrlObj->IsDerivedClass( "CDboTSCQCtrl" ) )
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSCQCtrl. Info[%s]. [%s]", pCtrlObj->GetClassName(), TS_CODE_TRACE() );
		m_pParent->GetControlFactory()->DeleteObj( pCtrlObj );
		return 0;
	}

	// TS trigger 와 Quest trigger를 연결 및 저장한다
	((CDboTSCQCtrl*)pCtrlObj)->SetTrigger( pTrig );
	((CDboTSCQCtrl*)pCtrlObj)->SetParent( this );

	((CDboTSCQCtrl*)pCtrlObj)->Build();

	return (CDboTSCQCtrl*)pCtrlObj;
}

CDboTSCQAgency::sCOL_TRIG_OBJ_INFO* CDboTSCQAgency::FindColTrigObj( unsigned int hHandle )
{
	mapdef_COL_TRIG_OBJ_LIST::iterator it = m_defColTrigObjCache.find( hHandle );
	if ( it == m_defColTrigObjCache.end() ) return NULL;
	return &it->second;
}

void CDboTSCQAgency::AttachColTrigObj( unsigned int hHandle )
{
	if ( NULL == FindColTrigObj( hHandle ) )
	{
		sCOL_TRIG_OBJ_INFO sInfo;

		sInfo.hHandle = hHandle;
		sInfo.uiStandTime = GetTickCount();

		m_defColTrigObjCache[hHandle] = sInfo;
	}
}

void CDboTSCQAgency::GetStarterOrRewardEventInfo( NTL_TS_T_ID tID, RwUInt32& uiEventType, RwUInt32& uiEventID, RwBool& bIsEventStarter )
{
	bIsEventStarter = TRUE;
	uiEventType = eEVENT_GEN_TYPE_INVALID;
	uiEventID = 0xffffffff;

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tID );

	if ( 0 == pQCtrl )
	{
		return;
	}

	CNtlTSCont* pCurCont = pQCtrl->GetCurTSP();

	if ( NULL == pCurCont )
	{
		return;
	}

	switch ( pCurCont->GetEntityType() )
	{
	case DBO_CONT_TYPE_ID_CONT_GCOND:
		{
			if ( ((CDboTSContGCond*)pCurCont)->IsRewardMark() )
			{
				CNtlTSEntity* pEvtEntity = pCurCont->GetEventEntity();

				if ( pEvtEntity )
				{
					if ( pEvtEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_NPC )
					{
						bIsEventStarter = FALSE;
						uiEventType = eEVENT_GEN_TYPE_CLICK_NPC;
						uiEventID = ((CDboTSClickNPC*)pEvtEntity)->GetNPCIdx();
					}
				}

				return;
			}
		}
		break;

	case DBO_CONT_TYPE_ID_CONT_REWARD:
		{
			CNtlTSEntity* pEvtEntity = pCurCont->GetEventEntity();

			if ( pEvtEntity )
			{
				if ( pEvtEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_NPC )
				{
					bIsEventStarter = FALSE;
					uiEventType = eEVENT_GEN_TYPE_CLICK_NPC;
					uiEventID = ((CDboTSClickNPC*)pEvtEntity)->GetNPCIdx();
				}
			}

			return;
		}
		break;
	}

	CNtlTSCont* pStartCont = pQCtrl->GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( START_CONTAINER_ID );

	if ( pStartCont && pStartCont->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START )
	{
		CNtlTSEntity* pEvtEntity = pStartCont->GetEventEntity();

		if ( pEvtEntity && pEvtEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_NPC )
		{
			bIsEventStarter = TRUE;
			uiEventType = eEVENT_GEN_TYPE_CLICK_NPC;
			uiEventID = ((CDboTSClickNPC*)pEvtEntity)->GetNPCIdx();
		}
	}
}

void CDboTSCQAgency::UG_Quest_Share( NTL_TS_T_ID tId )
{
	// 서버에게 퀘스트 공유 메시지를 전송한다
	API_GetSLPacketGenerator()->SendQuestShare( tId );

	// MSG 출력
	const wchar_t* pQuestText = GetQuestText( tId );
	CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_TRY", FALSE, pQuestText );
}

void CDboTSCQAgency::GU_Quest_Share_Nfy( WORD wResultCode, HOBJECT hActor, NTL_TS_T_ID tId )
{
	if ( RESULT_SUCCESS == wResultCode )
	{
		// 1. Check that the quest is already in progress.
		if ( FindProgressTrigger( tId ) )
		{
			// MSG 출력
			const wchar_t* pQuestText = GetQuestText( tId );
			CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_ALREADY_PROGRESS", FALSE, pQuestText );
			return;
		}

		// 2. If it does not exist in the quest list, check.
		CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindQuestFromTS( tId );
		if ( 0 == pTrig )
		{
			return;
		}

		// 3. Check that the quest has not progressed as it was not a recurring quest.
		if ( !pTrig->IsRepeatQuest() && GetCompletedQInfo()->HasBeenClearQuest( tId ) )
		{
			// MSG 출력
			const wchar_t* pQuestText = GetQuestText( tId );
			CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_HAS_BEEN", FALSE, pQuestText );
			return;
		}

		// 4. If you can not share according to the progress of other quests, check.
		mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
		for ( ; it != m_defCurTList.end(); ++it )
		{
			CDboTSTCtrl* pCtrl = it->second;
			CNtlTSCont* pCont = pCtrl->GetCurTSP();

			if ( pCtrl && pCont )
			{
				// Failed or error status quests are in a pending state
				// Does not affect quest sharing
				if ( pCtrl->IsFailed() || pCtrl->IsError() )
				{
					continue;
				}

				// 서버와 통신 중이면 퀘스트 공유 불가
				// ( 퀘스트 진행을 위해 서버와 통신중인 경우 )
				if ( pCtrl->IsCSComunication() )
				{
					// MSG 출력
					const wchar_t* pQuestText = GetQuestText( it->first );
					CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_OTHER_QUEST_PROGRESS", FALSE, pQuestText );
					return;
				}

				// 클라이언트와 통신 중이면 퀘스트 공유 불가
				// ( 클라이언트에서 퀘스트 관련 UI ( NPC 대화창 제외 )를 출력 중인 경우 )
				if ( pCtrl->IsClientWait() )
				{
					// MSG 출력
					const wchar_t* pQuestText = GetQuestText( it->first );
					CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_OTHER_QUEST_PROGRESS", FALSE, pQuestText );
					return;
				}

				// 서버와 통신후 대기중이면 퀘스트 공유 불가
				// ( NPC 대화창 출력 중인 경우 )
				if ( pCtrl->IsSvrComAfterClientWait() )
				{
					// MSG 출력
					const wchar_t* pQuestText = GetQuestText( it->first );
					CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_OTHER_QUEST_PROGRESS", FALSE, pQuestText );
					return;
				}

				// Quest can not be shared if it is not a condition container, a compensation container, a branch container, or a termination container.
				if ( pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_GCOND &&
					 pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_REWARD &&
					 pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_SWITCH &&
					 pCont->GetEntityType() != DBO_CONT_TYPE_ID_CONT_END )
				{
					// MSG 출력
					const wchar_t* pQuestText = GetQuestText( it->first );
					CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_OTHER_QUEST_PROGRESS", FALSE, pQuestText );
					return;
				}
			}
		}

		// 5. Check if the number of progressable quests is exceeded.
		int nGeneralQuestCnt = 0, nSpecialQuestCnt = 0;
		for ( it = m_defCurTList.begin(); it != m_defCurTList.end(); ++it )
		{
			NTL_TS_T_ID tCurId = it->first;

			if ( eQUEST_ID_RANGE_GENERAL_QUEST_MIN <= tCurId && tCurId < eQUEST_ID_RANGE_GENERAL_QUEST_MAX )
			{
				nGeneralQuestCnt++;
			}
			else if ( eQUEST_ID_RANGE_SPECIAL_QUEST_MIN <= tCurId && tCurId < eQUEST_ID_RANGE_SPECIAL_QUEST_MAX )
			{
				nSpecialQuestCnt++;
			}
			else
			{
				CNtlTSLog::Log( "Wrong quest id. Info[%d]. [%s]", tCurId, TS_CODE_TRACE() );
			}
		}

		if ( eQUEST_ID_RANGE_GENERAL_QUEST_MIN <= tId && tId < eQUEST_ID_RANGE_GENERAL_QUEST_MAX )
		{
			if ( nGeneralQuestCnt >= eMAX_CAN_PROGRESS_GENERAL_QUEST_NUM )
			{
				// MSG 출력
				CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_MAX_OVERFLOW", FALSE );
				return;
			}
		}
		else if ( eQUEST_ID_RANGE_SPECIAL_QUEST_MIN <= tId && tId < eQUEST_ID_RANGE_SPECIAL_QUEST_MAX )
		{
			if ( nSpecialQuestCnt >= eMAX_CAN_PROGRESS_SPECIAL_QUEST_NUM )
			{
				// MSG 출력
				CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, "DST_QUEST_SHARE_MAX_OVERFLOW", FALSE );
				return;
			}
		}
		else
		{
			return;
		}

		// 6.Check if the quest condition is met.

		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)MakeTriggerController( pTrig );
		pQCtrl->SetQuestShareMode( true );

		sCQRUN_PARAM sRunParam;
		sRunParam.SetControl( pQCtrl );
		sRunParam.SetAgency( this );
		sRunParam.SetQuestShare( true );
		sRunParam.SetQuestShareOutMsg( true );

		NTL_TSRESULT tsResult = pTrig->RunTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, m_pRecv, &sRunParam );

		if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
		{
			m_defCurTList[pTrig->GetID()] = pQCtrl;
			pQCtrl->Update();
		}
		else
		{
			GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pQCtrl );
		}
	}
	else
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(wResultCode, "").c_str() );
	}
}

void CDboTSCQAgency::GU_Avatar_TS_Completed_Info( const sCOMPLETE_QUEST_INFO& sInfo )
{
	OUT_QMSG_0( "[Quest] 퀘스트 완료 정보 처리중..." );

	LoadCompletedQInfo( sInfo );
}

void CDboTSCQAgency::GU_Avatar_TS_Progress_Info( const sPROGRESS_QUEST_INFO& sInfo )
{
	OUT_QMSG_0( "[Quest] 퀘스트 진행 정보 처리중..." );

	switch ( sInfo.byVer )
	{
	case 0:
		LoadQuestProgressInfo_V0( ((CDboTSCMain*)m_pParent)->FindQuestFromTS( sInfo.tId ), sInfo );
		break;
	default:
		CNtlTSLog::Log( "The progress quest info version mismatch. Info[%d]. [%s]", sInfo.byVer, TS_CODE_TRACE() );
		break;
	}
}

void CDboTSCQAgency::UG_Avatar_TS_Confirm_Step( NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, unsigned int* uiParam, unsigned char byEventType, unsigned int uiEventData )
{
	OUT_QMSG_9( "[Quest] User -> GameServer : Confirm 처리중 [%d,%d,%d,%d,%d,%d,%d,%d,%d]", tId, tcCurId, tcNextId, uiParam[1], uiParam[2], uiParam[3], uiParam[4], byEventType, uiEventData );

	// 서버에게 메시지를 보낸다
	API_GetSLPacketGenerator()->SendTSConfirmStepReq( TS_TYPE_QUEST_CS, tId, tcCurId, tcNextId, uiParam, byEventType, uiEventData );
}

void CDboTSCQAgency::GU_Avatar_TS_Confirm_Step( WORD wResultCode, NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId )
{
	OUT_QMSG_3( "[Quest] GameServer -> User : Confirm 처리중 [%d,%d,%d]", tId, tcCurId, tcNextId );

	if ( RESULT_SUCCESS != wResultCode )
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(wResultCode, "").c_str());
	}

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );

	if ( pQCtrl ) pQCtrl->GU_Avatar_TS_Confirm_Step( wResultCode, tcCurId, tcNextId );
}

void CDboTSCQAgency::UG_Avatar_TS_GiveUp_Quest( NTL_TS_T_ID tId )
{
	OUT_QMSG_1( "[Quest] User -> GameServer : Give up 처리중 [%d]", tId );

	// 서버에게 퀘스트 포기 메시지를 전송한다
	API_GetSLPacketGenerator()->SendQuestGiveUpReq( tId );
}

void CDboTSCQAgency::GU_Avatar_TS_GiveUp_Quest( WORD wResultCode, NTL_TS_T_ID tId )
{
	OUT_QMSG_2( "[Quest] GameServer -> User : Give up 처리중 [%d, %d]", wResultCode, tId );

	if ( RESULT_SUCCESS != wResultCode )
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(wResultCode, "").c_str());
	}

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );

	if ( pQCtrl ) pQCtrl->GU_Avatar_TS_GiveUp_Quest( wResultCode );
}

void CDboTSCQAgency::GU_Avatar_TS_SToC_Event_Start_NFY( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId )
{
	OUT_QMSG_3( "[Quest] GameServer -> User : Server event start message 처리중 [%d, %d, %d]", tId, tcId, taId );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );
	if ( pQCtrl ) pQCtrl->GU_Avatar_TS_SToC_Event_Start_NFY( tcId, taId );
}

void CDboTSCQAgency::GU_Avatar_TS_SToC_Event_End_NFY( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId )
{
	OUT_QMSG_3( "[Quest] GameServer -> User : Server event end message 처리중 [%d, %d, %d]", tId, tcId, taId );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );
	if ( pQCtrl ) pQCtrl->GU_Avatar_TS_SToC_Event_End_NFY( tcId, taId );
}

void CDboTSCQAgency::GU_Avatar_TS_SToC_Event_Update_NFY( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, BYTE bySvrEvtType, BYTE bySlot, uSTOC_EVT_UPDATE_DATA& uEvtData )
{
	OUT_QMSG_4( "[Quest] GameServer -> User : Server event update message 처리중 [%d, %d, %d, %d]", tId, tcId, taId, bySvrEvtType );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );
	if ( pQCtrl ) pQCtrl->GU_Avatar_TS_SToC_Event_Update_NFY( tcId, taId, bySvrEvtType, bySlot, uEvtData );
}

void CDboTSCQAgency::GU_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	OUT_QMSG_4( "[Quest] GameServer -> User : Update TS state message 처리중 [%d, %d, %d, %d]", tId, byType, wTSState, uiParam );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );

	if ( pQCtrl ) pQCtrl->GU_TS_Update_State( byType, wTSState, uiParam );
}

void CDboTSCQAgency::UG_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	OUT_QMSG_4( "[Quest] User -> GameServer : Update TS state message 처리중 [%d, %d, %d, %d]", tId, byType, wTSState, uiParam );

	// 서버에게 상태 업데이트 메시지를 전송한다
	API_GetSLPacketGenerator()->SendTSUpdateState( tId, TS_TYPE_QUEST_CS, byType, wTSState, uiParam );
}

void CDboTSCQAgency::GU_TS_Update_Event_Nfy( NTL_TS_EVENT_ID eID )
{
	OUT_QMSG_1( "[Quest] GameServer -> User : Update TS Event 처리중 [%d]", eID );

	// 해당 이벤트 아이디로 퀘스트가 시작하는 경우에 한해서는 바로 처리한다
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_RCV_SVR_EVT, eID );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			vecdef_QUEST_INFO_LIST::iterator it = pQuestInfo->QuestInfoList.begin();
			for ( ; it != pQuestInfo->QuestInfoList.end(); ++it )
			{
				sQUEST_INFO& sQuestInfo = *it;

				if ( eQUEST_PROGRESS_TYPE_NEW == sQuestInfo.eProgType )
				{
					UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_RCV_SVR_EVT, eID );
				}
			}
		}
	}

	// 현재 진행중인 퀘스트에 한해서는 이벤트 등록후 해당 이벤트가 준비된 상황에서 동작시킨다
	{
		mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
		for ( ; it != m_defCurTList.end(); ++it )
		{
			CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)it->second;

			pQCtrl->AttachSvrEventID( eID );
		}
	}
}

void CDboTSCQAgency::GU_TS_Excute_Trigger( NTL_TS_T_ID tID )
{
	//////////////////////////////////////////////////////////////////////////
	// Execute unconditionally because the server already checks the condition
	//////////////////////////////////////////////////////////////////////////

	CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindQuestFromTS( tID );
	if ( 0 == pTrig )
	{
		return;
	}

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)MakeTriggerController( pTrig );
	if ( 0 == pQCtrl )
	{
		return;
	}

	m_defCurTList[pTrig->GetID()] = pQCtrl;
}

void CDboTSCQAgency::GU_TS_RemoveTMQQuest( NTL_TS_T_ID tMin, NTL_TS_T_ID tMax )
{
	OUT_QMSG_0( "[Quest] GameServer -> User : Remove TMQ Quest 처리중" );

	// 현재 진행 중인 퀘스트 제거
	mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
	for ( ; it != m_defCurTList.end(); )
	{
		NTL_TS_T_ID tId = it->first;
		CDboTSCQCtrl* pCtrl = (CDboTSCQCtrl*)it->second;

		if ( tMin <= tId && tId < tMax )
		{
			it = m_defCurTList.erase( it );

			// 퀘스트 제거
			GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pCtrl );

			// 클라이언트에게 해당 퀘스트가 지워져야 함을 알림
			CNtlSLEventGenerator::RemovingTMQQuest_Nfy( tId );
		}
		else
		{
			++it;
		}
	}

	// 완료된 퀘스트 제거
	GetCompletedQInfo()->InitQuest( tMin, tMax );
}

void CDboTSCQAgency::GU_Qeust_Force_Completion( NTL_TS_T_ID tID )
{
	GetCompletedQInfo()->SetClearQuest( tID, true );
}

//////////////////////////////////////////////////////////////////////////
//
//	호출되는 시점.
//
//	1. 퀘스트 등록 액션이 실행되는 시점에 호출된다.
//	2. DB로 부터 퀘스트 정보를 로딩하는 시점에 호출된다.
//
//////////////////////////////////////////////////////////////////////////
void CDboTSCQAgency::TU_RegQuestInfoNfy( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, bool bNewRegister, bool bQuestShare, unsigned int uiQAreaName, unsigned int uiQState, unsigned int uiQTitle, unsigned int uiQGoal, eQUEST_SORT_TYPE eQSortType )
{
	OUT_QMSG_3( "[Quest] Trigger -> User : Regist quest info nfy message 처리중 [%d, %d, %d]", tId, tcId, taId );

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = NTL_TS_MAIN_GROUP_ID;
	key.tcID = tcId;
	key.taID = taId;

	CNtlSLEventGenerator::RegQuest_Nfy( key, bNewRegister, bQuestShare, uiQAreaName, uiQState, uiQTitle, uiQGoal, eQSortType );
}

//////////////////////////////////////////////////////////////////////////
//
//	호출되는 시점.
//
//	1. 퀘스트가 제거 되는 시점에서 호출된다.
//
//////////////////////////////////////////////////////////////////////////
void CDboTSCQAgency::TU_UnregQuestInfoNfy( NTL_TS_T_ID tId )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Unregist quest info nfy message 처리중 [%d]", tId );

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = NTL_TS_TG_ID_INVALID;
	key.tcID = NTL_TS_TC_ID_INVALID;
	key.taID = NTL_TS_TA_ID_INVALID;

	CNtlSLEventGenerator::UnregQuest_Nfy( key );
}

//////////////////////////////////////////////////////////////////////////
//
//	When it is called.
//
//	1. g_EventShowQuestWindow Called when an event occurs.
//
//////////////////////////////////////////////////////////////////////////
void CDboTSCQAgency::TU_ShowQuestWindowNfy( NTL_TS_T_ID tId )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show quest window nfy message 처리중 [%d]", tId );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );
	if ( 0 == pQCtrl ) return;

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = NTL_TS_TG_ID_INVALID;
	key.tcID = NTL_TS_TC_ID_INVALID;
	key.taID = NTL_TS_TA_ID_INVALID;

	RwUInt32 uiEventType, uiEventID;
	RwBool bIsEventStarter;

	GetStarterOrRewardEventInfo( tId, uiEventType, uiEventID, bIsEventStarter );

	CNtlSLEventGenerator::ShowQuestWindow_Nfy( key,
											   pQCtrl->GetArea(),
											   pQCtrl->GetTitle(),
											   pQCtrl->GetSort(),
											   pQCtrl->GetGrade(),
											   pQCtrl->GetGradeType(),
											   pQCtrl->GetGoal(),
											   pQCtrl->GetContents(),
											   pQCtrl->GetDefRwd(),
											   pQCtrl->GetSelRwd(),
		pQCtrl->GetRewardExp(),
		pQCtrl->GetRewardZeny(),
											   pQCtrl->GetServerEvtDataType(),
											   pQCtrl->GetServerEvtData(),
											   pQCtrl->GetLimitTime(),
											   uiEventType,
											   uiEventID,
											   bIsEventStarter);
}

//////////////////////////////////////////////////////////////////////////
//
//	호출되는 시점.
//
//	1. g_EventShowIndicator 이벤트가 발생했을 경우 호출된다.
//
//////////////////////////////////////////////////////////////////////////
void CDboTSCQAgency::TU_ShowQuestIndicatorNfy( NTL_TS_T_ID tId, bool bAuto )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show quest indicator nfy message 처리중 [%d]", tId );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );
	if ( 0 == pQCtrl ) return;

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = NTL_TS_TG_ID_INVALID;
	key.tcID = NTL_TS_TC_ID_INVALID;
	key.taID = NTL_TS_TA_ID_INVALID;

	CNtlSLEventGenerator::ShowIndicator_Nfy( key,
											 pQCtrl->GetCurState(),
											 pQCtrl->GetTitle(),
											 pQCtrl->GetGoal(),
											 pQCtrl->GetServerEvtDataType(),
											 pQCtrl->GetServerEvtData(),
											 pQCtrl->GetLimitTime(),
											 bAuto );
}

//////////////////////////////////////////////////////////////////////////
//
//	호출되는 시점.
//
//	1. Failed or Error 상태로 전이 될 때 호출된다.
//	2. DB로 부터 퀘스트 정보를 로딩하는 시점에 호출된다.
//
//////////////////////////////////////////////////////////////////////////
void CDboTSCQAgency::TU_UpdateQuestStateNfy( NTL_TS_T_ID tId, bool bOutStateMsg, unsigned int uiUpdatedQState, unsigned int uiQState, unsigned int uiQuestTitle, eSTOC_EVT_DATA_TYPE eEvtInfoType, const uSTOC_EVT_DATA& uEvtInfoData )
{
	OUT_QMSG_3( "[Quest] Trigger -> User : Update quest state nfy message 처리중 [%d, %d, %d]", tId, uiUpdatedQState, uiQState );

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = NTL_TS_TG_ID_INVALID;
	key.tcID = NTL_TS_TC_ID_INVALID;
	key.taID = NTL_TS_TA_ID_INVALID;

	RwUInt32 uiEventType, uiEventID;
	RwBool bIsEventStarter;

	GetStarterOrRewardEventInfo( tId, uiEventType, uiEventID, bIsEventStarter );

	CNtlSLEventGenerator::UpdateQuestState_Nfy( key, bOutStateMsg, uiUpdatedQState, uiQState, uiQuestTitle, eEvtInfoType, uEvtInfoData, uiEventType, uiEventID, bIsEventStarter );
}

//////////////////////////////////////////////////////////////////////////
//
//	호출되는 시점.
//
//	1. 시간 제한 퀘스트의 경우 시간의 흐름에 따라서 주기적으로 호출된다.
//	2. DB로 부터 퀘스트 정보를 로딩하는 시점에 호출된다.
//	3. 서버로 부터 서버 이벤트 정보를 수신 할 때마다 호출된다.
//	4. 서버 이벤트가 종료될 때 호출된다.
//
//////////////////////////////////////////////////////////////////////////
void CDboTSCQAgency::TU_UpdateQuestProgressInfoNfy( NTL_TS_T_ID tId, eSTOC_EVT_DATA_TYPE eEvtInfoType, uSTOC_EVT_DATA& uEvtInfoData, RwUInt32 uiTimeLimit )
{
	OUT_QMSG_3( "[Quest] Trigger -> User : Update quest progress info nfy message 처리중 [%d, %d, %d]", tId, eEvtInfoType, uiTimeLimit );

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = NTL_TS_TG_ID_INVALID;
	key.tcID = NTL_TS_TC_ID_INVALID;
	key.taID = NTL_TS_TA_ID_INVALID;

	CNtlSLEventGenerator::UpdateQuestProgressInfo_Nfy( key, eEvtInfoType, uEvtInfoData, uiTimeLimit );
}

void CDboTSCQAgency::TU_AcceptProposal( NTL_TS_T_ID tId, NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId )
{
	OUT_QMSG_3( "[Quest] Trigger -> User : Accept proposal quest nfy message 처리중 [%d, %d, %d]", tId, tgId, tcId );

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = tgId;
	key.tcID = tcId;
	key.taID = NTL_TS_TA_ID_INVALID;

	CNtlSLEventGenerator::QuestAcceptProposal_Nfy( key );
}

void CDboTSCQAgency::TU_AcceptReward( NTL_TS_T_ID tId, NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId, unsigned int uiQTitle, bool bOutStateMsg )
{
	OUT_QMSG_3( "[Quest] Trigger -> User : Accept reward nfy message 처리중 [%d, %d, %d]", tId, tgId, tcId );

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = tgId;
	key.tcID = tcId;
	key.taID = NTL_TS_TA_ID_INVALID;

	CNtlSLEventGenerator::QuestAcceptReward_Nfy( key, uiQTitle, bOutStateMsg );
}

void CDboTSCQAgency::TU_AcceptGiveUp( NTL_TS_T_ID tId, NTL_TS_TG_ID tgId, unsigned int uiQTitle )
{
	OUT_QMSG_2( "[Quest] Trigger -> User : Accept give up nfy message 처리중 [%d, %d]", tId, tgId );

	sTS_KEY key;
	key.byTSType = TS_TYPE_QUEST_CS;
	key.tID = tId;
	key.tgID = tgId;
	key.tcID = NTL_TS_TC_ID_INVALID;
	key.taID = NTL_TS_TA_ID_INVALID;

	CNtlSLEventGenerator::QuestAcceptGiveUp_Nfy( key, uiQTitle );
}

void CDboTSCQAgency::UT_GiveUpQuest( NTL_TS_T_ID tId )
{
	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( tId );
	if ( 0 == pQCtrl ) return;

	pQCtrl->UT_GiveUpQuest();
}

void CDboTSCQAgency::UT_UpdateAvatarPos( unsigned int uiWorldIdx, float fPosX, float fPosZ )
{
	if ( 0xffffffff == uiWorldIdx )
	{
		return;
	}

	// World 가 교체된 경우
	if ( m_sAvatarCurPos.uiWorldIx != uiWorldIdx )
	{
		sCOL_RGN_DATA sColRgnData;

		// Leave 처리
		if ( 0xffffffff != m_sAvatarCurPos.uiWorldIx )
		{
			sColRgnData.eWorldChangeType = sCOL_RGN_DATA::eWORLD_CHANGE_TYPE_LEAVE;
			sColRgnData.uiWorldIdx = m_sAvatarCurPos.uiWorldIx;
			sColRgnData.fBeginX = m_sAvatarCurPos.fPosX;
			sColRgnData.fBeginZ = m_sAvatarCurPos.fPosZ;
			sColRgnData.fEndX = m_sAvatarCurPos.fPosX;
			sColRgnData.fEndZ = m_sAvatarCurPos.fPosZ;

			SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );

			if ( !pQuestInfo->QuestInfoList.empty() )
			{
				sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
				UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );
			}

			// 바로 Enter 처리가 이뤄지는 것을 막고
			// 다음 프레임에 처리되도록 한다
			m_sAvatarCurPos.uiWorldIx = 0xffffffff;
		}
		else
		{
			sColRgnData.eWorldChangeType = sCOL_RGN_DATA::eWORLD_CHANGE_TYPE_ENTER;
			sColRgnData.uiWorldIdx = uiWorldIdx;
			sColRgnData.fBeginX = fPosX;
			sColRgnData.fBeginZ = fPosZ;
			sColRgnData.fEndX = fPosX;
			sColRgnData.fEndZ = fPosZ;

			SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );

			if ( !pQuestInfo->QuestInfoList.empty() )
			{
				sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
				UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );
			}

			m_sAvatarCurPos.uiWorldIx = uiWorldIdx;
			m_sAvatarCurPos.fPosX = fPosX;
			m_sAvatarCurPos.fPosZ = fPosZ;
		}
	}
	// World 가 교체되지 않은 경우
	else
	{
		// 같은 월드에서 이동을 한 경우
		if ( abs( fPosX - m_sAvatarCurPos.fPosX ) > 0.0001f ||
			 abs( fPosZ - m_sAvatarCurPos.fPosZ ) > 0.0001f )
		{
			sCOL_RGN_DATA sColRgnData;

			sColRgnData.eWorldChangeType = sCOL_RGN_DATA::eWORLD_CHANGE_TYPE_NO;
			sColRgnData.uiWorldIdx = uiWorldIdx;
			sColRgnData.fBeginX = m_sAvatarCurPos.fPosX;
			sColRgnData.fBeginZ = m_sAvatarCurPos.fPosZ;
			sColRgnData.fEndX = fPosX;
			sColRgnData.fEndZ = fPosZ;

			SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );

			if ( !pQuestInfo->QuestInfoList.empty() )
			{
				sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
				UT_EventDoQuest( sQuestInfo, eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );
			}

			m_sAvatarCurPos.uiWorldIx = uiWorldIdx;
			m_sAvatarCurPos.fPosX = fPosX;
			m_sAvatarCurPos.fPosZ = fPosZ;
		}
	}
}

void CDboTSCQAgency::TU_FinishQuest( unsigned char byTSType, NTL_TS_T_ID tID )
{
	CNtlSLEventGenerator::TSFinished( byTSType, tID );
}

void CDboTSCQAgency::UT_EventDoQuest( sQUEST_INFO& sQuestInfo, eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter /*= NULL*/, RwBool bTransCameraCtrlRight /*= FALSE*/ )
{
	OUT_QMSG_3( "[Quest] User -> Trigger : Do quest message 처리중 [%d, %d, %d]", sQuestInfo.sKey.tID, eEvtGenType, uiOwnerId );

	if ( sQuestInfo.bVisitOP )
	{
		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sQuestInfo.sKey.tID );

		if ( pQCtrl )
		{
			switch ( eEvtGenType )
			{
			case eEVENT_GEN_TYPE_CLICK_NPC:
				{
					// Visit의 경우는 카메라를 반드시 빼줘야 한다
					pQCtrl->RegNPCCameraStopEvt();
					pQCtrl->UnregNPCCameraStopEvt();

					API_GetSLPacketGenerator()->SendQuestObjectVisitReq( sQuestInfo.sKey.tID, Logic_GetActiveWorldId(), OBJTYPE_NPC, uiOwnerId );
				}
				return;

			case eEVENT_GEN_TYPE_CLICK_OBJECT:
				{
					// Visit의 경우는 카메라를 반드시 빼줘야 한다
					pQCtrl->RegNPCCameraStopEvt();
					pQCtrl->UnregNPCCameraStopEvt();

					API_GetSLPacketGenerator()->SendQuestObjectVisitReq( sQuestInfo.sKey.tID, Logic_GetActiveWorldId(), OBJTYPE_TOBJECT, uiOwnerId );
				}
				return;
			}
		}

		CNtlTSLog::Log( "Not execute the visit server event. Info[%d,%d,%d]. [%s]", sQuestInfo.sKey.tID, eEvtGenType, uiOwnerId, TS_CODE_TRACE() );

		return;
	}

	if ( eQUEST_PROGRESS_TYPE_NEW == sQuestInfo.eProgType )
	{
		NTL_TS_T_ID tID;

		// 일반 퀘스트와 특별한 퀘스트( TMQ, Tutorial... )의 개수를 센다
		int nGeneralQuestCnt = 0, nSpecialQuestCnt = 0;
		mapdef_TRIGGER_LIST::iterator itCurQuest = m_defCurTList.begin();
		for ( ; itCurQuest != m_defCurTList.end(); ++itCurQuest )
		{
			tID = itCurQuest->first;

			if ( eQUEST_ID_RANGE_GENERAL_QUEST_MIN <= tID && tID < eQUEST_ID_RANGE_GENERAL_QUEST_MAX )
			{
				nGeneralQuestCnt++;
			}
			else if ( eQUEST_ID_RANGE_SPECIAL_QUEST_MIN <= tID && tID < eQUEST_ID_RANGE_SPECIAL_QUEST_MAX )
			{
				nSpecialQuestCnt++;
			}
			else
			{
				CNtlTSLog::Log( "Wrong quest id. Info[%d]. [%s]", tID, TS_CODE_TRACE() );
			}
		}

		tID = sQuestInfo.sKey.tID;

		// 받을 수 있는 퀘스트 최대치를 넘은 경우는 공유 받을 수 없다
		if ( eQUEST_ID_RANGE_GENERAL_QUEST_MIN <= tID && tID < eQUEST_ID_RANGE_GENERAL_QUEST_MAX )
		{
			if ( nGeneralQuestCnt >= eMAX_CAN_PROGRESS_GENERAL_QUEST_NUM )
			{
				CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, "GAME_TS_WARNING_OVERFLOW_MAX_TS_NUM" );

				// Camera의 제어권이 넘어온 경우만 카메라를 빼준다
				if ( bTransCameraCtrlRight )
				{
					Logic_CancelNpcFacing();
				}

				return;
			}
		}
		else if ( eQUEST_ID_RANGE_SPECIAL_QUEST_MIN <= tID && tID < eQUEST_ID_RANGE_SPECIAL_QUEST_MAX )
		{
			if ( nSpecialQuestCnt >= eMAX_CAN_PROGRESS_SPECIAL_QUEST_NUM )
			{
				CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, "GAME_TS_WARNING_OVERFLOW_MAX_TS_NUM" );

				// Camera의 제어권이 넘어온 경우만 카메라를 빼준다
				if ( bTransCameraCtrlRight )
				{
					Logic_CancelNpcFacing();
				}

				return;
			}
		}

		// 진행 중에 있는 퀘스트는 새로이 진행할 수 없다
		CDboTSCQCtrl* pProgress = (CDboTSCQCtrl*)FindProgressTrigger( sQuestInfo.sKey.tID );
		if ( pProgress )
		{
			// Camera의 제어권이 넘어온 경우만 카메라를 빼준다
			if ( bTransCameraCtrlRight )
			{
				Logic_CancelNpcFacing();
			}

			return;
		}

		CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindQuestFromTS( sQuestInfo.sKey.tID );
		if ( 0 == pTrig )
		{
			// Camera의 제어권이 넘어온 경우만 카메라를 빼준다
			if ( bTransCameraCtrlRight )
			{
				Logic_CancelNpcFacing();
			}

			return;
		}

		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)MakeTriggerController( pTrig );

		sCQRUN_PARAM sRunParam;
		sRunParam.SetControl( pQCtrl );
		sRunParam.SetAgency( this );

		// Main group의 start container(시작 컨테이너)를 만족하면 퀘스트 진행

		pQCtrl->SetEventGenType( eEvtGenType );
		pQCtrl->SetEventGenId( uiOwnerId );
		if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )			pQCtrl->SetEventGenSkillUseInfo( (SSkillUseInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pQCtrl->SetEventGenItemTargetInfo( (vecdef_ItemTargetList*) pParameter );
		else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pQCtrl->SetEventGenScouterUseTargetInfo( (SScouterUseTargetInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pQCtrl->SetEventGenColRgnInfo( (sCOL_RGN_DATA*) pParameter );
		else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pQCtrl->SetEventGenRBData( (SRB_DATA*) pParameter );

		NTL_TSRESULT tsResult = pTrig->RunTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, m_pRecv, &sRunParam );

		pQCtrl->ClearAllEventGenInfo();

		if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
		{
			m_defCurTList[pTrig->GetID()] = pQCtrl;

			pQCtrl->SetTransCameraCtrlRight( bTransCameraCtrlRight );

			if ( eEVENT_GEN_TYPE_CLICK_NPC == eEvtGenType )			pQCtrl->UT_ClickNPC( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_CLICK_OBJECT == eEvtGenType )	pQCtrl->UT_ClickObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_COL_OBJECT == eEvtGenType )	pQCtrl->UT_CollisionObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pQCtrl->UT_ItemUse( uiOwnerId, (vecdef_ItemTargetList*) pParameter );
			else if ( eEVENT_GEN_TYPE_ITEM_GET == eEvtGenType )		pQCtrl->UT_ItemGet( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_EQUIP == eEvtGenType )	pQCtrl->UT_ItemEquip( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pQCtrl->UT_ScoutUse( uiOwnerId, (SScouterUseTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_RCV_SVR_EVT == eEvtGenType )	pQCtrl->UT_RcvSvrEvt( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )	pQCtrl->UT_SkillUse( uiOwnerId, (SSkillUseInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pQCtrl->UT_ColRgn( (sCOL_RGN_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pQCtrl->UT_RB( uiOwnerId, (SRB_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_CLICK_MOB == eEvtGenType )	pQCtrl->UT_ClickMob( uiOwnerId, (SMobTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_BIND_STONE == eEvtGenType )	pQCtrl->UT_BindStone();
			else if ( eEVENT_GEN_TYPE_SEARCH_QUEST == eEvtGenType )	pQCtrl->UT_SearchQuest();
			else if ( eEVENT_GEN_TYPE_ITEM_UPGRADE == eEvtGenType )	pQCtrl->UT_ItemUpgrade();
			else if ( eEVENT_GEN_TYPE_TELEPORT == eEvtGenType )		pQCtrl->UT_Teleport();
			else if ( eEVENT_GEN_TYPE_BUDOKAI == eEvtGenType )		pQCtrl->UT_Budokai();
			else if ( eEVENT_GEN_TYPE_SLOTMACHINE == eEvtGenType )	pQCtrl->UT_SlotMachine();
			else if ( eEVENT_GEN_TYPE_HOIPOIMIX == eEvtGenType )	pQCtrl->UT_HoipoiMix();
			else if ( eEVENT_GEN_TYPE_PRIVATESHOP == eEvtGenType )	pQCtrl->UT_PrivateShop();
			else if ( eEVENT_GEN_TYPE_FREEBATTLE == eEvtGenType )	pQCtrl->UT_FreeBattle();
			else if ( eEVENT_GEN_TYPE_ITEMIDENTITY == eEvtGenType )	pQCtrl->UT_ItemIdentity();
			else if ( eEVENT_GEN_TYPE_USEMAIL == eEvtGenType )		pQCtrl->UT_UseMail();
			else if ( eEVENT_GEN_TYPE_PARTY == eEvtGenType )		pQCtrl->UT_Party();

			else if (eEVENT_GEN_TYPE_CLICK_SIDEICON == eEvtGenType)	pQCtrl->UT_ClickSideIcon();
			else if (eEVENT_GEN_TYPE_LEVELCHECK == eEvtGenType)		pQCtrl->UT_LevelCheck();
			else if (eEVENT_GEN_TYPE_QUEST == eEvtGenType)			pQCtrl->UT_Quest();
			else if (eEVENT_GEN_TYPE_DIALOGOPEN == eEvtGenType)		pQCtrl->UT_DialogOpen();

			else
			{
				// Camera의 제어권이 넘어온 경우만 카메라를 빼준다
				if ( bTransCameraCtrlRight )
				{
					Logic_CancelNpcFacing();
				}
			}

			pQCtrl = NULL;
		}

		if ( pQCtrl )
		{
			GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pQCtrl );
		}
	}
	else
	{
		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sQuestInfo.sKey.tID );

		if( pQCtrl->GetCurTSP()->GetID() == sQuestInfo.sKey.tcID )
		{
			pQCtrl->SetTransCameraCtrlRight( bTransCameraCtrlRight );

			if ( eEVENT_GEN_TYPE_CLICK_NPC == eEvtGenType )			pQCtrl->UT_ClickNPC( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_CLICK_OBJECT == eEvtGenType )	pQCtrl->UT_ClickObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_COL_OBJECT == eEvtGenType )	pQCtrl->UT_CollisionObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pQCtrl->UT_ItemUse( uiOwnerId, (vecdef_ItemTargetList*) pParameter );
			else if ( eEVENT_GEN_TYPE_ITEM_GET == eEvtGenType )		pQCtrl->UT_ItemGet( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_EQUIP == eEvtGenType )	pQCtrl->UT_ItemEquip( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pQCtrl->UT_ScoutUse( uiOwnerId, (SScouterUseTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_RCV_SVR_EVT == eEvtGenType )	pQCtrl->UT_RcvSvrEvt( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )	pQCtrl->UT_SkillUse( uiOwnerId, (SSkillUseInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pQCtrl->UT_ColRgn( (sCOL_RGN_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pQCtrl->UT_RB( uiOwnerId, (SRB_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_CLICK_MOB == eEvtGenType )	pQCtrl->UT_ClickMob( uiOwnerId, (SMobTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_BIND_STONE == eEvtGenType )	pQCtrl->UT_BindStone();
			else if ( eEVENT_GEN_TYPE_SEARCH_QUEST == eEvtGenType )	pQCtrl->UT_SearchQuest();
			else if ( eEVENT_GEN_TYPE_ITEM_UPGRADE == eEvtGenType )	pQCtrl->UT_ItemUpgrade();
			else if ( eEVENT_GEN_TYPE_TELEPORT == eEvtGenType )		pQCtrl->UT_Teleport();
			else if ( eEVENT_GEN_TYPE_BUDOKAI == eEvtGenType )		pQCtrl->UT_Budokai();
			else if ( eEVENT_GEN_TYPE_SLOTMACHINE == eEvtGenType )	pQCtrl->UT_SlotMachine();
			else if ( eEVENT_GEN_TYPE_HOIPOIMIX == eEvtGenType )	pQCtrl->UT_HoipoiMix();
			else if ( eEVENT_GEN_TYPE_PRIVATESHOP == eEvtGenType )	pQCtrl->UT_PrivateShop();
			else if ( eEVENT_GEN_TYPE_FREEBATTLE == eEvtGenType )	pQCtrl->UT_FreeBattle();
			else if ( eEVENT_GEN_TYPE_ITEMIDENTITY == eEvtGenType )	pQCtrl->UT_ItemIdentity();
			else if ( eEVENT_GEN_TYPE_USEMAIL == eEvtGenType )		pQCtrl->UT_UseMail();
			else if ( eEVENT_GEN_TYPE_PARTY == eEvtGenType )		pQCtrl->UT_Party();

			else if (eEVENT_GEN_TYPE_CLICK_SIDEICON == eEvtGenType)	pQCtrl->UT_ClickSideIcon();
			else if (eEVENT_GEN_TYPE_LEVELCHECK == eEvtGenType)		pQCtrl->UT_LevelCheck();
			else if (eEVENT_GEN_TYPE_QUEST == eEvtGenType)			pQCtrl->UT_Quest();
			else if (eEVENT_GEN_TYPE_DIALOGOPEN == eEvtGenType)		pQCtrl->UT_DialogOpen();

			else
			{
				// Camera의 제어권이 넘어온 경우만 카메라를 빼준다
				if ( bTransCameraCtrlRight )
				{
					Logic_CancelNpcFacing();
				}
			}
		}
	}
}

void CDboTSCQAgency::UT_DoTutorialQuest( NTL_TS_T_ID tId )
{
	// Count the number of general quests and special quests (TMQ, Tutorial ...)
	int nGeneralQuestCnt = 0, nSpecialQuestCnt = 0;
	mapdef_TRIGGER_LIST::iterator itCurQuest = m_defCurTList.begin();
	for ( ; itCurQuest != m_defCurTList.end(); ++itCurQuest )
	{
		NTL_TS_T_ID tProgID = itCurQuest->first;

		if ( eQUEST_ID_RANGE_GENERAL_QUEST_MIN <= tProgID && tProgID < eQUEST_ID_RANGE_GENERAL_QUEST_MAX )
		{
			nGeneralQuestCnt++;
		}
		else if ( eQUEST_ID_RANGE_SPECIAL_QUEST_MIN <= tProgID && tProgID < eQUEST_ID_RANGE_SPECIAL_QUEST_MAX )
		{
			nSpecialQuestCnt++;
		}
		else
		{
			CNtlTSLog::Log( "Wrong quest id. Info[%d]. [%s]", tProgID, TS_CODE_TRACE() );
		}
	}

	// If you exceed the maximum number of quests you can receive, you can not share
	if ( eQUEST_ID_RANGE_GENERAL_QUEST_MIN <= tId && tId < eQUEST_ID_RANGE_GENERAL_QUEST_MAX )
	{
		if ( nGeneralQuestCnt >= eMAX_CAN_PROGRESS_GENERAL_QUEST_NUM )
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, "GAME_TS_WARNING_OVERFLOW_MAX_TS_NUM" );

			// Tutorial의 경우는 카메라를 빼준다
			Logic_CancelNpcFacing();

			return;
		}
	}
	else if ( eQUEST_ID_RANGE_SPECIAL_QUEST_MIN <= tId && tId < eQUEST_ID_RANGE_SPECIAL_QUEST_MAX )
	{
		if ( nSpecialQuestCnt >= eMAX_CAN_PROGRESS_SPECIAL_QUEST_NUM )
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, "GAME_TS_WARNING_OVERFLOW_MAX_TS_NUM" );

			// Tutorial의 경우는 카메라를 빼준다
			Logic_CancelNpcFacing();

			return;
		}
	}

	// Quests in progress can not be resumed
	CDboTSCQCtrl* pProgress = (CDboTSCQCtrl*)FindProgressTrigger( tId );
	if ( pProgress )
	{
		// For the Tutorial, remove the camera
		Logic_CancelNpcFacing();

		return;
	}

	CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindQuestFromTS( tId );
	if ( 0 == pTrig )
	{
		// For the Tutorial, remove the camera
		Logic_CancelNpcFacing();

		return;
	}

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)MakeTriggerController( pTrig );

	sCQRUN_PARAM sRunParam;
	sRunParam.SetControl( pQCtrl );
	sRunParam.SetAgency( this );

	// If the main container satisfies the start container, the quest proceeds
	NTL_TSRESULT tsResult = pTrig->RunTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, m_pRecv, &sRunParam );

	if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
	{
		m_defCurTList[pTrig->GetID()] = pQCtrl;

		// In the case of Tutorial, it assumes that camera control is taken and works
		pQCtrl->SetTransCameraCtrlRight( TRUE );

		pQCtrl = NULL;
	}

	if ( pQCtrl )
	{
		GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pQCtrl );
	}
}

void CDboTSCQAgency::TU_ShowProposalDialog( sTS_KEY& sKey, CDboTSContProposal* pCont, CDboTSContReward* pActRwd )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show proposal dialog message 처리중 [%d]", sKey.tID );

	if (pActRwd)
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		NTL_ASSERT(pSobAvatar != NULL, "Not exist avatar instance");

		BYTE byAvatarClass = Logic_GetPlayerClass(pSobAvatar);

		pActRwd->GetContRewardFinal(API_GetTableContainer(), byAvatarClass);
	}

	const sREWARD_INFO* pDefRwd = pActRwd ? pActRwd->GetDefRewardInfo() : NULL;
	const sREWARD_INFO* pSelRwd = pActRwd ? pActRwd->GetSelRewardInfo() : NULL;
	unsigned int uiExp = pActRwd ? pActRwd->GetRewardExp() : 0;
	unsigned int uiZeni = pActRwd ? pActRwd->GetRewardZeny() : 0;

	CNtlSLEventGenerator::QuestProposalDialog_Req( sKey,
												   pCont->GetDisplaySummary(),
												   pCont->GetTitle(),
												   pCont->GetGoal(),
												   pCont->GetSort(),
												   pCont->GetGrade(),
												   pCont->GetGradeType(),
												   pCont->GetContents(),
												   pDefRwd,
												   pSelRwd,
		uiExp, uiZeni);
}

void CDboTSCQAgency::UT_ShowProposalDialog( sTS_KEY& sKey, bool bAccept )
{
	OUT_QMSG_2( "[Quest] User -> Trigger : Show proposal dialog message 처리중 [%d, %d]", sKey.tID, bAccept );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ShowProposalDialog( sKey, bAccept );
}

void CDboTSCQAgency::TU_ShowRewardDialog( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl, CDboTSContReward* pContRwd )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show reward dialog message 처리중 [%d]", sKey.tID );

	if (pContRwd)
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		NTL_ASSERT(pSobAvatar != NULL, "Not exist avatar instance");

		BYTE byAvatarClass = Logic_GetPlayerClass(pSobAvatar);

		pContRwd->GetContRewardFinal(API_GetTableContainer(), byAvatarClass);
	}

	const sREWARD_INFO* pDefRwd = pContRwd ? pContRwd->GetDefRewardInfo() : NULL;
	const sREWARD_INFO* pSelRwd = pContRwd ? pContRwd->GetSelRewardInfo() : NULL;
	unsigned int uiExp = pContRwd ? pContRwd->GetRewardExp() : 0;
	unsigned int uiZeni = pContRwd ? pContRwd->GetRewardZeny() : 0;
	
	CNtlSLEventGenerator::QuestRewardDialog_Req( sKey,
												 pContRwd->GetRewardContType(),
												 pContRwd->GetLimitTime(),
												 pQCtrl->GetTitle(),
												 pQCtrl->GetGoal(),
												 pQCtrl->GetSort(),
												 pQCtrl->GetGrade(),
												 pQCtrl->GetGradeType(),
												 pContRwd->GetDesc(),
												 pDefRwd,
												 pSelRwd,
		uiExp, uiZeni);
}

void CDboTSCQAgency::UT_ShowRewardDialog( sTS_KEY& sKey, int nSelIdx, bool bCancel )
{
	OUT_QMSG_3( "[Quest] User -> Trigger : Show reward dialog message 처리중 [%d, %d, %d]", sKey.tID, nSelIdx, bCancel );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ShowRewardDialog( sKey, nSelIdx, bCancel );
}

void CDboTSCQAgency::TU_ShowUserSelectionDialog( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl, CDboTSContUsrSel* pCont )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show user selection dialog message 처리중 [%d]", sKey.tID );

	int nCnt;
	sTS_BRANCH_INFO sBranchInfo[eBRANCH_INFO_MAX];
	CDboTSContUsrSel::mapdef_NEXT_LINK_CIT cit = pCont->GetNextLinkBegin();
	CDboTSContUsrSel::mapdef_NEXT_LINK_CIT citEnd = pCont->GetNextLinkEnd();
	for ( nCnt = 0; cit != citEnd; ++cit )
	{
		if ( nCnt >= eBRANCH_INFO_MAX ) break;

		sBranchInfo[nCnt].tcID = cit->first;
		sBranchInfo[nCnt].uiBranchText = cit->second;
		nCnt++;
	}

	eUSER_SEL_TYPE eUSType = pCont->GetUserSelType();

	switch ( eUSType )
	{
	case eUSER_SEL_TYPE_GENERAL:
		{
			CNtlSLEventGenerator::QuestUserSelectDialog_Req( sKey, pCont->GetTargetType(), pCont->GetTargetTblIdx(), pQCtrl->GetTrigger()->GetTitle(), pCont->GetDesc(), nCnt, sBranchInfo );
		}
		break;

	case eUSER_SEL_TYPE_NARRATION:
		{
			CNtlSLEventGenerator::QuestNarrationUserSelectDialog_Req( sKey, pCont->GetUserSelProgState(), pCont->GetDesc(), nCnt, sBranchInfo );
		}
		break;

	default:
		{
			CNtlTSLog::Log( "Quest user select window type mismatch. Info[%d,%d]. [%s]", sKey.tID, sKey.tcID, TS_CODE_TRACE() );

			UT_ShowUserSelectionDialog( sKey, NTL_TS_TC_ID_INVALID, true );
		}
		break;
	}
}

void CDboTSCQAgency::UT_ShowUserSelectionDialog( sTS_KEY& sKey, NTL_TS_TC_ID tcSelId, bool bCancel )
{
	OUT_QMSG_3( "[Quest] User -> Trigger : Show user selection dialog message 처리중 [%d, %d, %d]", sKey.tID, tcSelId, bCancel );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ShowUserSelectionDialog( sKey, tcSelId, bCancel );
}

void CDboTSCQAgency::TU_ShowNarrationDialog( sTS_KEY& sKey, CDboTSContNarration* pCont )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show narration dialog message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::QuestNarrationDialog_Req( sKey,
													pCont->GetProgressState(),
													pCont->GetOwnerType(),
													pCont->GetOwnerIdx(),
													pCont->GetOwnerState(),
													pCont->GetDialogDirType(),
													pCont->GetDialog(),
													pCont->GetGUIType(),
													pCont->GetMaxLifeTime() );
}

void CDboTSCQAgency::UT_ShowNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	OUT_QMSG_2( "[Quest] User -> Trigger : Show narration dialog message 처리중 [%d, %d]", sKey.tID, bCancel );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ShowNarrationDialog( sKey, bCancel );
}

void CDboTSCQAgency::TU_ShowUnifiedNarrationDialog( sTS_KEY& sKey, CDboTSContUnifiedNarration* pCont )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show unified narration dialog message 처리중 [%d]", sKey.tID );

	CQuestNarrationTable* pQuestNarrationTbl = API_GetTableContainer()->GetQuestNarrationTable();
	sQUEST_NARRATION_TBLDAT* pData = (sQUEST_NARRATION_TBLDAT*) pQuestNarrationTbl->FindData( pCont->GetNarrationTblIdx() );
	if ( NULL == pData || pData->bType )
	{
		UT_ShowUnifiedNarrationDialog( sKey, true );
		return;
	}

	if ( m_pUnifiedQuestNarration )
	{
		CNtlTSLog::Log( "Overlapped unified quest narration. Info[%d, %d]. [%s]", sKey.tID, m_pUnifiedQuestNarration->GetTSKey().tID, TS_CODE_TRACE() );

		NTL_DELETE( m_pUnifiedQuestNarration );
	}

	m_pUnifiedQuestNarration = NTL_NEW CDboUnifiedQuestNarration;

	if ( !m_pUnifiedQuestNarration->Build( sKey, pData ) )
	{
		CNtlTSLog::Log( "Building Unified quest narration failed . Info[%d, %d]. [%s]", sKey.tID, m_pUnifiedQuestNarration->GetTSKey().tID, TS_CODE_TRACE() );

		UT_ShowUnifiedNarrationDialog( sKey, true );

		NTL_DELETE( m_pUnifiedQuestNarration );
	}
}

void CDboTSCQAgency::UT_ShowUnifiedNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	OUT_QMSG_2( "[Quest] User -> Trigger : Show unified narration dialog message 처리중 [%d, %d]", sKey.tID, bCancel );

	if ( bCancel )
	{
		NTL_DELETE( m_pUnifiedQuestNarration );

		CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

		if ( pQCtrl ) pQCtrl->UT_ShowUnifiedNarrationDialog( sKey, bCancel );
	}
	else
	{
		if ( m_pUnifiedQuestNarration &&
			(m_pUnifiedQuestNarration->GetTSKey().tID == sKey.tID) &&
			m_pUnifiedQuestNarration->IsUsing() )
		{
			m_pUnifiedQuestNarration->NextState( sKey );
		}
		else
		{
			NTL_DELETE( m_pUnifiedQuestNarration );

			CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

			if ( pQCtrl ) pQCtrl->UT_ShowUnifiedNarrationDialog( sKey, bCancel );
		}
	}
}

void CDboTSCQAgency::TU_ServerEvent( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show server event message 처리중 [%d]", sKey.tID );

	UT_ServerEvent( sKey );
}

void CDboTSCQAgency::UT_ServerEvent( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Show server event message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ServerEvent( sKey );
}

void CDboTSCQAgency::TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show npc conversation message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::QuestNPCDialog_Req( sKey, pAct->GetNPCConvType(), pAct->GetNPCIdx(), pAct->GetNPCConv() );
}

void CDboTSCQAgency::UT_ShowNPCConv( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Show npc conversation message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ShowNPCConv( sKey );
}

void CDboTSCQAgency::TU_RegisterQuestInfo( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Register quest info message 처리중 [%d]", sKey.tID );

	UT_RegisterQuestInfo( sKey );
}

void CDboTSCQAgency::UT_RegisterQuestInfo( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Register quest info message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_RegisterQuestInfo( sKey );
}

void CDboTSCQAgency::TU_CameraDirection( sTS_KEY& sKey, CDboTSActOPCam* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Camera message 처리중 [%d]", sKey.tID );

	UT_CameraDirection( sKey );
}

void CDboTSCQAgency::UT_CameraDirection( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Camera message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_CameraDirection( sKey );
}

void CDboTSCQAgency::TU_ShowObjConv( sTS_KEY& sKey, CDboTSActObjConv* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Show object conversation message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::QuestObjDialog_Req( sKey, pAct->GetObjConvType(), pAct->GetWorldTblIdx(), pAct->GetObjIdx(), pAct->GetObjConv() );
}

void CDboTSCQAgency::UT_ShowObjConv( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Show object conversation message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ShowObjConv( sKey );
}

void CDboTSCQAgency::TU_Hint( sTS_KEY& sKey, CDboTSActHint* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Hint message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::RegSideHintIcon( pAct->GetHintType(), pAct->GetTableIndex() );

	UT_Hint( sKey );
}

void CDboTSCQAgency::UT_Hint( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Hint message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_Hint( sKey );
}

void CDboTSCQAgency::TU_Open_Window( sTS_KEY& sKey, CDboTSActOpenWindow* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Open window message 처리중 [%d]", sKey.tID );

	switch ( pAct->GetWindowType() )
	{
	case eWINDOW_TYPE_TUTORIALHINT:
		CNtlSLEventGenerator::TutorialNfy( TUTORIAL_HINT, pAct->IsShow(), pAct->GetTableIdx() );
		break;
	case eWINDOW_TYPE_HELP:
		CNtlSLEventGenerator::TutorialNfy( TUTORIAL_HELP_DIALOG_SWITCH, pAct->IsShow() );
		break;
	case eWINDOW_TYPE_INVENTORY:
		CNtlSLEventGenerator::TutorialNfy( TUTORIAL_BAG_SWITCH, pAct->IsShow() );
		break;
	case eWINDOW_TYPE_SKILL:
		CNtlSLEventGenerator::TutorialNfy( TUTORIAL_SKILL_DIALOG_SWITCH, pAct->IsShow() );
		break;
	case eWINDOW_TYPE_CHARINFO:
		CNtlSLEventGenerator::TutorialNfy( TUTORIAL_CHARINFO_DIALOG_SWITCH, pAct->IsShow() );
		break;
	}

	UT_Open_Window( sKey );
}

void CDboTSCQAgency::UT_Open_Window( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Open window message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_Open_Window( sKey );
}

void CDboTSCQAgency::TU_PC_ConvNfy( sTS_KEY& sKey, CDboTSActPCConv* pAct )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : PC conversation message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::TSPCDialog_Nfy( pAct->GetConvTblIdx() );
}

void CDboTSCQAgency::TU_TelMudosa( sTS_KEY& sKey, CDboTSActTelMudosa* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Teleport Mudosa message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::TSMudosaTeleport_Req( sKey );
}

void CDboTSCQAgency::UT_TelMudosa( sTS_KEY& sKey, RwUInt8 byMudosaIndex, RwBool bCancel )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Teleport Mudosa message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_TelMudosa( sKey, byMudosaIndex, bCancel );
}

void CDboTSCQAgency::TU_RegCTiming( sTS_KEY& sKey, CDboTSActRegCTiming* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Register client timing message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );
	if ( NULL == pQCtrl )
	{
		UT_RegCTiming( sKey );
		return;
	}

	if ( pAct->IsRegisteMode() )
	{
		unsigned int uiFlags = pAct->GetFlags();

		if ( uiFlags & eREG_C_TIMING_MOB_CLICK_FLAG )
		{
			sREG_C_TIMING_DATA_MOB_CLICK* pData = NTL_NEW sREG_C_TIMING_DATA_MOB_CLICK;
			memcpy( pData, &pAct->GetMobClickData(), sizeof( sREG_C_TIMING_DATA_MOB_CLICK ) );

			pQCtrl->AddClientTimingData( eREG_C_TIMING_MOB_CLICK, pData );
		}
		if ( uiFlags & eREG_C_TIMING_NPC_CLICK_FLAG )
		{
			sREG_C_TIMING_DATA_NPC_CLICK* pData = NTL_NEW sREG_C_TIMING_DATA_NPC_CLICK;
			memcpy( pData, &pAct->GetNPCClickData(), sizeof( sREG_C_TIMING_DATA_NPC_CLICK ) );

			pQCtrl->AddClientTimingData( eREG_C_TIMING_NPC_CLICK, pData );
		}
		if ( uiFlags & eREG_C_TIMING_OBJ_CLICK_FLAG )
		{
			sREG_C_TIMING_DATA_OBJ_CLICK* pData = NTL_NEW sREG_C_TIMING_DATA_OBJ_CLICK;
			memcpy( pData, &pAct->GetObjClickData(), sizeof( sREG_C_TIMING_DATA_OBJ_CLICK ) );

			pQCtrl->AddClientTimingData( eREG_C_TIMING_OBJ_CLICK, pData );
		}
		if ( uiFlags & eREG_C_TIMING_ENTER_RGN_FLAG )
		{
			sREG_C_TIMING_DATA_ENTER_RGN* pData = NTL_NEW sREG_C_TIMING_DATA_ENTER_RGN;
			memcpy( pData, &pAct->GetEnterRgnData(), sizeof( sREG_C_TIMING_DATA_ENTER_RGN ) );

			pQCtrl->AddClientTimingData( eREG_C_TIMING_ENTER_RGN, pData );
		}
		if ( uiFlags & eREG_C_TIMING_GET_QITEM_FLAG )
		{
			sREG_C_TIMING_DATA_GET_QITEM* pData = NTL_NEW sREG_C_TIMING_DATA_GET_QITEM;
			memcpy( pData, &pAct->GetGetQItemData(), sizeof( sREG_C_TIMING_DATA_GET_QITEM ) );

			pQCtrl->AddClientTimingData( eREG_C_TIMING_GET_QITEM, pData );
		}
		if ( uiFlags & eREG_C_TIMING_QLISTWND_QTITLE_CLICK_FLAG )
		{
			pQCtrl->AddClientTimingData( eREG_C_TIMING_QLISTWND_QTITLE_CLICK, NULL );
		}
		if ( uiFlags & eREG_C_TIMING_ACCEPT_QUEST_FLAG )
		{
			pQCtrl->AddClientTimingData( eREG_C_TIMING_ACCEPT_QUEST, NULL );
		}
		if ( uiFlags & eREG_C_TIMING_SUCCESS_QUEST_FLAG )
		{
			pQCtrl->AddClientTimingData( eREG_C_TIMING_SUCCESS_QUEST, NULL );
		}
		if ( uiFlags & eREG_C_TIMING_PRE_REWARD_FLAG )
		{
			pQCtrl->AddClientTimingData( eREG_C_TIMING_PRE_REWARD, NULL );
		}
		if ( uiFlags & eREG_C_TIMING_POST_REWARD_FLAG )
		{
			pQCtrl->AddClientTimingData( eREG_C_TIMING_POST_REWARD, NULL );
		}

		pQCtrl->SetTimingExecutionData( pAct->GetExcuteType(), (uREG_C_TIMING_EXCUTE_DATA&)pAct->GetExcuteData() );
	}
	else
	{
		pQCtrl->ClearAllClientTimingData();
	}

	UT_RegCTiming( sKey );
}

void CDboTSCQAgency::UT_RegCTiming( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Register client timing message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_RegCTiming( sKey );
}

void CDboTSCQAgency::TU_ExcCGroup( sTS_KEY& sKey, CDboTSActExcCGroup* pAct )
{
	OUT_QMSG_1( "[Quest] Trigger -> User : Execute client group message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( NULL == pQCtrl )
	{
		UT_RegCTiming( sKey );
		return;
	}

	if ( pAct->IsRegister() )
	{
		NTL_TS_TG_ID tgID = pAct->GetExcuteClientTSGroupID();

		if ( tgID < NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN ||
			 tgID > NTL_TS_EXCEPT_CLIENT_GROUP_ID_END )
		{
			UT_RegCTiming( sKey );
			return;
		}

		pQCtrl->SetExcCGroup( tgID );

		pQCtrl->AttachUpdateExceptionGroup( tgID );
	}
	else
	{
		pQCtrl->SetExcCGroup( NTL_TS_TG_ID_INVALID );

		pQCtrl->ClearAllClientTimingData();
	}

	UT_RegCTiming( sKey );
}

void CDboTSCQAgency::UT_ExcCGroup( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Quest] User -> Trigger : Execute client group message 처리중 [%d]", sKey.tID );

	CDboTSCQCtrl* pQCtrl = (CDboTSCQCtrl*)FindProgressTrigger( sKey.tID );

	if ( pQCtrl ) pQCtrl->UT_ExcCGroup( sKey );
}


