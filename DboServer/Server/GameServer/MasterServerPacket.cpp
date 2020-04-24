#include "stdafx.h"
#include "SubNeighborServerInfoManager.h"
#include "GameServer.h"
#include "CPlayer.h"
#include "ObjectManager.h"
#include "NtlPacketMG.h"
#include "NtlPacketGM.h"
#include "NtlPacketGU.h"
#include "NtlPacketGT.h"
#include "NtlResultCode.h"
#include "GameMain.h"
#include "SummonPet.h"
#include "freebattle.h"



void CMasterServerSession::OnInvalid(CNtlPacket * pPacket)
{
}


void CMasterServerSession::RecvPingRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sMG_PING_REQ * req = (sMG_PING_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sGM_PING_RES));
	sGM_PING_RES * res = (sGM_PING_RES *)packet.GetPacketData();
	res->wOpCode = GM_PING_RES;
	res->dwTick = req->dwTick;
	res->serverChannelId = app->m_config.byChannel;
	res->serverId = app->m_config.byServerID;
	packet.SetPacketLen(sizeof(sGM_PING_RES));
	app->Send(GetHandle(), &packet);
}


//--------------------------------------------------------------------------------------//
//		RECEIVE SERVER DATA FROM MASTER SERVER
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvServerInfoAdd(CNtlPacket * pPacket)
{
	sMG_SERVERS_INFO_ADD * req = (sMG_SERVERS_INFO_ADD*)pPacket->GetPacketData();

	g_pServerInfoManager->RefreshServerInfo(&req->serverInfo);
}


//--------------------------------------------------------------------------------------//
//		RECEIVE GAME SERVER CHANNEL INFO
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvGameServerChannelInfo(CNtlPacket * pPacket)
{
	sMG_GAME_SERVER_CHANNEL_INFO * req = (sMG_GAME_SERVER_CHANNEL_INFO*)pPacket->GetPacketData();

	g_pServerInfoManager->RefreshServerChannelInfo(&req->gameServerChannelInfo);

	if (req->gameServerChannelInfo.bIsVisible == true)
	{
		CNtlPacket packet(sizeof(sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY));
		sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY * res = (sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY *)packet.GetPacketData();
		res->wOpCode = GU_SERVER_CHANNEL_INFO_REFRESHED_NFY;
		memmove(&res->serverChannelInfo, &req->gameServerChannelInfo, sizeof(sDBO_GAME_SERVER_CHANNEL_INFO));
		packet.SetPacketLen(sizeof(sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY));
		g_pObjectManager->SendPacketToAll(&packet);
	}
}


//--------------------------------------------------------------------------------------//
//		UPDATE CHAR SERVER
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvCharServerUpdate(CNtlPacket * pPacket)
{
	sMG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY * req = (sMG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY*)pPacket->GetPacketData();

	sDBO_SERVER_INFO* pCharServer = g_pServerInfoManager->GetServerInfo(NTL_SERVER_TYPE_CHARACTER, 0, 0, req->serverIndex);
	if (pCharServer)
	{
		pCharServer->byRunningState = req->byRunningState;
		pCharServer->dwLoad = req->dwLoad;
		pCharServer->dwMaxLoad = req->dwMaxLoad;
	}
}

//--------------------------------------------------------------------------------------//
//		UPDATE GAME SERVER
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvGameServerUpdate(CNtlPacket * pPacket)
{
	sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY * req = (sMG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY*)pPacket->GetPacketData();
	
	sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(req->serverFarmId, req->byServerChannelIndex);
	if (pChannel)
	{
		pChannel->byServerStatus = req->byServerStatus;
		pChannel->dwLoad = req->dwLoad;
		pChannel->dwMaxLoad = req->dwMaxLoad;
		pChannel->bIsScrambleChannel = req->bIsScrambleChannel;

		if (req->byServerChannelIndex != DOJO_CHANNEL_INDEX)
		{
			CNtlPacket packet(sizeof(sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY));
			sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY * res = (sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_SERVER_CHANNEL_INFO_REFRESHED_NFY;
			res->serverChannelInfo.serverFarmId = pChannel->serverFarmId;
			res->serverChannelInfo.byServerChannelIndex = pChannel->byServerChannelIndex;
			res->serverChannelInfo.bIsVisible = pChannel->bIsVisible;
			res->serverChannelInfo.byServerStatus = pChannel->byServerStatus;
			res->serverChannelInfo.dwMaxLoad = 100;
			res->serverChannelInfo.dwLoad = DWORD((float)req->dwLoad / (float)req->dwMaxLoad * 100.f);
			res->serverChannelInfo.bIsScrambleChannel = pChannel->bIsScrambleChannel;
			memcpy(&res->serverChannelInfo.sChannelBuff, &pChannel->sChannelBuff, sizeof(_CHANNEL_BUFF));
			res->serverChannelInfo.bUnknown = pChannel->bUnknown;
			packet.SetPacketLen(sizeof(sGU_SERVER_CHANNEL_INFO_REFRESHED_NFY));
			g_pObjectManager->SendPacketToAll(&packet);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		USER LOGIN RESULT
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvUserLogin(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sMG_LOGIN_RES * req = (sMG_LOGIN_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByAccount(req->accountId);
	if(player && player->GetClientSession())
	{
		if(req->wResultCode == GAME_SUCCESS)
		{
			CNtlPacket packet(sizeof(sGU_GAME_ENTER_RES));
			sGU_GAME_ENTER_RES * res = (sGU_GAME_ENTER_RES *)packet.GetPacketData();
			res->wOpCode = GU_GAME_ENTER_RES;
			res->wResultCode = GAME_SUCCESS;
			strcpy_s(res->achCommunityServerIP, NTL_MAX_LENGTH_OF_IP + 1, app->GetConfigChatServerIP().c_str() );
			res->wCommunityServerPort = app->GetConfigChatServerPort();
			res->timeDBOEnter = time(NULL);
			if (app->Send(player->GetClientSessionID(), &packet) == NTL_SUCCESS)
			{
				player->SendLoadPcDataReq();
				return;
			}
		}
		else
		{
			player->GetClientSession()->Disconnect(false);
		}
	}

	CNtlPacket packet(sizeof(sGM_LOGOUT_REQ));
	sGM_LOGOUT_REQ * res = (sGM_LOGOUT_REQ *)packet.GetPacketData();
	res->wOpCode = GM_LOGOUT_REQ;
	res->accountId = req->accountId;
	res->serverChannelId = app->GetGsChannel();
	res->serverId = app->GetGsServerId();
	packet.SetPacketLen(sizeof(sGM_LOGOUT_REQ));
	app->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		USER MOVE TO CHAR SERVER
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvUserMove(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sMG_MOVE_RES * req = (sMG_MOVE_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByAccount(req->accountId);
	if(player)
	{
		WORD resultcode = GAME_SUCCESS;

		CNtlPacket packet(sizeof(sGU_CHAR_EXIT_RES));
		sGU_CHAR_EXIT_RES* res = (sGU_CHAR_EXIT_RES *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_EXIT_RES;

		sDBO_SERVER_INFO* info = g_pServerInfoManager->GetIdlestServerInfo(NTL_SERVER_TYPE_CHARACTER, 0, 0);
		if(info)
		{
		//	ERR_LOG(LOG_USER, "Account %u connect success to char server %u, dwLoad %u, dwMaxLoad %u", req->accountId, info->byServerIndex, info->dwLoad, info->dwMaxLoad);

			strcpy_s(res->aServerInfo[0].szCharacterServerIP, NTL_MAX_LENGTH_OF_IP + 1, info->achPublicAddress );
			res->aServerInfo[0].wCharacterServerPortForClient = info->wPortForClient;
			res->aServerInfo[0].dwLoad = DWORD((float)info->dwLoad / (float)info->dwMaxLoad * 100.f);
			res->aServerInfo[0].serverchannelID = info->byServerChannelIndex;
			res->aServerInfo[0].serverfarmID = info->serverFarmId;
			memcpy(res->achAuthKey, req->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);

			info->dwLoad += 1;
		}
		else resultcode = GAME_CANNOT_CONNECT_DUE_TO_INVAlID_CHARACTER;

		res->byServerInfoCount = 1;
		res->wResultCode = resultcode;
		packet.SetPacketLen( sizeof(sGU_CHAR_EXIT_RES) );
		app->Send(player->GetClientSessionID(), &packet);
	}
}


//--------------------------------------------------------------------------------------//
//		RECEIVE SWITCH CHANNEL RESULT
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvPlayerSwitchChannel(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sMG_PLAYER_SWITCH_CHANNEL_RES * req = (sMG_PLAYER_SWITCH_CHANNEL_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByAccount(req->accountId);
	if (player && player->GetClientSession())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(req->serverInfo.serverFarmId, req->serverInfo.byServerChannelIndex);

			pChannel->dwLoad += 1;

			player->CreateDestServerInfo(&req->serverInfo, req->abyAuthKey);
			player->SetServerTeleportWorld(player->GetWorldID(), player->GetWorldTblidx());

			if (player->GetAspectStateId() == ASPECTSTATE_VEHICLE)
				player->EndVehicle(GAME_VEHICLE_END_BY_FORCED);

			if (player->GetPcIsFreeBattle())
				g_pFreeBattleManager->EndFreeBattle(player->GetFreeBattleID(), player->GetFreeBattleTarget());

			if (player->GetCurrentPetId() != INVALID_HOBJECT)
			{
				if (CSummonPet* pPet = g_pObjectManager->GetSummonPet(player->GetCurrentPetId()))
					pPet->Despawn(true);
			}

			player->SendCharStateLeaving(CHARLEAVING_CHANNEL_CHANGE, false);

			player->SaveToDB(false);
			app->GetGameMain()->GetWorldManager()->LeaveObject(player);

			CNtlPacket packet2(sizeof(sGT_CONNECT_CHANNEL_CHANGE_REQ));
			sGT_CONNECT_CHANNEL_CHANGE_REQ * res2 = (sGT_CONNECT_CHANNEL_CHANGE_REQ *)packet2.GetPacketData();
			res2->wOpCode = GT_CONNECT_CHANNEL_CHANGE_REQ;
			res2->accountID = req->accountId;
			res2->characterID = player->GetCharID();
			res2->gameServerChannelID = req->serverInfo.byServerChannelIndex;
			res2->gameServerIndex = req->serverInfo.serverFarmId;
			res2->hObject = INVALID_HOBJECT;
			packet2.SetPacketLen(sizeof(sGT_CONNECT_CHANNEL_CHANGE_REQ));
			app->SendTo(app->GetChatServerSession(), &packet2);
		}
		else
		{
			CNtlPacket packet(sizeof(sGU_CHAR_CHANNEL_CHANGE_RES));
			sGU_CHAR_CHANNEL_CHANGE_RES * res = (sGU_CHAR_CHANNEL_CHANGE_RES *)packet.GetPacketData();
			res->wOpCode = GU_CHAR_CHANNEL_CHANGE_RES;
			res->wResultCode = GAME_FAIL;
			packet.SetPacketLen(sizeof(sGU_CHAR_CHANNEL_CHANGE_RES));
			player->SendPacket(&packet);
		}
	}
}

void CMasterServerSession::RecvCharServerTeleportRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sMG_CHAR_SERVER_TELEPORT_RES * req = (sMG_CHAR_SERVER_TELEPORT_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByAccount(req->accountId);
	if (player && player->GetClientSession())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			player->CreateDestServerInfo(&req->serverInfo, req->abyAuthKey);

			CNtlPacket packet2(sizeof(sGT_CHAR_SERVER_TELEPORT));
			sGT_CHAR_SERVER_TELEPORT * res2 = (sGT_CHAR_SERVER_TELEPORT *)packet2.GetPacketData();
			res2->wOpCode = GT_CHAR_SERVER_TELEPORT;
			res2->charId = player->GetCharID();
			res2->sTeleportInfo = player->GetServerTeleportInfo();
			memcpy(res2->abyAuthKey, req->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
			packet2.SetPacketLen(sizeof(sGT_CHAR_SERVER_TELEPORT));
			app->SendTo(app->GetChatServerSession(), &packet2);
		}
		else
		{
			CNtlPacket packet(sizeof(sGU_CHAR_SERVER_CHANGE_RES));
			sGU_CHAR_SERVER_CHANGE_RES * res = (sGU_CHAR_SERVER_CHANGE_RES *)packet.GetPacketData();
			res->wOpCode = GU_CHAR_SERVER_CHANGE_RES;
			res->wResultCode = req->wResultCode;
			packet.SetPacketLen(sizeof(sGU_CHAR_SERVER_CHANGE_RES));
			player->SendPacket(&packet);
		}
	}
}
