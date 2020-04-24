#include "stdafx.h"
#include "MasterServer.h"
#include "PacketHead.h"

//--------------------------------------------------------------------------------------//
//		NFY THAT AUTH SERVER STARTED
//--------------------------------------------------------------------------------------//
void CAuthServerPassiveSession::Am_NfyServerBegin(CNtlPacket * pPacket, CMasterServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sAM_NOTIFY_SERVER_BEGIN * req = (sAM_NOTIFY_SERVER_BEGIN*)pPacket->GetPacketData();

	serverIndex = req->sServerInfo.byServerIndex;

	g_pSrvMgr->AddAuthServerSession(req->sServerInfo.byServerIndex, this);

	g_pSrvMgr->RefreshServerInfo(&req->sServerInfo);

	g_pSrvMgr->OnServerTurnedOn(&req->sServerInfo);
}

//--------------------------------------------------------------------------------------//
//		CHECK IF PLAYER IS ONLINE
//--------------------------------------------------------------------------------------//
void CAuthServerPassiveSession::Am_CheckPlayerOnline(CNtlPacket * pPacket, CMasterServer * app)
{
	sAM_ON_PLAYER_CHECK_REQ * req = (sAM_ON_PLAYER_CHECK_REQ*)pPacket->GetPacketData();

	bool playeronlinecheck = g_pSrvMgr->IsPlayerOnline(req->accountId);

	CNtlPacket packet(sizeof(sMA_ON_PLAYER_CHECK_RES));
	sMA_ON_PLAYER_CHECK_RES * res = (sMA_ON_PLAYER_CHECK_RES *)packet.GetPacketData();
	res->wOpCode = MA_ON_PLAYER_CHECK_RES;
	res->accountId = req->accountId;

	if(playeronlinecheck == false)
	{
		//check if auth key for account already exist
		if (g_pSrvMgr->GenerateAuthKey(res->abyAuthKey, req->accountId) == true) 
		{
			res->bIsOnline = false;
		} 
		else 
		{
			ERR_LOG(LOG_USER,"Account %d offline but failed to generate auth key", req->accountId);
			//log: player offline but auth already exist
			res->bIsOnline = true;
		}
	}
	else res->bIsOnline = true;


	NTL_SAFE_WCSCPY(res->awchUserId, req->awchUserId);
	res->bIsGM = req->bIsGM;
	res->dwAllowedFunctionForDeveloper = req->dwAllowedFunctionForDeveloper;
	res->lastServerFarmId = req->lastServerFarmId;
	packet.SetPacketLen( sizeof(sMA_ON_PLAYER_CHECK_RES) );
	app->Send( GetHandle(), &packet );
}