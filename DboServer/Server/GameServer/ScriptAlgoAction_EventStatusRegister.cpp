#include "stdafx.h"
#include "ScriptAlgoAction_EventStatusRegister.h"
#include "WPSNodeAction_EventStatusRegister.h"


CWpsAlgoAction_EventStatusRegister::CWpsAlgoAction_EventStatusRegister(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EVENT_STATUS_REGISTER, "SCRIPTCONTROL_ACTION_EVENT_STATUS_REGISTER")
{
}


CWpsAlgoAction_EventStatusRegister::~CWpsAlgoAction_EventStatusRegister()
{
}


bool CWpsAlgoAction_EventStatusRegister::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_EventStatusRegister* pAction = dynamic_cast<CWPSNodeAction_EventStatusRegister*>(pControlScriptNode);
	if (pAction)
	{
		m_sEventStatusData = pAction->m_sEventStatusData;
		m_eType = pAction->m_eType;
		m_mobSpawnGroupId = pAction->m_mobSpawnGroupId;
		m_eventId = pAction->m_eventId;
		m_wMaxCount = pAction->m_wMaxCount;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_EventStatusClear", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_EventStatusRegister::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
