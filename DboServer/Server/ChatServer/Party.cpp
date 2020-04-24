#include "stdafx.h"
#include "Party.h"
#include "ChatServer.h"
#include "Player.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////										PARTY	MANAGER										/////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPartyManager::CPartyManager()
{
}


CPartyManager::~CPartyManager()
{
}


void CPartyManager::CreateParty(PARTYID partyId, CHARACTERID hLeader)
{
	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(hLeader);
	if (pPlayer)
	{
		CParty* pParty = new CParty(partyId, pPlayer);

		m_map_Party.insert({ partyId, pParty });
	}
}


CParty* CPartyManager::GetParty(PARTYID partyid)
{
	std::map<PARTYID, CParty*>::iterator itr = m_map_Party.find(partyid);
	if (itr != m_map_Party.end())
		return itr->second;

	return NULL;
}


void CPartyManager::DisbandParty(PARTYID partyId)
{
	CParty* pParty = GetParty(partyId);
	if (pParty == NULL)
		return;

	//delete party
	m_map_Party.erase(partyId);
	SAFE_DELETE(pParty);
}


void CPartyManager::AddMember(PARTYID partyId, CHARACTERID hMember)
{
	CParty* pParty = GetParty(partyId);
	if (pParty == NULL)
		return;

	pParty->AddMember(hMember);
}


void CPartyManager::RemoveMember(PARTYID partyId, CHARACTERID hMember)
{
	CParty* pParty = GetParty(partyId);
	if (pParty == NULL)
		return;

	pParty->RemoveMember(hMember);
}


void CPartyManager::ChangePartyLeader(PARTYID partyId, CHARACTERID newLeaderCharId)
{
	CParty* pParty = GetParty(partyId);
	if (pParty == NULL)
		return;

	pParty->ChangeLeader(newLeaderCharId);
}


void CPartyManager::Broadcast(PARTYID partyId, CNtlPacket* pPacket, CHARACTERID hException)
{
	if (partyId == 0 || partyId == INVALID_PARTYID)
		return;

	CParty* pParty = GetParty(partyId);
	if (pParty == NULL)
		return;

	pParty->Broadcast(pPacket, hException);
}

void CPartyManager::RemoveAllParties(SERVERCHANNELID channelid)
{
	for (std::map<PARTYID, CParty*>::iterator it = m_map_Party.begin(); it != m_map_Party.end(); )
	{
		CParty* pParty = it->second;
		if (pParty && pParty->GetChannelID() == channelid)
		{
			it = m_map_Party.erase(it);
			SAFE_DELETE(pParty);
		}
		else ++it;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////											PARTY											/////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParty::CParty(PARTYID hPartyID, CPlayer* pLeader)
{
	if (pLeader)
	{
		m_hLeader = pLeader->GetCharID();
		m_hPartyID = hPartyID;
		m_serverChannelId = pLeader->GetChannel();

		pLeader->SetPartyID(hPartyID);

		m_vecMember.push_back(pLeader);
	}
}


CParty::~CParty()
{
	for (std::vector<CPlayer*>::iterator it = m_vecMember.begin(); it != m_vecMember.end(); it++)
	{
		CPlayer* pPlayer = *it;
		if (pPlayer)
		{
			pPlayer->SetPartyID(INVALID_PARTYID);
		}
	}
}


void CParty::AddMember(CHARACTERID hMember)
{
	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(hMember);
	if (pPlayer)
	{
		m_vecMember.push_back(pPlayer);
		pPlayer->SetPartyID(m_hPartyID);
	}
}


void CParty::RemoveMember(CHARACTERID hMember)
{
	for (std::vector<CPlayer*>::iterator it = m_vecMember.begin(); it != m_vecMember.end(); it++)
	{
		CPlayer* pPlayer = *it;
		if (pPlayer && pPlayer->GetCharID() == hMember)
		{
			pPlayer->SetPartyID(m_hPartyID);

			m_vecMember.erase(it);
			break;
		}
	}
}


void CParty::ChangeLeader(CHARACTERID hNewLeader)
{
	m_hLeader = hNewLeader;
}


void CParty::Broadcast(CNtlPacket* pPacket, CHARACTERID hException)
{
	for (std::vector<CPlayer*>::iterator it = m_vecMember.begin(); it != m_vecMember.end(); it++)
	{
		CPlayer* pPlayer = *it;
		if (pPlayer && pPlayer->GetPartyID() == this->m_hPartyID && pPlayer->GetCharID() != hException)
			g_pApp->Send(pPlayer->GetClientSessionID(), pPacket);
	}
}

void CParty::FillMembersCharID(CHARACTERID* pCharID, BYTE& rCount, CHARACTERID charException)
{
	for (std::vector<CPlayer*>::iterator it = m_vecMember.begin(); it != m_vecMember.end(); it++)
	{
		CPlayer* pPlayer = *it;
		if (pPlayer && pPlayer->GetPcInitState() && pPlayer->GetCharID() != charException)
			pCharID[rCount++] = pPlayer->GetCharID();
	}
}


