#include "stdafx.h"
#include "TqsAlgoAction_Reward.h"
#include "TQSNodeAction_Reward.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "TimeQuest.h"


CTqsAlgoAction_Reward::CTqsAlgoAction_Reward(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_PROGRESS_MESSAGE, "TQS_ALGOCONTROLID_ACTION_PROGRESS_MESSAGE")
{
	m_dwWaitTime = 60000;
}


CTqsAlgoAction_Reward::~CTqsAlgoAction_Reward()
{
}


bool CTqsAlgoAction_Reward::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_Reward* pNode = dynamic_cast<CTQSNodeAction_Reward*>(pControlScriptNode);
	if (pNode)
	{
		m_byTriggerType = pNode->m_byTriggerType;
		m_teid = pNode->m_teid;
		m_dwWaitTime = pNode->m_dwWaitTime;	//timer for leaving TMQ

		return true;
	}

	return false;
}


void CTqsAlgoAction_Reward::OnEnter()
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

	CNtlPacket packet(sizeof(sGU_TIMEQUEST_REWARD_NFY));
	sGU_TIMEQUEST_REWARD_NFY * res = (sGU_TIMEQUEST_REWARD_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TIMEQUEST_REWARD_NFY;
	res->byTriggerType = m_byTriggerType;
	res->teid = m_teid;
	res->dwWaitTime = m_dwWaitTime;
	//res->bIsBestRecord = true;
	//res->bIsDayRecord = true;
	GetOwner()->Broadcast(&packet);
}


int CTqsAlgoAction_Reward::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);

	if (m_dwWaitTime == 0)
	{
		m_status = COMPLETED;
	}

	return m_status;
}
