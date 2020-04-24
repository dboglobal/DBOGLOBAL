#include "stdafx.h"
#include "BotAiCondition_SPS_RecvEventFromTs.h"
#include "SPSNodeCondition_RecvEventFromTs.h"



CBotAiCondition_SPS_RecvEventFromTs::CBotAiCondition_SPS_RecvEventFromTs(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_TS, "BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_TS")
{
	m_bCreated = false;
	m_byTriggerType = INVALID_BYTE;
	m_teId = -1;
}

CBotAiCondition_SPS_RecvEventFromTs::~CBotAiCondition_SPS_RecvEventFromTs()
{
}


bool CBotAiCondition_SPS_RecvEventFromTs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeCondition_RecvEventFromTs* pCond = dynamic_cast<CSPSNodeCondition_RecvEventFromTs*>(pControlScriptNode);
	if (pCond)
	{
		m_byTriggerType = pCond->m_byTriggerType;
		m_teId = pCond->m_teId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeCondition_RecvEventFromTs", pControlScriptNode);
	return false;
}

void CBotAiCondition_SPS_RecvEventFromTs::OnEnter()
{
	if (!m_bCreated)
	{
		if (m_teId != -1)
		{
			GetBot()->GetSpsEventMap()->SetScriptEvent(m_teId);
		}

		m_bCreated = true;
	}
}

void CBotAiCondition_SPS_RecvEventFromTs::OnExit()
{
}

int CBotAiCondition_SPS_RecvEventFromTs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (GetBot()->GetSpsEventMap()->IsReceived(m_teId))
	{
		m_status = COMPLETED;
	}

	return m_status;
}

int CBotAiCondition_SPS_RecvEventFromTs::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_TRIGGER_EVENT)
	{
		CObjMsg_TriggerEvent* pEvt = static_cast<CObjMsg_TriggerEvent*>(pObjMsg);
		if (pEvt)
		{
			if (pEvt->byTriggerType == m_byTriggerType && pEvt->teid == m_teId)
				m_status = COMPLETED;
		}
	}

	return m_status;
}