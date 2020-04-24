#include "stdafx.h"
#include "TqsAlgoAction_PlayBgm.h"
#include "TQSNodeAction_PlayBgm.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CTqsAlgoAction_PlayBgm::CTqsAlgoAction_PlayBgm(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_PLAY_BGM, "TQS_ALGOCONTROLID_ACTION_PLAY_BGM")
{
}


CTqsAlgoAction_PlayBgm::~CTqsAlgoAction_PlayBgm()
{
}


bool CTqsAlgoAction_PlayBgm::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_PlayBgm* pAction = dynamic_cast<CTQSNodeAction_PlayBgm*>(pControlScriptNode);
	if (pAction)
	{
		m_bPlay = pAction->m_bPlay;
		m_strName = pAction->m_strName;
		m_bLoop = pAction->m_bLoop;
		m_bPrevBgmFadeOut = pAction->m_bPrevBgmFadeOut;
		m_dwDelay = pAction->m_dwDelay;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_PlayBgm", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_PlayBgm::OnUpdate(DWORD dwTickDiff, float fMultiple)
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

	GetOwner()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
