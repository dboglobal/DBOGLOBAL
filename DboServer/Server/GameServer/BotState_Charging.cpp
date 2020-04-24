#include "stdafx.h"
#include "BotState_Charging.h"
#include "GameServer.h"
#include "CPlayer.h"
#include "FormulaTable.h"


CBotState_Charging::CBotState_Charging(CCharacter* pParent)
	:CCharState(CHARSTATE_CHARGING, pParent)
{
}

CBotState_Charging::~CBotState_Charging()
{
}


void CBotState_Charging::OnEnter()
{
	m_fCurrentRPChargingTime = 0.0f;
	m_fCurrentValue = (float)m_pParent->GetCurRP();
	m_fDestValue = (float)m_pParent->GetCharAtt()->GetMaxRP();
	m_fRPChargingTime = (m_fDestValue - m_fCurrentValue) / (m_pParent->GetCharAtt()->GetRPRegen() / 1);

	m_pParent->StartIncreaseRP();

	CCharState::OnEnter();
}

void CBotState_Charging::OnExit()
{
	m_pParent->StopIncreaseRP();

	CCharState::OnExit();
}

int CBotState_Charging::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharState::OnUpdate(dwTickDiff, fMultiple);

	m_fCurrentRPChargingTime += (float)dwTickDiff / 1000.0f;

	if (m_fCurrentRPChargingTime >= m_fRPChargingTime)
	{
		m_fCurrentRPChargingTime = m_fRPChargingTime;
	}

	float fValue = m_fCurrentRPChargingTime / m_fRPChargingTime;

	if (fValue > 1.0f || m_fRPChargingTime == 0.0f)
		fValue = m_fDestValue;
	else if (fValue < 0.0f)
		fValue = m_fCurrentValue;
	else
		fValue = fValue * m_fDestValue + (1 - fValue) * m_fCurrentValue;

	//printf("fValue %f CurRp %u m_fCurrentRPChargingTime %f m_fRPChargingTime %f, GetCurRPBall %u\n", fValue, m_pParent->GetCurRP(), m_fCurrentRPChargingTime, m_fRPChargingTime, m_pParent->GetCurRPBall());

	if ((WORD)fValue >= m_pParent->GetCharAtt()->GetMaxRP())
	{
		if (m_pParent->GetCurRPBall() < m_pParent->GetMaxRPBall())
		{
			m_pParent->UpdateRpBall(1, true, false);
			m_pParent->SetCurRP(0);

			m_fCurrentRPChargingTime = 0.0f;
			m_fCurrentValue = 0.0f;
			m_fRPChargingTime = m_fDestValue / (m_pParent->GetCharAtt()->GetRPRegen() / 1);

			return 0;
		}
	}

	m_pParent->SetCurRP((WORD)fValue);

	return 0;
}

int CBotState_Charging::OnObjectMsg(CObjectMsg * pObjectMsg)
{
	return NULL;
}

bool CBotState_Charging::CanTransition(BYTE stateID)
{
	if (stateID == CHARSTATE_SITTING || stateID == CHARSTATE_GUARD || stateID == CHARSTATE_RIDEON || stateID == CHARSTATE_AIR_JUMP)
		return false;

	return true;
}

int CBotState_Charging::CopyTo(sCHARSTATE_DETAIL* pStateDetail)
{
	return NULL;
}

void CBotState_Charging::CopyFrom(sCHARSTATE_DETAIL* pStateDetail)
{
}

