/*****************************************************************************
 *
 * File			: NtlFSMCharActAgent.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: character act agent class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_CHARACT_AGENT_H__
#define __NTL_FSM_CHARACT_AGENT_H__

// shared
#include "NtlCharacter.h"

// simulation
#include "NtlFSMAgent.h"
#include "NtlSobActor.h"
#include "NtlSobCharProxy.h"


struct SNtlEventSobServerUpdateState;
struct SNtlEventSobServerUpdateAspect;
struct SNtlEventSobVehicleEngine;
class CNtlSobProxySystemEffect;
class CNtlPLEntity;

union UCharActModeFlag
{
	struct 
	{
		RwUInt32 bitSitDown : 1;			// true면 앉기 , false면 일어서기.
		RwUInt32 bitCharging : 1;			// true이면 charging
		RwUInt32 bitFightingMode : 1;		// 공격 모드.
		RwUInt32 bitClickDisable : 1;		// 클릭 금지.
		RwUInt32 bitClientUIDisable : 1;	// Prohibit client UI output.
		RwUInt32 bitAttackDisallow : 1;		// 공격 불가 모드.
		RwUInt32 bitDirectMode : 1;			// 연출 상태인가?
		RwUInt32 bitAutoRun : 1;			// auto run인가?
        RwUInt32 bitSequela : 1;            // 변신 휴유증 상태인가?
        RwUInt32 bitTransparent : 1;        // 투명 상태인가? (GM용)
        RwUInt32 bitInvisible : 1;          // 투명 상태인가? (몹 스킬)
        RwUInt32 bitHidingKi : 1;           // 기 숨기기 상태인가?
        RwUInt32 bitEmergency : 1;          // 빈사 상태 인가?
		RwUInt32 bitAirState : 1;
	};

	RwUInt32	uiActMode;
};

class CNtlFSMCharActAgent : public CNtlFSMAgent
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActAgent, NTL_DEFAULT_MEMORY_POOL)

protected:

	sCHARSTATE						m_sCharState;

	BYTE							m_byServerState;
	RwUInt64						m_ulServerCondition;
	RwV3d							m_vServerPos;
	RwV3d							m_vServerDir;
	RwBool							m_bActiveAdjustPosition;

	UCharActModeFlag				m_uActMode;   

	CNtlSobProxySystemEffect		*m_pLPDangerColor;
	CNtlSobProxySystemEffect		*m_pSystemEffectColor;
    CNtlSobProxySystemEffect*       m_pSystemInvisible;
    CNtlSobProxySystemEffect*       m_pSystemGMAlpha;
    CNtlSobProxySystemEffect*       m_pSystemHidingKi;          ///< 기 숨기기용 알파 이펙트	

    // 컨디션 관련 이펙트
    CNtlPLEntity					*m_pConditionTaunt;
    CNtlPLEntity					*m_pConditionConfuse;
    CNtlPLEntity					*m_pConditionTerror;  
	CNtlPLEntity* m_pConditionBleed;
	CNtlPLEntity* m_pConditionPoison;
	CNtlPLEntity* m_pConditionStomach;

protected:

	// simulation 객체의 서버 상태가 변환때 이벤트로 변환.
	virtual void ServerStateUpdateAnalysis(SNtlEventSobServerUpdateState *pServerState);
	virtual void ServerConditionUpdateAnalysis(RwUInt8 byServerState, RwUInt64 uiCondition);
    virtual void ServerAspectUpdateAnalysis(sASPECTSTATE& sAspectState, RwBool bCreateUpdate);

    // Simulation 객체가 Update 되면서 받는 상태 변환 함수들
	void ServerStateSpawn(sCHARSTATE *pServerState);
	void ServerStateStanding(sCHARSTATE *pServerState);
	void ServerStateCharging(sCHARSTATE *pServerState);
	void ServerStateBlocking(sCHARSTATE *pServerState);
	void ServerStateSitting(sCHARSTATE *pServerState);
	void ServerStateMoving(sCHARSTATE *pServerState);
	void ServerStateDestMove(sCHARSTATE *pServerState);
	void ServerStateFollowing(sCHARSTATE *pServerState);
	void ServerStateFalling(sCHARSTATE* pServerState);
	void ServerStateDash(sCHARSTATE *pServerState);
	void ServerStateTeleporting(sCHARSTATE *pServerState);
	void ServerStateSliding(sCHARSTATE *pServerState);
	void ServerStateKnockDown(sCHARSTATE *pServerState);
	void ServerStateStun(sCHARSTATE *pServerState);
	void ServerStateSleep(sCHARSTATE *pServerState);
	void ServerStateParalyze(sCHARSTATE *pServerState);
	void ServerStateFocusing(sCHARSTATE *pServerState);
	void ServerStateCasting(sCHARSTATE *pServerState);
	void ServerStateHTB(sCHARSTATE *pServerState);
	void ServerStateHTBSandbag(sCHARSTATE *pServerState);
	void ServerStateItemCasting(sCHARSTATE *pServerState);
	void ServerStateFainting(sCHARSTATE *pServerState);
	void ServerStatePrivateShop(sCHARSTATE *pServerState);
	void ServerStateDirection(sCHARSTATE *pServerState);
	void ServerStateOperation(sCHARSTATE *pServerState);
	void ServerStateDespawn(sCHARSTATE *pServerState);
	void ServerStateCamping(sCHARSTATE *pServerState);
	void ServerStateLeaving(sCHARSTATE *pServerState);
    void ServerStateRideOn(sCHARSTATE *pServerState);
    void ServerStateTurning(sCHARSTATE *pServerState);
    void ServerStateKeepingEffect(sCHARSTATE* pServerState);                    ///< 스킬 Action LOOP
	void ServerStateAirJump(sCHARSTATE* pServerState);
	void ServerStateAirDashAccel(sCHARSTATE* pServerState);

	// simulation 객체가 처음 생길때 서버 상태에 따른 변환.
	virtual void ServerStateCreateAnalysis(sCHARSTATE *pCharState);

	void ServerStateCreateStanding(sCHARSTATE *pServerState);
	void ServerStateCreateCharging(sCHARSTATE *pServerState);
	void ServerStateCreateBlocking(sCHARSTATE *pServerState);
	void ServerStateCreateSitting(sCHARSTATE *pServerState);
	void ServerStateCreateMoving(sCHARSTATE *pServerState);
	void ServerStateCreateDestMove(sCHARSTATE *pServerState);
	void ServerStateCreateDash(sCHARSTATE *pServerState);
	void ServerStateCreateFollowing(sCHARSTATE *pServerState);
	void ServerStateCreateFalling(sCHARSTATE *pServerState);	
	void ServerStateCreateTeleporting(sCHARSTATE *pServerState);
	void ServerStateCreateSliding(sCHARSTATE *pServerState);
	void ServerStateCreateKnockDown(sCHARSTATE *pServerState);
	void ServerStateCreateStun(sCHARSTATE *pServerState);
	void ServerStateCreateSleep(sCHARSTATE *pServerState);
	void ServerStateCreateParalyze(sCHARSTATE *pServerState);
	void ServerStateCreateFocusing(sCHARSTATE *pServerState);
	void ServerStateCreateCasting(sCHARSTATE *pServerState);
	void ServerStateCreateSkillAffecting(sCHARSTATE *pServerState);
	void ServerStateCreateHTB(sCHARSTATE *pServerState);
	void ServerStateCreateHTBSandbag(sCHARSTATE *pServerState);
	void ServerStateCreateItemCasting(sCHARSTATE *pServerState);
	void ServerStateCreateFainting(sCHARSTATE *pServerState);
	void ServerStateCreatePrivateShop(sCHARSTATE *pServerState);
	void ServerStateCreateDirection(sCHARSTATE *pServerState);
	void ServerStateCreateOperation(sCHARSTATE *pServerState);
	void ServerStateCreateDespawn(sCHARSTATE *pServerState);
	void ServerStateCreateCamping(sCHARSTATE *pServerState);
	void ServerStateCreateLeaving(sCHARSTATE *pServerState);
    void ServerStateCreateRideOn(sCHARSTATE *pServerState);
    void ServerStateCreateTurning(sCHARSTATE *pServerState);
	void ServerStateCreateAirJump(sCHARSTATE *pServerState);
	void ServerStateCreateAirAccel(sCHARSTATE *pServerState);

	// simulation 객체의 Aspect 상태 변화에 따른 변환.
	void ServerAspectCreateStateVehicle(sASPECTSTATE *pServerAspectState);
	void ServerAspectStateVehicle(sASPECTSTATE *pServerAspectState);

	// simulation 객체의 다음 상태에 대한 처리 함수.
	RwUInt32 AcquireFightingProc(void);
	RwUInt32 AcquireSkillActionProc(void);
	RwUInt32 AcquireHTBProc(void);
	RwUInt32 AcquireSandbagProc(void);
	RwUInt32 AcquireJumpProc(void);
	
public:

	CNtlFSMCharActAgent();
	~CNtlFSMCharActAgent();
	
	virtual void			Update(RwReal fElapsed);
	virtual RwUInt32		HandleEvents(RWS::CMsg &pMsg);	
	virtual RwUInt32		AcquireNextState(RwUInt32 uiCurrStateId);      ///< Next State를 결정한다.

	virtual void			NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState);
	virtual void			NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2); ///< event notify function.

	const sCHARSTATE*		GetServerFullState(void) const;
	RwUInt8					GetServerState(void) const;
    RwUInt64                GetServerCondition() { return m_ulServerCondition;}
	RwV3d					GetServerPosition(void)	{ return m_vServerPos; }
	RwV3d					GetServerDirection(void) { return m_vServerDir; }	
	
	void					SetActiveAdjustPosition(RwBool bActive);
	RwBool					IsActiveAdjustPosition(void);

    //-- 상태 State 설정/반환 함수들
	void					SetFightingMode(RwBool bFightingMode);
	RwBool					IsFightingMode(void);

	void					SetAirMode(BYTE byAirState);
	RwBool					IsAirMode(void);

	void					SetSitDown(RwBool bSitDown);
	RwBool					IsSitDown(void);

	void					SetCharging(RwBool bCharging);
	RwBool					IsCharging(void);

	void					SetClickDisable(RwBool bClickDisable);
	RwBool					IsClickDisable(void);

	void					SetClientUIDisable(RwBool bClientUIDisable);
	RwBool					IsClientUIDisable(void);

	void					SetAttackDisallow(RwBool bAttackDisallow);
	RwBool					IsAttackDisallow(void);

	void					SetDirectMode(RwBool bDirectMode);
	RwBool					IsDirectMode(void);

	void					SetAutoRun(RwBool bAutoRun);
	RwBool					IsAutoRun(void);

	RwBool					IsDie(void);

    void                    SetTransformSequela(RwBool bSequela);
    RwBool                  IsTransformSequela();

    void                    SetTransparent(RwBool bTransparent);
    RwBool                  IsTransparent();

    void                    SetInvisible(RwBool bInvisible);
    RwBool                  IsInvisible();

    void                    SetHidingKi(RwBool bHidingKi);
    RwBool                  IsHidingKi();

    void                    SetEmergency(RwBool bEmergency);            ///< 빈사 상태인지 설정한다
    RwBool                  IsEmergency();                              ///< 빈사 상태인지 반환한다.

    CNtlPLEntity*			CreateVisualSystemEffect(const RwChar *pKey);

	//-- condition 처리
	
	void					ConditionVisualColorFlickering(RwUInt64 uiServerCondition);
	void					ConditionVisualColorFlickeringPop(void);
	void					ConditionDiff(RwUInt64 uiServerCondition);
    void                    ConditionTaunt(RwBool bAffect);                          ///< 도발    
    void                    ConditionAfterEffect(RwBool bAffect);                    ///< 변신 휴유증 처리
    void                    ConditionConfused(RwBool bAffect);                       ///< 혼란 처리
    void                    ConditionTerror(RwBool bAffect);                         ///< 공포 처리
    void                    ConditionTransparent(RwBool bAffect);                    ///< 캐릭터 투명 처리 (GM용)
    void                    ConditionInvisible(RwBool bAffect);                      ///< 숨기 (몹용)
    void                    ConditionHidingKi(RwBool bAffect);                       ///< 기 숨기기
	void                    ConditionBleed(RwBool bAffect);
	void                    ConditionPoison(RwBool bAffect);
	void                    ConditionStomach(RwBool bAffect);
};


inline const sCHARSTATE* CNtlFSMCharActAgent::GetServerFullState(void) const
{
	return &m_sCharState;
}

inline RwUInt8 CNtlFSMCharActAgent::GetServerState(void) const
{
	return m_byServerState;
}

inline void	CNtlFSMCharActAgent::SetActiveAdjustPosition(RwBool bActive)
{
	m_bActiveAdjustPosition = bActive;
}

inline RwBool CNtlFSMCharActAgent::IsActiveAdjustPosition(void)
{
	return m_bActiveAdjustPosition; 
}

inline void CNtlFSMCharActAgent::SetFightingMode(RwBool bFightingMode)
{
	m_uActMode.bitFightingMode = bFightingMode;
}

inline RwBool CNtlFSMCharActAgent::IsFightingMode(void)
{
	return m_uActMode.bitFightingMode;
}

inline void CNtlFSMCharActAgent::SetAirMode(BYTE byAirState)
{
	m_uActMode.bitAirState = byAirState;
}

inline RwBool CNtlFSMCharActAgent::IsAirMode(void)
{
	return m_uActMode.bitAirState;
}

inline void CNtlFSMCharActAgent::SetSitDown(RwBool bSitDown)
{
	m_uActMode.bitSitDown = bSitDown;
}

inline RwBool CNtlFSMCharActAgent::IsSitDown(void)
{
	return m_uActMode.bitSitDown;
}

inline void CNtlFSMCharActAgent::SetCharging(RwBool bCharging)
{
	m_uActMode.bitCharging = bCharging;
}

inline RwBool CNtlFSMCharActAgent::IsCharging(void)
{
	return m_uActMode.bitCharging;
}

inline void CNtlFSMCharActAgent::SetClickDisable(RwBool bClickDisable)
{
	m_uActMode.bitClickDisable = bClickDisable;
}

inline RwBool CNtlFSMCharActAgent::IsClickDisable(void)
{
	return m_uActMode.bitClickDisable;
}

inline void CNtlFSMCharActAgent::SetClientUIDisable(RwBool bClientUIDisable)
{
	m_uActMode.bitClientUIDisable = bClientUIDisable;
}

inline RwBool CNtlFSMCharActAgent::IsClientUIDisable(void)
{
	return m_uActMode.bitClientUIDisable;
}

inline void CNtlFSMCharActAgent::SetAttackDisallow(RwBool bAttackDisallow)
{
	m_uActMode.bitAttackDisallow = bAttackDisallow;
}

inline RwBool CNtlFSMCharActAgent::IsAttackDisallow(void)
{
	return m_uActMode.bitAttackDisallow;
}

inline void CNtlFSMCharActAgent::SetDirectMode(RwBool bDirectMode)
{
	m_uActMode.bitDirectMode = bDirectMode;
}

inline RwBool CNtlFSMCharActAgent::IsDirectMode(void)
{
	return m_uActMode.bitDirectMode;
}

inline void	CNtlFSMCharActAgent::SetAutoRun(RwBool bAutoRun)
{
	m_uActMode.bitAutoRun = bAutoRun;
}

inline RwBool CNtlFSMCharActAgent::IsAutoRun(void)
{
	return m_uActMode.bitAutoRun;
}


inline RwBool CNtlFSMCharActAgent::IsTransformSequela()
{
    return m_uActMode.bitSequela;
}

inline void CNtlFSMCharActAgent::SetTransparent(RwBool bTransparent)
{
    m_uActMode.bitTransparent = bTransparent;
}

inline RwBool CNtlFSMCharActAgent::IsTransparent()
{
    return m_uActMode.bitTransparent;
}

inline void CNtlFSMCharActAgent::SetInvisible(RwBool bInvisible)
{
    m_uActMode.bitInvisible = bInvisible;
}

inline RwBool CNtlFSMCharActAgent::IsInvisible()
{
    return m_uActMode.bitInvisible;
}

inline void CNtlFSMCharActAgent::SetHidingKi(RwBool bHidingKi)
{
    m_uActMode.bitHidingKi = bHidingKi;
}

inline RwBool CNtlFSMCharActAgent::IsHidingKi()
{
    return m_uActMode.bitHidingKi;
}

inline void CNtlFSMCharActAgent::SetEmergency(RwBool bEmergency)
{
    m_uActMode.bitEmergency = bEmergency;
}

inline RwBool CNtlFSMCharActAgent::IsEmergency()
{
    return m_uActMode.bitEmergency;
}

#endif
