#include "stdafx.h"
#include "ScriptAlgoAction_SendAISEvent.h"
#include "WPSNodeAction_SendAISEvent.h"


CWpsAlgoAction_SendAISEvent::CWpsAlgoAction_SendAISEvent(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SEND_AIS_EVENT, "SCRIPTCONTROL_ACTION_SEND_AIS_EVENT")
{
}


CWpsAlgoAction_SendAISEvent::~CWpsAlgoAction_SendAISEvent()
{
}


bool CWpsAlgoAction_SendAISEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SendAISEvent* pAction = dynamic_cast<CWPSNodeAction_SendAISEvent*>(pControlScriptNode);
	if (pAction)
	{
		m_eObjType = pAction->m_eObjType;
		m_targetTblidx = pAction->m_targetTblidx;
		m_eventID = pAction->m_eventID;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_SendAISEvent", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SendAISEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
