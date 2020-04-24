#include "stdafx.h"
#include "ScriptAlgoAction_SendEventToTqs.h"
#include "WPSNodeAction_SendEventToTqs.h"


CWpsAlgoAction_SendEventToTqs::CWpsAlgoAction_SendEventToTqs(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SEND_EVENT_TO_TQS, "SCRIPTCONTROL_ACTION_SEND_EVENT_TO_TQS")
{
}


CWpsAlgoAction_SendEventToTqs::~CWpsAlgoAction_SendEventToTqs()
{
}


bool CWpsAlgoAction_SendEventToTqs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SendEventToTqs* pAction = dynamic_cast<CWPSNodeAction_SendEventToTqs*>(pControlScriptNode);
	if (pAction)
	{
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_SendEventToTqs", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SendEventToTqs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
