#include "stdafx.h"
#include "BotAiAction_Wait.h"
#include "SPSNodeAction_Wait.h"


CBotAiAction_Wait::CBotAiAction_Wait(CNpc* pBot)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_WAIT, "BOTCONTROL_ACTION_WAIT")
{
	m_dwTime = 0;
	m_dwWaitTime = INVALID_DWORD;
	m_eEventType = INVALID_SPS_EVENT_TYPE;
	m_eventID = INVALID_DWORD;
	m_bSetCondCount = false;
	m_bOperationAND = true;
	m_nCondCount = 0;
}

CBotAiAction_Wait::~CBotAiAction_Wait()
{
}


bool CBotAiAction_Wait::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_Wait* pAction = dynamic_cast<CSPSNodeAction_Wait*>(pControlScriptNode);
	if (pAction)
	{
		m_dwWaitTime = pAction->m_dwWaitTime;
		m_eventID = pAction->m_eventID;
		m_eEventType = pAction->m_eEventType;
		m_bOperationAND = pAction->m_bOperationAND;
		m_bSetCondCount = false;
		m_nCondCount = 0;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_Wait", pControlScriptNode);
	return false;
}

void CBotAiAction_Wait::OnEnter()
{
	if (!m_bSetCondCount)
	{
		m_nCondCount = m_subControlList.GetCount();
		m_bSetCondCount = true;

		if (m_eventID != INVALID_DWORD)
		{
			GetBot()->GetSpsEventMap()->SetScriptEvent(m_eventID);
		}
	}
}

void CBotAiAction_Wait::OnExit()
{
}


int CBotAiAction_Wait::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_nCondCount)
	{
		CComplexState::UpdateSubControlList(dwTickDiff, fMultiple);
		if (m_bOperationAND)
		{
			if (m_subControlList.GetCount() == 0)
				m_status = COMPLETED;
		}
		else if (m_nCondCount != m_subControlList.GetCount())
		{
			m_status = COMPLETED;
		}
	}
	else
	{
		if (GetBot()->GetSpsEventMap()->IsReceived(m_eventID))
		{
			m_status = COMPLETED;
		}
		else if (m_dwWaitTime != INVALID_DWORD && m_dwTime >= m_dwWaitTime)
		{
			m_status = COMPLETED;
		}
	}

	return m_status;
}

int CBotAiAction_Wait::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (m_nCondCount)
	{
		CComplexState::OnObjectMsg(pObjMsg);
	}
	else if (m_eventID != INVALID_DWORD)
	{
		switch (m_eEventType)
		{
			case SPS_EVENT_TYPE_TS:
			{
				if (pObjMsg->GetID() == OBJMSG_TRIGGER_EVENT)
				{
					CObjMsg_TriggerEvent* pTrig = check_cast<CObjMsg_TriggerEvent *>(pObjMsg);
					if (pTrig)
					{
						if (pTrig->teid == m_eventID)
							m_status = COMPLETED;
					}
				}
			}
			break;
			case SPS_EVENT_TYPE_TQS:
			{
				if (pObjMsg->GetID() == OBJMSG_TQS_EVENT)
				{
					CObjMsg_TqsEvent* pTqs = static_cast<CObjMsg_TqsEvent *>(pObjMsg);
					if (pTqs)
					{
						if (pTqs->eventID == m_eventID)
							m_status = COMPLETED;
					}
				}
			}
			break;
			case SPS_EVENT_TYPE_WPS:
			{
				if (pObjMsg->GetID() == OBJMSG_WPS_EVENT)
				{
					CObjMsg_WpsEvent* pWps = check_cast<CObjMsg_WpsEvent *>(pObjMsg);
					if (pWps)
					{
						if (pWps->eventID == m_eventID)
							m_status = COMPLETED;
					}
				}
			}
			break;

			default: ERR_LOG(LOG_SCRIPT, "m_eEventType not set %u", m_eEventType); break;
		}
	}

	return m_status;
}
