#include "stdafx.h"
#include "TqsAlgoAction_DirectPlay.h"
#include "TQSNodeAction_DirectPlay.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "TimeQuest.h"


CTqsAlgoAction_DirectPlay::CTqsAlgoAction_DirectPlay(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_DIRECT_PLAY, "TQS_ALGOCONTROLID_ACTION_DIRECT_PLAY")
{
	m_dwWaitTime = 0;
}


CTqsAlgoAction_DirectPlay::~CTqsAlgoAction_DirectPlay()
{
}


bool CTqsAlgoAction_DirectPlay::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_DirectPlay* pAction = dynamic_cast<CTQSNodeAction_DirectPlay*>(pControlScriptNode);
	if (pAction)
	{
		m_eDirectionType = pAction->m_eDirectionType;
		m_directTblidx = pAction->m_directTblidx;
		m_bSynchronize = pAction->m_bSynchronize;
		m_bCanSkip = pAction->m_bCanSkip;
		m_dwWaitTime = pAction->m_dwWaitTime;
		m_bEnding = pAction->m_bEnding;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_DirectPlay", pControlScriptNode);
	return false;
}


void CTqsAlgoAction_DirectPlay::OnEnter()
{
	if (m_directTblidx != INVALID_TBLIDX)
	{
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer && pPlayer->IsInitialized())
		{
			pPlayer->StartDirectPlay(DIRECT_PLAY_NORMAL, m_directTblidx, m_bSynchronize, m_bCanSkip, m_eDirectionType);

			pPlayer = GetOwner()->GetPlayersNext();
		}
	}
}


int CTqsAlgoAction_DirectPlay::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_bEnding) //check if cant skip and have to wait
	{
		m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);
		if (m_dwWaitTime == 0)
			m_status = COMPLETED;
	}
	else
	{
		bool bComplete = true;
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();

		//check if party skipped 
		if (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->GetParty() && pPlayer->GetParty()->IsCancelDirectPlay((BYTE)GetOwner()->GetPlayersCount()))
			{
				m_status = COMPLETED;

				while (pPlayer && pPlayer->IsInitialized())
				{
					if (pPlayer->GetCharStateID() == CHARSTATE_DIRECT_PLAY)
					{
						CNtlPacket packet(sizeof(sGU_CHAR_DIRECT_PLAY_END));
						sGU_CHAR_DIRECT_PLAY_END * res = (sGU_CHAR_DIRECT_PLAY_END*)packet.GetPacketData();
						res->wOpCode = GU_CHAR_DIRECT_PLAY_END;
						packet.SetPacketLen(sizeof(sGU_CHAR_DIRECT_PLAY_END));
						pPlayer->SendPacket(&packet);

						pPlayer->SendCharStateStanding();
					}

					pPlayer = GetOwner()->GetPlayersNext();
				}

				return m_status;
			}
		}

		//if not skipped then wait until cinematic time ends
		while (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->GetCharStateID() == CHARSTATE_DIRECT_PLAY)
			{
				return m_status;
			}

			pPlayer = GetOwner()->GetPlayersNext();
		}

		CPlayer* pFirstPlayer = GetOwner()->GetPlayersFirst();
		if(pFirstPlayer && pFirstPlayer->GetParty())
			pFirstPlayer->GetParty()->ResetPartyDirectPlay();

		m_status = COMPLETED;
	}

	return m_status;
}
