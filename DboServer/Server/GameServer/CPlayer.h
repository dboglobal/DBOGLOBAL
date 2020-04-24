#ifndef __CPLAYER__
#define __CPLAYER__

#include "Quest.h"
#include "NtlAdmin.h"
#include "Party.h"
#include "char.h"
#include "NtlMail.h"
#include "SkillManagerPc.h"
#include "BuffManagerPc.h"
#include "CharacterAttPC.h"
#include "HtbSkillManager.h"
#include "PlayerItemContainer.h"
#include "NtlEventDef.h"



class CPrivateShop;
class CTrade;
class CClientSession;
class CItem;
class CTimeLeapDungeon;
class CUltimateDungeon;
class CTimeQuest;
class QueryBuffer;
class CItemPet;
struct sSTATUS_TRANSFORM_TBLDAT;
struct sSERVER_INFO;
struct sDBO_SERVER_INFO;


struct sRANK_BATTLE_DATA
{
	eRANKBATTLE_TEAM_TYPE		eTeamType;
	eRANKBATTLE_MEMBER_STATE	eState;
	CNtlVector					vResetLoc;

	void				Init()
	{
		eState = RANKBATTLE_MEMBER_STATE_NONE;
		eTeamType = RANKBATTLE_TEAM_NONE;
		vResetLoc.Reset();
	}

};

class CPlayer : public CCharacter
{

public:
	CPlayer();
	virtual ~CPlayer();

	bool					Create(sPC_TBLDAT* pTbldat, sPC_PROFILE *pProfile, sCHARSTATE *pCharState);

	void					SetLoggedOut() { m_bIsLoggedOut = true; }
	bool					IsLoggedOut() { return m_bIsLoggedOut; }


	void					Send_GtUserEnterGame(bool bForSyncCommunity);
	void					Send_GtUserLeaveGame(eCHARLEAVING_TYPE eCharLeavingType, bool bIsKickOut, BYTE destGameServerChannelID, BYTE destGameServerIndex);

	void					Bann(std::string strReason, BYTE byDuration, ACCOUNTID gmAccountID);

private:

	DWORD					dwNextSaveTime;
	bool					m_bSkipSave;

	DWORD					dwLoggedTime; //tickcount when logged in

	bool					m_bIsLoggedOut;

	bool					m_bGtUserLeaveGameSent;

	bool					m_bCanUnstack;

	sSERVER_INFO*			m_pDestServerInfo; //when switch channel
	char					m_achAuthKey[NTL_MAX_SIZE_AUTH_KEY];

private:
	
	CQuest					*pQuest;

	CHtbSkillManager*		m_pHtbSkillManager;

private:

	sPLAYER_DATA			player_data;

	ACCOUNTID				uiAccountID;
	SERVERCHANNELID			m_byPrevChannelID;

	BYTE					m_byGameMasterLevel; // eADMIN_LEVEL
	bool					m_bIsGameMaster;
	bool					m_bIsTutorial;
	bool					m_bHasTutorialFlag;

	DWORD					m_dwLastCashShopRefresh;
	DWORD					m_dwItemShopCash;		//coins used to buy hls items
	DWORD					m_dwEventMachineCoin; //coins used for hls event machine
	DWORD					m_dwWaguMachineCoin; //coins used for HLS slot machine

	HOBJECT					m_hAttackTarget;

	DWORD					m_dwGuardDuration;

	WORD					m_wDashPassiveRequiredEP;

	HOBJECT					m_hTeleportProposalRequestor;
	CNtlVector				m_vTeleportProposalLoc;
	CNtlVector				m_vTeleportProposalDir;
	WORLDID					m_teleportProposalWorldID;
	BYTE					m_byTeleportProposalType;

	CNtlVector				m_vEnterTMQLoc; //player loc before he entered TMQ

	DWORD					m_dwLpEmergencyTick;
	DWORD					m_dwChatServerLocationSyncTick;
	DWORD					m_dwFreePvpZoneUpdateTick;

	std::set<PORTALID>		m_setPortalPoints;

	BYTE					m_currentHtbSkill;
	BYTE					m_byHtbUseBalls;
	bool					m_bHtbAttackSuccess;

	bool					m_bHasItemDice;

	DWORD					m_dwScrambleUpdateTickCount;

	bool					m_bIsReviving;

	DWORD					m_dwCombatModeTickCount;

	bool					m_bReceiveExpDisabled;

	DWORD					m_dwLastSpeedHackReport;

	bool					m_bIsAfk;
	DWORD					m_dwAfkTime;
	DWORD					m_dwLastCameraMove; // required for anti-bot check

	DWORD					m_dwCameraMoveCount; // how often camera has been moved (received from client because the server cant't track this)
	DWORD					m_dwCameraMoveDifference;

private:

	void				Initialize();
	void				Destroy();

public:

	sPC_TBLDAT*			GetTbldat() { return (sPC_TBLDAT*)m_pTbldat; }

	void				LoadData(sPC_PROFILE* pcdata, sPC_TBLDAT* pTbldat);

	virtual void		LeaveGame();

	virtual void		TickProcess(DWORD dwTickDiff, float fMultiple);
	void				UpdateFreePvpZone(DWORD dwTickDiff);

	CQuest*				GetQuests()	const		{ return pQuest; }

	virtual void		EncodeInsertPacket(CPlayer* pPlayer); //SHOW OBJECT PACKET
	virtual void		CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID);

	void				CreateDestServerInfo(sDBO_SERVER_INFO* pServerInfo, BYTE* pbyAuthKey);
	void				CopyDestServerInfo(sSERVER_INFO* pServerInfo, char* pchAuthKey);


	/************************************************************************/
	/* Player loading and savings*/
	//new
	void			SendLoadPcDataReq();
	void			RecvLoadPcDataRes(sPC_DATA* pPcData, sDBO_SERVER_CHANGE_INFO* pserverChangeInfo, bool bTutorialFlag, sCHAR_WAR_FOG_FLAG* pWarFogInfo, sMAIL_NEW_BREIF* pMailBrief, sRANKBATTLE_SCORE_INFO* pRankBattleScore, BYTE* pbyTitleIndexFlag, WORD wWaguCoin);

	void			RecvPcItemLoadRes(sITEM_DATA* pData, BYTE byCount, WORD wCurPacketCount);
	void			RecvPcSkillLoadRes(sSKILL_DATA* pData, BYTE byCount);
	void			RecvPcBuffLoadRes(sBUFF_DATA* pData, BYTE byCount);
	void			RecvPcHtbSkillLoadRes(sHTB_SKILL_DATA* pData, BYTE byCount);
	void			RecvPcQuestItemLoadRes(sQUEST_INVENTORY_DATA* pData, BYTE byCount);
	void			RecvPcQuestCompleteLoadRes(const sQUEST_COMPLETE_DATA& rData);
	void			RecvPcQuestProgressLoadRes(sQUEST_PROGRESS_DATA* pData, BYTE byCount);
	void			RecvPcQuickSlotLoadRes(sQUICK_SLOT_DATA* pData, BYTE byCount);
	void			RecvPcShortcutLoadRes(sSHORTCUT_DATA* pData, BYTE byCount);
	void			RecvPcItemRecipeLoadRes(sRECIPE_DATA* pData, WORD wCount);
	void			RecvPcMascotLoadRes(sMASCOT_DATA_EX* pData, BYTE byCount);
	void			RecvPcPortalLoadRes(PORTALID* aPortalID, BYTE byCount);
	void			RecvPcItemCoolTimeLoadRes(sDBO_ITEM_COOL_TIME* pData, BYTE byCount); //in this function we send avatar info end packet. Must be last packet!

	void			SendWarfogInfo(sCHAR_WAR_FOG_FLAG* pWarFogInfo);
	void			SendTitleInfo(BYTE* pbyTitleIndexFlag);
	void			SendRankBattleScoreInfo(const sRANKBATTLE_SCORE_INFO* pRankBattleScore);
	void			SendWaguCoinInfo(WORD wWaguCoin);

	void			SendAvatarInfoEnd();
	void			SendQueryTutorialUpdate(bool bFlag);

	void			_SaveItemCooldowns();

	void			SaveToDB(bool bIsTimer = true);	// Called when logout
	inline void		SetSkipSave(bool b) { m_bSkipSave = b; }

	/************************************************************************/


	virtual void			OnEnterWorld(CWorld* pWorld);
	virtual void			OnLeaveWorld(CWorld* pWorld);
	virtual void			OnEnterWorldComplete();

	void					LevelUp(DWORD dwNewExp, BYTE byLv = 1);

	virtual CSkillManager*	CreateSkillManager();
	virtual CBuffManager*	CreateBuffManager();
	virtual CCharacterAtt*	CreateCharAttManager();

	virtual bool			UpdateCurLP(int lpDiff, bool bIncrease, bool bAutoRecover/*, sLP_TRACE_DATA *rLpTraceData*/);
	virtual bool			UpdateCurEP(WORD wEpDiff, bool bIncrease, bool bAutoRecover);
	virtual bool			UpdateCurLpEp(int nLpDiff, WORD wEpDiff, bool bIncrease, bool bAutoRecover);
	virtual bool			UpdateCurRP(WORD wRpDiff, bool bIncrease, bool bHitCharge);
	virtual bool			UpdateCurAP(int apDiff, bool bIncrease);
	virtual bool			UpdateRpBall(BYTE byDiff, bool bIncrease, bool bDropByTime);

	inline void				SetCanUnstack(bool bFlag) { m_bCanUnstack = bFlag; }
	inline bool				GetCanUnstack() { return m_bCanUnstack; }

	inline void				SetIsReviving(bool bFlag) { m_bIsReviving = bFlag; }
	inline bool				IsReviving() { return m_bIsReviving; }

	inline bool				IsGameMaster() { return m_bIsGameMaster; }
	inline BYTE				GetGMLevel() { return m_byGameMasterLevel; }

	inline void				SetCashShopRefresh(DWORD dwTime) { m_dwLastCashShopRefresh = dwTime; }
	inline DWORD			GetCashShopRefresh() { return m_dwLastCashShopRefresh; }

	inline void				SetItemShopCash(DWORD cash) { m_dwItemShopCash = cash; }
	inline DWORD			GetItemShopCash() { return m_dwItemShopCash; }

	inline void				SetEventMachineCoin(DWORD cash) { m_dwEventMachineCoin = cash; }
	inline DWORD			GetEventMachineCoin() { return m_dwEventMachineCoin; }

	inline void				SetWaguMachineCoin(DWORD cash) { m_dwWaguMachineCoin = cash; }
	inline DWORD			GetWaguMachineCoin() { return m_dwWaguMachineCoin; }

	inline void				SetAccountID(ACCOUNTID id) { uiAccountID = id; }
	inline ACCOUNTID		GetAccountID() const { return uiAccountID; }
	inline void				SetPrevChannelID(SERVERCHANNELID id) { m_byPrevChannelID = id; }
	inline SERVERCHANNELID	GetPrevChannelID() const { return m_byPrevChannelID; }
	inline void				SetCharID(CHARACTERID id)  { player_data.charId = id; }
	inline CHARACTERID		GetCharID() const { return player_data.charId; }
	inline WCHAR*			GetCharName()	{ return player_data.awchName; }
	void					SetCharName(WCHAR* wchName) { NTL_SAFE_WCSCPY(player_data.awchName, wchName); }

	inline bool				IsAdult()  { return player_data.bIsAdult; }
	inline bool				IsTutorial()  { return m_bIsTutorial; }
	inline bool				SetTutorial(bool b) { return m_bIsTutorial = b; }
	inline bool				IsLpEmergancy() const { return player_data.bEmergency; }

	inline void				SetLpEmergency(bool b) { player_data.bEmergency = b; }

	inline BYTE				GetClass() { return GetTbldat()->byClass; }
	inline BYTE				GetRace() { return GetTbldat()->byRace; }
	inline BYTE				GetGender() { return GetTbldat()->byGender; }

	inline void				SetReputation(DWORD rep) { player_data.dwReputation = rep; }
	inline DWORD			GetReputation() const { return player_data.dwReputation; }

	inline void				SetBindLoc(sVECTOR3& v) { player_data.vBindLoc = v; }
	inline sVECTOR3&		GetBindLoc()  { return player_data.vBindLoc; }
	inline void				SetBindDir(sVECTOR3& v) { player_data.vBindDir = v; }
	inline sVECTOR3&		GetBindDir()  { return player_data.vBindDir; }
	inline void				SetBindWorldID(TBLIDX w) { player_data.bindWorldId = w; }
	inline TBLIDX			GetBindWorldID() const { return player_data.bindWorldId; }
	inline void				SetBindType(BYTE type) { player_data.byBindType = type; }
	inline BYTE				GetBindType() const { return player_data.byBindType; }
	inline void				SetBindObjectTblidx(TBLIDX w) { player_data.bindObjectTblidx = w; }
	inline TBLIDX			GetBindObjectTblidx()  { return player_data.bindObjectTblidx; }

	inline void			SetCostumeInvisible(bool bFlag) { player_data.bInvisibleCostume = bFlag; }
	inline bool			GetCostumeInvisible() { return player_data.bInvisibleCostume; }

	inline void			SetTutorialHint(DWORD dwHint) { player_data.dwTutorialHint = dwHint; }

	inline DWORD		GetMudosaPoints() const { return player_data.dwMudosaPoint; }
	inline void			SetMudosaPoints(DWORD dwPoints) { player_data.dwMudosaPoint = dwPoints; }
	void				UpdateMudosaPoints(DWORD dwAmount, bool bQuery = true);

	inline 	DWORD		GetWaguPoints()  { return player_data.dwWaguWaguPoints; }
	inline void			SetWaguPoints(DWORD dwPoints) { player_data.dwWaguWaguPoints = dwPoints; }
	void				UpdateWaguPoints(DWORD dwPoints);

	inline DWORD		GetSkillPoints()  { return player_data.dwSpPoint; }
	inline void			SetSkillPoints(DWORD sp) { player_data.dwSpPoint = sp; }
	void				UpdateCharSP(DWORD dwSkillPoints);

	void				UpdateExp(DWORD Exp, bool bIsMonsterKill);
	inline DWORD		GetMaxExp() const	{ return player_data.dwMaxExpInThisLevel; }

	void				UpdateAdult(bool bSetAdult);
	void				UpdateClass(BYTE byClass);

	inline void			SetExpReceiveDisabled(bool bFlag) { m_bReceiveExpDisabled = bFlag; }
	inline bool			IsReceiveExpDisabled() { return m_bReceiveExpDisabled; }

	//
	void				UpdatePvpZone(bool bStatus);
	inline void			SetPvpZone(bool bStatus){ player_data.bIsInFreePvpZone = bStatus; }
	inline bool			IsPvpZone()	{ return player_data.bIsInFreePvpZone; }

	inline void			SetCurrentHtbSkill(BYTE bySlot) { m_currentHtbSkill = bySlot; }
	inline BYTE			GetCurrentHtbSkill() { return m_currentHtbSkill; }
	inline void			SetHtbUseBalls(BYTE byBalls) { m_byHtbUseBalls = byBalls; }
	inline BYTE			GetHtbUseBalls() { return m_byHtbUseBalls; }

	//combat icon
	inline void			SetCombatMode(bool mode)	{ player_data.bCombatMode = mode; }
	inline bool			GetCombatMode()				{ return player_data.bCombatMode; }
	void				UpdateBattleCombatMode(bool status);

	inline HOBJECT			GetTeleportProposalRequestor() { return m_hTeleportProposalRequestor; }
	inline CNtlVector&		GetTeleportProposalLoc() { return m_vTeleportProposalLoc; }
	inline CNtlVector&		GetTeleportProposalDir() { return m_vTeleportProposalDir; }
	inline WORLDID			GetTeleportProposalWorldID() { return m_teleportProposalWorldID; }
	inline BYTE				GetTeleportProposalType() { return m_byTeleportProposalType; }

	inline CNtlVector&		GetEnterTmqLoc() { return m_vEnterTMQLoc; }
	inline void				SetEnterTmqLoc() { m_vEnterTMQLoc = GetCurLoc(); }

	void				CancelOperating();


	inline bool			HasItemDice() { return m_bHasItemDice; }
	inline void			SetItemDice(bool bFlag) { m_bHasItemDice = bFlag; }

	void				UpdateDbScramble(DWORD dwTickDiff);

	inline bool			GetDragonballScramble() { return player_data.bIsScrambleJoinFlag; }
	inline void			SetDragonballScramble(bool bFlag) { player_data.bIsScrambleJoinFlag = bFlag; }

	inline BYTE			GetDragonballScrambleBallFlag() { return player_data.byHasBattleDragonBallBitFlag; }
	inline void			SetDragonballScrambleBallFlag(BYTE bFlag) { player_data.byHasBattleDragonBallBitFlag = bFlag; }
	
	void				CancelDice();

	void				AfkCheck(DWORD dwTickDiff);
	void				SetLastCameraMoveTick();
	void				SetCameraMoveCount(DWORD dwCount) { m_dwCameraMoveDifference = dwCount - m_dwCameraMoveCount;
															m_dwCameraMoveCount = dwCount; }

	//lava
	void				EnterLava();
	void				LeaveLava();

	virtual void		OnTargetChanged(HOBJECT hOldTarget);

	virtual BYTE		GetMaxChainAttack() { return NtlGetBattleChainAttackSequence(GetLevel()); }

	virtual bool		AttackProgress(DWORD dwTickDiff, float fMultiple);
	virtual bool		OnAttackAction(CCharacter* pAttacker, int nDmg, BYTE byAttackResult, eFAINT_REASON eReason = FAINT_REASON_HIT);
	virtual bool		OnSkillAction(CCharacter* pAttacker, int nDmg, DWORD dwAggroPoint, BYTE byAttackResult, bool bWakeUpTarget);
	virtual bool		Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason = FAINT_REASON_HIT);

	virtual bool		OnBuffDamage(HOBJECT hCaster, float fValue);

	HOBJECT				GetAttackTarget() { return m_hAttackTarget; }
	void				SetAttackTarget(HOBJECT hAttackTarget) { m_hAttackTarget = hAttackTarget; }

	virtual bool		ConsiderAttackRange();
	virtual float		GetAttackRange(CCharacter* pTarget);

	virtual void		StartTeleport(CNtlVector& destLoc, CNtlVector& destDir, WORLDID worldid, BYTE byTeleType, TBLIDX directPlayIdx = INVALID_TBLIDX, bool bSyncDirectPlay = false, bool bTeleportAnotherServer = false);
	void				TeleportAnotherServer(CNtlVector& destLoc, CNtlVector& destDir, TBLIDX worldIdx, WORLDID worldid, BYTE byTeleType, BYTE byChannel, WORD wTime, bool bProposal = true);

	void				CancelTeleportProposal(BYTE byTeleportIndex);

	virtual bool		IsAttackable(CCharacterObject* pTarget);	//check if target is attack able

	void				SetNextAttackTime(); //Only used for players after using a skill

	void				CrashGuard();
	void				EndGuard();
	bool				StartGuard();
	void				DecreaseGuardTime(DWORD dwTickDiff);
	bool				IsGuardSuccess(BYTE byGuardType, bool& rbCanCounterAttack);

	void				GetDashPassiveRequiredEP(CSkill* pSkill, DWORD dwCoolTimeInMilliSecs, WORD & rwRequire_EP);

	TBLIDX				GetMissingDragonball(); //Used for dragonball hunt event. Return id of dragonball which dont have

	//recover LP/EP/AP
	virtual bool		Recover(DWORD dwTickDiff);

	//LP emergancy
	void				LpEmergency(DWORD dwTickDiff);

	//CHAT SERVER LOCATION SYNC
	void				ChatServerCoordinateSync(DWORD dwTickDiff);

	void				AddPortalPoint(PORTALID portalId) { m_setPortalPoints.insert(portalId); }
	bool				HasPortalPoint(PORTALID portalId) { return m_setPortalPoints.find(portalId) != m_setPortalPoints.end(); }

	virtual void		Revival(CNtlVector& rVecLoc, WORLDID worldID, eREVIVAL_TYPE eRevivalType, eTELEPORT_TYPE eTeleportType = TELEPORT_TYPE_DEFAULT);

	bool				DisconnectForSpeedHack(DWORD dwTickCount);


	virtual CStateManager*	CreateStateManager(eAIR_STATE eAirState);

	CSkillManagerPc*	GetSkillManager() { return (CSkillManagerPc*)m_pSkillManager; }
	CBuffManagerPc*		GetBuffManager() { return (CBuffManagerPc*)m_pBuffManager; }
	CCharacterAttPC*	GetCharAtt() { return (CCharacterAttPC*)m_pCharacterAtt; }
	CHtbSkillManager*	GetHtbSkillManager() { return m_pHtbSkillManager; }

/////////////////////////////////////////////////////////////////
// TIMED EVENT
public:

	bool				HasEventType(UINT eventType);

	//free battle
	void				StartFreeBattleEvent();
	void				EndFreeBattleEvent();
	void				event_FreeBattleEvent();

	//combat event
	void				TickProcessCombatMode(DWORD dwTickDiff);


	//disable trigger system event
	void				DisableTriggerSystem(DWORD dwTime);
	void				EnableTriggerSystem();

	//lava damage
	void				event_LavaDamage();

	//proposal teleport
	bool				StartTeleportProposal(CPlayer* pSummoner, WORD wTime, BYTE byTeleportType, BYTE byTeleportIndex, TBLIDX worldTblidx, WORLDID worldId, CNtlVector& destLoc, CNtlVector& destDir);
	void				event_TeleportProposal();


/////////////////////////////////////////////////////////////////
// MONEY
public:
	void			UpdateZeni(BYTE ChangeType, DWORD dwAmount, bool bIsNew, bool bDoQuery = true, bool bSendPacket = true);
	bool			CanReceiveZeni(DWORD dwZeni);



/////////////////////////////////////////////////////////////////
// FREE BATTLE
private:
	CHARACTERID		m_uiFreeBattleTarget;
	bool			m_bIsFreeBattle;
	DWORD			m_dwFreeBattleID;
	bool			m_bIsOutsideBattle;

public:

	inline void			SetFreeBattleTarget(CHARACTERID ID)	{ m_uiFreeBattleTarget = ID; }
	inline CHARACTERID	GetFreeBattleTarget()	const	{ return m_uiFreeBattleTarget; }
	inline void			SetPcIsFreeBattle(bool status)	{ m_bIsFreeBattle = status; }
	inline bool			GetPcIsFreeBattle()				{ return m_bIsFreeBattle; }
	inline void			SetFreeBattleID(DWORD ID)		{ m_dwFreeBattleID = ID; }
	inline DWORD		GetFreeBattleID()				{ return m_dwFreeBattleID; }

	inline void			SetOutsideFreeBattle(bool status){ m_bIsOutsideBattle = status; }
	inline bool			IsOutsideFreeBattle()			{ return m_bIsOutsideBattle; }

	inline void			InitFreeBattle()	{ m_uiFreeBattleTarget = INVALID_CHARACTERID; m_bIsFreeBattle = false; m_dwFreeBattleID = INVALID_DWORD; m_bIsOutsideBattle = false; }



/////////////////////////////////////////////////////////////////
// HOI POIT CRAFTING
public:
	inline BYTE			GetHoiPoiMixLv()				{ return player_data.sMixData.byLevel; }
	inline void			SetHoiPoiMixExp(DWORD exp)	{ player_data.sMixData.dwExp = exp; }
	inline DWORD		GetHoiPoiMixExp()				{ return player_data.sMixData.dwExp; }

	void				SetHoiPoiMixExpAndLevel(DWORD exp);

/////////////////////////////////////////////////////////////////
// NPC-SHOP
private:

	struct sNPC_SHOP_DATA
	{
		HOBJECT		hHandle;
		BYTE		byType;		//eNPC_SHOP_TYPE

		void		Init()
		{
			hHandle = INVALID_HOBJECT;
			byType = INVALID_BYTE;
		}

	}m_npcShopData;

public:

	inline HOBJECT		GetNpcShopHandle() { return m_npcShopData.hHandle; }
	inline BYTE			GetNpcShopType() { return m_npcShopData.byType; }

	inline void			SetNpcShopData(HOBJECT hHandle, BYTE byType)
	{
		m_npcShopData.hHandle = hHandle;
		m_npcShopData.byType = byType;
	}

	inline void			InitNpcShopData() { m_npcShopData.Init(); }


/////////////////////////////////////////////////////////////////
// TRADE
private:
	CTrade*				m_pkTrade;
public:
	inline void			SetTrade(CTrade* trade){ m_pkTrade = trade; }
	inline CTrade*		GetTrade()		{ return m_pkTrade; }

	inline bool			IsTrading() { return m_pkTrade ? true : false; }

/////////////////////////////////////////////////////////////////
// PRIVATE SHOP
public:
	void				CreatePrivateShop();
	void				ClosePrivateShop();

	void				SetPrivateShop(CPrivateShop* pkShop) { m_pkShop = pkShop; }
	CPrivateShop*		GetPrivateShop() { return m_pkShop; }

	inline bool			HasPrivateShop() { return m_pkShop ? true : false; }

	void				SetVisitPrivateShop(CPrivateShop* VisitShop) { m_pkVisitShop = VisitShop; }
	CPrivateShop*		GetVisitPrivateShop() const { return m_pkVisitShop; }

private:

	CPrivateShop*			m_pkShop;
	CPrivateShop*			m_pkVisitShop;

/////////////////////////////////////////////////////////////////
// MAIL
private:
	std::map<MAILID, sMAIL_NEW_PROFILE*>	m_map_MailInfo;

	DWORD									m_dwLastMailReloadStatistic;	//allow manual reloading once every minute

	DWORD									m_dwLastMailReload;	//allow manual reloading once every minute

	DWORD									m_dwLastMailLoad;		//only do database query once every 1 minute to avoid lots of queries when player keep opening mail

	void									ClearMails();

	DWORD									m_dwMailSentTime; //time when it is possible again to send a mail

public:

	bool					AddMail(sMAIL_NEW_PROFILE* mail);
	sMAIL_NEW_PROFILE*		GetMail(MAILID mailId);
	void					DeleteMail(MAILID mailId);
	size_t					CountMails() const { return m_map_MailInfo.size(); }

	void					ReloadMailsStatistic(bool bCheckTime = true);

	void					ManualReloadMails(HOBJECT hObject);

	void					LoadMails(HOBJECT hObject);

public:

	bool					IsMailAway() const { return player_data.bIsMailAway; }
	void					SetIsMailAway(bool b) { player_data.bIsMailAway = b; }

	bool					CanSendMail(DWORD dwTick) { return dwTick >= m_dwMailSentTime; }
	void					SetMailSentTime(DWORD dwTick) { m_dwMailSentTime = dwTick; }

/////////////////////////////////////////////////////////////////
//CASH SHOP
private:

	std::map<QWORD, sCASHITEM_BRIEF*>	mapCashShopItems;

public:

	void								AddCashShopItem(QWORD qwProductID, sCASHITEM_BRIEF* brief);
	void								DelCashShopItem(QWORD qwProductID);
	sCASHITEM_BRIEF*					GetCashShopItem(QWORD qwProductID);

	QWORD								GetLastCashItemProductID() { return (mapCashShopItems.size() == 0) ? 0 : mapCashShopItems.rbegin()->first; }

/////////////////////////////////////////////////////////////////
//HOI POI CRAFTING
private:
	std::map<TBLIDX, sRECIPE_DATA*>		p_HoiPoiRecipeData;
protected:

	void						ClearRecipes();

public:

	void						AddHoiPoiRecipe(TBLIDX recipetblidx, BYTE byRecipeType);
	sRECIPE_DATA*				GetHoiPoiRecipe(TBLIDX recipetblidx);

/////////////////////////////////////////////////////////////////
//QUICK TELEPORT SYSTEM
private:
	bool					m_bIsQuickTeleportLoaded;

	typedef std::map<BYTE, sQUICK_TELEPORT_INFO*>	tQuickTeleportMap;
	tQuickTeleportMap								m_mapQuickTeleport;

public:

	void					AddQuickTeleport(sQUICK_TELEPORT_INFO* data, BYTE bySlot, bool bQuery = true);
	sQUICK_TELEPORT_INFO*	GetQuickTeleportData(BYTE bySlot);
	bool					DelQuickTeleportData(BYTE bySlot);
	void					LoadAddQuickTeleport(sQUICK_TELEPORT_INFO& rInfo);

	void					AddDeathQuickTeleport();

	void					LoadQuickTeleportFromDB();
	void					LoadQuickTeleport();

	void					SetIsQuickTeleportLoaded(bool b)	{ m_bIsQuickTeleportLoaded = b; }
	bool					IsQuickTeleportLoaded()				{ return m_bIsQuickTeleportLoaded; }

/////////////////////////////////////////////////////////////////
//ITEM COOL DOWN
private:
	sDBO_ITEM_COOL_TIME*	m_ItemCooldown;

public:
	void					SetItemCooldown(sDBO_ITEM_COOL_TIME& cooltimedata);
	bool					IsItemCooldown(DWORD CoolTimeBitFlag);
	void					BeginItemCooldown(DWORD CoolTimeBitFlag, DWORD CoolTime, DWORD RemainTime);
	void					ItemCooltimeProcess(DWORD dwTime);

/////////////////////////////////////////////////////////////////
//VEHICLE SYSTEM
private:

	struct sVEHICLE
	{
		void Init()
		{
			hVehicleFuelId = INVALID_HOBJECT;
			vehicleTblidx = INVALID_TBLIDX;
			hVehicleId = INVALID_HOBJECT;
			bGetOffOnAttack = true;
			bAnimationPlay = false;
		}

		HOBJECT			hVehicleFuelId;
		TBLIDX			vehicleTblidx;
		HOBJECT			hVehicleId;
		bool			bGetOffOnAttack;
		bool			bAnimationPlay;

	}m_vehicle;

public:

	bool			StartVehicle(TBLIDX vehicleIdx, HOBJECT hVehicleId);
	void			EndVehicle(WORD wReasonCode);
	void			UpdateVehicleFuel(bool bInsert, BYTE byItemPlace = INVALID_BYTE, BYTE byItemPos = INVALID_BYTE);

	void			SetVehicle(TBLIDX vehicleTblidx, HOBJECT hVehicleId, bool bGetOffOnAttack);

	inline HOBJECT	GetVehicleItemHandle() { return m_vehicle.hVehicleId; }
	inline TBLIDX	GetVehicleTblidx() { return m_vehicle.vehicleTblidx; }

	inline void		SetVehicleFuelId(HOBJECT id)	{ m_vehicle.hVehicleFuelId = id; }
	inline HOBJECT	GetVehicleFuelId()		{ return m_vehicle.hVehicleFuelId; }

	inline void		SetVehicleAniPlay(bool bFlag) { m_vehicle.bAnimationPlay = bFlag; }
	inline bool		IsVehicleAniPlay() { return m_vehicle.bAnimationPlay; }


/////////////////////////////////////////////////////////////////
// ITEM / INVENTORY
private:

	HOBJECT						m_hCastItem;

	CPlayerItemContainer		m_playerItemContainer;

public:

	CPlayerItemContainer*		GetPlayerItemContainer() { return &m_playerItemContainer; }

	bool						EquipItem(CItem* item, BYTE byDestpos);
	bool						UnequipItem(CItem* item);

	void						DecreaseEquipmentDurability();

	void						SetItemCast(HOBJECT hCastItem) { m_hCastItem = hCastItem; }
	HOBJECT						GetItemCast() { return m_hCastItem; }

/////////////////////////////////////////////////////////////////
// BANK
private:

	bool								m_bIsUsingBank;

	bool								m_bBankLoaded;

public:

	inline bool							IsUsingBank() { return m_bIsUsingBank; }
	inline void							SetUsingBank(bool bFlag) { m_bIsUsingBank = bFlag; }

	inline bool							IsBankLoaded() { return m_bBankLoaded; }
	inline void							SetBankLoaded(bool bFlag) { m_bBankLoaded = bFlag; }

public:

	inline void		SetZeniBank(DWORD zeni)	{ player_data.dwZeniBank = zeni; }
	inline DWORD	GetZeniBank()		{ return player_data.dwZeniBank; }

/////////////////////////////////////////////////////////////////
//	RAGE POINTS (RP) SYSTEM
private:

	DWORD					m_dwStartRpDecreaseTickCount;
	bool					m_bDecreaseRpFlag;
	float					m_fRpDecreaseTickCount;
	float					m_fRPChargingTime;
	float					m_fRpCurrentValue;

public:

	void					UpdateMaxRpBalls();

	void					CancelCharging();
	virtual void			StartIncreaseRP();
	virtual void			StopIncreaseRP();
	void					DecreaseRPTickProcess(DWORD dwTickCount);


/////////////////////////////////////////////////////////////////
//	PARTY
private:
	CParty*			m_pParty;

	bool			m_bPartyInvitation;
	CHARACTERID		m_uiInvitedByID;
	PARTYID			m_uiPartyInviteID;

public:

	void			SetParty(CParty* pkParty);
	CParty*			GetParty() 	{ return m_pParty; }

	void			SetIsPartyInvite(bool status)	{ m_bPartyInvitation = status; }
	bool			GetIsPartyInvite()			{ return m_bPartyInvitation; }
	void			SetPartyInvitedByID(CHARACTERID id)	{ m_uiInvitedByID = id; }
	CHARACTERID		GetPartyInvitedByID()				{ return m_uiInvitedByID; }

	void			SetPartyInviteID(PARTYID id)	{ m_uiPartyInviteID = id; }
	PARTYID			GetPartyInviteID()				{ return m_uiPartyInviteID; }

	WORD			CanJoinParty(); //Return resultcode

	void			LeaveParty();

/////////////////////////////////////////////////////////////////
//	GUILD
private:

	GUILDID			m_guildId;
	WCHAR			m_wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];

public:

	void				SetGuildID(GUILDID id);
	GUILDID				GetGuildID() const { return m_guildId; }

	void				SetGuildName(WCHAR* name)	{ wcscpy_s(m_wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, name); }
	WCHAR*				GetGuildName()	{ return m_wszGuildName; }

	void				SetGuildDogiMark(sDBO_GUILD_MARK& sMark, sDBO_DOGI_DATA& sDogi);

	sDBO_GUILD_MARK*	GetGuildMark() { return &player_data.sMark; }
	sDBO_DOGI_DATA*		GetGuildDogi() { return &player_data.sDogi; }
	void				LeaveGuild();

/////////////////////////////////////////////////////////////////
//	WARFOG
private:
	BYTE			warFogFlag[NTL_MAX_SIZE_WAR_FOG];

public:
	void			SetWarFogFlag(BYTE* abyWarFogInfo) { memcpy(warFogFlag, abyWarFogInfo, sizeof(warFogFlag)); }
	bool			CheckWarFog(TBLIDX ContentTblidx);
	bool			AddWarFogFlag(TBLIDX ContentTblidx);
	BYTE*			GetWarFogFlag()	{ return warFogFlag; }

/////////////////////////////////////////////////////////////////
//	WARFOG
private:
	HOBJECT				m_hBusRideOn;

public:
	void				SetBusID(HOBJECT hBus) { m_hBusRideOn = hBus; }
	inline HOBJECT		GetBusID() { return m_hBusRideOn; }

/////////////////////////////////////////////////////////////////
//	CHAR TITLE
private:
	BYTE				m_TitleIndexFlag[NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG];

public:
	void				SetCharTitleID(TBLIDX id){ player_data.charTitle = id; }
	TBLIDX				GetCharTitleID()		 { return player_data.charTitle; }

	bool				CheckCharTitle(TBLIDX title);
	void				AddCharTitle(TBLIDX title);
	void				DelCharTitle(TBLIDX title);

	void				SetAllCharTitle(unsigned char* flags){ memcpy(m_TitleIndexFlag, flags, sizeof(m_TitleIndexFlag)); }
	unsigned char*		GetAllCharTitle() { return m_TitleIndexFlag; }


/////////////////////////////////////////////////////////////////
// PERSONAL MASCOTS
private:
	std::map<SLOTID, CItemPet*>		m_mapMascotData;

	CItemPet*						m_Mascot;
	bool							m_bRemoteBankSkillUsed;

public:

	void						SetCurrentMascot(CItemPet* pet) { m_Mascot = pet; }
	CItemPet*					GetCurrentMascot() { return m_Mascot; }

	CItemPet*					FindMascot(TBLIDX mascotIdx);
	CItemPet*					GetMascot(BYTE byIndex);

	void						InsertMascot(CItemPet* mascot);
	bool						RegisterMascot(TBLIDX mascotTblidx);
	void						DeleteMascot(BYTE byIndex);
	bool						OnlyDeleteMascot(BYTE byIndex);

	void						FusionMascot(BYTE byItemPlace, BYTE byItemPos, BYTE byMascotLevelUpSlot, BYTE byMascotOfferingSlot);

	void						SetRemoteBankSkillUsed(bool bFlag) { m_bRemoteBankSkillUsed = bFlag; }
	bool						CanOpenRemoteBank();


public:

	inline void						SetMascotTblidx(TBLIDX id) { player_data.mascotTblidx = id; }
	inline TBLIDX					GetMascotTblidx() { return player_data.mascotTblidx; }


/////////////////////////////////////////////////////////////////
// SKILL PET
private:
	HOBJECT						m_hPet;

public:

	CSummonPet*					GetSummonPet();

	void						SetCurrentPetId(HOBJECT hId) { m_hPet = hId; }
	HOBJECT						GetCurrentPetId() { return m_hPet; }

/////////////////////////////////////////////////////////////////
// SHENRON
private:

	HOBJECT			hShenronID;

public:

	inline void		SetShenronID(HOBJECT id) { this->hShenronID = id; }
	inline HOBJECT	GetShenronID() { return this->hShenronID; }


/////////////////////////////////////////////////////////////////
// DUNGEON

private:

	CTimeLeapDungeon*				m_pTimeLeapDungeon;
	CUltimateDungeon*				m_pUltimateDungeon;
	CBattleDungeon*					m_pBattleDungeon;

public:

	inline void						SetTLQ(CTimeLeapDungeon* pDungeon) { m_pTimeLeapDungeon = pDungeon; }
	inline CTimeLeapDungeon*		GetTLQ() { return m_pTimeLeapDungeon; }

	inline void						SetUD(CUltimateDungeon* pDungeon) { m_pUltimateDungeon = pDungeon; }
	inline CUltimateDungeon*		GetUD() { return m_pUltimateDungeon; }

	inline void						SetCCBD(CBattleDungeon* pDungeon) { m_pBattleDungeon = pDungeon; }
	inline CBattleDungeon*			GetCCBD() { return m_pBattleDungeon; }


/////////////////////////////////////////////////////////////////
// EVENT REWARD

private:
	
	DWORD								m_dwLastEventRewardLoad;

	std::map<TBLIDX, sEVENT_PC_NAME>	m_mapEventReward;

public:

	DWORD							GetLastEventRewardLoad() { return m_dwLastEventRewardLoad; }
	void							SetLastEventRewardLoad(DWORD dwTick) { m_dwLastEventRewardLoad = dwTick; }

	inline void						ClearEventRewardMap() { m_mapEventReward.clear(); }
	inline void						InsertEventReward(TBLIDX rewardTblidx, sEVENT_PC_NAME& info) { m_mapEventReward.insert({ rewardTblidx, info }); }
	bool							HasEventReward(TBLIDX rewardTblidx, CHARACTERID charId);
	inline void						EraseEventReward(TBLIDX rewardTblidx) { m_mapEventReward.erase(rewardTblidx); }

	void							LoadExistingEventReward();



/////////////////////////////////////////////////////////////////
// TRANSFORMATION (SUPER SAIYAN, KAIOKEN ETC)
private:

	bool						m_bTransformBySkill;
	sSTATUS_TRANSFORM_TBLDAT*	m_pTransformTbldat;
	BYTE						m_byTransformGrade;
	BYTE						m_byTransformSkillIndex;

public:

	bool						CanTransform(eSYSTEM_EFFECT_CODE effectCode, TBLIDX transformTblidx, bool bTransformBySkill);
	WORD						Transform(eSYSTEM_EFFECT_CODE effectCode, TBLIDX transformTblidx, bool bTransformBySkill, BYTE byTransformSkillIndex = INVALID_BYTE); //effectTblidx = skill or item id
	void						CancelTransformation();
	void						EndTransformation(bool bCalculateAttribute = false);

	BYTE						GetTransformGrade() { return m_byTransformGrade; }

	sSTATUS_TRANSFORM_TBLDAT*	GetTransformationTbldat() { return m_pTransformTbldat; }


/////////////////////////////////////////////////////////////////
// Fly system
private:

	DWORD				m_dwLastAirAccelStartCheck;

	bool				m_bCanAccel;

	void				CheckAirAccel();

public:

	void				SetAirDashStartTime(DWORD dwTime) { m_dwLastAirAccelStartCheck = dwTime; }

	void				SetCanAirAccel(bool bFlag) { m_bCanAccel = bFlag; }
	bool				CanAirAccel() { return m_bCanAccel; }

/////////////////////////////////////////////////////////////////
// VISIBLE OBJECTS
private:

	std::set<HOBJECT> m_visibleObjects;

public:

	bool					IsVisible(HOBJECT hObj) { return !(m_visibleObjects.find(hObj) == m_visibleObjects.end()); }
	void					AddVisibleObject(HOBJECT hObj) { m_visibleObjects.insert(hObj); }
	void					RemoveVisibleObject(HOBJECT hObj) { m_visibleObjects.erase(hObj); }
	BYTE					GetVisibleCount() { return (BYTE)m_visibleObjects.size(); }


/////////////////////////////////////////////////////////////////
// RANK BATTLE
private:
	TBLIDX					m_RankBattleRoomTblidx;
	HOBJECT					m_RankBattleRegisterObject;
	ROOMID					m_RankBattleRoomId;

	sRANKBATTLE_SCORE_INFO	m_sScoreInfo;

	sRANK_BATTLE_DATA		m_sRankBattleData;

public:

	void				SetRankBattleRoomTblidx(TBLIDX tblidx) { m_RankBattleRoomTblidx = tblidx; }
	TBLIDX				GetRankBattleRoomTblidx() { return m_RankBattleRoomTblidx; }
	void				SetRankBattleRegisterObject(HOBJECT hObject) { m_RankBattleRegisterObject = hObject; }
	HOBJECT				GetRankBattleRegisterObject() { return m_RankBattleRegisterObject; }
	void				SetRankBattleRoomId(ROOMID roomid) { m_RankBattleRoomId = roomid; }
	ROOMID				GetRankBattleRoomId() { return m_RankBattleRoomId; }
	void				LeaveRankBattleQueue();

	sRANKBATTLE_SCORE_INFO*	GetRankBattleScoreInfo() { return &m_sScoreInfo; }

	sRANK_BATTLE_DATA*		GetRankBattleData() { return &m_sRankBattleData; }

	void				UpdateRankBattleScore(DWORD dwWin, DWORD dwDraw, DWORD dwLose, DWORD dwMileage, float fPoint, bool bKoWin, BYTE byBattlemode);


/////////////////////////////////////////////////////////////////
// BUDOKAI
private:

	JOINID				m_joinId;
	BYTE				m_byMatchIndex;
	BYTE				m_byBudokaiPcState; // eMATCH_MEMBER_STATE
	TEAMTYPE			m_wBudokaiTeamType;

public:

	void				InitBudokai();

	inline void			SetJoinID(JOINID joinid) { m_joinId = joinid; }
	inline JOINID		GetJoinID() { return m_joinId; }

	inline void			SetMatchIndex(BYTE byIndex) { m_byMatchIndex = byIndex; }
	inline BYTE			GetMatchIndex() { return m_byMatchIndex; }

	inline void			SetBudokaiPcState(BYTE byState) { m_byBudokaiPcState = byState; }
	inline BYTE			GetBudokaiPcState() { return m_byBudokaiPcState; }

	inline void			SetBudokaiTeamType(TEAMTYPE type) { m_wBudokaiTeamType = type; }
	inline TEAMTYPE		GetBudokaiTeamType() { return m_wBudokaiTeamType; }


/////////////////////////////////////////////////////////////////
// TELEPORT
private:

	TBLIDX					m_uiTeleportDirectPlay;
	bool					m_bTeleportSyncDirectPlay;
	bool					m_bTeleportAnotherServer;
	sSERVERTELEPORT_INFO	m_sServerTeleportInfo;

public:

	TBLIDX					GetTeleportDirectPlay()  { return m_uiTeleportDirectPlay; }
	bool					GetTeleportSyncDirectPlay()  { return m_bTeleportSyncDirectPlay; }
	bool					GetTeleportAnotherServer() { return m_bTeleportAnotherServer; }

	sSERVERTELEPORT_INFO&	GetServerTeleportInfo() { return m_sServerTeleportInfo; }
	void					SetServerTeleportWorld(WORLDID worldId, TBLIDX WorldTblidx);

	void					ResetDirectPlay() { m_uiTeleportDirectPlay = INVALID_TBLIDX; m_bTeleportSyncDirectPlay = false; }


/////////////////////////////////////////////////////////////////
// TRIGGER-OBJECT
private:

	bool			m_bUseItemToExcuteTriggerObject;

public:

	bool			IsExecutingTriggerObjectWithItem() { return m_bUseItemToExcuteTriggerObject; }
	void			SetUseItemToExcuteTriggerObject(bool bFlag) { m_bUseItemToExcuteTriggerObject = bFlag; }



/////////////////////////////////////////////////////////////////
// FACING NPC
private:

	HOBJECT			m_hFacingHandle;

public:

	HOBJECT			GetFacingHandle() { return m_hFacingHandle; }
	void			SetFacingHandle(HOBJECT hHandle) { m_hFacingHandle = hHandle; }


/////////////////////////////////////////////////////////////////
// Hack check
private:

	float			m_fLastAppTime;
	DWORD			m_dwLastHackCheck;

public:

	void			SpeedHackCheck(float fClientAppTime, bool bSpeedHack);

};

#endif