#include "precomp_ntlsimulation.h"
#include "DboTSCTAgency.h"
#include "DboTSCTCtrl.h"
#include "DboTSCTRecv.h"
#include "DboTSCMain.h"
#include "NtlSLEventFunc.h"
#include "NtlResultCode.h"
#include "TableContainer.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "DboTSDefine.h"
#include "TableContainer.h"
#include "NtlSobAvatar.h"
#include "ObjectTable.h"
#include "NtlObject.h"
#include "NtlSobManager.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"
#include "MobTable.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "ObjectTable.h"
#include "QuestNarrationTable.h"
#include "DboUnifiedQuestNarration.h"
#include "ResultCodeString.h"

#include "NtlSLLogic.h"


/** 
	Client trigger agency
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCTAgency, CDboTSTAgency )


CDboTSCTAgency::CDboTSCTAgency( void )
{
	m_TriggerInfo.QuestInfoList.reserve( 10 );

	m_pUnifiedQuestNarration = NULL;

	LinkMsg( g_EventDoQuest );
	LinkMsg( g_EventQuestProposalDialog_Res );
	LinkMsg( g_EventQuestUserSelectDialog_Res );
	LinkMsg( g_EventQuestNarrationUserSelectDialog_Res );
	LinkMsg( g_EventQuestRewardDialog_Res );
	LinkMsg( g_EventQuestNarrationDialog_Res );

	LinkMsg( g_EventQuestNPCDialog_Res );
	LinkMsg( g_EventQuestObjDialog_Res );

	LinkMsg( g_EventTSItemUse );
	LinkMsg( g_EventTSItemGet );
	LinkMsg( g_EventTSItemEquip );
	LinkMsg( g_EventTSScoutUse );
	LinkMsg( g_EventTSSkillUse );
	LinkMsg( g_EventTSRankBattle );
	LinkMsg( g_EventTSColTriggerObject );

	LinkMsg( g_EventTSMudosaTeleport_Res );

	LinkMsg( g_EventSobTargetSelect );
	LinkMsg( g_EventSobAttackSelect );

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

CDboTSCTAgency::~CDboTSCTAgency( void )
{
	mapdef_TRIGGER_LIST::iterator it = m_defCurTList.begin();
	for ( ; it != m_defCurTList.end(); ++it )
	{
		CNtlTSControlObject* pObj = it->second;
		m_pParent->GetControlFactory()->DeleteObj( pObj );
	}
	m_defCurTList.clear();

	UnLinkMsg( g_EventDoQuest );
	UnLinkMsg( g_EventQuestProposalDialog_Res );
	UnLinkMsg( g_EventQuestUserSelectDialog_Res );
	UnLinkMsg( g_EventQuestNarrationUserSelectDialog_Res );
	UnLinkMsg( g_EventQuestRewardDialog_Res );
	UnLinkMsg( g_EventQuestNarrationDialog_Res );

	UnLinkMsg( g_EventQuestNPCDialog_Res );
	UnLinkMsg( g_EventQuestObjDialog_Res );

	UnLinkMsg( g_EventTSItemUse );
	UnLinkMsg( g_EventTSItemGet );
	UnLinkMsg( g_EventTSItemEquip );
	UnLinkMsg( g_EventTSScoutUse );
	UnLinkMsg( g_EventTSSkillUse );
	UnLinkMsg( g_EventTSRankBattle );
	UnLinkMsg( g_EventTSColTriggerObject );

	UnLinkMsg( g_EventTSMudosaTeleport_Res );

	UnLinkMsg( g_EventSobTargetSelect );
	UnLinkMsg( g_EventSobAttackSelect );

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

SGET_QUEST_INFO* CDboTSCTAgency::GetQuestInfoList( eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter /*= NULL*/ )
{
	m_TriggerInfo.dwType = 0;
	m_TriggerInfo.QuestInfoList.clear();

	//////////////////////////////////////////////////////////////////////////
	//
	// 현재 Object trigger에서는 바인드 및 포탈 가능 여부만 검사 함
	//
	//////////////////////////////////////////////////////////////////////////

	switch ( eEvtGenType )
	{
	case eEVENT_GEN_TYPE_COL_OBJECT:
	case eEVENT_GEN_TYPE_CLICK_OBJECT:
		{
			CObjectTable* pObjTable = API_GetTableContainer()->GetObjectTable( GetNtlSLGlobal()->GetAvatarInfo()->sWorldInfo.tblidx );
			if ( pObjTable )
			{
				sOBJECT_TBLDAT* pTblData = (sOBJECT_TBLDAT*)pObjTable->FindData( uiOwnerId /* 이벤트 타입이 오브젝트 관련 이므로 uiOwnerId는 오브젝트 테이블 인덱스가 된다*/ );

				if ( pTblData )
				{
					m_TriggerInfo.dwType = pTblData->dwFunction;
				}
			}
		}
		break;
	}

	sQUEST_INFO sTriggerInfo;

	// 1. 현재 진행 중인 트리거가 해당 오브젝트 아이디를 진행 이벤트로 사용할 수 있는지 검사한다
	mapdef_TRIGGER_LIST::iterator itCPT = m_defCurTList.begin();
	for ( ; itCPT != m_defCurTList.end(); ++itCPT )
	{
		bool bCan = false;
		CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)itCPT->second;

		if ( eEVENT_GEN_TYPE_CLICK_NPC == eEvtGenType )			bCan = pTCtrl->CanProgressClickNPC( uiOwnerId );
		else if ( eEVENT_GEN_TYPE_CLICK_OBJECT == eEvtGenType )	bCan = pTCtrl->CanProgressClickObject( uiOwnerId );
		else if ( eEVENT_GEN_TYPE_COL_OBJECT == eEvtGenType )	bCan = pTCtrl->CanProgressCollisionObject( uiOwnerId );
		else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		bCan = pTCtrl->CanProgressItemUse( uiOwnerId, (vecdef_ItemTargetList*) pParameter );
		else if ( eEVENT_GEN_TYPE_ITEM_GET == eEvtGenType )		bCan = pTCtrl->CanProgressItemGet( uiOwnerId );
		else if ( eEVENT_GEN_TYPE_ITEM_EQUIP == eEvtGenType )	bCan = pTCtrl->CanProgressItemEquip( uiOwnerId );
		else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	bCan = pTCtrl->CanProgressScoutUse( uiOwnerId, (SScouterUseTargetInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_RCV_SVR_EVT == eEvtGenType )	bCan = pTCtrl->CanProgressRcvSvrEvt( uiOwnerId );
		else if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )	bCan = pTCtrl->CanProgressSkillUse( uiOwnerId, (SSkillUseInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	bCan = pTCtrl->CanProgressColRgn( (sCOL_RGN_DATA*) pParameter );
		else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			bCan = pTCtrl->CanProgressRB( uiOwnerId, (SRB_DATA*) pParameter );
		else if ( eEVENT_GEN_TYPE_CLICK_MOB == eEvtGenType )	bCan = pTCtrl->CanProgressClickMob( uiOwnerId, (SMobTargetInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_BIND_STONE == eEvtGenType )	bCan = pTCtrl->CanProgressBindStone();
		else if ( eEVENT_GEN_TYPE_SEARCH_QUEST == eEvtGenType )	bCan = pTCtrl->CanProgressSearchQuest();
		else if ( eEVENT_GEN_TYPE_ITEM_UPGRADE == eEvtGenType )	bCan = pTCtrl->CanProgressItemUpgrade();
		else if ( eEVENT_GEN_TYPE_TELEPORT == eEvtGenType )		bCan = pTCtrl->CanProgressTeleport();
		else if ( eEVENT_GEN_TYPE_BUDOKAI == eEvtGenType )		bCan = pTCtrl->CanProgressBudokai();
		else if ( eEVENT_GEN_TYPE_SLOTMACHINE == eEvtGenType )	bCan = pTCtrl->CanProgressSlotMachine();
		else if ( eEVENT_GEN_TYPE_HOIPOIMIX == eEvtGenType )	bCan = pTCtrl->CanProgressHoipoiMix();
		else if ( eEVENT_GEN_TYPE_PRIVATESHOP == eEvtGenType )	bCan = pTCtrl->CanProgressPrivateShop();
		else if ( eEVENT_GEN_TYPE_FREEBATTLE == eEvtGenType )	bCan = pTCtrl->CanProgressFreeBattle();
		else if ( eEVENT_GEN_TYPE_ITEMIDENTITY == eEvtGenType )	bCan = pTCtrl->CanProgressItemIdentity();
		else if ( eEVENT_GEN_TYPE_USEMAIL == eEvtGenType )		bCan = pTCtrl->CanProgressUseMail();
		else if ( eEVENT_GEN_TYPE_PARTY == eEvtGenType )		bCan = pTCtrl->CanProgressParty();

		else if (eEVENT_GEN_TYPE_FLINK == eEvtGenType)		bCan = pTCtrl->CanProgressFLink();
		else if (eEVENT_GEN_TYPE_CLICK_SIDEICON == eEvtGenType)		bCan = pTCtrl->CanProgressClickSideIcon();
		else if (eEVENT_GEN_TYPE_LEVELCHECK == eEvtGenType)		bCan = pTCtrl->CanProgressLevelCheck();
		else if (eEVENT_GEN_TYPE_QUEST == eEvtGenType)		bCan = pTCtrl->CanProgressQuest();
		else if (eEVENT_GEN_TYPE_DIALOGOPEN == eEvtGenType)		bCan = pTCtrl->CanProgressDialogOpen();
		

		else return &m_TriggerInfo;

		if ( bCan )
		{
			sTriggerInfo.sKey.Init();

			sTriggerInfo.sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;
			sTriggerInfo.sKey.tID = pTCtrl->GetTrigger()->GetID();
			sTriggerInfo.sKey.tcID = pTCtrl->GetCurTSP()->GetID();
			sTriggerInfo.dwQuestTitle = pTCtrl->GetTrigger()->GetTitle();
			sTriggerInfo.dwQuestStartDialog = 0xffffffff;
			sTriggerInfo.eProgType = eQUEST_PROGRESS_TYPE_PROGRESS;	// PC Trigger인 경우 무조건 진행 상태로 표현한다

			m_TriggerInfo.QuestInfoList.push_back( sTriggerInfo );
		}
	}

	// 2. 해당 오브젝트 아이디로 시작 할 수 있는 트러거가 존재하면 시작한다
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
		return &m_TriggerInfo;
	}

	const std::vector<NTL_TS_T_ID>* pSTrigList = pEvtMapper->FindTSList( uiOwnerId, "pctrigger" );
	if ( NULL == pSTrigList || pSTrigList->empty() ) return &m_TriggerInfo;

	sCTRUN_PARAM sRunParam;
	sRunParam.SetAgency( this );

	std::vector<NTL_TS_T_ID>::const_iterator itSQ;

	for ( itSQ = pSTrigList->begin(); itSQ != pSTrigList->end(); ++itSQ )
	{
		// 현재 진행중인 퀘스트이면 더 이상 진행 할 수 없다
		if ( m_defCurTList.find( *itSQ ) != m_defCurTList.end() ) continue;

		CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindTriggerFromTS( *itSQ );
		if ( 0 == pTrig ) continue;

		CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)MakeTriggerController( pTrig );

		// Main group의 start container(시작 컨테이너)를 만족하면 퀘스트 진행

		sRunParam.SetControl( pTCtrl );

		pTCtrl->SetEventGenType( eEvtGenType );
		pTCtrl->SetEventGenId( uiOwnerId );
		if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )			pTCtrl->SetEventGenSkillUseInfo( (SSkillUseInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pTCtrl->SetEventGenItemTargetInfo( (vecdef_ItemTargetList*) pParameter );
		else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pTCtrl->SetEventGenScouterUseTargetInfo( (SScouterUseTargetInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pTCtrl->SetEventGenColRgnInfo( (sCOL_RGN_DATA*) pParameter );
		else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pTCtrl->SetEventGenRBData( (SRB_DATA*) pParameter );

		NTL_TSRESULT tsResult = pTCtrl->RunTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, m_pRecv, &sRunParam );

		pTCtrl->ClearAllEventGenInfo();

		if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
		{
			sTriggerInfo.sKey.Init();

			sTriggerInfo.sKey.byTSType = TS_TYPE_PC_TRIGGER_CS;
			sTriggerInfo.sKey.tID = pTrig->GetID();
			sTriggerInfo.eProgType = eQUEST_PROGRESS_TYPE_NEW;
			sTriggerInfo.dwQuestTitle = pTrig->GetTitle();
			CDboTSContStart* pStartCont = (CDboTSContStart*)pTrig->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( START_CONTAINER_ID );
			sTriggerInfo.dwQuestStartDialog = pStartCont ? pStartCont->GetSTDiag() : 0xffffffff;

			m_TriggerInfo.QuestInfoList.push_back( sTriggerInfo );
		}

		GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pTCtrl );
	}

	return &m_TriggerInfo;
}

void CDboTSCTAgency::Update( void )
{
	CDboTSTAgency::Update();

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

void CDboTSCTAgency::HandleEvents( RWS::CMsg& Msg )
{
	if ( Msg.Id == g_EventDoQuest )
	{
		SNtlEventDoQuest* pEventData = (SNtlEventDoQuest*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sQuestInfo.sKey.byTSType ) return;

		UT_EventDoTrigger( pEventData->sQuestInfo, pEventData->eEventGenType, pEventData->uiOwnerID );
	}
	else if ( Msg.Id == g_EventQuestProposalDialog_Res )
	{
		SNtlEventQuestProposalDialog_Res* pEventData = (SNtlEventQuestProposalDialog_Res*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowProposalDialog( pEventData->sTSKey, pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestUserSelectDialog_Res )
	{
		SNtlEventQuestUserSelectDialog_Res* pEventData = (SNtlEventQuestUserSelectDialog_Res*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowUserSelectionDialog( pEventData->sTSKey, pEventData->tcSelID, !pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestNarrationUserSelectDialog_Res )
	{
		SNtlEventQuestNarrationUserSelectDialog_Res* pEventData = (SNtlEventQuestNarrationUserSelectDialog_Res*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowUserSelectionDialog( pEventData->sTSKey, pEventData->tcSelID, !pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestRewardDialog_Res )
	{
		SNtlEventQuestRewardDialog_Res* pEventData = (SNtlEventQuestRewardDialog_Res*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowRewardDialog( pEventData->sTSKey, pEventData->nSelRwdIdx, !pEventData->bResult );
	}
	else if ( Msg.Id == g_EventQuestNarrationDialog_Res )
	{
		SNtlEventQuestNarrationDialog_Res* pEventData = (SNtlEventQuestNarrationDialog_Res*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

		CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( pEventData->sTSKey.tID );

		if ( pTCtrl && pTCtrl->GetCurTSP()->GetEntityType() == DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION )
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
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

		UT_ShowNPCConv( pEventData->sTSKey );
	}
	else if ( Msg.Id == g_EventQuestObjDialog_Res )
	{
		SNtlEventQuestObjDialog_Res* pEventData = (SNtlEventQuestObjDialog_Res*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

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

		int nCnt = pQuestInfo->QuestInfoList.size();
		for ( int i = 0; i < nCnt; ++i )
		{
			sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
			UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_ITEM_USE, pServerData->itemTblidx, &defItemTargetList );
		}
	}
	else if ( Msg.Id == g_EventTSItemGet )
	{
		SNtlEventTSItemGet* pEventData = (SNtlEventTSItemGet*) Msg.pData;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEM_GET, pEventData->uiItemIdx );

		int nCnt = pQuestInfo->QuestInfoList.size();
		for ( int i = 0; i < nCnt; ++i )
		{
			sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
			UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_ITEM_GET, pEventData->uiItemIdx );
		}
	}
	else if ( Msg.Id == g_EventTSItemEquip )
	{
		SNtlEventTSItemEquip* pEventData = (SNtlEventTSItemEquip*) Msg.pData;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEM_EQUIP, pEventData->uiItemIdx );

		int nCnt = pQuestInfo->QuestInfoList.size();
		for ( int i = 0; i < nCnt; ++i )
		{
			sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
			UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_ITEM_EQUIP, pEventData->uiItemIdx );
		}
	}
	else if ( Msg.Id == g_EventTSScoutUse )
	{
		SNtlEventTSScoutUse* pEventData = (SNtlEventTSScoutUse*) Msg.pData;

		SScouterUseTargetInfo sScoutTargetInfo;
		sScoutTargetInfo.uiClassID = pEventData->uiTargetClassID;
		sScoutTargetInfo.tblIdx = pEventData->uiTargetIdx;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_SCOUT_USE, pEventData->uiItemIdx, &sScoutTargetInfo );

		int nCnt = pQuestInfo->QuestInfoList.size();
		for ( int i = 0; i < nCnt; ++i )
		{
			SScouterUseTargetInfo sScoutTargetInfo2;
			sScoutTargetInfo2.uiClassID = pEventData->uiTargetClassID;
			sScoutTargetInfo2.tblIdx = pEventData->uiTargetIdx;

			sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
			UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_SCOUT_USE, pEventData->uiItemIdx, &sScoutTargetInfo2 );
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

		int nCnt = pQuestInfo->QuestInfoList.size();
		for ( int i = 0; i < nCnt; ++i )
		{
			sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
			UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_SKILL_USE, pEventData->uiSkillIdx, &sSkillUseInfo );
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
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_RB, pEventData->uiRBTblIdx, &sRBData );
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
					UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_COL_OBJECT, tblIdx );
				}
			}
		}
	}
	else if ( Msg.Id == g_EventTSMudosaTeleport_Res )
	{
		SNtlEventTSMudosaTeleport_Res* pEventData = (SNtlEventTSMudosaTeleport_Res*) Msg.pData;
		if ( TS_TYPE_PC_TRIGGER_CS != pEventData->sTSKey.byTSType ) return;

		UT_TelMudosa( pEventData->sTSKey, pEventData->byMudosaIndex, pEventData->bCancel );
	}
	else if ( Msg.Id == g_EventSobTargetSelect )
	{
		SNtlEventSobTargetSelect* pEventData = (SNtlEventSobTargetSelect*) Msg.pData;

		CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pEventData->hSerialId );
		if ( NULL == pSobObj ||
			pSobObj->GetClassID() != SLCLASS_MONSTER )
		{
			return;
		}

		DWORD dwMobGroup = ((CNtlSobMonsterAttr*)((CNtlSobMonster*)pSobObj)->GetSobAttr())->GetMobTbl()->dwMobGroup;

		SMobTargetInfo sMobTargetInfo;
		sMobTargetInfo.hTarget = pEventData->hSerialId;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );

		int nCnt = pQuestInfo->QuestInfoList.size();
		for ( int i = 0; i < nCnt; ++i )
		{
			sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
			UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );
		}
	}
	else if ( Msg.Id == g_EventSobAttackSelect )
	{
		SNtlEventSobAttackSelect* pEventData = (SNtlEventSobAttackSelect*) Msg.pData;

		CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pEventData->hSerialId );
		if ( NULL == pSobObj ||
			pSobObj->GetClassID() != SLCLASS_MONSTER )
		{
			return;
		}

		DWORD dwMobGroup = ((CNtlSobMonsterAttr*)((CNtlSobMonster*)pSobObj)->GetSobAttr())->GetMobTbl()->dwMobGroup;

		SMobTargetInfo sMobTargetInfo;
		sMobTargetInfo.hTarget = pEventData->hSerialId;

		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );

		int nCnt = pQuestInfo->QuestInfoList.size();
		for ( int i = 0; i < nCnt; ++i )
		{
			sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
			UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_CLICK_MOB, dwMobGroup, &sMobTargetInfo );
		}
	}
	else if ( Msg.Id == g_EventTSBindStone)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_BIND_STONE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_BIND_STONE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSSearchQuest)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_SEARCH_QUEST, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_SEARCH_QUEST, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSItemUpgrade)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEM_UPGRADE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_ITEM_UPGRADE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSTeleport)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_TELEPORT, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_TELEPORT, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSBudokai)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_BUDOKAI, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_BUDOKAI, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSSlotMachine)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_SLOTMACHINE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_SLOTMACHINE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSHoipoiMix)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_HOIPOIMIX, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_HOIPOIMIX, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSPrivateShop)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_PRIVATESHOP, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_PRIVATESHOP, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSFreeBattle)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_FREEBATTLE, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_FREEBATTLE, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSItemIdentity)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_ITEMIDENTITY, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_ITEMIDENTITY, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSUseMail)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_USEMAIL, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_USEMAIL, 0 );
		}
	}
	else if ( Msg.Id == g_EventTSParty)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_PARTY, 0 );

		if ( !pQuestInfo->QuestInfoList.empty() )
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_PARTY, 0 );
		}
	}

	else if (Msg.Id == g_EventTSClickSideIcon)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_CLICK_SIDEICON, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger(sQuestInfo, eEVENT_GEN_TYPE_CLICK_SIDEICON, 0);
		}
	}
	else if (Msg.Id == g_EventTSLevelCheck)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_LEVELCHECK, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger(sQuestInfo, eEVENT_GEN_TYPE_LEVELCHECK, 0);
		}
	}
	else if (Msg.Id == g_EventTSQuest)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_QUEST, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger(sQuestInfo, eEVENT_GEN_TYPE_QUEST, 0);
		}
	}
	else if (Msg.Id == g_EventTSDialogOpen)
	{
		SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList(eEVENT_GEN_TYPE_DIALOGOPEN, 0);

		if (!pQuestInfo->QuestInfoList.empty())
		{
			sQUEST_INFO& sQuestInfo = pQuestInfo->QuestInfoList[0];
			UT_EventDoTrigger(sQuestInfo, eEVENT_GEN_TYPE_DIALOGOPEN, 0);
		}
	}
}

CDboTSTCtrl* CDboTSCTAgency::MakeTriggerController( CNtlTSTrigger* pTrig )
{
	// Trigger controller 를 생성한다
	CNtlTSControlObject* pCtrlObj = GetParent()->GetControlFactory()->CreateObj( "CDboTSCTCtrl" );
	if ( !pCtrlObj->IsDerivedClass( "CDboTSCTCtrl" ) )
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSCTCtrl. Info[%s]. [%s]", pCtrlObj->GetClassName(), TS_CODE_TRACE() );
		m_pParent->GetControlFactory()->DeleteObj( pCtrlObj );
		return 0;
	}

	// TS trigger 와 Trigger를 연결 및 저장한다
	((CDboTSCTCtrl*)pCtrlObj)->SetTrigger( pTrig );
	((CDboTSCTCtrl*)pCtrlObj)->SetParent( this );

	return (CDboTSCTCtrl*)pCtrlObj;
}

CDboTSCTAgency::sCOL_TRIG_OBJ_INFO* CDboTSCTAgency::FindColTrigObj( unsigned int hHandle )
{
	mapdef_COL_TRIG_OBJ_LIST::iterator it = m_defColTrigObjCache.find( hHandle );
	if ( it == m_defColTrigObjCache.end() ) return NULL;
	return &it->second;
}

void CDboTSCTAgency::AttachColTrigObj( unsigned int hHandle )
{
	if ( NULL == FindColTrigObj( hHandle ) )
	{
		sCOL_TRIG_OBJ_INFO sInfo;

		sInfo.hHandle = hHandle;
		sInfo.uiStandTime = GetTickCount();

		m_defColTrigObjCache[hHandle] = sInfo;
	}
}

void CDboTSCTAgency::UG_Avatar_TS_Confirm_Step( NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, unsigned int* uiParam, unsigned char byEventType, unsigned int uiEventData )
{
	OUT_QMSG_6( "[Trigger] User -> GameServer : Confirm 처리중 [%d,%d,%d,%d,%d,%d]", tId, tcCurId, tcNextId, uiParam, byEventType, uiEventData );

	// 서버에게 메시지를 보낸다
	API_GetSLPacketGenerator()->SendTSConfirmStepReq( TS_TYPE_PC_TRIGGER_CS, tId, tcCurId, tcNextId, uiParam, byEventType, uiEventData );
}

void CDboTSCTAgency::GU_Avatar_TS_Confirm_Step( WORD wResultCode, NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId )
{
	OUT_QMSG_3( "[Trigger] GameServer -> User : Confirm 처리중 [%d,%d,%d]", tId, tcCurId, tcNextId );

	if ( RESULT_SUCCESS != wResultCode )
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(wResultCode, "").c_str());
	}

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( tId );

	if ( pTCtrl ) pTCtrl->GU_Avatar_TS_Confirm_Step( wResultCode, tcCurId, tcNextId );
}

void CDboTSCTAgency::GU_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	OUT_QMSG_4( "[Trigger] GameServer -> User : Update TS state message 처리중 [%d, %d, %d, %d]", tId, byType, wTSState, uiParam );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( tId );

	if ( pTCtrl ) pTCtrl->GU_TS_Update_State( byType, wTSState, uiParam );
}

void CDboTSCTAgency::UG_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam )
{
	OUT_QMSG_4( "[Trigger] User -> GameServer : Update TS state message 처리중 [%d, %d, %d, %d]", tId, byType, wTSState, uiParam );

	// 서버에게 상태 업데이트 메시지를 전송한다
	API_GetSLPacketGenerator()->SendTSUpdateState( tId, TS_TYPE_PC_TRIGGER_CS, byType, wTSState, uiParam );
}

void CDboTSCTAgency::GU_TS_Update_Event_Nfy( NTL_TS_EVENT_ID eID )
{
	OUT_QMSG_1( "[Trigger] GameServer -> User : Update TS Event 처리중 [%d]", eID );

	SGET_QUEST_INFO* pQuestInfo = GetQuestInfoList( eEVENT_GEN_TYPE_RCV_SVR_EVT, eID );

	int nCnt = pQuestInfo->QuestInfoList.size();
	for ( int i = 0; i < nCnt; ++i )
	{
		sQUEST_INFO& Info = pQuestInfo->QuestInfoList[i];
		UT_EventDoTrigger( Info, eEVENT_GEN_TYPE_RCV_SVR_EVT, eID );
	}
}

void CDboTSCTAgency::UT_UpdateAvatarPos( unsigned int uiWorldIdx, float fPosX, float fPosZ )
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
				UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );
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
				UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );
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
				UT_EventDoTrigger( sQuestInfo, eEVENT_GEN_TYPE_COL_REGION, 0, &sColRgnData );
			}

			m_sAvatarCurPos.uiWorldIx = uiWorldIdx;
			m_sAvatarCurPos.fPosX = fPosX;
			m_sAvatarCurPos.fPosZ = fPosZ;
		}
	}
}

void CDboTSCTAgency::TU_FinishQuest( unsigned char byTSType, NTL_TS_T_ID tID )
{
	CNtlSLEventGenerator::TSFinished( byTSType, tID );
}

void CDboTSCTAgency::UT_EventDoTrigger( sQUEST_INFO& sQuestInfo, eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter /*= NULL*/ )
{
	OUT_QMSG_3( "[Trigger] User -> Trigger : Do trigger message 처리중 [%d, %d, %d]", sQuestInfo.sKey.tID, eEvtGenType, uiOwnerId );

	if ( eQUEST_PROGRESS_TYPE_NEW == sQuestInfo.eProgType )
	{
		if ( m_defCurTList.size() >= eMAX_CAN_PROGRESS_TRIGGER_NUM )
		{
			return;
		}

		// 진행 중에 있는 트리거는 새로이 진행할 수 없다
		CDboTSCTCtrl* pProgress = (CDboTSCTCtrl*)FindProgressTrigger( sQuestInfo.sKey.tID );
		if ( pProgress ) return;

		CNtlTSTrigger* pTrig = ((CDboTSCMain*)m_pParent)->FindTriggerFromTS( sQuestInfo.sKey.tID );
		if ( 0 == pTrig ) return;

		CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)MakeTriggerController( pTrig );

		sCTRUN_PARAM sRunParam;
		sRunParam.SetControl( pTCtrl );
		sRunParam.SetAgency( this );

		// Main group의 start container(시작 컨테이너)를 만족하면 퀘스트 진행

		pTCtrl->SetEventGenType( eEvtGenType );
		pTCtrl->SetEventGenId( uiOwnerId );
		if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )			pTCtrl->SetEventGenSkillUseInfo( (SSkillUseInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pTCtrl->SetEventGenItemTargetInfo( (vecdef_ItemTargetList*) pParameter );
		else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pTCtrl->SetEventGenScouterUseTargetInfo( (SScouterUseTargetInfo*) pParameter );
		else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pTCtrl->SetEventGenColRgnInfo( (sCOL_RGN_DATA*) pParameter );
		else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pTCtrl->SetEventGenRBData( (SRB_DATA*) pParameter );

		NTL_TSRESULT tsResult = pTrig->RunTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, m_pRecv, &sRunParam );

		pTCtrl->ClearAllEventGenInfo();

		if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
		{
			m_defCurTList[pTrig->GetID()] = pTCtrl;

			if ( eEVENT_GEN_TYPE_CLICK_NPC == eEvtGenType )			pTCtrl->UT_ClickNPC( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_CLICK_OBJECT == eEvtGenType )	pTCtrl->UT_ClickObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_COL_OBJECT == eEvtGenType )	pTCtrl->UT_CollisionObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pTCtrl->UT_ItemUse( uiOwnerId, (vecdef_ItemTargetList*) pParameter );
			else if ( eEVENT_GEN_TYPE_ITEM_GET == eEvtGenType )		pTCtrl->UT_ItemGet( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_EQUIP == eEvtGenType )	pTCtrl->UT_ItemEquip( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pTCtrl->UT_ScoutUse( uiOwnerId, (SScouterUseTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_RCV_SVR_EVT == eEvtGenType )	pTCtrl->UT_RcvSvrEvt( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )	pTCtrl->UT_SkillUse( uiOwnerId, (SSkillUseInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pTCtrl->UT_ColRgn( (sCOL_RGN_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pTCtrl->UT_RB( uiOwnerId, (SRB_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_CLICK_MOB == eEvtGenType )	pTCtrl->UT_ClickMob( uiOwnerId, (SMobTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_BIND_STONE == eEvtGenType )	pTCtrl->UT_BindStone();
			else if ( eEVENT_GEN_TYPE_SEARCH_QUEST == eEvtGenType )	pTCtrl->UT_SearchQuest();
			else if ( eEVENT_GEN_TYPE_ITEM_UPGRADE == eEvtGenType )	pTCtrl->UT_ItemUpgrade();
			else if ( eEVENT_GEN_TYPE_TELEPORT == eEvtGenType )		pTCtrl->UT_Teleport();
			else if ( eEVENT_GEN_TYPE_BUDOKAI == eEvtGenType )		pTCtrl->UT_Budokai();
			else if ( eEVENT_GEN_TYPE_SLOTMACHINE == eEvtGenType )	pTCtrl->UT_SlotMachine();
			else if ( eEVENT_GEN_TYPE_HOIPOIMIX == eEvtGenType )	pTCtrl->UT_HoipoiMix();
			else if ( eEVENT_GEN_TYPE_PRIVATESHOP == eEvtGenType )	pTCtrl->UT_PrivateShop();
			else if ( eEVENT_GEN_TYPE_FREEBATTLE == eEvtGenType )	pTCtrl->UT_FreeBattle();
			else if ( eEVENT_GEN_TYPE_ITEMIDENTITY == eEvtGenType )	pTCtrl->UT_ItemIdentity();
			else if ( eEVENT_GEN_TYPE_USEMAIL == eEvtGenType )		pTCtrl->UT_UseMail();
			else if ( eEVENT_GEN_TYPE_PARTY == eEvtGenType )		pTCtrl->UT_Party();

			else if (eEVENT_GEN_TYPE_CLICK_SIDEICON == eEvtGenType)	pTCtrl->UT_ClickSideIcon();
			else if (eEVENT_GEN_TYPE_LEVELCHECK == eEvtGenType)		pTCtrl->UT_LevelCheck();
			else if (eEVENT_GEN_TYPE_QUEST == eEvtGenType)			pTCtrl->UT_Quest();
			else if (eEVENT_GEN_TYPE_DIALOGOPEN == eEvtGenType)		pTCtrl->UT_DialogOpen();

			pTCtrl = NULL;
		}

		if ( pTCtrl )
		{
			GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pTCtrl );
		}
	}
	else
	{
		CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sQuestInfo.sKey.tID );

		if( pTCtrl->GetCurTSP()->GetID() == sQuestInfo.sKey.tcID )
		{
			if ( eEVENT_GEN_TYPE_CLICK_NPC == eEvtGenType )			pTCtrl->UT_ClickNPC( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_CLICK_OBJECT == eEvtGenType )	pTCtrl->UT_ClickObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_COL_OBJECT == eEvtGenType )	pTCtrl->UT_CollisionObject( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_USE == eEvtGenType )		pTCtrl->UT_ItemUse( uiOwnerId, (vecdef_ItemTargetList*) pParameter );
			else if ( eEVENT_GEN_TYPE_ITEM_GET == eEvtGenType )		pTCtrl->UT_ItemGet( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_ITEM_EQUIP == eEvtGenType )	pTCtrl->UT_ItemEquip( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SCOUT_USE == eEvtGenType )	pTCtrl->UT_ScoutUse( uiOwnerId, (SScouterUseTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_RCV_SVR_EVT == eEvtGenType )	pTCtrl->UT_RcvSvrEvt( uiOwnerId );
			else if ( eEVENT_GEN_TYPE_SKILL_USE == eEvtGenType )	pTCtrl->UT_SkillUse( uiOwnerId, (SSkillUseInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_COL_REGION == eEvtGenType )	pTCtrl->UT_ColRgn( (sCOL_RGN_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_RB == eEvtGenType )			pTCtrl->UT_RB( uiOwnerId, (SRB_DATA*) pParameter );
			else if ( eEVENT_GEN_TYPE_CLICK_MOB == eEvtGenType )	pTCtrl->UT_ClickMob( uiOwnerId, (SMobTargetInfo*) pParameter );
			else if ( eEVENT_GEN_TYPE_BIND_STONE == eEvtGenType )	pTCtrl->UT_BindStone();
			else if ( eEVENT_GEN_TYPE_SEARCH_QUEST == eEvtGenType )	pTCtrl->UT_SearchQuest();
			else if ( eEVENT_GEN_TYPE_ITEM_UPGRADE == eEvtGenType )	pTCtrl->UT_ItemUpgrade();
			else if ( eEVENT_GEN_TYPE_TELEPORT == eEvtGenType )		pTCtrl->UT_Teleport();
			else if ( eEVENT_GEN_TYPE_BUDOKAI == eEvtGenType )		pTCtrl->UT_Budokai();
			else if ( eEVENT_GEN_TYPE_SLOTMACHINE == eEvtGenType )	pTCtrl->UT_SlotMachine();
			else if ( eEVENT_GEN_TYPE_HOIPOIMIX == eEvtGenType )	pTCtrl->UT_HoipoiMix();
			else if ( eEVENT_GEN_TYPE_PRIVATESHOP == eEvtGenType )	pTCtrl->UT_PrivateShop();
			else if ( eEVENT_GEN_TYPE_FREEBATTLE == eEvtGenType )	pTCtrl->UT_FreeBattle();
			else if ( eEVENT_GEN_TYPE_ITEMIDENTITY == eEvtGenType )	pTCtrl->UT_ItemIdentity();
			else if ( eEVENT_GEN_TYPE_USEMAIL == eEvtGenType )		pTCtrl->UT_UseMail();
			else if ( eEVENT_GEN_TYPE_PARTY == eEvtGenType )		pTCtrl->UT_Party();
			
			else if (eEVENT_GEN_TYPE_CLICK_SIDEICON == eEvtGenType)	pTCtrl->UT_ClickSideIcon();
			else if (eEVENT_GEN_TYPE_LEVELCHECK == eEvtGenType)		pTCtrl->UT_LevelCheck();
			else if (eEVENT_GEN_TYPE_QUEST == eEvtGenType)			pTCtrl->UT_Quest();
			else if (eEVENT_GEN_TYPE_DIALOGOPEN == eEvtGenType)		pTCtrl->UT_DialogOpen();
		}
	}
}

void CDboTSCTAgency::TU_ShowProposalDialog( sTS_KEY& sKey, CDboTSContProposal* pCont, CDboTSContReward* pActRwd )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Show proposal dialog message 처리중 [%d]", sKey.tID );

	if (pActRwd)
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		NTL_ASSERT(pSobAvatar != NULL, "Not exist avatar instance");

		BYTE byAvatarClass = Logic_GetPlayerClass(pSobAvatar);

		pActRwd->GetContRewardFinal(API_GetTableContainer(), byAvatarClass);
	}

	const sREWARD_INFO* pDefRwd = pActRwd ? pActRwd->GetDefRewardInfo() : NULL;
	const sREWARD_INFO* pSelRwd = pActRwd ? pActRwd->GetSelRewardInfo() : NULL;

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
		pActRwd->GetRewardExp(), pActRwd->GetRewardZeny());
}

void CDboTSCTAgency::UT_ShowProposalDialog( sTS_KEY& sKey, bool bAccept )
{
	OUT_QMSG_2( "[Trigger] User -> Trigger : Show proposal dialog message 처리중 [%d, %d]", sKey.tID, bAccept );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_ShowProposalDialog( sKey, bAccept );
}

void CDboTSCTAgency::TU_ShowRewardDialog( sTS_KEY& sKey, CDboTSCTCtrl* pTCtrl, CDboTSContReward* pContRwd )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Show reward dialog message 처리중 [%d]", sKey.tID );

	if (pContRwd)
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		NTL_ASSERT(pSobAvatar != NULL, "Not exist avatar instance");

		BYTE byAvatarClass = Logic_GetPlayerClass(pSobAvatar);

		pContRwd->GetContRewardFinal(API_GetTableContainer(), byAvatarClass);

		const sREWARD_INFO* pDefRwd = pContRwd->GetDefRewardInfo();
		const sREWARD_INFO* pSelRwd = pContRwd->GetSelRewardInfo();

	    CNtlSLEventGenerator::QuestRewardDialog_Req( sKey,
												     pContRwd->GetRewardContType(),
												     pContRwd->GetLimitTime(),
												     pTCtrl->GetTrigger()->GetTitle(),
												     0xffffffff,
												     0xffffffff,
												     0xffffffff,
												     eGRADE_TYPE_INVALID,
												     pContRwd->GetDesc(),
												     pDefRwd,
												     pSelRwd,
			pContRwd->GetRewardExp(), pContRwd->GetRewardZeny());
    }
}

void CDboTSCTAgency::UT_ShowRewardDialog( sTS_KEY& sKey, int nSelIdx, bool bCancel )
{
	OUT_QMSG_3( "[Trigger] User -> Trigger : Show reward dialog message 처리중 [%d, %d, %d]", sKey.tID, nSelIdx, bCancel );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_ShowRewardDialog( sKey, nSelIdx, bCancel );
}

void CDboTSCTAgency::TU_ShowUserSelectionDialog( sTS_KEY& sKey, CDboTSCTCtrl* pTCtrl, CDboTSContUsrSel* pCont )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Show user selection dialog message 처리중 [%d]", sKey.tID );

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
			CNtlSLEventGenerator::QuestUserSelectDialog_Req( sKey, pCont->GetTargetType(), pCont->GetTargetTblIdx(), pTCtrl->GetTrigger()->GetTitle(), pCont->GetDesc(), nCnt, sBranchInfo );
		}
		break;

	case eUSER_SEL_TYPE_NARRATION:
		{
			CNtlSLEventGenerator::QuestNarrationUserSelectDialog_Req( sKey, pCont->GetUserSelProgState(), pCont->GetDesc(), nCnt, sBranchInfo );
		}
		break;

	default:
		{
			CNtlTSLog::Log( "Trigger user select window type mismatch. Info[%d,%d]. [%s]", sKey.tID, sKey.tcID, TS_CODE_TRACE() );

			UT_ShowUserSelectionDialog( sKey, NTL_TS_TC_ID_INVALID, true );
		}
		break;
	}
}

void CDboTSCTAgency::UT_ShowUserSelectionDialog( sTS_KEY& sKey, NTL_TS_TC_ID tcSelId, bool bCancel )
{
	OUT_QMSG_3( "[Trigger] User -> Trigger : Show user selection dialog message 처리중 [%d, %d, %d]", sKey.tID, tcSelId, bCancel );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_ShowUserSelectionDialog( sKey, tcSelId, bCancel );
}

void CDboTSCTAgency::TU_ShowNarrationDialog( sTS_KEY& sKey, CDboTSContNarration* pCont )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Show narration dialog message 처리중 [%d]", sKey.tID );

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

void CDboTSCTAgency::UT_ShowNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	OUT_QMSG_2( "[Trigger] User -> Trigger : Show narration dialog message 처리중 [%d, %d]", sKey.tID, bCancel );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_ShowNarrationDialog( sKey, bCancel );
}

void CDboTSCTAgency::TU_ShowUnifiedNarrationDialog( sTS_KEY& sKey, CDboTSContUnifiedNarration* pCont )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Show unified narration dialog message 처리중 [%d]", sKey.tID );

	CQuestNarrationTable* pQuestNarrationTbl = API_GetTableContainer()->GetQuestNarrationTable();
	sQUEST_NARRATION_TBLDAT* pData = (sQUEST_NARRATION_TBLDAT*) pQuestNarrationTbl->FindData( pCont->GetNarrationTblIdx() );
	if ( NULL == pData || pData->bType == true )
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

void CDboTSCTAgency::UT_ShowUnifiedNarrationDialog( sTS_KEY& sKey, bool bCancel )
{
	OUT_QMSG_2( "[Trigger] User -> Trigger : Show unified narration dialog message 처리중 [%d, %d]", sKey.tID, bCancel );

	if ( m_pUnifiedQuestNarration &&
		 (m_pUnifiedQuestNarration->GetTSKey().tID == sKey.tID) &&
		 m_pUnifiedQuestNarration->IsUsing() )
	{
		m_pUnifiedQuestNarration->NextState( sKey );
	}
	else
	{
		NTL_DELETE( m_pUnifiedQuestNarration );

		CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

		if ( pTCtrl ) pTCtrl->UT_ShowUnifiedNarrationDialog( sKey, bCancel );
	}
}

void CDboTSCTAgency::TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Show npc conversation message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::QuestNPCDialog_Req( sKey, pAct->GetNPCConvType(), pAct->GetNPCIdx(), pAct->GetNPCConv() );
}

void CDboTSCTAgency::UT_ShowNPCConv( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Trigger] User -> Trigger : Show npc conversation message 처리중 [%d]", sKey.tID );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_ShowNPCConv( sKey );
}

void CDboTSCTAgency::TU_CameraDirection( sTS_KEY& sKey, CDboTSActOPCam* pAct )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Camera message 처리중 [%d]", sKey.tID );

	UT_CameraDirection( sKey );
}

void CDboTSCTAgency::UT_CameraDirection( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Trigger] User -> Trigger : Camera message 처리중 [%d]", sKey.tID );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_CameraDirection( sKey );
}


void CDboTSCTAgency::TU_ShowObjConv( sTS_KEY& sKey, CDboTSActObjConv* pAct )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Show object conversation message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::QuestObjDialog_Req( sKey, pAct->GetObjConvType(), pAct->GetWorldTblIdx(), pAct->GetObjIdx(), pAct->GetObjConv() );
}

void CDboTSCTAgency::UT_ShowObjConv( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Trigger] User -> Trigger : Show object conversation message 처리중 [%d]", sKey.tID );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_ShowObjConv( sKey );
}


void CDboTSCTAgency::TU_Hint( sTS_KEY& sKey, CDboTSActHint* pAct )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Hint message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::RegSideHintIcon( pAct->GetHintType(), pAct->GetTableIndex() );

	UT_Hint( sKey );
}

void CDboTSCTAgency::UT_Hint( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Trigger] User -> Trigger : Hint message 처리중 [%d]", sKey.tID );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_Hint( sKey );
}

void CDboTSCTAgency::TU_Open_Window( sTS_KEY& sKey, CDboTSActOpenWindow* pAct )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Open Window message 처리중 [%d]", sKey.tID );

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

void CDboTSCTAgency::UT_Open_Window( sTS_KEY& sKey )
{
	OUT_QMSG_1( "[Trigger] User -> Trigger : Open Window message 처리중 [%d]", sKey.tID );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_Open_Window( sKey );
}

void CDboTSCTAgency::TU_PC_ConvNfy( sTS_KEY& sKey, CDboTSActPCConv* pAct )
{
	OUT_QMSG_1( "[Trigger] User -> Trigger : PC conversation message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::TSPCDialog_Nfy( pAct->GetConvTblIdx() );
}

void CDboTSCTAgency::TU_TelMudosa( sTS_KEY& sKey, CDboTSActTelMudosa* pAct )
{
	OUT_QMSG_1( "[Trigger] Trigger -> User : Teleport Mudosa message 처리중 [%d]", sKey.tID );

	CNtlSLEventGenerator::TSMudosaTeleport_Req( sKey );
}

void CDboTSCTAgency::UT_TelMudosa( sTS_KEY& sKey, RwUInt8 byMudosaIndex, RwBool bCancel )
{
	OUT_QMSG_1( "[Trigger] User -> Trigger : Teleport Mudosa message 처리중 [%d]", sKey.tID );

	CDboTSCTCtrl* pTCtrl = (CDboTSCTCtrl*)FindProgressTrigger( sKey.tID );

	if ( pTCtrl ) pTCtrl->UT_TelMudosa( sKey, byMudosaIndex, bCancel );
}
