#include "stdafx.h"
#include "BotAiAction_DirectPlay.h"
#include "SPSNodeAction_DirectPlay.h"
#include "TableContainerManager.h"
#include "DirectionLinkTable.h"
#include "NtlPacketGU.h"
#include "NtlDirection.h"


CBotAiAction_DirectPlay::CBotAiAction_DirectPlay(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_DIRECT_PLAY, "BOTCONTROL_ACTION_DIRECT_PLAY")
{
	m_directionTblidx = INVALID_TBLIDX;
	m_byDirectPlayType = DIRECTION_PLAY;
	m_dwPlayTime = INVALID_DWORD;
	m_dwTime = 0;
	m_eEventType = INVALID_SPS_EVENT_TYPE;
	m_eventID = INVALID_DWORD;
}


CBotAiAction_DirectPlay::~CBotAiAction_DirectPlay()
{
}


bool CBotAiAction_DirectPlay::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_DirectPlay* pAction = dynamic_cast<CSPSNodeAction_DirectPlay*>(pControlScriptNode);
	if (pAction)
	{
		m_directionTblidx = pAction->m_directionTblidx;

		if (pAction->m_dwPlayTime == INVALID_DWORD)
		{
			sDIRECTION_LINK_TBLDAT* pTbldat = (sDIRECTION_LINK_TBLDAT*)g_pTableContainer->GetDirectionLinkTable()->FindData(m_directionTblidx);
			if (pTbldat)
			{
				m_dwPlayTime = (DWORD)GetBot()->GetAniTbldat()->GetAnimationLength((ANIMATIONID)pTbldat->dwAnimationID);
			}
			else
			{
				m_dwPlayTime = INVALID_DWORD;
			}
		}
		else
		{
			m_dwPlayTime = pAction->m_dwPlayTime;
		}

		m_eventID = pAction->m_eventID;
		m_eEventType = pAction->m_eEventType;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_DirectPlay", pControlScriptNode);
	return false;
}

void CBotAiAction_DirectPlay::OnEnter()
{
	if (m_directionTblidx != INVALID_TBLIDX)
	{
		GetBot()->StartDirectPlay(DIRECT_PLAY_NORMAL, m_directionTblidx, false, false, m_byDirectPlayType);
	}

	if (m_eventID != INVALID_DWORD)
		GetBot()->GetSpsEventMap()->SetScriptEvent(m_eventID);
}

void CBotAiAction_DirectPlay::OnExit()
{
}


int CBotAiAction_DirectPlay::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_eEventType == INVALID_SPS_EVENT_TYPE && m_eventID == INVALID_DWORD)
	{
		if (m_dwPlayTime == INVALID_DWORD)
		{
			m_status = COMPLETED;
		}
		else
		{
			m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
			if (m_dwTime >= m_dwPlayTime)
				m_status = COMPLETED;
		}
	}
	else
	{
		if (GetBot()->GetSpsEventMap()->IsReceived(m_eventID))
		{
			m_status = COMPLETED;
		}
	}

	return m_status;
}

int CBotAiAction_DirectPlay::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (m_eventID == INVALID_DWORD)
		return m_status;

	if (m_eEventType == SPS_EVENT_TYPE_TS)
	{
		if (pObjMsg->GetID() == OBJMSG_TRIGGER_EVENT)
		{
			CObjMsg_TriggerEvent* pEvt = static_cast<CObjMsg_TriggerEvent*>(pObjMsg);
			if (pEvt)
			{
				if (pEvt->teid == m_eventID)
					m_status = COMPLETED;
			}
		}
	}
	else if (m_eEventType == SPS_EVENT_TYPE_TQS)
	{
		if (pObjMsg->GetID() == OBJMSG_TQS_EVENT)
		{
			CObjMsg_TqsEvent* pEvt = static_cast<CObjMsg_TqsEvent*>(pObjMsg);
			if (pEvt)
			{
				if (pEvt->eventID == m_eventID)
					m_status = COMPLETED;
			}
		}
	}
	else if (m_eEventType == SPS_EVENT_TYPE_WPS)
	{
		if (pObjMsg->GetID() == OBJMSG_WPS_EVENT)
		{
			CObjMsg_WpsEvent* pEvt = static_cast<CObjMsg_WpsEvent*>(pObjMsg);
			if (pEvt)
			{
				if (pEvt->eventID == m_eventID)
					m_status = COMPLETED;
			}
		}
	}

	return m_status;
}