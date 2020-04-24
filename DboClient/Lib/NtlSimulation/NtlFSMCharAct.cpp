#include "precomp_ntlsimulation.h"
#include "NtlFSMCharAct.h"
#include "NtlDebug.h"

#include "NtlFSMCharActState.h"
#include "NtlSobCharProxy.h"
#include "NtlSobActor.h"

DEFINITION_MEMORY_POOL(CNtlFSMCharAct)

CNtlFSMCharAct::CNtlFSMCharAct()
{
}

CNtlFSMCharAct::~CNtlFSMCharAct()
{
}

RwBool CNtlFSMCharAct::Create(void)
{
	NTL_FUNCTION("CNtlFSMCharAct::Create");

	CNtlFSMBase::Create();

	CNtlFSMStateBase *pState;

	// create
	pState = NTL_NEW CNtlFSMCharActCreateState;
	Register(pState);

	// spawn
	pState = NTL_NEW CNtlFSMCharActSpawnState;
	Register(pState);
	
	// idle
	pState = NTL_NEW CNtlFSMCharActIdleState;
	Register(pState);

	// move
	pState = NTL_NEW CNtlFSMCharActMoveState;
	Register(pState);

	// swimming
	pState = NTL_NEW CNtlFSMCharActMoveSwimming;
	Register(pState);

	// adjust move
	pState = NTL_NEW CNtlFSMCharActAdjustMoveState;
	Register(pState);

	// dash move
	pState = NTL_NEW CNtlFSMCharActDashState;
	Register(pState);

	// jump
	pState = NTL_NEW CNtlFSMCharActJumpState;
	Register(pState);

	// falling
	pState = NTL_NEW CNtlFSMCharActFallingState;
	Register(pState);

	// charging
	pState = NTL_NEW CNtlFSMCharActChargingState;
	Register(pState);

	// blocking
	pState = NTL_NEW CNtlFSMCharActBlockingState;
	Register(pState);

	// sit down
	pState = NTL_NEW CNtlFSMCharActSitDownState;
	Register(pState);

	// sit
	pState = NTL_NEW CNtlFSMCharActSitState;
	Register(pState);

	// stand up
	pState = NTL_NEW CNtlFSMCharActStandUpState;
	Register(pState);

	// smooth heading
	pState = NTL_NEW CNtlFSMCharActSmoothHeadingState;
	Register(pState);

	// trigger object operation
	pState = NTL_NEW CNtlFSMCharActTriggerOperateState;
	Register(pState);

	// fighting follow
	pState = NTL_NEW CNtlFSMCharActFollowState;
	Register(pState);

	// fighting follow
	pState = NTL_NEW CNtlFSMCharActFightingFollowState;
	Register(pState);

	// fighting wait
	pState = NTL_NEW CNtlFSMCharActFightingPoseState;
	Register(pState);

	// fighting
	pState = NTL_NEW CNtlFSMCharActFightingState;
	Register(pState);

	pState = NTL_NEW CNtlFSMCharActFightingShrinkState;
	Register(pState);

	// hurt
	pState = NTL_NEW CNtlFSMCharActHurtState;
	Register(pState);

	// knock down
	pState = NTL_NEW CNtlFSMCharActKnockDownState;
	Register(pState);

	// sliding
	pState = NTL_NEW CNtlFSMCharActSlidingState;
	Register(pState);

	// stun
	pState = NTL_NEW CNtlFSMCharActStunState;
	Register(pState);

	// sleep
	pState = NTL_NEW CNtlFSMCharActSleepState;
	Register(pState);

	// paralyze
	pState = NTL_NEW CNtlFSMCharActParalyzeState;
	Register(pState);

	// skill follow
	pState = NTL_NEW CNtlFSMCharActSkillFollowState;
	Register(pState);

	// skill ready
	pState = NTL_NEW CNtlFSMCharActSkillReadyState;
	Register(pState);

	// skill casting
	pState = NTL_NEW CNtlFSMCharActSkillCastingState;
	Register(pState);

	// skill focus
	pState = NTL_NEW CNtlFSMCharActSkillFocusState;
	Register(pState);

	// skill action
	pState = NTL_NEW CNtlFSMCharActSkillActionState;
	Register(pState);

	// social action
	pState = NTL_NEW CNtlFSMCharActSocialActionState;
	Register(pState);

	// htb skill follow
	pState = NTL_NEW CNtlFSMCharActHTBFollowState;
	Register(pState);

	// htb skill ready
	pState = NTL_NEW CNtlFSMCharActHTBReadyState;
	Register(pState);

	// htb skill action
	pState = NTL_NEW CNtlFSMCharActHTBState;
	Register(pState);

	// htb sandbag
	pState = NTL_NEW CNtlFSMCharActHTBSandbagState;
	Register(pState);

	// item ready
	pState = NTL_NEW CNtlFSMCharActItemReadyState;
	Register(pState);

	// item casting
	pState = NTL_NEW CNtlFSMCharActItemCastingState;
	Register(pState);

	// fainting
	pState = NTL_NEW CNtlFSMCharActFaintingState;
	Register(pState);

	// die
	pState = NTL_NEW CNtlFSMCharActDieState;
	Register(pState);

	// teleport
	pState = NTL_NEW CNtlFSMCharActTeleportState;
	Register(pState);

	// private shop
	pState = NTL_NEW CNtlFSMCharActPrivateShopState;
	Register(pState);

	// direction
	pState = NTL_NEW CNtlFSMCharActDirectionState;
	Register(pState);

	// despawn
	pState = NTL_NEW CNtlFSMCharActDespawnState;
	Register(pState);

	// leaving
	pState = NTL_NEW CNtlFSMCharActLeavingState;
	Register(pState);

    // On Bus
    pState = NTL_NEW CNtlFSMCharActOnBus();
    Register(pState);

	// On Vehicle
	pState = NTL_NEW CNtlFSMCharActOnVehicle();
	Register(pState);

    // Bus Turning
    pState = NTL_NEW CNtlFSMCharActTurning();
    Register(pState);

    // 변신 휴유증
    pState = NTL_NEW CNtlFSMCharTransformSequela();
    Register(pState);

    // 회전 어택
    pState = NTL_NEW CNtlFSMCharSpinAttack();
    Register(pState);

	// 회전 어택
	pState = NTL_NEW CNtlFSMCharRollingAttack();
	Register(pState);

    // Pushing
    pState = NTL_NEW CNtlFSMCharPushing();
    Register(pState);

	// Ride on / off vehicle
	pState = NTL_NEW CNtlFSMCharRideOnOff();
	Register(pState);

    // Keeping Effect (Action Loop)
    pState = NTL_NEW CNtlFSMCharKeepingEffect();
    Register(pState);

	// air jump
	pState = NTL_NEW CNtlFSMCharActAirJumpState();
	Register(pState);

	// air accel
	pState = NTL_NEW CNtlFSMCharActAirAccelState();
	Register(pState);

	NTL_RETURN(TRUE);
}

void CNtlFSMCharAct::Destroy(void)
{
	NTL_FUNCTION("CNtlFSMCharAct::Destroy");

	CNtlFSMBase::Destroy();

	NTL_RETURNVOID();
}

CNtlFSMStateBase* CNtlFSMCharAct::Transition( CNtlSobActor *pActor, CNtlFSMStateBase *pOldState, RwUInt32 uiNewStateId ) 
{
    CNtlFSMStateBase *pNewState = CNtlFSMBase::Transition(pActor, pOldState, uiNewStateId);
    if(pNewState && !(pNewState->GetFlags() & NTL_FSMSF_VISUAL_RPBOUNS))
    {
        CNtlSobCharProxy* pProxy = (CNtlSobCharProxy*)pActor->GetSobProxy();
        pProxy->DetachRPBonusEffect();
    }

    return pNewState;
}
