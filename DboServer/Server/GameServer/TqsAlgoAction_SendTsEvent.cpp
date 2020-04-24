#include "stdafx.h"
#include "TqsAlgoAction_SendTsEvent.h"
#include "TQSNodeAction_SendTsEvent.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CTqsAlgoAction_SendTsEvent::CTqsAlgoAction_SendTsEvent(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_SEND_TS_EVENT, "TQS_ALGOCONTROLID_ACTION_SEND_TS_EVENT")
{

}


CTqsAlgoAction_SendTsEvent::~CTqsAlgoAction_SendTsEvent()
{
}


bool CTqsAlgoAction_SendTsEvent::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_SendTsEvent* pNode = dynamic_cast<CTQSNodeAction_SendTsEvent*>(pControlScriptNode);
	if (pNode)
	{
		m_eSendType = pNode->m_eSendType;
		m_byTriggerType = pNode->m_byTriggerType;
		m_teid = pNode->m_teid;
		m_vOrigin = pNode->m_vOrigin;
		m_fEffectRadius = pNode->m_fEffectRadius;

		return true;
	}
	return false;
}

int CTqsAlgoAction_SendTsEvent::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID()) //check if player not in world or not in range
		{
			pPlayer->GetQuests()->GetEventMap()->OnEventReceived(m_teid);
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}

	CNtlPacket packet(sizeof(sGU_TS_UPDATE_EVENT_NFY));
	sGU_TS_UPDATE_EVENT_NFY * res = (sGU_TS_UPDATE_EVENT_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TS_UPDATE_EVENT_NFY;
	res->byTsType = m_byTriggerType;
	res->teid = m_teid;

	GetOwner()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
