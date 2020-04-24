#include "stdafx.h"
#include "BotAiAction_SendSpsEvent.h"
#include "SPSNodeAction_SendSpsEvent.h"
#include "WpsAlgoObject.h"


CBotAiAction_SendSpsEvent::CBotAiAction_SendSpsEvent(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SEND_SPS_EVENT, "BOTCONTROL_ACTION_SEND_SPS_EVENT")
{
	m_eEventType = INVALID_SPS_EVENT_TYPE;
	m_byTriggerType = INVALID_BYTE;
	m_eventID = INVALID_DWORD;
}


CBotAiAction_SendSpsEvent::~CBotAiAction_SendSpsEvent()
{
}

bool CBotAiAction_SendSpsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SendSpsEvent* pAction = dynamic_cast<CSPSNodeAction_SendSpsEvent*>(pControlScriptNode);
	if (pAction)
	{
		m_eEventType = pAction->m_eEventType;
		m_byTriggerType = pAction->m_byTriggerType;
		m_eventID = pAction->m_eventID;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SendSpsEvent", pControlScriptNode);
	return false;
}

void CBotAiAction_SendSpsEvent::OnEnter()
{
}

void CBotAiAction_SendSpsEvent::OnExit()
{
}

int CBotAiAction_SendSpsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	switch (m_eEventType)
	{
		case SPS_EVENT_TYPE_TQS:
		{
			CScriptAlgoObject* pScript = GetBot()->GetCurWorld()->GetFirstScript(SSD_SCRIPT_TYPE_TQS); //Get from world
			if (pScript)
			{
				CObjMsg_TqsEvent cMsg;
				cMsg.eventID = m_eventID;
				pScript->OnObjectMsg(&cMsg);
			}
			else ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR: Bot OBJTYPE %u, Tblidx %u dont have TQS script. Cant send event-id %u", GetBot()->GetObjType(), GetBot()->GetTblidx(), m_eventID);
		}
		break;

		case SPS_EVENT_TYPE_WPS:
		{
			CScriptAlgoObject* pScript = GetBot()->GetFirstScript(SSD_SCRIPT_TYPE_WPS);
			if (pScript)
			{
				CObjMsg_SendSpsEventToWps cMsg;
				cMsg.eventId = m_eventID;
				pScript->OnObjectMsg(&cMsg);
			}
			else
				ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR: Bot OBJTYPE %u, Tblidx %u dont have WPS script. Cant send event-id %u", GetBot()->GetObjType(), GetBot()->GetTblidx(), m_eventID);
		}
		break;

		default: ERR_LOG(LOG_BOTAI, "IMPORTANT-ERROR: m_eEventType '%u' not set", m_eEventType); break;
	}

	m_status = COMPLETED;
	return m_status;
}

