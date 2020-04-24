#include "stdafx.h"
#include "QueryServer.h"


CServerManager::CServerManager()
{
}

CServerManager::~CServerManager()
{
}

void CServerManager::AddGameServer(SERVERCHANNELID byChannel, HSESSION sessionId)
{
	mapGameServerChannelSession.insert({ byChannel , sessionId });
}

void CServerManager::RemoveGameServer(SERVERCHANNELID byChannel)
{
	mapGameServerChannelSession.erase(byChannel);
}

HSESSION CServerManager::GetGsSession(SERVERCHANNELID channelid)
{
	std::map<SERVERCHANNELID, HSESSION>::iterator it = mapGameServerChannelSession.find(channelid);
	if (it != mapGameServerChannelSession.end())
	{
		return it->second;
	}

	return INVALID_HSESSION;
}

void CServerManager::Broadcast(CNtlPacket* pPacket, SERVERCHANNELID exception/* = INVALID_SERVERCHANNELID*/)
{
	std::map<SERVERCHANNELID, HSESSION>::iterator it = mapGameServerChannelSession.begin();

	for (; it != mapGameServerChannelSession.end(); it++)
	{
		if (it->first != exception)
			g_pApp->Send(it->second, pPacket);
	}
}
