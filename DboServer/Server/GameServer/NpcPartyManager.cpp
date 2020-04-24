#include "stdafx.h"
#include "Npc.h"


CNpcPartyManager::CNpcPartyManager()
{
	this->Init();
}

CNpcPartyManager::~CNpcPartyManager()
{
	Destroy();
}



void CNpcPartyManager::Destroy()
{
	for (std::vector<CNpcParty*>::iterator it = m_vecNpcPartyPtr.begin(); it != m_vecNpcPartyPtr.end(); it++)
		delete *it;

	m_vecNpcPartyPtr.clear();
	m_lstNpcPartyPtrPool.clear();
}


void CNpcPartyManager::Init()
{
	m_vecNpcPartyPtr.reserve(256);
}


void CNpcPartyManager::TickProcess(DWORD dwTickDiff, float fMultiple)
{
}


CNpcParty* CNpcPartyManager::FindParty(PARTYID partyId)
{
	std::map<PARTYID, NPC_PARTYPTR>::iterator it = m_mapNpcPartyId.find(partyId);
	if (it == m_mapNpcPartyId.end())
		return NULL;

	if (m_vecNpcPartyPtr.size() > it->second)
	{
		return m_vecNpcPartyPtr[it->second];
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "m_vecNpcPartyPtr.size() %u <= it->second %u", m_vecNpcPartyPtr.size(), it->second);
	}

	return NULL;
}


CNpcParty* CNpcPartyManager::FindParty(PARTYID partyId, NPC_PARTYPTR partyPtr)
{
	if (m_vecNpcPartyPtr.size() > partyPtr)
	{
		CNpcParty* pParty = m_vecNpcPartyPtr[partyPtr];
		if (pParty && pParty->GetPartyId() == partyId)
			return pParty;
	}

	return NULL;
}


CNpcParty* CNpcPartyManager::CreateParty(PARTYID partyId)
{
	NPC_PARTYPTR partyPtr = INVALID_NPCPARTY_PTR;

	if (m_lstNpcPartyPtrPool.size() > 0)
	{
		partyPtr = m_lstNpcPartyPtrPool.front();
		m_lstNpcPartyPtrPool.pop_front();

		if (m_vecNpcPartyPtr.size() <= partyPtr)
		{
			ERR_LOG(LOG_SYSTEM, "m_vecNpcPartyPtr.size() %u <= partyPtr[%u]", m_vecNpcPartyPtr.size(), partyPtr);
			return NULL;
		}

		if (m_vecNpcPartyPtr[partyPtr])
		{
			ERR_LOG(LOG_SYSTEM, "m_vecNpcPartyPtr[partyPtr] != NULL %u", partyPtr);
			return NULL;
		}
	}
	else
	{
		partyPtr = (NPC_PARTYPTR)m_vecNpcPartyPtr.size();
		m_vecNpcPartyPtr.push_back(NULL);
	}

	CNpcParty* pParty = new CNpcParty;

	if (pParty->Create(partyId, partyPtr))
	{
		m_mapNpcPartyId.insert(std::map<PARTYID, NPC_PARTYPTR>::value_type(partyId, partyPtr));
		m_vecNpcPartyPtr[partyPtr] = pParty;
	}
	else
	{
		m_lstNpcPartyPtrPool.push_back(partyPtr);
		SAFE_DELETE(pParty);
	}

	return pParty;
}


void CNpcPartyManager::DestroyParty(CNpcParty *pParty)
{
	if (pParty)
	{
		m_mapNpcPartyId.erase(pParty->GetPartyId());
		m_lstNpcPartyPtrPool.push_back(pParty->GetPartyPtr());
		if (m_vecNpcPartyPtr.size() > pParty->GetPartyPtr())
		{
			m_vecNpcPartyPtr[pParty->GetPartyPtr()] = NULL;
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "m_vecNpcPartyPtr.size() <= pParty->GetPartyPtr()");
		}

		SAFE_DELETE(pParty);
	}
}


bool CNpcPartyManager::JoinParty(CNpc *pBot, PARTYID partyId, bool bLeader)
{
	CNpcParty* pParty = FindParty(partyId);
	if (!pParty)
		pParty = CreateParty(partyId);

	if (pParty)
	{
		if (pParty->AddMember(pBot, bLeader))
		{
			pBot->SetPartyPtr(pParty->GetPartyPtr());
			return true;
		}
	}

	return false;
}


void CNpcPartyManager::LeaveParty(CNpc *pBot)
{
	if (pBot)
	{
		CNpcParty *pParty = FindParty(pBot->GetPartyID(), pBot->GetPartyPtr());
		if (pParty)
		{
			pParty->DeleteHelpList(pBot->GetID());
			pParty->RemoveMemberWithHandle(pBot->GetID());

			if (pParty->IsEmpty())
				DestroyParty(pParty);

			pBot->SetPartyPtr(INVALID_NPCPARTY_PTR);
		}
		else
			ERR_LOG(LOG_SYSTEM, "can not find npc party. tblidx[%u] partyid[%u] partyptr[%u]", pBot->GetTblidx(), pBot->GetPartyID(), pBot->GetPartyPtr());
	}
	else ERR_LOG(LOG_SYSTEM, "NULL == pBot");
}

