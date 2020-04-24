#include "stdafx.h"
#include "MasterServer.h"
#include "PacketHead.h"


//--------------------------------------------------------------------------------------//
//		NFY THAT CHAR SERVER STARTED
//--------------------------------------------------------------------------------------//
void CCharServerPassiveSession::Cm_NfyServerBegin(CNtlPacket * pPacket, CMasterServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sCM_NOTIFY_SERVER_BEGIN * req = (sCM_NOTIFY_SERVER_BEGIN*)pPacket->GetPacketData();

	serverIndex = req->sServerInfo.byServerIndex;

	g_pSrvMgr->AddCharServerSession(req->sServerInfo.byServerIndex, this);

	g_pSrvMgr->RefreshServerInfo(&req->sServerInfo);

	g_pSrvMgr->OnServerTurnedOn(&req->sServerInfo);
}

//--------------------------------------------------------------------------------------//
//		USER ENTER CHAR SERVER
//--------------------------------------------------------------------------------------//
void CCharServerPassiveSession::Cm_UserLogin(CNtlPacket * pPacket, CMasterServer * app)
{
	sCM_LOGIN_REQ * req = (sCM_LOGIN_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHARACTER_SUCCESS;
	//check auth key
	if(g_pSrvMgr->CheckAuthKey(req->abyAuthKey, req->accountId) == true)
	{
		g_pSrvMgr->UpdatePlayerCount(NTL_SERVER_TYPE_CHARACTER, serverIndex, 0, 0, true, 1);
		g_pSrvMgr->AddPlayerCharServer(req->accountId, serverIndex);
	}
	else
		resultcode = CHARACTER_AUTHKEY_FAIL;

	CNtlPacket packet(sizeof(sMC_LOGIN_RES));
	sMC_LOGIN_RES * res = (sMC_LOGIN_RES *)packet.GetPacketData();
	res->wOpCode = MC_LOGIN_RES;
	res->accountId = req->accountId;
	res->wResultCode = resultcode;
	packet.SetPacketLen( sizeof(sMC_LOGIN_RES) );
	app->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		USER LEAVE CHAR SERVER
//--------------------------------------------------------------------------------------//
void CCharServerPassiveSession::Cm_UserLeave(CNtlPacket * pPacket, CMasterServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sCM_LOGOUT_REQ * req = (sCM_LOGOUT_REQ*)pPacket->GetPacketData();

	g_pSrvMgr->UpdatePlayerCount(NTL_SERVER_TYPE_CHARACTER, serverIndex, 0, 0, false, 1);
	g_pSrvMgr->DelPlayer(req->accountId, false);
}
//--------------------------------------------------------------------------------------//
//		CHAR EXIT
//--------------------------------------------------------------------------------------//
void CCharServerPassiveSession::Cm_CharExit(CNtlPacket * pPacket, CMasterServer * app)
{
	sCM_CHARACTER_EXIT_REQ * req = (sCM_CHARACTER_EXIT_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sMC_CHARACTER_EXIT_RES));
	sMC_CHARACTER_EXIT_RES * res = (sMC_CHARACTER_EXIT_RES *)packet.GetPacketData();
	res->wOpCode = MC_CHARACTER_EXIT_RES;
	res->accountId = req->accountId;
	res->byMoveType = req->byMoveType;
	res->wResultCode = CHARACTER_SUCCESS;
	packet.SetPacketLen( sizeof(sMC_CHARACTER_EXIT_RES) );
	app->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		USER MOVE
//--------------------------------------------------------------------------------------//
void CCharServerPassiveSession::Cm_UserMove(CNtlPacket * pPacket, CMasterServer * app)
{
	sCM_MOVE_REQ * req = (sCM_MOVE_REQ*)pPacket->GetPacketData();
	WORD resultcode = CHARACTER_SUCCESS;
	
	if (g_pSrvMgr->GenerateAuthKey(req->abyAuthKey, req->accountId) == false) 
	{
		resultcode = CHARACTER_AUTHKEY_CREATE_FAIL;
		ERR_LOG(LOG_USER,"Account %d failed to generate auth key", req->accountId);
	}

	CNtlPacket packet(sizeof(sMC_MOVE_RES));
	sMC_MOVE_RES * res = (sMC_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = MC_MOVE_RES;
	res->accountId = req->accountId;
	memcpy(res->abyAuthKey, req->abyAuthKey, sizeof(res->abyAuthKey));
	res->wResultCode = resultcode;
	packet.SetPacketLen( sizeof(sMC_MOVE_RES) );
	app->Send(GetHandle(), &packet);
}