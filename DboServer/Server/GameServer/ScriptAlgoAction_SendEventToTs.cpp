#include "stdafx.h"
#include "ScriptAlgoAction_SendEventToTs.h"
#include "WPSNodeAction_SendEventToTs.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_SendEventToTs::CWpsAlgoAction_SendEventToTs(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SEND_EVENT_TO_TS, "SCRIPTCONTROL_ACTION_SEND_EVENT_TO_TS")
{

}


CWpsAlgoAction_SendEventToTs::~CWpsAlgoAction_SendEventToTs()
{
}


bool CWpsAlgoAction_SendEventToTs::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SendEventToTs* pAction = dynamic_cast<CWPSNodeAction_SendEventToTs*>(pControlScriptNode);
	if (pAction)
	{
		m_byTriggerType = pAction->m_byTriggerType;
		m_eApplyType = pAction->m_eApplyType;
		m_byTargetType = pAction->m_byTargetType;
		m_targetTblidx = pAction->m_targetTblidx;
		m_vLoc = pAction->m_vLoc;
		m_fRadius = pAction->m_fRadius;
		m_teId = pAction->m_teId;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_RemoveMobGroup", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SendEventToTs::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_TS_UPDATE_EVENT_NFY));
	sGU_TS_UPDATE_EVENT_NFY * res = (sGU_TS_UPDATE_EVENT_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TS_UPDATE_EVENT_NFY;
	res->byTsType = m_byTriggerType;
	res->teid = m_teId;

	switch (m_eApplyType)
	{
		case SSD_TSEVENT_APPLY_TYPE_REGISTER_ALL:
		{
			CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
			while (pPlayer)
			{
				if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID()) //check if player not in world or not in range
				{
					pPlayer->GetQuests()->GetEventMap()->OnEventReceived(m_teId);
				}

				pPlayer = GetOwner()->GetPlayersNext();
			}

			GetOwner()->Broadcast(&packet);
		}
		break;
		
		default:
		{
			ERR_LOG(LOG_SCRIPT, "Script-ID %u have valid ApplyType %u", GetOwner()->GetScriptID(), m_eApplyType);
		}
		break;
	}

	m_status = COMPLETED;
	return m_status;
}
