#include "stdafx.h"
#include "BotState_KeepingEffect.h"
#include "char.h"


CBotState_KeepingEffect::CBotState_KeepingEffect(CCharacter* pParent)
	:CCharState(CHARSTATE_KEEPING_EFFECT, pParent)
{
	m_skillId = INVALID_TBLIDX;
}

CBotState_KeepingEffect::~CBotState_KeepingEffect()
{
}


void CBotState_KeepingEffect::OnEnter()
{
	CCharState::OnEnter();
}

void CBotState_KeepingEffect::OnExit()
{
	m_skillId = INVALID_TBLIDX;

	if(m_pParent->IsPC() == false) //only apply on PC. Otherwise if pc is in this state and use skill which has casting, then he stays forever in casting
		m_pParent->GetSkillManager()->CancelCasting(); //if this not here then mobs wont use any skills anymore after using a skill which set them into keeping effect

	// in case we go into keeping-effect state while already in keeping-effect state.
	if (GetNextStateID() == CHARSTATE_KEEPING_EFFECT)
		m_pParent->RemoveKeepEffectTargets();

	CCharState::OnExit();
}

int CBotState_KeepingEffect::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	return 0;
}

int CBotState_KeepingEffect::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return 0;
}

bool CBotState_KeepingEffect::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_GUARD || stateID == CHARSTATE_RIDEON)
		return false;

	return true;
}

int CBotState_KeepingEffect::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	pStateDetail->sCharStateKeepingEffect.skillId = m_skillId;

	return sizeof(sCHARSTATE_KEEPING_EFFECT);
}

void CBotState_KeepingEffect::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
	m_skillId = pStateDetail->sCharStateKeepingEffect.skillId;
}

