#include "stdafx.h"
#include "TqsAlgoCondition_AllPlayerFaint.h"
#include "TQSNodeCondition_AllPlayerFaint.h"
#include "TimeQuest.h"
#include "CPlayer.h"


CTqsAlgoCondition_AllPlayerFaint::CTqsAlgoCondition_AllPlayerFaint(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_ALL_PLAYER_FAINT, "TQS_ALGOCONTROLID_CONDITION_ALL_PLAYER_FAINT")
{
}


CTqsAlgoCondition_AllPlayerFaint::~CTqsAlgoCondition_AllPlayerFaint()
{
}


bool CTqsAlgoCondition_AllPlayerFaint::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeCondition_AllPlayerFaint* pCond = dynamic_cast<CTQSNodeCondition_AllPlayerFaint*>(pControlScriptNode);
	if (pCond)
	{
		m_bTmqFail = pCond->m_bTmqFail;

		return true;
	}

	return false;
}

int CTqsAlgoCondition_AllPlayerFaint::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (!pPlayer->IsFainting())
		{
			return m_status;
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}

	if (m_bTmqFail)
	{
		m_status = CHANGED;

		GetOwner()->GetTimeQuest()->UpdateTmqFail();
	}
	else m_status = COMPLETED;

	
	return m_status;
}
