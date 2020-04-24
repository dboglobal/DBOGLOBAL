#include "stdafx.h"
#include "TqsAlgoCondition_AreaEnter.h"
#include "TQSNodeCondition_AreaEnter.h"
#include "CPlayer.h"
#include "TimeQuest.h"


CTqsAlgoCondition_AreaEnter::CTqsAlgoCondition_AreaEnter(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_AREAENTER, "TQS_ALGOCONTROLID_CONDITION_AREAENTER")
{
}


CTqsAlgoCondition_AreaEnter::~CTqsAlgoCondition_AreaEnter()
{
}


bool CTqsAlgoCondition_AreaEnter::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeCondition_AreaEnter* pCond = dynamic_cast<CTQSNodeCondition_AreaEnter*>(pControlScriptNode);
	if (pCond)
	{
		m_vStart = pCond->m_vStart;
		m_vEnd = pCond->m_vEnd;
		m_vOrigin = pCond->m_vOrigin;
		m_fRadius = pCond->m_fRadius;
		m_bAllMember = pCond->m_bAllMember;
		m_dwTimeLimit = pCond->m_dwTimeLimit;
		m_bTmqFail = pCond->m_bTmqFail;

		return true;
	}

	return false;
}

int CTqsAlgoCondition_AreaEnter::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_bAllMember) //check all member
	{
		bool bFlag = true;

		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer)
		{
			if (pPlayer->GetWorldID() != GetOwner()->GetWorld()->GetID() || !pPlayer->IsInRange(m_vOrigin, m_fRadius)) //check if player not in world or not in range
			{
				bFlag = false;
				break;
			}

			pPlayer = GetOwner()->GetPlayersNext();
		}

		if (bFlag)
			m_status = COMPLETED;
	}
	else //check if at least one person is inside
	{
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer)
		{
			if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID() && pPlayer->IsInRange(m_vOrigin, m_fRadius)) //check if player in world && in range
			{
				m_status = COMPLETED;
				break;
			}

			pPlayer = GetOwner()->GetPlayersNext();
		}
	}

	if (m_status == COMPLETED && m_bTmqFail)
	{
		GetOwner()->GetTimeQuest()->UpdateTmqFail();
	}
	
	return m_status;
}
