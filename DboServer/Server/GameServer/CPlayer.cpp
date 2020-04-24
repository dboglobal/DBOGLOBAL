#include "stdafx.h"
#include "CPlayer.h"
#include "GameServer.h"
#include "freebattle.h"
#include "privateshop.h"
#include "trade.h"
#include "NtlBitFlagManager.h"
#include "TableContainerManager.h"
#include "StatusTransformTable.h"
#include "ItemMixExpTable.h"
#include "ExpTable.h"
#include "QuestItemTable.h"
#include "FormulaTable.h"
#include "GameMain.h"
#include "NtlResultCode.h"
#include "ItemPet.h"
#include "SummonPet.h"
#include "item.h"
#include "NtlRandom.h"
#include "TimeLeapDungeon.h"
#include "DungeonManager.h"
#include "Guild.h"
#include "NtlPacketGT.h"
#include "NtlPacketGQ.h"
#include "TriggerManager.h"
#include "TriggerObject.h"
#include "RankBattle.h"
#include "NtlNavi.h"
#include "ExpEvent.h"
#include "BudokaiManager.h"
#include "DragonballScramble.h"
#include "BusSystem.h" // #include "NtlPacketGU.h"


bool DeleteItemUponLogin(TBLIDX itemIdx)
{
	if (itemIdx >= 200041 && itemIdx <= 200047)
		return true;

	return false;
}


const DWORD	TIME_AUTO_STORE_PC_DATA = 5 * 60000;


CPlayer::CPlayer()
	:CCharacter(OBJTYPE_PC)
{
	this->Initialize();
}

CPlayer::~CPlayer()
{
	if (!m_bIsDeleted)
		this->Destroy();
}

//--------------------------------------------------------------------------------------//
//		CREATE
//--------------------------------------------------------------------------------------//
bool CPlayer::Create(sPC_TBLDAT* pTbldat, sPC_PROFILE *pProfile, sCHARSTATE *pCharState)
{
	if (CCharacter::Create(pCharState))
	{
		m_pHtbSkillManager = new CHtbSkillManager;
		m_pHtbSkillManager->Create(this);

		LoadData(pProfile, pTbldat);
		return true;
	}

	return false;
}

void CPlayer::Send_GtUserEnterGame(bool bForSyncCommunity)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packetChat(sizeof(sGT_USER_ENTER_GAME));
	sGT_USER_ENTER_GAME * res = (sGT_USER_ENTER_GAME *)packetChat.GetPacketData();
	res->wOpCode = GT_USER_ENTER_GAME;
	wcscpy_s(res->awchName, NTL_MAX_SIZE_CHAR_NAME + 1, GetCharName());
	res->byClass = GetClass();
	res->byLevel = GetLevel();
	res->byRace = GetRace();
	res->charId = GetCharID();
	res->dwReputation = GetReputation();
	res->guildId = GetGuildID();
	res->hHandle = GetID();
	GetCurLoc().CopyTo(res->vCurrentPosition);
	res->worldId = GetWorldID();
	res->accountId = GetAccountID();
	res->mapNameTblidx = GetMapNameTblidx();
	res->bForSyncCommunity = bForSyncCommunity;
	res->wWaguWaguCoin = (WORD)GetWaguMachineCoin();
	res->wEventCoin = (WORD)GetEventMachineCoin();
	res->channelId = app->GetGsChannel();
	res->dwWaguPoints = GetWaguPoints();
	res->gmGroupId = GetGMLevel();
	packetChat.SetPacketLen(sizeof(sGT_USER_ENTER_GAME));
	app->SendTo(app->GetChatServerSession(), &packetChat);
}

void CPlayer::Send_GtUserLeaveGame(eCHARLEAVING_TYPE eCharLeavingType, bool bIsKickOut, BYTE destGameServerChannelID, BYTE destGameServerIndex)
{
	if (m_bGtUserLeaveGameSent == false)
	{
		CGameServer* app = (CGameServer*)g_pApp;
		m_bGtUserLeaveGameSent = true;

		CNtlPacket packet(sizeof(sGT_USER_LEAVE_GAME));
		sGT_USER_LEAVE_GAME * res = (sGT_USER_LEAVE_GAME *)packet.GetPacketData();
		res->wOpCode = GT_USER_LEAVE_GAME;
		res->accountId = GetAccountID();
		res->bIsKickOut = bIsKickOut;
		res->destGameServerChannelID = destGameServerChannelID;
		res->destGameServerIndex = destGameServerIndex;
		res->eCharLeavingType = eCharLeavingType;
		packet.SetPacketLen(sizeof(sGT_USER_LEAVE_GAME));
		app->SendTo(app->GetChatServerSession(), &packet);
	}
}


void CPlayer::Bann(std::string strReason, BYTE byDuration, ACCOUNTID gmAccountID)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (IsInitialized() && m_bSkipSave == false)
	{
		SetSkipSave(true);

		CNtlPacket pQry(sizeof(sGQ_ACCOUNT_BANN));
		sGQ_ACCOUNT_BANN * qRes = (sGQ_ACCOUNT_BANN *)pQry.GetPacketData();
		qRes->wOpCode = GQ_ACCOUNT_BANN;
		qRes->gmAccountID = gmAccountID;
		qRes->targetAccountID = GetAccountID();
		strcpy_s(qRes->szReason, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, strReason.c_str());
		qRes->byDuration = byDuration;
		pQry.SetPacketLen(sizeof(sGQ_ACCOUNT_BANN));
		app->SendTo(app->GetQueryServerSession(), &pQry);

		GetClientSession()->Disconnect(false);
	}
}


//--------------------------------------------------------------------------------------//
//		SET PLAYER PROFILE
//--------------------------------------------------------------------------------------//
void CPlayer::LoadData(sPC_PROFILE* pcdata, sPC_TBLDAT* pTbldat)
{
	SetTblidx(pTbldat->tblidx);

	m_pTbldat = pTbldat;

	char* strModelName;

	if (pcdata->bIsAdult)
		strModelName = pTbldat->szModel_Adult;
	else
		strModelName = pTbldat->szModel_Child;

	m_pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(strModelName);

	if (m_pAniTbldat)
	{

		m_fScale = pTbldat->fScale;
		if (RefreshObjectRadius())
		{
			wcscpy_s(player_data.awchName, NTL_MAX_SIZE_CHAR_NAME + 1, pcdata->awchName);
			player_data.bChangeClass = pcdata->bChangeClass;
			player_data.bindObjectTblidx = pcdata->bindObjectTblidx;
			player_data.bindWorldId = pcdata->bindWorldId;
			player_data.bIsAdult = pcdata->bIsAdult;
			player_data.byBindType = pcdata->byBindType;
			SetLevel(pcdata->byLevel);
			SetExp(pcdata->dwCurExp);
			player_data.dwMaxExpInThisLevel = pcdata->dwMaxExpInThisLevel;
			player_data.dwMudosaPoint = pcdata->dwMudosaPoint;
			player_data.dwReputation = pcdata->dwReputation;
			player_data.dwSpPoint = pcdata->dwSpPoint;
			player_data.dwTutorialHint = pcdata->dwTutorialHint;
			SetZeni(pcdata->dwZenny);
			SetGuildID(pcdata->guildId);
			player_data.sMixData = pcdata->sMixData;
			player_data.sPcShape = pcdata->sPcShape;
			SetCurEP(pcdata->wCurEP);
			SetCurLP(pcdata->curLp);
			SetCurRP(pcdata->wCurRP);
			SetCurAP(pcdata->curAP);

			m_byCurRPBall = pcdata->byCurRPBall;
			player_data.charTitle = pcdata->charTitle;
			player_data.dwWaguWaguPoints = pcdata->dwWaguWaguPoints;
			player_data.mascotTblidx = pcdata->mascotTblidx;
			player_data.bInvisibleCostume = pcdata->bInvisibleCostume; //if true then cant see costume
			player_data.bInvisibleTitle = pcdata->bInvisibleTitle; //if true, then cant see char title
			player_data.bIsInFreePvpZone = pcdata->bIsInFreePvpZone;
			player_data.bIsScrambleJoinFlag = pcdata->bIsScrambleJoinFlag;
			player_data.bIsScrambleRewardedFlag = pcdata->bIsScrambleRewardedFlag;
			player_data.byHasBattleDragonBallBitFlag = 0;

			SetInitialized(true);
		}
	}
	else
	{
		ERR_LOG(LOG_USER, "NULL == pAniTbldat, model name[%s]", strModelName);
	}
}


//--------------------------------------------------------------------------------------//
//		CALLED WHEN LOGOUT
//--------------------------------------------------------------------------------------//
void CPlayer::LeaveGame()
{
	CGameServer* app = (CGameServer*)g_pApp;

	//ERR_LOG(LOG_USER, "Player %d disconnected from the Server", GetCharID());

	if (!IsInitialized())
		return;

	g_pBusSystem->RemovePlayerSync(this);

	if (GetBusID() != INVALID_HOBJECT)
	{
		g_pBusSystem->RemovePlayerFromBus(this, GetBusID(), false);
		SetBusID(INVALID_HOBJECT);
	}

	if (GetDragonballScramble())
		g_pDragonballScramble->SpawnBall(this, true, true);

	if (GetCharStateID() == CHARSTATE_SITTING) //if we logout while having food activate, remove!
	{
		GetBuffManager()->EndSubBuff(ACTIVE_HEAL_OVER_TIME, INVALID_SYSTEM_EFFECT_CODE);
		GetBuffManager()->EndSubBuff(ACTIVE_EP_OVER_TIME, INVALID_SYSTEM_EFFECT_CODE);
	}

	g_pEventMgr->RemoveEvents(this);
	
	if (GetPlayerItemContainer()->IsUsingGuildBank())
	{
		GetPlayerItemContainer()->FreeGuildBank();
	}

	if (GetRankBattleRoomTblidx() != INVALID_TBLIDX)
	{
		if (GetRankBattleRoomId() != INVALID_ROOMID)
		{
			if (GetCurWorld() && GetCurWorld()->GetRuleType() != GAMERULE_RANKBATTLE && HasEventType(EVENT_TELEPORT_PROPOSAL))
			{
				g_pRankbattleManager->CancelTeleport(this);
			}
			else
			{
				g_pRankbattleManager->LeaveBattle(this);

				SetCurLoc(GetBeforeTeleportLoc());
				SetWorldID(GetBeforeTeleportWorldID());
			}
		}
		else
		{
			g_pRankbattleManager->LeaveRoom(this, GetRankBattleRoomTblidx(), true);
		}
	}


	//!! THIS MUST BE CALLED BEFORE CALLING "LEAVEPARTY" !! remove mark from all partys which marked me. If we dont call this before "LeaveParty" then m_setMarkedByEnemyParty has invalid party inside
	g_pPartyManager->CleanMark(&m_setMarkedByEnemyParty, GetID());

	if (GetParty())
		GetParty()->LeaveParty(this);

	event_TeleportProposal(); //unset all teleport proposal data. Also called when time out.

	GetQuests()->LeaveWorld(true);

	if (GetPcIsFreeBattle())
		g_pFreeBattleManager->EndFreeBattle(GetFreeBattleID(), GetFreeBattleTarget());
	else if (GetFreeBattleID() != INVALID_DWORD)
		g_pFreeBattleManager->DeleteFreeBattle(GetFreeBattleID());

	if (GetTLQ())
	{
		CWorld* pWorld = GetTLQ()->GetWorld();
		SetCurLoc(pWorld->GetTbldat()->outWorldLoc);
		SetWorldID(pWorld->GetTbldat()->outWorldTblidx);
		SetMapNameTblidx(GetNaviEngine()->GetTextAllIndex(pWorld->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z));
		g_pDungeonManager->DestroyTimeLeapDungeon(GetCharID(), GetTLQ());
	}
	else if (GetUD())
	{
		CWorld* pWorld = GetUD()->GetWorld();
		SetCurLoc(pWorld->GetTbldat()->outWorldLoc);
		SetWorldID(pWorld->GetTbldat()->outWorldTblidx);
		SetMapNameTblidx(GetNaviEngine()->GetTextAllIndex(pWorld->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z));
		GetUD()->LeaveDungeon(GetID());

		SetUD(NULL);
	}
	else if (GetCCBD())
	{
		CWorld* pWorld = GetCCBD()->GetWorld();
		SetCurLoc(pWorld->GetTbldat()->outWorldLoc);
		SetWorldID(pWorld->GetTbldat()->outWorldTblidx);
		SetMapNameTblidx(GetNaviEngine()->GetTextAllIndex(pWorld->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z));
		GetCCBD()->LeaveDungeon(GetID());

		SetCCBD(NULL);
	}
	else if (GetTMQ())
	{
		CWorld* pWorld = GetTMQ()->GetWorld();
		SetCurLoc(pWorld->GetTbldat()->outWorldLoc);
		SetWorldID(pWorld->GetTbldat()->outWorldTblidx);
		SetMapNameTblidx(GetNaviEngine()->GetTextAllIndex(pWorld->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z));
		GetTMQ()->RemoveMember(GetID());

		SetTMQ(NULL);
	}
	else if (GetCurWorld() && GetCurWorld()->GetRuleType() == GAMERULE_DOJO)
	{
		SetCurLoc(GetCurWorld()->GetTbldat()->outWorldLoc);
		SetWorldID(GetCurWorld()->GetTbldat()->outWorldTblidx);
		SetMapNameTblidx(GetNaviEngine()->GetTextAllIndex(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z));

		if (CItem* item = GetPlayerItemContainer()->GetItemByIdx(19451))
		{
			item->SetCount(0, false, true);
		}
	}
	else
	{
		if (IsFainting() || IsDespawning())
		{
			SetCurLoc(GetBindLoc());
			SetWorldID(GetBindWorldID());
		}
		else if (IsTeleporting())
		{
			SetCurLoc(GetTeleportLoc());
			SetWorldID(GetTeleportWorldID());
		}

		if (app->IsDojoChannel() && GetMatchIndex() != INVALID_BYTE)
		{
			SetBudokaiPcState(MATCH_MEMBER_STATE_GIVEUP);
			g_pBudokaiManager->PlayerDisconnect(GetCharID(), GetID(), GetJoinID(), GetMatchIndex(), GetBudokaiTeamType());
		}
	}

	if (m_pkShop)
	{
		g_pPShopManager->DestroyPrivateShop(this);
		m_pkShop = NULL;
	}

	if (m_pkVisitShop)
	{
		m_pkVisitShop->LeaveShop(this, true);
		m_pkVisitShop = NULL;
	}

	if (m_pkTrade)
	{
		if (m_pkTrade->GetCompany())
			g_pTradeManager->TradeEnd(m_pkTrade->GetCompany());

		g_pTradeManager->DestroyTrade(m_pkTrade);
	}

	if (hShenronID != INVALID_HOBJECT)
		g_pShenronManager->DespawnShenron(this, false);


	//Notify script that player leaves game
	RemAllScript();

	SaveToDB(false);

	GetTargetListManager()->ClearAggro();

	if (CSummonPet* pPet = GetSummonPet())
		pPet->Despawn();

	app->GetGameMain()->GetWorldManager()->LeaveObject(this);

	SetClientSessionID(INVALID_HSESSION);

	CCharacter::LeaveGame();
}


//--------------------------------------------------------------------------------------//
//		DESTROY PLAYER
//--------------------------------------------------------------------------------------//
void CPlayer::Destroy()
{
	for (std::map<SLOTID, CItemPet*>::iterator it = m_mapMascotData.begin(); it != m_mapMascotData.end(); it++)
	{
		CItemPet* pPet = it->second;
		SAFE_DELETE(pPet);
	}
	m_mapMascotData.clear();

	SAFE_DELETE(pQuest);

	for (tQuickTeleportMap::iterator it = m_mapQuickTeleport.begin(); it != m_mapQuickTeleport.end(); it++)
	{
		sQUICK_TELEPORT_INFO* pInfo = it->second;
		SAFE_DELETE(pInfo);
	}
	m_mapQuickTeleport.clear();

	SAFE_DELETE_ARRAY(m_ItemCooldown);

	ClearRecipes();
	ClearMails();

	SetClientSession(NULL);
	SetClientSessionID(INVALID_HSESSION);
	SetCharID(INVALID_CHARACTERID);
}

//--------------------------------------------------------------------------------------//
//		INITIALIZE player
//--------------------------------------------------------------------------------------//
void CPlayer::Initialize()
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_visibleObjects.clear();

	::ZeroMemory(m_achAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	m_pDestServerInfo = NULL;

	m_dwLastSpeedHackReport = 0;

	m_bCanUnstack = true;
	m_bGtUserLeaveGameSent = false;

	m_bHasTutorialFlag = false;

	m_dwScrambleUpdateTickCount = 2000;

	m_playerItemContainer.Init(this);

	m_hTeleportProposalRequestor = INVALID_HOBJECT;
	m_vTeleportProposalLoc.Reset();
	m_vTeleportProposalDir.Reset();
	m_teleportProposalWorldID = INVALID_WORLDID;
	m_byTeleportProposalType = INVALID_BYTE;

	m_dwLpEmergencyTick = 3000;
	m_dwChatServerLocationSyncTick = 6000;
	m_dwFreePvpZoneUpdateTick = 0;

	m_setPortalPoints.clear();

	m_RankBattleRoomTblidx = INVALID_TBLIDX;
	m_RankBattleRegisterObject = INVALID_HOBJECT;
	m_RankBattleRoomId = INVALID_ROOMID;
	m_sRankBattleData.Init();

	m_pTimeLeapDungeon = NULL;
	m_pUltimateDungeon = NULL;
	m_pBattleDungeon = NULL;

	m_bIsLoggedOut = false;

	this->pQuest = new CQuest;
	this->pQuest->Create(this);

	m_bIsQuickTeleportLoaded = false;
	m_mapQuickTeleport.clear();
	this->m_ItemCooldown = new sDBO_ITEM_COOL_TIME[ITEM_COOL_TIME_GROUP_COUNT];
	for (int i = 0; i < ITEM_COOL_TIME_GROUP_COUNT; i++)
	{
		m_ItemCooldown[i].byItemCoolTimeGroupIndex = i;
		m_ItemCooldown[i].dwInitialItemCoolTime = 0;
		m_ItemCooldown[i].dwItemCoolTimeRemaining = 0;
	}

	m_dwLastCashShopRefresh = 0;
	m_dwItemShopCash = 0;
	m_dwEventMachineCoin = 0;

	m_byGameMasterLevel = ADMIN_LEVEL_NONE;
	m_bIsGameMaster = false;
	dwLoggedTime = app->GetCurTickCount();

	uiAccountID = INVALID_ACCOUNTID;
	m_byPrevChannelID = INVALID_SERVERCHANNELID;
	m_bIsTutorial = false;

	m_currentHtbSkill = INVALID_BYTE;
	m_byHtbUseBalls = 0;

	ZeroMemory(player_data.awchName, NTL_MAX_SIZE_CHAR_NAME + 1);
	player_data.bEmergency = false;
	player_data.bindObjectTblidx = INVALID_TBLIDX;
	player_data.bindWorldId = INVALID_WORLDID;
	player_data.bInvisibleCostume = false;
	player_data.bIsAdult = false;
	player_data.bChangeClass = false;
	player_data.bIsInFreePvpZone = false;
	player_data.bIsMailAway = false;
	player_data.bIsScrambleJoinFlag = false;
	player_data.bIsScrambleRewardedFlag = false;
	player_data.byBindType = DBO_BIND_TYPE_INITIAL_LOCATION;
	player_data.byHasBattleDragonBallBitFlag = 0;
	player_data.charId = INVALID_CHARACTERID;
	player_data.bInvisibleTitle = false;
	player_data.charTitle = INVALID_TBLIDX;
	player_data.dwMapInfoIndex = INVALID_TBLIDX;
	player_data.dwMaxExpInThisLevel = 0;
	player_data.dwMudosaPoint = 0;
	player_data.dwReputation = 0;
	player_data.dwSpPoint = 0;
	player_data.dwTutorialHint = INVALID_DWORD;
	player_data.dwWaguWaguPoints = 0;
	player_data.dwZeniBank = 0;
	player_data.mascotTblidx = INVALID_TBLIDX;

	memset(&player_data.sDogi, 0xffffffff, sizeof(player_data.sDogi));

	memset(&player_data.sMark, 0xffffffff, sizeof(player_data.sMark));
	memset(&player_data.sMixData, NULL, sizeof(player_data.sMixData));
	memset(&player_data.sPcShape, NULL, sizeof(player_data.sPcShape));
	memset(&player_data.vBindDir, NULL, sizeof(player_data.vBindDir));
	memset(&player_data.vBindLoc, NULL, sizeof(player_data.vBindLoc));

	player_data.bCombatMode = false;

	m_pkShop = NULL;
	m_pkVisitShop = NULL;

	m_dwLastMailReloadStatistic = 0;
	m_dwLastMailReload = 0;
	m_dwLastMailLoad = 0;
	m_dwMailSentTime = 0;

	memset(&warFogFlag, NULL, sizeof(warFogFlag));

	ZeroMemory(m_TitleIndexFlag, NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG);

	m_guildId = 0;
	ZeroMemory(m_wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1);

	m_bPartyInvitation = false;
	m_uiInvitedByID = INVALID_HOBJECT;
	m_uiPartyInviteID = INVALID_PARTYID;

	InitFreeBattle();

	m_pkTrade = NULL;

	m_bSkipSave = true;

	dwNextSaveTime = app->GetCurTickCount() + TIME_AUTO_STORE_PC_DATA;

	m_hPet = INVALID_HOBJECT;
	hShenronID = INVALID_HOBJECT;

	m_vehicle.Init();
	
	m_bTransformBySkill = false;
	m_pTransformTbldat = NULL;
	m_byTransformGrade = 0;
	m_byTransformSkillIndex = INVALID_BYTE;

	m_Mascot = NULL;
	m_bRemoteBankSkillUsed = false;

	m_pParty = NULL;

	m_hAttackTarget = INVALID_HOBJECT;

	m_dwGuardDuration = 0;
	m_wDashPassiveRequiredEP = 0;

	m_dwLastAirAccelStartCheck = 0;
	m_bCanAccel = false;
	m_hCastItem = INVALID_HOBJECT;

	m_bIsUsingBank = false;
	m_bBankLoaded = false;

	m_bDecreaseRpFlag = false;
	m_fRpDecreaseTickCount = 0.0f;
	m_dwStartRpDecreaseTickCount = NTL_CHAR_RP_REGEN_WAIT_TIME;

	m_joinId = INVALID_JOINID;
	m_byMatchIndex = INVALID_BYTE;
	m_byBudokaiPcState = MATCH_MEMBER_STATE_NONE;
	m_wBudokaiTeamType = INVALID_MATCH_TEAM_TYPE;

	ResetDirectPlay();
	m_bTeleportAnotherServer = false;
	m_sServerTeleportInfo.Init();

	m_bHasItemDice = false;

	m_bUseItemToExcuteTriggerObject = false;
	m_hBusRideOn = INVALID_HOBJECT;

	m_bIsReviving = false;
	m_dwCombatModeTickCount = 0;
	m_bReceiveExpDisabled = false;

	m_dwLastEventRewardLoad = 0;

	m_hFacingHandle = INVALID_HOBJECT;

	m_bIsAfk = false;
	m_dwAfkTime = 0;
	m_dwLastCameraMove = app->GetCurTickCount();
	m_dwCameraMoveCount = 0;
	m_dwCameraMoveDifference = 0;

	m_npcShopData.Init();

	m_dwLastHackCheck = 0;
	m_fLastAppTime = 0.0f;
}

void CPlayer::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	if (!IsInitialized())
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	if (m_bIsLoggedOut) //check if connection to game lost
	{
		if(IsSkillAffecting() || IsKeepingEffect())
			SendCharStateStanding(); //this is required to release all targets in keepingeffect map in case player is using skill which keeps effect

		g_pObjectManager->DestroyCharacter(this);
		return;
	}

	if (GetClientSession() && GetClientSession()->GetUserState() != NTL_USER_STATE_IN_GAME) //check if player is fully connected
		return;

	CCharacter::TickProcess(dwTickDiff, fMultiple);

	// AFK CHECK
	AfkCheck(dwTickDiff);

	// LP EMERGENCY
	LpEmergency(dwTickDiff);

	// Battle Combat Mode
	TickProcessCombatMode(dwTickDiff);

	//SYNC LOCATION
	ChatServerCoordinateSync(dwTickDiff);

	//BATTLE TICK PROCESS
	if (GetPcIsFreeBattle())
	{
		if (g_pFreeBattleManager->IsOutsideFreebattle(this))
			StartFreeBattleEvent();
		else
			EndFreeBattleEvent();
	}
	else
	{
		UpdateFreePvpZone(dwTickDiff);

		UpdateDbScramble(dwTickDiff);
	}

	//ITEM COOLDOWN
	ItemCooltimeProcess(dwTickDiff);

	//QUESTS UPDATE
	GetQuests()->Update(dwTickDiff);


	if (GetAirState() == AIR_STATE_ON)
		CheckAirAccel();

	if (GetCurrentMascot())
		GetCurrentMascot()->TickProcess(dwTickDiff);

	if (GetHtbSkillManager())
		GetHtbSkillManager()->TickProcess(dwTickDiff);

	/*Decrease RP*/
	DecreaseRPTickProcess(dwTickDiff);

	if (app->GetCurTickCount() > dwNextSaveTime)
	{
		SaveToDB();
		dwNextSaveTime = TIME_AUTO_STORE_PC_DATA + app->GetCurTickCount();
	}

	// last client info
	m_dwLastHackCheck += dwTickDiff;
	if (m_dwLastHackCheck > (120 * 1000)) // 120 seconds
	{
		m_dwLastHackCheck = 0;
		ERR_LOG(LOG_USER, "Player %u kicked due to not sending client info", GetCharID());
		GetClientSession()->Disconnect(false);

		return;
	}
	
	// bot check
	if (m_bIsAfk == false) // only check if player is not afk
	{
		if (!IsStanding() && !IsPrivateShop() && !IsSitting() && !IsFainting() && !IsFollowing() && !IsDirectPlay()) // don't dc in one of these states
		{
			if (app->GetCurTickCount() - m_dwLastCameraMove > (60000 * 15) && (m_dwCameraMoveDifference < 10 || m_dwCameraMoveDifference > 20)) // 15 minutes
			{
				m_bIsAfk = true;
				ERR_LOG(LOG_USER, "Player %u kicked due to botting. m_dwCameraMoveCount = %u, m_dwCameraMoveDifference = %u, m_dwLastCameraMove = %u", GetCharID(), m_dwCameraMoveCount, m_dwCameraMoveDifference, m_dwLastCameraMove);
				GetClientSession()->Disconnect(false);

				return;
			}
		}
	}
}


void CPlayer::UpdateFreePvpZone(DWORD dwTickDiff)
{
	if (GetDragonballScramble())
		return;

	m_dwFreePvpZoneUpdateTick += dwTickDiff;

	if (m_dwFreePvpZoneUpdateTick >= 200)
	{
		m_dwFreePvpZoneUpdateTick = 0;
		if (GetCurWorld())
		{
			if (!IsPvpZone())
			{
				//if (GetNaviEngine()->IsBasicAttributeSet(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE))
				if(IsInBattleArena(GetWorldTblidx(), GetCurLoc()))
				{
					UpdatePvpZone(true);
				}
			}
			else if (IsPvpZone())
			{
				//if (GetNaviEngine()->IsBasicAttributeSet(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE) == false)
				if (IsInBattleArena(GetWorldTblidx(), GetCurLoc()) == false)
				{
					UpdatePvpZone(false);
				}
			}
		}
	}
}


void CPlayer::EncodeInsertPacket(CPlayer* pPlayer)
{
	if (IsGameMaster() && GetStateManager()->IsCharCondition(CHARCOND_TRANSPARENT))
		return;

	return CSpawnObject::EncodeInsertPacket(pPlayer);
}

void CPlayer::CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID)
{
	pObjectInfo->objType = OBJTYPE_PC;

	m_pStateManager->CopyTo(&pObjectInfo->pcState);

	wcscpy_s(pObjectInfo->pcBrief.awchName, NTL_MAX_SIZE_CHAR_NAME + 1, GetCharName());
	pObjectInfo->pcBrief.bEmergency = player_data.bEmergency;
	pObjectInfo->pcBrief.bInvisibleCostume = player_data.bInvisibleCostume;
	pObjectInfo->pcBrief.bIsAdult = player_data.bIsAdult;
	pObjectInfo->pcBrief.bIsInFreePvpZone = player_data.bIsInFreePvpZone;
	pObjectInfo->pcBrief.bIsScrambleJoinFlag = player_data.bIsScrambleJoinFlag;

	pObjectInfo->pcBrief.byHasBattleDragonBallBitFlag = player_data.byHasBattleDragonBallBitFlag;
	pObjectInfo->pcBrief.byLevel = GetLevel();
	pObjectInfo->pcBrief.bySizeRate = m_bySizeRate;
	pObjectInfo->pcBrief.charId = GetCharID();

	pObjectInfo->pcBrief.charTitle = player_data.charTitle;
	pObjectInfo->pcBrief.bInvisibleTitle = player_data.bInvisibleTitle;

	if (pObjectInfo->pcBrief.bInvisibleTitle)
		pObjectInfo->pcBrief.charTitle = INVALID_TBLIDX;

	pObjectInfo->pcBrief.curLp = GetCurLP();
	pObjectInfo->pcBrief.fLastFlyAccelSpeed = GetFlyAccelSpeed();
	pObjectInfo->pcBrief.fLastFlyDashSpeed = GetFlyDashSpeed();
	pObjectInfo->pcBrief.fLastFlySpeed = GetFlySpeed();
	pObjectInfo->pcBrief.fLastRunSpeed = GetRunSpeed();
	pObjectInfo->pcBrief.fSkillAnimationSpeedModifier = GetSkillAnimationSpeedModifier();
	pObjectInfo->pcBrief.mascotTblidx = player_data.mascotTblidx;
	pObjectInfo->pcBrief.maxLp = GetMaxLP();
	pObjectInfo->pcBrief.partyId = GetPartyID();

	//memcpy(&pObjectInfo->pcBrief.sDogi, &player_data.sDogi, sizeof(sDBO_DOGI_DATA));
	pObjectInfo->pcBrief.sDogi.byDojoColor = INVALID_BYTE;
	pObjectInfo->pcBrief.sDogi.byGuildColor = INVALID_BYTE;
	pObjectInfo->pcBrief.sDogi.byType = INVALID_BYTE;
	pObjectInfo->pcBrief.sDogi.guildId = 0;

	GetPlayerItemContainer()->CopyItemBrief(pObjectInfo->pcBrief.sItemBrief);

	memcpy(&pObjectInfo->pcBrief.sMark, &player_data.sMark, sizeof(sDBO_GUILD_MARK));
	memcpy(&pObjectInfo->pcBrief.sPcShape, &player_data.sPcShape, sizeof(sPC_SHAPE));
	pObjectInfo->pcBrief.tblidx = GetTblidx();
	pObjectInfo->pcBrief.wAttackSpeedRate = GetCharAtt()->GetAttackSpeedRate();
	pObjectInfo->pcBrief.wCurEP = GetCurEP();
	pObjectInfo->pcBrief.wMaxEP = GetMaxEP();
	wcscpy_s(pObjectInfo->pcBrief.wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, GetGuildName());
}

void CPlayer::CreateDestServerInfo(sDBO_SERVER_INFO * pServerInfo, BYTE* pbyAuthKey)
{
	if (pServerInfo)
	{
		if (m_pDestServerInfo == NULL)
			m_pDestServerInfo = new sSERVER_INFO;

		strcpy_s(m_pDestServerInfo->szCharacterServerIP, NTL_MAX_LENGTH_OF_IP + 1, pServerInfo->achPublicAddress);
		m_pDestServerInfo->wCharacterServerPortForClient = pServerInfo->wPortForClient;
		m_pDestServerInfo->dwLoad = pServerInfo->dwLoad;
		m_pDestServerInfo->serverchannelID = pServerInfo->byServerChannelIndex;
		m_pDestServerInfo->serverfarmID = pServerInfo->serverFarmId;
	}

	memcpy(m_achAuthKey, pbyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
}

void CPlayer::CopyDestServerInfo(sSERVER_INFO * pServerInfo, char* pchAuthKey)
{
	if (pServerInfo)
	{
		strcpy_s(pServerInfo->szCharacterServerIP, NTL_MAX_LENGTH_OF_IP + 1, m_pDestServerInfo->szCharacterServerIP);
		pServerInfo->wCharacterServerPortForClient = m_pDestServerInfo->wCharacterServerPortForClient;
		pServerInfo->dwLoad = m_pDestServerInfo->dwLoad;
		pServerInfo->serverchannelID = m_pDestServerInfo->serverchannelID;
		pServerInfo->serverfarmID = m_pDestServerInfo->serverfarmID;
	}

	memcpy(pchAuthKey, m_achAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	delete m_pDestServerInfo;
	m_pDestServerInfo = NULL;
}


//--------------------------------------------------------------------------------------//
//		STORE PLAYER DATA ON LOGOUT
//--------------------------------------------------------------------------------------//
void CPlayer::SaveToDB(bool bIsTimer)
{
	if (m_bSkipSave)
		return;

	if (!GetClientSession())
	{
		ERR_LOG(LOG_USER, "Character::Save : no session when saving (char id: %d)", GetCharID());
		return;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	DWORD dwPlayTimeInSeconds = (app->GetCurTickCount() - dwLoggedTime) / 1000;

	if (bIsTimer)
	{
		CNtlPacket packet(sizeof(sGQ_RUNTIME_PCDATA_SAVE_NFY));
		sGQ_RUNTIME_PCDATA_SAVE_NFY* res = (sGQ_RUNTIME_PCDATA_SAVE_NFY*)packet.GetPacketData();
		res->wOpCode = GQ_RUNTIME_PCDATA_SAVE_NFY;
		res->charID = GetCharID();
		res->dwExp = GetExp();
		res->worldTblidx = GetWorldTblidx();
		res->worldId = GetWorldID();
		GetCurLoc().CopyTo(res->vLoc);
		GetCurDir().CopyTo(res->vDir);
		res->dwAddPlayTime = dwPlayTimeInSeconds;
		packet.SetPacketLen(sizeof(sGQ_RUNTIME_PCDATA_SAVE_NFY));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
	else
	{
		//if transform by skill
		if (m_bTransformBySkill && m_byTransformSkillIndex != INVALID_BYTE)
		{
			GetSkillManager()->StartSkillCoolDown(m_byTransformSkillIndex);
			EndTransformation();
		}

		m_bSkipSave = true; //dont need to store in db anymore

		CNtlPacket packet(sizeof(sGQ_SAVE_PC_DATA_REQ));
		sGQ_SAVE_PC_DATA_REQ* res = (sGQ_SAVE_PC_DATA_REQ*)packet.GetPacketData();
		res->wOpCode = GQ_SAVE_PC_DATA_REQ;
		res->sPcData.charId = GetCharID();
		res->sPcData.dwEXP = GetExp();
		res->sPcData.worldTblidx = GetWorldTblidx();
		res->sPcData.worldId = GetWorldID();
		GetCurLoc().CopyTo(res->sPcData.fPositionX, res->sPcData.fPositionY, res->sPcData.fPositionZ);
		GetCurDir().CopyTo(res->sPcData.fDirX, res->sPcData.fDirY, res->sPcData.fDirZ);
		res->sPcData.charLp = GetCurLP();
		res->sPcData.wEP = GetCurEP();
		res->sPcData.wRP = GetCurRP();
		res->sPcData.byCurRPBall = GetCurRPBall();
		res->sPcData.charAP = GetCurAP();
		res->sPcData.dwMapInfoIndex = GetMapNameTblidx();
		res->sPcData.eAirState = GetAirState();
		res->sPcData.mascotTblidx = INVALID_TBLIDX; // GetMascotTblidx();
		res->sPcData.charTitle = GetCharTitleID();
		res->sPcData.dwTutorialHint = player_data.dwTutorialHint;
		res->sPcData.bIsScrambleJoinFlag = player_data.bIsScrambleJoinFlag;
		res->sPcData.bIsScrambleRewardedFlag = player_data.bIsScrambleRewardedFlag;
		res->dwAddPlayTime = dwPlayTimeInSeconds;

		if(GetClientSession())
			NTL_SAFE_STRCPY(res->IP, GetClientSession()->GetRemoteIP());
		
		if (m_sServerTeleportInfo.worldId != INVALID_WORLDID && IsLeaving())
		{
		//	printf("m_sServerTeleportInfo.vLoc.x %f m_sServerTeleportInfo.worldId %u\n", m_sServerTeleportInfo.vLoc.x, m_sServerTeleportInfo.worldId);
			res->serverChangeInfo.destWorldId = m_sServerTeleportInfo.worldId;
			res->serverChangeInfo.destWorldTblidx = m_sServerTeleportInfo.worldTblidx;
			res->serverChangeInfo.prevServerChannelId = app->GetGsChannel();
	
			res->sPcData.fPositionX = m_sServerTeleportInfo.vLoc.x;
			res->sPcData.fPositionY = m_sServerTeleportInfo.vLoc.y;
			res->sPcData.fPositionZ = m_sServerTeleportInfo.vLoc.z;
			res->sPcData.fDirX = m_sServerTeleportInfo.vDir.x;
			res->sPcData.fDirY = m_sServerTeleportInfo.vDir.y;
			res->sPcData.fDirZ = m_sServerTeleportInfo.vDir.z;
		}
		else res->serverChangeInfo.Init();

		packet.SetPacketLen(sizeof(sGQ_SAVE_PC_DATA_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		_SaveItemCooldowns();
		GetSkillManager()->_SaveSkillData();
		GetBuffManager()->_SaveBuffCooldowns();
		GetHtbSkillManager()->_SaveHtbCooldowns();

		CNtlPacket packet2(sizeof(sGQ_MASCOT_SAVE_DATA_REQ));
		sGQ_MASCOT_SAVE_DATA_REQ* res2 = (sGQ_MASCOT_SAVE_DATA_REQ*)packet2.GetPacketData();
		res2->wOpCode = GQ_MASCOT_SAVE_DATA_REQ;
		res2->byCount = 0;
		res2->charId = GetCharID();
		
		for (std::map<SLOTID, CItemPet*>::iterator it = m_mapMascotData.begin(); it != m_mapMascotData.end(); it++)
		{
			CItemPet* pMascot = it->second;
			if (pMascot)
			{
				res2->dwExp[res2->byCount] = pMascot->GetMascotData()->dwCurExp;
				res2->dwVp[res2->byCount] = pMascot->GetMascotData()->dwCurVP;
				res2->slotId[res2->byCount++] = it->first;
			}
		}

		if (res2->byCount > 0)
		{
			packet2.SetPacketLen(sizeof(sGQ_MASCOT_SAVE_DATA_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet2);
		}
	}

	//Quests
	GetQuests()->SaveQuestProgress();


	dwLoggedTime = app->GetCurTickCount();

	if (bIsTimer)
		this->ReloadMailsStatistic();

//	ERR_LOG(LOG_SYSTEM, "Player store player data in db time %u bIsTimer %i\n", GetTickCount() - dwStartProcess, bIsTimer);
}


void CPlayer::SendLoadPcDataReq()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_PC_DATA_LOAD_REQ));
	sGQ_PC_DATA_LOAD_REQ * res = (sGQ_PC_DATA_LOAD_REQ *)packet.GetPacketData();
	res->wOpCode = GQ_PC_DATA_LOAD_REQ;
	res->accountId = GetAccountID();
	res->charId = GetCharID();
	app->SendTo(app->GetQueryServerSession(), &packet);
}


void CPlayer::RecvLoadPcDataRes(sPC_DATA * pPcData, sDBO_SERVER_CHANGE_INFO * pserverChangeInfo, bool bTutorialFlag, sCHAR_WAR_FOG_FLAG * pWarFogInfo, sMAIL_NEW_BREIF * pMailBrief, sRANKBATTLE_SCORE_INFO * pRankBattleScore, BYTE * pbyTitleIndexFlag, WORD wWaguCoin)
{
	CGameServer* app = (CGameServer*)g_pApp;

	SetPrevChannelID(pserverChangeInfo->prevServerChannelId);

	/*Did we teleport to another channel?*/
	if(pserverChangeInfo->destWorldId != INVALID_WORLDID)
	{
		if (app->GetGameMain()->GetWorldManager()->FindWorld(pserverChangeInfo->destWorldId))
		{
			pPcData->worldId = pserverChangeInfo->destWorldId;
			pPcData->worldTblidx = pserverChangeInfo->destWorldTblidx;
		}
		else
		{
			ERR_LOG(LOG_USER, "ERROR: Player %u tryed to teleport from channel %u to channel %u dest world id %u dest world tblidx %u", 
				pPcData->charId, pserverChangeInfo->prevServerChannelId, app->GetGsChannel(), pserverChangeInfo->destWorldId, pserverChangeInfo->destWorldTblidx);
		}
	}


	sPC_TBLDAT* pPcTbldar = (sPC_TBLDAT*)g_pTableContainer->GetPcTable()->GetPcTbldat(pPcData->byRace, pPcData->byClass, pPcData->byGender);
	sEXP_TBLDAT* pExpTbldat = (sEXP_TBLDAT*)g_pTableContainer->GetExpTable()->FindData(pPcData->byLevel);

	CNtlPacket packet(sizeof(sGU_AVATAR_CHAR_INFO));
	sGU_AVATAR_CHAR_INFO * res = (sGU_AVATAR_CHAR_INFO *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_CHAR_INFO;
	res->byPlayerMaxLevel = 70;
	res->handle = GetID();
	res->sPcProfile.tblidx = pPcTbldar->tblidx;
	res->sPcProfile.dwMaxExpInThisLevel = pExpTbldat->dwNeed_Exp;
	res->sPcProfile.charId = pPcData->charId;
	NTL_SAFE_WCSCPY(res->sPcProfile.awchName, pPcData->awchName);
	res->sPcProfile.byLevel = pPcData->byLevel;
	res->sPcProfile.dwCurExp = pPcData->dwEXP;
	res->sPcProfile.bChangeClass = pPcData->bChangeClass;

	res->sPcProfile.curLp = pPcData->charLp;
	res->sPcProfile.wCurEP = pPcData->wEP;
	res->sPcProfile.wCurRP = pPcData->wRP;
	res->sPcProfile.byCurRPBall = pPcData->byCurRPBall;
	res->sPcProfile.curAP = pPcData->charAP;

	res->sPcProfile.sPcShape.byFace = pPcData->byFace;
	res->sPcProfile.bIsAdult = pPcData->bIsAdult;
	res->sPcProfile.sPcShape.byHair = pPcData->byHair;
	res->sPcProfile.sPcShape.byHairColor = pPcData->byHairColor;
	res->sPcProfile.sPcShape.bySkinColor = pPcData->bySkinColor;
	res->sCharState.sCharStateBase.vCurLoc.x = pPcData->fPositionX;
	res->sCharState.sCharStateBase.vCurLoc.y = pPcData->fPositionY;
	res->sCharState.sCharStateBase.vCurLoc.z = pPcData->fPositionZ;
	res->sCharState.sCharStateBase.vCurDir.x = pPcData->fDirX;
	res->sCharState.sCharStateBase.vCurDir.y = pPcData->fDirY;
	res->sCharState.sCharStateBase.vCurDir.z = pPcData->fDirZ;
	res->sCharState.sCharStateBase.eAirState = pPcData->eAirState;

	SetWorldID(pPcData->worldId);
	SetMapNameTblidx(pPcData->dwMapInfoIndex);

	res->sPcProfile.dwZenny = pPcData->dwMoney;
	SetZeniBank(0);

	m_bHasTutorialFlag = bTutorialFlag;

	res->sPcProfile.dwTutorialHint = pPcData->dwTutorialHint;
	res->sPcProfile.dwReputation = pPcData->dwReputation;
	res->sPcProfile.dwMudosaPoint = pPcData->dwMudosaPoint;
	res->sPcProfile.dwSpPoint = pPcData->dwSpPoint;

	res->sPcProfile.charTitle = pPcData->charTitle;
	res->sPcProfile.bInvisibleTitle = pPcData->bInvisibleTitle;

	memcpy(&res->sPcProfile.sMixData, &pPcData->sMixData, sizeof(sHOIPOIMIX_DATA));

	res->sPcProfile.bIsGameMaster = m_bIsGameMaster = pPcData->bIsGameMaster;
	m_byGameMasterLevel = pPcData->byAdminLevel;

	if (CGuild* pGuild = g_pGuildManager->GetGuild(pPcData->guildId))
	{
		res->sPcProfile.guildId = pPcData->guildId;
		SetGuildName(pGuild->GetGuildName());

		SetGuildDogiMark(*pGuild->GetGuildMark(), *pGuild->GetGuildDogiData());
	}

	SetIsMailAway(pPcData->bIsMailAway);

	res->sPcProfile.bIsInFreePvpZone = false;


	res->sPcProfile.bIsScrambleJoinFlag = pPcData->bIsScrambleJoinFlag;
	res->sPcProfile.bIsScrambleRewardedFlag = pPcData->bIsScrambleRewardedFlag;
	res->sPcProfile.byRankBattleRemainCount = pPcData->byRankBattleRemainCount;

	res->sPcProfile.byHasBattleDragonBallBitFlag = 0;

	res->sPcProfile.mascotTblidx = pPcData->mascotTblidx;

	res->sPcProfile.dwWaguWaguPoints = pPcData->dwWaguWaguPoints;
	res->sPcProfile.bInvisibleCostume = pPcData->bInvisibleCostume;


	res->sPcProfile.bindObjectTblidx = pPcData->bindObjectTblidx;
	res->sPcProfile.bindWorldId = pPcData->bindWorldId;
	res->sPcProfile.byBindType = pPcData->byBindType;

	player_data.vBindLoc.x = pPcData->vBindLoc.x;
	player_data.vBindLoc.y = pPcData->vBindLoc.y;
	player_data.vBindLoc.z = pPcData->vBindLoc.z;
	player_data.vBindDir.x = pPcData->vBindDir.x;
	player_data.vBindDir.y = pPcData->vBindDir.y;
	player_data.vBindDir.z = pPcData->vBindDir.z;
	

	res->sCharState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;
	res->sCharState.sCharStateBase.dwConditionFlag = 0;
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
	res->sCharState.sCharStateDetail.sCharStateSpawning.byTeleportType = TELEPORT_TYPE_GAME_IN;
	res->sCharState.sCharStateDetail.sCharStateSpawning.bSpawnDirection = true;
	res->sCharState.sCharStateDetail.sCharStateSpawning.bIsFaint = false;
	res->wCharStateSize = (WORD)sizeof(sCHARSTATE_BASE) + (WORD)sizeof(sCHARSTATE_SPAWNING); //this is correct. Always must include size of charstate base

	packet.SetPacketLen(sizeof(sGU_AVATAR_CHAR_INFO));
	app->Send(GetClientSessionID(), &packet); //Send to player

	SetJoinID(pPcData->wJoinID);

	SendWarfogInfo(pWarFogInfo);
	SendTitleInfo(pbyTitleIndexFlag);
	SendRankBattleScoreInfo(pRankBattleScore);
	SendWaguCoinInfo(wWaguCoin);

	CNtlPacket packetMail(sizeof(sGU_CHAR_MAIL_INFO));
	sGU_CHAR_MAIL_INFO * resMail = (sGU_CHAR_MAIL_INFO *)packetMail.GetPacketData();
	resMail->wOpCode = GU_CHAR_MAIL_INFO;
	resMail->byMailCount = pMailBrief->byMailCount;
	resMail->byManagerCount = pMailBrief->byUnReadManager;
	resMail->byNormalCount = pMailBrief->byUnReadNormal;
	packetMail.SetPacketLen(sizeof(sGU_CHAR_MAIL_INFO));
	app->Send(GetClientSessionID(), &packetMail);


	if (Create(pPcTbldar, &res->sPcProfile, &res->sCharState))
	{
		//send to chat server that player logged in
		Send_GtUserEnterGame(false);

		if(GetPrevChannelID() == INVALID_SERVERCHANNELID)
		{
			
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "Fail. Player Create failed");
	}
}

void CPlayer::RecvPcItemLoadRes(sITEM_DATA * pData, BYTE byCount, WORD wCurPacketCount)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGU_AVATAR_ITEM_INFO));
	sGU_AVATAR_ITEM_INFO * res = (sGU_AVATAR_ITEM_INFO *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_ITEM_INFO;
	res->byItemCount = 0;

	for(BYTE i = 0; i < byCount; i++)
	{
		ITEMID itemId = pData[i].itemId;
		BYTE byPlace = pData[i].byPlace;
		BYTE byPos = pData[i].byPosition;

		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(pData[i].itemNo);

		if (pItemTbldat == NULL || DeleteItemUponLogin(pData[i].itemNo))
		{
			ERR_LOG(LOG_USER, "ERROR: Player %u cannot load item id %I64u. Item tblidx %u does not exist", GetCharID(), itemId, pData[i].itemNo);

			CNtlPacket packet2(sizeof(sGQ_ITEM_DELETE_REQ));
			sGQ_ITEM_DELETE_REQ * res2 = (sGQ_ITEM_DELETE_REQ *)packet2.GetPacketData();
			res2->wOpCode = GQ_ITEM_DELETE_REQ;
			res2->charId = GetCharID();
			res2->itemId = itemId;
			res2->handle = GetID();
			res2->byPlace = byPlace;
			res2->byPos = byPos;
			packet2.SetPacketLen(sizeof(sGQ_ITEM_DELETE_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet2);

			continue;
		}

		if (pData[i].byStackcount == 0)
		{
			ERR_LOG(LOG_USER, "ERROR: Player %u cannot load item id %I64u (tblidx:%u). Stack count is 0", GetCharID(), itemId, pData[i].itemNo);

			CNtlPacket packet2(sizeof(sGQ_ITEM_DELETE_REQ));
			sGQ_ITEM_DELETE_REQ * res2 = (sGQ_ITEM_DELETE_REQ *)packet2.GetPacketData();
			res2->wOpCode = GQ_ITEM_DELETE_REQ;
			res2->charId = GetCharID();
			res2->itemId = itemId;
			res2->handle = GetID();
			res2->byPlace = byPlace;
			res2->byPos = byPos;
			packet2.SetPacketLen(sizeof(sGQ_ITEM_DELETE_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet2);

			continue;
		}

		//check if non-bag item is at bagslot
		if (byPlace == CONTAINER_TYPE_BAGSLOT)
		{
			if (pItemTbldat->byItem_Type != ITEM_TYPE_BAG)
			{
				ERR_LOG(LOG_USER, "ERROR: Player %u cannot load item id %I64u. Item is on bagslot but is not a bag", GetCharID(), itemId);

				CNtlPacket packet2(sizeof(sGQ_ITEM_DELETE_REQ));
				sGQ_ITEM_DELETE_REQ * res2 = (sGQ_ITEM_DELETE_REQ *)packet2.GetPacketData();
				res2->wOpCode = GQ_ITEM_DELETE_REQ;
				res2->charId = GetCharID();
				res2->itemId = itemId;
				res2->handle = GetID();
				res2->byPlace = byPlace;
				res2->byPos = byPos;
				packet2.SetPacketLen(sizeof(sGQ_ITEM_DELETE_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet2);

				continue;
			}
		}

		//bug fix check. Check if inventory exist in which the item is inside.
		if (byPlace >= CONTAINER_TYPE_BAG1 && byPlace <= CONTAINER_TYPE_BAG5)
		{
			CItem* pBag = GetPlayerItemContainer()->GetActiveBag(byPlace - 1);
			if (pBag)
			{
				if (pBag->GetPlace() != CONTAINER_TYPE_BAGSLOT || pBag->GetPos() != byPlace - 1)
				{
					ERR_LOG(LOG_USER, "ERROR: Player %u cannot load item id %I64u. Bag (id %I64u) invalid place %u != %u / pos %u != %u", GetCharID(), itemId, pBag->GetItemID(), pBag->GetPlace(), CONTAINER_TYPE_BAGSLOT, pBag->GetPos(), byPlace - 1);

					CNtlPacket packet2(sizeof(sGQ_ITEM_DELETE_REQ));
					sGQ_ITEM_DELETE_REQ * res2 = (sGQ_ITEM_DELETE_REQ *)packet2.GetPacketData();
					res2->wOpCode = GQ_ITEM_DELETE_REQ;
					res2->charId = GetCharID();
					res2->itemId = itemId;
					res2->handle = GetID();
					res2->byPlace = byPlace;
					res2->byPos = byPos;
					packet2.SetPacketLen(sizeof(sGQ_ITEM_DELETE_REQ));
					app->SendTo(app->GetQueryServerSession(), &packet2);

					continue;
				}

				//check if the item position is out of inventory range
				if (pBag->GetBagSize() <= byPos)
				{
					ERR_LOG(LOG_USER, "ERROR: Player %u cannot load item id %I64u. Item is in wrong Position. Bag (id %I64u) max size %u. Item place %u pos %u", GetCharID(), itemId, pBag->GetItemID(), pBag->GetBagSize(), byPlace, byPos);

					CNtlPacket packet2(sizeof(sGQ_ITEM_DELETE_REQ));
					sGQ_ITEM_DELETE_REQ * res2 = (sGQ_ITEM_DELETE_REQ *)packet2.GetPacketData();
					res2->wOpCode = GQ_ITEM_DELETE_REQ;
					res2->charId = GetCharID();
					res2->itemId = itemId;
					res2->handle = GetID();
					res2->byPlace = byPlace;
					res2->byPos = byPos;
					packet2.SetPacketLen(sizeof(sGQ_ITEM_DELETE_REQ));
					app->SendTo(app->GetQueryServerSession(), &packet2);

					continue;
				}
			}
			else
			{
				ERR_LOG(LOG_USER, "ERROR: Player %u cannot load item id %I64u no such bag exist on pos %u. Item place %u, pos %u", GetCharID(), itemId, byPlace - 1, byPlace, byPos);

				CNtlPacket packet2(sizeof(sGQ_ITEM_DELETE_REQ));
				sGQ_ITEM_DELETE_REQ * res2 = (sGQ_ITEM_DELETE_REQ *)packet2.GetPacketData();
				res2->wOpCode = GQ_ITEM_DELETE_REQ;
				res2->charId = GetCharID();
				res2->itemId = itemId;
				res2->handle = GetID();
				res2->byPlace = byPlace;
				res2->byPos = byPos;
				packet2.SetPacketLen(sizeof(sGQ_ITEM_DELETE_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet2);

				continue;
			}
		}

		res->aItemProfile[res->byItemCount].handle = g_pObjectManager->CreateUID();
		res->aItemProfile[res->byItemCount].tblidx = pData[i].itemNo;
		res->aItemProfile[res->byItemCount].byPlace = byPlace;
		res->aItemProfile[res->byItemCount].byPos = byPos;
		res->aItemProfile[res->byItemCount].byStackcount = pData[i].byStackcount;
		res->aItemProfile[res->byItemCount].byRank = pData[i].byRank;
		res->aItemProfile[res->byItemCount].byCurDur = pData[i].byCurrentDurability;
		res->aItemProfile[res->byItemCount].byGrade = pData[i].byGrade;
		res->aItemProfile[res->byItemCount].bNeedToIdentify = pData[i].bNeedToIdentify;
		res->aItemProfile[res->byItemCount].byBattleAttribute = pData[i].byBattleAttribute;
		wcscpy_s(res->aItemProfile[res->byItemCount].awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, pData[i].awchMaker);
		memcpy(&res->aItemProfile[res->byItemCount].sOptionSet, &pData[i].sOptionSet, sizeof(sITEM_OPTION_SET));
		res->aItemProfile[res->byItemCount].nUseStartTime = pData[i].nUseStartTime;
		res->aItemProfile[res->byItemCount].nUseEndTime = pData[i].nUseEndTime;
		res->aItemProfile[res->byItemCount].byRestrictState = pData[i].byRestrictState;
		res->aItemProfile[res->byItemCount].byDurationType = pData[i].byDurationType;


		CItem* pItem = g_pItemManager->CreateFromDB(res->aItemProfile[res->byItemCount].handle, pData[i], this);
		if (pItem == NULL)
		{
			ERR_LOG(LOG_USER, "cannot create item by tblidx %u (id %I64u)", res->aItemProfile[res->byItemCount].tblidx, itemId);
			g_pObjectManager->DeleteUID(res->aItemProfile[res->byItemCount].handle);

			continue;
		}

		res->byItemCount++;
	}


	res->byBeginCount = wCurPacketCount - res->byItemCount;
	packet.SetPacketLen(sizeof(sGU_AVATAR_ITEM_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::RecvPcSkillLoadRes(sSKILL_DATA * pData, BYTE byCount)
{
	if (GetSkillManager() == NULL)
	{
		ERR_LOG(LOG_GENERAL, "GetSkillManager() == NULL");
		return;
	}

	CNtlPacket packet(sizeof(sGU_AVATAR_SKILL_INFO));
	sGU_AVATAR_SKILL_INFO * res = (sGU_AVATAR_SKILL_INFO *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_SKILL_INFO;
	res->bySkillCount = byCount;

	for(BYTE i = 0; i < byCount; i++)
	{
		CSkillPc* pSkill = new CSkillPc;

		sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pData[i].skillId);
		if (pSkillTbldat)
		{
			if (pSkill->Create(pSkillTbldat, this, pData[i].skillIndex, (DWORD)pData[i].nRemainSec, pData[i].nExp, pData[i].byRpBonusType, pData[i].bIsRpBonusAuto))
			{
				GetSkillManager()->AddSkill(pSkill);
				pSkill->CopyToInfo(&res->aSkillInfo[i]);
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "fail : skill create fail. Player[%u] index[%u] skill tblidx[%u]", GetCharID(), pData[i].skillIndex, pData[i].skillId);
				res->bySkillCount--;
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "fail : Can not find Skill tbldat. Player[%u] index[%u] skill tblidx[%u]", GetCharID(), pData[i].skillIndex, pData[i].skillId);
			res->bySkillCount--;
		}
	}
	
	packet.SetPacketLen(sizeof(sGU_AVATAR_SKILL_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::RecvPcBuffLoadRes(sBUFF_DATA * pData, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_AVATAR_BUFF_INFO));
	sGU_AVATAR_BUFF_INFO * res = (sGU_AVATAR_BUFF_INFO *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_BUFF_INFO;
	res->byBuffCount = byCount;

	BYTE ii = 0;

	for(BYTE i = 0; i < byCount; i++)
	{
		res->aBuffInfo[ii].sourceTblidx = pData[i].sourceTblidx;
		res->aBuffInfo[ii].bySourceType = pData[i].bySourceType;
		res->aBuffInfo[ii].dwInitialDuration = pData[i].dwInitialDuration;
		res->aBuffInfo[ii].dwTimeRemaining = pData[i].dwTimeRemaining;
		res->aBuffInfo[ii].buffIndex = pData[i].buffIndex;
		res->aBuffInfo[ii].bActive = true;
		res->aBuffInfo[ii].aBuffParameter[0].buffParameter.fParameter = (float)pData[i].effectValue1;
		res->aBuffInfo[ii].aBuffParameter[1].buffParameter.fParameter = (float)pData[i].effectValue2;
		res->aBuffInfo[ii].aBuffParameter[0].buffParameter.commonConfigTblidx = pData[i].anArgument1[0];
		res->aBuffInfo[ii].aBuffParameter[1].buffParameter.commonConfigTblidx = pData[i].anArgument2[0];
		res->aBuffInfo[ii].aBuffParameter[0].buffParameter.dwRemainTime = pData[i].anArgument1[1];
		res->aBuffInfo[ii].aBuffParameter[1].buffParameter.dwRemainTime = pData[i].anArgument2[1];
		res->aBuffInfo[ii].aBuffParameter[0].buffParameter.dwRemainValue = pData[i].anArgument1[2];
		res->aBuffInfo[ii].aBuffParameter[1].buffParameter.dwRemainValue = pData[i].anArgument2[2];

		if (GetBuffManager()->RegisterLoadBuff(res->aBuffInfo[i], pData[i].byBuffGroup) == false)
		{
			res->byBuffCount--;
		}
		else
		{
			++ii;
		}
	}

//	packet.SetPacketLen(sizeof(WORD) + sizeof(BYTE) + sizeof(sBUFF_INFO) * res->byBuffCount);
	g_pApp->Send(GetClientSessionID(), &packet); //Send to player
}

void CPlayer::RecvPcHtbSkillLoadRes(sHTB_SKILL_DATA * pData, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_AVATAR_HTB_INFO));
	sGU_AVATAR_HTB_INFO * res = (sGU_AVATAR_HTB_INFO *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_HTB_INFO;
	res->byHTBSkillCount = byCount;

	for(BYTE i = 0; i < byCount; i++)
	{
		CHtbSkill* pSkill = new CHtbSkill;

		sHTB_SET_TBLDAT* pSkillTbldat = (sHTB_SET_TBLDAT*)g_pTableContainer->GetHTBSetTable()->FindData(pData[i].skillId);
		if (pSkillTbldat)
		{
			if (pSkill->Create(pSkillTbldat, this, i, pData[i].dwSkillTime))
			{
				GetHtbSkillManager()->AddHtbSkill(pSkill);
				pSkill->CopyToInfo(&res->aHTBSkillnfo[i]);
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "fail : skill create fail. Player[%u] skill tblidx[%u]", GetCharID(), pData[i].skillId);
				res->byHTBSkillCount--;
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "fail : Can not find Skill tbldat. Player[%u] skill tblidx[%u]", GetCharID(), pData[i].skillId);
			res->byHTBSkillCount--;
		}
	}

	packet.SetPacketLen(sizeof(sGU_AVATAR_HTB_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::RecvPcQuestItemLoadRes(sQUEST_INVENTORY_DATA * pData, BYTE byCount)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGU_AVATAR_QUEST_INVENTORY_INFO));
	sGU_AVATAR_QUEST_INVENTORY_INFO * res = (sGU_AVATAR_QUEST_INVENTORY_INFO *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_QUEST_INVENTORY_INFO;

	for(BYTE i = 0; i < byCount; i++)
	{
		if (g_pTableContainer->GetQuestItemTable()->FindData(pData[i].tblidx) == NULL)
		{
			ERR_LOG(LOG_USER, "ERROR: User %u has quest item %u which does not exist.. Delete..", GetCharID(), pData[i].tblidx);

			CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_DELETE_REQ));
			sGQ_QUEST_ITEM_DELETE_REQ * rQry = (sGQ_QUEST_ITEM_DELETE_REQ *)pQry.GetPacketData();
			rQry->wOpCode = GQ_QUEST_ITEM_DELETE_REQ;
			rQry->charId = GetCharID();
			rQry->handle = GetID();
			rQry->aItems.byUpdateType = eQUEST_ITEM_UPDATE_TYPE_DELETE;
			rQry->aItems.byPos = pData[i].byPos;
			pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_DELETE_REQ));
			app->SendTo(app->GetQueryServerSession(), &pQry);

			continue;
		}

		res->aInventoryInfo[res->byItemCount].tblidx = pData[i].tblidx;
		res->aInventoryInfo[res->byItemCount].byCurStackCount = pData[i].byCurStackCount;
		res->aInventoryInfo[res->byItemCount].byPos = pData[i].byPos;

		GetQuests()->AddQuestItemData(&res->aInventoryInfo[res->byItemCount]);

		res->byItemCount++;
	}

	packet.SetPacketLen(sizeof(sGU_AVATAR_QUEST_INVENTORY_INFO));
	g_pApp->Send(GetClientSessionID(), &packet); //Send to player
}

void CPlayer::RecvPcQuestCompleteLoadRes(const sQUEST_COMPLETE_DATA & rData)
{
	CNtlPacket packet2(sizeof(sGU_AVATAR_QUEST_COMPLETE_INFO));
	sGU_AVATAR_QUEST_COMPLETE_INFO * res2 = (sGU_AVATAR_QUEST_COMPLETE_INFO *)packet2.GetPacketData();
	res2->wOpCode = GU_AVATAR_QUEST_COMPLETE_INFO;
	memcpy(&res2->completeInfo, &rData, sizeof(rData));
	g_pApp->Send(GetClientSessionID(), &packet2);

	GetQuests()->SetQuestCompleteInfo(rData);
}

void CPlayer::RecvPcQuestProgressLoadRes(sQUEST_PROGRESS_DATA * pData, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_AVATAR_QUEST_PROGRESS_INFO));
	sGU_AVATAR_QUEST_PROGRESS_INFO * res = (sGU_AVATAR_QUEST_PROGRESS_INFO *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_QUEST_PROGRESS_INFO;
	res->byProgressCount = byCount;

	for(BYTE i = 0; i < byCount; i++)
	{
		memcpy(&res->progressInfo[i], &pData[i], sizeof(sQUEST_PROGRESS_DATA));

		GetQuests()->StartQuest(&res->progressInfo[i]);
	}

	packet.SetPacketLen(sizeof(sGU_AVATAR_QUEST_PROGRESS_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::RecvPcQuickSlotLoadRes(sQUICK_SLOT_DATA * pData, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_QUICK_SLOT_INFO));
	sGU_QUICK_SLOT_INFO * res = (sGU_QUICK_SLOT_INFO *)packet.GetPacketData();
	res->wOpCode = GU_QUICK_SLOT_INFO;
	res->byQuickSlotCount = byCount;

	for(BYTE i = 0; i < byCount; i++)
	{
		CItem* item = GetPlayerItemContainer()->GetItem(pData[i].itemID);
		if (item)
			res->asQuickSlotData[i].hItem = item->GetID();
		else
			res->asQuickSlotData[i].hItem = INVALID_HOBJECT;

		res->asQuickSlotData[i].bySlot = pData[i].bySlot;
		res->asQuickSlotData[i].byType = pData[i].byType;
		res->asQuickSlotData[i].tblidx = pData[i].tblidx;
	}

	packet.SetPacketLen(sizeof(sGU_QUICK_SLOT_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::RecvPcShortcutLoadRes(sSHORTCUT_DATA * pData, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_CHAR_KEY_INFO));
	sGU_CHAR_KEY_INFO * res = (sGU_CHAR_KEY_INFO *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_KEY_INFO;
	res->byCount = byCount;

	for(BYTE i = 0; i < byCount; i++)
	{
		res->asData[i].wActionID = pData[i].wActionID;
		res->asData[i].wKey = pData[i].wKey;
	}

	packet.SetPacketLen(sizeof(sGU_CHAR_KEY_INFO));
	g_pApp->Send(GetClientSessionID(), &packet); //Send to player
}

void CPlayer::RecvPcItemRecipeLoadRes(sRECIPE_DATA * pData, WORD wCount)
{
	CNtlPacket packet(sizeof(sGU_HOIPOIMIX_ITEM_RECIPE_INFO));
	sGU_HOIPOIMIX_ITEM_RECIPE_INFO * res = (sGU_HOIPOIMIX_ITEM_RECIPE_INFO *)packet.GetPacketData();
	res->wOpCode = GU_HOIPOIMIX_ITEM_RECIPE_INFO;
	res->wCount = wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		res->asRecipeData[i].byRecipeType = pData[i].byRecipeType;
		res->asRecipeData[i].recipeTblidx = pData[i].recipeTblidx;

		AddHoiPoiRecipe(res->asRecipeData[i].recipeTblidx, res->asRecipeData[i].byRecipeType);
	}

	packet.SetPacketLen(sizeof(sGU_HOIPOIMIX_ITEM_RECIPE_INFO));
	g_pApp->Send(GetClientSessionID(), &packet); //Send to player
}

void CPlayer::RecvPcItemCoolTimeLoadRes(sDBO_ITEM_COOL_TIME * pData, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_ITEM_COOL_TIME_INFO));
	sGU_ITEM_COOL_TIME_INFO * res = (sGU_ITEM_COOL_TIME_INFO *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_COOL_TIME_INFO;
	res->byCount = byCount;
	
	for(BYTE i = 0; i < byCount; i++)
	{
		res->aItemCoolTime[i].byItemCoolTimeGroupIndex = pData[i].byItemCoolTimeGroupIndex;
		res->aItemCoolTime[i].dwInitialItemCoolTime = pData[i].dwInitialItemCoolTime;
		res->aItemCoolTime[i].dwItemCoolTimeRemaining = pData[i].dwItemCoolTimeRemaining;

		SetItemCooldown(res->aItemCoolTime[i]);
	}

	packet.SetPacketLen(sizeof(sGU_ITEM_COOL_TIME_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);


	// -- SEND AVATAR INFO END

	SendAvatarInfoEnd();
}

void CPlayer::RecvPcMascotLoadRes(sMASCOT_DATA_EX * pData, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_MASCOT_INFO_EX));
	sGU_MASCOT_INFO_EX* res = (sGU_MASCOT_INFO_EX*)packet.GetPacketData();
	res->wOpCode = GU_MASCOT_INFO_EX;
	res->byCount = byCount;

	for(BYTE i = 0; i < byCount; i++)
	{
		sMASCOT_STATUS_TBLDAT * mascotTbldat = (sMASCOT_STATUS_TBLDAT*)g_pTableContainer->GetMascotStatusTable()->FindData(pData[i].tblidx);
		if (mascotTbldat)
		{
			memcpy(&res->asMascotData[i], &pData[i], sizeof(sMASCOT_DATA_EX));

			res->asMascotData[i].byItemRank = mascotTbldat->byRank;

			CItemPet* mascot = new CItemPet;
			mascot->SetPetData(mascotTbldat, this, &res->asMascotData[i]);
			InsertMascot(mascot);
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "Could not find mascot tblidx %u", pData[i].tblidx);
			res->byCount--;
		}
	}

	packet.SetPacketLen(sizeof(sGU_MASCOT_INFO_EX));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::RecvPcPortalLoadRes(PORTALID * aPortalID, BYTE byCount)
{
	CNtlPacket packet(sizeof(sGU_PORTAL_INFO));
	sGU_PORTAL_INFO * res = (sGU_PORTAL_INFO *)packet.GetPacketData();
	res->wOpCode = GU_PORTAL_INFO;
	res->byCount = byCount;
	
	for(BYTE i = 0; i < byCount; i++)
	{
		res->aPortalID[i] = aPortalID[i];

		AddPortalPoint(res->aPortalID[i]);
	}

	packet.SetPacketLen(sizeof(sGU_PORTAL_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::SendWarfogInfo(sCHAR_WAR_FOG_FLAG * pWarFogInfo)
{
	CNtlPacket packet(sizeof(sGU_WAR_FOG_INFO));
	sGU_WAR_FOG_INFO * res = (sGU_WAR_FOG_INFO *)packet.GetPacketData();
	res->wOpCode = GU_WAR_FOG_INFO;
	memcpy(res->abyWarFogInfo, pWarFogInfo->achWarFogFlag, sizeof(res->abyWarFogInfo));
	packet.SetPacketLen(sizeof(sGU_WAR_FOG_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);

	SetWarFogFlag(res->abyWarFogInfo);
}

void CPlayer::SendTitleInfo(BYTE * pbyTitleIndexFlag)
{
	CNtlPacket packet(sizeof(sGU_CHARTITLE_LIST_INFO));
	sGU_CHARTITLE_LIST_INFO * res = (sGU_CHARTITLE_LIST_INFO *)packet.GetPacketData();
	res->wOpCode = GU_CHARTITLE_LIST_INFO;
	memcpy(res->TitleIndexFlag, pbyTitleIndexFlag, sizeof(res->TitleIndexFlag));
	packet.SetPacketLen(sizeof(sGU_CHARTITLE_LIST_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);

	SetAllCharTitle(res->TitleIndexFlag);
}

void CPlayer::SendRankBattleScoreInfo(const sRANKBATTLE_SCORE_INFO * pRankBattleScore)
{
	CNtlPacket packet(sizeof(sGU_CHAR_RANKBATTLE_SCORE));
	sGU_CHAR_RANKBATTLE_SCORE * res = (sGU_CHAR_RANKBATTLE_SCORE *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_RANKBATTLE_SCORE;
	res->sScoreInfo.dwWin = pRankBattleScore->dwWin;
	res->sScoreInfo.dwDraw = pRankBattleScore->dwDraw;
	res->sScoreInfo.dwLose = pRankBattleScore->dwLose;
	res->sScoreInfo.wStraightKOWin = pRankBattleScore->wStraightKOWin;
	res->sScoreInfo.wMaxStraightKOWin = pRankBattleScore->wMaxStraightKOWin;
	res->sScoreInfo.wMaxStraightWin = pRankBattleScore->wMaxStraightWin;
	res->sScoreInfo.wStraightWin = pRankBattleScore->wStraightWin;
	res->sScoreInfo.fPoint = pRankBattleScore->fPoint;
	packet.SetPacketLen(sizeof(sGU_CHAR_RANKBATTLE_SCORE));
	g_pApp->Send(GetClientSessionID(), &packet); //Send to player

	memcpy(&m_sScoreInfo, pRankBattleScore, sizeof(sRANKBATTLE_SCORE_INFO));
}

void CPlayer::SendWaguCoinInfo(WORD wWaguCoin)
{
	CNtlPacket packet(sizeof(sGU_WAGUWAGUMACHINE_COIN_INFO));
	sGU_WAGUWAGUMACHINE_COIN_INFO * res = (sGU_WAGUWAGUMACHINE_COIN_INFO *)packet.GetPacketData();
	res->wOpCode = GU_WAGUWAGUMACHINE_COIN_INFO;
	res->wWaguWaguCoin = wWaguCoin;
	packet.SetPacketLen(sizeof(sGU_WAGUWAGUMACHINE_COIN_INFO));
	g_pApp->Send(GetClientSessionID(), &packet);

	SetWaguMachineCoin(wWaguCoin);
}

void CPlayer::SendAvatarInfoEnd()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (m_bHasTutorialFlag) //tutorial has not already been finished yet
	{
		if (app->IsDojoChannel() == false)
		{
			if (IsTutorial()) //check if client request tutorial
			{
				TBLIDX tmqIdx = 1;
				if (GetRace() == RACE_NAMEK)
					tmqIdx = 4;
				else if (GetRace() == RACE_NAMEK)
					tmqIdx = 6;

				g_pDungeonManager->CreateTimeQuest(this, tmqIdx, TIMEQUEST_DIFFICULTY_EASY, TIMEQUEST_MODE_INDIVIDUAL);
			}
			else
			{
				SendQueryTutorialUpdate(true);
			}
		}

		m_bHasTutorialFlag = false;
	}

	CNtlPacket packetEnd(sizeof(sGU_AVATAR_INFO_END));
	sGU_AVATAR_INFO_END * resEnd = (sGU_AVATAR_INFO_END *)packetEnd.GetPacketData();
	resEnd->wOpCode = GU_AVATAR_INFO_END;
	packetEnd.SetPacketLen(sizeof(sGU_AVATAR_INFO_END));
	g_pApp->Send(GetClientSessionID(), &packetEnd);
}

void CPlayer::SendQueryTutorialUpdate(bool bFlag)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_TUTORIAL_DATA_UPDATE_REQ));
	sGQ_TUTORIAL_DATA_UPDATE_REQ * res = (sGQ_TUTORIAL_DATA_UPDATE_REQ *)packet.GetPacketData();
	res->wOpCode = GQ_TUTORIAL_DATA_UPDATE_REQ;
	res->charId = GetCharID();
	res->handle = GetID();
	res->bTutorialFlag = bFlag;
	packet.SetPacketLen(sizeof(sGQ_TUTORIAL_DATA_UPDATE_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);
}


void CPlayer::_SaveItemCooldowns()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_SAVE_ITEM_COOL_TIME_DATA_REQ));
	sGQ_SAVE_ITEM_COOL_TIME_DATA_REQ* res = (sGQ_SAVE_ITEM_COOL_TIME_DATA_REQ*)packet.GetPacketData();
	res->wOpCode = GQ_SAVE_ITEM_COOL_TIME_DATA_REQ;
	res->hSubject = GetID();
	res->charId = GetCharID();

	memcpy(res->aItemCoolTimeData, m_ItemCooldown, sizeof(res->aItemCoolTimeData));

	packet.SetPacketLen(sizeof(sGQ_SAVE_ITEM_COOL_TIME_DATA_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);
}


//--------------------------------------------------------------------------------------//
//		ON ENTER WORLD
//--------------------------------------------------------------------------------------//
void CPlayer::OnEnterWorld(CWorld* pWorld)
{
	CSpawnObject::OnEnterWorld(pWorld);
}
//--------------------------------------------------------------------------------------//
//		ON LEAVE WORLD
//--------------------------------------------------------------------------------------//
void CPlayer::OnLeaveWorld(CWorld* pWorld)
{
	CSpawnObject::OnLeaveWorld(pWorld);
}
//--------------------------------------------------------------------------------------//
//		ENTER WORLD COMPLETE
//--------------------------------------------------------------------------------------//
void CPlayer::OnEnterWorldComplete()
{
	CSpawnObject::OnEnterWorldComplete();
}

//--------------------------------------------------------------------------------------//
//		SET PARTY
//--------------------------------------------------------------------------------------//
void CPlayer::SetParty(CParty* pkParty)
{
	if (pkParty == m_pParty)
		return;

	if (pkParty && m_pParty)
		ERR_LOG(LOG_USER, "%u is trying to reassigning party (current %p, new party %p)", this->GetID(), m_pParty, pkParty);


	m_pParty = pkParty;
}

//--------------------------------------------------------------------------------------//
//		CAN JOIN PARTY
//--------------------------------------------------------------------------------------//
WORD CPlayer::CanJoinParty()
{
	WORD result = GAME_SUCCESS;

	if (GetIsPartyInvite()) //check if already has invitation
		result = GAME_PARTY_TARGET_ALREADY_HAS_INVITATION;

	if (GetParty()) //check if a party already set
		result = GAME_PARTY_TARGET_ALREADY_IN_PARTY;

	if (!IsInitialized())
		result = GAME_CHAR_IS_WRONG_STATE;

	if (GetTMQ() != NULL || GetTLQ() != NULL || GetCCBD() != NULL)
		result = GAME_PARTYMATCHING_ANY_MEMBER_IN_DYNAMIC_WORLD;

//	ERR_LOG(LOG_USER, "%d CanJoinParty check (result %d)", GetCharID(), result);

	return result;
}


void CPlayer::LeaveParty()
{
	GetParty()->LeaveParty(this);

	CNtlPacket packet(sizeof(sGU_PARTY_MEMBER_LEFT_NFY));
	sGU_PARTY_MEMBER_LEFT_NFY * res = (sGU_PARTY_MEMBER_LEFT_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MEMBER_LEFT_NFY;
	res->hMember = GetID();
	packet.SetPacketLen(sizeof(sGU_PARTY_MEMBER_LEFT_NFY));
	SendPacket(&packet);
}


//--------------------------------------------------------------------------------------//
//		SET GUILD ID
//--------------------------------------------------------------------------------------//
void CPlayer::SetGuildID(GUILDID id)
{
	if (id == 0 && GetGuildID() != 0) //leave guild
	{
		g_pGuildManager->UpdateGuildMember(GetID(), GetGuildID(), false);
	}
	else if (id != 0 && GetGuildID() == 0) // join guild
	{
		g_pGuildManager->UpdateGuildMember(GetID(), id, true);
	}
	else if(id != 0 && GetGuildID() != 0 && id != GetGuildID()) //already inside guild but join another guild
	{
		g_pGuildManager->UpdateGuildMember(GetID(), GetGuildID(), false); //leave old
		g_pGuildManager->UpdateGuildMember(GetID(), id, true); //join new
	}

	m_guildId = id;
}


//--------------------------------------------------------------------------------------//
//		SET GUILD
//--------------------------------------------------------------------------------------//
void CPlayer::SetGuildDogiMark(sDBO_GUILD_MARK& sMark, sDBO_DOGI_DATA& sDogi)
{
	memcpy(&player_data.sMark, &sMark, sizeof(player_data.sMark));
	memcpy(&player_data.sDogi, &sDogi, sizeof(player_data.sDogi));
}

//--------------------------------------------------------------------------------------//
//		LEAVE GUILD
//--------------------------------------------------------------------------------------//
void CPlayer::LeaveGuild()
{
	g_pGuildManager->UpdateGuildMember(GetID(), GetGuildID(), false);

	ZeroMemory(m_wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1);
	memset(&player_data.sMark, 0xffffffff, sizeof(player_data.sMark));
	memset(&player_data.sDogi, 0xffffffff, sizeof(player_data.sDogi));

	SetGuildID(0);

	//update guild name
	CNtlPacket packet(sizeof(sGU_GUILD_NAME_CHANGED_NFY));
	sGU_GUILD_NAME_CHANGED_NFY * res = (sGU_GUILD_NAME_CHANGED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_GUILD_NAME_CHANGED_NFY;
	res->hSubject = GetID();
	packet.SetPacketLen(sizeof(sGU_GUILD_NAME_CHANGED_NFY));
	Broadcast(&packet, this);

	//update guild dogi
	CNtlPacket packet2(sizeof(sGU_DOGI_UPDATE_NFY));
	sGU_DOGI_UPDATE_NFY * res2 = (sGU_DOGI_UPDATE_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_DOGI_UPDATE_NFY;
	res2->handle = GetID();
	res2->sData = player_data.sDogi;
	packet2.SetPacketLen(sizeof(sGU_DOGI_UPDATE_NFY));
	Broadcast(&packet2);

	//dojo mark changed
	CNtlPacket packet3(sizeof(sGU_GUILD_MARK_CHANGED_NFY));
	sGU_GUILD_MARK_CHANGED_NFY * res3 = (sGU_GUILD_MARK_CHANGED_NFY *)packet3.GetPacketData();
	res3->wOpCode = GU_GUILD_MARK_CHANGED_NFY;
	res3->hSubject = GetID();
	res3->sMark = player_data.sMark;
	packet3.SetPacketLen(sizeof(sGU_GUILD_MARK_CHANGED_NFY));
	Broadcast(&packet3);
}


//--------------------------------------------------------------------------------------//
//		CHECK IF CHAR TITLE ALREADY SET
//--------------------------------------------------------------------------------------//
bool CPlayer::CheckCharTitle(TBLIDX title)
{
	unsigned char c = m_TitleIndexFlag[title / NTL_MAX_CHAR_TITLE_FLAG_COUNT];
	char cShift = (title % NTL_MAX_CHAR_TITLE_FLAG_COUNT) * 1;

	if (c & (0x01 << cShift))
	{
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------//
//		ADD CHARACTER TITLE
//--------------------------------------------------------------------------------------//
void CPlayer::AddCharTitle(TBLIDX title)
{
	CGameServer* app = (CGameServer*)g_pApp;

	unsigned char &c = m_TitleIndexFlag[title / 8];
	char cShift = (title % 8) * 1;
	c |= 0x01 << cShift;

	CNtlPacket packet(sizeof(sGU_CHARTITLE_ADD));
	sGU_CHARTITLE_ADD * res = (sGU_CHARTITLE_ADD*)packet.GetPacketData();
	res->wOpCode = GU_CHARTITLE_ADD;
	res->tblIndex = title + 1;
	packet.SetPacketLen(sizeof(sGU_CHARTITLE_ADD));
	app->Send(GetClientSessionID(), &packet);

	CNtlPacket packetQry(sizeof(sGQ_CHARTITLE_ADD_REQ));
	sGQ_CHARTITLE_ADD_REQ * resQry = (sGQ_CHARTITLE_ADD_REQ*)packetQry.GetPacketData();
	resQry->wOpCode = GQ_CHARTITLE_ADD_REQ;
	resQry->charId = GetCharID();
	resQry->charTitle = title + 1;
	memcpy(resQry->TitleIndexFlag, m_TitleIndexFlag, sizeof(m_TitleIndexFlag));
	packetQry.SetPacketLen(sizeof(sGQ_CHARTITLE_ADD_REQ));
	app->SendTo(app->GetQueryServerSession(), &packetQry);
}

void CPlayer::DelCharTitle(TBLIDX title)
{
	CGameServer* app = (CGameServer*)g_pApp;

	unsigned char &c = m_TitleIndexFlag[title / 8];
	char cShift = (title % 8) * 1;
	c &= ~(0x01 << cShift);

	CNtlPacket packet(sizeof(sGU_CHARTITLE_DELETE));
	sGU_CHARTITLE_DELETE * res = (sGU_CHARTITLE_DELETE*)packet.GetPacketData();
	res->wOpCode = GU_CHARTITLE_DELETE;
	res->tblIndex = title + 1;
	packet.SetPacketLen(sizeof(sGU_CHARTITLE_DELETE));
	app->Send(GetClientSessionID(), &packet);

	CNtlPacket packetQry(sizeof(sGQ_CHARTITLE_DELETE_REQ));
	sGQ_CHARTITLE_DELETE_REQ * resQry = (sGQ_CHARTITLE_DELETE_REQ*)packetQry.GetPacketData();
	resQry->wOpCode = GQ_CHARTITLE_DELETE_REQ;
	resQry->charId = GetCharID();
	resQry->charTitle = title + 1;
	memcpy(resQry->TitleIndexFlag, m_TitleIndexFlag, sizeof(m_TitleIndexFlag));
	packetQry.SetPacketLen(sizeof(sGQ_CHARTITLE_DELETE_REQ));
	app->SendTo(app->GetQueryServerSession(), &packetQry);

	if (GetCharTitleID() == title + 1)
	{
		//send to database
		CNtlPacket packet2(sizeof(sGQ_CHARTITLE_SELECT_REQ));
		sGQ_CHARTITLE_SELECT_REQ* res2 = (sGQ_CHARTITLE_SELECT_REQ*)packet2.GetPacketData();
		res2->wOpCode = GQ_CHARTITLE_SELECT_REQ;
		res2->charId = GetCharID();
		res2->charTitle = INVALID_TBLIDX;
		app->SendTo(app->GetQueryServerSession(), &packet2);

		//send nfy to other players that my title updated
		CNtlPacket packet3(sizeof(sGU_CHARTITLE_SELECT_NFY));
		sGU_CHARTITLE_SELECT_NFY* res3 = (sGU_CHARTITLE_SELECT_NFY*)packet3.GetPacketData();
		res3->wOpCode = GU_CHARTITLE_SELECT_NFY;
		res3->charTitle = INVALID_TBLIDX;
		res3->handle = GetID();
		BroadcastToNeighbor(&packet3);

		//update char id
		SetCharTitleID(INVALID_TBLIDX);

		//update attributes
		GetCharAtt()->CalculateAll();
	}
}

//--------------------------------------------------------------------------------------//
//		UPDATE ZENI
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateZeni(BYTE ChangeType, DWORD dwAmount, bool bIsNew, bool bDoQuery/* = true*/, bool bSendPacket/* = true*/)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (bIsNew)
		m_dwZeni = UnsignedSafeIncrease<DWORD>(m_dwZeni, dwAmount);
	else
		m_dwZeni = UnsignedSafeDecrease<DWORD>(m_dwZeni, dwAmount);

	if (bDoQuery)
	{
		CNtlPacket pQry(sizeof(sGQ_UPDATE_CHAR_ZENNY_REQ));
		sGQ_UPDATE_CHAR_ZENNY_REQ * qRes = (sGQ_UPDATE_CHAR_ZENNY_REQ *)pQry.GetPacketData();
		qRes->wOpCode = GQ_UPDATE_CHAR_ZENNY_REQ;
		qRes->handle = GetID();
		qRes->charId = GetCharID();
		qRes->dwZenny = dwAmount;
		qRes->bIsNew = bIsNew;
		qRes->byZennyChangeType = ChangeType;
		pQry.SetPacketLen(sizeof(sGQ_UPDATE_CHAR_ZENNY_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);
	}

	if (bSendPacket)
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_ZENNY));
		sGU_UPDATE_CHAR_ZENNY * res = (sGU_UPDATE_CHAR_ZENNY *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_ZENNY;
		res->handle = GetID();
		res->byChangeType = ChangeType;
		res->dwZenny = m_dwZeni;
		res->bIsNew = bIsNew;
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_ZENNY));
		app->Send(GetClientSessionID(), &packet);
	}
}

//--------------------------------------------------------------------------------------//
//		CHECK IF PLAYER CAN RECEIVE ZENI
//--------------------------------------------------------------------------------------//
bool CPlayer::CanReceiveZeni(DWORD dwZeni)
{
	if (GetZeni() >= NTL_CHAR_MAX_SAVE_ZENNY)
		return false;

	DWORD dwFinal = UnsignedSafeIncrease<DWORD>(GetZeni(), dwZeni);
	if (dwFinal > NTL_CHAR_MAX_SAVE_ZENNY || dwFinal == INVALID_DWORD)
		return false;

	return true;
}


void CPlayer::AddCashShopItem(QWORD qwProductID, sCASHITEM_BRIEF* brief)
{
	sCASHITEM_BRIEF* newbrief = new sCASHITEM_BRIEF;
	memcpy(newbrief, brief, sizeof(sCASHITEM_BRIEF));
	mapCashShopItems.insert(std::make_pair(qwProductID, newbrief));
}

void CPlayer::DelCashShopItem(QWORD qwProductID)
{
	std::map<QWORD, sCASHITEM_BRIEF*>::iterator it = mapCashShopItems.find(qwProductID);
	if (it != mapCashShopItems.end())
	{
		sCASHITEM_BRIEF* pBrief = it->second;

		mapCashShopItems.erase(it);
		delete pBrief;
	}
}

sCASHITEM_BRIEF * CPlayer::GetCashShopItem(QWORD qwProductID)
{
	std::map<QWORD, sCASHITEM_BRIEF*>::iterator it = mapCashShopItems.find(qwProductID);
	if (it != mapCashShopItems.end())
	{
		return it->second;
	}

	return nullptr;
}


//--------------------------------------------------------------------------------------//
//		UPDATE MUDOSA POINTS
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateMudosaPoints(DWORD dwAmount, bool bQuery/* = true*/)
{
	CGameServer * app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_MUDOSA_POINT));
	sGU_UPDATE_CHAR_MUDOSA_POINT * res = (sGU_UPDATE_CHAR_MUDOSA_POINT *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_MUDOSA_POINT;
	res->dwMudosaPoint = dwAmount;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_MUDOSA_POINT));
	app->Send(GetClientSessionID(), &packet);

	SetMudosaPoints(dwAmount);

	if (bQuery)
	{
		CNtlPacket pQry(sizeof(sGQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ));
		sGQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ * qRes = (sGQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ *)pQry.GetPacketData();
		qRes->wOpCode = GQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ;
		qRes->handle = GetID();
		qRes->charId = GetCharID();
		qRes->dwMudosaPoint = dwAmount;
		pQry.SetPacketLen(sizeof(sGQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);
	}
}
//--------------------------------------------------------------------------------------//
//		UPDATE WAGU WAGO POINTS
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateWaguPoints(DWORD dwPoints)
{
	SetWaguPoints(dwPoints);

	if(GetWaguPoints() > NTL_MAX_WAGU_WAGU_SHOPPOINTS)
		SetWaguPoints(NTL_MAX_WAGU_WAGU_SHOPPOINTS);
}

//--------------------------------------------------------------------------------------//
//		UPDATE MAX RP BALLS
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateMaxRpBalls()
{
	BYTE rpball = CalculateRPBallMaxByLevel(GetLevel());
	if (rpball != GetMaxRPBall())
	{
		SetMaxRPBall(rpball);

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_RP_BALL_MAX));
		sGU_UPDATE_CHAR_RP_BALL_MAX * res = (sGU_UPDATE_CHAR_RP_BALL_MAX*)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_RP_BALL_MAX;
		res->byMaxRPBall = rpball;
		g_pApp->Send(GetClientSessionID(), &packet);
	}
}

//--------------------------------------------------------------------------------------//
//		CANCEL CHARGING
//--------------------------------------------------------------------------------------//
void CPlayer::CancelCharging()
{
	CNtlPacket packet(sizeof(sGU_CHAR_CHARGE_CANCELED_NFY));
	sGU_CHAR_CHARGE_CANCELED_NFY * res = (sGU_CHAR_CHARGE_CANCELED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_CHARGE_CANCELED_NFY;
	res->hSubject = GetID();
	packet.SetPacketLen(sizeof(sGU_CHAR_CHARGE_CANCELED_NFY));
	Broadcast(&packet);

	SendCharStateStanding();
}



//--------------------------------------------------------------------------------------//
//		START INCREASING RP
//--------------------------------------------------------------------------------------//
void CPlayer::StartIncreaseRP()
{
	m_bDecreaseRpFlag = false;

	CNtlPacket packet(sizeof(sGU_AVATAR_RP_INCREASE_START_NFY));
	sGU_AVATAR_RP_INCREASE_START_NFY * res = (sGU_AVATAR_RP_INCREASE_START_NFY *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_RP_INCREASE_START_NFY;
	packet.SetPacketLen(sizeof(sGU_AVATAR_RP_INCREASE_START_NFY));
	g_pApp->Send(GetClientSessionID(), &packet);
}
//--------------------------------------------------------------------------------------//
//		STOP INCREASING RP
//--------------------------------------------------------------------------------------//
void CPlayer::StopIncreaseRP()
{
	m_bDecreaseRpFlag = false;
	m_dwStartRpDecreaseTickCount = NTL_CHAR_RP_REGEN_WAIT_TIME;

	CNtlPacket packet(sizeof(sGU_AVATAR_RP_INCREASE_STOP_NFY));
	sGU_AVATAR_RP_INCREASE_STOP_NFY * res = (sGU_AVATAR_RP_INCREASE_STOP_NFY *)packet.GetPacketData();
	res->wOpCode = GU_AVATAR_RP_INCREASE_STOP_NFY;
	packet.SetPacketLen(sizeof(sGU_AVATAR_RP_INCREASE_STOP_NFY));
	g_pApp->Send(GetClientSessionID(), &packet);
}

void CPlayer::DecreaseRPTickProcess(DWORD dwTickCount)
{
	if (m_bDecreaseRpFlag == false)
	{
		if (IsFainting() == false && GetCombatMode() == false && GetCharStateID() != CHARSTATE_CHARGING)
		{
			m_dwStartRpDecreaseTickCount = UnsignedSafeDecrease<DWORD>(m_dwStartRpDecreaseTickCount, dwTickCount);
			if (m_dwStartRpDecreaseTickCount == 0)
			{
				m_fRpDecreaseTickCount = 0.0f;
				m_bDecreaseRpFlag = true;
				m_fRpCurrentValue = (float)GetCurRP();
				m_fRPChargingTime = m_fRpCurrentValue / ((float)GetCharAtt()->GetRPDimimutionRate() / 4.0f);

				CNtlPacket packet(sizeof(sGU_AVATAR_RP_DECREASE_START_NFY));
				sGU_AVATAR_RP_DECREASE_START_NFY * res = (sGU_AVATAR_RP_DECREASE_START_NFY *)packet.GetPacketData();
				res->wOpCode = GU_AVATAR_RP_DECREASE_START_NFY;
				packet.SetPacketLen(sizeof(sGU_AVATAR_RP_DECREASE_START_NFY));
				g_pApp->Send(GetClientSessionID(), &packet);
			}
		}

		return;
	}


	if (IsFainting() == false && GetCombatMode() == false && GetCharStateID() != CHARSTATE_CHARGING)
	{
		if (GetCurRP() > 0 || GetCurRPBall() > 0)
		{
			float fDestValue = 0.0f;

			m_fRpDecreaseTickCount += (float)dwTickCount / 1000.0f;

			if (m_fRpDecreaseTickCount >= m_fRPChargingTime)
			{
				m_fRpDecreaseTickCount = m_fRPChargingTime;
			}

			float fValue = m_fRpDecreaseTickCount / m_fRPChargingTime;
			if (fValue > 1.0f || m_fRPChargingTime == 0.0f)
				fValue = fDestValue;
			else if (fValue < 0.0f)
				fValue = m_fRpCurrentValue;
			else
				fValue = fValue * fDestValue + (1 - fValue) * m_fRpCurrentValue;
			
			//NTL_PRINT(PRINT_APP, "fValue %f m_fRpCurrentValue %f m_fRpDecreaseTickCount %f m_fRPChargingTime %f, GetCharAtt()->GetLastRPDimimutionRate() %u", fValue, m_fRpCurrentValue, m_fRpDecreaseTickCount, m_fRPChargingTime, GetCharAtt()->GetLastRPDimimutionRate());

			if ((WORD)fValue <= 0)
			{
				m_fRpDecreaseTickCount = 0.0f;
				m_fRPChargingTime = (float)GetCharAtt()->GetMaxRP() / ((float)GetCharAtt()->GetRPDimimutionRate() / 4.0f);
					
				if (GetCurRPBall() > 0)
				{
					UpdateRpBall(1, false, true);
					SetCurRP(GetCharAtt()->GetMaxRP());
					m_fRpCurrentValue = (float)GetCharAtt()->GetMaxRP();

					return;
				}
			}

			SetCurRP((WORD)fValue);
		}
	}
	else
	{
		StopIncreaseRP();
	}
}


//--------------------------------------------------------------------------------------//
//		UPDATE SKILL POINTS
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateCharSP(DWORD dwSkillPoints)
{
	SetSkillPoints(dwSkillPoints);

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_SP));
	sGU_UPDATE_CHAR_SP * res = (sGU_UPDATE_CHAR_SP *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_SP;
	res->dwSpPoint = dwSkillPoints;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_SP));
	g_pApp->Send(GetClientSessionID(), &packet);
}
//--------------------------------------------------------------------------------------//
//		UPDATE ADULT
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateAdult(bool bSetAdult)
{
	CGameServer* app = (CGameServer*)g_pApp;

	player_data.bIsAdult = bSetAdult;

	CNtlPacket packetQry(sizeof(sGQ_SWITCH_CHILD_ADULT));
	sGQ_SWITCH_CHILD_ADULT * resQry = (sGQ_SWITCH_CHILD_ADULT *)packetQry.GetPacketData();
	resQry->wOpCode = GQ_SWITCH_CHILD_ADULT;
	resQry->charId = GetCharID();
	resQry->hSubject = GetID();
	resQry->bIsAdult = bSetAdult;
	packetQry.SetPacketLen(sizeof(sGQ_SWITCH_CHILD_ADULT));
	app->SendTo(app->GetQueryServerSession(), &packetQry);

	CNtlPacket packet(sizeof(sGU_CHILD_ADULT_SWITCHED_NFY));
	sGU_CHILD_ADULT_SWITCHED_NFY * res = (sGU_CHILD_ADULT_SWITCHED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHILD_ADULT_SWITCHED_NFY;
	res->bIsAdult = bSetAdult;
	res->hSubject = GetID();
	packet.SetPacketLen(sizeof(sGU_CHILD_ADULT_SWITCHED_NFY));
	Broadcast(&packet);
}
//--------------------------------------------------------------------------------------//
//		UPDATE CLASS
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateClass(BYTE byClass)
{
	sPC_TBLDAT* pcdata = (sPC_TBLDAT*)g_pTableContainer->GetPcTable()->GetPcTbldat(GetTbldat()->byRace, byClass, GetTbldat()->byGender);
	if (pcdata)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		m_pTbldat = pcdata;

		CNtlPacket packetQry(sizeof(sGQ_CHAR_CONVERT_CLASS_REQ));
		sGQ_CHAR_CONVERT_CLASS_REQ * resQry = (sGQ_CHAR_CONVERT_CLASS_REQ *)packetQry.GetPacketData();
		resQry->wOpCode = GQ_CHAR_CONVERT_CLASS_REQ;
		resQry->charId = GetCharID();
		resQry->handle = GetID();
		resQry->byClass = byClass;
		packetQry.SetPacketLen(sizeof(sGQ_CHAR_CONVERT_CLASS_REQ));
		app->SendTo(app->GetQueryServerSession(), &packetQry);

		CNtlPacket packet(sizeof(sGU_CHAR_CONVERT_CLASS));
		sGU_CHAR_CONVERT_CLASS * res = (sGU_CHAR_CONVERT_CLASS *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_CONVERT_CLASS;
		res->byClass = byClass;
		res->hTarget = GetID();
		packet.SetPacketLen(sizeof(sGU_CHAR_CONVERT_CLASS));
		Broadcast(&packet);
	}
}

//--------------------------------------------------------------------------------------//
//		ADD EXP
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateExp(DWORD Exp, bool bIsMonsterKill)
{
	CGameServer * app = (CGameServer*)NtlSfxGetApp();

	if (GetLevel() >= app->GetPlayerMaxLevel())
		return; //dont give exp if pc already max level

	DWORD bonusexp = 0;

	if (bIsMonsterKill == true)
	{
		bonusexp = DWORD((float)Exp * (float)GetCharAtt()->GetExpBoost() / 100.f);

		bonusexp += DWORD((float)Exp * (float)g_pExpEvent->GetExpBonus() / 100.f);
	}

	//printf("bonusexp %d Exp %d\n", bonusexp, Exp);

	if ((GetExp() + Exp + bonusexp) > GetMaxExp())
	{
		DWORD newexp = (GetExp() + Exp + bonusexp) - GetMaxExp(); //exp needed until level up

		this->LevelUp(newexp);
		this->SetExp(newexp);

		/* Set EXP*/
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_EXP));
		sGU_UPDATE_CHAR_EXP * res = (sGU_UPDATE_CHAR_EXP*)packet.GetPacketData();
		res->handle = this->GetID();
		res->wOpCode = GU_UPDATE_CHAR_EXP;
		res->dwCurExp = newexp;
		res->dwAcquisitionExp = Exp;
		res->dwIncreasedExp = Exp + bonusexp;
		res->dwBonusExp = bonusexp;
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_EXP));
		g_pApp->Send(GetClientSessionID(), &packet);

	}
	else
	{
		/* Player didnt lvl up so gain exp */
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_EXP));
		sGU_UPDATE_CHAR_EXP * res = (sGU_UPDATE_CHAR_EXP*)packet.GetPacketData();
		res->handle = this->GetID();
		res->wOpCode = GU_UPDATE_CHAR_EXP;
		res->dwCurExp = this->GetExp() + Exp + bonusexp;
		res->dwAcquisitionExp = Exp;
		res->dwIncreasedExp = Exp + bonusexp;
		res->dwBonusExp = bonusexp;
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_EXP));
		g_pApp->Send(GetClientSessionID(), &packet);

		this->SetExp(res->dwCurExp);
	}
}
//--------------------------------------------------------------------------------------//
//		LEVEL UP
//--------------------------------------------------------------------------------------//
void CPlayer::LevelUp(DWORD dwNewExp, BYTE byLv/* = 1*/)
{
	CGameServer * app = (CGameServer*)NtlSfxGetApp();
	BYTE curlv = GetLevel();

	sEXP_TBLDAT *ExpData = (sEXP_TBLDAT *)g_pTableContainer->GetExpTable()->FindData(curlv + byLv);
	if (ExpData)
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LEVEL));
		sGU_UPDATE_CHAR_LEVEL * res = (sGU_UPDATE_CHAR_LEVEL*)packet.GetPacketData();
		res->byCurLevel = curlv + byLv;
		res->byPrevLevel = curlv;
		res->dwMaxExpInThisLevel = ExpData->dwNeed_Exp;
		res->handle = GetID();
		res->wOpCode = GU_UPDATE_CHAR_LEVEL;
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_LEVEL));
		Broadcast(&packet);

		player_data.dwMaxExpInThisLevel = ExpData->dwNeed_Exp;

		DWORD getsp = Dbo_GetLevelUpGainSP(curlv + byLv, (curlv + byLv) - curlv);
		UpdateCharSP(GetSkillPoints() + getsp);
		SetLevel(curlv + byLv);

		/*CALCULATE NEW ATTRIBUTE*/
		GetCharAtt()->CalculateAll();

		/*Update max RP Ball*/
		UpdateMaxRpBalls();

		//send to chat server
		app->GetChatServerSession()->SendUpdatePcLevel(this);

		//send to query server

		CNtlPacket packetQry(sizeof(sGQ_PC_UPDATE_LEVEL_REQ));
		sGQ_PC_UPDATE_LEVEL_REQ* resQry = (sGQ_PC_UPDATE_LEVEL_REQ*)packetQry.GetPacketData();
		resQry->wOpCode = GQ_PC_UPDATE_LEVEL_REQ;
		resQry->handle = GetID();
		resQry->charId = GetCharID();
		resQry->dwEXP = dwNewExp;
		resQry->byLevel = curlv + byLv;
		resQry->dwSP = GetSkillPoints();
		packetQry.SetPacketLen(sizeof(sGQ_PC_UPDATE_LEVEL_REQ));
		app->SendTo(app->GetQueryServerSession(), &packetQry);


		/*SET LP/EP TO FULL*/
		UpdateCurLpEp(GetMaxLP(), GetMaxEP(), true, false);

		//update party
		if (this->GetPartyID() != INVALID_PARTYID && this->GetParty())
			this->GetParty()->UpdateMemberLevel(this);
	}
	else ERR_LOG(LOG_SYSTEM, "Couldnt find exp table for level %u player %u\n", curlv + byLv, GetCharID());
}


//--------------------------------------------------------------------------------------//
//		CREATE SKILL MANAGER
//--------------------------------------------------------------------------------------//
CSkillManager* CPlayer::CreateSkillManager()
{
	CSkillManagerPc* pManager = new CSkillManagerPc;

	if (!pManager->Create(this, NTL_MAX_PC_HAVE_SKILL))
	{
		ERR_LOG(LOG_SYSTEM, "StateManager Create Fail");
		SAFE_DELETE(pManager);
		return NULL;
	}

	return pManager;
}

//--------------------------------------------------------------------------------------//
//		CREATE BUFF MANAGER
//--------------------------------------------------------------------------------------//
CBuffManager* CPlayer::CreateBuffManager()
{
	CBuffManagerPc* pManager = new CBuffManagerPc;

	if (!pManager->Create(this))
	{
		ERR_LOG(LOG_SYSTEM, "BuffManager Create Fail");
		return NULL;
	}

	return pManager;
}

//--------------------------------------------------------------------------------------//
//		CREATE CHAR ATTRIBUTE MANAGER
//--------------------------------------------------------------------------------------//
CCharacterAtt* CPlayer::CreateCharAttManager()
{
	CCharacterAttPC* pManager = new CCharacterAttPC;

	if (!pManager->Create(this))
	{
		ERR_LOG(LOG_SYSTEM, "CCharacterAtt Create Fail");
		return NULL;
	}

	return pManager;
}


//--------------------------------------------------------------------------------------//
//		UPDATE LP
//--------------------------------------------------------------------------------------//
bool CPlayer::UpdateCurLP(int lpDiff, bool bIncrease, bool bAutoRecover/*, sLP_TRACE_DATA *rLpTraceData*/)
{
	if (CCharacter::UpdateCurLP(lpDiff, bIncrease, bAutoRecover))
	{
		if (GetPartyID() != INVALID_PARTYID)
			GetParty()->SetMemberLP(this);

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE EP
//--------------------------------------------------------------------------------------//
bool CPlayer::UpdateCurEP(WORD wEpDiff, bool bIncrease, bool bAutoRecover)
{
	if (CCharacter::UpdateCurEP(wEpDiff, bIncrease, bAutoRecover))
	{
		//update party
		if (GetPartyID() != INVALID_PARTYID)
			GetParty()->SetMemberEP(this);

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE EP & LP
//--------------------------------------------------------------------------------------//
bool CPlayer::UpdateCurLpEp(int nLpDiff, WORD wEpDiff, bool bIncrease, bool bAutoRecover)
{
	if (CCharacter::UpdateCurLpEp(nLpDiff, wEpDiff, bIncrease, bAutoRecover))
	{
		//update party
		if (GetPartyID() != INVALID_PARTYID && GetParty())
		{
			GetParty()->SetMemberLpEp(this);

		}

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE RP
//--------------------------------------------------------------------------------------//
bool CPlayer::UpdateCurRP(WORD wRpDiff, bool bIncrease, bool bHitCharge)
{
	if (CCharacterObject::UpdateCurRP(wRpDiff, bIncrease, bHitCharge))
	{
		if (bIncrease) //check if we receive rp while rp is decreasing. This is required to stop the decreasing.
		{
			m_bDecreaseRpFlag = false;
			m_dwStartRpDecreaseTickCount = NTL_CHAR_RP_REGEN_WAIT_TIME;
		}

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_RP));
		sGU_UPDATE_CHAR_RP * res = (sGU_UPDATE_CHAR_RP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_RP;
		res->bHitDelay = bHitCharge;
		res->wCurRP = GetCurRP();
		res->wMaxRP = GetCharAtt()->GetMaxRP();
		SendPacket(&packet);

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE AP
//--------------------------------------------------------------------------------------//
bool CPlayer::UpdateCurAP(int apDiff, bool bIncrease)
{
	if (CCharacterObject::UpdateCurAP(apDiff, bIncrease))
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_AP));
		sGU_UPDATE_CHAR_AP * res = (sGU_UPDATE_CHAR_AP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_AP;
		res->handle = GetID();
		res->curAP = GetCurAP();
		res->maxAP = GetCharAtt()->GetMaxAP();
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_AP));
		SendPacket(&packet);

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE RP BALL
//--------------------------------------------------------------------------------------//
bool CPlayer::UpdateRpBall(BYTE byDiff, bool bIncrease, bool bDropByTime)
{
	if (CCharacterObject::UpdateRpBall(byDiff, bIncrease, bDropByTime))
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_RP_BALL));
		sGU_UPDATE_CHAR_RP_BALL * res = (sGU_UPDATE_CHAR_RP_BALL*)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_RP_BALL;
		res->byCurRPBall = GetCurRPBall();
		res->bDropByTime = bDropByTime;
		g_pApp->Send(GetClientSessionID(), &packet);

		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//		UPDATE EXP AND LEVEL
//--------------------------------------------------------------------------------------//
void CPlayer::SetHoiPoiMixExpAndLevel(DWORD exp)
{
	if (GetHoiPoiMixLv() >= DBO_HOIPOIMIX_MIX_LEVEL_MAX) //check if already max level
		return;

	sITEM_MIX_EXP_TBLDAT* mixExp = (sITEM_MIX_EXP_TBLDAT*)g_pTableContainer->GetItemMixExpTable()->FindData(GetHoiPoiMixLv());
	if (mixExp)
	{
		BYTE curlevel = GetHoiPoiMixLv();
		DWORD curexp = UnsignedSafeIncrease<DWORD>(GetHoiPoiMixExp(), exp);

		if (curexp >= mixExp->dwNeedEXP)
		{
			if (g_pTableContainer->GetItemMixExpTable()->FindData(curlevel + 1)) //check if next level exist
			{
				curlevel += 1;
				
				player_data.sMixData.byLevel = curlevel; //set cur level

				curexp -= mixExp->dwNeedEXP;
			}
			else
			{
				curexp = 0;
			}
		}
		
		SetHoiPoiMixExp(curexp);
	}
}


//--------------------------------------------------------------------------------------//
//		CREATE PRIVATE SHOP
//--------------------------------------------------------------------------------------//
void CPlayer::CreatePrivateShop()
{
	CGameServer* app = (CGameServer*)g_pApp;

	WORD wResultCode = GAME_SUCCESS;

	if (app->GetGsChannel() == DOJO_CHANNEL_INDEX)
		wResultCode = GAME_FAIL;
	else if (GetCurWorld() == NULL)
	{
		wResultCode = GAME_WORLD_NOT_EXIST;
	}
	else if (GetCurWorld()->GetTbldat()->bDynamic)
	{
		wResultCode = GAME_FAIL;
	}
	else if (GetDragonballScrambleBallFlag() > 0 || (GetDragonballScramble() && app->GetGsChannel() == 0))
	{
		wResultCode = GAME_FAIL;
	}
	else if (GetCharStateID() != CHARSTATE_STANDING || GetAirState() == AIR_STATE_ON || GetCombatMode() || GetAspectStateId() != ASPECTSTATE_INVALID) //Only allow open shop while standing
	{
		wResultCode = GAME_CHAR_IS_WRONG_STATE;
	}
	else if (GetPrivateShop() || GetVisitPrivateShop())
	{
		wResultCode = GAME_PRIVATESHOP_PRIVATESHOP_ANOTHER_PRIVATESHOP;
	}
	else if (GetNaviEngine()->IsBasicAttributeSet(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z, DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP))
	{
		wResultCode = GAME_PRIVATESHOP_PRIVATESHOP_NOT_CREATE_PLACE;
	}
	else if (GetFacingHandle() != INVALID_HOBJECT)
	{
		wResultCode = GAME_FAIL;
	}
	else if(GetNpcShopHandle() != INVALID_HOBJECT)
	{
		wResultCode = GAME_FAIL;
	}

	if (wResultCode == GAME_SUCCESS)
	{
		m_pkShop = g_pPShopManager->CreatePrivateShop(this);

		if (m_pkShop)
		{
			CNtlPacket packet(sizeof(sGU_PRIVATESHOP_CREATE_RES));
			sGU_PRIVATESHOP_CREATE_RES* res = (sGU_PRIVATESHOP_CREATE_RES*)packet.GetPacketData();
			res->wOpCode = GU_PRIVATESHOP_CREATE_RES;
			res->wResultCode = GAME_SUCCESS;
			res->sPrivateShopData.hOwner = GetID();
			res->sPrivateShopData.sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_NULL;
			res->sPrivateShopData.sSummaryPrivateShopData.bIsOwnerEmpty = true;
			packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_CREATE_RES));
			g_pApp->Send(GetClientSessionID(), &packet);

			CNtlPacket packet2(sizeof(sGU_PRIVATESHOP_CREATE_NFY));
			sGU_PRIVATESHOP_CREATE_NFY* res2 = (sGU_PRIVATESHOP_CREATE_NFY*)packet2.GetPacketData();
			res2->wOpCode = GU_PRIVATESHOP_CREATE_NFY;
			res2->hOwner = GetID();
			res2->sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_NULL;
			res2->sSummaryPrivateShopData.bIsOwnerEmpty = true;
			packet2.SetPacketLen(sizeof(sGU_PRIVATESHOP_CREATE_NFY));
			Broadcast(&packet2, this);

			SendCharStatePrivateShop(false, PRIVATESHOP_STATE_NULL, L"");

			return;
		}
		else
		{
			wResultCode = GAME_FAIL;
		}
	}
	
	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_CREATE_RES));
	sGU_PRIVATESHOP_CREATE_RES* res = (sGU_PRIVATESHOP_CREATE_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_CREATE_RES;
	res->wResultCode = wResultCode;
	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_CREATE_RES));
	g_pApp->Send(GetClientSessionID(), &packet);
}
//--------------------------------------------------------------------------------------//
//		CLOSE PRIVATE SHOP
//--------------------------------------------------------------------------------------//
void CPlayer::ClosePrivateShop()
{
	if (GetPrivateShop() && g_pPShopManager->DestroyPrivateShop(this))
	{
		m_pkShop = NULL;

		CNtlPacket packet(sizeof(sGU_PRIVATESHOP_EXIT_RES));
		sGU_PRIVATESHOP_EXIT_RES* res = (sGU_PRIVATESHOP_EXIT_RES*)packet.GetPacketData();
		res->wOpCode = GU_PRIVATESHOP_EXIT_RES;
		res->wResultCode = GAME_SUCCESS;
		packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_EXIT_RES));
		SendPacket(&packet);

		SendCharStateStanding();
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_PRIVATESHOP_EXIT_RES));
		sGU_PRIVATESHOP_EXIT_RES* res = (sGU_PRIVATESHOP_EXIT_RES*)packet.GetPacketData();
		res->wOpCode = GU_PRIVATESHOP_EXIT_RES;
		res->wResultCode = GAME_FAIL;
		packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_EXIT_RES));
		SendPacket(&packet);
	}
}

bool CPlayer::AddWarFogFlag(TBLIDX ContentTblidx)
{
	BYTE* curWarFlag = this->GetWarFogFlag();
	CNtlBitFlagManager flagMgr;

	flagMgr.Create(curWarFlag, NTL_MAX_COUNT_WAR_FOG);

	if (flagMgr.Set(ContentTblidx) == false)
	{
		ERR_LOG(LOG_USER, "AddWarFogFlag: Unable to set ContentTblidx %d.", ContentTblidx);
		return false;
	}

	SetWarFogFlag(flagMgr.GetRawData());

	return true;
}

bool CPlayer::CheckWarFog(TBLIDX ContentTblidx)
{
	CNtlBitFlagManager flagMgr;
	flagMgr.Create(warFogFlag, NTL_MAX_COUNT_WAR_FOG);

	return flagMgr.IsSet(ContentTblidx);
}

///////////////////
// clear mails
void CPlayer::ClearMails()
{
	for (std::map<MAILID, sMAIL_NEW_PROFILE*>::iterator it = m_map_MailInfo.begin(); it != m_map_MailInfo.end(); it++)
	{
		sMAIL_NEW_PROFILE* pInfo = it->second;
		SAFE_DELETE(pInfo);
	}

	m_map_MailInfo.clear();
}


bool CPlayer::AddMail(sMAIL_NEW_PROFILE* mail)
{
	if (mail == NULL)
		return false;

	std::map<MAILID, sMAIL_NEW_PROFILE*>::iterator it = m_map_MailInfo.find(mail->mailID);
	if (m_map_MailInfo.end() != it)
		return false;

	sMAIL_NEW_PROFILE* newmail = new sMAIL_NEW_PROFILE;
	memcpy(newmail, mail, sizeof(sMAIL_NEW_PROFILE));

	m_map_MailInfo.insert(std::make_pair(newmail->mailID, newmail));
	return true;
}


sMAIL_NEW_PROFILE* CPlayer::GetMail(MAILID mailId)
{
	std::map<MAILID, sMAIL_NEW_PROFILE*>::iterator it = m_map_MailInfo.find(mailId);
	if (m_map_MailInfo.end() == it)
		return NULL;

	return it->second;
}


void CPlayer::DeleteMail(MAILID mailId)
{
	std::map<MAILID, sMAIL_NEW_PROFILE*>::iterator it = m_map_MailInfo.find(mailId);
	if (m_map_MailInfo.end() == it)
		return;

	sMAIL_NEW_PROFILE* mail = it->second;

	m_map_MailInfo.erase(it);
	delete mail;
}


void CPlayer::ReloadMailsStatistic(bool bCheckTime/* = true*/)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (bCheckTime)
	{
		if (app->GetCurTickCount() < m_dwLastMailReloadStatistic)
			return;
	}

	m_dwLastMailReloadStatistic = app->GetCurTickCount() + 60000;

	CNtlPacket pQry(sizeof(sGQ_MAIL_RELOAD_REQ));
	sGQ_MAIL_RELOAD_REQ * qRes = (sGQ_MAIL_RELOAD_REQ *)pQry.GetPacketData();
	qRes->wOpCode = GQ_MAIL_RELOAD_REQ;
	qRes->handle = GetID();
	qRes->hObject = INVALID_HOBJECT;
	qRes->charID = GetCharID();
	qRes->bIsSchedule = true;
	pQry.SetPacketLen(sizeof(sGQ_MAIL_RELOAD_REQ));
	app->SendTo(app->GetQueryServerSession(), &pQry);
}

void CPlayer::ManualReloadMails(HOBJECT hObject)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (app->GetCurTickCount() >= m_dwLastMailReload)
	{
		m_dwLastMailReload = app->GetCurTickCount() + 60000;

		CNtlPacket pQry(sizeof(sGQ_MAIL_RELOAD_REQ));
		sGQ_MAIL_RELOAD_REQ * qRes = (sGQ_MAIL_RELOAD_REQ *)pQry.GetPacketData();
		qRes->wOpCode = GQ_MAIL_RELOAD_REQ;
		qRes->handle = GetID();
		qRes->hObject = hObject;
		qRes->charID = GetCharID();
		qRes->bIsSchedule = false;
		pQry.SetPacketLen(sizeof(sGQ_MAIL_RELOAD_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);
	}
	else
	{
		CNtlPacket packet2(sizeof(sGU_MAIL_RELOAD_RES));
		sGU_MAIL_RELOAD_RES * res2 = (sGU_MAIL_RELOAD_RES *)packet2.GetPacketData();
		res2->wOpCode = GU_MAIL_RELOAD_RES;
		res2->hObject = hObject;
		res2->wResultCode = GAME_FAIL;
		packet2.SetPacketLen(sizeof(sGU_MAIL_RELOAD_RES));
		g_pApp->Send(GetClientSessionID(), &packet2);
	}
}


void CPlayer::LoadMails(HOBJECT hObject)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (app->GetCurTickCount() >= m_dwLastMailLoad)
	{
		m_dwLastMailLoad = app->GetCurTickCount() + 60000;

		CNtlPacket pQry(sizeof(sGQ_MAIL_START_REQ));
		sGQ_MAIL_START_REQ * qRes = (sGQ_MAIL_START_REQ *)pQry.GetPacketData();
		qRes->wOpCode = GQ_MAIL_START_REQ;
		qRes->handle = GetID();
		qRes->hObject = hObject;
		qRes->charID = GetCharID();
		pQry.SetPacketLen(sizeof(sGQ_MAIL_START_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);
	}
	else
	{
		CNtlPacket packet2(sizeof(sGU_MAIL_START_RES));
		sGU_MAIL_START_RES * res2 = (sGU_MAIL_START_RES *)packet2.GetPacketData();
		res2->wOpCode = GU_MAIL_START_RES;
		res2->hObject = hObject;
		res2->bIsAway = IsMailAway();
		res2->wResultCode = GAME_SUCCESS;
		packet2.SetPacketLen(sizeof(sGU_MAIL_START_RES));
		app->Send(GetClientSessionID(), &packet2);
	}
}


//--------------------------------------------------------------------------------------//
//		INSERT MASCOT
//--------------------------------------------------------------------------------------//
void CPlayer::InsertMascot(CItemPet* mascot)
{
	if (mascot == NULL)
	{
		ERR_LOG(LOG_USER, "Mascot data is NULL. Player %u", GetCharID());
		return;
	}

	m_mapMascotData.insert(std::make_pair(mascot->GetMascotData()->byIndex, mascot));
}
//--------------------------------------------------------------------------------------//
//		REGISTER MASCOT
//--------------------------------------------------------------------------------------//
bool CPlayer::RegisterMascot(TBLIDX mascotTblidx)
{
	if (m_mapMascotData.size() >= GMT_MAX_PC_HAVE_MASCOT)
		return false;

	BYTE i = 0;

	std::map<SLOTID, CItemPet*>::iterator it = m_mapMascotData.begin();
	while (it != m_mapMascotData.end())
	{
		if (it->first == i)
			i++;
		else
			break;

		++it;
	}


	sMASCOT_STATUS_TBLDAT * mascotTbldat = (sMASCOT_STATUS_TBLDAT*)g_pTableContainer->GetMascotStatusTable()->FindData(mascotTblidx);
	if (mascotTbldat)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		DWORD dwVP = 100 + (RandomRange(mascotTbldat->wVpUpMin, mascotTbldat->wVpUpMax) * (mascotTbldat->byRank - 1));

		CNtlPacket packetQry(sizeof(sGQ_MASCOT_REGISTER_REQ));
		sGQ_MASCOT_REGISTER_REQ* resQry = (sGQ_MASCOT_REGISTER_REQ*)packetQry.GetPacketData();
		resQry->wOpCode = GQ_MASCOT_REGISTER_REQ;
		resQry->charId = GetCharID();
		resQry->slotId = i;
		resQry->tblidx = mascotTblidx;
		resQry->byRank = mascotTbldat->byRank;
		resQry->dwVP = dwVP;
		packetQry.SetPacketLen(sizeof(sGQ_MASCOT_REGISTER_REQ));
		app->SendTo(app->GetQueryServerSession(), &packetQry);


		CItemPet* mascot = new CItemPet;

		sMASCOT_DATA_EX mascotdata;
		mascotdata.tblidx = mascotTblidx;
		mascotdata.itemTblidx = mascotTblidx;
		mascotdata.byIndex = i;
		mascotdata.dwCurVP = dwVP;
		mascotdata.dwMaxVP = dwVP;
		mascotdata.dwCurExp = 0;
		mascotdata.byItemRank = mascotTbldat->byRank;
		mascotdata.skillTblidx[0] = INVALID_TBLIDX;
		mascotdata.skillTblidx[1] = INVALID_TBLIDX;
		mascotdata.skillTblidx[2] = INVALID_TBLIDX;
		mascotdata.skillTblidx[3] = INVALID_TBLIDX;

		mascot->SetPetData(mascotTbldat, this, &mascotdata);

		InsertMascot(mascot);

		CNtlPacket packet(sizeof(sGU_MASCOT_REGISTER_EX_NFY));
		sGU_MASCOT_REGISTER_EX_NFY* res = (sGU_MASCOT_REGISTER_EX_NFY*)packet.GetPacketData();
		res->wOpCode = GU_MASCOT_REGISTER_EX_NFY;
		memcpy(&res->mascotdata, &mascotdata, sizeof(sMASCOT_DATA_EX));
		packet.SetPacketLen(sizeof(sGU_MASCOT_REGISTER_EX_NFY));
		app->Send(GetClientSessionID(), &packet);
	}
	else return false;

	return true;
}
//--------------------------------------------------------------------------------------//
//		DELETE MASCOT
//--------------------------------------------------------------------------------------//
void CPlayer::DeleteMascot(BYTE byIndex)
{
	std::map<SLOTID, CItemPet*>::iterator it = m_mapMascotData.find(byIndex);
	if (it != m_mapMascotData.end())
	{
		CItemPet* mascot = it->second;

		WORD resultcode = GAME_SUCCESS;

		//Check if pet is summoned
		if (player_data.mascotTblidx == mascot->GetMascotData()->tblidx)
			resultcode = SUMMONING_MASCOT_CANT_DELETE;

		CGameServer* app = (CGameServer*)g_pApp;

		CNtlPacket packet(sizeof(sGU_MASCOT_DELETE_EX_RES));
		sGU_MASCOT_DELETE_EX_RES* res = (sGU_MASCOT_DELETE_EX_RES*)packet.GetPacketData();
		res->wOpCode = GU_MASCOT_DELETE_EX_RES;
		res->wResultCode = resultcode;
		res->byIndex = byIndex;
		packet.SetPacketLen(sizeof(sGU_MASCOT_DELETE_EX_RES));
		app->Send(GetClientSessionID(), &packet);

		if (resultcode == GAME_SUCCESS)
		{
			m_mapMascotData.erase(it);

			CNtlPacket packetQry(sizeof(sGQ_MASCOT_DELETE_REQ));
			sGQ_MASCOT_DELETE_REQ* resQry = (sGQ_MASCOT_DELETE_REQ*)packetQry.GetPacketData();
			resQry->wOpCode = GQ_MASCOT_DELETE_REQ;
			resQry->charId = GetCharID();
			resQry->slotId = byIndex;
			packetQry.SetPacketLen(sizeof(sGQ_MASCOT_DELETE_REQ));
			app->SendTo(app->GetQueryServerSession(), &packetQry);

			SAFE_DELETE(mascot);
		}
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_MASCOT_DELETE_EX_RES));
		sGU_MASCOT_DELETE_EX_RES* res = (sGU_MASCOT_DELETE_EX_RES*)packet.GetPacketData();
		res->wOpCode = GU_MASCOT_DELETE_EX_RES;
		res->wResultCode = MASCOT_NOT_EXIST;
		packet.SetPacketLen(sizeof(sGU_MASCOT_DELETE_EX_RES));
		g_pApp->Send(GetClientSessionID(), &packet);
	}
}
bool CPlayer::OnlyDeleteMascot(BYTE byIndex)
{
	std::map<SLOTID, CItemPet*>::iterator it = m_mapMascotData.find(byIndex);
	if (it != m_mapMascotData.end())
	{
		CItemPet* mascot = it->second;

		m_mapMascotData.erase(it);

		SAFE_DELETE(mascot);

		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------//
//		FUSION MASCOT (lvl up)
//--------------------------------------------------------------------------------------//
void CPlayer::FusionMascot(BYTE byItemPlace, BYTE byItemPos, BYTE byMascotLevelUpSlot, BYTE byMascotOfferingSlot)
{
	CGameServer* app = (CGameServer*)g_pApp;

	WORD resultcode = GAME_SUCCESS;

	int nPercent = 70;
	ITEMID itemId = INVALID_ITEMID;

	CItem* item = NULL;
	CItemPet* mainMascot = NULL;
	CItemPet* offeringMascot = NULL;

	if (IsInvenContainer(byItemPlace))
	{
		item = GetPlayerItemContainer()->GetItem(byItemPlace, byItemPos);
		mainMascot = GetMascot(byMascotLevelUpSlot);
		offeringMascot = GetMascot(byMascotOfferingSlot);

		if (GetCurrentMascot()) //not allowed to have summoned pet while fusion
			resultcode = MASCOT_FAIL;
		else if (mainMascot == NULL || offeringMascot == NULL) //check if main mascot exist
			resultcode = MASCOT_NOT_EXIST;
		else if (mainMascot->GetExp() < mainMascot->GetNeedExp()) //check mascot exp
			resultcode = MASCOT_FAIL;
		else if (mainMascot->CanLevelUp() == false) //check if already max level
			resultcode = MASCOT_FAIL;
		else if (item)
		{
			if (item->GetTbldat()->byItem_Type == ITEM_TYPE_EVENT_MASCOT_LEVELUP && item->GetCount() > 0)
				nPercent = 100;
			else
				resultcode = MASCOT_ITEM_INVALID;
		}
	}
	else resultcode = GAME_FAIL;


	CNtlPacket packet(sizeof(sGU_MASCOT_FUSION_RES));
	sGU_MASCOT_FUSION_RES * res = (sGU_MASCOT_FUSION_RES *)packet.GetPacketData();
	res->wOpCode = GU_MASCOT_FUSION_RES;
	res->wResultCode = resultcode;
	res->bIsSuccess = Dbo_CheckProbability(nPercent);
	res->byMascotIndex = byMascotLevelUpSlot;
	res->byOfferingMascotIndex = byMascotOfferingSlot;
	res->byItemPlace = byItemPlace;
	res->byItemPos = byItemPos;

	if (resultcode == GAME_SUCCESS)
	{
		/*remove offering pet*/
		m_mapMascotData.erase(byMascotOfferingSlot);
		SAFE_DELETE(offeringMascot);

		if (item)
		{
			res->byItemCount = item->GetCount() - 1;
			itemId = item->GetItemID();
			item->SetCount(res->byItemCount, false, false);
		}

		if (res->bIsSuccess)
		{
			mainMascot->LevelUp();

			res->byNewRank = mainMascot->GetTbldat()->byRank;
			res->dwMaxVP = mainMascot->GetMaxVP();
			res->nextMascotTblidx = mainMascot->GetTblidx();
		}

		/*Send packet to query server*/
		CNtlPacket packetQry(sizeof(sGQ_MASCOT_FUSION_REQ));
		sGQ_MASCOT_FUSION_REQ* resQry = (sGQ_MASCOT_FUSION_REQ*)packetQry.GetPacketData();
		resQry->wOpCode = GQ_MASCOT_FUSION_REQ;
		resQry->handle = GetID();
		resQry->charId = GetCharID();
		resQry->byNewRank = res->byNewRank;
		resQry->bIsSuccess = res->bIsSuccess;
		resQry->dwMaxVP = res->dwMaxVP;
		resQry->nextMascotTblidx = res->nextMascotTblidx;
		resQry->byMascotIndex = res->byMascotIndex;
		resQry->byOfferingMascotIndex = res->byOfferingMascotIndex;
		resQry->byItemPlace = res->byItemPlace;
		resQry->byItemPos = res->byItemPos;
		resQry->byItemCount = res->byItemCount;
		resQry->itemId = itemId;
		packetQry.SetPacketLen(sizeof(sGQ_MASCOT_FUSION_REQ));
		app->SendTo(app->GetQueryServerSession(), &packetQry);
	}

	//send packet to client
	packet.SetPacketLen(sizeof(sGU_MASCOT_FUSION_RES));
	g_pApp->Send(GetClientSessionID(), &packet);
}

bool CPlayer::CanOpenRemoteBank()
{
	if (m_bRemoteBankSkillUsed)
	{
		m_bRemoteBankSkillUsed = false;
		return true;
	}

	return false;
}

CSummonPet * CPlayer::GetSummonPet()
{
	return g_pObjectManager->GetSummonPet(GetCurrentPetId());
}

CItemPet * CPlayer::FindMascot(TBLIDX mascotIdx)
{
	for (std::map<SLOTID, CItemPet*>::iterator it = m_mapMascotData.begin(); it != m_mapMascotData.end(); it++)
	{
		CItemPet* pPet = it->second;
		if (pPet->GetTblidx() == mascotIdx)
			return pPet;
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------//
//		GET MASCOT DATA
//--------------------------------------------------------------------------------------//
CItemPet* CPlayer::GetMascot(BYTE byIndex)
{
	std::map<SLOTID, CItemPet*>::iterator it = m_mapMascotData.find(byIndex);
	if (it != m_mapMascotData.end())
	{
		return it->second;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------//
//		SEND BATTLE COMBATING STATUS
//--------------------------------------------------------------------------------------//
void	CPlayer::UpdateBattleCombatMode(bool status)
{
	if (GetCombatMode() == true && status == true) //if we restart the combat mode
	{
		m_dwCombatModeTickCount = NTL_BATTLE_COMBAT_DISABLE;
	}
	else if (GetCombatMode() != status)
	{
		if (status == false)
		{
			//do not allow player get out of combat while having negative condition
			if(GetStateManager()->IsCharCondition(CHARCOND_TAUNT) || GetStateManager()->IsCharCondition(CHARCOND_CONFUSED) || GetStateManager()->IsCharCondition(CHARCOND_TERROR) || GetStateManager()->IsCharCondition(CHARCOND_TAIYOU_KEN)
				|| GetStateManager()->IsCharCondition(CHARCOND_BATTLE_INABILITY) || GetStateManager()->IsCharCondition(CHARCOND_SKILL_INABILITY) || GetStateManager()->IsCharCondition(CHARCOND_RABIES) || GetStateManager()->IsCharCondition(CHARCOND_DRUNK))
			{
				m_dwCombatModeTickCount = NTL_BATTLE_COMBAT_DISABLE;
				return;
			}

			//do not allow player get out of combat while having negative state
			if ((GetCharStateID() >= CHARSTATE_SLIDING && GetCharStateID() <= CHARSTATE_KEEPING_EFFECT) || GetCharStateID() == CHARSTATE_STUNNED || (GetCharStateID() >= CHARSTATE_PARALYZED && GetCharStateID() <= CHARSTATE_SANDBAG))
			{
				m_dwCombatModeTickCount = NTL_BATTLE_COMBAT_DISABLE;
				return;
			}

			ClearCrowdControlReduction();
		}
		else
		{
			m_dwCombatModeTickCount = NTL_BATTLE_COMBAT_DISABLE;
		}

		SetCombatMode(status);

		CNtlPacket packet(sizeof(sGU_CHAR_IS_BATTLECOMBATING));
		sGU_CHAR_IS_BATTLECOMBATING * res = (sGU_CHAR_IS_BATTLECOMBATING *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_IS_BATTLECOMBATING;
		res->bIsBattle = status;
		packet.SetPacketLen(sizeof(sGU_CHAR_IS_BATTLECOMBATING));
		g_pApp->Send(GetClientSessionID(), &packet);
	}
}

void CPlayer::LeaveRankBattleQueue()
{
	if (m_RankBattleRoomTblidx == INVALID_TBLIDX)
		return;

	CNtlPacket packet(sizeof(sGU_RANKBATTLE_LEAVE_NFY));
	sGU_RANKBATTLE_LEAVE_NFY * res = (sGU_RANKBATTLE_LEAVE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_LEAVE_NFY;
	packet.SetPacketLen(sizeof(sGU_RANKBATTLE_LEAVE_NFY));
	SendPacket(&packet);

	m_RankBattleRoomTblidx = INVALID_TBLIDX;
	m_RankBattleRegisterObject = INVALID_HOBJECT;
	m_RankBattleRoomId = INVALID_ROOMID;
}


void CPlayer::UpdateRankBattleScore(DWORD dwWin, DWORD dwDraw, DWORD dwLose, DWORD dwMileage, float fPoint, bool bKoWin, BYTE byBattlemode)
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_sScoreInfo.dwWin += dwWin;
	m_sScoreInfo.dwDraw += dwDraw;
	m_sScoreInfo.dwLose += dwLose;
	m_sScoreInfo.fPoint += fPoint;

	if (bKoWin)
	{
		m_sScoreInfo.wStraightKOWin += 1;

		if (m_sScoreInfo.wStraightKOWin > m_sScoreInfo.wMaxStraightKOWin)
			m_sScoreInfo.wMaxStraightKOWin = m_sScoreInfo.wStraightKOWin;
	}
	else
	{
		m_sScoreInfo.wStraightKOWin = 0;
	}

	if (dwWin)
	{
		m_sScoreInfo.wStraightWin += 1;

		if (m_sScoreInfo.wStraightWin > m_sScoreInfo.wMaxStraightWin)
			m_sScoreInfo.wMaxStraightWin = m_sScoreInfo.wStraightWin;
	}
	else
	{
		m_sScoreInfo.wStraightWin = 0;
	}


	CNtlPacket packet(sizeof(sGU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY));
	sGU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY * res = (sGU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY;
	res->byBattlemode = byBattlemode;
	memcpy(&res->sTotalScore, &m_sScoreInfo, sizeof(sRANKBATTLE_SCORE_INFO));
	SendPacket(&packet);

	if(dwMileage > 0)
		UpdateMudosaPoints(GetMudosaPoints() + dwMileage, false);


	CNtlPacket pQry(sizeof(sGQ_RANKBATTLE_SCORE_UPDATE_REQ));
	sGQ_RANKBATTLE_SCORE_UPDATE_REQ * qRes = (sGQ_RANKBATTLE_SCORE_UPDATE_REQ *)pQry.GetPacketData();
	qRes->wOpCode = GQ_RANKBATTLE_SCORE_UPDATE_REQ;
	qRes->handle = GetID();
	qRes->charID = GetCharID();
	qRes->byBattleMode = byBattlemode;
	memcpy(&qRes->sScoreInfo, &m_sScoreInfo, sizeof(sRANKBATTLE_SCORE_INFO));
	qRes->dwMudosaPoint = dwMileage;
	pQry.SetPacketLen(sizeof(sGQ_RANKBATTLE_SCORE_UPDATE_REQ));
	app->SendTo(app->GetQueryServerSession(), &pQry);
}

void CPlayer::InitBudokai()
{
	m_joinId = INVALID_JOINID;
	m_byMatchIndex = INVALID_BYTE;
	m_byBudokaiPcState = MATCH_MEMBER_STATE_NONE;
	m_wBudokaiTeamType = INVALID_TEAMTYPE;
}

void CPlayer::SetServerTeleportWorld(WORLDID worldId, TBLIDX WorldTblidx)
{
	m_sServerTeleportInfo.worldId = worldId;
	m_sServerTeleportInfo.worldTblidx = WorldTblidx;
	GetCurLoc().CopyTo(m_sServerTeleportInfo.vLoc);
	GetCurDir().CopyTo(m_sServerTeleportInfo.vDir);
}

void CPlayer::SpeedHackCheck(float fClientAppTime, bool bSpeedHack)
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_fLastAppTime = fClientAppTime;

	if (bSpeedHack)
	{
		ERR_LOG(LOG_USER, "Player %u is using speed-hack. LastHackCheck (in MS): %u", GetCharID(), m_dwLastHackCheck);
		GetClientSession()->Disconnect(false);
		return;
	}

	m_dwLastHackCheck = 0;
	m_fLastAppTime = fClientAppTime;
}


//--------------------------------------------------------------------------------------//
//		CANCEL GATHERING QUEST OBJ ITEM. USED WHEN PLAYER GETS ATTACKED OR DIE WHILE OPERATING
//--------------------------------------------------------------------------------------//
void	CPlayer::CancelOperating()
{
	SendCharStateStanding();
}

void CPlayer::UpdateDbScramble(DWORD dwTickDiff)
{
	if (GetCurWorld() && GetDragonballScrambleBallFlag() > 0)
	{
		m_dwScrambleUpdateTickCount = UnsignedSafeDecrease<DWORD>(m_dwScrambleUpdateTickCount, dwTickDiff);
		if (m_dwScrambleUpdateTickCount == 0)
		{
			m_dwScrambleUpdateTickCount = 2000;

			if (GetNaviEngine()->IsBasicAttributeSet(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) == true)
				g_pDragonballScramble->SpawnBall(this);
		}
	}
}

void CPlayer::CancelDice()
{
	if (HasItemDice())
	{
		if (GetParty())
		{
			CNtlPacket packet(sizeof(sGU_ITEM_DICE_RES));
			sGU_ITEM_DICE_RES * res = (sGU_ITEM_DICE_RES*)packet.GetPacketData();
			res->wOpCode = GU_ITEM_DICE_RES;
			res->hItemHandle = GetParty()->GetDiceItemHandle();
			res->wResultCode = GetID();
			res->byDice = DBO_ITEM_DICE_CANCELED;
			packet.SetPacketLen(sizeof(sGU_ITEM_DICE_RES));
			SendPacket(&packet);

			GetParty()->LeaveDice(GetID());
		}
		else
		{
			ERR_LOG(LOG_USER, "Can not cancel dice. Player is not inside a party.");
		}

		SetItemDice(false);
	}
}

void CPlayer::AfkCheck(DWORD dwTickDiff)
{
	if (GetStateManager()->GetCharStateTime() > (60000 * 15)) // 15 minutes
	{
		m_bIsAfk = true;
		m_dwAfkTime += dwTickDiff;
	}
	else
	{
		SetLastCameraMoveTick(); // to avoid disconnect when staying afk for a while

		m_bIsAfk = false;
		m_dwAfkTime = 0;
	}
}

void CPlayer::SetLastCameraMoveTick()
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_dwLastCameraMove = app->GetCurTickCount();
}


bool CPlayer::HasEventReward(TBLIDX rewardTblidx, CHARACTERID charId)
{
	std::map<TBLIDX, sEVENT_PC_NAME>::iterator it = m_mapEventReward.find(rewardTblidx);
	if (it != m_mapEventReward.end())
	{
		if (it->second.charId == INVALID_CHARACTERID || it->second.charId == charId)
			return true;
	}

	return false;
}

void CPlayer::LoadExistingEventReward()
{
	CNtlPacket packet(sizeof(sGU_EVENT_REWARD_LOAD_RES));
	sGU_EVENT_REWARD_LOAD_RES* res = (sGU_EVENT_REWARD_LOAD_RES*)packet.GetPacketData();
	res->wOpCode = GU_EVENT_REWARD_LOAD_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byCount = 0;


	CNtlPacket packetInfo(sizeof(sGU_EVENT_REWARD_LOAD_INFO));
	sGU_EVENT_REWARD_LOAD_INFO* resInfo = (sGU_EVENT_REWARD_LOAD_INFO*)packetInfo.GetPacketData();
	resInfo->wOpCode = GU_EVENT_REWARD_LOAD_INFO;
	resInfo->nCount = 0;


	for(std::map<TBLIDX, sEVENT_PC_NAME>::iterator it = m_mapEventReward.begin(); it != m_mapEventReward.end(); it++)
	{
		sEVENT_PC_NAME* pInfo = &it->second;

		NTL_SAFE_WCSCPY(res->asNameInfo[res->byCount].awchName, pInfo->awchName);
		res->asNameInfo[res->byCount].charId = pInfo->charId;

		resInfo->aInfo[res->byCount].charId = pInfo->charId;
		resInfo->aInfo[res->byCount].eventTblidx = it->first;

		res->byCount += 1;
		resInfo->nCount += 1;
	}

	g_pApp->Send(GetClientSessionID(), &packetInfo);

	g_pApp->Send(GetClientSessionID(), &packet);
}

//---------------------------------------------------------------------//
//		CHECK IF CAN TRANSFORM
//---------------------------------------------------------------------//
bool CPlayer::CanTransform(eSYSTEM_EFFECT_CODE effectCode, TBLIDX transformTblidx, bool bTransformBySkill)
{
	if (effectCode == ACTIVE_KAIOKEN && CannotMoveByBuff())
		return false;

	if (effectCode == ACTIVE_KAIOKEN && GetAspectStateId() == ASPECTSTATE_KAIOKEN)
		return true;

	if (GetAspectStateId() != ASPECTSTATE_INVALID)
		return false;
	else if (g_pTableContainer->GetStatusTransformTable()->FindData(transformTblidx) == NULL)
		return false;

	return true;
}

//---------------------------------------------------------------------//
//		TRANSFORMATION
//		return resultcode
//---------------------------------------------------------------------//
WORD CPlayer::Transform(eSYSTEM_EFFECT_CODE effectCode, TBLIDX transformTblidx, bool bTransformBySkill, BYTE byTransformSkillIndex/* = INVALID_BYTE*/)
{
	m_bTransformBySkill = bTransformBySkill;
	m_byTransformSkillIndex = byTransformSkillIndex;
	m_pTransformTbldat = (sSTATUS_TRANSFORM_TBLDAT*)g_pTableContainer->GetStatusTransformTable()->FindData(transformTblidx);

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
	sGU_UPDATE_CHAR_ASPECT_STATE * res = (sGU_UPDATE_CHAR_ASPECT_STATE*)packet.GetPacketData();
	res->handle = GetID();
	res->wOpCode = GU_UPDATE_CHAR_ASPECT_STATE;

	switch (effectCode)
	{
		case ACTIVE_SUPER_SAIYAN:
		{
			res->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_SUPER_SAIYAN;
			res->aspectState.sAspectStateDetail.sSuperSaiyan.bySourceGrade = m_byTransformGrade = 1;
		}
		break;
		case ACTIVE_PURE_MAJIN:
		{
			res->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_PURE_MAJIN;
			res->aspectState.sAspectStateDetail.sPureMajin.bySourceGrade = m_byTransformGrade = 1;
		}
		break;
		case ACTIVE_KAIOKEN:
		{
			res->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_KAIOKEN;
			res->aspectState.sAspectStateDetail.sKaioken.bySourceGrade = m_byTransformGrade += 1;
		}
		break;
		case ACTIVE_GREAT_NAMEK:
		{
			res->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_GREAT_NAMEK;
			res->aspectState.sAspectStateDetail.sGreatNamek.bySourceGrade = m_byTransformGrade = 1;
		}
		break;

		default: break;
	}

	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
	BroadcastToNeighbor(&packet);

	GetStateManager()->CopyAspectFrom(&res->aspectState);

	if (bTransformBySkill)
		GetCharAtt()->CalculateAll();

	return GAME_SUCCESS;
}

//---------------------------------------------------------------------//
//		CANCEL TRANSFORMATION
//---------------------------------------------------------------------//
void CPlayer::CancelTransformation()
{
	if (m_pTransformTbldat)
	{
		//if transform by skill
		if (m_bTransformBySkill)
		{
			GetSkillManager()->StartSkillCoolDown(m_byTransformSkillIndex);
			EndTransformation(true);
		}
		else //else transform by item
		{
			switch (GetAspectStateId())
			{
				case ASPECTSTATE_SUPER_SAIYAN: GetBuffManager()->EndBuff(ACTIVE_SUPER_SAIYAN); break;
				case ASPECTSTATE_PURE_MAJIN: GetBuffManager()->EndBuff(ACTIVE_PURE_MAJIN); break;
				case ASPECTSTATE_GREAT_NAMEK: GetBuffManager()->EndBuff(ACTIVE_GREAT_NAMEK); break;
				default: break;
			}
		}

		CNtlPacket packet(sizeof(sGU_TRANSFORM_CANCEL_RES));
		sGU_TRANSFORM_CANCEL_RES * res = (sGU_TRANSFORM_CANCEL_RES*)packet.GetPacketData();
		res->wOpCode = GU_TRANSFORM_CANCEL_RES;
		res->wResultCode = GAME_SUCCESS;
		g_pApp->Send(GetClientSessionID(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_TRANSFORM_CANCEL_RES));
		sGU_TRANSFORM_CANCEL_RES * res = (sGU_TRANSFORM_CANCEL_RES*)packet.GetPacketData();
		res->wOpCode = GU_TRANSFORM_CANCEL_RES;
		res->wResultCode = GAME_SKILL_NOT_TRANSFORMED;
		g_pApp->Send(GetClientSessionID(), &packet);
	}
}


//---------------------------------------------------------------------//
//		END TRANSFORMATION
//---------------------------------------------------------------------//
void CPlayer::EndTransformation(bool bCalculateAttribute/* = false*/)
{
	SendAspectStateInvalid(); //here we set transform id to invalid
	m_pTransformTbldat = NULL;
	m_byTransformGrade = 0;
	m_byTransformSkillIndex = INVALID_BYTE;

	if (bCalculateAttribute)
		GetCharAtt()->CalculateAll();
}


//--------------------------------------------------------------------------------------//
//	ENTER LAVA AREA
//--------------------------------------------------------------------------------------//
void	CPlayer::EnterLava()
{
	if (g_pEventMgr->HasEvent(this, EVENT_ON_LAVA))
		return;

	//Do damage once enter lava and then let event handle damage
	event_LavaDamage();

	if (!IsFainting())
		g_pEventMgr->AddEvent(this, &CPlayer::event_LavaDamage, EVENT_ON_LAVA, 2000, 0xFFFFFFFF, 0);
}
//--------------------------------------------------------------------------------------//
//	LEAVE LAVA AREA
//--------------------------------------------------------------------------------------//
void	CPlayer::LeaveLava()
{
	g_pEventMgr->RemoveEvents(this, EVENT_ON_LAVA);
}


//--------------------------------------------------------------------------------------//
//	ON TARGET CHANGED
//--------------------------------------------------------------------------------------//
void CPlayer::OnTargetChanged(HOBJECT hOldTarget)
{
	CCharacter::OnTargetChanged(hOldTarget);

	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget && pTarget->IsInitialized())
	{
		//show targets target
		CNtlPacket packet(sizeof(sGU_CHAR_TARGET_CHANGED));
		sGU_CHAR_TARGET_CHANGED * res = (sGU_CHAR_TARGET_CHANGED *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_TARGET_CHANGED;
		res->hTarget = pTarget->GetTargetHandle();
		res->hObject = GetTargetHandle();
		packet.SetPacketLen(sizeof(sGU_CHAR_TARGET_CHANGED));
		g_pApp->Send(GetClientSessionID(), &packet);

		if (GetPartyID() != INVALID_PARTYID && GetTargetHandle() != INVALID_HOBJECT)
		{
			if (pTarget->GetTargetListManager()->GetAggroCount() > 0)
			{
				//Show targets aggro
				CNtlPacket packet2(sizeof(sGU_AGGRO_LIST_NFY));
				sGU_AGGRO_LIST_NFY * res2 = (sGU_AGGRO_LIST_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_AGGRO_LIST_NFY;
				res2->hTarget = pTarget->GetID();
				pTarget->GetTargetListManager()->GetAggroList(&res2->dwTotalAggroPoint, &res2->byCount, res2->aAggroInfo);
				packet2.SetPacketLen(sizeof(sGU_AGGRO_LIST_NFY));
				g_pApp->Send(GetClientSessionID(), &packet2);
			}
		}

		//show buffs from target
		if(pTarget->GetID() != GetID()) //dont send my buff info to myself
			pTarget->GetBuffManager()->SendBuffInfo(this);
	}
}


bool CPlayer::AttackProgress(DWORD dwTickDiff, float fMultiple)
{
	if (CCharacterObject::AttackProgress(dwTickDiff, fMultiple))
	{
		CGameServer* app = (CGameServer*)g_pApp;

		BYTE byWeaponType = ITEM_TYPE_UNKNOWN;
		CItem* pWeapon = GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_HAND);
		if (pWeapon)
			byWeaponType = pWeapon->GetTbldat()->byItem_Type;
		
		sCHAR_DATA_INFO* animationInfo = GetAniTbldat()->GetChainAttack(GetTbldat()->byClass, ITEM_TYPE_GLOVE, m_byChainSequence);
		if (!animationInfo)
			return false;
		
	//	printf("m_dwNextAttackTime %u GetAttackSpeedRate() %u Animation-Duration %f \n", m_dwNextAttackTime, GetAttackSpeedRate(), animationInfo->fDurationTime);

		CCharacter* pVictim = g_pObjectManager->GetChar(GetAttackTarget());
		if (!pVictim || !pVictim->IsInitialized())
			return false;

		if (!IsAttackable(pVictim))
			return false;
		else if (ConsiderAttackRange() == false)
			return false;
		
		UpdateBattleCombatMode(true); //Start/Reset combat event

		AttackAction(pVictim, true);

		float fAnimationTime = animationInfo->fDurationTime * 1000.f; //chain attack duration time (GetChainAttackDurationTime())
		float fAttackAnimationSpeed = (GetAttackSpeedRate() == 0) ? 1.0f : 1000.0f / (float)GetAttackSpeedRate();
		float fRequiredAttackAnimationTime = fAnimationTime / fAttackAnimationSpeed;

		m_dwNextAttackTime = app->GetCurTickCount() + (DWORD)fRequiredAttackAnimationTime;

		if (Dbo_CheckProbability(30 - (GetLevel() - pVictim->GetLevel())))
			UpdateCurRP(GetCharAtt()->GetRPRegen(), true, false);

		return true;
	}

	return false;
}


bool CPlayer::OnAttackAction(CCharacter* pAttacker, int nDmg, BYTE byAttackResult, eFAINT_REASON eReason/* = FAINT_REASON_HIT*/)
{
	if (byAttackResult != BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE)
	{
		if (Dbo_CheckProbability(30 - (GetLevel() - pAttacker->GetLevel())))
			UpdateCurRP(GetCharAtt()->GetRPRegen(), true, false);
	}

	UpdateBattleCombatMode(true); //Start/Reset combat event

	if (GetAspectStateId() == ASPECTSTATE_VEHICLE && m_vehicle.bGetOffOnAttack == true)
		EndVehicle(GAME_VEHICLE_END_BY_HIT);


	if (GetCharStateID() == CHARSTATE_OPERATING)//Cancel operating
		CancelOperating();
	else if (GetCharStateID() == CHARSTATE_SITTING)//if sitting, stand up
		SendCharStateStanding();
	else if (GetCharStateID() == CHARSTATE_CHARGING)//Cancel rp charging
	{
		CancelCharging();
		UpdateCurRP(100, false, false);
	}

	//Cancel flight
	if (GetAirState() == AIR_STATE_ON)
		SendCharStateFalling(NTL_MOVE_NONE);

	return CCharacter::OnAttackAction(pAttacker, nDmg, byAttackResult, eReason);
}

bool CPlayer::OnSkillAction(CCharacter * pAttacker, int nDmg, DWORD dwAggroPoint, BYTE byAttackResult, bool bWakeUpTarget)
{
	UpdateBattleCombatMode(true); //Start/Reset combat event

	if (GetAspectStateId() == ASPECTSTATE_VEHICLE)
		EndVehicle(GAME_VEHICLE_END_BY_HIT);

	if (GetCharStateID() == CHARSTATE_OPERATING)//Cancel operating
		CancelOperating();
	else if (GetCharStateID() == CHARSTATE_SITTING)//if sitting, stand up
		SendCharStateStanding();
	else if (GetCharStateID() == CHARSTATE_CHARGING) //Cancel rp charging
	{
		CancelCharging();
		UpdateCurRP(100, false, false);
	}

	//Cancel flight
	if (GetAirState() == AIR_STATE_ON)
		SendCharStateFalling(NTL_MOVE_NONE);

	return CCharacter::OnSkillAction(pAttacker, nDmg, dwAggroPoint, byAttackResult, bWakeUpTarget);
}



//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CPlayer::Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (m_pTransformTbldat) //if transformed then cancel
		CancelTransformation();

	if (GetAspectStateId() == ASPECTSTATE_VEHICLE) //cancel vehicle when faint
		EndVehicle(GAME_VEHICLE_END_BY_HIT);

	if (GetPcIsFreeBattle() == false)
	{
		if (CCharacterObject::Faint(pkKiller, byReason))
		{
			CCharacter::Faint(pkKiller, byReason);

			UpdateBattleCombatMode(false);

			if (GetDragonballScrambleBallFlag() > 0)
				g_pDragonballScramble->SpawnBall(this, true);

			bool bApply = true;

			if (GetCurrentPetId() != INVALID_HOBJECT)
			{
				if (CSummonPet* pPet = g_pObjectManager->GetSummonPet(GetCurrentPetId()))
					pPet->Despawn(true);
			}

			if (GetCurWorld())
			{
				if(GetCurWorld()->GetRuleType() == GAMERULE_RANKBATTLE) //check if player in rank battle)
				{
					bApply = false;
					g_pRankbattleManager->UpdatePlayerState(GetRankBattleRoomTblidx(), GetRankBattleRoomId(), this, RANKBATTLE_MEMBER_STATE_FAINT);
				}
				else if (GetCurWorld()->GetRuleType() == GAMERULE_DOJO) //check if player in rank battle)
				{
					bApply = false;

					SetCurRP(0); //just set value is enough because client reset the rp by itself (DO NOT WHEN PLAYER DIE IN RANK BATTLE)
					SetRPBall(0);//just set value is enough because client reset the rp ball by itself (DO NOT WHEN PLAYER DIE IN RANK BATTLE)
				}
				else if (GetCurWorld()->GetRuleType() == GAMERULE_MINORMATCH || GetCurWorld()->GetRuleType() == GAMERULE_MAJORMATCH || GetCurWorld()->GetRuleType() == GAMERULE_FINALMATCH)//check if die in budokaiif 
				{
					if (GetBudokaiPcState() != MATCH_MEMBER_STATE_FAINT && app->IsDojoChannel())
					{
						SetBudokaiPcState(MATCH_MEMBER_STATE_FAINT);

						//update score etc
						if (g_pBudokaiManager->GetMatchDepth() == INVALID_BUDOKAI_MATCH_DEPTH)
						{
							if(pkKiller)
								g_pBudokaiManager->MinorMatchUpdateScore(GetMatchIndex(), ((CPlayer*)pkKiller)->GetBudokaiTeamType(), pkKiller->GetID(), GetID());
							else
								g_pBudokaiManager->MinorMatchUpdateScore(GetMatchIndex(), INVALID_TEAMTYPE, INVALID_HOBJECT, GetID());
						}
						else if (g_pBudokaiManager->GetMatchDepth() >= BUDOKAI_MATCH_DEPTH_8)
							g_pBudokaiManager->MajorMatchUpdateScore(GetMatchIndex(), GetBudokaiTeamType(), GetID(), GetJoinID());
						else if (g_pBudokaiManager->GetMatchDepth() <= BUDOKAI_MATCH_DEPTH_4)
							g_pBudokaiManager->FinalMatchUpdateScore(GetMatchIndex(), GetBudokaiTeamType(), GetID(), GetJoinID());
						else
							ERR_LOG(LOG_SYSTEM, "BudokaiManager: MatchDepth %u not found", g_pBudokaiManager->GetMatchDepth());

						bApply = false;
					}

					SetCurRP(0); //just set value is enough because client reset the rp by itself (DO NOT WHEN PLAYER DIE IN RANK BATTLE)
					SetRPBall(0);//just set value is enough because client reset the rp ball by itself (DO NOT WHEN PLAYER DIE IN RANK BATTLE)
				}
				else
				{
					SetCurRP(0); //just set value is enough because client reset the rp by itself (DO NOT WHEN PLAYER DIE IN RANK BATTLE)
					SetRPBall(0);//just set value is enough because client reset the rp ball by itself (DO NOT WHEN PLAYER DIE IN RANK BATTLE)
				}
			}

			if (IsInBattleArena(GetWorldTblidx(), GetCurLoc())) //check if player is in free pvp arena
			{
				bApply = false;
			}

			if (bApply)
			{
				if(GetCurWorld() && GetCurWorld()->GetTbldat()->bDynamic == false)
					AddDeathQuickTeleport();

				if(IsGameMaster() == false)		//do not decrease durability when player is a gm..
					DecreaseEquipmentDurability();

				GetQuests()->PlayerDied(); //inform quests that player died. Some quests might fail.
			}

			GetCharAtt()->CalculateAll();
			return true;
		}
	}
	else
	{
		SetCurLP(1); //set current LP to 1 because client-side the lp dont go below 1 if battle ends
		g_pFreeBattleManager->EndFreeBattle(GetFreeBattleID(), GetFreeBattleTarget());

		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//		return true on death
//--------------------------------------------------------------------------------------//
bool CPlayer::OnBuffDamage(HOBJECT hCaster, float fValue)
{
	if (!CCharacter::OnBuffDamage(hCaster, fValue))
	{
		if(hCaster != this->GetID()) //Start/Reset combat event
			UpdateBattleCombatMode(true);

		return false;
	}

	return true;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CPlayer::ConsiderAttackRange()
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetAttackTarget());
	if (pTarget)
	{
		if (pTarget->GetAirState() == AIR_STATE_ON)
		{
			if (IsInRange3(pTarget, GetAttackRange(pTarget)))
				return true;
		}
		else
		{
			if (IsInRange(pTarget, GetAttackRange(pTarget)))
				return true;
		}
	}

	return false;
}

float CPlayer::GetAttackRange(CCharacter * pTarget)
{
	float fAttackRange = CCharacterObject::GetAttackRange();

	if (GetStateManager()->GetAspectStateID() == ASPECTSTATE_GREAT_NAMEK)
	{
		fAttackRange = DBO_GREAT_NAMEK_ATTACK_RANGE;
	}

	fAttackRange += CCharacter::GetAttackRange(pTarget);

	fAttackRange += 0.5f; // add some padding

	return fAttackRange;
}


void CPlayer::StartTeleport(CNtlVector& destLoc, CNtlVector& destDir, WORLDID worldid, BYTE byTeleType, TBLIDX directPlayIdx, bool bSyncDirectPlay, bool bTeleportAnotherServer)
{
	if (GetAspectStateId() == ASPECTSTATE_VEHICLE)
		EndVehicle(GAME_VEHICLE_END_BY_TELEPORT);

	if (GetAirState() == AIR_STATE_ON)
		SetAirState(AIR_STATE_OFF);

	if(GetPcIsFreeBattle())
		g_pFreeBattleManager->EndFreeBattle(GetFreeBattleID(), GetFreeBattleTarget());

	if (GetCurrentPetId() != INVALID_HOBJECT)
	{
		if (CSummonPet* pPet = g_pObjectManager->GetSummonPet(GetCurrentPetId()))
			pPet->Despawn(true);
	}

	m_uiTeleportDirectPlay = directPlayIdx;
	m_bTeleportSyncDirectPlay = bSyncDirectPlay;
	m_bTeleportAnotherServer = bTeleportAnotherServer;

	CCharacter::StartTeleport(destLoc, destDir, worldid, byTeleType);
}

void CPlayer::TeleportAnotherServer(CNtlVector & destLoc, CNtlVector & destDir, TBLIDX worldIdx, WORLDID worldid, BYTE byTeleType, BYTE byChannel, WORD wTime, bool bProposal/* = true*/)
{
	m_bTeleportAnotherServer = true;

	if (GetAspectStateId() == ASPECTSTATE_VEHICLE)
		EndVehicle(GAME_VEHICLE_END_BY_TELEPORT);

	if (GetAirState() == AIR_STATE_ON)
		SetAirState(AIR_STATE_OFF);

	if (GetPcIsFreeBattle())
		g_pFreeBattleManager->EndFreeBattle(GetFreeBattleID(), GetFreeBattleTarget());

	if (GetCurrentPetId() != INVALID_HOBJECT)
	{
		if (CSummonPet* pPet = g_pObjectManager->GetSummonPet(GetCurrentPetId()))
			pPet->Despawn(true);
	}

	CancelDice();

	m_hTeleportProposalRequestor = INVALID_HOBJECT;

	m_sServerTeleportInfo.byTeleportType = byTeleType;
	destDir.CopyTo(m_sServerTeleportInfo.vDir);
	destLoc.CopyTo(m_sServerTeleportInfo.vLoc);
	m_sServerTeleportInfo.worldId = worldid;
	m_sServerTeleportInfo.worldTblidx = worldIdx;
	m_sServerTeleportInfo.serverChannelId = byChannel;
	m_sServerTeleportInfo.serverIndex = 0;

	m_vTeleportProposalLoc = destLoc;
	m_vTeleportProposalDir = destDir;
	m_teleportProposalWorldID = worldid;
	m_byTeleportProposalType = byTeleType;

	if (bProposal)
	{
		if (g_pEventMgr->HasEvent(this, EVENT_TELEPORT_PROPOSAL)) // check if we already have a proposal
			g_pEventMgr->RemoveEvents(this, EVENT_TELEPORT_PROPOSAL); // remove current proposal

		g_pEventMgr->AddEvent(this, &CPlayer::event_TeleportProposal, EVENT_TELEPORT_PROPOSAL, wTime * 1000, 1, 0);

		CNtlPacket packet(sizeof(sGU_TELEPORT_PROPOSAL_NFY));
		sGU_TELEPORT_PROPOSAL_NFY * res = (sGU_TELEPORT_PROPOSAL_NFY *)packet.GetPacketData();
		res->wOpCode = GU_TELEPORT_PROPOSAL_NFY;
		res->worldTblidx = worldIdx;
		res->byTeleportType = byTeleType;
		res->byInfoIndex = 0;
		res->wWaitTime = wTime;
		res->dwReserve = INVALID_DWORD;
		res->byBudokaiMatchDepth = INVALID_BUDOKAI_MATCH_DEPTH;
		packet.SetPacketLen(sizeof(sGU_TELEPORT_PROPOSAL_NFY));
		SendPacket(&packet);
	}
	else
	{
		CCharacter::StartTeleport(destLoc, destDir, worldid, byTeleType);
	}
}

void CPlayer::CancelTeleportProposal(BYTE byTeleportIndex)
{
	if (g_pEventMgr->HasEvent(this, EVENT_TELEPORT_PROPOSAL)) // check if we already have a proposal
		g_pEventMgr->RemoveEvents(this, EVENT_TELEPORT_PROPOSAL); // remove current proposal

	CNtlPacket packet(sizeof(sGU_TELEPORT_CONFIRM_RES));
	sGU_TELEPORT_CONFIRM_RES* res = (sGU_TELEPORT_CONFIRM_RES*)packet.GetPacketData();
	res->wOpCode = GU_TELEPORT_CONFIRM_RES;
	res->wResultCode = GAME_SUCCESS;
	res->bClearInterface = true;
	res->bTeleport = false;
	res->byTeleportIndex = byTeleportIndex;
	packet.SetPacketLen(sizeof(sGU_TELEPORT_CONFIRM_RES));
	SendPacket(&packet);

	//reset teleport data
	ResetDirectPlay();
	//reset teleport shit
	event_TeleportProposal();
}


//--------------------------------------------------------------------------------------//
//	CHECK IF TARGET ATTACK ABLE
//--------------------------------------------------------------------------------------//
bool CPlayer::IsAttackable(CCharacterObject* pTarget)
{
	if (CCharacterObject::IsAttackable(pTarget))
	{
		if (pTarget->IsPC())
		{
			CPlayer* pPlayerTargt = (CPlayer*)pTarget;

			if (GetCurWorld() == NULL || pPlayerTargt->GetCurWorld() == NULL)
				return false;

			if (GetDragonballScramble() && pPlayerTargt->GetDragonballScramble())
			{
				if (GetPartyID() != INVALID_PARTYID && GetPartyID() == pPlayerTargt->GetPartyID())
					return false;

				if (GetNaviEngine()->IsBasicAttributeSet(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) == false
					&& GetNaviEngine()->IsBasicAttributeSet(pPlayerTargt->GetCurWorld()->GetNaviInstanceHandle(), pPlayerTargt->GetCurLoc().x, pPlayerTargt->GetCurLoc().z, DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE) == false )
					return true;
			}

			if (GetFreeBattleID() != INVALID_DWORD && GetFreeBattleID() == pPlayerTargt->GetFreeBattleID())
				return true;

			if (IsPvpZone() == true && IsInBattleArena(pPlayerTargt->GetWorldTblidx(), pPlayerTargt->GetCurLoc()) == true) //dont allow players attack players who are not in battle arena
				return true;

			if (m_sRankBattleData.eState == RANKBATTLE_MEMBER_STATE_ATTACKABLE && pPlayerTargt->GetRankBattleData()->eState == RANKBATTLE_MEMBER_STATE_ATTACKABLE)
			{
				if(m_sRankBattleData.eTeamType != pPlayerTargt->GetRankBattleData()->eTeamType)
					return true;
			}

			else if (GetCurWorld()->GetRuleType() == GAMERULE_DOJO && GetGuildID() != pPlayerTargt->GetGuildID())
				return true;

			else if (GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL && pPlayerTargt->GetBudokaiPcState() == MATCH_MEMBER_STATE_NORMAL && GetBudokaiTeamType() != pPlayerTargt->GetBudokaiTeamType())
				return true;

			return false;
		}

		return true;
	}

	return false;
}

void CPlayer::SetNextAttackTime()
{
	CGameServer* app = (CGameServer*)g_pApp;

	sCHAR_DATA_INFO* animationInfo = GetAniTbldat()->GetChainAttack(GetTbldat()->byClass, ITEM_TYPE_GLOVE, m_byChainSequence);
	if (!animationInfo)
		return;

	float fAnimationTime = animationInfo->fDurationTime * 1000.f; //chain attack duration time (GetChainAttackDurationTime())
	float fAttackAnimationSpeed = (GetAttackSpeedRate() == 0) ? 1.0f : 1000.0f / (float)GetAttackSpeedRate();
	float fRequiredAttackAnimationTime = fAnimationTime / fAttackAnimationSpeed;

	m_dwNextAttackTime = app->GetCurTickCount() + (DWORD)fRequiredAttackAnimationTime;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CPlayer::CrashGuard()
{
	SendCharStateStanding();

	CNtlPacket packet(sizeof(sGU_CHAR_GUARD_CRASHED_NFY));
	sGU_CHAR_GUARD_CRASHED_NFY * res = (sGU_CHAR_GUARD_CRASHED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_GUARD_CRASHED_NFY;
	res->hSubject = this->GetID();
	packet.SetPacketLen(sizeof(sGU_CHAR_GUARD_CRASHED_NFY));
	Broadcast(&packet);
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CPlayer::EndGuard()
{
	CSkill* pSkill = GetSkillManager()->FindSkillWithSystemEffectCode(PASSIVE_BLOCK_MODE);
	if (!pSkill)
	{
		ERR_LOG(LOG_USER, "ERROR CAN NOT END GUARD BECAUSE PASSIVE NOT EXIST. PLAYER %u", GetCharID());
		return;
	}

	float fCoolDown = (float)pSkill->GetOriginalTableData()->dwCoolTimeInMilliSecs;

	fCoolDown += fCoolDown * GetCharAtt()->GetCoolTimeChangePercent() / 100.0f;

	if (fCoolDown < 1.0f)
		fCoolDown = 0.0f;

	CNtlPacket packet(sizeof(sGU_CHAR_BLOCK_MODE_COOL_TIME_NFY));
	sGU_CHAR_BLOCK_MODE_COOL_TIME_NFY * res = (sGU_CHAR_BLOCK_MODE_COOL_TIME_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_BLOCK_MODE_COOL_TIME_NFY;
	res->dwCoolTimeRemaining = (DWORD)fCoolDown;
	packet.SetPacketLen(sizeof(sGU_CHAR_BLOCK_MODE_COOL_TIME_NFY));
	SendPacket(&packet);

	pSkill->SetCoolTimeRemaining(res->dwCoolTimeRemaining);
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CPlayer::StartGuard()
{
	CSkill* pSkill = GetSkillManager()->FindSkillWithSystemEffectCode(PASSIVE_BLOCK_MODE);
	if (!pSkill)
	{
		ERR_LOG(LOG_USER, "ERROR CAN NOT START GUARD BECAUSE PASSIVE NOT EXIST. PLAYER %u", GetCharID());
		return false;
	}

	if (pSkill->GetCoolTimeRemaining() > 0)
		return false;

	m_dwGuardDuration = pSkill->GetOriginalTableData()->dwKeepTimeInMilliSecs;

	float fRequireEP = (float)pSkill->GetOriginalTableData()->wRequire_EP;
	{
		if (GetCharAtt()->GetRequiredEpChangePercent() != 0.0f)
			fRequireEP += fRequireEP * GetCharAtt()->GetRequiredEpChangePercent() / 100.0f;

		if (fRequireEP > 1.0f)
			UpdateCurEP((WORD)fRequireEP, false, false);

		UpdateCurEP(pSkill->GetOriginalTableData()->wRequire_EP, false, false);
	}

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->handle = this->GetID();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_GUARD;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));
	Broadcast(&packet);

	GetStateManager()->CopyFrom(&res->sCharState);

	return true;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CPlayer::DecreaseGuardTime(DWORD dwTickDiff)
{
	m_dwGuardDuration = UnsignedSafeDecrease<DWORD>(m_dwGuardDuration, dwTickDiff);

	if (m_dwGuardDuration == 0)
	{
		SendCharStateStanding();
	}
}

bool CPlayer::IsGuardSuccess(BYTE byGuardType, bool& rbCanCounterAttack)
{
	if (byGuardType != DBO_GUARD_TYPE_DEFENCE)
	{
		CBuff* pPerfectGuard = GetBuffManager()->FindBuff(ACTIVE_PERFECT_GUARD);
		if (pPerfectGuard && pPerfectGuard->IsActive())
		{
			if (pPerfectGuard->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue > 0)
			{
				pPerfectGuard->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue -= 1;

				if (pPerfectGuard->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue == 0)
					GetBuffManager()->RemoveBuff(pPerfectGuard->GetBuffIndex(), pPerfectGuard->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_ETC); //ETC because we do not need to recalculate the stats

				return true;
			}
		}
	}

	int nGuardRate = (int)GetCharAtt()->GetGuardRate() / 70;
	
	switch (byGuardType)
	{
		case DBO_GUARD_TYPE_DEFENCE: break;
		case DBO_GUARD_TYPE_DAMAGE_SKILL: nGuardRate += (int)GetCharAtt()->GetSkillDamageBlockModeSuccessRate(); break;
		case DBO_GUARD_TYPE_CURSE_SKILL: nGuardRate += (int)GetCharAtt()->GetCurseBlockModeSuccessRate(); rbCanCounterAttack = false; break;
		case DBO_GUARD_TYPE_KNOCKDOWN: nGuardRate += (int)GetCharAtt()->GetKnockDownBlockModeSuccessRate(); break;
		case DBO_GUARD_TYPE_HTB: nGuardRate += (int)GetCharAtt()->GetHtbKnockDownBlockModeSuccessRate(); break;

		default: break;
	}
	//printf("nGuardRate %u, GetLastGuardRate %u, GetSkillDamageBlockModeSuccessRate %f\n", nGuardRate, GetCharAtt()->GetLastGuardRate(), GetCharAtt()->GetSkillDamageBlockModeSuccessRate());
	return Dbo_CheckProbability(nGuardRate);
}


//--------------------------------------------------------------------------------------//
//	Check if can use DASH PASSIVE SKILL
//--------------------------------------------------------------------------------------//
void CPlayer::GetDashPassiveRequiredEP(CSkill* pSkill, DWORD dwCoolTimeInMilliSecs, WORD & rwRequire_EP)
{
	if (pSkill->GetCoolTimeRemaining() > 0) // check if using dash while in cool-time (increase mana usage)
		m_wDashPassiveRequiredEP = UnsignedSafeIncrease<WORD>(m_wDashPassiveRequiredEP, m_wDashPassiveRequiredEP);
	else
		m_wDashPassiveRequiredEP = rwRequire_EP;

	rwRequire_EP = m_wDashPassiveRequiredEP;

	if (Dbo_CheckProbability(50))
	{
		CItemPet* pMascot = GetCurrentMascot();
		if (pMascot)
		{
			TBLIDX skillIdx = pMascot->GetDashEpReduceSkill();
			if (skillIdx != INVALID_TBLIDX)
			{
				sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(skillIdx);
				if (pSkillTbldat)
				{
					rwRequire_EP -= WORD((float)rwRequire_EP * (float)pSkillTbldat->aSkill_Effect_Value[0] / 100.f);
				}
			}
		}
	}

	if (GetCurEP() < rwRequire_EP)
		rwRequire_EP = GetCurEP();

	float fCoolDown = (float)dwCoolTimeInMilliSecs;

	fCoolDown += fCoolDown * GetCharAtt()->GetCoolTimeChangePercent() / 100.0f;

	if (fCoolDown < 1.0f)
		fCoolDown = 0.0f;

	pSkill->SetCoolTimeRemaining((DWORD)fCoolDown);
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
TBLIDX CPlayer::GetMissingDragonball()
{
	return GetPlayerItemContainer()->GetMissingDragonball();
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CPlayer::Recover(DWORD dwTickDiff)
{
	if (GetCurWorld() == NULL)
		return false;
	if (GetCurWorld()->GetRuleType() == GAMERULE_CCBATTLEDUNGEON)
		return false;

	if (CCharacter::Recover(dwTickDiff) == false)
		return false;

	BYTE byState = GetCharStateID();

	// RECOVER LP / EP
	{
		int NewLP = 0;
		WORD NewEP = 0;

		if (GetCombatMode())
		{
			NewLP += GetCharAtt()->GetLPBattleRegen();
			NewEP += GetCharAtt()->GetEPBattleRegen();
		}
		else if (byState == CHARSTATE_SITTING)
		{
			NewLP += GetCharAtt()->GetLPSitdownRegen();
			NewEP += GetCharAtt()->GetEPSitdownRegen();
		}
		else
		{
			NewLP += GetCharAtt()->GetLPRegen();
			NewEP += GetCharAtt()->GetEPRegen();
		}
		//	printf("recover %u lp %u ep \n", NewLP, NewEP);

		AcquireLpEpEventID();
		UpdateCurLpEp(NewLP, NewEP, true, true);
	}

	// RECOVER/DECREASE AP
	int NewAP = 0;
	bool bIncrease = true;

	if (byState == CHARSTATE_AIR_DASH_ACCEL)
	{
		NewAP = GetCharAtt()->GetAPDegen();
		bIncrease = false;
	}
	else if (byState == CHARSTATE_SITTING)
		NewAP = GetCharAtt()->GetAPSitdownRegen();
	else
		NewAP = GetCharAtt()->GetAPRegen();

	//	printf("GetCurAP() %u NewAP %u bIncrease %i\n", GetCurAP(), NewAP, bIncrease);

	if (UpdateCurAP(NewAP, bIncrease))
	{
		if (GetCurAP() <= 0 && GetAirState() == AIR_STATE_ON)
			SendCharStateFalling(NTL_MOVE_NONE);
	}

	return true;
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CPlayer::LpEmergency(DWORD dwTickDiff)
{
	if (IsFainting())
		return;

	m_dwLpEmergencyTick = UnsignedSafeDecrease<DWORD>(m_dwLpEmergencyTick, dwTickDiff);

	if (m_dwLpEmergencyTick > 0)
		return;

	float lpPercent = ((float)GetCurLP() / (float)GetMaxLP()) * 100.0f;

	if (IsLpEmergancy() == false && lpPercent <= CFormulaTable::m_afRate[6100][1])
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP_STATUS_NFY));
		sGU_UPDATE_CHAR_LP_STATUS_NFY* res = (sGU_UPDATE_CHAR_LP_STATUS_NFY*)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_LP_STATUS_NFY;
		res->handle = GetID();
		res->bEmergency = true;
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_LP_STATUS_NFY));
		Broadcast(&packet);

		SetLpEmergency(true);
	}
	else if (IsLpEmergancy() == true && lpPercent > CFormulaTable::m_afRate[6100][1])
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP_STATUS_NFY));
		sGU_UPDATE_CHAR_LP_STATUS_NFY* res = (sGU_UPDATE_CHAR_LP_STATUS_NFY*)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_LP_STATUS_NFY;
		res->handle = GetID();
		res->bEmergency = false;
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_LP_STATUS_NFY));
		Broadcast(&packet);

		SetLpEmergency(false);
	}

	m_dwLpEmergencyTick = 2000; //reset time
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CPlayer::ChatServerCoordinateSync(DWORD dwTickDiff)
{
	m_dwChatServerLocationSyncTick = UnsignedSafeDecrease<DWORD>(m_dwChatServerLocationSyncTick, dwTickDiff);

	if (m_dwChatServerLocationSyncTick > 0)
		return;

	if (GetCurWorld() && IsLocInWorldBoundary(GetCurLoc(), GetCurWorld()))
	{
		m_dwChatServerLocationSyncTick = 5000;
		CGameServer* app = (CGameServer*)g_pApp;

		//CNtlPacket packet(sizeof(sGU_CHAR_COORDINATE_EACH_TICK_NFY));
		//sGU_CHAR_COORDINATE_EACH_TICK_NFY * res = (sGU_CHAR_COORDINATE_EACH_TICK_NFY *)packet.GetPacketData();
		//res->wOpCode = GU_CHAR_COORDINATE_EACH_TICK_NFY;
		//res->hSubject = GetID();
		//NtlLocationCompress(&res->vCurLoc, GetCurLoc()->x, GetCurLoc()->y, GetCurLoc()->z);
		//NtlDirectionCompress(&res->vCurDir, GetCurDir()->x, GetCurDir()->y, GetCurDir()->z);
		//packet.SetPacketLen( sizeof(sGU_CHAR_COORDINATE_EACH_TICK_NFY) );
		//app->Send(GetClientSessionID(), &packet);

		//set map name tblidx
		SetMapNameTblidx(GetNaviEngine()->GetTextAllIndex(GetCurWorld()->GetNaviInstanceHandle(), GetCurLoc().x, GetCurLoc().z));

		//send coordinate packet to chat server
		if (app->GetChatServerSession())
		{
			CNtlPacket chatPacket(sizeof(sGT_UPDATE_PLAYER_POSITION));
			sGT_UPDATE_PLAYER_POSITION * chatRes = (sGT_UPDATE_PLAYER_POSITION *)chatPacket.GetPacketData();
			chatRes->wOpCode = GT_UPDATE_PLAYER_POSITION;
			chatRes->accountId = GetAccountID();
			chatRes->mapNameTblidx = GetMapNameTblidx();
			GetCurLoc().CopyTo(chatRes->vCurrentPosition);
			chatRes->worldId = GetWorldID();
			app->SendTo(app->GetChatServerSession(), &chatPacket); //Send to chat server
		}

		if (GetParty())
			GetParty()->UpdateMemberLocation(this);
	}
}


void CPlayer::Revival(CNtlVector & rVecLoc, WORLDID worldID, eREVIVAL_TYPE eRevivalType, eTELEPORT_TYPE eTeleportType /* = TELEPORT_TYPE_DEFAULT */)
{
	SetIsReviving(true);

	if (GetStateManager()->IsCharCondition(CHARCOND_NULLIFIED_DAMAGE) == false)
		GetStateManager()->RemoveConditionState(CHARCOND_NULLIFIED_DAMAGE, NULL, false); // if player somehow die while skill usage.. Just to make 100% sure, no immortal bug going to happen

	switch (eRevivalType)
	{
		case REVIVAL_TYPE_CURRENT_POSITION: //by TLQ or ITEM
		{
			//If not revived inside TLQ then dont heal. Because you will be healed by the item
			SetIsReviving(false);
			SendCharStateSpawning(eTeleportType);
		}
		break;
		case REVIVAL_TYPE_BIND_POINT:
		{
			StartTeleport(rVecLoc, GetCurDir(), worldID, eTeleportType);
		}
		break;
		case REVIVAL_TYPE_RESCUED:
		{
			if (GetCurWorld())
			{
				if (GetCurWorld()->GetRuleType() == GAMERULE_RANKBATTLE) //check if player in rank battle
				{
					eTeleportType = TELEPORT_TYPE_RANKBATTLE;
				}
				else if (GetCurWorld()->GetRuleType() == GAMERULE_MAJORMATCH) //check if player in budokai
				{
					eTeleportType = TELEPORT_TYPE_MAJORMATCH;
					SetBudokaiPcState(MATCH_MEMBER_STATE_RESCUE);
				}
				else if (GetCurWorld()->GetRuleType() == GAMERULE_FINALMATCH) //check if player in budokai
				{
					eTeleportType = TELEPORT_TYPE_FINALMATCH;
					SetBudokaiPcState(MATCH_MEMBER_STATE_RESCUE);
				}
			}
			
			StartTeleport(rVecLoc, GetCurDir(), worldID, eTeleportType);
		}
		break;
		case REVIVAL_TYPE_SPECIFIED_POSITION:
		{
			StartTeleport(rVecLoc, GetCurDir(), worldID, eTeleportType);
		}
		break;

		default: break;
	}
}

bool CPlayer::DisconnectForSpeedHack(DWORD dwTickCount)
{
	if (m_dwLastSpeedHackReport > dwTickCount) // check if last speedhack report was within 3 seconds
	{
		return true;
	}

	m_dwLastSpeedHackReport = dwTickCount + 30000;

	return false;
}


CStateManager * CPlayer::CreateStateManager(eAIR_STATE eAirState)
{
	CStateManager* pManager = new CStateManager;

	if (!pManager->Create(this, eAirState))
	{
		ERR_LOG(LOG_SYSTEM, "StateManager Create Fail");
		return NULL;
	}

	return pManager;
}

//--------------------------------------------------------------------------------------//
//	Check if can begin air accel move
//--------------------------------------------------------------------------------------//
void CPlayer::CheckAirAccel()
{
	if (GetMoveFlag() == NTL_MOVE_FLAG_FLY_DASH && GetCharStateID() == CHARSTATE_AIR_DASH_ACCEL)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		DWORD dwTickCount = app->GetCurTickCount();
		if (m_dwLastAirAccelStartCheck + 3000 <= dwTickCount) //start accel after X seconds of dash
		{
			m_dwLastAirAccelStartCheck = dwTickCount + 10000;

			if (GetMoveDirection() == NTL_MOVE_F || GetMoveDirection() == NTL_MOVE_F_TURN_L || GetMoveDirection() == NTL_MOVE_F_TURN_R)
			{
				m_bCanAccel = true;

				SendCharStateAirDashAccel(GetMoveDirection(), NTL_MOVE_FLAG_FLY_ACCEL);
			}
		}
	}
}