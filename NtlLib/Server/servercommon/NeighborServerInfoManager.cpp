#include "stdafx.h"
#include "NeighborServerInfoManager.h"
#include "Utils.h"
#include <time.h>

#define AUTHKEY_KEY "s1f5dfg6sfg" //max 16

const DWORD AUTH_KEY_EXPIRE_SEC = 1000 * 60 * 15; //15 minutes



CNeighborServerInfoManager::CNeighborServerInfoManager()
{
	Init();
}

CNeighborServerInfoManager::~CNeighborServerInfoManager()
{
	Destroy();
}



bool CNeighborServerInfoManager::Create(eNtlServerType byOwnerServerType)
{
	if (byOwnerServerType <= NTL_SERVER_TYPE_LAST)
	{
		m_byOwnerServerType = (BYTE)byOwnerServerType;
		return true;
	}

	return false;
}


void CNeighborServerInfoManager::Destroy()
{
	DestroyAllServerInfo();
	m_byOwnerServerType = NTL_SERVER_TYPE_INVALID;
	m_byOwnerServerIndex = 0;
}


void CNeighborServerInfoManager::Init()
{
	m_listMasterServerInfo.clear();
	m_listAuthServerInfo.clear();
	m_listCharacterServerInfo.clear();
	m_listGameServerInfo.clear();
	m_listQueryServerInfo.clear();
	m_listCommunityServerInfo.clear();
	
	m_pAuthServerFarmInfo = NULL;
	m_pCharacterServerFarmInfo = NULL;

	m_mapGameServerFarmInfo.clear();

	m_byOwnerServerType = NTL_SERVER_TYPE_INVALID;
	m_byOwnerServerIndex = 0;
}


bool CNeighborServerInfoManager::AddAuthServerFarmInfo(sDBO_AUTH_SERVER_FARM_INFO *pServerFarmInfoRef)
{
	if (pServerFarmInfoRef)
	{
		if (m_pAuthServerFarmInfo)
		{
			printf("Line 62: m_pAuthServerFarmInfo already set \n");
			return false;
		}

		m_pAuthServerFarmInfo = new sAUTH_SERVER_FARM_INFO;

		m_pAuthServerFarmInfo->byServerStatus = pServerFarmInfoRef->byServerStatus;
		m_pAuthServerFarmInfo->dwMaxLoad = pServerFarmInfoRef->dwMaxLoad;
		m_pAuthServerFarmInfo->dwLoad = pServerFarmInfoRef->dwLoad;

		return true;
	}

	printf("Line 78: pServerFarmInfoRef is null \n");
	return false;
}


bool CNeighborServerInfoManager::AddCharacterServerFarmInfo(sDBO_CHARACTER_SERVER_FARM_INFO *pServerFarmInfoRef)
{
	if (pServerFarmInfoRef)
	{
		if (m_pCharacterServerFarmInfo)
		{
			printf("Line 89: m_pCharacterServerFarmInfo already set \n");
			return false;
		}

		m_pCharacterServerFarmInfo = new sCHARACTER_SERVER_FARM_INFO;

		m_pCharacterServerFarmInfo->byServerStatus = pServerFarmInfoRef->byServerStatus;
		m_pCharacterServerFarmInfo->dwMaxLoad = pServerFarmInfoRef->dwMaxLoad;
		m_pCharacterServerFarmInfo->dwLoad = pServerFarmInfoRef->dwLoad;

		return true;
	}

	printf("Line 103: pServerFarmInfoRef is null \n");
	return false;
}


bool CNeighborServerInfoManager::AddServerFarmInfo(sDBO_GAME_SERVER_FARM_INFO *pServerFarmInfoRef)
{
	if (!pServerFarmInfoRef)
	{
		printf("Line 112: pServerFarmInfoRef is null \n");
		return false;
	}

	if (GetServerFarmInfo_Internal(pServerFarmInfoRef->serverFarmId))
		return false;

	sSERVER_FARM_INFO* pInfo = new sSERVER_FARM_INFO(pServerFarmInfoRef);

	m_mapGameServerFarmInfo.insert(std::make_pair(pInfo->serverFarmId, pInfo));
	return true;
}


bool CNeighborServerInfoManager::AddServerChannelInfo(sDBO_GAME_SERVER_CHANNEL_INFO *pServerChannelInfoRef)
{
	if (!pServerChannelInfoRef)
	{
		printf("Line 130: pServerChannelInfoRef is null \n");
		return false;
	}
	
	sSERVER_FARM_INFO* pServerFarmInfo = GetServerFarmInfo_Internal(pServerChannelInfoRef->serverFarmId);
	if (pServerFarmInfo)
	{
		if (GetServerChannelInfo_Internal(pServerFarmInfo, pServerChannelInfoRef->byServerChannelIndex))
			return false;

		sSERVER_CHANNEL_INFO* pInfo = new sSERVER_CHANNEL_INFO(pServerChannelInfoRef);

		pServerFarmInfo->mapGameServerChannelInfo.insert(std::make_pair(pInfo->byServerChannelIndex, pInfo));

		return true;
	}

	return false;
}


bool CNeighborServerInfoManager::AddServerInfo(sDBO_SERVER_INFO *pServerInfoRef)
{
	if (!pServerInfoRef)
	{
		printf("Line 153: pServerInfoRef is null \n");
		return false;
	}

	std::list<sDBO_SERVER_INFO* >* pServerInfoList = GetServerInfoList(pServerInfoRef->serverFarmId, pServerInfoRef->byServerChannelIndex, pServerInfoRef->byServerType);

	if (pServerInfoList)
	{
		if (GetServerInfoHelper(pServerInfoList, pServerInfoRef->byServerIndex))
			return false;

		sDBO_SERVER_INFO* pInfo = new sDBO_SERVER_INFO(pServerInfoRef);

		pServerInfoList->push_back(pInfo);

		switch (pServerInfoRef->byServerType)
		{
			case NTL_SERVER_TYPE_GAME:
			{
				m_listGameServerInfo.push_back(pInfo);
			}
			break;
			case NTL_SERVER_TYPE_NPC:
			{
				//	m_listNpcServerInfo.push_back(pInfo);
			}
			break;
			case NTL_SERVER_TYPE_QUERY:
			{
				m_listQueryServerInfo.push_back(pInfo);
			}
			break;
			case NTL_SERVER_TYPE_COMMUNITY:
			{
				m_listCommunityServerInfo.push_back(pInfo);
			}
			break;
		}

		return true;
	}

	return false;
}



bool CNeighborServerInfoManager::RefreshAuthServerFarmInfo(sDBO_AUTH_SERVER_FARM_INFO *pServerFarmInfoRef)
{
	if (!pServerFarmInfoRef)
	{
		printf("Line 206: pServerFarmInfoRef is null \n");
		return false;
	}

	if (m_pAuthServerFarmInfo == NULL)
		m_pAuthServerFarmInfo = new sAUTH_SERVER_FARM_INFO;


	m_pAuthServerFarmInfo->byServerStatus = pServerFarmInfoRef->byServerStatus;
	m_pAuthServerFarmInfo->dwMaxLoad = pServerFarmInfoRef->dwMaxLoad;
	m_pAuthServerFarmInfo->dwLoad = pServerFarmInfoRef->dwLoad;

	return true;
}


bool CNeighborServerInfoManager::RefreshCharacterServerFarmInfo(sDBO_CHARACTER_SERVER_FARM_INFO *pServerFarmInfoRef)
{
	if (!pServerFarmInfoRef)
	{
		printf("Line 230: pServerFarmInfoRef is null \n");
		return false;
	}

	if (m_pCharacterServerFarmInfo == NULL)
		m_pCharacterServerFarmInfo = new sCHARACTER_SERVER_FARM_INFO;

	m_pCharacterServerFarmInfo->byServerStatus = pServerFarmInfoRef->byServerStatus;
	m_pCharacterServerFarmInfo->dwMaxLoad = pServerFarmInfoRef->dwMaxLoad;
	m_pCharacterServerFarmInfo->dwLoad = pServerFarmInfoRef->dwLoad;

	return true;
}


bool CNeighborServerInfoManager::RefreshServerFarmInfo(sDBO_GAME_SERVER_FARM_INFO *pServerFarmInfoRef)
{
	if (!pServerFarmInfoRef)
	{
		printf("Line 252: pServerFarmInfoRef is null \n");
		return false;
	}

	sSERVER_FARM_INFO* pInfo = GetServerFarmInfo_Internal(pServerFarmInfoRef->serverFarmId);
	if (pInfo)
	{
		pInfo->operator=(*pServerFarmInfoRef);
		return true;
	}
	
	sSERVER_FARM_INFO* pNewInfo = new sSERVER_FARM_INFO(pServerFarmInfoRef);
	m_mapGameServerFarmInfo.insert(std::make_pair(pServerFarmInfoRef->serverFarmId, pNewInfo));
	printf("Add new Server farm. ID %u \n", pServerFarmInfoRef->serverFarmId);
	return true;
}


bool CNeighborServerInfoManager::RefreshServerChannelInfo(sDBO_GAME_SERVER_CHANNEL_INFO *pServerChannelInfoRef)
{
	if (!pServerChannelInfoRef)
	{
		printf("Line 270: pServerChannelInfoRef is null \n");
		return false;
	}

	sSERVER_FARM_INFO* pFarm = GetServerFarmInfo_Internal(pServerChannelInfoRef->serverFarmId);
	if (!pFarm)
		return false;

	sSERVER_CHANNEL_INFO* pInfo = GetServerChannelInfo_Internal(pFarm, pServerChannelInfoRef->byServerChannelIndex);
	if (pInfo)
	{
		pInfo->operator=(*pServerChannelInfoRef);
		return true;
	}

	sSERVER_CHANNEL_INFO* pNewInfo = new sSERVER_CHANNEL_INFO(pServerChannelInfoRef);
	pFarm->mapGameServerChannelInfo.insert(std::make_pair(pServerChannelInfoRef->byServerChannelIndex, pNewInfo));
	printf("Add new Server Channel. Farm %u Channel %u\n", pServerChannelInfoRef->serverFarmId, pServerChannelInfoRef->byServerChannelIndex);
	return true;
}


bool CNeighborServerInfoManager::RefreshServerInfo(sDBO_SERVER_INFO *pServerInfoRef)
{
	if (!pServerInfoRef)
	{
		printf("Line 297: pServerInfoRef is null \n");
		return false;
	}

	std::list<sDBO_SERVER_INFO* >* pServerInfoList = GetServerInfoList(pServerInfoRef->serverFarmId, pServerInfoRef->byServerChannelIndex, pServerInfoRef->byServerType);
	if (!pServerInfoList)
	{
		printf("Line 304: GetServerInfoList failed, pServerInfoRef->serverFarmId = %u, pServerInfoRef->byServerChannelIndex = %u, pServerInfoRef->byServerType = %u \n", pServerInfoRef->serverFarmId, pServerInfoRef->byServerChannelIndex, pServerInfoRef->byServerType);
		return false;
	}

	sDBO_SERVER_INFO* pHelper = GetServerInfoHelper(pServerInfoList, pServerInfoRef->byServerIndex);
	if (pHelper)
	{
		pHelper->operator=(*pServerInfoRef);
		return true;
	}

	sDBO_SERVER_INFO* pNewInfo = new sDBO_SERVER_INFO(pServerInfoRef);
	printf("Add new Server Type %u, Farm %u Channel %u Index %u \n", pServerInfoRef->byServerType, pServerInfoRef->serverFarmId, pServerInfoRef->byServerChannelIndex, pServerInfoRef->byServerIndex);
	pServerInfoList->push_back(pNewInfo);

	switch (pServerInfoRef->byServerType)
	{
		case NTL_SERVER_TYPE_GAME:
		{
			m_listGameServerInfo.push_back(pNewInfo);
		}
		break;
		case NTL_SERVER_TYPE_NPC:
		{
		//	m_listNpcServerInfo.push_back(pNewInfo);
		}
		break;
		case NTL_SERVER_TYPE_QUERY:
		{
			m_listQueryServerInfo.push_back(pNewInfo);
		}
		break;
		case NTL_SERVER_TYPE_COMMUNITY:
		{
			m_listCommunityServerInfo.push_back(pNewInfo);
		}
		break;
	}

	return true;
}


bool CNeighborServerInfoManager::DestroyAllServerInfo()
{
	if (m_pAuthServerFarmInfo)
		delete m_pAuthServerFarmInfo;

	if (m_pCharacterServerFarmInfo)
		delete m_pCharacterServerFarmInfo;

	for (std::list<sDBO_SERVER_INFO* >::iterator it = m_listMasterServerInfo.begin(); it != m_listMasterServerInfo.end(); it++)
		delete *it;
	m_listMasterServerInfo.clear();

	for (std::list<sDBO_SERVER_INFO* >::iterator it = m_listAuthServerInfo.begin(); it != m_listAuthServerInfo.end(); it++)
		delete *it;
	m_listAuthServerInfo.clear();

	for (std::list<sDBO_SERVER_INFO* >::iterator it = m_listCharacterServerInfo.begin(); it != m_listCharacterServerInfo.end(); it++)
		delete *it;
	m_listCharacterServerInfo.clear();

	for (std::list<sDBO_SERVER_INFO* >::iterator it = m_listGameServerInfo.begin(); it != m_listGameServerInfo.end(); it++)
		delete *it;
	m_listGameServerInfo.clear();

	for (std::list<sDBO_SERVER_INFO* >::iterator it = m_listQueryServerInfo.begin(); it != m_listQueryServerInfo.end(); it++)
		delete *it;
	m_listQueryServerInfo.clear();

	for (std::list<sDBO_SERVER_INFO* >::iterator it = m_listCommunityServerInfo.begin(); it != m_listCommunityServerInfo.end(); it++)
		delete *it;
	m_listCommunityServerInfo.clear();

	for (std::map<SERVERFARMID, sSERVER_FARM_INFO *>::iterator it = m_mapGameServerFarmInfo.begin(); it != m_mapGameServerFarmInfo.end(); it++)
		delete it->second;
	m_mapGameServerFarmInfo.clear();


	return true;
}


sSERVER_FARM_INFO* CNeighborServerInfoManager::GetServerFarmInfo(SERVERFARMID serverFarmId)
{
	return GetServerFarmInfo_Internal(serverFarmId);
}


sSERVER_CHANNEL_INFO* CNeighborServerInfoManager::GetServerChannelInfo(SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex)
{
	sSERVER_FARM_INFO *pServerFarmInfo = GetServerFarmInfo_Internal(serverFarmId);
	if (pServerFarmInfo)
		return GetServerChannelInfo_Internal(pServerFarmInfo, byServerChannelIndex);

	return NULL;
}


sDBO_SERVER_INFO* CNeighborServerInfoManager::GetServerInfo(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex)
{
	std::list<sDBO_SERVER_INFO *>* pServerInfoList = GetServerInfoList(serverFarmId, byServerChannelIndex, byServerType);
	if (pServerInfoList)
		return GetServerInfoHelper(pServerInfoList, byServerIndex);

	return NULL;
}


sDBO_SERVER_INFO* CNeighborServerInfoManager::GetServerInfo(BYTE byServerType, WCHAR *pwszServerIP)
{
	static char* pszServerIP;
	::WideCharToMultiByte(::GetACP(), 0, pwszServerIP, -1, pszServerIP, NTL_MAX_LENGTH_OF_IP, NULL, NULL);
	return GetServerInfo(byServerType, pszServerIP);
}


sDBO_SERVER_INFO* CNeighborServerInfoManager::GetServerInfo(BYTE byServerType, char *pszServerIP)
{
	switch (byServerType)
	{
		case NTL_SERVER_TYPE_MASTER:
		{
			return GetServerInfoHelper(&m_listMasterServerInfo, pszServerIP);
		}
		break;
		case NTL_SERVER_TYPE_AUTH:
		{
			return GetServerInfoHelper(&m_listAuthServerInfo, pszServerIP);
		}
		break;
		case NTL_SERVER_TYPE_CHARACTER:
		{
			return GetServerInfoHelper(&m_listCharacterServerInfo, pszServerIP);
		}
		break;

		case NTL_SERVER_TYPE_GAME:
		case NTL_SERVER_TYPE_NPC:
		case NTL_SERVER_TYPE_QUERY:
		case NTL_SERVER_TYPE_COMMUNITY:
		{
			for (std::map<SERVERFARMID, sSERVER_FARM_INFO *>::iterator it = m_mapGameServerFarmInfo.begin(); it != m_mapGameServerFarmInfo.end(); it++)
			{
				sSERVER_FARM_INFO* pGameFarm = it->second;

				for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it2 = pGameFarm->mapGameServerChannelInfo.begin(); it2 != pGameFarm->mapGameServerChannelInfo.end(); it2++)
				{
					sSERVER_CHANNEL_INFO* pServerChannelInfo = it2->second;

					switch (byServerType)
					{
						case NTL_SERVER_TYPE_GAME:
						{
							return GetServerInfoHelper(&pServerChannelInfo->listGameServerInfoRef, pszServerIP);
						}
						break;
						case NTL_SERVER_TYPE_NPC:
						{
							return GetServerInfoHelper(&pServerChannelInfo->listNpcServerInfoRef, pszServerIP);
						}
						break;
						case NTL_SERVER_TYPE_QUERY:
						{
							return GetServerInfoHelper(&pGameFarm->listQueryServerInfoRef, pszServerIP);
						}
						break;
						case NTL_SERVER_TYPE_COMMUNITY:
						{
							return GetServerInfoHelper(&pGameFarm->listCommunityServerInfoRef, pszServerIP);
						}
						break;

						default: printf("line 476: byServerType invalid %u \n", byServerType); break;
					}
				}
			}
		}
		break;

		/*case NTL_SERVER_TYPE_LOG:
		{
			return GetServerInfoHelper(&m_listLogServerInfo, pszServerIP);
		}
		break;
		case NTL_SERVER_TYPE_OPERATING:
		{
			return GetServerInfoHelper(&m_listOperatingServerInfo, pszServerIP);
		}
		break;
		case NTL_SERVER_TYPE_REPORT:
		{
			return GetServerInfoHelper(&m_listReportServerInfo, pszServerIP);
		}
		break;*/

		default: printf("line 499: byServerType invalid %u \n", byServerType); break;
	}

	return NULL;
}


sDBO_SERVER_INFO* CNeighborServerInfoManager::GetIdlestServerInfo(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex)
{
	std::list<sDBO_SERVER_INFO *>* pServerInfoList = GetServerInfoList(serverFarmId, byServerChannelIndex, byServerType);
	if (pServerInfoList)
	{
		sDBO_SERVER_INFO* pReturnInfo = NULL;

		for (std::list<sDBO_SERVER_INFO *>::iterator it = pServerInfoList->begin(); it != pServerInfoList->end(); it++)
		{
			sDBO_SERVER_INFO* pInfo = *it;

			if (pInfo->bIsOn && pInfo->byRunningState == DBO_SERVER_RUNNING_STATE_RUNNING)
			{
				if (pReturnInfo)
				{
					if (pReturnInfo->dwLoad > pInfo->dwLoad)
						pReturnInfo = pInfo;
				}
				else
				{
					pReturnInfo = pInfo;
				}
			}
		}

		return pReturnInfo;
	}

	return NULL;
}


sDBO_SERVER_INFO* CNeighborServerInfoManager::GetIdlestQueryServerInfo()
{
	sDBO_SERVER_INFO* pReturnInfo = NULL;

	for (std::list<sDBO_SERVER_INFO *>::iterator it = m_listQueryServerInfo.begin(); it != m_listQueryServerInfo.end(); it++)
	{
		sDBO_SERVER_INFO* pInfo = *it;

		if (pInfo->bIsOn && pInfo->byRunningState == DBO_SERVER_RUNNING_STATE_RUNNING)
		{
			if (pReturnInfo)
			{
				if (pReturnInfo->dwLoad > pInfo->dwLoad)
					pReturnInfo = pInfo;
			}
			else
			{
				pReturnInfo = pInfo;
			}
		}
	}

	return pReturnInfo;
}


bool CNeighborServerInfoManager::SetServerOn(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex)
{
	sDBO_SERVER_INFO* pInfo = GetServerInfo(byServerType, serverFarmId, byServerChannelIndex, byServerIndex);
	if (pInfo)
	{
		if (pInfo->bIsOn)
			return true;
		else
		{
			pInfo->bIsOn = true;
			pInfo->byRunningState = DBO_SERVER_RUNNING_STATE_RUNNING;
			pInfo->dwLoad = 0;
			return OnServerTurnedOn(pInfo);
		}
	}

	return false;
}


bool CNeighborServerInfoManager::SetServerOff(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex)
{
	sDBO_SERVER_INFO* pInfo = GetServerInfo(byServerType, serverFarmId, byServerChannelIndex, byServerIndex);
	if (pInfo)
	{
		if (pInfo->bIsOn)
		{
			pInfo->bIsOn = false;
			pInfo->byRunningState = DBO_SERVER_RUNNING_STATE_SHUTDOWN;
			pInfo->dwLoad = 0;

			return OnServerTurnedOff(pInfo);
		}
		else
		{
			return true;
		}
	}

	return false;
}


bool CNeighborServerInfoManager::SetServerRunningState(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex, BYTE byRunningState)
{
	sDBO_SERVER_INFO* pInfo = GetServerInfo(byServerType, serverFarmId, byServerChannelIndex, byServerIndex);
	if (pInfo)
	{
		pInfo->byRunningState = byRunningState;
		return OnServerRunningStateChanged(pInfo);
	}

	return false;
}


bool CNeighborServerInfoManager::UpdateAuthServerFarmStatus(BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad)
{
	if (m_pAuthServerFarmInfo)
	{
		m_pAuthServerFarmInfo->byServerStatus = byServerStatus;
		m_pAuthServerFarmInfo->dwMaxLoad = dwMaxLoad;
		m_pAuthServerFarmInfo->dwLoad = dwLoad;
	}
	return true;
}


bool CNeighborServerInfoManager::UpdateCharacterServerFarmStatus(BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad)
{
	if (m_pCharacterServerFarmInfo)
	{
		m_pCharacterServerFarmInfo->byServerStatus = byServerStatus;
		m_pCharacterServerFarmInfo->dwMaxLoad = dwMaxLoad;
		m_pCharacterServerFarmInfo->dwLoad = dwLoad;
	}
	return true;
}


bool CNeighborServerInfoManager::UpdateGameServerFarmStatus(SERVERFARMID serverFarmId, BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad)
{
	sDBO_GAME_SERVER_FARM_INFO* pInfo = GetServerFarmInfo(serverFarmId);
	if (!pInfo)
		return false;

	pInfo->byServerStatus = byServerStatus;
	pInfo->dwMaxLoad = dwMaxLoad;
	pInfo->dwLoad = dwLoad;

	return true;
}


bool CNeighborServerInfoManager::UpdateGameServerChannelStatus(SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad, bool bIsScrambleChannel, _CHANNEL_BUFF& rsChannelBuff)
{
	sDBO_GAME_SERVER_CHANNEL_INFO* pInfo = GetServerChannelInfo(serverFarmId, byServerChannelIndex);
	if (!pInfo)
		return false;

	pInfo->byServerStatus = byServerStatus;
	pInfo->dwMaxLoad = dwMaxLoad;
	pInfo->dwLoad = dwLoad;
	pInfo->bIsScrambleChannel = bIsScrambleChannel;
	memcpy(&pInfo->sChannelBuff, &rsChannelBuff, sizeof(_CHANNEL_BUFF));

	return true;
}


bool CNeighborServerInfoManager::UpdateServerInfo(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex, sDBO_SERVER_INFO& newServerInfo, bool bIsScrambleChannel, _CHANNEL_BUFF& rChannelBuff)
{
	sDBO_SERVER_INFO* pInfo = GetServerInfo(byServerType, serverFarmId, byServerChannelIndex, byServerIndex);
	if (!pInfo)
		return false;

	DWORD dwCurLoad = pInfo->dwLoad;


	if (byServerType == NTL_SERVER_TYPE_GAME)
	{
		sSERVER_FARM_INFO* pFarm = GetServerFarmInfo_Internal(serverFarmId);
		if (pFarm)
			pFarm->dwLoad = newServerInfo.dwLoad + pFarm->dwLoad - dwCurLoad;
		else
			printf("Line 694: cant find game server farm info. serverfarmID=%u, serverchannelID=%u, serverindex=%u \n", serverFarmId, byServerChannelIndex, byServerIndex);

		sSERVER_CHANNEL_INFO* pChannel = GetServerChannelInfo_Internal(pFarm, byServerChannelIndex);
		if (pChannel)
		{
			pChannel->dwLoad = newServerInfo.dwLoad + pChannel->dwLoad - dwCurLoad;
			pChannel->bIsScrambleChannel = bIsScrambleChannel;
			memcpy(&pChannel->sChannelBuff, &rChannelBuff, sizeof(_CHANNEL_BUFF));
		}
		else
			printf("Line 704: cant find game server channel info. serverfarmID=%u, serverchannelID=%u, serverindex=%u \n", serverFarmId, byServerChannelIndex, byServerIndex);
	}

	pInfo->dwMaxLoad = newServerInfo.dwMaxLoad;
	pInfo->dwLoad = newServerInfo.dwLoad;
	pInfo->dwProcessUsage = newServerInfo.dwProcessUsage;
	pInfo->dwSystemUsage = newServerInfo.dwSystemUsage;
	pInfo->ui64MemoryUsage = newServerInfo.ui64MemoryUsage;
	pInfo->dwRecvQuery = newServerInfo.dwRecvQuery;
	pInfo->dwEndQuery = newServerInfo.dwEndQuery;
	pInfo->dwCurQuery = newServerInfo.dwCurQuery;
	pInfo->dwPing = newServerInfo.dwPing;
	pInfo->lastUpdatedTime = time(NULL);
	OnServerLoadChanged(pInfo);

	return true;
}


bool CNeighborServerInfoManager::UpdateConnectedUserInfo(SERVERFARMID serverFarmId, SERVERCHANNELID serverChannelId, sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO& rConnectedUserInfo)
{
	UNREFERENCED_PARAMETER(rConnectedUserInfo);
	sSERVER_FARM_INFO* pServerFarmInfo = GetServerFarmInfo_Internal(serverFarmId);
	if (!pServerFarmInfo)
		return false;

	sSERVER_CHANNEL_INFO* pChannel = GetServerChannelInfo_Internal(pServerFarmInfo, serverChannelId);
	if (!pChannel)
		return false;

	//memcpy(&pChannel->connectedUserInfo, &rConnectedUserInfo, sizeof(sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO));
	return true;
}


bool CNeighborServerInfoManager::GetLoadExcludingSpecialServerChannel(SERVERFARMID serverFarmId, DWORD & rdwMaxLoad, DWORD & rdwLoad)
{
	rdwMaxLoad = 0;
	rdwLoad = 0;

	sSERVER_FARM_INFO* pFarm = GetServerFarmInfo_Internal(serverFarmId);
	if (!pFarm)
		return false;

	for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it = pFarm->mapGameServerChannelInfo.begin(); it != pFarm->mapGameServerChannelInfo.end(); it++)
	{
		sSERVER_CHANNEL_INFO* pChannel = it->second;
		if (pChannel)
		{
			DWORD dwTempMaxLoad = 0;
			DWORD dwTempLoad = 0;

			if (GetLoadExcludingSpecialServer(serverFarmId, pChannel->byServerChannelIndex, dwTempLoad, dwTempMaxLoad))
			{
				rdwMaxLoad += dwTempMaxLoad;
				rdwLoad += dwTempLoad;
			}
		}
		else
			printf("Line 788 pChannel is NULL \n");
	}

	return true;
}


bool CNeighborServerInfoManager::GetLoadExcludingSpecialServer(SERVERFARMID serverFarmId, SERVERCHANNELID serverChannelId, DWORD & rdwMaxLoad, DWORD & rdwLoad)
{
	rdwMaxLoad = 0;
	rdwLoad = 0;

	sSERVER_FARM_INFO* pFarm = GetServerFarmInfo_Internal(serverFarmId);
	if (!pFarm)
		return false;

	sSERVER_CHANNEL_INFO* pChannel = GetServerChannelInfo_Internal(pFarm, serverChannelId);
	if (!pChannel)
		return false;

	for (std::list<sDBO_SERVER_INFO*>::iterator it = pChannel->listGameServerInfoRef.begin(); it != pChannel->listGameServerInfoRef.end(); it++)
	{
		sDBO_SERVER_INFO* pInfo = *it;
		if (pInfo)
		{
			if (pInfo->byServerType == NTL_SERVER_TYPE_GAME && !pInfo->byGameServerRole)
			{
				rdwMaxLoad += pInfo->dwMaxLoad;
				rdwLoad += pInfo->dwLoad;
			}
		}
		else printf("Line 794: pInfo == NULL \n");
	}

	return true;
}


std::map<SERVERFARMID, sSERVER_CHANNEL_INFO  *>* CNeighborServerInfoManager::GetRawServerChannelList(SERVERFARMID serverFarmId)
{
	sSERVER_FARM_INFO* pInfo = GetServerFarmInfo_Internal(serverFarmId);
	if (pInfo)
		return &pInfo->mapGameServerChannelInfo;

	return NULL;
}


std::list<sDBO_SERVER_INFO* >* CNeighborServerInfoManager::GetRawServerList(BYTE byServerType)
{
	switch (byServerType)
	{
		case NTL_SERVER_TYPE_MASTER:
		{
			return &m_listMasterServerInfo;
		}
		break;
		case NTL_SERVER_TYPE_AUTH:
		{
			return &m_listAuthServerInfo;
		}
		break;
		case NTL_SERVER_TYPE_CHARACTER:
		{
			return &m_listCharacterServerInfo;
		}
		break;
		case NTL_SERVER_TYPE_GAME:
		{
			return &m_listGameServerInfo;
		}
		break;
		/*case NTL_SERVER_TYPE_NPC:
		{
			return &m_listNpcServerInfo;
		}
		break;*/
		case NTL_SERVER_TYPE_QUERY:
		{
			return &m_listQueryServerInfo;
		}
		break;
		case NTL_SERVER_TYPE_COMMUNITY:
		{
			return &m_listCommunityServerInfo;
		}
		break;
		//case NTL_SERVER_TYPE_LOG:
		//{
		//	return &m_listLogServerInfo;
		//}
		//break;
		//case NTL_SERVER_TYPE_OPERATING:
		//{
		//	return &m_listOperatingServerInfo;
		//}
		//break;
		//case NTL_SERVER_TYPE_REPORT:
		//{
		//	return &m_listReportServerInfo;
		//}
		//break;

		default: printf("Line 866: unknown server type %u \n", byServerType); break;
	}

	return NULL;
}


bool CNeighborServerInfoManager::OnServerTurnedOn(sDBO_SERVER_INFO *pServerInfo)
{
	UNREFERENCED_PARAMETER(pServerInfo);
	return true;
}


bool CNeighborServerInfoManager::OnServerTurnedOff(sDBO_SERVER_INFO *pServerInfo)
{
	UNREFERENCED_PARAMETER(pServerInfo);
	return true;
}


bool CNeighborServerInfoManager::OnServerRunningStateChanged(sDBO_SERVER_INFO *pServerInfo)
{
	UNREFERENCED_PARAMETER(pServerInfo);
	return true;
}


bool CNeighborServerInfoManager::OnServerLoadChanged(sDBO_SERVER_INFO *pServerInfo)
{
	UNREFERENCED_PARAMETER(pServerInfo);
	return true;
}


sSERVER_FARM_INFO* CNeighborServerInfoManager::GetServerFarmInfo_Internal(SERVERFARMID serverFarmId)
{
	std::map<SERVERFARMID, sSERVER_FARM_INFO *>::iterator it = m_mapGameServerFarmInfo.find(serverFarmId);
	if (it == m_mapGameServerFarmInfo.end())
		return NULL;

	return it->second;
}


sSERVER_CHANNEL_INFO* CNeighborServerInfoManager::GetServerChannelInfo_Internal(sSERVER_FARM_INFO *pServerFarmInfo, SERVERCHANNELID byServerChannelIndex)
{
	if (!pServerFarmInfo)
	{
		printf("Line 911: pServerFarmInfo == NULL \n");
		return NULL;
	}

	std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it = pServerFarmInfo->mapGameServerChannelInfo.find(byServerChannelIndex);
	if (it == pServerFarmInfo->mapGameServerChannelInfo.end())
		return NULL;
	
	return it->second;
}


std::list<sDBO_SERVER_INFO* >* CNeighborServerInfoManager::GetServerInfoList(SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, BYTE byServerType)
{
	switch (byServerType)
	{
	case NTL_SERVER_TYPE_MASTER:
	{
		return &m_listMasterServerInfo;
	}
	break;
	case NTL_SERVER_TYPE_AUTH:
	{
		return &m_listAuthServerInfo;
	}
	break;
	case NTL_SERVER_TYPE_CHARACTER:
	{
		return &m_listCharacterServerInfo;
	}
	break;
	case NTL_SERVER_TYPE_GAME:
//	case NTL_SERVER_TYPE_NPC:
	{
		sSERVER_FARM_INFO* pServerFarmInfo = GetServerFarmInfo_Internal(serverFarmId);
		if (pServerFarmInfo)
		{
			sSERVER_CHANNEL_INFO* pChannel = GetServerChannelInfo_Internal(pServerFarmInfo, byServerChannelIndex);
			if (pChannel)
			{
				if (byServerType == NTL_SERVER_TYPE_GAME)
					return &pChannel->listGameServerInfoRef;
			//	else if (byServerType == NTL_SERVER_TYPE_NPC)
			//		return &pChannel->listNpcServerInfoRef;
			}
		}
	}
	break;
	case NTL_SERVER_TYPE_QUERY:
	case NTL_SERVER_TYPE_COMMUNITY:
	{
		sSERVER_FARM_INFO* pServerFarmInfo = GetServerFarmInfo_Internal(serverFarmId);
		if (pServerFarmInfo)
		{
			if (byServerType == NTL_SERVER_TYPE_QUERY)
				return &pServerFarmInfo->listQueryServerInfoRef;
			else if (byServerType == NTL_SERVER_TYPE_COMMUNITY)
				return &pServerFarmInfo->listCommunityServerInfoRef;
		}
	}
	break;
	//case NTL_SERVER_TYPE_LOG:
	//{
	//	return &m_listLogServerInfo;
	//}
	//break;
	//case NTL_SERVER_TYPE_OPERATING:
	//{
	//	return &m_listOperatingServerInfo;
	//}
	//break;
	//case NTL_SERVER_TYPE_REPORT:
	//{
	//	return &m_listReportServerInfo;
	//}
	//break;

	default: printf("Line 988: unknown server type %u \n", byServerType); break;
	}

	return NULL;
}


sDBO_SERVER_INFO* CNeighborServerInfoManager::GetServerInfoHelper(std::list<sDBO_SERVER_INFO* >* pServerInfoList, SERVERINDEX byServerIndex)
{
	if (pServerInfoList)
	{
		for (std::list<sDBO_SERVER_INFO*>::iterator it = pServerInfoList->begin(); it != pServerInfoList->end(); it++)
		{
			sDBO_SERVER_INFO* pInfo = *it;
			if (pInfo)
			{
				if (pInfo->byServerIndex == byServerIndex)
					return pInfo;
			}
		}
	}

	return NULL;
}


sDBO_SERVER_INFO* CNeighborServerInfoManager::GetServerInfoHelper(std::list<sDBO_SERVER_INFO* >* pServerInfoList, char* pszServerIP)
{
	if (pServerInfoList && pszServerIP)
	{
		for (std::list<sDBO_SERVER_INFO*>::iterator it = pServerInfoList->begin(); it != pServerInfoList->end(); it++)
		{
			sDBO_SERVER_INFO* pInfo = *it;
			if (pInfo)
			{
				if (!strcmp(pInfo->achPublicAddress, pszServerIP))
					return pInfo;
			}
		}
	}

	return NULL;
}


bool CNeighborServerInfoManager::GenerateAuthKey(void* pvAuthKey, ACCOUNTID accid)
{
	std::map<ACCOUNTID, sAUTHINFO*>::iterator it = m_mapAuthKeys.find(accid);
	if (it != m_mapAuthKeys.end())
	{
		SAFE_DELETE(it->second);
		m_mapAuthKeys.erase(it);
	}

	memset(pvAuthKey, NULL, sizeof(pvAuthKey));
	memcpy(pvAuthKey, AUTHKEY_KEY, NTL_MAX_SIZE_AUTH_KEY);

	if (CreateAuthKey(pvAuthKey, NTL_MAX_SIZE_AUTH_KEY) == false)
		return false;

	sAUTHINFO* auth = new sAUTHINFO;
	auth->dwTimeRegistered = GetTickCount();
	memcpy(auth->byAuthKey, pvAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	m_mapAuthKeys.insert(std::make_pair(accid, auth));

	return true;
}

void CNeighborServerInfoManager::GenerateAuthKey(BYTE* pvAuthKey)
{
	memset(pvAuthKey, NULL, sizeof(pvAuthKey));
	memcpy(pvAuthKey, AUTHKEY_KEY, NTL_MAX_SIZE_AUTH_KEY);

	CreateAuthKey(pvAuthKey, NTL_MAX_SIZE_AUTH_KEY);
}

void CNeighborServerInfoManager::StoreAuthKey(BYTE * pvAuthKey, ACCOUNTID accid)
{
	std::map<ACCOUNTID, sAUTHINFO*>::iterator it = m_mapAuthKeys.find(accid);
	if (it == m_mapAuthKeys.end())
	{
		sAUTHINFO* auth = new sAUTHINFO;
		auth->dwTimeRegistered = GetTickCount();
		memcpy(auth->byAuthKey, pvAuthKey, NTL_MAX_SIZE_AUTH_KEY);
		m_mapAuthKeys.insert(std::make_pair(accid, auth));
	}
	else
	{
		ERR_LOG(LOG_GENERAL,"Account: %u auth key already exist.. Updating current one.. New:First:%u, Second:%u, Old:First:%u, Second:%u ", accid, pvAuthKey[0], pvAuthKey[1], it->second->byAuthKey[0], it->second->byAuthKey[1]);
		it->second->dwTimeRegistered = GetTickCount();
		memcpy(it->second->byAuthKey, pvAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	}
}

bool CNeighborServerInfoManager::CheckAuthKey(BYTE* pvAuthKey, ACCOUNTID accid)
{
	bool bFlag = false;

	std::map<ACCOUNTID, sAUTHINFO*>::iterator it = m_mapAuthKeys.find(accid);
	if (it != m_mapAuthKeys.end())
	{
		sAUTHINFO* auth = it->second;

		//check time
		if (GetTickCount() - auth->dwTimeRegistered > AUTH_KEY_EXPIRE_SEC)
		{
			ERR_LOG(LOG_GENERAL, "Account: %u auth key expired difference %u, max allowed %u", accid, GetTickCount() - auth->dwTimeRegistered, AUTH_KEY_EXPIRE_SEC);
		}
		else
		{
			if (std::equal(auth->byAuthKey, auth->byAuthKey + NTL_MAX_SIZE_AUTH_KEY, pvAuthKey))
			{
				bFlag = true;
			}
			else
			{
				ERR_LOG(LOG_USER, "Account: %u comparing failed %d %d - %d %d", accid, auth->byAuthKey[0], auth->byAuthKey[1], pvAuthKey[0], pvAuthKey[1]);
			}
		}

		SAFE_DELETE(auth);
		m_mapAuthKeys.erase(it);
	}
	else
	{
		ERR_LOG(LOG_USER, "Account: %u no auth key found", accid);
	}

	return bFlag;
}