#include "precomp_ntlsimulation.h"
#include "NtlSobAvatar.h"

// shared
#include "PCTable.h"
#include "NtlCharacter.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLCharacter.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"

// simulation
#include "NtlFSMStateBase.h"
#include "NtlSobAttrFactory.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobItem.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "TableContainer.h"
#include "NtlFSMDef.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMCharActLayer.h"
#include "NtlSobCharProxy.h"
#include "NtlSLLogic.h"
#include "DboTSCMain.h"
#include "NtlSLApi.h"
#include "NtlClock.h"
#include "DboTSCQAgency.h"
#include "DboTSCTAgency.h"
#include "DboTSCObjAgency.h"
#include "NtlCameraController.h"

DEFINITION_MEMORY_POOL(CNtlSobAvatar)

CNtlSobAvatar::CNtlSobAvatar()
{
	m_charID = INVALID_SERIAL_ID;

	m_pQAgency = NULL;
	m_pPCTAgency = NULL;
	m_pObjAgency = NULL;
	m_bValidAvatarData	 = false;

	CDboTSCMain *pTCSMain = GetTSCMain();
	if ( pTCSMain && GetTSCMain()->IsCreated() )
	{
		m_pQAgency	= pTCSMain->CreateAgency_Quest();
		m_pPCTAgency = pTCSMain->CreateAgency_Trigger();
		m_pObjAgency = pTCSMain->CreateAgency_Object();
	}
}

CNtlSobAvatar::~CNtlSobAvatar()
{
	CDboTSCMain *pTCSMain = GetTSCMain();
	if ( pTCSMain )
	{
		pTCSMain->DeleteAgency_Quest();
		pTCSMain->DeleteAgency_Trigger();
		pTCSMain->DeleteAgency_Object();
		m_pQAgency = NULL;
		m_pPCTAgency = NULL;
		m_pObjAgency = NULL;
	}
}

RwBool CNtlSobAvatar::CreateContents(void)
{
	if(!m_SkillContainer.Create())
		return FALSE;

	// DB에 저장된 캐릭터 ID
	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	m_charID = pAvatarInfo->sCharPf.charId;

	// Party
	m_Party.Create();

	// Guild
	m_Guild.Create();
	m_GuildWarehouse.Create();

	// OtherParam
	m_OtherParam.Create();

	// PrivateShop
	m_PrivateShop.Create();

	// TMQ
	m_TMQ.Create();

	// CCBD
	m_CCBD.Create();

	// MailSystem
	m_MailSystem.Create();

	// RankBattle
	m_RankBattle.Create();

    // Friend List
    m_FriendList.Create();
    m_BlackList.Create();

	// Tenkaichi Budokai
	m_TBudokai.Create();

	// Hoipoi Mix
	m_HoipoiMix.Create();

	// 도장
	m_Dojo.Create();

	m_bValidAvatarData = true;

	return TRUE;
}

void CNtlSobAvatar::DestroyContents(void)
{
	// skill container destroy
	m_SkillContainer.Destroy();

	// pl slot item destroy
	m_Inventory.Destroy();

	// Warehouse
	m_WareHouse.Destroy();

	// Party
	m_Party.Destroy();

	// Guild
	m_Guild.Destroy();
	m_GuildWarehouse.Destroy();

	// OtherParam
	m_OtherParam.Destroy();

	// PrivateShop
	m_PrivateShop.Destroy();

	// TMQ
	m_TMQ.Destroy();

	// CCBD
	m_CCBD.Destroy();

	// MailSystem
	m_MailSystem.Destroy();

	// RankBattle
	m_RankBattle.Destroy();

    // Friend List
    m_FriendList.Destroy();
    m_BlackList.Destroy();

	m_TBudokai.Destroy();

	m_HoipoiMix.Destroy();

	m_Dojo.Destroy();

	m_bValidAvatarData = false;
}

void CNtlSobAvatar::CreateEvent(void)
{
	LinkMsg(g_EventActionMapHalt, 0);
	LinkMsg(g_EventActionMapJump, 0);
	LinkMsg(g_EventActionMapAirJump, 0);
	LinkMsg(g_EventActionMapSitAndStand, 0);
	LinkMsg(g_EventActionMapHTBUse, 0);
	LinkMsg(g_EventActionMapItemUse, 0);
	LinkMsg(g_EventActionMapSocialUse, 0);
	LinkMsg(g_EventActionMapPrivateShopOpen, 0);
	LinkMsg(g_EventActionMapCharging, 0);
	LinkMsg(g_EventActionMapBlocking, 0);
	LinkMsg(g_EventActionMapAutoFollow, 0);

	LinkMsg(g_EventTerrainClick, 0);
	LinkMsg(g_EventInputChangeHeading, 0);
	LinkMsg(g_EventKeyboardMove, 0);
	LinkMsg(g_EventKeyboardMoveStop, 0);
	LinkMsg(g_EventKeyboardDashMove, 0);
	LinkMsg(g_EventMouseDashMove, 0);
	LinkMsg(g_EventAirDashMove);
	LinkMsg(g_EventAirAccelMove);
	LinkMsg(g_EventSobTargetSelect, 0);
    LinkMsg(g_EventSobTargetSelectRelease, NULL);
	LinkMsg(g_EventSobAttackSelect, 0);

	LinkMsg(g_EventSobEquipItemDurDown, 0);
	LinkMsg(g_EventSobItemDurUpdate, 0);
	LinkMsg(g_EventSobItemUpgrade, 0);
	LinkMsg(g_EventSobItemChangeOptionSet);
	LinkMsg(g_EventSobItemChangeBattleAttribute, 0);
	LinkMsg(g_EventSobEquippedItemUpgradeAll, 0);

	LinkMsg(g_EventSobItemRestrictionUpdate, 0);
	LinkMsg(g_EventItemSocketInsertBead, 0);
	LinkMsg(g_EventItemSocketDestroyBead, 0);

	LinkMsg(g_EventSobWarehouseUpdate, 0);
	LinkMsg(g_EventSobWarehouseItemCreate, 0);
	LinkMsg(g_EventSobWarehouseItemAdd, 0);	
	LinkMsg(g_EventSobWarehouseItemDelete, 0);
	LinkMsg(g_EventSobWarehouseItemMove, 0);
	LinkMsg(g_EventSobWarehouseItemStackMove, 0);
	LinkMsg(g_EventSobFakeBuffAdd, 0);
	LinkMsg(g_EventSobFakeBuffDrop, 0);
	LinkMsg(g_EventSobFakeBuffRefreshAll, 0);
	LinkMsg(g_EventSobFakeBuffActivate);

	LinkMsg(g_EventSLGuild, 0);

	LinkMsg(g_EventGuildWarehouse, 0);
	LinkMsg(g_EventGuildWarehouseItemInfo, 0);
	LinkMsg(g_EventGuildWarehouseItemMove, 0);
	LinkMsg(g_EventGuildWarehouseItemStackMove, 0);
	
	LinkMsg(g_EventHelpHint, 0);
	LinkMsg(g_EventBind, 0);

	LinkMsg(g_EventRegisterQuestPosMark);
	LinkMsg(g_EventUnregisterQuestPosMark);
	LinkMsg(g_EventShowQuestPosMark);

	LinkMsg(g_EventPrivateShopStateVisitor, 0);
	LinkMsg(g_EventPrivateShopItem, 0);
	LinkMsg(g_EventPrivateShopItemDataInfo, 0);
	LinkMsg(g_EventPrivateShopItemSelect, 0);
	LinkMsg(g_EventPrivateShopItemState, 0);
	LinkMsg(g_EventPrivateShopItemBuying, 0);
		
	// quest 수락 및 보상.
	LinkMsg(g_EventQuestAcceptProposal_Nfy, 0);
	LinkMsg(g_EventQuestAcceptReward_Nfy, 0);

	// Social Action Cooling
	LinkMsg(g_EventSobSocialActionCooling, 0);

	// skill Init
	LinkMsg( g_EventSkillInit );	

	// skill passive effect
	LinkMsg( g_EventSetSkillPassiveEffect );

	LinkMsg(g_EventTMQ);
	LinkMsg(g_EventTMQCleintState);

	// cc battle dungeon
	LinkMsg(g_EventBattleDungeonStateUpdate);

	// MailSystem
	LinkMsg(g_EventMailStart, 0);
	LinkMsg(g_EventMailLoadInfo, 0);
	LinkMsg(g_EventMailLoadData, 0);
	LinkMsg(g_EventMailSend, 0);
	LinkMsg(g_EventMailRead, 0);
	LinkMsg(g_EventMailDel, 0);
	LinkMsg(g_EventMailMultiDelRes, 0 );
	LinkMsg(g_EventMailReturn, 0);
	LinkMsg(g_EventMailItemReceive, 0);
	LinkMsg(g_EventMailReload, 0);
	LinkMsg(g_EventMailReloadNfy, 0);
	LinkMsg(g_EventMailLoad, 0);
	LinkMsg(g_EventMailLock, 0);
	LinkMsg(g_EventMailCloseNfy, 0);
	LinkMsg(g_EventCharAway, 0);

	// RPBonus
	LinkMsg(g_EventRpBonusSetupRes, 0);
	
	LinkMsg(g_EventChangeWorldConceptState, 0);

    // TMQ Director
    LinkMsg(g_EventDirectionNfy, NULL);
    LinkMsg(g_EventWorldChange, NULL);

    // 변신
    LinkMsg(g_EventTransform, NULL);

    // 컨디션 관련
    LinkMsg(g_EventAfterEffectRemainTimeNfy, NULL);

	LinkMsg(NPEI_IS_MAPNAME_CHANGE, 0);

	// 채널 변경
	LinkMsg(g_EventGameServerChannelChanged, 0);

	// TS Data reload
	LinkMsg(g_EventReloadTS, 0);

	LinkMsg(g_EventBusMove, 0);

	// 도장
	LinkMsg(g_EventDojo, 0);	
	LinkMsg(g_EventDojoState, 0);
	LinkMsg(g_EventScrambleDefaultInfo, 0);
}

void CNtlSobAvatar::DestroyEvent(void)
{
	UnLinkMsg(g_EventActionMapHalt);
	UnLinkMsg(g_EventActionMapJump);
	UnLinkMsg(g_EventActionMapAirJump);
	UnLinkMsg(g_EventActionMapSitAndStand);
	UnLinkMsg(g_EventActionMapHTBUse);
	UnLinkMsg(g_EventActionMapItemUse);
	UnLinkMsg(g_EventActionMapSocialUse);
	UnLinkMsg(g_EventActionMapPrivateShopOpen);
	UnLinkMsg(g_EventActionMapCharging);
	UnLinkMsg(g_EventActionMapBlocking);
	UnLinkMsg(g_EventActionMapAutoFollow);
	
	UnLinkMsg(g_EventTerrainClick);
	UnLinkMsg(g_EventInputChangeHeading);
	UnLinkMsg(g_EventKeyboardMove);
	UnLinkMsg(g_EventKeyboardMoveStop);
	UnLinkMsg(g_EventKeyboardDashMove);
	UnLinkMsg(g_EventMouseDashMove);
	UnLinkMsg(g_EventAirDashMove);
	UnLinkMsg(g_EventAirAccelMove);
	UnLinkMsg(g_EventSobTargetSelect);
    UnLinkMsg(g_EventSobTargetSelectRelease);
	UnLinkMsg(g_EventSobAttackSelect);

	UnLinkMsg(g_EventSobEquipItemDurDown);
	UnLinkMsg(g_EventSobItemDurUpdate);
	UnLinkMsg(g_EventSobItemUpgrade);
	UnLinkMsg(g_EventSobItemChangeOptionSet);
	UnLinkMsg(g_EventSobItemChangeBattleAttribute);
	UnLinkMsg(g_EventSobEquippedItemUpgradeAll);

	UnLinkMsg(g_EventSobItemRestrictionUpdate);
	UnLinkMsg(g_EventItemSocketInsertBead);
	UnLinkMsg(g_EventItemSocketDestroyBead);
	
	UnLinkMsg(g_EventSobWarehouseUpdate);
	UnLinkMsg(g_EventSobWarehouseItemCreate);
	UnLinkMsg(g_EventSobWarehouseItemAdd);	
	UnLinkMsg(g_EventSobWarehouseItemDelete);
	UnLinkMsg(g_EventSobWarehouseItemMove);
	UnLinkMsg(g_EventSobWarehouseItemStackMove);
	UnLinkMsg(g_EventSobFakeBuffAdd);
	UnLinkMsg(g_EventSobFakeBuffDrop);
	UnLinkMsg(g_EventSobFakeBuffRefreshAll);
	UnLinkMsg(g_EventSobFakeBuffActivate);

	UnLinkMsg(g_EventSLGuild);

	UnLinkMsg(g_EventGuildWarehouse);
	UnLinkMsg(g_EventGuildWarehouseItemInfo);
	UnLinkMsg(g_EventGuildWarehouseItemMove);
	UnLinkMsg(g_EventGuildWarehouseItemStackMove);

	UnLinkMsg(g_EventHelpHint);
	UnLinkMsg(g_EventBind);

	UnLinkMsg(g_EventRegisterQuestPosMark);
	UnLinkMsg(g_EventUnregisterQuestPosMark);
	UnLinkMsg(g_EventShowQuestPosMark);

	UnLinkMsg(g_EventPrivateShopStateVisitor);
	UnLinkMsg(g_EventPrivateShopItem);
	UnLinkMsg(g_EventPrivateShopItemDataInfo);
	UnLinkMsg(g_EventPrivateShopItemSelect);
	UnLinkMsg(g_EventPrivateShopItemState);
	UnLinkMsg(g_EventPrivateShopItemBuying);

	// quest 수락 및 보상.
	UnLinkMsg(g_EventQuestAcceptProposal_Nfy);
	UnLinkMsg(g_EventQuestAcceptReward_Nfy);

	// Social Action Cooling
	UnLinkMsg(g_EventSobSocialActionCooling);

	// Skill Init
	UnLinkMsg( g_EventSkillInit );

	// skill passive effect
	UnLinkMsg( g_EventSetSkillPassiveEffect );

	UnLinkMsg(g_EventTMQ);
	UnLinkMsg(g_EventTMQCleintState);

	// cc battle dungeon
	UnLinkMsg(g_EventBattleDungeonStateUpdate);

	// MailSystem
	UnLinkMsg(g_EventMailStart);
	UnLinkMsg(g_EventMailLoadInfo);
	UnLinkMsg(g_EventMailLoadData);
	UnLinkMsg(g_EventMailSend);
	UnLinkMsg(g_EventMailRead);
	UnLinkMsg(g_EventMailDel);
	UnLinkMsg(g_EventMailMultiDelRes);
	UnLinkMsg(g_EventMailReturn);
	UnLinkMsg(g_EventMailItemReceive);
	UnLinkMsg(g_EventMailReload);
	UnLinkMsg(g_EventMailReloadNfy);
	UnLinkMsg(g_EventMailLoad);
	UnLinkMsg(g_EventMailLock);
	UnLinkMsg(g_EventMailCloseNfy);
	UnLinkMsg(g_EventCharAway);

	UnLinkMsg(g_EventRpBonusSetupRes);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventDirectionNfy);
	UnLinkMsg(g_EventWorldChange);
	UnLinkMsg(g_EventTransform);	
	UnLinkMsg(NPEI_IS_MAPNAME_CHANGE);
	UnLinkMsg(g_EventAfterEffectRemainTimeNfy);
	UnLinkMsg(g_EventGameServerChannelChanged);

	UnLinkMsg(g_EventReloadTS);
	UnLinkMsg(g_EventBusMove);

	UnLinkMsg(g_EventDojo);	
	UnLinkMsg(g_EventDojoState);
	UnLinkMsg(g_EventScrambleDefaultInfo);	
}

RwBool CNtlSobAvatar::Create(void)
{
	NTL_FUNCTION("CNtlSobAvatar::Create");

	if(!CNtlSobPlayer::Create())
	{
		NTL_RETURN(FALSE);
	}

	SetFlags(GetFlags() | SLFLAG_NOT_HITSHOCK | SLFLAG_CAMERASHAKE_ENABLE | SLFLAG_SERVER_SENDER);

	// class name 설정.
	SetClassName(SLCLASS_NAME_AVATAR);

	if(!CreateContents())
	{
		NTL_RETURN(FALSE);
	}

	// event link
	CreateEvent();
	
	NTL_RETURN(TRUE);
}


void CNtlSobAvatar::Destroy(void)
{
	NTL_FUNCTION("CNtlSobAvatar::Destroy");

	// event unlink
	DestroyEvent();

	DestroyContents();
		
	CNtlSobPlayer::Destroy();
	
	NTL_RETURNVOID();
}

void CNtlSobAvatar::ServerChangeOut(void)
{
	DestroyContents();

	CNtlSobPlayer::ServerChangeOut();	
}

void CNtlSobAvatar::ServerChangeIn(void)
{
	CreateContents();

	CNtlSobPlayer::ServerChangeIn();	
}

void CNtlSobAvatar::Update(RwReal fElapsed)
{
	CNtlSobPlayer::Update(fElapsed); 

	API_GetGameClock()->Update( GetTickCount() );

	if ( m_pQAgency )
	{
		m_pQAgency->UT_UpdateAvatarPos( Logic_GetActiveWorldTableId(), m_vPos.x, m_vPos.z );

		m_pQAgency->Update();
	}
	if ( m_pPCTAgency )
	{
		m_pPCTAgency->UT_UpdateAvatarPos( Logic_GetActiveWorldTableId(), m_vPos.x, m_vPos.z );

		m_pPCTAgency->Update();
	}
	if ( m_pObjAgency )
	{
		m_pObjAgency->Update();
	}

	// Item group Cooltime
	m_Inventory.CoolTimeUpdate( fElapsed );

	m_Party.Update(fElapsed);
	m_TMQ.Update(fElapsed);
	m_Dojo.Update(fElapsed);
}

CNtlFSMLayer* CNtlSobAvatar::CreateFSM(void)
{
	CNtlFSMCharActLayer *pFSMLayer = NTL_NEW CNtlFSMCharActLayer;
	pFSMLayer->SetActor(this);
	pFSMLayer->Create(SLCONTROLLER_PLYAER); 
	return pFSMLayer;
}

void CNtlSobAvatar::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobAvatar::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	
		// game attribute data setting.
		GetSobAttr()->HandleEvents(pMsg);

		// item setting
		m_Inventory.HandleEvents(pMsg);

		// quest item setting
		m_QuestInventory.HandleEvents(pMsg);

		// skill setting
		// peessi: The skill must be initialized beforehand because it updates the cooldown information of the previous login status.
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_SKILL_UI_INIT, 0 );		

		m_SkillContainer.HandleEvents(pMsg);

		// buff setting
		m_pBuffContainer->HandleEvents(pMsg);

		// quest setting
		if( m_pQAgency)
			m_pQAgency->HandleEvents(pMsg);

		if( m_pPCTAgency)
			m_pPCTAgency->HandleEvents(pMsg);

		if( m_pObjAgency)
			m_pObjAgency->HandleEvents(pMsg);

		// proxy setting
		GetSobProxy()->HandleEvents(pMsg);
		GetSobProxy()->EnablePicking(FALSE);

		// Coordinate and orientation settings.
		RwV3d vLoc, vDir;
		CNtlMath::MathRwV3dAssign(&vLoc, pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x,
								pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y,
								pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z);
		CNtlMath::MathRwV3dAssign(&vDir, pAvatarInfo->sCharState.sCharStateBase.vCurDir.x,
								pAvatarInfo->sCharState.sCharStateBase.vCurDir.y,
								pAvatarInfo->sCharState.sCharStateBase.vCurDir.z);

		SetPosition(&vLoc);
		SetDirection(&vDir);
		
		// actor
		CNtlSobActor::HandleEvents(pMsg); 

		//--------------------------
		// event 발생.
		// gui data update
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_ATTR | EVENT_AIUT_SKILL | EVENT_AIUT_ITEM | EVENT_AIUT_ZENNY | EVENT_AIUT_QUESTITEM | EVENT_AIUT_BUFF, 0 );
		
		if( Logic_IsFirstClass( this ) && pAvatarInfo->sCharPf.bChangeClass )	// 1차직이면서 전직가능할 떄만 보낸다.
			CNtlSLEventGenerator::ChangeClassAuthorityChanged( pAvatarInfo->sCharPf.bChangeClass );
		
		// server sync avatar type
		SetServerSyncAvatarType(DBO_AVATAR_TYPE_AVATAR);

		// Character Title
		SetCharTitle(pAvatarInfo->sCharPf.bInvisibleTitle, pAvatarInfo->sCharPf.charTitle);
	}
	else if(pMsg.Id == g_EventSobItemAdd || pMsg.Id == g_EventSobItemDelete || pMsg.Id == g_EventSobEquipItemDurDown ||			 
			pMsg.Id == g_EventSobItemMove || pMsg.Id == g_EventSobItemStackMove || pMsg.Id == g_EventSobItemUpdate ||
			pMsg.Id == g_EventSobItemDurUpdate || pMsg.Id == g_EventSobItemUpgrade || pMsg.Id == g_EventSobEquippedItemUpgradeAll || pMsg.Id == g_EventSobItemChangeBattleAttribute)
	{
		CNtlSobPlayer::HandleEvents(pMsg); 
		m_Inventory.HandleEvents(pMsg);		
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_ITEM, 0);
	}
	else if (pMsg.Id == g_EventSobItemChangeOptionSet)
	{
		m_Inventory.HandleEvents(pMsg);
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_ITEM, 0);
	}
	else if (pMsg.Id == g_EventSobItemRestrictionUpdate)
	{
		m_Inventory.HandleEvents(pMsg);

		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_ITEM, 0);
	}
	else if (pMsg.Id == g_EventItemSocketInsertBead || pMsg.Id == g_EventItemSocketDestroyBead)
	{
		m_Inventory.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventSobWarehouseUpdate)
	{
		m_WareHouse.HandleEvents(pMsg);

		SNtlEventSobWarehouseUpdate *pSobWarehouseUpdate = reinterpret_cast<SNtlEventSobWarehouseUpdate*>( pMsg.pData );
		CNtlSLEventGenerator::SobUpdate(pSobWarehouseUpdate->uiSerial, EVENT_AIUT_WAREHOUSE, pSobWarehouseUpdate->uiMessageType);
	}
	else if(pMsg.Id == g_EventSobWarehouseItemCreate || pMsg.Id == g_EventSobWarehouseItemAdd || pMsg.Id == g_EventSobWarehouseItemDelete ||
			pMsg.Id == g_EventSobWarehouseItemMove || pMsg.Id == g_EventSobWarehouseItemStackMove )
	{
		m_WareHouse.HandleEvents(pMsg);
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_ITEM, 0);		
	}
	else if(pMsg.Id == g_EventSobQuestItemAdd || pMsg.Id == g_EventSobQuestItemDelete ||
			pMsg.Id == g_EventSobQuestItemMove || pMsg.Id == g_EventSobQuestItemUpdate)
	{
		m_QuestInventory.HandleEvents(pMsg);
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_QUESTITEM, 0);
	}
	else if(pMsg.Id == g_EventSobSkillAdd)
	{
		m_SkillContainer.HandleEvents(pMsg);
		SNtlEventSobSkillAdd *pSkillAdd = reinterpret_cast<SNtlEventSobSkillAdd*>(pMsg.pData);
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_SKILL_LEARN, pSkillAdd->bySkillSlotIdx);
	}
	else if(pMsg.Id == g_EventSobSkillUpgrade)
	{
		m_SkillContainer.HandleEvents(pMsg);
		SNtlEventSobSkillUpgrade *pSkillUpgrade = reinterpret_cast<SNtlEventSobSkillUpgrade*>(pMsg.pData);
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_SKILL_LEVELUP, pSkillUpgrade->bySkillSlotIdx);
	}
	else if(pMsg.Id == g_EventSobHTBSkillAdd)
	{
		m_SkillContainer.HandleEvents(pMsg);
		SNtlEventSobHTBSkillAdd *pHTBSkillAdd = reinterpret_cast<SNtlEventSobHTBSkillAdd*>(pMsg.pData);
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_HTBSKILL_LEARN, pHTBSkillAdd->byHTBSkillSlotIdx );
	}
	else if(pMsg.Id == g_EventSobSkillAction || pMsg.Id == g_EventSobHTBSkillAction)
	{
		m_SkillContainer.HandleEvents(pMsg);
		CNtlSobPlayer::HandleEvents(pMsg); 	
	}
	else if(pMsg.Id == g_EventSobSocialActionCooling)
	{
		m_SkillContainer.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemUseAction)
	{
		m_Inventory.HandleEvents(pMsg);
		CNtlSobPlayer::HandleEvents(pMsg);
	}		
	else if(pMsg.Id == g_EventQuestAcceptProposal_Nfy || pMsg.Id == g_EventQuestAcceptReward_Nfy)
	{
		GetSobProxy()->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}
	else if(pMsg.Id == g_EventSLGuild)
	{
		m_Guild.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventGuildWarehouse || pMsg.Id == g_EventGuildWarehouseItemInfo ||
			pMsg.Id == g_EventGuildWarehouseItemMove || pMsg.Id == g_EventGuildWarehouseItemStackMove )
	{
		m_GuildWarehouse.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventChangeGuildName)
	{
		m_Guild.HandleEvents(pMsg);
		CNtlSobPlayer::HandleEvents(pMsg); 
	}
	else if (pMsg.Id == g_EventChangeCharName)
	{
		m_Guild.HandleEvents(pMsg);// update name in guild list
		m_FriendList.HandleEvents(pMsg); // update name in friend list
		m_Party.HandleEvents(pMsg);		// update party member name

		CNtlSobPlayer::HandleEvents(pMsg);
	}
	else if (pMsg.Id == g_EventHelpHint || pMsg.Id == g_EventBind)
	{
		m_OtherParam.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventSobFakeBuffAdd || pMsg.Id == g_EventSobFakeBuffDrop ||
			pMsg.Id == g_EventSobFakeBuffRefreshAll || pMsg.Id == g_EventSobFakeBuffActivate)
	{
		m_Party.HandleEvents(pMsg);
	}
	else if (pMsg.Id == g_EventPrivateShopState || 
			pMsg.Id == g_EventPrivateShopStateVisitor ||
			pMsg.Id == g_EventPrivateShopItem ||
			pMsg.Id == g_EventPrivateShopItemDataInfo ||
			pMsg.Id == g_EventPrivateShopItemSelect ||
			pMsg.Id == g_EventPrivateShopItemState ||
			pMsg.Id == g_EventPrivateShopItemBuying )
	{
		m_PrivateShop.HandleEvents(pMsg);

		if(pMsg.Id == g_EventPrivateShopState)
			CNtlSobPlayer::HandleEvents(pMsg); 
	}
	else if( pMsg.Id == g_EventTMQ || pMsg.Id == g_EventTMQCleintState )
	{
		m_TMQ.HandleEvents(pMsg);

		CNtlSobPlayer::HandleEvents(pMsg);
	}
	else if (pMsg.Id == g_EventBattleDungeonStateUpdate)
	{
		m_CCBD.HandleEvents(pMsg);
	}
	else if( pMsg.Id == g_EventRegisterQuestPosMark ||
			 pMsg.Id == g_EventUnregisterQuestPosMark ||
			 pMsg.Id == g_EventShowQuestPosMark )
	{
		m_OtherParam.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventMailStart ||
			pMsg.Id == g_EventMailLoadInfo ||
			pMsg.Id == g_EventMailLoadData ||
			pMsg.Id == g_EventMailSend ||
			pMsg.Id == g_EventMailRead ||
			pMsg.Id == g_EventMailDel ||
			pMsg.Id == g_EventMailMultiDelRes ||
			pMsg.Id == g_EventMailReturn ||
			pMsg.Id == g_EventMailItemReceive ||
			pMsg.Id == g_EventMailReload ||
			pMsg.Id == g_EventMailReloadNfy ||
			pMsg.Id == g_EventMailLoad ||
			pMsg.Id == g_EventMailLock ||
			pMsg.Id == g_EventCharAway || 
			pMsg.Id == g_EventMailCloseNfy)
	{
		m_MailSystem.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventChangeWorldConceptState)
	{
		m_TMQ.HandleEvents(pMsg);
		m_CCBD.HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventRpBonusSetupRes)
	{
		m_SkillContainer.HandleEvents(pMsg);
		SNtlEventRpBonusSetupRes* pData = reinterpret_cast<SNtlEventRpBonusSetupRes*>(pMsg.pData);
		CNtlSLEventGenerator::SobUpdate(GetSerialID(), EVENT_AIUT_SKILL_RPBONUS, pData->bySlotIndex);
	}
    else if(pMsg.Id == g_EventSobTargetSelect)
    {
        SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>( pMsg.pData );
        SetTargetID(pSobTargetSelect->hSerialId);

        CNtlSobPlayer::HandleEvents(pMsg); 
    }
    else if(pMsg.Id == g_EventSobTargetSelectRelease)
    {
		SetTargetID(INVALID_SERIAL_ID);
		CNtlSobPlayer::HandleEvents(pMsg); // // required to remove the yellow effect from yourself as selected target
    }
    else if(pMsg.Id == g_EventSobAttackSelect)
    {
        SNtlEventSobAttackSelect* pSobAttackSelect = (SNtlEventSobAttackSelect*)pMsg.pData;
        SetTargetID(pSobAttackSelect->hSerialId);

        CNtlSobPlayer::HandleEvents(pMsg);
    }    
	else if(pMsg.Id == NPEI_IS_MAPNAME_CHANGE)
	{
		m_Guild.HandleEvents(pMsg);
	}
    else if(pMsg.Id == g_EventTransform)
    {
        if(GetSobProxy())
            GetSobProxy()->HandleEvents(pMsg);

        CNtlSobPlayer::HandleEvents(pMsg); // Idle에서도 처리한다.
    }
    else if(pMsg.Id == g_EventWorldChange || pMsg.Id == g_EventDirectionNfy)            
    {
        // 툴팁을 보이고 있는 경우 해제한다.
        // 클라이언트 레이어의 UI쪽으로 메시지를 쏴서 툴팁 제거
        CNtlSLEventGenerator::ShowToolTip(FALSE, 0, 0, L"");

        // 프리카메라인 경우 해제한다
        CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_FREE);

        if(GetSobProxy())
            GetSobProxy()->HandleEvents(pMsg);
    }
	else if(pMsg.Id == g_EventGameServerChannelChanged)
	{
		DeleteTSAgency();
	}
	else if( pMsg.Id == g_EventSkillInit )
	{
		m_SkillContainer.HandleEvents( pMsg );
		CNtlSLEventGenerator::SobUpdate( GetSerialID(), EVENT_AIUT_SKILL_INIT, 0 );		
	}
	else if( pMsg.Id == g_EventSetSkillPassiveEffect )
	{
		m_SkillContainer.HandleEvents( pMsg );	
	}
	else if ( pMsg.Id == g_EventReloadTS )
	{
		CDboTSCMain* pTCSMain = GetTSCMain();

		pTCSMain->Delete();

		if ( pTCSMain->Create() )
		{
			m_pQAgency	= pTCSMain->CreateAgency_Quest();
			m_pPCTAgency = pTCSMain->CreateAgency_Trigger();
			m_pObjAgency = pTCSMain->CreateAgency_Object();
		}
		else
		{
			m_pQAgency	= NULL;
			m_pPCTAgency = NULL;
			m_pObjAgency = NULL;
		}
	}
	else if( pMsg.Id == g_EventBusMove )
	{
		m_OtherParam.HandleEvents(pMsg);
	}
	else if( pMsg.Id == g_EventDojo ||
			 pMsg.Id == g_EventDojoState ||
			 pMsg.Id == g_EventScrambleDefaultInfo )
	{
		m_Dojo.HandleEvents(pMsg);
	}	
	else
	{
		CNtlSobPlayer::HandleEvents(pMsg); 
	}
		
	NTL_RETURNVOID();
}

RwBool CNtlSobAvatar::IsAvatarReady(void)
{
	if(m_pFSMLayer)
	{
		CNtlFSMStateBase *pStateBase = m_pFSMLayer->GetCurrentState();
		if(!pStateBase)
			return FALSE;

		return pStateBase->IsActorActiveState();
	}
	else 
		return FALSE;
}

void CNtlSobAvatar::DeleteTSAgency(void)
{
	CDboTSCMain *pTCSMain = GetTSCMain();
	if ( pTCSMain )
	{
		pTCSMain->DeleteAgency_Quest();
		pTCSMain->DeleteAgency_Trigger();
		pTCSMain->DeleteAgency_Object();
		m_pQAgency = NULL;
		m_pPCTAgency = NULL;
		m_pObjAgency = NULL;
	}
}

// avooo' position & direction test
#include "NtlPLSceneManager.h"
void CNtlSobAvatar::SetPosition(const RwV3d* pPos)
{
	if( !pPos )
		return;

	// avooo' position
	DBO_ASSERT(-INVALID_FLOAT < pPos->x, "min posX : " << pPos->x);
	DBO_ASSERT(-INVALID_FLOAT < pPos->y, "min posX : " << pPos->y);
	DBO_ASSERT(-INVALID_FLOAT < pPos->z, "min posX : " << pPos->z);

	DBO_ASSERT(INVALID_FLOAT > pPos->x, "max posX : " << pPos->x);
	DBO_ASSERT(INVALID_FLOAT > pPos->y, "max posX : " << pPos->y);
	DBO_ASSERT(INVALID_FLOAT > pPos->z, "max posX : " << pPos->z);

	RwV3d v3Pos = *pPos;
	if (!GetSceneManager()->GetAvailablePos(v3Pos))
	{
		DBO_FAIL("unavailable avatar position. x : " << pPos->x << " y : " << pPos->y << " z : " << pPos->z);
		return;
	}

	CNtlSobFeelingActor::SetPosition(pPos);
}

// avooo's position & direction test
void CNtlSobAvatar::SetDirection(const RwV3d *pDir)
{
	if( !pDir )
		return;

	DBO_ASSERT(-INVALID_FLOAT < pDir->x, "min dirX : " << pDir->x);
	DBO_ASSERT(-INVALID_FLOAT < pDir->y, "min dirX : " << pDir->y);
	DBO_ASSERT(-INVALID_FLOAT < pDir->z, "min dirX : " << pDir->z);

	DBO_ASSERT(INVALID_FLOAT > pDir->x, "max dirX : " << pDir->x);
	DBO_ASSERT(INVALID_FLOAT > pDir->y, "max dirX : " << pDir->y);
	DBO_ASSERT(INVALID_FLOAT > pDir->z, "max dirX : " << pDir->z);

	CNtlSob::SetDirection(pDir);
}