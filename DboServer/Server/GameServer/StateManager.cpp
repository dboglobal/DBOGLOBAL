#include "stdafx.h"
#include "StateManager.h"
#include "char.h"
#include "NtlPacketGU.h"
#include "BotState_AirDashAccel.h"
#include "BotState_AirJump.h"
#include "BotState_Camping.h"
#include "BotState_Casting.h"
#include "BotState_CastingItem.h"
#include "BotState_Charging.h"
#include "BotState_DashPassive.h"
#include "BotState_Despawning.h"
#include "BotState_DestMove.h"
#include "BotState_DirectPlay.h"
#include "BotState_Fainting.h"
#include "BotState_Falling.h"
#include "BotState_Focusing.h"
#include "BotState_Follow.h"
#include "BotState_Guard.h"
#include "BotState_HTB.h"
#include "BotState_KeepingEffect.h"
#include "BotState_Knockdown.h"
#include "BotState_Leaving.h"
#include "BotState_Moving.h"
#include "BotState_Operating.h"
#include "BotState_Paralyzed.h"
#include "BotState_PrivateShop.h"
#include "BotState_RideOn.h"
#include "BotState_Sandbag.h"
#include "BotState_Sitting.h"
#include "BotState_SkillAffecting.h"
#include "BotState_Sleeping.h"
#include "BotState_Sliding.h"
#include "BotState_Spawning.h"
#include "BotState_Standing.h"
#include "BotState_Stunned.h"
#include "BotState_Teleporting.h"
#include "BotState_Turning.h"
#include "AspectState_GreatNamek.h"
#include "AspectState_Kaioken.h"
#include "AspectState_PureMajin.h"
#include "AspectState_RollingAttack.h"
#include "AspectState_SpinningAttack.h"
#include "AspectState_SuperSaiyan.h"
#include "AspectState_Vehicle.h"
#include "ConditionState_Invisible.h"
#include "ConditionState_HidingKi.h"
#include "ConditionState_Invincible.h"
#include "ConditionState_Taunt.h"
#include "ConditionState_AttackDisallow.h"
#include "ConditionState_Transparent.h"
#include "ConditionState_CantBeTargetted.h"
#include "ConditionState_DirectPlay.h"
#include "ConditionState_Bleeding.h"
#include "ConditionState_Poison.h"
#include "ConditionState_Stomachache.h"
#include "ConditionState_Burn.h"
#include "ConditionState_Confused.h"
#include "ConditionState_Terror.h"
#include "ConditionState_Barrier.h"
#include "ConditionState_DamageReflection.h"
#include "ConditionState_AfterEffect.h"
#include "ConditionState_ChargingBlocked.h"
#include "ConditionState_FakeDeath.h"
#include "ConditionState_NullifiedDamage.h"
#include "ConditionState_ClickDisable.h"
#include "ConditionState_ClientUiDisable.h"
#include "ConditionState_TaiyouKen.h"
#include "ConditionState_BattleInability.h"
#include "ConditionState_SkillInability.h"
#include "ConditionState_RevivalAftereffect.h"
#include "ConditionState_LpAutoRecover.h"
#include "ConditionState_EpAutoRecover.h"
#include "ConditionState_Rabies.h"
#include "ConditionState_Drunk.h"
#include "ConditionState_ExcitationMale.h"
#include "ConditionState_ExcitationFemale.h"



CStateManager::CStateManager()
{
	Init();
}

CStateManager::~CStateManager()
{
	Destroy();
}


bool CStateManager::Create(CCharacter* pParentRef, eAIR_STATE eAirState)
{
	if (pParentRef)
	{
			////// STATES

			CBotState_Spawning* pSpawn = new CBotState_Spawning(pParentRef);
			RegisterState(pSpawn);

			CBotState_Standing* pStanding = new CBotState_Standing(pParentRef);
			RegisterState(pStanding);

			CBotState_Sitting* pSitting = new CBotState_Sitting(pParentRef);
			RegisterState(pSitting);

			CBotState_Fainting* pFainting = new CBotState_Fainting(pParentRef);
			RegisterState(pFainting);

			CBotState_Camping* pCamping = new CBotState_Camping(pParentRef);
			RegisterState(pCamping);

			CBotState_Moving* pMoving = new CBotState_Moving(pParentRef);
			RegisterState(pMoving);

			CBotState_DestMove* pDestMove = new CBotState_DestMove(pParentRef);
			RegisterState(pDestMove);

			CBotState_Follow* pFollow = new CBotState_Follow(pParentRef);
			RegisterState(pFollow);

			CBotState_Falling* pFalling = new CBotState_Falling(pParentRef);
			RegisterState(pFalling);

			CBotState_DashPassive* pDashPassive = new CBotState_DashPassive(pParentRef);
			RegisterState(pDashPassive);

			CBotState_Teleporting* pTeleporting = new CBotState_Teleporting(pParentRef);
			RegisterState(pTeleporting);

			CBotState_Sliding* pSliding = new CBotState_Sliding(pParentRef);
			RegisterState(pSliding);

			CBotState_Knockdown* pKnockdown = new CBotState_Knockdown(pParentRef);
			RegisterState(pKnockdown);

			CBotState_Focusing* pFocusing = new CBotState_Focusing(pParentRef);
			RegisterState(pFocusing);

			CBotState_Casting* pCasting = new CBotState_Casting(pParentRef);
			RegisterState(pCasting);

			CBotState_SkillAffecting* pSkillAffecting = new CBotState_SkillAffecting(pParentRef);
			RegisterState(pSkillAffecting);

			CBotState_KeepingEffect* pKeepingEffect = new CBotState_KeepingEffect(pParentRef);
			RegisterState(pKeepingEffect);

			CBotState_CastingItem* pCastingItem = new CBotState_CastingItem(pParentRef);
			RegisterState(pCastingItem);

			CBotState_Stunned* pStunned = new CBotState_Stunned(pParentRef);
			RegisterState(pStunned);

			CBotState_Sleeping* pSleeping = new CBotState_Sleeping(pParentRef);
			RegisterState(pSleeping);

			CBotState_Paralyzed* pParalyzed = new CBotState_Paralyzed(pParentRef);
			RegisterState(pParalyzed);

			CBotState_HTB* pHTB = new CBotState_HTB(pParentRef);
			RegisterState(pHTB);

			CBotState_Sandbag* pSandBag = new CBotState_Sandbag(pParentRef);
			RegisterState(pSandBag);

			CBotState_Charging* pCharging = new CBotState_Charging(pParentRef);
			RegisterState(pCharging);

			CBotState_Guard* pGuard = new CBotState_Guard(pParentRef);
			RegisterState(pGuard);

			CBotState_Despawning* pDespawning = new CBotState_Despawning(pParentRef);
			RegisterState(pDespawning);

			CBotState_PrivateShop* pPrivateShop = new CBotState_PrivateShop(pParentRef);
			RegisterState(pPrivateShop);

			CBotState_Leaving* pLeaving = new CBotState_Leaving(pParentRef);
			RegisterState(pLeaving);

			CBotState_DirectPlay* pDirectPlay = new CBotState_DirectPlay(pParentRef);
			RegisterState(pDirectPlay);

			CBotState_Operating* pOperating = new CBotState_Operating(pParentRef);
			RegisterState(pOperating);

			CBotState_RideOn* pRideOn = new CBotState_RideOn(pParentRef);
			RegisterState(pRideOn);

			CBotState_Turning* pTurning = new CBotState_Turning(pParentRef);
			RegisterState(pTurning);

			CBotState_AirJump* pAirJump = new CBotState_AirJump(pParentRef);
			RegisterState(pAirJump);

			CBotState_AirDashAccel* pAirDashAccel = new CBotState_AirDashAccel(pParentRef);
			RegisterState(pAirDashAccel);

			////// ASPECT STATES
			CAspectState_SuperSaiyan* pStateSSJ = new CAspectState_SuperSaiyan(pParentRef);
			RegisterAspectState(pStateSSJ);

			CAspectState_PureMajin* pStatePureMajin = new CAspectState_PureMajin(pParentRef);
			RegisterAspectState(pStatePureMajin);

			CAspectState_GreatNamek* pStateGreatNamek = new CAspectState_GreatNamek(pParentRef);
			RegisterAspectState(pStateGreatNamek);

			CAspectState_Kaioken* pStateKaioken = new CAspectState_Kaioken(pParentRef);
			RegisterAspectState(pStateKaioken);

			CAspectState_SpinningAttack* pStateSpinningAttack = new CAspectState_SpinningAttack(pParentRef);
			RegisterAspectState(pStateSpinningAttack);

			CAspectState_Vehicle* pStateVehicle = new CAspectState_Vehicle(pParentRef);
			RegisterAspectState(pStateVehicle);

			CAspectState_RollingAttack* pStateRollingAttack = new CAspectState_RollingAttack(pParentRef);
			RegisterAspectState(pStateRollingAttack);

			////// CHAR CONDITIONS
			CConditionState_Invisible* pCondInvisible = new CConditionState_Invisible(pParentRef);
			RegisterConditionState(pCondInvisible);

			CConditionState_HidingKi* pCondHideKi = new CConditionState_HidingKi(pParentRef);
			RegisterConditionState(pCondHideKi);

			CConditionState_Invincible* pCondInvincible = new CConditionState_Invincible(pParentRef);
			RegisterConditionState(pCondInvincible);

			CConditionState_Taunt* pCondTaunt = new CConditionState_Taunt(pParentRef);
			RegisterConditionState(pCondTaunt);

			CConditionState_AttackDisallow* pCondAttackDisallow = new CConditionState_AttackDisallow(pParentRef);
			RegisterConditionState(pCondAttackDisallow);

			CConditionState_Transparent* pCondTransparent = new CConditionState_Transparent(pParentRef);
			RegisterConditionState(pCondTransparent);

			CConditionState_CantBeTargetted* pCondCantBeTargetted = new CConditionState_CantBeTargetted(pParentRef);
			RegisterConditionState(pCondCantBeTargetted);

			CConditionState_DirectPlay* pCondDirectPlay = new CConditionState_DirectPlay(pParentRef);
			RegisterConditionState(pCondDirectPlay);

			CConditionState_Bleeding* pCondBleeding = new CConditionState_Bleeding(pParentRef);
			RegisterConditionState(pCondBleeding);

			CConditionState_Poison* pCondPoison = new CConditionState_Poison(pParentRef);
			RegisterConditionState(pCondPoison);

			CConditionState_Stomachache* pCondStomachache = new CConditionState_Stomachache(pParentRef);
			RegisterConditionState(pCondStomachache);

			CConditionState_Burn* pCondBurn = new CConditionState_Burn(pParentRef);
			RegisterConditionState(pCondBurn);

			CConditionState_Confused* pCondConfused = new CConditionState_Confused(pParentRef);
			RegisterConditionState(pCondConfused);

			CConditionState_Terror* pCondTerror = new CConditionState_Terror(pParentRef);
			RegisterConditionState(pCondTerror);

			CConditionState_Barrier* pCondBarrier = new CConditionState_Barrier(pParentRef);
			RegisterConditionState(pCondBarrier);

			CConditionState_DamageReflection* pCondDamageReflection = new CConditionState_DamageReflection(pParentRef);
			RegisterConditionState(pCondDamageReflection);

			CConditionState_AfterEffect* pCondAftereffect = new CConditionState_AfterEffect(pParentRef);
			RegisterConditionState(pCondAftereffect);

			CConditionState_ChargingBlocked* pCondChargingBlocked = new CConditionState_ChargingBlocked(pParentRef);
			RegisterConditionState(pCondChargingBlocked);

			CConditionState_FakeDeath* pCondFakeDeath = new CConditionState_FakeDeath(pParentRef);
			RegisterConditionState(pCondFakeDeath);

			CConditionState_NullifiedDamage* pCondNullifiedDmg = new CConditionState_NullifiedDamage(pParentRef);
			RegisterConditionState(pCondNullifiedDmg);

			CConditionState_ClickDisable* pCondClickDisable = new CConditionState_ClickDisable(pParentRef);
			RegisterConditionState(pCondClickDisable);

			CConditionState_ClientUiDisable* pClientUiDisabled = new CConditionState_ClientUiDisable(pParentRef);
			RegisterConditionState(pClientUiDisabled);

			CConditionState_TaiyouKen* pCondTaiyouken = new CConditionState_TaiyouKen(pParentRef);
			RegisterConditionState(pCondTaiyouken);

			CConditionState_BattleInability* pCondBattleInability = new CConditionState_BattleInability(pParentRef);
			RegisterConditionState(pCondBattleInability);

			CConditionState_SkillInability* pCondSkillInability = new CConditionState_SkillInability(pParentRef);
			RegisterConditionState(pCondSkillInability);

			CConditionState_RevivalAftereffect* pCondRevivalAftereffect = new CConditionState_RevivalAftereffect(pParentRef);
			RegisterConditionState(pCondRevivalAftereffect);

			CConditionState_LpAutoRecover* pCondLpAutoRecover = new CConditionState_LpAutoRecover(pParentRef);
			RegisterConditionState(pCondLpAutoRecover);

			CConditionState_EpAutoRecover* pCondEpAutoRecover = new CConditionState_EpAutoRecover(pParentRef);
			RegisterConditionState(pCondEpAutoRecover);

			CConditionState_Rabies* pCondRabies = new CConditionState_Rabies(pParentRef);
			RegisterConditionState(pCondRabies);

			CConditionState_Drunk* pCondDrunk = new CConditionState_Drunk(pParentRef);
			RegisterConditionState(pCondDrunk);

			CConditionState_ExcitationMale* pCondExcitationMale = new CConditionState_ExcitationMale(pParentRef);
			RegisterConditionState(pCondExcitationMale);

			CConditionState_ExcitationFemale* pCondExcitationFemale = new CConditionState_ExcitationFemale(pParentRef);
			RegisterConditionState(pCondExcitationFemale);

			///

			this->m_pParentRef = pParentRef;
			this->m_eAirState = eAirState;

			return true;
	}
	else ERR_LOG(LOG_SYSTEM, "pParentRef == NULL");

	return true;
}

void CStateManager::Init()
{
	m_pParentRef = NULL;
	m_pCharState = NULL;
	m_pAspectState = NULL;
	m_qwConditionFlag = 0;
	m_eAirState = AIR_STATE_OFF;

	memset(m_apConditionState, NULL, sizeof(m_apConditionState));
	
	memset(m_apStateInstance, NULL, sizeof(m_apStateInstance));
	memset(m_apAspectStateInstance, NULL, sizeof(m_apAspectStateInstance));
	memset(m_apConditionStateInstance, NULL, sizeof(m_apConditionStateInstance));
}

void CStateManager::Destroy()
{
	for (int i = 0; i < CHARCONDITION_COUNT; i++)
	{
		m_apConditionState[i] = NULL;

		if (m_apConditionStateInstance[i])
			delete m_apConditionStateInstance[i];
	}

	for (int i = 0; i < CHARSTATE_COUNT; i++)
	{
		if (m_apStateInstance[i])
			delete m_apStateInstance[i];
	}

	for (int i = 0; i < ASPECTSTATE_COUNT; i++)
	{
		if (m_apAspectStateInstance[i])
			delete m_apAspectStateInstance[i];
	}

	m_pParentRef = NULL;
	m_pCharState = NULL;
	m_pAspectState = NULL;
	m_qwConditionFlag = 0;
	m_eAirState = AIR_STATE_OFF;
}


CCharState* CStateManager::GetCharStateInstance(BYTE stateID)
{
	if (stateID < CHARSTATE_COUNT)
		return m_apStateInstance[stateID];

	ERR_LOG(LOG_USER, "stateID %u > CHARSTATE_COUNT %u", stateID, CHARSTATE_COUNT);
	return NULL;
}

CAspectState* CStateManager::GetAspectInstance(BYTE stateID)
{
	if (stateID < ASPECTSTATE_COUNT)
		return m_apAspectStateInstance[stateID];

	ERR_LOG(LOG_USER, "stateID %u > ASPECTSTATE_COUNT %u", stateID, ASPECTSTATE_COUNT);
	return NULL;
}

CConditionState * CStateManager::GetConditionInstance(BYTE stateID)
{
	if (stateID < CHARCONDITION_COUNT)
		return m_apConditionStateInstance[stateID];

	ERR_LOG(LOG_USER, "stateID %u > CHARCONDITION_COUNT %u", stateID, CHARCONDITION_COUNT);
	return NULL;
}


int CStateManager::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_pCharState)
	{
		m_pCharState->SetStateTime(GetCharState()->GetStateTime() + dwTickDiff);

		m_pCharState->OnUpdate(dwTickDiff, fMultiple);

		if (m_pCharState->IsFinished())
		{
			if (m_pCharState->GetNextStateID() == INVALID_BYTE)
			{
				ERR_LOG(LOG_GENERAL, "GetCharState()->GetNextStateID() == INVALID_BYTE");
				return 1;
			}

			if (ChangeCharState(m_pCharState->GetNextStateID(), NULL, true))
				return 2;
		}
	}

	if (m_pAspectState)
	{
		m_pAspectState->SetStateTime(m_pAspectState->GetStateTime() + dwTickDiff);
		m_pAspectState->OnUpdate(dwTickDiff, fMultiple); 

		if (m_pAspectState) //OnUpdate could remove current aspectstate -> so check if still available.
		{
			if (m_pAspectState->IsFinished())
			{
				if (ChangeAspectState(m_pAspectState->GetNextStateID(), NULL, true))
					return 2;
			}
		}
	}

	if (m_qwConditionFlag)
	{
		for (int j = 0; j < CHARCONDITION_COUNT; j++)
		{
			CConditionState* pCondState = m_apConditionState[j];
			if (pCondState)
			{
				pCondState->SetStateTime(pCondState->GetStateTime() + dwTickDiff);

				int nCondUpdateRes = pCondState->OnUpdate(dwTickDiff, fMultiple);
				if (nCondUpdateRes == 1 || nCondUpdateRes == 3)
					RemoveConditionState(j, NULL, true);
			}
		}
	}

	return 0;
}

bool CStateManager::ChangeCharState(BYTE stateID, void *pvInitialData, bool bUpdate)
{
	CCharState* pBase = NULL;

	if (m_pCharState)
	{
		if (!m_pCharState->CanTransition(stateID))
			return false;

		/*if (m_pCharState->GetStateID() == stateID)
		{
			m_pCharState->SetPrevStateID(stateID);
			m_pCharState->SetStateTime(0);
			if (bUpdate)
				m_pParentRef->OnUpdateState();
			return true;
		}*/

		pBase = GetCharStateInstance(stateID);
		if (!pBase)
		{
			return false;
		}

		pBase->SetPrevStateID(m_pCharState->GetStateID());
		pBase->SetStateTime(0);
		pBase->SetFinish(false);


		m_pCharState->SetPrevStateID(INVALID_BYTE);
		m_pCharState->SetNextStateID(stateID); // set next state ID, so at "onExit" we know which state we will go to.

		m_pCharState->OnExit();
		m_pCharState->SetStateTime(0);
		m_pCharState->SetNextStateID(INVALID_BYTE); // init next state id
	}
	else
	{
		pBase = GetCharStateInstance(stateID);
		if (!pBase)
		{
			ERR_LOG(LOG_GENERAL, "GetCharStateInstance is null");
			return false;
		}
	}

	m_pCharState = pBase;

	m_pCharState->SetInitialData(pvInitialData);
	m_pCharState->OnEnter();
	m_pCharState->ResetInitialData();

	if (bUpdate)
		m_pParentRef->OnUpdateState();

	return true;
}

bool CStateManager::CanCharStateTransition(BYTE stateID)
{
	if (GetCharState())
	{
		return GetCharState()->CanTransition(stateID);
	}

	return false;
}

BYTE CStateManager::GetCharStateID()
{
	if (!GetCharState())
	{
		ERR_LOG(LOG_GENERAL, "No current charstate found. Obj Type %u", m_pParentRef->GetObjType());
		return INVALID_BYTE;
	}

	return GetCharState()->GetStateID();
}

DWORD CStateManager::GetCharStateTime()
{
	if (!GetCharState())
	{
		ERR_LOG(LOG_GENERAL, "No current charstate found. Obj Type %u", m_pParentRef->GetObjType());
		return INVALID_DWORD;
	}

	return GetCharState()->GetStateTime();
}

bool CStateManager::IsCharState(BYTE stateID)
{
	if (GetCharState())
	{
		return GetCharState()->GetStateID() == stateID;
	}

	return false;
}

CCharState* CStateManager::GetCharState()
{
	return this->m_pCharState;
}

bool CStateManager::ChangeAspectState(BYTE stateID, void *pvInitialData, bool bUpdate)
{
	CAspectState* pBase = NULL;

	if (m_pAspectState)
	{
		if (stateID != ASPECTSTATE_INVALID)
		{
			if (!m_pAspectState->CanTransition(stateID))
				return false;

			if (m_pAspectState->GetStateID() == stateID)
			{
				m_pAspectState->SetPrevStateID(m_pAspectState->GetStateID());
				m_pAspectState->SetStateTime(0);

				if (bUpdate)
					m_pParentRef->OnUpdateAspectState();

				return true;
			}
		}
		m_pAspectState->OnExit();
		m_pAspectState->SetPrevStateID(ASPECTSTATE_INVALID);
		m_pAspectState->SetNextStateID(ASPECTSTATE_INVALID);
	}

	if (stateID != ASPECTSTATE_INVALID)
	{
		pBase = GetAspectInstance(stateID);
		if (!pBase)
		{
			ERR_LOG(LOG_USER, "NULL == GetAspectInstance( %d )", stateID);
			return false;
		}

		if (m_pAspectState)
			pBase->SetPrevStateID(m_pAspectState->GetStateID());
		else
			pBase->SetPrevStateID(ASPECTSTATE_INVALID);

		pBase->SetStateTime(0);
		pBase->SetFinish(false);
	}

	m_pAspectState = pBase;

	if (m_pAspectState)
	{
		m_pAspectState->SetInitialData(pvInitialData);
		m_pAspectState->OnEnter();
		m_pAspectState->ResetInitialData();
	}

	if (bUpdate)
		m_pParentRef->OnUpdateAspectState();

	return true;
}

BYTE CStateManager::GetAspectStateID()
{
	if (m_pAspectState)
		return m_pAspectState->GetStateID();

	return ASPECTSTATE_INVALID;
}

bool CStateManager::AddConditionState(BYTE stateID, void *pvInitialData, bool bUpdate)
{
	if (stateID < CHARCONDITION_COUNT)
	{
		CConditionState* pCondState = GetConditionInstance(stateID);
		if (pCondState)
		{
			if (pCondState->GetStateID() == stateID)
			{
				if(m_apConditionState[stateID])
				{
					if (pCondState->GetReferCount() == 0)
					{
						ERR_LOG(LOG_GENERAL, "The ReferCount should be bigger than 0 because the state is set now., stateID = %d(%s), pState->GetReferCount() = %u", stateID, pCondState->GetStateName(), pCondState->GetReferCount());
					}

					pCondState->IncreaseReferCount();
					pCondState->OnEnter(); //not sure
				//	printf("CStateManager::AddConditionState: if (!(unsigned __int8)((int(__fastcall *)(CStateBase *))v23->vfptr[1].__vecDelDtor)(v23)) \n");
				//	if (!(unsigned __int8)((int(__fastcall *)(CStateBase *))v23->vfptr[1].__vecDelDtor)(v23))
						return true;
				}
				else
				{
					if (pCondState->GetReferCount() > 0)
					{
						ERR_LOG(LOG_GENERAL, "The ReferCount should be 0 because the state is not set now., stateID = %d(%s), pState->GetReferCount() = %u", stateID, pCondState->GetStateName(), pCondState->GetReferCount());
					}

					pCondState->SetReferCount(1);
					m_apConditionState[stateID] = pCondState;
				}

				pCondState->SetInitialData(pvInitialData);
				pCondState->OnEnter();
				pCondState->ResetInitialData();

				QWORD qwOldConditionFlag = m_qwConditionFlag;
				BIT_FLAG_SET(m_qwConditionFlag, MAKE_BIT_FLAG64(stateID));

				if (bUpdate && qwOldConditionFlag != m_qwConditionFlag)
				{
					m_pParentRef->OnUpdateCondition();
				}

				return true;
			}
		}
		else ERR_LOG(LOG_USER, "ERROR: CConditionState = NULL. stateID[%u]", stateID);
	}
	else ERR_LOG(LOG_USER, "stateID[%u] >= CHARCONDITION_COUNT[%u]", stateID, CHARCONDITION_COUNT);

	return false;
}

bool CStateManager::RemoveConditionState(BYTE stateID, void *pvReferenceDataForExiting, bool bUpdate)
{
	if (stateID < CHARCONDITION_COUNT)
	{
		CConditionState* pCondState = GetConditionInstance(stateID);
		if (pCondState)
		{
			if (pCondState->GetStateID() == stateID)
			{
				if (m_apConditionState[stateID])
				{
					if (pCondState->GetReferCount() > 0)
					{
						pCondState->DecreaseReferCount();
					}
					else
					{
						ERR_LOG(LOG_GENERAL, "The ReferCount should be bigger than 0 because the state is set now., stateID = %d(%s), pState->GetReferCount() = %u", stateID, pCondState->GetStateName(), pCondState->GetReferCount());
					}

					if (pCondState->GetReferCount() == 0)
						m_apConditionState[stateID] = NULL;

					pCondState->SetReferenceDataForExiting(pvReferenceDataForExiting);
					pCondState->OnExit();
					pCondState->ResetReferenceDataForExiting();
				}
				else if (pCondState->GetReferCount() > 0)
				{
					ERR_LOG(LOG_GENERAL, "The ReferCount should be 0 because the state is not set now., stateID = %d(%s), pState->GetReferCount() = %u", stateID, pCondState->GetStateName(), pCondState->GetReferCount());
					
					pCondState->SetReferCount(0);
				}
				
				QWORD qwOldConditionFlag = m_qwConditionFlag;
				
				if (m_apConditionState[stateID] == NULL)
				{
					BIT_FLAG_UNSET(m_qwConditionFlag, MAKE_BIT_FLAG64(stateID));
				}

				if (bUpdate && qwOldConditionFlag != m_qwConditionFlag)
				{
					m_pParentRef->OnUpdateCondition();
				}

				return true;
			}
		}
	}
	else ERR_LOG(LOG_USER, "stateID[%u] >= CHARCONDITION_COUNT[%u]", stateID, CHARCONDITION_COUNT);

	return false;
}

bool CStateManager::AddConditionFlags(QWORD qwCharConditionFlag, bool bUpdate)
{
	for (int i = 0; i < CHARCONDITION_COUNT; i++)
	{
		if (BIT_FLAG_TEST(qwCharConditionFlag, MAKE_BIT_FLAG64(i)))
			AddConditionState(i, NULL, false);
	}

	if (bUpdate)
		m_pParentRef->OnUpdateCondition();

	return true;
}

bool CStateManager::RemoveConditionFlags(QWORD qwCharConditionFlag, bool bUpdate)
{
	for (int i = 0; i < CHARCONDITION_COUNT; i++)
	{
		if (BIT_FLAG_TEST(qwCharConditionFlag, MAKE_BIT_FLAG64(i)))
		{
			RemoveConditionState(i, NULL, false);
		}
	}

	if (bUpdate)
		m_pParentRef->OnUpdateCondition();

	return true;
}

bool CStateManager::IsCharCondition(BYTE stateID)
{
	return BIT_FLAG_TEST(m_qwConditionFlag, MAKE_BIT_FLAG64(stateID));
}

bool CStateManager::IsValidTarget(DWORD dwNoCheckStateFlag)
{
	if (GetCharState())
	{
		if (BIT_FLAG_TEST(dwNoCheckStateFlag, MAKE_BIT_FLAG(GetCharState()->GetStateID())))
			return true;
		else
		{
			return GetCharState()->IsValidTarget();
		}
	}

	return false;
}


int CStateManager::OnObjectMsg(CObjectMsg* pObjectMsg)
{
	if (GetCharState() && GetCharState()->OnObjectMsg(pObjectMsg) == 4)
		return 4;
	else if (m_pAspectState && m_pAspectState->OnObjectMsg(pObjectMsg) == 4)
		return 4;
	else
	{
		/*for (int j = 0; j < CHARCONDITION_COUNT; j++)
		{
			if (m_apConditionState[j] && m_apConditionState[j]->OnObjectMsg(pObjectMsg) == 4)
				return 4;
		}*/
	}

	return FALSE;
}

int CStateManager::CopyTo(sCHARSTATE* pCharState)
{
	int nSize = 0;

	if (GetCharState())
	{
		CStateBase* pStateBase = GetCharState();

		pCharState->sCharStateBase.byStateID = pStateBase->GetStateID();
		pCharState->sCharStateBase.dwStateTime = pStateBase->GetStateTime();
		pCharState->sCharStateBase.dwConditionFlag = m_qwConditionFlag;
		CopyAspectTo(&pCharState->sCharStateBase.aspectState);
		pCharState->sCharStateBase.bFightMode = m_pParentRef->GetFightMode();
		m_pParentRef->GetCurLoc().CopyTo(pCharState->sCharStateBase.vCurLoc);
		m_pParentRef->GetCurDir().CopyTo(pCharState->sCharStateBase.vCurDir);
		pCharState->sCharStateBase.eAirState = GetAirState();

		nSize = sizeof(sCHARSTATE_BASE);
		nSize += pStateBase->CopyTo(&pCharState->sCharStateDetail);

		return nSize;
	}
	else ERR_LOG(LOG_USER, "NULL == GetCharState()");

	return nSize;
}

bool CStateManager::CopyFrom(sCHARSTATE* pCharState)
{
	if (ChangeCharState(pCharState->sCharStateBase.byStateID, NULL, false))
	{
		SetAirState((eAIR_STATE)pCharState->sCharStateBase.eAirState);
		CopyAspectFrom(&pCharState->sCharStateBase.aspectState);
	//	SetConditionState(pCharState->sCharStateBase.dwConditionFlag);		//if we do this, then we can not update condition state in OnExit/
		m_pParentRef->SetFightMode(pCharState->sCharStateBase.bFightMode);
		m_pParentRef->SetCurLoc(pCharState->sCharStateBase.vCurLoc, m_pParentRef->GetCurWorld());
		m_pParentRef->SetCurDir(pCharState->sCharStateBase.vCurDir);

		GetCharState()->CopyFrom(&pCharState->sCharStateDetail);

		return true;
	}

	return false;
}

int CStateManager::CopyAspectTo(sASPECTSTATE* pAspectState)
{
	if (m_pAspectState)
	{
		pAspectState->sAspectStateBase.byAspectStateId = m_pAspectState->GetStateID();
		m_pAspectState->CopyAspectTo(&pAspectState->sAspectStateDetail);
	}
	else pAspectState->sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

	return sizeof(sASPECTSTATE);
}

void CStateManager::CopyAspectFrom(sASPECTSTATE* pAspectState)
{
	if (pAspectState->sAspectStateBase.byAspectStateId == ASPECTSTATE_INVALID)
		ChangeAspectState(ASPECTSTATE_INVALID, NULL, false);
	else
		ChangeAspectState(pAspectState->sAspectStateBase.byAspectStateId, NULL, false);

	if (m_pAspectState)
		m_pAspectState->CopyAspectFrom(&pAspectState->sAspectStateDetail);
}

void CStateManager::RegisterState(CCharState *pState)
{
	m_apStateInstance[pState->GetStateID()] = pState;
}

void CStateManager::RegisterAspectState(CAspectState *pState)
{
	m_apAspectStateInstance[pState->GetStateID()] = pState;
}

void CStateManager::RegisterConditionState(CConditionState *pState)
{
	m_apConditionStateInstance[pState->GetStateID()] = pState;
}