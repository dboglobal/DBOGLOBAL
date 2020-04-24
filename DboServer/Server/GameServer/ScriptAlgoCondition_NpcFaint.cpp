#include "stdafx.h"
#include "ScriptAlgoCondition_NpcFaint.h"
#include "WPSNodeCondition_NpcFaint.h"
#include "Npc.h"


CWpsAlgoCondition_NpcFaint::CWpsAlgoCondition_NpcFaint(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_NPCFAINT, "SCRIPTCONTROL_CONDITION_NPCFAINT")
{
	m_bWpsFail = false;
}


CWpsAlgoCondition_NpcFaint::~CWpsAlgoCondition_NpcFaint()
{
}


bool CWpsAlgoCondition_NpcFaint::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_NpcFaint* pNode = dynamic_cast<CWPSNodeCondition_NpcFaint*>(pControlScriptNode);
	if (pNode)
	{
		m_npcTblidx = pNode->m_npcTblidx;
		m_bWpsFail = pNode->m_bWpsFail;
		return true;
	}

	return false;
}

int CWpsAlgoCondition_NpcFaint::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	/*
		This function is COMPLETED when 1. NPC does not exist. 2. NPC is not inside world 3. NPC is fainting.
	*/

	CNpc* pNpc = GetOwner()->FindNpc(m_npcTblidx);

	///SHOULD WE CHECK NPC FROM WORLD?
//	if (pNpc == NULL && GetOwner()->GetWorld())
//	{
//		ERR_LOG(LOG_GENERAL, "IMPORTANT-ERROR: pNPC IS NULL! TRY TO GET FROM WORLD.");
//		pNpc = GetOwner()->GetWorld()->FindNpc(m_npcTblidx);
//	}

	if (pNpc == NULL || !pNpc->IsInitialized()) //did we already remove the npc? 
	{
		m_status = COMPLETED;
	}
	else if (pNpc->GetCurWorld() == NULL || pNpc->IsFainting())
	{
		m_status = COMPLETED;
	}

	if (m_status == COMPLETED && m_bWpsFail == true)
	{
		m_status = CHANGED;
		GetOwner()->GetController()->ChangeControlStage_Failed();
	}

	return m_status;
}
