#include "stdafx.h"
#include "ScriptAlgoAction_SystemMessage.h"
#include "WPSNodeAction_SystemMessage.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_SystemMessage::CWpsAlgoAction_SystemMessage(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_SYSTEM_MESSAGE, "SCRIPTCONTROL_ACTION_SYSTEM_MESSAGE")
{
}


CWpsAlgoAction_SystemMessage::~CWpsAlgoAction_SystemMessage()
{
}


bool CWpsAlgoAction_SystemMessage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_SystemMessage* pAction = dynamic_cast<CWPSNodeAction_SystemMessage*>(pControlScriptNode);
	if (pAction)
	{
		m_eDisplayType = pAction->m_eDisplayType;
		m_textTblidx = pAction->m_textTblidx;
		m_wMessageLengthInUnicode = pAction->m_wMessageLengthInUnicode;
		NTL_SAFE_WCSCPY(m_awchMessage, pAction->m_awchMessage);

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_SystemMessage", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_SystemMessage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_textTblidx != INVALID_TBLIDX)
	{
		CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_SCRIPT_TEXT));
		sGU_SYSTEM_DISPLAY_SCRIPT_TEXT* res = (sGU_SYSTEM_DISPLAY_SCRIPT_TEXT *)packet.GetPacketData();
		res->wOpCode = GU_SYSTEM_DISPLAY_SCRIPT_TEXT;
		res->byDisplayType = m_eDisplayType;
		res->textTblidx = m_textTblidx;

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
	}
	else
	{
		CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
		sGU_SYSTEM_DISPLAY_TEXT* res = (sGU_SYSTEM_DISPLAY_TEXT *)packet.GetPacketData();
		res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
		res->byDisplayType = m_eDisplayType;
		res->wMessageLengthInUnicode = m_wMessageLengthInUnicode;
		NTL_SAFE_WCSCPY(res->awchMessage, m_awchMessage);

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
	}

	m_status = COMPLETED;
	return m_status;
}
