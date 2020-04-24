#ifndef __SERVER_CHAR___
#define __SERVER_CHAR___

#include "NtlBattle.h"
#include "NtlTeleport.h"
#include "NtlHlsItem.h"
#include "SpawnTable.h"
#include "MobTable.h"
#include "NpcTable.h"

#include "TargetListManager.h"

#include "BotAiController.h"
#include "MeHaterManager.h" // #include "CharacterObject.h"
#include "BotBreakUp.h"


class CPcTrigger;
class CTriggerObject;

class CNtlPacket;
class CGameServer;

class CPlayer;
class CNpcParty;

class CScriptAlgoObject;
class CTimeQuest;


//====================================================================
//  CCharacter
//  for characters
//====================================================================
class CCharacter : public CCharacterObject
{

	//////////////////////////////////////////////////////////////////////////////////

public:

	bool			SendCharStateCasting(DWORD castingtime, DWORD timeremaining, TBLIDX skillid, HOBJECT target);
	void			SendCharStateStanding(bool bCheckState = true);
	bool			SendCharStateSkillAffecting(TBLIDX skillid, HOBJECT hTarget);
	void			SendCharStateItemCasting(DWORD castingtime, DWORD timeremaining, TBLIDX itemid, HOBJECT target);
	void			SendCharStateIdle();
	bool			SendCharStateKeepingEffect(TBLIDX skillTblidx);
	void			SendCharStateSpawning(BYTE byType, bool bSpawnDirection = true, bool bIsFaint = false);
	void			SendCharStateFalling(BYTE byMoveDirection);
	void			SendCharStateDespawning(BYTE byTeleportType);
	void			SendCharStateOperating(TBLIDX directTblidx, DWORD dwOperateTime, HOBJECT hTarget);
	bool			SendCharStateFollowing(HOBJECT hTarget, float fDistance, BYTE byMoveReason, sVECTOR3& vDestLoc, bool bRunMode, bool bBroadCast = true);
	void			SendCharStateTeleporting();
	bool			SendCharStateFocusing();
	void			SendCharStateLeaving(eCHARLEAVING_TYPE eCharLeaveType, bool bSendPacket);
	void			SendCharStateDash(CNtlVector& destLoc);

	void			SendCharStateRiding(HOBJECT hBusId); //USED FOR BUS SYSTEM
	void			SendCharStateFaint(BYTE byReason);
	void			SendCharStatePrivateShop(bool bIsOwnerEmpty, BYTE byShopState, WCHAR* wcPrivateShopName);

	void			SendCharStateSleeping();
	void			SendCharStateParalyzed();
	void			SendCharStateStunned(eDBO_STUN_TYPE eStunType = DBO_STUN_TYPE_GENERAL, HOBJECT hCaster = INVALID_HOBJECT, bool bCheckCaster = false);
	void			SendCharStateSandbag();

	void			SendCharStateKnockdown(sVECTOR3& rShift);

	void			SendCharStateAirDashAccel(BYTE byMoveDir, BYTE byMoveFlag);
	bool			SendCharStateMoving(BYTE byMoveDir, BYTE byFlag, bool bBroadCast);

	void			SendCharStateDestMove(bool bHaveSecondDestLoc, CNtlVector& rSecondDestLoc, BYTE actionPatternIndex, BYTE byDestLocCount, sVECTOR3* pavDestLoc, bool bRunMode);

	void			StandUpIfPossible(BYTE byEndState, WORD wEffectCodeEnd = INVALID_WORD);

	void			StartCandy();
	void			EndCandy();


	//aspect state
	void			SendAspectStateSpinningAttack();
	void			SendAspectStateInvalid();
	void			SendAspectStateRollingAttack();


	void			Send_BotNonBattleActionPatternSet_UnitIdx(BYTE actionpatternIdx);  //should not be used for players


public:
	CCharacter(eOBJTYPE eObjType);
	virtual ~CCharacter();


	bool			Create(sCHARSTATE* sCharState);
	void			Destroy();

	virtual void	TickProcess(DWORD dwTickDiff, float fMultiple);

	virtual void	LeaveGame();

protected:

	void			Initialize();

protected:

	BYTE							m_bySizeRate;

	std::set<PARTYID>				m_setMarkedByEnemyParty;		//party id from enemy which marked me as target.

private:

	DWORD							m_dwRecoverTick;

	/////////////////////////////////////////////////////////////////
	//	BASIC THINGS
public:

	void				SetMarkedByParty(PARTYID partyId) { m_setMarkedByEnemyParty.insert(partyId); }
	void				RemoveMarkedByParty(PARTYID partyId) { m_setMarkedByEnemyParty.erase(partyId); }

	virtual float		GetAttackRange(CCharacter* pTarget);

	virtual float		GetCurSpeed();

	virtual bool		IsChaseAble() { return true; }
	virtual bool		IsMoveAble() { return true; }
	virtual bool		IsFleeAble() { return true; }

	virtual HOBJECT		ConsiderScanTarget(WORD wRange) { return INVALID_HOBJECT; }

	ALLIANCEID			GetAllianceID() { return INVALID_ALLIANCEID; }

	void				SetFixedTarget(HOBJECT hTarget) { GetTargetListManager()->SetFixedTarget(hTarget); }
	HOBJECT				GetFixedTarget() { return GetTargetListManager()->GetFixedTarget(); }

	bool				IsBotStateMovingMode();

	virtual bool		UpdateFollow(DWORD dwTickDiff, float fMultiple);

	virtual bool		UpdateCurLP(int lpDiff, bool bIncrease, bool bAutoRecover/*, sLP_TRACE_DATA *rLpTraceData*/);
	virtual bool		UpdateCurEP(WORD wEpDiff, bool bIncrease, bool bAutoRecover);
	virtual bool		UpdateCurLpEp(int nLpDiff, WORD wEpDiff, bool bIncrease, bool bAutoRecover);

	virtual bool		IsInSightRange(CCharacter* pChar) { return false; }
	virtual bool		ConsiderAttackRange() { return false; }

	bool				ChangeModelData(const char* pszModelDataResourceName);

	void				StartDirectPlay(BYTE byDirectionType, TBLIDX directTblidx, bool bSynchronize, bool bCanSkip, BYTE byPlayMode, bool bSendDirectPlay = true);
	void				EndDirectPlay();

	void				UpdateSizeRate(BYTE bySizeRate);

	virtual void		Revival(CNtlVector& rVecLoc, WORLDID worldID, eREVIVAL_TYPE eRevivalType, eTELEPORT_TYPE eTeleportType = TELEPORT_TYPE_DEFAULT) {}

	//recover LP/EP/AP
	virtual bool		Recover(DWORD dwTickDiff);

	void				StartInvincibleEvent();
	void				OnInvincibleEvent();

public:


	/////////////////////////////////////////////////////////////////
	//	BATTLE
public:

	void				ResetMeAttackBot() { m_hMeAttackBot = INVALID_HOBJECT; }
	void				SetMeAttackBot(HOBJECT hMeAttackBot) { m_hMeAttackBot = hMeAttackBot; }
	HOBJECT				GetMeAttackBot() { return m_hMeAttackBot; }

	void				InitKnockDown() { m_dwKnockdownTime = 0; }
	void				IncKnockDownTime(DWORD dwTime) { m_dwKnockdownTime += dwTime; }
	DWORD				GetKnockDownTime() { return m_dwKnockdownTime; }

	void				EnterConfusedState(HOBJECT hCaster);
	void				EnterTerrorState(HOBJECT hCaster);

	inline void			SetTargetInConfusion(HOBJECT hTarget) { m_hTargetInConfusion = hTarget; }
	inline HOBJECT		GetTargetInConfusion() { return m_hTargetInConfusion; }

	inline void			SetCheckTargetInConfusion(bool bFlag) { m_bCheckTargetInConfusion = bFlag; }
	inline bool			GetCheckTargetInConfusion() { return m_bCheckTargetInConfusion; }

	void				InsertKeepEffectTarget(HOBJECT hObj, BYTE byBuffIndex);
	void				EraseKeepEffectTarget(HOBJECT hObj);
	void				ClearKeepEffectMap();
	void				RemoveKeepEffectTargets();

	float				GetCrowdControlReduction(HOBJECT hTarget, TBLIDX skillIdx);
	void				ClearCrowdControlReduction();

	void				SetSpecialSkillTblidx(TBLIDX idx) { m_specialSkillTblidx = idx; }
	TBLIDX				GetSpecialSkillTblidx() { return m_specialSkillTblidx; }

	inline	void		SetHtbSkillCaster(HOBJECT handle) { m_hHtbSkillCaster = handle; }
	inline HOBJECT		GetHtbSkillCaster() { return m_hHtbSkillCaster; }


private:

	HOBJECT				m_hMeAttackBot;
		
	DWORD				m_dwKnockdownTime;

	HOBJECT				m_hTargetInConfusion;
	bool				m_bCheckTargetInConfusion;

	TBLIDX				m_specialSkillTblidx;

	std::multimap<HOBJECT, BYTE>	m_mapKeepEffectTargets; //<OBJ-Handle, Skill-ID>

	std::map<TBLIDX, float>		m_mapCrowdcontrolReduction; //<BASIC-SKILLID, reduction in %>

	HOBJECT				m_hHtbSkillCaster; //Handle of object which uses htb on me

public:

	typedef std::map<HOBJECT, DWORD>	TDamageMap;

	typedef struct SAttackLog
	{
		HOBJECT		hHandle;
		DWORD		dwTime;
	} AttackLog;


public:

	void			AttackAction(CCharacter* pVictim, bool bIsChainAttack);
	void			SpecialAttackAction(CCharacter* pVictim, BYTE byAttackType, BYTE bySourceType, TBLIDX sourceTblidx, CSkill* pSkill);
	virtual bool	OnAttackAction(CCharacter* pAttacker, int nDmg, BYTE byAttackResult, eFAINT_REASON eReason = FAINT_REASON_HIT);
	virtual bool	OnSkillAction(CCharacter* pAttacker, int nDmg, DWORD dwAggroPoint, BYTE byAttackResult, bool bWakeUpTarget);


	virtual bool	ChangeFightMode(bool bFightMode);
	virtual bool	ChangeAttackProgress(bool bAttackProgress);

	BYTE			GetAttackType();
	virtual bool	Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason = FAINT_REASON_HIT);

	float			GetAttackFollowRange();


public:

	/////////NEW

	bool			IsTargetAttackble(CCharacter* pTarget, WORD wRange);
	bool			IsTargetAttackble(HOBJECT hTarget, WORD wRange);

	virtual void	OnTargetChanged(HOBJECT hOldTarget);

	bool			IsReachable(CCharacter *pTarget, std::list<CNtlVector>& rlistCollisionPos);
	bool			IsReachable(HOBJECT hTarget, std::list<CNtlVector>& rlistCollisionPos);

	bool			ApplyPush(CNtlVector& rvShift);
	void			SetSizeRate(BYTE bySizeRate) { m_bySizeRate = bySizeRate; }

	/////////////////////////////////////////////////////////////////
	// 
public:

	void			SendEffectAffected(TBLIDX EffectTblidx, BYTE bySrcType, TBLIDX SrcTblidx, float fArgument1, float fArgument2, HOBJECT hCaster);
	void			SendAutoRecoverNfy(TBLIDX Tblidx, BYTE buffIndex, BYTE bySrcType, TBLIDX commonConfigTblidx, DWORD dwRemainTime, DWORD dwRemainValue, int recoverValue);
	void			LpEpRecoverOnHit(HOBJECT hCaster, float fDmg);

	////////////////////////////////////////////////////////////////////////////////////////
	// Buffs
public:

	bool					CannotMoveByBuff();

	virtual bool			OnBuffDamage(HOBJECT hCaster, float fValue);

public:

	void				SetTauntTarget(HOBJECT hTarget);

private:

	CTargetListManager*	m_pTargetListManager;
	CTargetListManager*	CreateTargetListManager();

	CMeHaterManager		m_cMeHaterManager;
	CBotBreakUp			m_cBotBreakUp;


public:

	CTargetListManager*	GetTargetListManager() { return m_pTargetListManager; }

	CBotBreakUp*		GetBotBreakUp() { return &m_cBotBreakUp; }
	CMeHaterManager*	GetMeHaterManager() { return &m_cMeHaterManager; }

	virtual void		ChangeAggro(HOBJECT hProvoker, BYTE byAggroChangeType, DWORD dwValue);

public:

	CBotAiController*		GetBotController() { return (CBotAiController*)m_pObjectController; }

	/////////////////////////////////////////////////////////////////
	// PARTY
public:

	inline PARTYID			GetPartyID()	{ return m_partyId; }
	inline void				SetPartyID(PARTYID id) { m_partyId = id; }

	virtual CNpcParty*		GetNpcParty() { return NULL; }

private:
	PARTYID			m_partyId;

	/////////////////////////////////////////////////////////////////
	//	ENTITY


protected:

	CNtlVector				m_vTeleportLoc;
	CNtlVector				m_vTeleportDir;
	WORLDID					m_uiTeleportWorldID;
	BYTE					m_byTeleportType;

	CNtlVector				m_vBeforeTeleportLoc;
	WORLDID					m_uiBeforeTeleportWorldID;

public:

	inline void				SetTeleportLoc(CNtlVector& loc)		{ m_vTeleportLoc = loc; }
	inline void				SetTeleportDir(CNtlVector& dir)		{ m_vTeleportDir = dir; }
	inline CNtlVector&		GetTeleportLoc() 					{ return m_vTeleportLoc; }
	inline CNtlVector&		GetTeleportDir() 					{ return m_vTeleportDir; }
	inline void				SetTeleportWorldID(WORLDID world)	{ m_uiTeleportWorldID = world; }
	inline WORLDID			GetTeleportWorldID() const			{ return m_uiTeleportWorldID; }


	inline void				SetBeforeTeleportLoc(CNtlVector& loc) { m_vBeforeTeleportLoc = loc; }
	inline CNtlVector&		GetBeforeTeleportLoc() { return m_vBeforeTeleportLoc; }
	inline void				SetBeforeTeleportWorldID(WORLDID world) { m_uiBeforeTeleportWorldID = world; }
	inline WORLDID			GetBeforeTeleportWorldID() const { return m_uiBeforeTeleportWorldID; }

	inline bool				IsTeleporting()  { return GetStateManager()->IsCharState(CHARSTATE_TELEPORTING); }

	inline void				SetTeleportType(BYTE byType)  { m_byTeleportType = byType; }
	inline BYTE				GetTeleportType()  { return m_byTeleportType; }

	inline void				ResetTeleportData() { m_vTeleportLoc.Reset(); m_vTeleportDir.Reset(); m_uiTeleportWorldID = INVALID_WORLDID; }

	virtual void			StartTeleport(CNtlVector& destLoc, CNtlVector& destDir, WORLDID worldid, BYTE byTeleType, TBLIDX directPlayIdx = INVALID_TBLIDX, bool bSyncDirectPlay = false, bool bTeleportAnotherServer = false);



/////////////////////////////////////////////////////////////////
//	Script
public:

	bool					AddScript(CScriptAlgoObject* pScript);
	void					RemScript(DWORD dwScriptID);
	CScriptAlgoObject*		GetScript(DWORD dwScriptID);
	int						GetScriptCount();

	void					RemAllScript();
	void					ClearScripts();

	CScriptAlgoObject*		GetFirstScript(eSSD_SCRIPT_TYPE scripType);


private:

	std::map<DWORD, CScriptAlgoObject*> m_ScriptMap;




/////////////////////////////////////////////////////////////////
//	Dungeon
public:

	inline void						SetTMQ(CTimeQuest* pDungeon) { m_pTimeQuestDungeon = pDungeon; }
	inline CTimeQuest*				GetTMQ() { return m_pTimeQuestDungeon; }

private:

	CTimeQuest*						m_pTimeQuestDungeon;


};
#endif