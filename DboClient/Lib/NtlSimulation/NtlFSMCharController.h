/*****************************************************************************
 *
 * File			: NtlFSMCharController.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 17	
 * Abstract		: FSM character controller class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_CHARCONTROLLER_H__
#define __NTL_FSM_CHARCONTROLLER_H__

#include "NtlSLEvent.h"
#include "NtlFSMController.h"

/// character controller util
class CNtlFSMSyncMsgAttackBegin
{
private:

	RwBool			m_bAttackBegin;
	RwReal			m_fBeginAndEndTime;
	SERIAL_HANDLE	m_hTargetSerialId;
	RwUInt8			m_byAvatarControlId;

public:

	CNtlFSMSyncMsgAttackBegin();
	~CNtlFSMSyncMsgAttackBegin();

	void Update(CNtlSobActor *pActor, RwReal fElapsed);

	void			SetTargetSerialId(SERIAL_HANDLE hTargetSerialId);
	SERIAL_HANDLE	GetTargetSerialId(void) const;

	void			SetAttackBeginFlag(RwBool bFlag);
	RwBool			GetAttackBeginFlag(void) const;

	void			SetAvatarControlId(RwUInt8 byControlId);
	RwUInt8			GetAvatarControlId(void) const;
};


inline SERIAL_HANDLE CNtlFSMSyncMsgAttackBegin::GetTargetSerialId(void) const
{
	return m_hTargetSerialId;
}

inline void	CNtlFSMSyncMsgAttackBegin::SetAttackBeginFlag(RwBool bFlag)
{
	m_bAttackBegin = bFlag;
}

inline RwBool CNtlFSMSyncMsgAttackBegin::GetAttackBeginFlag(void) const
{
	return m_bAttackBegin;
}

inline void	CNtlFSMSyncMsgAttackBegin::SetAvatarControlId(RwUInt8 byControlId)
{
	m_byAvatarControlId = byControlId;
}

inline RwUInt8 CNtlFSMSyncMsgAttackBegin::GetAvatarControlId(void) const
{
	return m_byAvatarControlId;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMAutoCmdAI
{
protected:

	CNtlFSMAutoCmdAI() {}

public:

	virtual ~CNtlFSMAutoCmdAI() {}

	virtual void	Update(RwReal fElapsed) {}
	virtual RwBool	IsFinish(void)	{ return TRUE; }
};


class CNtlFSMAutoFollowCmdAI : public CNtlFSMAutoCmdAI
{
private:

	enum EAutoFollowState
	{
		EAFS_IDLE,
		EAFS_MOVE
	};

	RwReal				m_fThinkTime;
	EAutoFollowState	m_eState;
	RwBool				m_bFinish;

	CNtlSobActor		*m_pActor;
	CNtlSobActor		*m_pAliveTarActor;	

private:

	RwBool	IsAliveTarget(void);
	RwBool	IsRoundInTarget(void);

	void	UpdateIdle(RwReal fElapsed);
	void	UpdateMove(RwReal fElapsed);

public:

	CNtlFSMAutoFollowCmdAI(CNtlSobActor *pActor);
	~CNtlFSMAutoFollowCmdAI();

	virtual void	Update(RwReal fElapsed);
	virtual RwBool	IsFinish(void);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMPlayerController : public CNtlFSMController
{
	DECLEAR_MEMORY_POOL(CNtlFSMPlayerController, NTL_DEFAULT_MEMORY_POOL)

protected:

	CNtlFSMAutoCmdAI			*m_pAutoCmdAI;
	CNtlFSMSyncMsgAttackBegin	*m_pSyncMsgAttacKBegin;

	RwBool m_bAttackRangeCheck;
	RwReal m_fRangeCheckTime;

	// keyboard move
	RwUInt8 m_byKeyboardMoveFlags;

	typedef RwUInt32 (CNtlFSMPlayerController::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	MapFuncEventHandler m_mapEventFunc;

	typedef void (CNtlFSMPlayerController::*NotifyFuncEventHander)(RWS::CMsg &pMsg, RwUInt32, RwUInt32, RwUInt32);
	typedef std::map<const RWS::CRegisteredMsgs*, NotifyFuncEventHander> MapFuncNotifyEventHandler;
	MapFuncNotifyEventHandler m_mapNotifyEventFunc;

private:

	void CreateAutoCmdAI(void);
	void ReleaseAutoCmdAI(void);

	RwUInt32 ServerStateUpdateAnalysis(SNtlEventSobServerUpdateState *pServerState);

	void UpdateAutoCmdAI(RwReal fElapsed);
	void UpdateFollowCheck(RwReal fElapsed);

	/**
	/* event handler
	*/

	virtual RwUInt32 SobCreateEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 SobServerUpdateStateEventHandler(RWS::CMsg &pMsg);

	virtual RwUInt32 ActionMapHaltEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapSitAndStandEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapJumpEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapAirJumpEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapSkillUseEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapHTBUseEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapItemUseEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapSocialUseEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapPrivateShopOpenEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapChargingEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 ActionMapAutoFollowEventHandler(RWS::CMsg &pMsg);

	virtual RwUInt32 TerrainClickEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 KeyboardMoveEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 KeyboardMoveStopEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 MouseDashMoveEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 KeyboardDashMoveEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 AirDashMoveEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 AirAccelMoveEventHandler(RWS::CMsg& pMsg);
	virtual RwUInt32 InputChangeHeadingEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 SobTargetSelectEventHandler(RWS::CMsg &pMsg);
	virtual RwUInt32 SobAttackSelectEventHandler(RWS::CMsg &pMsg);
    virtual RwUInt32 SobSendAttackEndHandler(RWS::CMsg& pMsg);
	virtual RwUInt32 SobFaintingEventHandler(RWS::CMsg &pMsg);
	
	/**
	/* Notify update state function                                                                     
	*/

	// notify update state change(old)
	virtual void NotifyOldUpdateStateProc(RwUInt32 uiOldState, RwUInt32 uiNextState);

	//notify update state change(next)
	virtual void NotifyNextUpdateStateSpawnProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateIdleProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateHurtProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateSkillReadyProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateHTBReadyProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateChaseFollowProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateFightingFollowProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateSkillFollowProc(RwUInt32 uiOldState);
	virtual void NotifyNextUpdateStateFallingProc(RwUInt32 uiOldState);

	// attack range check
	virtual void NotifyUpdateStateAttackRangeCheck(RwUInt32 uiOldState, RwUInt32 uiNextState);
	
	/**
	/* Notify event handler
	*/

	// notify event handler
	virtual void NotifyActionMapJumpEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapAirJumpEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapChargingEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapBlockingEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapSitAndDownEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapSkillUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapSocialUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapPrivateShopOpenEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapHTBUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapItemUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyActionMapAutoFollowEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);    

	virtual void NotifyInputChangeHeadingEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifySobSkillCancelEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyTerrainClickEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyKeyboardMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyKeyboardMoveStopEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyMouseDashMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyKeyboardDashMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyAirDashMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifyAirAccelMoveEventHandler(RWS::CMsg& pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifySobTargetSelectEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifySobAttackSelectEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifySobFollowMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifySobHitEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifySobSpecialAttackEventHandler(RWS::CMsg& pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	virtual void NotifySobAnimPlayEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
	
public:

	CNtlFSMPlayerController();
	~CNtlFSMPlayerController();

	virtual void     Update(RwReal fElapsed);	
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void     NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState);
	virtual void     NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);

public:

	CNtlFSMSyncMsgAttackBegin* GetFSMSyncMsgAttackBegin(void) const;
};

inline CNtlFSMSyncMsgAttackBegin* CNtlFSMPlayerController::GetFSMSyncMsgAttackBegin(void) const
{
	return m_pSyncMsgAttacKBegin;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMPlayerSyncController : public CNtlFSMController
{
	DECLEAR_MEMORY_POOL(CNtlFSMPlayerSyncController, NTL_DEFAULT_MEMORY_POOL)

public:

	CNtlFSMPlayerSyncController();

	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState);
	virtual void NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
};



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMServerController : public CNtlFSMController
{
	DECLEAR_MEMORY_POOL(CNtlFSMServerController, NTL_DEFAULT_MEMORY_POOL)

private:

	RwReal m_fRangeCheckTime;

	// update server state
	void UpdateServerFollowState(RwReal fElapsed);

public:

	CNtlFSMServerController();

	virtual void Update(RwReal fElapsed);	
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState);
	virtual void NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2);
};

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMStateBase;

class CNtlSummonPetAI
{
public:

	enum ESummonPetState
	{
		ESPS_IDLE,
		ESPS_POSITION_SET,
		ESPS_OBSERVATION,
		ESPS_COLLISION_AVOID,
		ESPS_TARGET_FOLLOW,
		ESPS_BATTLE,
		ESPS_BATTLE_POSITION_SET,
		ESPS_DIE
	};

private:

	ESummonPetState		m_eState;
	CNtlFSMController	*m_pController;
	RwReal				m_fStateTimer;
	RwReal				m_fThinkingTimer;
	RwReal				m_fPathTimer;
	RwReal				m_fSkillTimer[NTL_MAX_NPC_HAVE_SKILL];			// Skill Timer (AI로 스킬을 판단하는 시간)

	RwBool				m_bCompulsoryAttack;
	SERIAL_HANDLE		m_hBattleSerialId;

	std::vector<RwInt32> m_vecUseSlotIdx;
		
private:

	void				UpdateIdle(RwReal fElapsed);
	void				UpdatePositionSet(RwReal fElapsed);
	void				UpdateObservation(RwReal fElapsed);
	void				UpdateCollisionAvoid(RwReal fElapsed);
	void				UpdateTargetFollow(RwReal fElapsed);
	void				UpdateBattle(RwReal fElapsed);
	void				UpdateBattlePositionSet(RwReal fElapsed);
	void				UpdateDie(RwReal fElapsed);

	void				EnterState(ESummonPetState eState);
	void				ExitState(ESummonPetState eState);

	void				ResetStateTimer(void);
	void				ResetThinkingTimer(void);
	void				ResetPathTimer(void);
	void				ResetSkillTimer(RwInt32 iSlotIdx);

	RwBool				IsThinking(void);
	CNtlFSMStateBase*	GetActorState(void);
	RwUInt32			GetActorStateId(void);
	CNtlFSMStateBase*	GetOwnerActorState(void);
	RwUInt32			GetOwnerActorStateId(void);
	SERIAL_HANDLE		GetOwnerTargetSerial(void);
	RwBool				IsRoundInOwner(RwReal& fDist);
	RwReal				GetDistanceOwner(void);
	RwBool				IsOwnerFighting(void);
	
	void				Wander(void);
	void				JumpPosition(void);
	SERIAL_HANDLE		SearchFightingTarget(void);
	RwBool				IsKeepBattle(void);
	RwBool				BattleWander(void);
	RwBool				SkillAction(void);
	RwBool				SkillAction(RwInt32 iSlotIdx);
	RwBool				IsThinkingUseSkill(RwInt32 iSlotIdx);

	RwBool				IsPathCollision(RwV3d vStart, RwV3d vDir, RwReal fLen);
	RwV3d				GetPathFinding(void);
		
private:

	void				EventGeneratorCollisionAvoid(void);
	void				EventGeneratorFollowState(void);
	void				EventGeneratorAttackFollowState(void);

public:

	CNtlSummonPetAI();
	~CNtlSummonPetAI();

	void				Create(CNtlFSMController *pController);
	void				Destroy(void);

	void				Update(RwReal fElapsed);

	void				ChangeState(ESummonPetState eState, RwBool bEqualStateChange = FALSE);

	CNtlSobActor*		GetActor(void);

	void				UserCommandAttack(SERIAL_HANDLE hTargetSerialId);
};

class CNtlFSMSummonPetController : public CNtlFSMPlayerController
{
	DECLEAR_MEMORY_POOL(CNtlFSMSummonPetController, NTL_DEFAULT_MEMORY_POOL)

private:

	CNtlSummonPetAI *m_pPetAI;

public:

	CNtlFSMSummonPetController();
	~CNtlFSMSummonPetController();

	virtual void		Update(RwReal fElapsed);
	virtual RwUInt32	HandleEvents(RWS::CMsg &pMsg);
	virtual void		NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState);
};


#endif
