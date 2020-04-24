#include "stdafx.h"
#include "MasterServer.h"
#include "PacketHead.h"


//--------------------------------------------------------------------------------------//
//		NFY THAT GAMESERVER SERVER STARTED
//--------------------------------------------------------------------------------------//
void CGameServerPassiveSession::Gm_NfyServerBegin(CNtlPacket * pPacket, CMasterServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGM_NOTIFY_SERVER_BEGIN * req = (sGM_NOTIFY_SERVER_BEGIN*)pPacket->GetPacketData();

	serverFarmID = req->serverInfo.serverFarmId;
	serverChannelID = req->serverInfo.byServerChannelIndex;

	g_pSrvMgr->AddGameServerSession(_SERVER_PAIR_ID(serverFarmID, serverChannelID), this);

	g_pSrvMgr->RefreshServerFarmInfo(&req->gameServerFarmInfo);
	g_pSrvMgr->RefreshServerChannelInfo(&req->gameServerChannelInfo);

	g_pSrvMgr->RefreshServerInfo(&req->serverInfo);

	g_pSrvMgr->OnServerTurnedOn(&req->serverInfo);
}

//--------------------------------------------------------------------------------------//
//		PLAYER LOGIN INTO GAMESERVER
//--------------------------------------------------------------------------------------//
void CGameServerPassiveSession::Gm_PlayerEnter(CNtlPacket * pPacket, CMasterServer * app)
{
	sGM_LOGIN_REQ * req = (sGM_LOGIN_REQ*)pPacket->GetPacketData();

	WORD resultcode = GAME_SUCCESS;
	//check auth key
	if(g_pSrvMgr->CheckAuthKey(req->abyAuthKey, req->accountId) == true)
	{
	//	printf("GAME SERVER LOGIN : USER %u, SERVER %d CHANNEL %d \n", req->accountId, req->serverId, req->serverChannelId);
		g_pSrvMgr->UpdatePlayerCount(NTL_SERVER_TYPE_GAME, 0, req->serverId, req->serverChannelId, true, 1);

		g_pSrvMgr->AddPlayerGameServer(req->accountId, req->serverId, req->serverChannelId);
	}
	else
	{
		resultcode = GAME_INVALID_AUTH_KEY;
		ERR_LOG(LOG_USER, "USER %u COULD NOT ENTER THE CHANNEL %u BECAUSE CHECK AUTH KEY FAILED", req->accountId, req->serverChannelId);
	}

	CNtlPacket packet(sizeof(sMG_LOGIN_RES));
	sMG_LOGIN_RES * res = (sMG_LOGIN_RES *)packet.GetPacketData();
	res->wOpCode = MG_LOGIN_RES;
	res->accountId = req->accountId;
	res->wResultCode = resultcode;
	memcpy(res->abyAuthKey, req->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	packet.SetPacketLen( sizeof(sMG_LOGIN_RES) );
	app->SendTo(this, &packet);
}

//--------------------------------------------------------------------------------------//
//		PLAYER LOGOUT FROM GAMESERVER
//--------------------------------------------------------------------------------------//
void CGameServerPassiveSession::Gm_PlayerLeave(CNtlPacket * pPacket, CMasterServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGM_LOGOUT_REQ * req = (sGM_LOGOUT_REQ*)pPacket->GetPacketData();

	//printf("GAME SERVER LOGOUT : USER %u SERVER %d CHANNEL %d \n", req->accountId, req->serverId, req->serverChannelId);

	g_pSrvMgr->UpdatePlayerCount(NTL_SERVER_TYPE_GAME, 0, req->serverId, req->serverChannelId, false, 1);

	g_pSrvMgr->DelPlayer(req->accountId, true, req->serverId, req->serverChannelId);
}
//--------------------------------------------------------------------------------------//
//		PLAYER MOVE TO CHAR SERVER
//--------------------------------------------------------------------------------------//
void CGameServerPassiveSession::Gm_PlayerMove(CNtlPacket * pPacket, CMasterServer * app)
{
	sGM_MOVE_REQ * req = (sGM_MOVE_REQ*)pPacket->GetPacketData();

	WORD resultcode = GAME_SUCCESS;

	BYTE abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];

	if (g_pSrvMgr->GenerateAuthKey(abyAuthKey, req->accountId) == false) 
	{
		resultcode = CHARACTER_AUTHKEY_CREATE_FAIL;
		ERR_LOG(LOG_USER,"Account %d failed to generate auth key", req->accountId);
	}

	CNtlPacket packet(sizeof(sMG_MOVE_RES));
	sMG_MOVE_RES * res = (sMG_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = MG_MOVE_RES;
	res->accountId = req->accountId;
	res->wResultCode = resultcode;
	memcpy(res->abyAuthKey, abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	packet.SetPacketLen( sizeof(sMG_MOVE_RES) );
	app->Send(GetHandle(), &packet);
}

//--------------------------------------------------------------------------------------//
//		PLAYER SWITCH CHANNEL
//--------------------------------------------------------------------------------------//
void CGameServerPassiveSession::Gm_PlayerSwitchChannel(CNtlPacket * pPacket, CMasterServer * app)
{
	sGM_PLAYER_SWITCH_CHANNEL_REQ * req = (sGM_PLAYER_SWITCH_CHANNEL_REQ*)pPacket->GetPacketData();

	WORD resultcode = GAME_SUCCESS;

	BYTE abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];

	if (g_pSrvMgr->GenerateAuthKey(abyAuthKey, req->accountId) == false)
	{
		resultcode = CHARACTER_AUTHKEY_CREATE_FAIL;
		ERR_LOG(LOG_USER, "Account %d failed to generate auth key", req->accountId);
	}

	CNtlPacket packet(sizeof(sMG_PLAYER_SWITCH_CHANNEL_RES));
	sMG_PLAYER_SWITCH_CHANNEL_RES * res = (sMG_PLAYER_SWITCH_CHANNEL_RES *)packet.GetPacketData();
	res->wOpCode = MG_PLAYER_SWITCH_CHANNEL_RES;
	res->accountId = req->accountId;
	res->wResultCode = resultcode;

	if (resultcode == GAME_SUCCESS)
	{
		sSERVER_CHANNEL_INFO* pChannel = g_pSrvMgr->GetServerChannelInfo(req->serverId, req->destServerChannelId);
		if (pChannel)
		{
			if (pChannel->byServerStatus == DBO_SERVER_STATUS_UP)
			{
				if (pChannel->dwLoad < pChannel->dwMaxLoad)
				{
					memcpy(&res->serverInfo, pChannel->listGameServerInfoRef.front(), sizeof(sDBO_SERVER_INFO));
					memcpy(res->abyAuthKey, abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
				}
				else res->wResultCode = GAME_CANNOT_CONNECT_TARGET_CHANNEL_FULL;
			}
			else res->wResultCode = GAME_SERVER_LOCKED;
		}
		else res->wResultCode = GAME_WRONG_SERVER_CHANNEL_HAS_BEEN_SPECIFIED;
	}

	packet.SetPacketLen(sizeof(sMG_PLAYER_SWITCH_CHANNEL_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerPassiveSession::RecvCharServerTeleportReq(CNtlPacket * pPacket, CMasterServer * app)
{
	sGM_CHAR_SERVER_TELEPORT_REQ * req = (sGM_CHAR_SERVER_TELEPORT_REQ*)pPacket->GetPacketData();

	WORD resultcode = GAME_SUCCESS;

	BYTE abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];

	if (g_pSrvMgr->GenerateAuthKey(abyAuthKey, req->accountId) == false)
	{
		resultcode = CHARACTER_AUTHKEY_CREATE_FAIL;
		ERR_LOG(LOG_USER, "Account %d failed to generate auth key", req->accountId);
	}

	CNtlPacket packet(sizeof(sMG_CHAR_SERVER_TELEPORT_RES));
	sMG_CHAR_SERVER_TELEPORT_RES * res = (sMG_CHAR_SERVER_TELEPORT_RES *)packet.GetPacketData();
	res->wOpCode = MG_CHAR_SERVER_TELEPORT_RES;
	res->accountId = req->accountId;
	res->wResultCode = resultcode;

	if (resultcode == GAME_SUCCESS)
	{
		sSERVER_CHANNEL_INFO* pChannel = g_pSrvMgr->GetServerChannelInfo(req->serverId, req->destServerChannelId);
		if (pChannel)
		{
			if (pChannel->byServerStatus == DBO_SERVER_STATUS_UP)
			{
				if (pChannel->dwLoad < pChannel->dwMaxLoad)
				{
					res->serverInfo = *pChannel->listGameServerInfoRef.front();
					memcpy(res->abyAuthKey, abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
				}
				else res->wResultCode = GAME_CANNOT_CONNECT_TARGET_CHANNEL_FULL;
			}
			else res->wResultCode = GAME_SERVER_LOCKED;
		}
		else res->wResultCode = GAME_WRONG_SERVER_CHANNEL_HAS_BEEN_SPECIFIED;
	}

	packet.SetPacketLen(sizeof(sMG_CHAR_SERVER_TELEPORT_RES));
	app->Send(GetHandle(), &packet);
}


//--------------------------------------------------------------------------------------//
//		SERVER SHUTDOWN INFO
//--------------------------------------------------------------------------------------//
void CGameServerPassiveSession::Gm_RecvServerShutdownNfy(CNtlPacket * pPacket, CMasterServer * app)
{
	UNREFERENCED_PARAMETER(pPacket);
	UNREFERENCED_PARAMETER(app);

	g_pSrvMgr->SetServerOff(NTL_SERVER_TYPE_GAME, serverFarmID, serverChannelID, serverChannelID);
}

void CGameServerPassiveSession::RecvDragonballScrambleSeasonStateNfy(CNtlPacket * pPacket, CMasterServer * app)
{
	sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * req = (sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY*)pPacket->GetPacketData();

	bool bFlag = (req->wSeasonState == 1) ? true : false;

	g_pSrvMgr->UpdateScrambleChannel(NTL_SERVER_TYPE_GAME, req->serverFarmID, req->serverChannelID, bFlag);
}
