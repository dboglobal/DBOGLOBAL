#include "stdafx.h"
#include "ScriptAlgoAction_PlayBgm.h"
#include "WPSNodeAction_PlayBgm.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_PlayBgm::CWpsAlgoAction_PlayBgm(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_PLAY_BGM, "SCRIPTCONTROL_ACTION_PLAY_BGM")
{
}


CWpsAlgoAction_PlayBgm::~CWpsAlgoAction_PlayBgm()
{
}


bool CWpsAlgoAction_PlayBgm::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_PlayBgm* pAction = dynamic_cast<CWPSNodeAction_PlayBgm*>(pControlScriptNode);
	if (pAction)
	{
		m_bPlay = pAction->m_bPlay;
		m_strName = pAction->m_strName;
		m_bLoop = pAction->m_bLoop;
		m_bPrevBgmFadeOut = pAction->m_bPrevBgmFadeOut;
		m_dwDelay = pAction->m_dwDelay;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_PlayBgm", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_PlayBgm::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_PLAY_BGM));
	sGU_PLAY_BGM* res = (sGU_PLAY_BGM *)packet.GetPacketData();
	res->wOpCode = GU_PLAY_BGM;
	res->bLoop = m_bLoop;
	res->bPlay = m_bPlay;
	res->bPrevBgmFadeOut = m_bPrevBgmFadeOut;
//	res->byLength
	res->dwDelay = m_dwDelay;
	strcpy(res->szName, m_strName.c_str());
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
