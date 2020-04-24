/*****************************************************************************
*
* File			: NtlFSMUtil.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2006. 11. 28	
* Abstract		: fsm utility
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_FSM_UTIL_H__
#define __NTL_FSM_UTIL_H__

class CNtlFSMStateBase;
class CNtlSobActor;

// 이동이 가능한 상태인가?
RwBool FSMUtil_CanMoveState(CNtlFSMStateBase *pStateBase);

// 공격이 가능한 상태인가?
RwBool FSMUtil_CanAttackState(CNtlFSMStateBase *pStateBase);

// 공격을 받을 수 있는 상태인가?
RwBool FSMUtil_CanHurtState(CNtlFSMStateBase *pStateBase);

// skill을 사용 가능한 상태인가?
RwBool FSMUtil_CanSkillUse(CNtlFSMStateBase *pStateBase);

// idle 상태인가?
RwBool FSMUtil_IsIdle(CNtlFSMStateBase *pStateBase);	

// 이동 상태인가?
RwBool FSMUtil_IsMove(CNtlFSMStateBase *pStateBase);		

// 개인 상점 상태인가?
RwBool FSMUtil_IsPrivateShopping(CNtlFSMStateBase *pStateBase);

// charging 상태인가?
RwBool FSMUtil_IsCharging(CNtlFSMStateBase *pStateBase);

// teleporting 상태인가?
RwBool FSMUtil_IsTeleporting(CNtlFSMStateBase *pStateBase);

// 버스에 탑승 상태인가?
RwBool FSMUtil_IsOnBus(CNtlFSMStateBase* pStateBase);

// 탈 것에 탑승 상태인가?
RwBool FSMUtil_IsOnVehicle(CNtlFSMStateBase* pStateBase);



// 
RwBool FSMUtil_IsStandingAdjustMoveClass(CNtlSobActor *pActor);

RwBool FSMUtil_StandingAdjustMove(CNtlFSMStateBase* pStateBase, CNtlSobActor *pActor);

#endif
