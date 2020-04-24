#include "stdafx.h"
#include "TqsAlgoCondition_NpcFaint.h"
#include "TQSNodeCondition_NpcFaint.h"
#include "Npc.h"
#include "TimeQuest.h"


CTqsAlgoCondition_NpcFaint::CTqsAlgoCondition_NpcFaint(CTqsAlgoObject* pObject) :
	CTqsAlgoCondition_Base(pObject, TQS_ALGOCONTROLID_CONDITION_NPCFAINT, "TQS_ALGOCONTROLID_CONDITION_NPCFAINT")
{
}


CTqsAlgoCondition_NpcFaint::~CTqsAlgoCondition_NpcFaint()
{
}


bool CTqsAlgoCondition_NpcFaint::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeCondition_NpcFaint* pNode = dynamic_cast<CTQSNodeCondition_NpcFaint*>(pControlScriptNode);
	if (pNode)
	{
		m_npcTblidx = pNode->m_npcTblidx;
		m_bTmqFail = pNode->m_bTmqFail;
		return true;
	}

	return false;
}

int CTqsAlgoCondition_NpcFaint::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	/*
		This function is COMPLETED when 1. NPC is not inside world 2. NPC is fainting.
	*/

	CNpc* pNpc = GetOwner()->FindNpc(m_npcTblidx);

	if (pNpc == NULL)
	{
		return m_status;
	}
	
	
	if (pNpc->GetCurWorld() == NULL || pNpc->IsFainting() || !pNpc->IsInitialized())
	{
		if (m_bTmqFail)
		{
			m_status = CHANGED;

			GetOwner()->GetTimeQuest()->UpdateTmqFail();
		}
		else m_status = COMPLETED;
	}

	return m_status;
}
