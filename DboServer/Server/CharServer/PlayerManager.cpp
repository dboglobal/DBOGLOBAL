#include "stdafx.h"
#include "CharServer.h"
#include "QueueManager.h"
#include "Player.h"


CPlayerManager::CPlayerManager()
{
	m_map_Player.clear();
	m_vecDeleteAccount.clear();
}

CPlayerManager::~CPlayerManager()
{
	Release();
}

void CPlayerManager::Release()
{
}


void CPlayerManager::TickProcess(DWORD dwTickDiff)
{
	m_mutex.lock();

	for (std::vector<CPlayer*>::iterator it = m_vecDeleteAccount.begin(); it != m_vecDeleteAccount.end(); )
	{
		CPlayer* pPlayer = *it;
		if (pPlayer)
		{
			if (pPlayer->DecreaseDeleteTime(dwTickDiff))
			{
				delete pPlayer;

				it = m_vecDeleteAccount.erase(it);
			}
			else ++it;
		}
		else
		{
			it = m_vecDeleteAccount.erase(it);
		}
	}

	m_mutex.unlock();
}

void CPlayerManager::DisconnectAll()
{
	m_mutex.lock();

	for (std::map<ACCOUNTID, CPlayer*>::iterator it = m_map_Player.begin(); it != m_map_Player.end(); it++)
	{
		CPlayer* pPlayer = it->second;
		if (pPlayer && pPlayer->GetSession())
		{
			pPlayer->GetSession()->Disconnect(false);

			m_vecDeleteAccount.push_back(it->second);
		}
	}

	m_map_Player.clear();

	m_mutex.unlock();
}

void CPlayerManager::RemovePlayer(CPlayer* pPlayer)
{
	m_mutex.lock();

	std::map<ACCOUNTID, CPlayer*>::iterator it2 = m_map_Player.find(pPlayer->GetAccountID());
	if (it2 != m_map_Player.end())
	{
		if(it2->second)
			m_vecDeleteAccount.push_back(it2->second);

		m_map_Player.erase(it2);
	}

	m_mutex.unlock();
}


CPlayer* CPlayerManager::GetPlayer(ACCOUNTID AccountID)
{
	m_mutex.lock();
	std::map<ACCOUNTID, CPlayer*>::iterator it = m_map_Player.find(AccountID);
	if (it == m_map_Player.end())
	{
		m_mutex.unlock();
		return NULL;
	}

	m_mutex.unlock();

	return it->second;
}


CPlayer* CPlayerManager::CreatePlayer(ACCOUNTID AccountID, CClientSession* pSession)
{
	m_mutex.lock();

	std::map<ACCOUNTID, CPlayer*>::iterator it = m_map_Player.find(AccountID); //check if already online
	if (it != m_map_Player.end())
	{
		m_mutex.unlock();
		return NULL;
	}

	CPlayer* pPlayer = new CPlayer(AccountID, pSession);

	m_map_Player.insert(std::make_pair(AccountID, pPlayer));

	m_mutex.unlock();

	return pPlayer;
}


void CPlayerManager::SendToAccount(CNtlPacket * pPacket, ACCOUNTID AccountID)
{
	m_mutex.lock();
	std::map<ACCOUNTID, CPlayer*>::iterator it = m_map_Player.find(AccountID);
	if (m_map_Player.end() == it)
	{
		NTL_PRINT(PRINT_APP, "PLAYER AccountID %d NOT FOUND m_map_Player.size() %u", AccountID, m_map_Player.size());
		m_mutex.unlock();
		return;
	}

	CPlayer* pPlayer = it->second;

	if (pPlayer && pPlayer->GetSessionHandle() != INVALID_HSESSION)
		g_pApp->Send(it->second->GetSessionHandle(), pPacket);

	m_mutex.unlock();
}

void CPlayerManager::SendToAll(CNtlPacket * pPacket)
{
	m_mutex.lock();
	for(std::map<ACCOUNTID, CPlayer*>::iterator it = m_map_Player.begin(); it != m_map_Player.end(); it++ )
	{
		CPlayer* pPlayer = it->second;
		if (pPlayer && pPlayer->GetSessionHandle() != INVALID_HSESSION)
		{
			g_pApp->Send(pPlayer->GetSessionHandle(), pPacket);
		}
	}
	m_mutex.unlock();
}

void CPlayerManager::SendToAll(CNtlPacket * pPacket, SERVERFARMID serverid)
{
	m_mutex.lock();
	for (std::map<ACCOUNTID, CPlayer*>::iterator it = m_map_Player.begin(); it != m_map_Player.end(); it++)
	{
		CPlayer* pPlayer = it->second;
		if (pPlayer && pPlayer->GetSessionHandle() != INVALID_HSESSION)
		{
			if (pPlayer->GetServerFarmID() == serverid)
				g_pApp->Send(pPlayer->GetSessionHandle(), pPacket);
		}
	}
	m_mutex.unlock();
}