#include "stdafx.h"
#include "BotAiAction_Say.h"
#include "NtlPacketGU.h"
#include "SPSNodeAction_Say.h"
#include "SvrScrVariableMap.h"
#include "NpcSpeechTable.h"
#include "TableContainerManager.h"

CBotAiAction_Say::CBotAiAction_Say(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_DIALOG, "BOTCONTROL_ACTION_DIALOG")
{
	m_byDialogType = INVALID_CHAR_DIALOG;
	m_textTblidx = INVALID_TBLIDX;
}


CBotAiAction_Say::~CBotAiAction_Say()
{
}


bool CBotAiAction_Say::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_Say* pActionSay = dynamic_cast<CSPSNodeAction_Say*>(pControlScriptNode);
	if (pActionSay)
	{
		m_byDialogType = pActionSay->m_byDialogType;
		m_textTblidx = pActionSay->m_textTblidx;
		m_strText.operator=(pActionSay->m_strText.c_str());
		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_Say", pControlScriptNode);
	return false;
}

void CBotAiAction_Say::OnEnter()
{
}

void CBotAiAction_Say::OnExit()
{
}


int CBotAiAction_Say::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_CHAR_DIALOG));
	sGU_CHAR_DIALOG * res = (sGU_CHAR_DIALOG *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_DIALOG;
	res->hSubject = GetBot()->GetID();
	res->byDialogType = (BYTE)m_byDialogType;
	res->textTblidx = m_textTblidx;
	res->bIsRefSpeechTable = false;

	if (m_textTblidx != INVALID_TBLIDX)
	{
		sNPC_SPEECH_TBLDAT* pTbldat = (sNPC_SPEECH_TBLDAT*)g_pTableContainer->GetNpcSpeechTable()->FindData(m_textTblidx);
		if (pTbldat)
		{
			res->bIsRefSpeechTable = true;
		}
	}

	res->wTextLen = (WORD)m_strText.GetString().length();

	wcscpy_s(res->awchText, NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1, m_strText.c_str());
	packet.SetPacketLen(sizeof(sGU_CHAR_DIALOG));
	GetBot()->Broadcast(&packet, GetBot());

	m_status = COMPLETED;
	return m_status;
}
