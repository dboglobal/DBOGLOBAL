#include "precomp_ntlsimulation.h"
#include "NtlFSMUtil.h"

// shared
#include "NtlMovement.h"

//
#include "NtlFSMDef.h"
#include "NtlFSMStateBase.h"
#include "NtlFSMCharActAgent.h"
#include "NtlSLEventFunc.h"

// 이동이 가능한 상태인가?
RwBool FSMUtil_CanMoveState(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase->GetFlags() & NTL_FSMSF_NOT_MOVE)
		return FALSE;

	return TRUE;
}

// 공격이 가능한 상태인가?
RwBool FSMUtil_CanAttackState(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase->GetFlags() & NTL_FSMSF_NOT_ATTACK_CAN)
		return FALSE;

	return TRUE;
}

// 공격을 받을 수 있는 상태인가?
RwBool FSMUtil_CanHurtState(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase->GetFlags() & NTL_FSMSF_NOT_HURT_CAN)
		return FALSE;

	return TRUE;
}

RwBool FSMUtil_CanSkillUse(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase->GetFlags() & NTL_FSMSF_NOT_SKILLUSE)
		return FALSE;

	return TRUE;
}

RwBool FSMUtil_IsIdle(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase == NULL)
		return TRUE;

	if(pStateBase->GetStateId() == NTL_FSMSID_IDLE)
		return TRUE;

	return FALSE;
}

RwBool FSMUtil_IsMove(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase == NULL)
		return TRUE;

	RwUInt32 uiStateId = pStateBase->GetStateId();

	if(uiStateId == NTL_FSMSID_MOVE || uiStateId == NTL_FSMSID_SWIMMING ||
		uiStateId == NTL_FSMSID_JUMP || uiStateId == NTL_FSMSID_AIR_JUMP)
		return TRUE;

	return FALSE;
}

RwBool FSMUtil_IsPrivateShopping(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase == NULL)
		return FALSE;

	if(pStateBase->GetStateId() == NTL_FSMSID_PRIVATESHOP)
		return TRUE;

	return FALSE;
}

RwBool FSMUtil_IsCharging(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase == NULL)
		return FALSE;

	if(pStateBase->GetStateId() == NTL_FSMSID_CHARGING)
		return TRUE;

	return FALSE;
}

RwBool FSMUtil_IsTeleporting(CNtlFSMStateBase *pStateBase)
{
	if(pStateBase == NULL)
		return FALSE;

	if(pStateBase->GetStateId() == NTL_FSMSID_TELEPORT)
		return TRUE;

	return FALSE;
}

RwBool FSMUtil_IsOnBus( CNtlFSMStateBase* pStateBase ) 
{
    if(pStateBase == NULL)
        return FALSE;

    return pStateBase->GetStateId() == NTL_FSMSID_ONBUS;
}

RwBool FSMUtil_IsOnVehicle( CNtlFSMStateBase* pStateBase ) 
{
	if(pStateBase == NULL)
		return FALSE;

	return pStateBase->GetStateId() == NTL_FSMSID_ONVEHICLE;
}

RwBool FSMUtil_IsStandingAdjustMoveClass(CNtlSobActor *pActor)
{
	RwUInt32 uiClassID = pActor->GetClassID();
	if(uiClassID == SLCLASS_MONSTER || uiClassID == SLCLASS_PLAYER)
		return TRUE;

	return FALSE;
}

RwBool FSMUtil_StandingAdjustMove(CNtlFSMStateBase* pStateBase, CNtlSobActor *pActor)
{
	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(pStateBase->GetFSMAgent());
	RwV3d vServerPos = pCharActAgent->GetServerPosition();
	RwV3d vServerDir = pCharActAgent->GetServerDirection();
	RwV3d vPos = pActor->GetPosition();
	
	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vPos, &vServerPos);
	vSub.y = 0.0f;

	RwReal fLen = RwV3dLength(&vSub);

	if(fLen > 0.6f)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
		pMoveStuff->Reset();

		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
		pMoveStuff->byType = NTL_MOVETARGET_LOC;

		RwV3dAssignMacro(&pMoveStuff->vCurrLoc, &vPos);  
		RwV3dAssignMacro(&pMoveStuff->vDest, &vServerPos);  
	
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;

		return TRUE;
	}

	return FALSE;
}