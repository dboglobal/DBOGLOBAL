#include "stdafx.h"
#include "BotAiAction_Escort.h"
#include "SPSNodeAction_Escort.h"
#include "BotAiAction_EscortFollow.h"
#include "BotAiState.h"


CBotAiAction_Escort::CBotAiAction_Escort(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_DIRECT_PLAY, "BOTCONTROL_ACTION_DIRECT_PLAY")
{
	m_eEscortType = INVALID_ESCORT_TYPE;
	m_fRadius = 5.0f;
	m_bRunMode = false;
	m_eventID = INVALID_DWORD;
}


CBotAiAction_Escort::~CBotAiAction_Escort()
{
}


bool CBotAiAction_Escort::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_Escort* pAction = dynamic_cast<CSPSNodeAction_Escort*>(pControlScriptNode);
	if (pAction)
	{
		m_eEscortType = pAction->m_eEscortType;
		m_vDestLoc = pAction->m_vDestLoc;
		m_fRadius = pAction->m_fRadius;
		m_bRunMode = pAction->m_bRunMode;
		m_eventID = pAction->m_eventID;
		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_Escort", pControlScriptNode);
	return false;
}

void CBotAiAction_Escort::OnEnter()
{
	GetBot()->GetEscortManager()->Clear();
}

void CBotAiAction_Escort::OnExit()
{
}


int CBotAiAction_Escort::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}

int CBotAiAction_Escort::OnObjectMsg(CObjectMsg* pObjMsg) //on retail npc-server its OnEvent but we use objectmsg same as event because makes no sense to create event
{
	if (pObjMsg->GetID() != OBJMSG_START_ESCORT)
		return m_status;

	CObjMsg_StartEscort* pMsg = check_cast<CObjMsg_StartEscort *>(pObjMsg);
	if (!pMsg)
		return m_status;
	
	if (pMsg->m_byEscortType != m_eEscortType)
		return m_status;
	
	if (m_eEscortType == ESCORT_TYPE_TARGET_FOLLOW)
	{
		CBotAiState* pCurState = GetBot()->GetBotController()->GetCurrentState();
		if (pCurState == NULL)
			return m_status;
		
		CBotAiAction_EscortFollow* pEscortFollow = new CBotAiAction_EscortFollow(GetBot(), m_vDestLoc, m_fRadius, m_bRunMode, m_eventID);
		if (pCurState->AddSubControlQueue(pEscortFollow, true))
		{
			m_status = FAILED;
			return m_status;
		}
	}

	m_status = COMPLETED;
	return m_status;
}