#include "stdafx.h"
#include "Npc.h" // char.h & escortmanager.h
#include "ObjectManager.h"
#include "NtlResultCode.h"
#include "NtlPacketGU.h"
#include "CPlayer.h"



CEscortManager::CEscortManager()
{
	Init();
}

CEscortManager::~CEscortManager()
{
}


bool CEscortManager::Create(CCharacter* pBot)
{
	m_pParent = pBot;
	return true;
}


void CEscortManager::TickProcess(DWORD dwTickTime, float fMultiple)
{
}


bool CEscortManager::AddTarget(bool bShared, eESCORT_TYPE eEscortType, HOBJECT handle, CHARACTERID charId, PARTYID partyID, BYTE byTriggerType, WORD tid, WORD *pwResultCode)
{
	if (m_byEscortTargetCount >= 5)
	{
		if (pwResultCode)
			*pwResultCode = GAME_TS_WARNING_ESCORT_EXCEED_MEMBER;
		return false;
	}

	if (m_tid == INVALID_WORD)
	{
		m_bShared = bShared;
		m_eEscortType = eEscortType;
		m_escortPartyID = partyID;
		m_byTriggerType = byTriggerType;
		m_tid = tid;
		goto LABEL_26;
	}

	if (m_bShared)
	{
		if (byTriggerType == m_byTriggerType)
		{
			if (tid == m_tid)
			{
				if (partyID == m_escortPartyID)
				{
				LABEL_26:

					if (FindTarget(handle) == INVALID_BYTE)
					{
						m_ahEscortTarget[m_byEscortTargetCount].hPc = handle;
						m_ahEscortTarget[m_byEscortTargetCount++].charId = charId;
						return true;
					}
					else
					{
						if (pwResultCode)
							*pwResultCode = GAME_TS_WARNING_ESCORT_ALREADY_EXIST;
					}
				}
				else
				{
					if (pwResultCode)
						*pwResultCode = GAME_TS_WARNING_ESCORT_PARTY_WRONG;
				}
			}
			else
			{
				if (pwResultCode)
					*pwResultCode = GAME_TS_WARNING_ESCORT_TRIGGER_ID_WRONG;
			}
		}
		else
		{
			if (pwResultCode)
				*pwResultCode = GAME_TS_WARNING_ESCORT_TRIGGER_TYPE_WRONG;
		}
	}
	else
	{
		if (pwResultCode)
			*pwResultCode = GAME_TS_WARNING_ESCORT_NOT_SHARED;
	}

	return false;
}

void CEscortManager::RemoveTarget(HOBJECT handle)
{
	for (BYTE j = 0; j < m_byEscortTargetCount; j++)
	{
		if (m_ahEscortTarget[j].hPc == handle)
		{
			RemoveMemberWithSlot(j);
			return;
		}
	}
}


void CEscortManager::Clear()
{
	m_bShared = false;
	m_eEscortType = INVALID_ESCORT_TYPE;
	m_byEscortTargetCount = 0;
	memset(m_ahEscortTarget, INVALID_HOBJECT, sizeof(m_ahEscortTarget));
	m_escortPartyID = INVALID_PARTYID;
	m_byTriggerType = INVALID_BYTE;
	m_tid = INVALID_WORD;
	m_byCurrentTargetSlot = 0;
}


CCharacter* CEscortManager::GetCurrentTarget()
{
	if (m_byCurrentTargetSlot != INVALID_BYTE)
	{
		BYTE bySlot = m_byCurrentTargetSlot;
		for (BYTE i = 0; i < m_byEscortTargetCount; i++)
		{
			if (bySlot == m_byEscortTargetCount)
				bySlot = 0;

			CCharacter* pTarget = g_pObjectManager->GetChar(m_ahEscortTarget[bySlot].hPc);
			if (pTarget)
			{
				m_byCurrentTargetSlot = bySlot;
				return pTarget;
			}
			else
			{
				RemoveMemberWithSlot(bySlot);
			}
		}
	}

	return NULL;
}

BYTE CEscortManager::GetTargetCount()
{
	return m_byEscortTargetCount;
}

HOBJECT CEscortManager::GetTargetWithSlot(BYTE byPos)
{
	if (byPos < m_byEscortTargetCount)
		return m_ahEscortTarget[byPos].hPc;
	
	return INVALID_HOBJECT;
}

CHARACTERID CEscortManager::GetTargetIdWithSlot(BYTE byPos)
{
	if (byPos < m_byEscortTargetCount)
		return m_ahEscortTarget[byPos].charId;

	return INVALID_CHARACTERID;
}


void CEscortManager::SendEvent(BYTE byEventType, BYTE byTriggerType, DWORD dwEventID)
{
	//send event to players which connected to escort npc

	for (BYTE j = 0; j < m_byEscortTargetCount; j++)
	{
		HOBJECT handle = GetTargetWithSlot(j);
		if (handle != INVALID_HOBJECT)
		{
			if (CPlayer* pPlayer = g_pObjectManager->GetPC(handle))
			{
				if (pPlayer->IsInitialized())
				{
					CNtlPacket packet(sizeof(sGU_TS_UPDATE_EVENT_NFY));
					sGU_TS_UPDATE_EVENT_NFY * res = (sGU_TS_UPDATE_EVENT_NFY *)packet.GetPacketData();
					res->wOpCode = GU_TS_UPDATE_EVENT_NFY;
					res->byTsType = byTriggerType;
					res->teid = (NTL_TS_EVENT_ID)dwEventID;
					packet.SetPacketLen(sizeof(sGU_TS_UPDATE_EVENT_NFY));

					switch (byEventType)
					{
						case TS_EVENT_SEND_TYPE_PARTY:
						{
							if (pPlayer->GetParty())
							{
								pPlayer->GetParty()->AddEventIdToAll(dwEventID);
								pPlayer->GetParty()->SendMsgToParty(&packet);
							}
							else
							{
								pPlayer->GetQuests()->GetEventMap()->OnEventReceived(dwEventID);

								pPlayer->SendPacket(&packet);
							}
						}
						break;
						case TS_EVENT_SEND_TYPE_PC:
						{
							pPlayer->GetQuests()->GetEventMap()->OnEventReceived(dwEventID);

							pPlayer->SendPacket(&packet);
						}
						break;
					}
				}
			}
		}
	}
}


bool CEscortManager::IsEscortRun()
{
	return false;
}


void CEscortManager::Init()
{
	Clear();
}

void CEscortManager::RemoveMemberWithSlot(BYTE byPos)
{
	if (byPos <= m_byEscortTargetCount)
	{
		this->m_ahEscortTarget[byPos].hPc = m_ahEscortTarget[m_byEscortTargetCount - 1].hPc;
		this->m_ahEscortTarget[byPos].charId = m_ahEscortTarget[m_byEscortTargetCount - 1].charId;
		this->m_ahEscortTarget[m_byEscortTargetCount - 1].hPc = INVALID_HOBJECT;
		this->m_ahEscortTarget[m_byEscortTargetCount-- - 1].charId = INVALID_CHARACTERID;
	}
}

BYTE CEscortManager::FindTarget(HOBJECT handle)
{
	for (BYTE j = 0; j < m_byEscortTargetCount; j++)
	{
		if (handle == m_ahEscortTarget[j].hPc)
			return j;
	}
	return INVALID_BYTE;
}
