#include "stdafx.h"
#include "TqsAlgoAction_BroadMessage.h"
#include "TQSNodeAction_BroadMessage.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"

CTqsAlgoAction_BroadMessage::CTqsAlgoAction_BroadMessage(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_BROAD_MESSAGE, "TQS_ALGOCONTROLID_ACTION_BROAD_MESSAGE")
{
}


CTqsAlgoAction_BroadMessage::~CTqsAlgoAction_BroadMessage()
{
}


bool CTqsAlgoAction_BroadMessage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_BroadMessage* pAction = dynamic_cast<CTQSNodeAction_BroadMessage*>(pControlScriptNode);
	if (pAction)
	{
		m_eExecutionType = pAction->m_eExecutionType;
		m_eUIDirType = pAction->m_eUIDirType;
		m_eUIShowHideType = pAction->m_eUIShowHideType;
		m_eUIBalloonShapeType = pAction->m_eUIBalloonShapeType;
		m_eUISpeechDirType = pAction->m_eUISpeechDirType;
		m_eOwnerCondition = pAction->m_eOwnerCondition;
		m_eOwnerType = pAction->m_eOwnerType;
		m_uiOwnerTblIdx = pAction->m_uiOwnerTblIdx;
		m_uiQuestTextTblidx = pAction->m_uiQuestTextTblidx;
		m_fDisplayTime = pAction->m_fDisplayTime;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_BroadMessage", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_BroadMessage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_BROAD_MESSAGE_BEG_NFY));
	sGU_BROAD_MESSAGE_BEG_NFY* res = (sGU_BROAD_MESSAGE_BEG_NFY *)packet.GetPacketData();
	res->wOpCode = GU_BROAD_MESSAGE_BEG_NFY;
	res->byOwnerCondition = m_eOwnerCondition;
	res->byOwnerType = m_eOwnerType;
	res->byUIBalloonShapeType = m_eUIBalloonShapeType;
	res->byUIDirType = m_eUIDirType;
	res->byUIShowHideTypel = m_eUIShowHideType;
	res->byUISpeechDirType = m_eUISpeechDirType;
	res->fDisplayTime = m_fDisplayTime;
	res->uiOwnerTblIdx = m_uiOwnerTblIdx;
	res->uiQuestTextTblidx = m_uiQuestTextTblidx;

	GetOwner()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
