#include "stdafx.h"
#include "ScriptAlgoAction_BroadMessage.h"
#include "WPSNodeAction_BroadMessage.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"

CWpsAlgoAction_BroadMessage::CWpsAlgoAction_BroadMessage(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_BROAD_MESSAGE, "SCRIPTCONTROL_ACTION_BROAD_MESSAGE")
{
}


CWpsAlgoAction_BroadMessage::~CWpsAlgoAction_BroadMessage()
{
}


bool CWpsAlgoAction_BroadMessage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_BroadMessage* pAction = dynamic_cast<CWPSNodeAction_BroadMessage*>(pControlScriptNode);
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

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_BroadMessage", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_BroadMessage::OnUpdate(DWORD dwTickDiff, float fMultiple)
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
	packet.SetPacketLen(sizeof(sGU_PLAY_BGM));

	if (GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_STATIC_WORLD || GetOwner()->GetExecuterType() == WORLDPLAY_EXECUTER_DYNAMIC_WORLD)
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
