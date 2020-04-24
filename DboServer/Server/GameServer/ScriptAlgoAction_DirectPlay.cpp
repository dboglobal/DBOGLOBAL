#include "stdafx.h"
#include "ScriptAlgoAction_DirectPlay.h"
#include "WPSNodeAction_DirectPlay.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "DirectionLinkTable.h"
#include "TableContainerManager.h"
#include "NtlDirection.h"


CWpsAlgoAction_DirectPlay::CWpsAlgoAction_DirectPlay(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_DIRECT_PLAY, "SCRIPTCONTROL_ACTION_DIRECT_PLAY")
{
	m_dwWaitTime = 0;
}


CWpsAlgoAction_DirectPlay::~CWpsAlgoAction_DirectPlay()
{
}


bool CWpsAlgoAction_DirectPlay::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_DirectPlay* pAction = dynamic_cast<CWPSNodeAction_DirectPlay*>(pControlScriptNode);
	if (pAction)
	{
		m_eDirectionType = pAction->m_eDirectionType;
		m_directTblidx = pAction->m_directTblidx;
		m_bSynchronize = pAction->m_bSynchronize;
		m_dwPlayTime = pAction->m_dwPlayTime;
		m_dwWaitTime = pAction->m_dwWaitTime;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_DirectPlay", pControlScriptNode);
	return false;
}


void CWpsAlgoAction_DirectPlay::OnEnter()
{
	if (m_directTblidx != INVALID_TBLIDX)
	{
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer && pPlayer->IsInitialized())
		{
			pPlayer->StartDirectPlay(DIRECT_PLAY_NORMAL, m_directTblidx, m_bSynchronize, false, m_eDirectionType);

			pPlayer = GetOwner()->GetPlayersNext();
		}
	}
}


int CWpsAlgoAction_DirectPlay::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_dwPlayTime == INVALID_DWORD)
	{
		bool bComplete = true;
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->GetCharStateID() == CHARSTATE_DIRECT_PLAY)
			{
				bComplete = false;
				break;
			}

			pPlayer = GetOwner()->GetPlayersNext();
		}

		if (bComplete)
			m_status = COMPLETED;
	}
	else
	{
		m_dwWaitTime = UnsignedSafeIncrease<DWORD>(m_dwWaitTime, dwTickDiff);
		if (m_dwWaitTime >= m_dwPlayTime)
			m_status = COMPLETED;
	}

	return m_status;
}
