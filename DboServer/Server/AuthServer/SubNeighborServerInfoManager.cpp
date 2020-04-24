#include "Stdafx.h"
#include "AuthServer.h"
#include "NtlPacketAM.h"


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
	g_pEventMgr->AddEvent(this, &CSubNeighborServerInfoManager::OnEvent_ServerAliveUpdate, SERVER_EVENT_KEEP_ALIVE, 4000, INVALID_DWORD, 0);
}

void CSubNeighborServerInfoManager::OnEvent_ServerAliveUpdate()
{
	CAuthServer* app = (CAuthServer*)g_pApp;

	CNtlPacket packet(sizeof(sAM_PING_RES));
	sAM_PING_RES * res = (sAM_PING_RES *)packet.GetPacketData();
	res->wOpCode = AM_PING_RES;
	res->dwTick = GetTickCount();
	packet.SetPacketLen(sizeof(sAM_PING_RES));
	app->SendTo(app->m_pMasterServerSession, &packet);
}