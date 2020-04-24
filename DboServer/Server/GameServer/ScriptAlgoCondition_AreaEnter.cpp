#include "stdafx.h"
#include "ScriptAlgoCondition_AreaEnter.h"
#include "WPSNodeCondition_AreaEnter.h"
#include "CPlayer.h"


CWpsAlgoCondition_AreaEnter::CWpsAlgoCondition_AreaEnter(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_AREAENTER, "SCRIPTCONTROL_CONDITION_AREAENTER")
{
}


CWpsAlgoCondition_AreaEnter::~CWpsAlgoCondition_AreaEnter()
{
}


bool CWpsAlgoCondition_AreaEnter::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_AreaEnter* pCond = dynamic_cast<CWPSNodeCondition_AreaEnter*>(pControlScriptNode);
	if (pCond)
	{
		m_vStart = pCond->m_vStart;
		m_vEnd = pCond->m_vEnd;
		m_vOrigin = pCond->m_vOrigin;
		m_fRadius = pCond->m_fRadius;
		m_bAllMember = pCond->m_bAllMember;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_AreaEnter::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// IN CASE THIS IS CALLED BY "LOOP COND" DO NOT SET m_status. RETURN STATUS DIRECTLY TO AVOID BUG.

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
			return COMPLETED;
	}
	else //check if at least one person is inside
	{
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer)
		{
			if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID() && pPlayer->IsInRange(m_vOrigin, m_fRadius)) //check if player in world && in range
			{
				return COMPLETED;
			}

			pPlayer = GetOwner()->GetPlayersNext();
		}
	}
	
	return m_status;
}
