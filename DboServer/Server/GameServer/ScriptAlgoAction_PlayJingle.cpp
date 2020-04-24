#include "stdafx.h"
#include "ScriptAlgoAction_PlayJingle.h"
#include "WPSNodeAction_PlayJingle.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_PlayJingle::CWpsAlgoAction_PlayJingle(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_PLAY_JINGLE, "SCRIPTCONTROL_ACTION_PLAY_JINGLE")
{
}


CWpsAlgoAction_PlayJingle::~CWpsAlgoAction_PlayJingle()
{
}


bool CWpsAlgoAction_PlayJingle::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_PlayJingle* pAction = dynamic_cast<CWPSNodeAction_PlayJingle*>(pControlScriptNode);
	if (pAction)
	{
		m_bApplyFadeInOut = pAction->m_bApplyFadeInOut;
		m_strName = pAction->m_strName;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_PlayJingle", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_PlayJingle::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_PLAY_JINGLE));
	sGU_PLAY_JINGLE* res = (sGU_PLAY_JINGLE *)packet.GetPacketData();
	res->wOpCode = GU_PLAY_JINGLE;
	res->bApplyFadeInOut = m_bApplyFadeInOut;
	res->byLength = (BYTE)(m_strName.GetString().length());
	strcpy(res->szName, m_strName.c_str());
	packet.SetPacketLen(sizeof(sGU_PLAY_JINGLE));

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
