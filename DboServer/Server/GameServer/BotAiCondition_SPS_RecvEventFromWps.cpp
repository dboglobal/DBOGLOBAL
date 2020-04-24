#include "stdafx.h"
#include "BotAiCondition_SPS_RecvEventFromWps.h"
#include "SPSNodeCondition_RecvEventFromWps.h"



CBotAiCondition_SPS_RecvEventFromWps::CBotAiCondition_SPS_RecvEventFromWps(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_WPS, "BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_WPS")
{
	m_bCreated = false;
	m_wpsTblidx = INVALID_TBLIDX;
	m_eventId = INVALID_DWORD;
}

CBotAiCondition_SPS_RecvEventFromWps::~CBotAiCondition_SPS_RecvEventFromWps()
{
}


bool CBotAiCondition_SPS_RecvEventFromWps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_RecvEventFromWps* pCond = dynamic_cast<CSPSNodeCondition_RecvEventFromWps*>(pControlScriptNode);
	if (pCond)
	{
		m_wpsTblidx = pCond->m_wpsTblidx;
		m_eventId = pCond->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeCondition_RecvEventFromWps", pControlScriptNode);
	return false;
}

void CBotAiCondition_SPS_RecvEventFromWps::OnEnter()
{
	if (!m_bCreated)
	{
		if (m_eventId != -1)
		{
			GetBot()->GetSpsEventMap()->SetScriptEvent(m_eventId);
		}

		m_bCreated = true;
	}
}

void CBotAiCondition_SPS_RecvEventFromWps::OnExit()
{
}

int CBotAiCondition_SPS_RecvEventFromWps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetBot()->GetSpsEventMap()->IsReceived(m_eventId))
	{
		//if(m_eventId == 897)
		//	printf("CBotAiCondition_SPS_RecvEventFromWps: received event %u \n", m_eventId);

		m_status = COMPLETED;
	}

	return m_status;
}

int CBotAiCondition_SPS_RecvEventFromWps::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() != OBJMSG_SEND_WPS_EVENT_TO_SPS)
		return m_status;

	CObjMsg_SendWpsEventToSps* pMsg = dynamic_cast<CObjMsg_SendWpsEventToSps*>(pObjMsg);
	if (pMsg)
	{
		if (pMsg->eventID == m_eventId)
		{
		//	printf("CBotAiCondition_SPS_RecvEventFromWps OnObjectMsg: received event %u \n", m_eventId);
			m_status = COMPLETED;
		}
	}

	return m_status;
}