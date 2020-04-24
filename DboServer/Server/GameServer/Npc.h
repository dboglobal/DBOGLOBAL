#ifndef __CNPC__
#define __CNPC__

#include "char.h"
#include "ControlStateManager.h"
#include "BotAiController.h"
#include "BotPathFinder.h"
#include "NpcParty.h"
#include "ActionPattern.h"
#include "HelpMeListManager.h"
#include "EscortManager.h"
#include "ControlScriptEventMap.h"
#include "NpcSpeechTable.h"
#include "HitBySkillContainer.h"
#include "SkillManagerBot.h"
#include "BuffManagerBot.h"
#include "CharacterAttBot.h"


class CNpc : public CCharacter
{

public:

	const static DWORD				CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS = 100;

	CNpc(eOBJTYPE eObjType = OBJTYPE_NPC);
	virtual ~CNpc();

protected:

	virtual void		CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID);

	void				Initialize();
	void				Destroy();

	bool				Create(sCHARSTATE* sCharState);

public:

	bool					CreateDataAndSpawn(WORLDID worldId, sNPC_TBLDAT* npcTbldat, sSPAWN_TBLDAT* spawnTbldat, bool bSpawnOnServerStart = false, BYTE bySpawnFuncFlag = SPAWN_FUNC_FLAG_RESPAWN);

	bool					CreateDataAndSpawn(sNPC_DATA& sData, sNPC_TBLDAT* npcTbldat);

	virtual bool			DoTransformation(TBLIDX tblidx, ACTIONPATTERNTBLIDX actionPatternTblidx, TBLIDX tblidxOnlyOneSkillUse);

	virtual void			Spawn(bool bSpawnOnServerStart);

	virtual void			OnEnterWorld(CWorld* pWorld);
	virtual void			OnLeaveWorld(CWorld* pWorld);

	virtual void			LeaveGame();

	virtual void			TickProcess(DWORD dwTickDiff, float fMultiple);

	virtual int				OnObjectMsg(CObjectMsg* pObjectMsg);

	bool					CreateShenron(sNPC_TBLDAT* pNpcTbldat, CNtlVector& rLoc, sVECTOR2& rDir);

	CCharacterAttBot*		GetCharAtt() { return (CCharacterAttBot*)m_pCharacterAtt; }

	void					ConsiderRespawn();

	//recover LP/EP
	virtual bool			Recover(DWORD dwTickDiff);

	virtual CSkillManager*	CreateSkillManager();
	virtual CBuffManager*	CreateBuffManager();
	virtual CCharacterAtt*	CreateCharAttManager();
	virtual CStateManager*	CreateStateManager(eAIR_STATE eAirState);

public:

	TBLIDX			GetMerchant(BYTE byMerchantTab);

	sNPC_TBLDAT*	GetTbldat() { return reinterpret_cast<sNPC_TBLDAT*>(m_pTbldat); }

	DWORD			GetNpcFuncBitflag()  { return GetTbldat()->dwFunc_Bit_Flag; }

	BYTE			GetAttackType()  { return GetTbldat()->byAttack_Type; }


protected:

	bool					m_bIsInRespawn; //is bot in respawn progress?

	sBOT_PROFILE			bot_profile;

	eRELATION_TYPE			m_eRelationPc;

public:

	bool					IsInRespawn() { return m_bIsInRespawn; }

	sBOT_PROFILE*			GetBotProfile() { return &bot_profile; }

	BYTE				GetSpawnRange() { return bot_profile.bySpawnRange; }
	void				SetSpawnFuncFlag(BYTE byFlag) { bot_profile.bySpawnFuncFlag = byFlag; }
	BYTE				GetSpawnFuncFlag() { return bot_profile.bySpawnFuncFlag; }
	WORD				GetSpawnTime() { return bot_profile.wSpawnTime; }
	BYTE				GetMoveType() { return bot_profile.byMoveType; }
	BYTE				GetWanderRange() { return bot_profile.byWanderRange; }
	BYTE				GetMoveRange() { return bot_profile.byMoveRange; }
	TBLIDX				GetPathTblidx() { return bot_profile.pathTblidx; }

	BYTE				GetNestType() { return bot_profile.sBotSubData.byNestType; }
	BYTE				GetNestRange() { return bot_profile.sBotSubData.byNestRange; }
	TBLIDX				GetOnlyOneSkillUse() { return bot_profile.sBotSubData.tblidxOnlyOneSkillUse; }

	void				SetPcRelation(eRELATION_TYPE eRelation) { m_eRelationPc = eRelation; }
	eRELATION_TYPE		GetPcRelation() { return m_eRelationPc; }

	void				LoadSkillTable(TBLIDX tblidxOnlyOneSkillUse);

	HOBJECT				RemoveTargetAndPartyHelpList(HOBJECT hTarget);

	bool				IsAttacked(DWORD dwAttackTime);
	void				SetAttacked(bool bAttacked, DWORD dwLastAttackedTime);
	virtual bool		Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason = FAINT_REASON_HIT);

	float				GetTargetAndMeBodySizeRange(CCharacter* pTarget);

	virtual bool		HasFunction(DWORD dwFuncFlag);
	bool				HasAttribute(DWORD dwFuncFlag);

	bool				HasNearbyPlayer(bool bQuadPageCheck);
	bool				HasNearbyPlayerInQuadArea();
	bool				HasNearbyPlayerInArea();

	bool				IsTargetAttackState(CCharacter* pTarget);
	bool				CanSeeOnLineOfSight(CCharacter* pTarget);

	void				SetActionStatusFlag(eBOT_ACTIONSTATUS eActionStatus, bool bSet);
	bool				IsActionStatusFlag(eBOT_ACTIONSTATUS eActionStatus);

	CNtlVector&			GetBattleNestLoc();

	virtual bool		SetTargetHandle(HOBJECT hHandle);

	void				UpdateNickName(TBLIDX nickNameIdx);

	bool				IsPerceiveEnergy(CCharacter* pTarget);

private:

	bool				m_bIsAttacked;
	DWORD				m_dwLastAttackedTime;


public:

	void				SetLinkPc(CHARACTERID charId, HOBJECT hPc) { m_linkCharId = charId; m_hLinkPc = hPc; }
	CHARACTERID			GetLinkPcId() { return m_linkCharId; }
	HOBJECT				GetLinkPc() { return m_hLinkPc; }

private:

	CHARACTERID			m_linkCharId;
	HOBJECT				m_hLinkPc;

private:

	int						m_nActionStatusFlag;
	CActionPattern			m_cActionPattern;
	CHitBySkillContainer	m_cHitBySkillContainer;

protected:

	bool					RefreshActionPatternData(TBLIDX actionPatternTblidx);

public:

	CActionPattern*			GetActionPattern() { return &m_cActionPattern; }
	CHitBySkillContainer*	GetHitBySkillContainer() { return &m_cHitBySkillContainer; }

	CSkillManagerBot*		GetSkillManager() { return (CSkillManagerBot*)m_pSkillManager; }
	CBuffManagerBot*		GetBuffManager() { return (CBuffManagerBot*)m_pBuffManager; }

	//BOT PATH
private:
	CBotPathFinder*			m_pPathFinder;
	CBotPathFinder*			CreatePathFinder();

public:
	CBotPathFinder*			GetPathFinder() { return m_pPathFinder; }

private:
	NPC_PARTYPTR			m_partyPtr;

public:

	virtual CNpcParty*		GetNpcParty();
	sVECTOR3&				GetPartyLeaderDistance() { return bot_profile.vPartyLeaderDistance; }

	void					SetPartyPtr(NPC_PARTYPTR partyPtr) { m_partyPtr = partyPtr; }
	NPC_PARTYPTR			GetPartyPtr() { return m_partyPtr; }

	void					HelpParty(HOBJECT hProvoker);
	void					HelpAlliance();
	void					HelpDefend();

private:

	void					AllianceBroadcast(CObjectMsg *pObjectMsg, bool bPost, CCharacterObject *pExcept, DWORD dwDelayTime);
	void					AllianceHelp(CObjectMsg *pObjectMsg, const BYTE byHelpMaxCnt, CCharacterObject *pExcept);

public:

	bool					IsPartyLeader() { return bot_profile.bPartyLeader; }

	CNpc*					GetPartyLeader(HOBJECT hLeader, bool* bIsMePartyLeader);
	bool					IsSummonablePartyMember();



	//AI
private:

	CHelpMeListManager		m_cHelpMeListManager;
	CControlStateManager	m_cControlStateManager;

	CEscortManager*			m_pEscortManager;

	bool					m_bPlayScriptEnd;

protected:

	DWORD					m_dwAi_Bit_Flag;

public:

	DWORD*					GetAiBitFlag()	{ return &m_dwAi_Bit_Flag; }
	virtual bool			HasAIFunction(DWORD dwAIBitFlag);
	virtual bool			IsChaseAble();
	virtual bool			IsMoveAble();
	virtual bool			IsFleeAble();
	virtual bool			IsInSightRange(CCharacter* pChar);

	int						OnMsg_YouKeepAggro(CObjectMsg * pObjectMsg);
	int						OnMsg_YouHit(CObjectMsg * pObjectMsg);
	int						OnMsg_YouHitBySkill(CObjectMsg * pObjectMsg);
	int						OnMsg_YouHitHTB(CObjectMsg * pObjectMsg);
	int						OnMsg_YouHelpMe(CObjectMsg * pObjectMsg);
	int						OnMsg_AllianceYouHelpMe(CObjectMsg * pObjectMsg);

	CControlStateManager*	GetControlStateManager() { return &m_cControlStateManager; }
	bool					PrepareControlStateManager();
	bool					PrepareHelpMeListManager();

	CEscortManager*			GetEscortManager() { return m_pEscortManager; }
	CEscortManager*			CreateEscortManager();


	virtual CObjectController*	CreateObjectController();


	bool				IsPlayScriptEnd() { return m_bPlayScriptEnd; }
	void				SetPlayScriptEnd(bool bEnd) { m_bPlayScriptEnd = bEnd; }
	void				SetPlayScript(TBLIDX playScript, TBLIDX playScriptScene);
	TBLIDX				GetPlayScript() { return bot_profile.sScriptData.playScript; }
	TBLIDX				GetPlayScriptScene() { return bot_profile.sScriptData.playScriptScene; }

	TBLIDX				GetAiScript() { return bot_profile.sScriptData.tblidxAiScript; }
	TBLIDX				GetAiScriptScene() { return bot_profile.sScriptData.tblidxAiScriptScene; }

	TBLIDX				GetBattleScript() { return bot_profile.sScriptData.tblidxBattleScript; }
	DWORD				GetBattleScriptScene() { return bot_profile.sScriptData.dwBattleScriptScene; }

	virtual bool		ConsiderNavLoc(CNtlVector& rDestLoc);
	virtual bool		ConsiderWanderLoc(CNtlVector& rDestLoc);
	virtual bool		ConsiderPatrolLoc(CNtlVector& rDestLoc);
	virtual bool		ConsiderFlee(CNtlVector& rDestLoc);
	virtual bool		ConsiderTurnDir(CNtlVector& rDestDir);

	virtual HOBJECT		ConsiderAttackTarget(DWORD dwTickTime);

	HOBJECT				SelectTarget_AI_Type();

	virtual HOBJECT		ConsiderScanTarget(WORD wRange);

	HOBJECT				ConsiderScanObject(TBLIDX tblidx, WORD wRange);
	HOBJECT				ConsiderScanTargetFixedPc(WORD wRange);
	HOBJECT				ConsiderScanTargetRingRange(WORD wRingRange);

	bool				ConsiderAttackRange(float fBreakupPos);
	virtual float		GetAttackRange(CCharacter* pTarget);

	//virtual bool		ConsiderRespawn();
	virtual bool		ConsiderTargetSightRange();
	virtual bool		ConsiderSpawnRange();

	bool				ConsiderRangedAttack();
	bool				ConsiderRange(float fSkillAttackRange, float fBreakupPos);
	bool				ConsiderRetreatLoc(CNtlVector& rDestLoc, float fOffsetMin, float fOffsetMax);
	bool				ConsiderRetreatFleeLoc(CNtlVector& rDestLoc, float fOffsetMin, float fOffsetMax);

	bool				GetRangedAttackLoc(CNtlVector& rDestLoc);

	BYTE				ConsiderPathMovement(CCharacter* pTarget, int* pnPathFindingResult);

	bool				IsChasingCheatingTarget() { return m_bIsChasingCheatingTarget; }
	CNtlVector&			GetCheatingTargetCollisionLoc() { return m_vCheatingTargetCollisionLoc; }


	void				SetFirstBattleLoc(CNtlVector& vLoc) { m_vecFirstBattleLoc = vLoc ; }
	CNtlVector&			GetFirstBattleLoc() { return m_vecFirstBattleLoc; }

	void				SetFirstBattleDir(CNtlVector& vDir) { m_vecFirstBattleDir.operator=(vDir); }
	CNtlVector&			GetFirstBattleDir() { return m_vecFirstBattleDir; }

	bool				IsRetreat() { return m_bIsRetreat; }
	void				SetRetreat(bool bIsRetreat) { m_bIsRetreat = bIsRetreat; }

	void				SetTimid(eTIMID_TYPE eTimidType, float fTimidRange);
	eTIMID_TYPE			GetTimidType() { return m_eTimidType; }
	float				GetTimidRange() { return m_fTimidRange; }

	CControlScriptEventMap*	GetSpsEventMap() { return &m_cSpsEventMap; }

	sNPC_SPEECH_GROUP*	GetSpeechGroup() { return m_pSpeechGroup; }

	virtual BYTE		GetMaxChainAttack() { return NTL_BATTLE_MAX_NPC_ATTACK_TYPE; }

	virtual bool		AttackProgress(DWORD dwTickDiff, float fMultiple);
	virtual bool		OnAttackAction(CCharacter* pAttacker, int nDmg, BYTE byAttackResult, eFAINT_REASON eReason = FAINT_REASON_HIT);
	virtual bool		OnSkillAction(CCharacter* pAttacker, int nDmg, DWORD dwAggroPoint, BYTE byAttackResult, bool bWakeUpTarget);

	virtual void		ChangeAggro(HOBJECT hProvoker, BYTE byAggroChangeType, DWORD dwValue);

	inline void			SetStandAlone(bool bFlag) { m_bStandAlone = bFlag; }
	inline bool			GetStandAlone() { return m_bStandAlone; }

	inline void			SetEffectiveLevel(BYTE byLv) { m_byEffectiveLevel = byLv; }
	inline BYTE			GetEffectiveLevel() { return m_byEffectiveLevel; }

private:

	bool				ConsiderWanderLoc(CNtlVector& rStandardLoc, CNtlVector& rDestLoc);

	DWORD				m_dwElapsedTimeSinceCheatingTargetSet;
	bool				m_bIsChasingCheatingTarget;
	CNtlVector			m_vCheatingTargetCollisionLoc;

	DWORD				m_dwChangeAttackTargetDelayTime;
	bool				m_bIsChangeTargetWait;

	bool				m_bIsRetreat;

	eTIMID_TYPE			m_eTimidType;
	float				m_fTimidRange;

	CControlScriptEventMap m_cSpsEventMap;

	bool				m_bStandAlone; //true = is not spawned by script

	BYTE				m_byEffectiveLevel;


protected:

	sNPC_SPEECH_GROUP*	m_pSpeechGroup;


protected:

	void				ResetCheatingTargetInfo();
	void				IncreaseChasingCheatingTargetInfo(DWORD dwTickDiff, std::list<CNtlVector>& rlistCollisionPos);

	CNtlVector			m_vecFirstBattleLoc;
	CNtlVector			m_vecFirstBattleDir;


public:

	inline CNtlVector&		GetEnterLoc() { return bot_profile.vSpawnLoc; }
	inline CNtlVector&		GetEnterDir() { return bot_profile.vSpawnDir; }



private:

	DWORD			m_dwLastHasNearbyPlayerQuadTime;
	bool			m_bCacheHasNearbyPlayerQuad;

};



#endif