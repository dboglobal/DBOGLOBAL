#include "stdafx.h"
#include "TqsAlgoAction_PlayJingle.h"
#include "TQSNodeAction_PlayJingle.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CTqsAlgoAction_PlayJingle::CTqsAlgoAction_PlayJingle(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_PLAY_JINGLE, "TQS_ALGOCONTROLID_ACTION_PLAY_JINGLE")
{
}


CTqsAlgoAction_PlayJingle::~CTqsAlgoAction_PlayJingle()
{
}


bool CTqsAlgoAction_PlayJingle::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_PlayJingle* pAction = dynamic_cast<CTQSNodeAction_PlayJingle*>(pControlScriptNode);
	if (pAction)
	{
		m_bApplyFadeInOut = pAction->m_bApplyFadeInOut;
		m_strName = pAction->m_strName;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_PlayJingle", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_PlayJingle::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_PLAY_JINGLE));
	sGU_PLAY_JINGLE* res = (sGU_PLAY_JINGLE *)packet.GetPacketData();
	res->wOpCode = GU_PLAY_JINGLE;
	res->bApplyFadeInOut = m_bApplyFadeInOut;
	res->byLength = (BYTE)m_strName.GetString().length();
	strcpy(res->szName, m_strName.c_str());
	packet.SetPacketLen(sizeof(sGU_PLAY_JINGLE));

	GetOwner()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
