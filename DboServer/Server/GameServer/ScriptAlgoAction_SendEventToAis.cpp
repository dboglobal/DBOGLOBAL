#include "stdafx.h"
#include "ScriptAlgoAction_SendEventToAis.h"
#include "WPSNodeAction_SendEventToAis.h"


CWpsAlgoAction_SendEventToAis::CWpsAlgoAction_SendEventToAis(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SEND_EVENT_TO_AIS, "SCRIPTCONTROL_ACTION_SEND_EVENT_TO_AIS")
{
}


CWpsAlgoAction_SendEventToAis::~CWpsAlgoAction_SendEventToAis()
{
}


bool CWpsAlgoAction_SendEventToAis::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SendEventToAis* pAction = dynamic_cast<CWPSNodeAction_SendEventToAis*>(pControlScriptNode);
	if (pAction)
	{
		m_eTargetType = pAction->m_eTargetType;
		m_targetTblidx = pAction->m_targetTblidx;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_SendEventToAis", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SendEventToAis::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
