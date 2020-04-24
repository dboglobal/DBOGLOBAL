#include "stdafx.h"
#include "BotAiCondition_AIS_RecvEventFromWps.h"
#include "AISNodeCondition_RecvEventFromWPS.h"



CBotAiCondition_AIS_RecvEventFromWps::CBotAiCondition_AIS_RecvEventFromWps(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_WPS, "BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_WPS")
{
	m_wpsTblidx = INVALID_TBLIDX;
	m_eventId = INVALID_DWORD;
	m_scriptnodeID = INVALID_DWORD;
}

CBotAiCondition_AIS_RecvEventFromWps::~CBotAiCondition_AIS_RecvEventFromWps()
{
}

bool CBotAiCondition_AIS_RecvEventFromWps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CAISNodeCondition_RecvEventFromWPS* pCond = dynamic_cast<CAISNodeCondition_RecvEventFromWPS*>(pControlScriptNode);
	if (pCond)
	{
		m_wpsTblidx = pCond->m_wpsTblidx;
		m_eventId = pCond->m_eventId;
		m_scriptnodeID = pControlScriptNode->GetNodeIndex();

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeCondition_RecvEventFromWPS", pControlScriptNode);
	return false;
}

void CBotAiCondition_AIS_RecvEventFromWps::CopyTo(CControlState* pTo)
{
}

void CBotAiCondition_AIS_RecvEventFromWps::OnEnter()
{
}

void CBotAiCondition_AIS_RecvEventFromWps::OnExit()
{
}

int CBotAiCondition_AIS_RecvEventFromWps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	ERR_LOG(LOG_BOTAI, "ERROR TO DO");
	return m_status;
}

int CBotAiCondition_AIS_RecvEventFromWps::OnObjectMsg(CObjectMsg* pObjMsg)
{
	ERR_LOG(LOG_BOTAI, "ERROR TO DO");
	return m_status;
}