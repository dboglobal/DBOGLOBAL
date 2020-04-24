#include "stdafx.h"
#include "ScriptAlgoAction_While.h"
#include "WPSNodeAction_While.h"
#include "WorldPlayScriptManager.h"
#include "WorldPlayControlFactory.h"


CWpsAlgoAction_While::CWpsAlgoAction_While(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_WHILE, "SCRIPTCONTROL_ACTION_WHILE")
{

}


CWpsAlgoAction_While::~CWpsAlgoAction_While()
{
}


bool CWpsAlgoAction_While::AttachControlScriptNode(CControlScriptNode * pControlScriptNode)
{
	CWPSNodeAction_While* pAction = dynamic_cast<CWPSNodeAction_While*>(pControlScriptNode);
	if (pAction)
	{
		m_scriptnodeID = pAction->GetNodeIndex();

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_While", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_While::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nActionStatus = UpdateSubControlQueue(dwTickDiff, fMultiple); //update actions

	if (nActionStatus == COMPLETED) //CHECK IF "LOOP" HAS BEEN COMPLETED. Then Update condition
	{
		int nCondStatus = UpdateSubControlList(dwTickDiff, fMultiple); //update condition

		//IF ACTION, CONDITION == COMPLETED AND PARENT == "CHILD" THEN RESTART LOOP
		if (nActionStatus == COMPLETED && nCondStatus == COMPLETED)
		{
			CControlScriptNode* pPlayScript = g_pWorldPlayScriptManager->GetWorldPlayScriptNode(GetOwner()->GetScriptID(), m_scriptnodeID);
			if (pPlayScript == NULL)
			{
				ERR_LOG(LOG_BOTAI, "can not find 'CWPSNodeAction_While'. script[%u] node index[%u]", GetOwner()->GetScriptID(), m_scriptnodeID);
				m_status = FAILED;
				return m_status;
			}

			if (pPlayScript->GetID() != WPS_NODE_ACTION_WHILE)
			{
				ERR_LOG(LOG_BOTAI, "CControlScriptNode is not 'WPS_NODE_ACTION_WHILE'. script[%u] node index[%u]", GetOwner()->GetScriptID(), m_scriptnodeID);
				m_status = FAILED;
				return m_status;
			}

			CControlScriptNode* pParent = pPlayScript->GetParent();
			if (pParent)
			{
				if (pParent->GetID() == WPS_NODE_CONDITION_CHILD) //ONLY RESTART IF PARENT IS "CHILD"
				{
				//	ERR_LOG(LOG_GENERAL, "Script %u RESTART WHILE m_scriptnodeID %u", GetOwner()->GetScriptID(), m_scriptnodeID);
					CWorldPlayControlFactory factory(GetOwner());
					if (!pPlayScript->AttachScript(this, &factory, false))
					{
						ERR_LOG(LOG_BOTAI, "could not attach script. script[%u] node index[%u]", GetOwner()->GetScriptID(), m_scriptnodeID);
						m_status = FAILED;
					}

					return ACTIVATED;
				}
			}
		}

		m_status = nCondStatus;
	}

	return m_status;
}
