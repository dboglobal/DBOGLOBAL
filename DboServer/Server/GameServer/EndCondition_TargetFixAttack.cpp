#include "stdafx.h"
#include "EndCondition_TargetFixAttack.h"
#include "Npc.h"


CEndCondition_TargetFixAttack::CEndCondition_TargetFixAttack(CNpc* pBot, HOBJECT hTarget, eOBJTYPE eObjType, CSPSNodeAction_TargetFixAttack::eENDTYPE eEndType, float fEndValue_Percent_Target_LP)
:CEndCondition(pBot, "CEndCondition_TargetFixAttack")
{
	m_hTarget = hTarget;
	m_dwTime = 0;
	m_eObjType = eObjType;
	m_eEndType = eEndType;
	m_fEndValue_Percent_Target_LP = fEndValue_Percent_Target_LP;
}

CEndCondition_TargetFixAttack::~CEndCondition_TargetFixAttack()
{
}


int CEndCondition_TargetFixAttack::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime < 1000)
		return 0;

	m_dwTime = 0;

	if (GetBot()->GetFixedTarget() == INVALID_HOBJECT)
		return 1;

	if (m_eObjType < OBJTYPE_PC || m_eObjType > OBJTYPE_MOB)
		return 0;

	CCharacter* pTarget = g_pObjectManager->GetChar(m_eObjType, m_hTarget);
	if (!pTarget || !pTarget->IsInitialized())
		return 1;

	if (m_eEndType == CSPSNodeAction_TargetFixAttack::ePERCENT_TARGET_LP)
	{
		if (pTarget->ConsiderLPLow(m_fEndValue_Percent_Target_LP) == true)
			return 1;

		return 0;
	}

	ERR_LOG(LOG_BOTAI, "fail : m_eEndType( %d )", m_eEndType);

	return 2;
}

void CEndCondition_TargetFixAttack::OnExit()
{
	GetBot()->SetFixedTarget(INVALID_HOBJECT);
}