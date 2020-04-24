#include "stdafx.h"
#include "TqsAlgoAction_ProgressMessage.h"
#include "TQSNodeAction_ProgressMessage.h"
#include "NtlDirection.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "TimeQuest.h"
#include "NtlResultCode.h"


CTqsAlgoAction_ProgressMessage::CTqsAlgoAction_ProgressMessage(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_PROGRESS_MESSAGE, "TQS_ALGOCONTROLID_ACTION_PROGRESS_MESSAGE")
{

}


CTqsAlgoAction_ProgressMessage::~CTqsAlgoAction_ProgressMessage()
{
}


bool CTqsAlgoAction_ProgressMessage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_ProgressMessage* pNode = dynamic_cast<CTQSNodeAction_ProgressMessage*>(pControlScriptNode);
	if (pNode)
	{
		m_eProgressType = pNode->m_eProgressType;
		m_eMessageType = pNode->m_eMessageType;
		m_byMessageValue = pNode->m_byMessageValue;

		return true;
	}
	return false;
}

int CTqsAlgoAction_ProgressMessage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	GetOwner()->GetTimeQuest()->SetStage(m_byMessageValue);

	if (m_eProgressType == PROGRESS_MESSAGE_TYPE_TMQ)
	{
		if (m_eMessageType == TIMEQUEST_PROGRESS_MESSAGE_TYPE_START)
		{
			GetOwner()->GetTimeQuest()->SetState(TIMEQUEST_GAME_STATE_STAGE_READY);
		}
	}
	else
	{
		if (m_eMessageType == TIMEQUEST_PROGRESS_MESSAGE_TYPE_START)
		{
			GetOwner()->GetTimeQuest()->SetState(TIMEQUEST_GAME_STATE_STAGE_START);
		}
		else if (m_eMessageType == TIMEQUEST_PROGRESS_MESSAGE_TYPE_CLEAR)
		{
			GetOwner()->GetTimeQuest()->SetState(TIMEQUEST_GAME_STATE_STAGE_FINISH);
		}
	}

	CNtlPacket packet(sizeof(sGU_PROGRESS_MESSAGE_NFY));
	sGU_PROGRESS_MESSAGE_NFY * res = (sGU_PROGRESS_MESSAGE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PROGRESS_MESSAGE_NFY;
	res->byMessageType = m_eMessageType;
	res->byMessageValue = m_byMessageValue;
	res->byProgressType = m_eProgressType;
	GetOwner()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
