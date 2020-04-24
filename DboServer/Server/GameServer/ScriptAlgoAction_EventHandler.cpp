#include "stdafx.h"
#include "ScriptAlgoAction_EventHandler.h"
#include "WPSNodeAction_EventHandler.h"


CWpsAlgoAction_EventHandler::CWpsAlgoAction_EventHandler(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EVENT_HANDLER, "SCRIPTCONTROL_ACTION_EVENT_HANDLER")
{
}


CWpsAlgoAction_EventHandler::~CWpsAlgoAction_EventHandler()
{
}


bool CWpsAlgoAction_EventHandler::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_EventHandler* pAction = dynamic_cast<CWPSNodeAction_EventHandler*>(pControlScriptNode);
	if (pAction)
	{
		m_eEventType = pAction->m_eEventType;
		m_tblidx = pAction->m_tblidx;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_EventHandler", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_EventHandler::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
