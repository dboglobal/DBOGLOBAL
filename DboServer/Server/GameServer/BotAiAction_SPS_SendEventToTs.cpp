#include "stdafx.h"
#include "BotAiAction_SPS_SendEventToTs.h"
#include "SPSNodeAction_SendEventToTs.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CBotAiAction_SPS_SendEventToTs::CBotAiAction_SPS_SendEventToTs(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_SPS_SEND_EVENT_TO_TS, "BOTCONTROL_ACTION_SPS_SEND_EVENT_TO_TS")
{
	m_eTriggerType = CSPSNodeAction_SendEventToTs::INVALID_TRIGGERTYPE;
	m_eventId = INVALID_DWORD;
}


CBotAiAction_SPS_SendEventToTs::~CBotAiAction_SPS_SendEventToTs()
{
}


bool CBotAiAction_SPS_SendEventToTs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SendEventToTs* pAction = dynamic_cast<CSPSNodeAction_SendEventToTs*>(pControlScriptNode);
	if (pAction)
	{
		m_eTriggerType = pAction->m_eTriggerType;
		m_eventId = pAction->m_eventId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SendEventToTs", pControlScriptNode);
	return false;
}


void CBotAiAction_SPS_SendEventToTs::OnEnter()
{
}

void CBotAiAction_SPS_SendEventToTs::OnExit()
{
}


int CBotAiAction_SPS_SendEventToTs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_eTriggerType == CSPSNodeAction_SendEventToTs::TRIGGER_TYPE_QUEST)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(GetBot()->GetLinkPc());

		if (pPlayer == NULL || !pPlayer->IsInitialized() || pPlayer->GetCharID() != GetBot()->GetLinkPcId())
		{
			m_status = COMPLETED;
			return m_status;
		}

		pPlayer->GetQuests()->UpdateQuestEvt(m_eventId);
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "IMPORTANT-ERROR: m_eTriggerType %u not set yet. SPS Script %u, event %u", m_eTriggerType, GetBot()->GetPlayScript(), m_eventId);
	}
	
	m_status = COMPLETED;
	return m_status;
}