#include "stdafx.h"
#include "MasterServer.h"
#include "PacketHead.h"


CSubNeighborServerInfoManager::CSubNeighborServerInfoManager()
{
	m_mapAuthServerSession.clear();
	m_mapCharacterServerSession.clear();
	m_mapChatServerSession.clear();
	m_mapGameServerSession.clear();

	m_map_CharServerPlayers.clear();
	m_map_GameServerPlayers.clear();

	m_pEventHolder = new EventableObjectHolder(-1);
}

CSubNeighborServerInfoManager::~CSubNeighborServerInfoManager()
{
	SAFE_DELETE(m_pEventHolder);
}



void CSubNeighborServerInfoManager::TickProcess(DWORD dwTickDiff)
{
	m_pEventHolder->Update(dwTickDiff);
}

void CSubNeighborServerInfoManager::UpdateScrambleChannel(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, bool bIsScrambleChannel)
{
	sDBO_GAME_SERVER_CHANNEL_INFO* pInfo = GetServerChannelInfo(serverFarmId, byServerChannelIndex);
	if (!pInfo)
		return;

	pInfo->bIsScrambleChannel = bIsScrambleChannel;

	OnEvent_ServerLoadUpdate();
}


void CSubNeighborServerInfoManager::StartEvents()
{
	g_pEventMgr->AddEvent(this, &CSubNeighborServerInfoManager::OnEvent_ServerLoadUpdate, SERVER_EVENT_LOAD_UPDATE, 30000, INVALID_DWORD, 0);
}


void CSubNeighborServerInfoManager::OnEvent_ServerLoadUpdate()
{
	//Char server
	for (std::list<sDBO_SERVER_INFO *>::iterator it = m_listCharacterServerInfo.begin(); it != m_listCharacterServerInfo.end(); it++)
	{
		sDBO_SERVER_INFO* pCharserverInfo = *it;
		if (pCharserverInfo && pCharserverInfo->byRunningState == DBO_SERVER_RUNNING_STATE_RUNNING)
		{
			//send to all auth servers
			CNtlPacket aPacket(sizeof(sMA_SERVER_INFO_CHANGED_NFY));
			sMA_SERVER_INFO_CHANGED_NFY * aRes = (sMA_SERVER_INFO_CHANGED_NFY *)aPacket.GetPacketData();
			aRes->wOpCode = MA_SERVER_INFO_CHANGED_NFY;
			aRes->byServerType = NTL_SERVER_TYPE_CHARACTER;
			aRes->serverFarmId = pCharserverInfo->serverFarmId;
			aRes->serverChannelId = pCharserverInfo->byServerChannelIndex;
			aRes->serverIndex = pCharserverInfo->byServerIndex;
			aRes->bIsOn = pCharserverInfo->bIsOn;
			aRes->byRunningState = pCharserverInfo->byRunningState;
			aRes->dwLoad = pCharserverInfo->dwLoad;
			aPacket.SetPacketLen(sizeof(sMA_SERVER_INFO_CHANGED_NFY));
			BroadcastServer(NTL_SERVER_TYPE_AUTH, &aPacket);

			//send to all game
			CNtlPacket packet(sizeof(sMG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY));
			sMG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY * res = (sMG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY *)packet.GetPacketData();
			res->wOpCode = MG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY;
			res->byRunningState = pCharserverInfo->byRunningState;
			res->dwLoad = pCharserverInfo->dwLoad;
			res->dwMaxLoad = pCharserverInfo->dwMaxLoad;
			res->serverIndex = pCharserverInfo->byServerIndex;
			packet.SetPacketLen(sizeof(sMG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY));
			BroadcastServer(NTL_SERVER_TYPE_GAME, &packet);
		}
	}

	//game server
	for (std::map<SERVERFARMID, sSERVER_FARM_INFO *>::iterator it2 = m_mapGameServerFarmInfo.begin(); it2 != m_mapGameServerFarmInfo.end(); it2++)
	{
		sSERVER_FARM_INFO* pFarm = it2->second;
		if (pFarm)
		{
			//send farm to character server
			CNtlPacket packet1(sizeof(sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY));
			sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY * res1 = (sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY *)packet1.GetPacketData();
			res1->wOpCode = MC_GAME_SERVER_FARM_INFO_CHANGED_NFY;
			res1->serverFarmId = pFarm->serverFarmId;
			res1->byServerStatus = pFarm->byServerStatus;
			res1->dwMaxLoad = pFarm->dwMaxLoad;
			res1->dwLoad = pFarm->dwLoad;
			packet1.SetPacketLen(sizeof(sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY));
			BroadcastServer(NTL_SERVER_TYPE_CHARACTER, &packet1);

			for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it3 = pFarm->mapGameServerChannelInfo.begin(); it3 != pFarm->mapGameServerChannelInfo.end(); it3++)
			{
				sSERVER_CHANNEL_INFO* pChannel = it3->second;
				if (pChannel && pChannel->byServerStatus == DBO_SERVER_STATUS_UP && pChannel->bIsVisible)
				{
					//send to other channels
					CNtlPacket packet2(sizeof(sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
					sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY * res2 = (sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY *)packet2.GetPacketData();
					res2->wOpCode = MG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY;
					res2->byServerStatus = pChannel->byServerStatus;
					res2->dwLoad = pChannel->dwLoad;
					res2->dwMaxLoad = pChannel->dwMaxLoad;
					res2->byServerChannelIndex = pChannel->byServerChannelIndex;
					res2->serverFarmId = pChannel->serverFarmId;
					res2->bIsScrambleChannel = pChannel->bIsScrambleChannel;
					memcpy(&res2->sChannelBuff, &pChannel->sChannelBuff, sizeof(_CHANNEL_BUFF));
					packet2.SetPacketLen(sizeof(sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
					BroadcastServer(NTL_SERVER_TYPE_GAME, &packet2);

					//send channel to char server
					CNtlPacket packet3(sizeof(sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
					sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY * res3 = (sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY *)packet3.GetPacketData();
					res3->wOpCode = MC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY;
					res3->serverFarmId = pChannel->serverFarmId;
					res3->byServerChannelIndex = pChannel->byServerChannelIndex;
					res3->byServerIndex = pChannel->byServerChannelIndex;
					res3->byServerStatus = pChannel->byServerStatus;
					res3->dwMaxLoad = pChannel->dwMaxLoad;
					res3->dwLoad = pChannel->dwLoad;
					res3->bIsScrambleChannel = pChannel->bIsScrambleChannel;
					packet3.SetPacketLen(sizeof(sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
					BroadcastServer(NTL_SERVER_TYPE_CHARACTER, &packet3);
				}
			}
		}
	}
}


//Add player to character server online list
void CSubNeighborServerInfoManager::AddPlayerCharServer(ACCOUNTID AccID, SERVERINDEX serverIndex)
{
	m_map_CharServerPlayers.insert(std::make_pair(AccID, serverIndex));
}


//Add player to game server online list
void CSubNeighborServerInfoManager::AddPlayerGameServer(ACCOUNTID AccID, SERVERFARMID serverfarm, SERVERCHANNELID channel)
{
	auto it = m_map_GameServerPlayers.find(AccID);
	if (it != m_map_GameServerPlayers.end())
	{
		it->second.serverFarmId = serverfarm;
		it->second.serverChannelId = channel;
	}
	else
	{
		m_map_GameServerPlayers.insert(std::make_pair(AccID, _SERVER_PAIR_ID(serverfarm, channel)));
	}
}


//remove all players from server index
void CSubNeighborServerInfoManager::RemoveAllPlayerFromCharServer(SERVERINDEX serverIndex)
{
	for (auto it = m_map_CharServerPlayers.begin(); it != m_map_CharServerPlayers.end(); )
	{
		if (it->second == serverIndex)
		{
			it = m_map_CharServerPlayers.erase(it);
		}
		else ++it;
	}
}


//remove all players from server&channel
void CSubNeighborServerInfoManager::RemoveAllPlayerFromGameServer(_SERVER_PAIR_ID serverPairID)
{
	for (auto it = m_map_GameServerPlayers.begin(); it != m_map_GameServerPlayers.end(); )
	{
		if (it->second == serverPairID)
		{
			it = m_map_GameServerPlayers.erase(it);
		}
		else ++it;
	}
}



//remove player from character or game server online list
void CSubNeighborServerInfoManager::DelPlayer(ACCOUNTID AccID, bool bIsGameServer, SERVERFARMID serverfarmId/* = INVALID_SERVERFARMID*/, SERVERCHANNELID channelId/* = INVALID_SERVERCHANNELID*/)
{
	if(bIsGameServer == false)
	{
		m_map_CharServerPlayers.erase(AccID);
	}
	else
	{
		auto it = m_map_GameServerPlayers.find(AccID);
		if (it != m_map_GameServerPlayers.end())
		{
			if(it->second.serverFarmId == serverfarmId && it->second.serverChannelId == channelId)
				m_map_GameServerPlayers.erase(AccID);
			else
			{
				//this happen when player switch channel
			}
		}
	}
}

//check if player is in character or game server online
bool CSubNeighborServerInfoManager::IsPlayerOnline(ACCOUNTID AccID)
{
	auto it = m_map_CharServerPlayers.find(AccID);
	if (it != m_map_CharServerPlayers.end())
	{
		ERR_LOG(LOG_USER, "USER %u ALREADY LOGGED IN CHAR SERVER INDEX %u", it->first, it->second);
		return true;
	}

	auto it2 = m_map_GameServerPlayers.find(AccID);
	if (it2 != m_map_GameServerPlayers.end())
	{
		ERR_LOG(LOG_USER, "USER %u ALREADY LOGGED IN GAME SERVER %u CHANNEL %u", it2->first, it2->second.serverFarmId, it2->second.serverChannelId);
		return true;
	}

	return false;
}

void CSubNeighborServerInfoManager::PrintOnlinePlayers()
{
	ERR_LOG(LOG_GENERAL, "PRINT %I64u ONLINE PLAYERS - BEGIN", m_map_GameServerPlayers.size());

	for (auto it = m_map_GameServerPlayers.begin(); it != m_map_GameServerPlayers.end(); it++)
	{
		ERR_LOG(LOG_GENERAL, "Account: %u, Server-Farm: %u, Server-Channel: %u", it->first, it->second.serverFarmId, it->second.serverChannelId);
	}

	ERR_LOG(LOG_GENERAL, "PRINT ONLINE PLAYERS - END");
}

int CSubNeighborServerInfoManager::GetPlayerInCharServer(SERVERINDEX serverIndex)
{
	if (serverIndex == INVALID_SERVERINDEX)
		return (int)m_map_CharServerPlayers.size();

	return 0;
}

int CSubNeighborServerInfoManager::GetPlayerInGameServer(SERVERCHANNELID byChannel)
{
	if (byChannel == INVALID_SERVERCHANNELID)
		return (int)m_map_GameServerPlayers.size();

	return 0;
}






//--------------------------------------------------------------------------------------//
//		ADD/UPDATE AUTH SERVER SESSION
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::AddAuthServerSession(SERVERINDEX serverIndex, CAuthServerPassiveSession* pSession)
{
	if (m_mapAuthServerSession.find(serverIndex) != m_mapAuthServerSession.end())
	{
		printf("Auth server(index %u) already registered. Removing server index %u and adding new version \n", serverIndex, serverIndex);
		m_mapAuthServerSession.erase(serverIndex); //remove old if available
	}

	m_mapAuthServerSession.insert(std::make_pair(serverIndex, pSession)); //insert new
}

//--------------------------------------------------------------------------------------//
//		ADD/UPDATE CHAR SERVER
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::AddCharServerSession(SERVERINDEX serverIndex, CCharServerPassiveSession* pSession)
{
	if (m_mapCharacterServerSession.find(serverIndex) != m_mapCharacterServerSession.end())
	{
		printf("Char server(index %u) already registered. Removing server index %u and adding new version \n", serverIndex, serverIndex);
		m_mapCharacterServerSession.erase(serverIndex); //remove old if available
	}

	m_mapCharacterServerSession.insert(std::make_pair(serverIndex, pSession)); //insert new
}

//--------------------------------------------------------------------------------------//
//		ADD/UPDATE CHAT SERVER
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::AddChatServerSession(SERVERINDEX serverIndex, CChatServerPassiveSession* pSession)
{
	if (m_mapChatServerSession.find(serverIndex) != m_mapChatServerSession.end())
	{
		printf("Chat server(index %u) already registered. Removing server index %u and adding new version \n", serverIndex, serverIndex);
		m_mapChatServerSession.erase(serverIndex); //remove old if available
	}

	m_mapChatServerSession.insert(std::make_pair(serverIndex, pSession)); //insert new
}

//--------------------------------------------------------------------------------------//
//		ADD/UPDATE GAME SERVER
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::AddGameServerSession(_SERVER_PAIR_ID serverPairID, CGameServerPassiveSession* pSession)
{
	if (m_mapGameServerSession.find(serverPairID) != m_mapGameServerSession.end())
	{
		printf("Game server(farm %u channel %u) already registered. Removing server farm %u channel %u and adding new version \n", serverPairID.serverFarmId, serverPairID.serverChannelId, serverPairID.serverFarmId, serverPairID.serverChannelId);
		m_mapGameServerSession.erase(serverPairID); //remove old if available
	}

	m_mapGameServerSession.insert(std::make_pair(serverPairID, pSession)); //insert new
}


//--------------------------------------------------------------------------------------//
//		SEND PACKET TO SERVER
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::SendToServer(BYTE byServerType, SERVERINDEX serverIndex, SERVERFARMID serverFarmID, SERVERCHANNELID serverChannelID, CNtlPacket* pPacket)
{
	switch (byServerType)
	{
	case NTL_SERVER_TYPE_AUTH:
	{
		std::map<SERVERINDEX, CAuthServerPassiveSession *>::iterator it = m_mapAuthServerSession.find(serverIndex);
		if (it != m_mapAuthServerSession.end())
		{
			CAuthServerPassiveSession* pSession = it->second;
			if (pSession)
			{
				g_pApp->SendTo(pSession, pPacket);
			}
		}
	}
	break;
	case NTL_SERVER_TYPE_CHARACTER:
	{
		std::map<SERVERINDEX, CCharServerPassiveSession *>::iterator it = m_mapCharacterServerSession.find(serverIndex);
		if (it != m_mapCharacterServerSession.end())
		{
			CCharServerPassiveSession* pSession = it->second;
			if (pSession)
			{
				g_pApp->SendTo(pSession, pPacket);
			}
		}
	}
	break;
	case NTL_SERVER_TYPE_GAME:
	{
		std::map<_SERVER_PAIR_ID, CGameServerPassiveSession *>::iterator it = m_mapGameServerSession.find(_SERVER_PAIR_ID(serverFarmID, serverChannelID));
		if (it != m_mapGameServerSession.end())
		{
			CGameServerPassiveSession* pSession = it->second;
			if (pSession)
			{
				g_pApp->SendTo(pSession, pPacket);
			}
		}
	}
	break;
	case NTL_SERVER_TYPE_QUERY:
	{
	}
	break;
	case NTL_SERVER_TYPE_COMMUNITY:
	{
		std::map<SERVERINDEX, CChatServerPassiveSession *>::iterator it = m_mapChatServerSession.find(serverIndex);
		if (it != m_mapChatServerSession.end())
		{
			CChatServerPassiveSession* pSession = it->second;
			if (pSession)
			{
				g_pApp->SendTo(pSession, pPacket);
			}
		}
	}
	break;

	default: break;
	}
}


//--------------------------------------------------------------------------------------//
//		SEND PACKET TO ALL SERVER
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::BroadcastServer(BYTE byServerType, CNtlPacket* pPacket, SERVERINDEX exceptServerIndex/* = INVALID_SERVERINDEX*/, SERVERFARMID excepServerFarmID/* = INVALID_SERVERFARMID*/)
{
	switch (byServerType)
	{
		case NTL_SERVER_TYPE_AUTH:
		{
			for (std::map<SERVERINDEX, CAuthServerPassiveSession *>::iterator it = m_mapAuthServerSession.begin(); it != m_mapAuthServerSession.end(); it++)
			{
				CAuthServerPassiveSession* pSession = it->second;
				if (pSession && it->first != exceptServerIndex)
				{
					g_pApp->SendTo(pSession, pPacket);
				}
			}
		}
		break;
		case NTL_SERVER_TYPE_CHARACTER:
		{
			for (std::map<SERVERINDEX, CCharServerPassiveSession *>::iterator it = m_mapCharacterServerSession.begin(); it != m_mapCharacterServerSession.end(); it++)
			{
				CCharServerPassiveSession* pSession = it->second;
				if (pSession && it->first != exceptServerIndex)
				{
					g_pApp->SendTo(pSession, pPacket);
				}
			}
		}
		break;
		case NTL_SERVER_TYPE_GAME:
		{
			for (std::map<_SERVER_PAIR_ID, CGameServerPassiveSession *>::iterator it = m_mapGameServerSession.begin(); it != m_mapGameServerSession.end(); it++)
			{
				CGameServerPassiveSession* pSession = it->second;
				if (pSession && it->first.serverFarmId != excepServerFarmID && it->first.serverChannelId != exceptServerIndex)
				{
					g_pApp->SendTo(pSession, pPacket);
				}
			}
		}
		break;
		case NTL_SERVER_TYPE_QUERY:
		{
		}
		break;
		case NTL_SERVER_TYPE_COMMUNITY:
		{
			for (std::map<SERVERINDEX, CChatServerPassiveSession *>::iterator it = m_mapChatServerSession.begin(); it != m_mapChatServerSession.end(); it++)
			{
				CChatServerPassiveSession* pSession = it->second;
				if (pSession && it->first != exceptServerIndex)
				{
					g_pApp->SendTo(pSession, pPacket);
				}
			}
		}
		break;

		default: break;
	}
}


//--------------------------------------------------------------------------------------//
//		Broadcast to farm server channels
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::BroadcastFarmServer(BYTE byServerType, SERVERFARMID serverFarmID, SERVERCHANNELID exceptionChannelID, CNtlPacket* pPacket)
{
	switch (byServerType)
	{
		case NTL_SERVER_TYPE_GAME:
		{
			std::map<SERVERFARMID, sSERVER_FARM_INFO *>::iterator it = m_mapGameServerFarmInfo.find(serverFarmID);
			if (it != m_mapGameServerFarmInfo.end())
			{
				sSERVER_FARM_INFO* pFarm = it->second;
				if (pFarm)
				{
					for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it2 = pFarm->mapGameServerChannelInfo.begin(); it2 != pFarm->mapGameServerChannelInfo.end(); it2++)
					{
						sSERVER_CHANNEL_INFO* pChannel = it2->second;
						if (pChannel)
						{
							if (pChannel->serverFarmId == serverFarmID && pChannel->byServerChannelIndex != exceptionChannelID)
							{
								std::map<_SERVER_PAIR_ID, CGameServerPassiveSession *>::iterator it3 = m_mapGameServerSession.find(_SERVER_PAIR_ID(serverFarmID, pChannel->byServerChannelIndex));
								if (it3 != m_mapGameServerSession.end())
								{
									CGameServerPassiveSession* pSession = it3->second;
									if(pSession)
										g_pApp->SendTo(pSession, pPacket);
								}
							}
						}
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
bool CSubNeighborServerInfoManager::OnServerTurnedOn(sDBO_SERVER_INFO *pServerInfo)
{
	switch (pServerInfo->byServerType)
	{
		case NTL_SERVER_TYPE_AUTH:
		{
			//Send character servers to auth server
			for (std::list<sDBO_SERVER_INFO *>::iterator it = m_listCharacterServerInfo.begin(); it != m_listCharacterServerInfo.end(); it++)
			{
				sDBO_SERVER_INFO* pInfo = *it;
				if (pInfo && pInfo->bIsOn)
				{
					CNtlPacket aPacket(sizeof(sMA_SERVERS_INFO_ADD));
					sMA_SERVERS_INFO_ADD * aRes = (sMA_SERVERS_INFO_ADD *)aPacket.GetPacketData();
					aRes->wOpCode = MA_SERVERS_INFO_ADD;
					aRes->serverInfo = *pInfo;
					aPacket.SetPacketLen(sizeof(sMA_SERVERS_INFO_ADD));
					BroadcastServer(NTL_SERVER_TYPE_AUTH, &aPacket);
				}
			}
		}
		break;
		case NTL_SERVER_TYPE_CHARACTER:
		{
			//send to auth server
			CNtlPacket aPacket(sizeof(sMA_SERVERS_INFO_ADD));
			sMA_SERVERS_INFO_ADD * aRes = (sMA_SERVERS_INFO_ADD *)aPacket.GetPacketData();
			aRes->wOpCode = MA_SERVERS_INFO_ADD;
			aRes->serverInfo = *pServerInfo;
			aPacket.SetPacketLen(sizeof(sMA_SERVERS_INFO_ADD));
			BroadcastServer(NTL_SERVER_TYPE_AUTH, &aPacket);

			//send all game server to char server
			for (std::map<SERVERFARMID, sSERVER_FARM_INFO *>::iterator it = m_mapGameServerFarmInfo.begin(); it != m_mapGameServerFarmInfo.end(); it++)
			{
				sSERVER_FARM_INFO* pServerFarm = it->second;
				if (pServerFarm)
				{
					//send farm server info to character server
					CNtlPacket cPacket(sizeof(sMC_GAME_SERVER_FARM_INFO));
					sMC_GAME_SERVER_FARM_INFO * cRes = (sMC_GAME_SERVER_FARM_INFO *)cPacket.GetPacketData();
					cRes->wOpCode = MC_GAME_SERVER_FARM_INFO;
					cRes->gameServerFarmInfo.serverFarmId = pServerFarm->serverFarmId;
					wcscpy_s(cRes->gameServerFarmInfo.wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, pServerFarm->wszGameServerFarmName);
					cRes->gameServerFarmInfo.byServerStatus = pServerFarm->byServerStatus;
					cRes->gameServerFarmInfo.dwMaxLoad = pServerFarm->dwMaxLoad;
					cRes->gameServerFarmInfo.dwLoad = pServerFarm->dwLoad;
					cPacket.SetPacketLen(sizeof(sMC_GAME_SERVER_FARM_INFO));
					SendToServer(NTL_SERVER_TYPE_CHARACTER, pServerInfo->byServerIndex, pServerInfo->serverFarmId, pServerInfo->byServerChannelIndex, &cPacket);

					for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it2 = pServerFarm->mapGameServerChannelInfo.begin(); it2 != pServerFarm->mapGameServerChannelInfo.end(); it2++) //rotate channels from farm server
					{
						sSERVER_CHANNEL_INFO* pChannelInfo = it2->second;
						if (pChannelInfo)
						{
							//send channel info to character server
							CNtlPacket cPacket2(sizeof(sMC_GAME_SERVER_CHANNEL_INFO));
							sMC_GAME_SERVER_CHANNEL_INFO * cRes2 = (sMC_GAME_SERVER_CHANNEL_INFO *)cPacket2.GetPacketData();
							cRes2->wOpCode = MC_GAME_SERVER_CHANNEL_INFO;
							cRes2->gameServerChannelInfo.serverFarmId = pChannelInfo->serverFarmId;
							cRes2->gameServerChannelInfo.byServerChannelIndex = pChannelInfo->byServerChannelIndex;
							cRes2->gameServerChannelInfo.bIsVisible = pChannelInfo->bIsVisible;
							cRes2->gameServerChannelInfo.byServerStatus = pChannelInfo->byServerStatus;
							cRes2->gameServerChannelInfo.dwMaxLoad = pChannelInfo->dwMaxLoad;
							cRes2->gameServerChannelInfo.dwLoad = pChannelInfo->dwLoad;
							cRes2->gameServerChannelInfo.bIsScrambleChannel = pChannelInfo->bIsScrambleChannel;
							memcpy(&cRes2->gameServerChannelInfo.sChannelBuff, &pChannelInfo->sChannelBuff, sizeof(_CHANNEL_BUFF));
							cRes2->gameServerChannelInfo.bUnknown = pChannelInfo->bUnknown;
							cPacket2.SetPacketLen(sizeof(sMC_GAME_SERVER_CHANNEL_INFO));
							SendToServer(NTL_SERVER_TYPE_CHARACTER, pServerInfo->byServerIndex, pServerInfo->serverFarmId, pServerInfo->byServerChannelIndex, &cPacket2);

							CNtlPacket cPacket3(sizeof(sMC_SERVERS_INFO_ADD));
							sMC_SERVERS_INFO_ADD * cRes3 = (sMC_SERVERS_INFO_ADD *)cPacket3.GetPacketData();
							cRes3->wOpCode = MC_SERVERS_INFO_ADD;
							cRes3->serverInfo = *pChannelInfo->listGameServerInfoRef.front();
							cPacket3.SetPacketLen(sizeof(sMC_SERVERS_INFO_ADD));
							SendToServer(NTL_SERVER_TYPE_CHARACTER, pServerInfo->byServerIndex, pServerInfo->serverFarmId, pServerInfo->byServerChannelIndex, &cPacket3);
						}
					}
				}
			}

		}
		break;

		case NTL_SERVER_TYPE_GAME:
		{
			//when gameserver start then set farm-server dwLoad/dwMaxLoad
			sSERVER_FARM_INFO* pFarm = GetServerFarmInfo(pServerInfo->serverFarmId);
			if (pFarm)
			{
				pFarm->dwLoad += pServerInfo->dwLoad;
				pFarm->dwMaxLoad += pServerInfo->dwMaxLoad;
			}
			else
			{
				printf("OnServerTurnedOn: Cant find game server farm info. Farm ID %u Server Index %u\n", pServerInfo->serverFarmId, pServerInfo->byServerIndex);
				return false;
			}

			sSERVER_CHANNEL_INFO* pChannel = GetServerChannelInfo_Internal(pFarm, pServerInfo->byServerChannelIndex);
			if (!pChannel)
			{
				printf("OnServerTurnedOn: Cant find game server channel info. Farm ID %u Channel id %u Server Index %u\n", pServerInfo->serverFarmId, pServerInfo->byServerChannelIndex, pServerInfo->byServerIndex);
				return false;
			}

			//GAME SERVER ROTATION
			for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it = pFarm->mapGameServerChannelInfo.begin(); it != pFarm->mapGameServerChannelInfo.end(); it++)
			{
				sSERVER_CHANNEL_INFO* pChannelInfo = it->second;
				if (pChannelInfo && pChannelInfo->byServerStatus == DBO_SERVER_STATUS_UP)
				{
					if (pChannelInfo->byServerChannelIndex != pChannel->byServerChannelIndex) //dont send to myself
					{
						//send partner-channels to this channel (all channel in same farm server)
						CNtlPacket ggPacket(sizeof(sMG_GAME_SERVER_CHANNEL_INFO));
						sMG_GAME_SERVER_CHANNEL_INFO * ggRes = (sMG_GAME_SERVER_CHANNEL_INFO *)ggPacket.GetPacketData();
						ggRes->wOpCode = MG_GAME_SERVER_CHANNEL_INFO;
						ggRes->gameServerChannelInfo.serverFarmId = pChannelInfo->serverFarmId;
						ggRes->gameServerChannelInfo.byServerChannelIndex = pChannelInfo->byServerChannelIndex;
						ggRes->gameServerChannelInfo.bIsVisible = pChannelInfo->bIsVisible;
						ggRes->gameServerChannelInfo.byServerStatus = pChannelInfo->byServerStatus;
						ggRes->gameServerChannelInfo.dwMaxLoad = pChannelInfo->dwMaxLoad;
						ggRes->gameServerChannelInfo.dwLoad = pChannelInfo->dwLoad;
						ggRes->gameServerChannelInfo.bIsScrambleChannel = pChannelInfo->bIsScrambleChannel;
						memcpy(&ggRes->gameServerChannelInfo.sChannelBuff, &pChannelInfo->sChannelBuff, sizeof(_CHANNEL_BUFF));
						ggRes->gameServerChannelInfo.bUnknown = pChannelInfo->bUnknown;
						ggPacket.SetPacketLen(sizeof(sMG_GAME_SERVER_CHANNEL_INFO));
						SendToServer(NTL_SERVER_TYPE_GAME, pChannel->byServerChannelIndex, pChannel->serverFarmId, pChannel->byServerChannelIndex, &ggPacket);

						CNtlPacket ggPacket2(sizeof(sMG_SERVERS_INFO_ADD));
						sMG_SERVERS_INFO_ADD * ggRes2 = (sMG_SERVERS_INFO_ADD *)ggPacket2.GetPacketData();
						ggRes2->wOpCode = MG_SERVERS_INFO_ADD;
						ggRes2->serverInfo = *pChannelInfo->listGameServerInfoRef.front();
						ggPacket2.SetPacketLen(sizeof(sMG_SERVERS_INFO_ADD));
						SendToServer(NTL_SERVER_TYPE_GAME, pChannel->byServerChannelIndex, pChannel->serverFarmId, pChannel->byServerChannelIndex, &ggPacket2);


						//send channel to partner-channels (all channel in same farm server)
						CNtlPacket ggPacket3(sizeof(sMG_GAME_SERVER_CHANNEL_INFO));
						sMG_GAME_SERVER_CHANNEL_INFO * ggRes3 = (sMG_GAME_SERVER_CHANNEL_INFO *)ggPacket3.GetPacketData();
						ggRes3->wOpCode = MG_GAME_SERVER_CHANNEL_INFO;
						ggRes3->gameServerChannelInfo.serverFarmId = pChannel->serverFarmId;
						ggRes3->gameServerChannelInfo.byServerChannelIndex = pChannel->byServerChannelIndex;
						ggRes3->gameServerChannelInfo.bIsVisible = pChannel->bIsVisible;
						ggRes3->gameServerChannelInfo.byServerStatus = pChannel->byServerStatus;
						ggRes3->gameServerChannelInfo.dwMaxLoad = pChannel->dwMaxLoad;
						ggRes3->gameServerChannelInfo.dwLoad = pChannel->dwLoad;
						ggRes3->gameServerChannelInfo.bIsScrambleChannel = pChannel->bIsScrambleChannel;
						memcpy(&ggRes3->gameServerChannelInfo.sChannelBuff, &pChannel->sChannelBuff, sizeof(_CHANNEL_BUFF));
						ggRes3->gameServerChannelInfo.bUnknown = pChannel->bUnknown;
						ggPacket3.SetPacketLen(sizeof(sMG_GAME_SERVER_CHANNEL_INFO));
						SendToServer(NTL_SERVER_TYPE_GAME, pChannelInfo->byServerChannelIndex, pChannelInfo->serverFarmId, pChannelInfo->byServerChannelIndex, &ggPacket3);

						CNtlPacket ggPacket4(sizeof(sMG_SERVERS_INFO_ADD));
						sMG_SERVERS_INFO_ADD * ggRes4 = (sMG_SERVERS_INFO_ADD *)ggPacket4.GetPacketData();
						ggRes4->wOpCode = MG_SERVERS_INFO_ADD;
						ggRes4->serverInfo = *pChannel->listGameServerInfoRef.front();
						ggPacket4.SetPacketLen(sizeof(sMG_SERVERS_INFO_ADD));
						SendToServer(NTL_SERVER_TYPE_GAME, pChannelInfo->byServerChannelIndex, pChannelInfo->serverFarmId, pChannelInfo->byServerChannelIndex, &ggPacket4);
					}
				}
			}

			//CHAR SERVER ROTATION
			for (std::list<sDBO_SERVER_INFO *>::iterator it2 = m_listCharacterServerInfo.begin(); it2 != m_listCharacterServerInfo.end(); it2++)
			{
				sDBO_SERVER_INFO* pCharServer = *it2;
				if (pCharServer && pCharServer->bIsOn)
				{
					//send char server to this channel
					CNtlPacket cgPacket(sizeof(sMG_SERVERS_INFO_ADD));
					sMG_SERVERS_INFO_ADD * cgRes = (sMG_SERVERS_INFO_ADD *)cgPacket.GetPacketData();
					cgRes->wOpCode = MG_SERVERS_INFO_ADD;
					cgRes->serverInfo = *pCharServer;
					cgPacket.SetPacketLen(sizeof(sMG_SERVERS_INFO_ADD));
					SendToServer(NTL_SERVER_TYPE_GAME, pChannel->byServerChannelIndex, pChannel->serverFarmId, pChannel->byServerChannelIndex, &cgPacket);


					//send game server to char server
					CNtlPacket cPacket(sizeof(sMC_GAME_SERVER_FARM_INFO));
					sMC_GAME_SERVER_FARM_INFO * cRes = (sMC_GAME_SERVER_FARM_INFO *)cPacket.GetPacketData();
					cRes->wOpCode = MC_GAME_SERVER_FARM_INFO;
					cRes->gameServerFarmInfo.serverFarmId = pFarm->serverFarmId;
					wcscpy_s(cRes->gameServerFarmInfo.wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, pFarm->wszGameServerFarmName);
					cRes->gameServerFarmInfo.byServerStatus = pFarm->byServerStatus;
					cRes->gameServerFarmInfo.dwMaxLoad = pFarm->dwMaxLoad;
					cRes->gameServerFarmInfo.dwLoad = pFarm->dwLoad;
					cPacket.SetPacketLen(sizeof(sMC_GAME_SERVER_FARM_INFO));
					SendToServer(NTL_SERVER_TYPE_CHARACTER, pCharServer->byServerIndex, pCharServer->serverFarmId, pCharServer->byServerChannelIndex, &cPacket);

					CNtlPacket cPacket2(sizeof(sMC_GAME_SERVER_CHANNEL_INFO));
					sMC_GAME_SERVER_CHANNEL_INFO * cRes2 = (sMC_GAME_SERVER_CHANNEL_INFO *)cPacket2.GetPacketData();
					cRes2->wOpCode = MC_GAME_SERVER_CHANNEL_INFO;
					cRes2->gameServerChannelInfo.serverFarmId = pChannel->serverFarmId;
					cRes2->gameServerChannelInfo.byServerChannelIndex = pChannel->byServerChannelIndex;
					cRes2->gameServerChannelInfo.bIsVisible = pChannel->bIsVisible;
					cRes2->gameServerChannelInfo.byServerStatus = pChannel->byServerStatus;
					cRes2->gameServerChannelInfo.dwMaxLoad = pChannel->dwMaxLoad;
					cRes2->gameServerChannelInfo.dwLoad = pChannel->dwLoad;
					cRes2->gameServerChannelInfo.bIsScrambleChannel = pChannel->bIsScrambleChannel;
					memcpy(&cRes2->gameServerChannelInfo.sChannelBuff, &pChannel->sChannelBuff, sizeof(_CHANNEL_BUFF));
					cRes2->gameServerChannelInfo.bUnknown = pChannel->bUnknown;
					cPacket2.SetPacketLen(sizeof(sMC_GAME_SERVER_CHANNEL_INFO));
					SendToServer(NTL_SERVER_TYPE_CHARACTER, pCharServer->byServerIndex, pCharServer->serverFarmId, pCharServer->byServerChannelIndex, &cPacket2);

					CNtlPacket cPacket3(sizeof(sMC_SERVERS_INFO_ADD));
					sMC_SERVERS_INFO_ADD * cRes3 = (sMC_SERVERS_INFO_ADD *)cPacket3.GetPacketData();
					cRes3->wOpCode = MC_SERVERS_INFO_ADD;
					cRes3->serverInfo = *pServerInfo;
					cPacket3.SetPacketLen(sizeof(sMC_SERVERS_INFO_ADD));
					SendToServer(NTL_SERVER_TYPE_CHARACTER, pCharServer->byServerIndex, pCharServer->serverFarmId, pCharServer->byServerChannelIndex, &cPacket3);
				}
			}
		}

		default: break;
	}


	return CNeighborServerInfoManager::OnServerTurnedOn(pServerInfo);
}

//--------------------------------------------------------------------------------------//
//		SERVER IS ONLINE AND NOW OFFLINE ANNOUNCEMENT
//--------------------------------------------------------------------------------------//
bool CSubNeighborServerInfoManager::OnServerTurnedOff(sDBO_SERVER_INFO *pServerInfo)
{
	switch (pServerInfo->byServerType)
	{
		case NTL_SERVER_TYPE_AUTH:
		{
			m_mapAuthServerSession.erase(pServerInfo->byServerIndex); //remove session
		}
		break;

		case NTL_SERVER_TYPE_CHARACTER:
		{
			m_mapCharacterServerSession.erase(pServerInfo->byServerIndex); //remove session
			RemoveAllPlayerFromCharServer(pServerInfo->byServerIndex);

			//send to all auth servers
			CNtlPacket aPacket(sizeof(sMA_SERVER_INFO_CHANGED_NFY));
			sMA_SERVER_INFO_CHANGED_NFY * aRes = (sMA_SERVER_INFO_CHANGED_NFY *)aPacket.GetPacketData();
			aRes->wOpCode = MA_SERVER_INFO_CHANGED_NFY;
			aRes->byServerType = NTL_SERVER_TYPE_CHARACTER;
			aRes->serverFarmId = pServerInfo->serverFarmId;
			aRes->serverChannelId = pServerInfo->byServerChannelIndex;
			aRes->serverIndex = pServerInfo->byServerIndex;
			aRes->bIsOn = false;
			aRes->byRunningState = DBO_SERVER_RUNNING_STATE_SHUTDOWN;
			aRes->dwLoad = 0;
			aPacket.SetPacketLen(sizeof(sMA_SERVER_INFO_CHANGED_NFY));
			BroadcastServer(NTL_SERVER_TYPE_AUTH, &aPacket);

			//send to all game servers
			CNtlPacket gPacket(sizeof(sMG_SERVER_INFO_CHANGED_NFY));
			sMG_SERVER_INFO_CHANGED_NFY * gRes = (sMG_SERVER_INFO_CHANGED_NFY *)gPacket.GetPacketData();
			gRes->wOpCode = MG_SERVER_INFO_CHANGED_NFY;
			gRes->byServerType = NTL_SERVER_TYPE_CHARACTER;
			gRes->serverFarmId = pServerInfo->serverFarmId;
			gRes->serverChannelId = pServerInfo->byServerChannelIndex;
			gRes->serverIndex = pServerInfo->byServerIndex;
			gRes->bIsOn = false;
			gRes->byRunningState = DBO_SERVER_RUNNING_STATE_SHUTDOWN;
			gRes->dwLoad = 0;
			gPacket.SetPacketLen(sizeof(sMG_SERVER_INFO_CHANGED_NFY));
			BroadcastServer(NTL_SERVER_TYPE_GAME, &gPacket);
		}
		break;

		case NTL_SERVER_TYPE_GAME:
		{
			m_mapGameServerSession.erase(_SERVER_PAIR_ID(pServerInfo->serverFarmId, pServerInfo->byServerChannelIndex)); //remove session
			RemoveAllPlayerFromGameServer(_SERVER_PAIR_ID(pServerInfo->serverFarmId, pServerInfo->byServerChannelIndex));

			sSERVER_FARM_INFO* pFarm = GetServerFarmInfo(pServerInfo->serverFarmId);
			if (!pFarm)
				return false;

			sSERVER_CHANNEL_INFO* pChannel = g_pSrvMgr->GetServerChannelInfo_Internal(pFarm, pServerInfo->byServerChannelIndex);
			if (!pChannel)
				return false;

			pFarm->dwLoad -= pChannel->dwLoad;
			pFarm->dwMaxLoad -= pChannel->dwMaxLoad;

			pChannel->dwLoad = 0;
			pChannel->byServerStatus = DBO_SERVER_STATUS_DOWN;

			//send to all char server that game farm server updated
			CNtlPacket cPacket(sizeof(sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY));
			sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY * cRes = (sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY *)cPacket.GetPacketData();
			cRes->wOpCode = MC_GAME_SERVER_FARM_INFO_CHANGED_NFY;
			cRes->serverFarmId = pChannel->serverFarmId;
			cRes->byServerStatus = DBO_SERVER_STATUS_DOWN;
			cRes->dwMaxLoad = pFarm->dwMaxLoad;
			cRes->dwLoad = pFarm->dwLoad;
			cPacket.SetPacketLen(sizeof(sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY));
			BroadcastServer(NTL_SERVER_TYPE_CHARACTER, &cPacket);

			//send to all character servers that channel X updated
			CNtlPacket cPacket2(sizeof(sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
			sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY * cRes2 = (sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY *)cPacket2.GetPacketData();
			cRes2->wOpCode = MC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY;
			cRes2->byServerChannelIndex = pChannel->byServerChannelIndex;
			cRes2->byServerIndex = pChannel->byServerChannelIndex;
			cRes2->byServerStatus = DBO_SERVER_STATUS_DOWN;
			cRes2->dwLoad = 0;
			cRes2->serverFarmId = pChannel->serverFarmId;
			cRes2->bIsScrambleChannel = pChannel->bIsScrambleChannel;
			cPacket2.SetPacketLen(sizeof(sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
			BroadcastServer(NTL_SERVER_TYPE_CHARACTER, &cPacket2);

			//send to all channels in same game farm server
			for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it = pFarm->mapGameServerChannelInfo.begin(); it != pFarm->mapGameServerChannelInfo.end(); it++)
			{
				sSERVER_CHANNEL_INFO* pChannelInfo = it->second;
				if (pChannelInfo && pChannelInfo->byServerStatus == DBO_SERVER_STATUS_UP) 
				{
					CNtlPacket gPacket(sizeof(sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
					sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY * gRes = (sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY *)gPacket.GetPacketData();
					gRes->wOpCode = MG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY;
					gRes->byServerChannelIndex = pChannel->byServerChannelIndex;
					gRes->byServerStatus = DBO_SERVER_STATUS_DOWN;
					gRes->dwLoad = 0;
					gRes->serverFarmId = pChannel->serverFarmId;
					gRes->bIsScrambleChannel = pChannel->bIsScrambleChannel;
					gPacket.SetPacketLen(sizeof(sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY));
					SendToServer(NTL_SERVER_TYPE_GAME, pChannelInfo->byServerChannelIndex, pChannelInfo->serverFarmId, pChannelInfo->byServerChannelIndex, &gPacket);
				}
			}

		}
		break;
	}

	return CNeighborServerInfoManager::OnServerTurnedOff(pServerInfo);
}


//--------------------------------------------------------------------------------------//
//		UPDATE PLAYER COUNT (dwLoad)
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::UpdatePlayerCount(BYTE byServerType, SERVERINDEX serverIndex, SERVERFARMID serverFarmID, SERVERCHANNELID serverChannelID, bool bPlus, DWORD dwCount)
{
	switch (byServerType)
	{
		case NTL_SERVER_TYPE_CHARACTER:
		{
			sDBO_SERVER_INFO* pCharServer = g_pSrvMgr->GetServerInfo(byServerType, serverFarmID, serverChannelID, serverIndex);
			if (pCharServer)
			{
				if (bPlus)
					pCharServer->dwLoad = UnsignedSafeIncrease<DWORD>(pCharServer->dwLoad, dwCount);
				else
					pCharServer->dwLoad = UnsignedSafeDecrease<DWORD>(pCharServer->dwLoad, dwCount);
			}
		}
		break;
		case NTL_SERVER_TYPE_GAME:
		{
			sSERVER_FARM_INFO* pFarm = g_pSrvMgr->GetServerFarmInfo(serverFarmID);
			if (!pFarm)
				return;

			sSERVER_CHANNEL_INFO* pChannel = g_pSrvMgr->GetServerChannelInfo_Internal(pFarm, serverChannelID);
			if (!pChannel)
				return;

			sDBO_SERVER_INFO* pServerInfo = pChannel->listGameServerInfoRef.front();
			if (!pServerInfo)
				return;

			if (bPlus)
			{
				pFarm->dwLoad = UnsignedSafeIncrease<DWORD>(pFarm->dwLoad, dwCount);
				pChannel->dwLoad = UnsignedSafeIncrease<DWORD>(pChannel->dwLoad, dwCount);
				pServerInfo->dwLoad = UnsignedSafeIncrease<DWORD>(pServerInfo->dwLoad, dwCount);
			}
			else
			{
				pFarm->dwLoad = UnsignedSafeDecrease<DWORD>(pFarm->dwLoad, dwCount);
				pChannel->dwLoad = UnsignedSafeDecrease<DWORD>(pChannel->dwLoad, dwCount);
				pServerInfo->dwLoad = UnsignedSafeIncrease<DWORD>(pServerInfo->dwLoad, dwCount);
			}
		}
		break;

		default: printf("UpdatePlayerCount: byServerType %u not set. Index %u farm %u channel %u", byServerType, serverIndex, serverFarmID, serverChannelID); break;
	}
}

