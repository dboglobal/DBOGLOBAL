#include "stdafx.h"
#include "ScriptAlgoAction_FlashPlay.h"
#include "WPSNodeAction_FlashPlay.h"


CWpsAlgoAction_FlashPlay::CWpsAlgoAction_FlashPlay(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_FLASH_PLAY, "SCRIPTCONTROL_ACTION_FLASH_PLAY")
{
}


CWpsAlgoAction_FlashPlay::~CWpsAlgoAction_FlashPlay()
{
}


bool CWpsAlgoAction_FlashPlay::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_FlashPlay* pAction = dynamic_cast<CWPSNodeAction_FlashPlay*>(pControlScriptNode);
	if (pAction)
	{
		m_eDirectionType = pAction->m_eDirectionType;
		m_directTblidx = pAction->m_directTblidx;
		m_bSynchronize = pAction->m_bSynchronize;
		m_dwPlayTime = pAction->m_dwPlayTime;
		m_dwWaitTime = pAction->m_dwWaitTime;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_FlashPlay", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_FlashPlay::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}
