#include "stdafx.h"
#include "ScriptAlgoAction_TelecastMessage.h"
#include "WPSNodeAction_TelecastMessage.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_TelecastMessage::CWpsAlgoAction_TelecastMessage(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_TELECAST_MESSAGE, "SCRIPTCONTROL_ACTION_TELECAST_MESSAGE")
{
}


CWpsAlgoAction_TelecastMessage::~CWpsAlgoAction_TelecastMessage()
{
}


bool CWpsAlgoAction_TelecastMessage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_TelecastMessage* pAction = dynamic_cast<CWPSNodeAction_TelecastMessage*>(pControlScriptNode);
	if (pAction)
	{
		m_bApply = pAction->m_bApply;
		m_npcTblidx = pAction->m_npcTblidx;
		m_eTelecastType = pAction->m_eTelecastType;
		m_speechTblidx = pAction->m_speechTblidx;
		m_dwDisplayTime = pAction->m_dwDisplayTime;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_TelecastMessage", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_TelecastMessage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_TELECAST_MESSAGE_BEG_NFY));
	sGU_TELECAST_MESSAGE_BEG_NFY* res = (sGU_TELECAST_MESSAGE_BEG_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TELECAST_MESSAGE_BEG_NFY;
	res->byTelecastType = m_eTelecastType;
	res->dwDisplayTime = m_dwDisplayTime;
	res->npcTblidx = m_npcTblidx;
	res->speechTblidx = m_speechTblidx;
	packet.SetPacketLen(sizeof(sGU_TELECAST_MESSAGE_BEG_NFY));

	if (GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_STATIC_WORLD
		|| GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_DYNAMIC_WORLD
		|| GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_OBJECTTRIGGER
		)
		GetOwner()->GetWorld()->Broadcast(&packet);
	else
	{
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer)
		{
			pPlayer->SendPacket(&packet);

			pPlayer = GetOwner()->GetPlayersNext();
		}
	}

	m_status = COMPLETED;
	return m_status;
}
