#include "stdafx.h"
#include "MasterServer.h"
#include "PacketHead.h"

//--------------------------------------------------------------------------------------//
//		NFY THAT CHAT SERVER STARTED
//--------------------------------------------------------------------------------------//
void CChatServerPassiveSession::Tm_NfyServerBegin(CNtlPacket * pPacket, CMasterServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sTM_NOTIFY_SERVER_BEGIN * req = (sTM_NOTIFY_SERVER_BEGIN*)pPacket->GetPacketData();


	g_pSrvMgr->AddChatServerSession(req->sServerInfo.byServerIndex, this);

	g_pSrvMgr->RefreshServerInfo(&req->sServerInfo);

	g_pSrvMgr->OnServerTurnedOn(&req->sServerInfo);
}