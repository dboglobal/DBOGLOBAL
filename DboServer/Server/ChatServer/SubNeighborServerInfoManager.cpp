#include "Stdafx.h"
#include "ChatServer.h"
#include "NtlPacketTM.h"


CSubNeighborServerInfoManager::CSubNeighborServerInfoManager()
{
	m_pEventHolder = new EventableObjectHolder(-1);
}

CSubNeighborServerInfoManager::~CSubNeighborServerInfoManager()
{
}



void CSubNeighborServerInfoManager::TickProcess(DWORD dwTickDiff)
{
	m_pEventHolder->Update(dwTickDiff);
}


void CSubNeighborServerInfoManager::StartEvents()
{
	g_pEventMgr->AddEvent(this, &CSubNeighborServerInfoManager::OnEvent_ServerAliveUpdate, SERVER_EVENT_KEEP_ALIVE, 4000, -1, 0);
}

void CSubNeighborServerInfoManager::OnEvent_ServerAliveUpdate()
{
	CChatServer* app = (CChatServer*)g_pApp;

	CNtlPacket packet(sizeof(sTM_PING_RES));
	sTM_PING_RES * res = (sTM_PING_RES *)packet.GetPacketData();
	res->wOpCode = TM_PING_RES;
	res->dwTick = GetTickCount();
	packet.SetPacketLen(sizeof(sTM_PING_RES));
	app->SendTo(app->GetMasterServerSession(), &packet);
}


//--------------------------------------------------------------------------------------//
//		ADD GAME SERVER CHANNEL
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::AddGameServerChannel(SERVERCHANNELID channelid, HSESSION sessionId)
{
	mapGameServerChannelSession[channelid] = sessionId;
}

//--------------------------------------------------------------------------------------//
//		GET GAME SERVER CHANNEL INFO
//--------------------------------------------------------------------------------------//
HSESSION CSubNeighborServerInfoManager::GetGsSession(SERVERCHANNELID channelid)
{
	auto it = mapGameServerChannelSession.find(channelid);
	if (it != mapGameServerChannelSession.end()) 
	{
		return it->second;
	}

	return INVALID_HSESSION;
}

//--------------------------------------------------------------------------------------//
//		SEND PACKET TO ALL CHANNELS
//--------------------------------------------------------------------------------------//
void CSubNeighborServerInfoManager::Broadcast(CNtlPacket* pPacket, SERVERCHANNELID exception/* = INVALID_SERVERCHANNELID*/)
{
	auto it =	mapGameServerChannelSession.begin();

	for (; it != mapGameServerChannelSession.end(); it++)
	{
		if (it->first != exception)
			g_pApp->Send(it->second, pPacket);
	}
}