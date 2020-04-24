#include "stdafx.h"
#include "TqsAlgoAction_TimeCountdown.h"
#include "TQSNodeAction_TimeCountdown.h"
#include "TimeQuest.h"


CTqsAlgoAction_TimeCountdown::CTqsAlgoAction_TimeCountdown(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_TIME_COUNTDOWN, "TQS_ALGOCONTROLID_ACTION_TIME_COUNTDOWN")
{
}


CTqsAlgoAction_TimeCountdown::~CTqsAlgoAction_TimeCountdown()
{
}


bool CTqsAlgoAction_TimeCountdown::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_TimeCountdown* pAction = dynamic_cast<CTQSNodeAction_TimeCountdown*>(pControlScriptNode);
	if (pAction)
	{
		m_bCountdown = pAction->m_bCountdown;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_TimeCountdown", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_TimeCountdown::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_bCountdown)
	{
		GetOwner()->GetTimeQuest()->SetCountdownStart(time(NULL));
	}
	else
	{
		GetOwner()->GetTimeQuest()->SetCountdownEnd(time(NULL));
	}

	m_status = COMPLETED;
	return m_status;
}
