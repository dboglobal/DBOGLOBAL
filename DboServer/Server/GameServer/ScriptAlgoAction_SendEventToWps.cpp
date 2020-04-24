#include "stdafx.h"
#include "ScriptAlgoAction_SendEventToWps.h"
#include "WPSNodeAction_SendEventToWps.h"
#include "ObjectMsg.h"
#include "World.h"

CWpsAlgoAction_SendEventToWps::CWpsAlgoAction_SendEventToWps(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SEND_EVENT_TO_WPS, "SCRIPTCONTROL_ACTION_SEND_EVENT_TO_WPS")
{
}


CWpsAlgoAction_SendEventToWps::~CWpsAlgoAction_SendEventToWps()
{
}


bool CWpsAlgoAction_SendEventToWps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SendEventToWps* pAction = dynamic_cast<CWPSNodeAction_SendEventToWps*>(pControlScriptNode);
	if (pAction)
	{
		m_wpsTblidx = pAction->m_wpsTblidx;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_SendEventToWps", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SendEventToWps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_STATIC_WORLD 
		|| GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_DYNAMIC_WORLD
		|| GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_OBJECTTRIGGER
		)
	{
		CScriptAlgoObject* pObj = GetOwner()->GetWorld()->GetScript(m_wpsTblidx);
		if (pObj)
		{
			CObjMsg_SendWpsEventToWps pMsg;
			pMsg.eventId = m_eventId;
			pMsg.wpsTblidx = GetOwner()->GetScriptID();
			pObj->OnObjectMsg(&pMsg);

			m_status = COMPLETED;
		}
		else
		{
			//ERR_LOG(LOG_SCRIPT, "ERROR! Could not find WPS %u. Script %u", m_wpsTblidx, GetOwner()->GetScriptID());
			m_status = COMPLETED;
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "ERROR: GetExecuterType != WORLDPLAY_EXECUTER_STATIC_WORLD && != WORLDPLAY_EXECUTER_DYNAMIC_WORLD && != WORLDPLAY_EXECUTER_OBJECTTRIGGER. Script %u", m_wpsTblidx, GetOwner()->GetScriptID());
	//	m_status = SYSTEMERROR;
	}

	return m_status;
}
