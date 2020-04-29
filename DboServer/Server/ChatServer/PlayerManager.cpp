#include "Stdafx.h"
#include "ChatServer.h"
#include "NtlChat.h"


CPlayerManager::CPlayerManager()
{

}

CPlayerManager::~CPlayerManager()
{
	
}

void CPlayerManager::Init()
{	

}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CPlayerManager::TickProcess(DWORD dwTickDiff)
{
	for (auto it = m_map_Player.begin(); it != m_map_Player.end(); it++)
	{
		CPlayer* pPlayer = it->second;
		if (pPlayer && pPlayer->GetPcInitState())
		{
			pPlayer->TickProcess(dwTickDiff);
		}
	}
}


//--------------------------------------------------------------------------------------//
//		CREATE/ADD PLAYER TO MAP
//--------------------------------------------------------------------------------------//
CPlayer* CPlayerManager::CreatePlayer(ACCOUNTID AccID)
{
	CPlayer* player = new CPlayer;
	m_map_Player.insert(std::make_pair(AccID, player));

	return player;
}
//--------------------------------------------------------------------------------------//
//		REMOVE PLAYER FROM MAP
//--------------------------------------------------------------------------------------//
void CPlayerManager::RemovePlayer(ACCOUNTID AccID)
{
	auto it = m_map_Player.find(AccID);
	if (it == m_map_Player.end()) 
		return; // prevent duplicated destrunction

	CPlayer* player = it->second;
	m_map_Player.erase(AccID);

	player->Logout();
	SAFE_DELETE(player);
}

//--------------------------------------------------------------------------------------//
//		REMOVE ALL PLAYERS
//--------------------------------------------------------------------------------------//
void CPlayerManager::RemoveAllPlayers(SERVERCHANNELID channelId/* = INVALID_SERVERCHANNELID*/)
{
	auto it = m_map_Player.begin();

	for (; it != m_map_Player.end(); )
	{
		CPlayer* player = it->second;
		if (player && (player->GetChannel() == channelId || channelId == INVALID_SERVERCHANNELID))
		{
			if (player->GetClientSession())
			{
				if(channelId != INVALID_SERVERCHANNELID)
					player->Logout();

				player->GetClientSession()->SetPlayer(NULL);
				player->GetClientSession()->Disconnect(false);
			}

			SAFE_DELETE(player);
			it = m_map_Player.erase(it);
		}
		else
			++it;
	}
}


//--------------------------------------------------------------------------------------//
//		find player
//--------------------------------------------------------------------------------------//
CPlayer* CPlayerManager::Find(HSESSION hClientSession)
{
	for (auto it = m_map_Player.begin(); it != m_map_Player.end(); it++)
	{
		CPlayer* player = it->second;
		if (player->GetClientSession())
		{
			if (player->GetClientSession()->GetHandle() == hClientSession)
				return it->second;
		}
	}

	return NULL;
}
//--------------------------------------------------------------------------------------//
//		find player by object id
//--------------------------------------------------------------------------------------//
CPlayer* CPlayerManager::FindPlayer(HOBJECT hPlayerID)
{
	for(auto it = m_map_Player.begin(); it != m_map_Player.end(); it++ )
	{
		CPlayer* player = it->second;
		if (player->GetCharID() == hPlayerID)
			return it->second;
	}

	return NULL;
}
//--------------------------------------------------------------------------------------//
//		find player by account id
//--------------------------------------------------------------------------------------//
CPlayer* CPlayerManager::FindPlayerWithAccID(ACCOUNTID AccID)
{
	auto it = m_map_Player.find(AccID);
	if (m_map_Player.end() == it)
		return NULL;

	return it->second;
}
//--------------------------------------------------------------------------------------//
//		find player by character id
//--------------------------------------------------------------------------------------//
CPlayer* CPlayerManager::FindPlayerWithCharID(CHARACTERID CharID)
{
	for(auto it = m_map_Player.begin(); it != m_map_Player.end(); it++ )
	{
		CPlayer* pcdata = it->second;
		if(pcdata)
		{
			if(pcdata->GetCharID() == CharID)
				return pcdata;
		}
	}
	
	return NULL;
}
//--------------------------------------------------------------------------------------//
//		find player by character name
//--------------------------------------------------------------------------------------//
CPlayer* CPlayerManager::FindPlayerByName(WCHAR* wchName)
{
	for(auto it = m_map_Player.begin(); it != m_map_Player.end(); it++ )
	{
		CPlayer* pcdata = it->second;
		if(pcdata)
		{
			if (_wcsicmp(pcdata->GetCharName(), wchName) == 0 && pcdata->GetClientSession())
			{
				return pcdata;
			}
		}
	}
	
	return NULL;
}


//--------------------------------------------------------------------------------------//
//		send packet to friends
//--------------------------------------------------------------------------------------//
void CPlayerManager::UserSendMsgToFriends(CNtlPacket* pPacket, CHARACTERID CharId)
{
	for(auto it = m_map_Player.begin(); it != m_map_Player.end(); it++ )
	{
		CPlayer* player = it->second;
		if(player && player->GetPcInitState() && player->GetReadyForCommunityServer() && player->GetCharID() != CharId)
		{
			sFRIEND_FULL_INFO* friendinfo = player->GetFriendData(CharId);
			if(friendinfo && friendinfo->bIsBlack == false)
			{
				player->SendPacket(pPacket);
			}
		}
	}
}
//--------------------------------------------------------------------------------------//
//		Send packet to everyone in chat server
//--------------------------------------------------------------------------------------//
void CPlayerManager::SendMsgToAll(CNtlPacket* pPacket, CHARACTERID senderCharId/* = INVALID_CHARATACTERID*/, bool bCheckBlacklist/* = false*/)
{
	for(auto it = m_map_Player.begin(); it != m_map_Player.end(); it++ )
	{
		CPlayer* pPlayer = it->second;
		if (pPlayer)
		{
			if (bCheckBlacklist)
			{
				if(pPlayer->IsBlackListed(senderCharId) == false)
					g_pApp->Send(pPlayer->GetClientSessionID(), pPacket);
			}
			else
			{
				g_pApp->Send(pPlayer->GetClientSessionID(), pPacket);
			}
		}
	}
}
//--------------------------------------------------------------------------------------//
//		send packet to all within range
//--------------------------------------------------------------------------------------//
void CPlayerManager::SendMsgToInRange(CNtlPacket* pPacket, CPlayer* pPlayer, float fRange, SERVERCHANNELID channel, WORLDID worldid, bool bCheckBlacklist/* = false*/)
{
	for(auto it = m_map_Player.begin(); it != m_map_Player.end(); it++ )
	{
		CPlayer* pTarget = it->second;
		if (pTarget)
		{
			if (pTarget->GetPcInitState() == true && pTarget->GetChannel() == channel && pTarget->GetWorldID() == worldid)
			{
				if (NtlGetDistance(pPlayer->GetLocation(), pTarget->GetLocation()) <= fRange)
				{
					if (bCheckBlacklist)
					{
						if(pTarget->IsBlackListed(pPlayer->GetCharID()) == false)
							g_pApp->Send(pTarget->GetClientSessionID(), pPacket);
					}
					else
					{
						g_pApp->Send(pTarget->GetClientSessionID(), pPacket);
					}
				}
			}
		}
	}
}