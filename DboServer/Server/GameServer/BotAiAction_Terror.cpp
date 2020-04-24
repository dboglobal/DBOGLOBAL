#include "stdafx.h"
#include "BotAiAction_Terror.h"
#include "BotAiAction_NavMove.h"
#include "NtlRandom.h"


CBotAiAction_Terror::CBotAiAction_Terror(CNpc* pBot)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_TERROR, "BOTCONTROL_ACTION_TERROR")
{

}

CBotAiAction_Terror::~CBotAiAction_Terror()
{
}



void CBotAiAction_Terror::OnEnter()
{
	if (!GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_TERROR))
		GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_TERROR, true);

	CNtlVector pNextDest;
	GetNextDest(NULL, &pNextDest);

	CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), pNextDest, true, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
	if (!AddSubControlQueue(pNavMove, true))
		m_status = FAILED;
}

void CBotAiAction_Terror::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_TERROR, false);
}


int CBotAiAction_Terror::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetBot()->GetStateManager()->IsCharCondition(CHARCOND_TERROR))
	{
		if (UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED)
		{
			CNtlVector pNextDest;
			GetNextDest(NULL, &pNextDest);

			CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), pNextDest, true, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
			if (!AddSubControlQueue(pNavMove, true))
				m_status = FAILED;
		}
	}
	else
	{
		m_status = COMPLETED;
	}

	return m_status;
}

bool CBotAiAction_Terror::GetNextDest(CNtlVector* pNextDir, CNtlVector* pNextDest)
{
	CNtlVector vNewDir;

	if (pNextDir)
	{
		vNewDir.operator=(*pNextDir);
	}
	else
	{
		vNewDir.x = RandomRangeF(-1.0, 1.0);
		vNewDir.z = RandomRangeF(-1.0, 1.0);
		vNewDir.y = 0.0f;

		if (!vNewDir.SafeNormalize())
			vNewDir.operator=(CNtlVector::UNIT_X);
	}

	pNextDest->operator=(GetBot()->GetCurLoc().operator+(vNewDir.operator*(30.0f)));
	return true;
}