#include "stdafx.h"
#include "NtlPacketMC.h"
#include "NtlPacketCU.h"
#include "CharServer.h"
#include "NtlResultCode.h"
#include "Player.h"
#include "NtlService.h"


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CMasterServerSession::OnInvalid(CNtlPacket * pPacket)
{
	UNREFERENCED_PARAMETER(pPacket);
}

//--------------------------------------------------------------------------------------//
//		RECEIVE GAME SERVER DATA
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvServerInfoAdd(CNtlPacket * pPacket)
{
	sMC_SERVERS_INFO_ADD * req = (sMC_SERVERS_INFO_ADD*)pPacket->GetPacketData();

	/*
		Info: When adding gameserver then farm info and channel info must be send first.
	*/

	g_pServerInfoManager->RefreshServerInfo(&req->serverInfo);
}

//--------------------------------------------------------------------------------------//
//		RECEIVE GAME SERVER FARM INFO
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvGameServarFarmInfo(CNtlPacket * pPacket)
{
	sMC_GAME_SERVER_FARM_INFO * req = (sMC_GAME_SERVER_FARM_INFO*)pPacket->GetPacketData();

	g_pServerInfoManager->RefreshServerFarmInfo(&req->gameServerFarmInfo);

	CNtlPacket packet(sizeof(sCU_SERVER_FARM_INFO));
	sCU_SERVER_FARM_INFO * res = (sCU_SERVER_FARM_INFO *)packet.GetPacketData();
	res->wOpCode = CU_SERVER_FARM_INFO;
	memmove(&res->serverFarmInfo, &req->gameServerFarmInfo, sizeof(sDBO_GAME_SERVER_FARM_INFO));
	packet.SetPacketLen(sizeof(sCU_SERVER_FARM_INFO));
	g_PlrMgr->SendToAll(&packet);
}

//--------------------------------------------------------------------------------------//
//		RECEIVE GAME SERVER CHANNEL INFO
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvGameServerChannelInfo(CNtlPacket * pPacket)
{
	sMC_GAME_SERVER_CHANNEL_INFO * req = (sMC_GAME_SERVER_CHANNEL_INFO*)pPacket->GetPacketData();

	g_pServerInfoManager->RefreshServerChannelInfo(&req->gameServerChannelInfo);
	
	if (req->gameServerChannelInfo.bIsVisible == false)
		return;

	CNtlPacket packet(sizeof(sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY));
	sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY * res = (sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY *)packet.GetPacketData();
	res->wOpCode = CU_SERVER_CHANNEL_INFO_REFRESHED_NFY;
	memmove(&res->serverChannelInfo, &req->gameServerChannelInfo, sizeof(sDBO_GAME_SERVER_CHANNEL_INFO));
	packet.SetPacketLen(sizeof(sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY));
	g_PlrMgr->SendToAll(&packet, req->gameServerChannelInfo.serverFarmId);
}


//--------------------------------------------------------------------------------------//
//		GAME SERVER FARM SERVER UPDATED NFY
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvGameServerFarmInfoChangedNfy(CNtlPacket * pPacket)
{
	sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY * req = (sMC_GAME_SERVER_FARM_INFO_CHANGED_NFY*)pPacket->GetPacketData();

	sSERVER_FARM_INFO* pFarm = g_pServerInfoManager->GetServerFarmInfo(req->serverFarmId);
	if (pFarm)
	{
	//	printf("RecvGameServerFarmInfoChangedNfy: Update Srv %d  \n", pFarm->serverFarmId);
		pFarm->dwLoad = req->dwLoad;
		pFarm->dwMaxLoad = req->dwMaxLoad;
		pFarm->byServerStatus = req->byServerStatus;
	//	printf("pFarm->dwLoad %u pFarm->dwMaxLoad %u \n", pFarm->dwLoad, pFarm->dwMaxLoad);
		//update server farm
		CNtlPacket packet(sizeof(sCU_SERVER_FARM_INFO_REFRESHED_NFY));
		sCU_SERVER_FARM_INFO_REFRESHED_NFY * res = (sCU_SERVER_FARM_INFO_REFRESHED_NFY *)packet.GetPacketData();
		res->wOpCode = CU_SERVER_FARM_INFO_REFRESHED_NFY;
		res->serverFarmId = req->serverFarmId;
		res->byServerStatus = req->byServerStatus;
		res->dwMaxLoad = 100; // 100%
		res->dwLoad = DWORD((float)req->dwLoad / (float)req->dwMaxLoad * 100.f);
		packet.SetPacketLen(sizeof(sCU_SERVER_FARM_INFO_REFRESHED_NFY));
		g_PlrMgr->SendToAll(&packet);
	}
}

//--------------------------------------------------------------------------------------//
//		UPDATE CHANNEL DATA
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvGameServerChannelUpdated(CNtlPacket * pPacket)
{
	sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY * req = (sMC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY*)pPacket->GetPacketData();

	sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(req->serverFarmId, req->byServerChannelIndex);
	if (pChannel)
	{
	//	printf("RecvGameServerChannelUpdated: Update Srv %d Channel %d \n", pChannel->serverFarmId, pChannel->byServerChannelIndex);
		pChannel->dwLoad = req->dwLoad;
		pChannel->dwMaxLoad = req->dwMaxLoad;
		pChannel->byServerStatus = req->byServerStatus;
		pChannel->bIsScrambleChannel = req->bIsScrambleChannel;
	//	printf("pChannel->dwLoad %u pChannel->dwMaxLoad %u \n", pChannel->dwLoad, pChannel->dwMaxLoad);

		if (pChannel->bIsVisible == false)
			return;

		//update channel

		CNtlPacket packet(sizeof(sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY));
		sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY * res = (sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY *)packet.GetPacketData();
		res->wOpCode = CU_SERVER_CHANNEL_INFO_REFRESHED_NFY;
		res->serverChannelInfo.serverFarmId = pChannel->serverFarmId;
		res->serverChannelInfo.byServerChannelIndex = pChannel->byServerChannelIndex;
		res->serverChannelInfo.bIsVisible = pChannel->bIsVisible;
		res->serverChannelInfo.byServerStatus = pChannel->byServerStatus;
		res->serverChannelInfo.dwMaxLoad = 100;
		res->serverChannelInfo.dwLoad = DWORD((float)req->dwLoad / (float)req->dwMaxLoad * 100.f);
		res->serverChannelInfo.bIsScrambleChannel = pChannel->bIsScrambleChannel;
		memcpy(&res->serverChannelInfo.sChannelBuff, &pChannel->sChannelBuff, sizeof(_CHANNEL_BUFF));
		res->serverChannelInfo.bUnknown = pChannel->bUnknown;
		packet.SetPacketLen( sizeof(sCU_SERVER_CHANNEL_INFO_REFRESHED_NFY) );
		g_PlrMgr->SendToAll(&packet, req->serverFarmId);
	}
}


//--------------------------------------------------------------------------------------//
//		USER LOGIN TO CHAR SERVER RESULT
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvUserLoginRes(CNtlPacket * pPacket)
{
	sMC_LOGIN_RES * req = (sMC_LOGIN_RES*)pPacket->GetPacketData();
	
	CPlayer* player = g_PlrMgr->GetPlayer(req->accountId);
	if(player && player->GetSession())
	{
		if(req->wResultCode == CHARACTER_SUCCESS)
		{
			SERVERFARMID lastServerFarmId = player->GetServerFarmID();

			//check if server exist or offline
			if (lastServerFarmId != INVALID_SERVERFARMID)
			{
				sSERVER_FARM_INFO* pServerFarm = g_pServerInfoManager->GetServerFarmInfo(lastServerFarmId);
				if (pServerFarm == NULL)
				{
					lastServerFarmId = INVALID_SERVERFARMID;
				}
				else if (pServerFarm->byServerStatus == DBO_SERVER_STATUS_DOWN)
				{
					lastServerFarmId = INVALID_SERVERFARMID;
				}
			}

			CNtlPacket packet(sizeof(sCU_LOGIN_RES));
			sCU_LOGIN_RES * res = (sCU_LOGIN_RES *)packet.GetPacketData();
			res->wOpCode = CU_LOGIN_RES;
			res->wResultCode = CHARACTER_SUCCESS;
			res->lastServerFarmId = lastServerFarmId;
			res->dwRaceAllowedFlag = DBO_ALLOWED_FUNC_FOR_DEV_FLAG_HUMAN + DBO_ALLOWED_FUNC_FOR_DEV_FLAG_NAMEK + DBO_ALLOWED_FUNC_FOR_DEV_FLAG_MAJIN;
			res->wSlotBasic = CREATE_CHAR_BASIC_SLOT;
			res->wSlotPremium = CREATE_CHAR_PREMIUM_SLOT;
			packet.SetPacketLen(sizeof(sCU_LOGIN_RES));
			g_pApp->Send(player->GetSessionHandle(), &packet);
		}
		else
		{
			ERR_LOG(LOG_USER, "Login failed. Account %u already online or auth key expired", req->accountId);
			player->GetSession()->Disconnect(false);
		}
	}
	else
	{
		ERR_LOG(LOG_USER, "Login failed. Account %u not exist or dont have a season", req->accountId);

		if (player)
			player->GetSession()->Disconnect(false);
	}
}
//--------------------------------------------------------------------------------------//
//		CHAR EXIT
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvCharExit(CNtlPacket * pPacket)
{
	sMC_CHARACTER_EXIT_RES * req = (sMC_CHARACTER_EXIT_RES*)pPacket->GetPacketData();

	CPlayer* player = g_PlrMgr->GetPlayer(req->accountId);
	if(player && player->GetSessionHandle() != INVALID_HSESSION)
	{
		CNtlPacket packet(sizeof(sCU_CHARACTER_EXIT_RES));
		sCU_CHARACTER_EXIT_RES * res = (sCU_CHARACTER_EXIT_RES *)packet.GetPacketData();
		res->wOpCode = CU_CHARACTER_EXIT_RES;
		res->wResultCode = CHARACTER_SUCCESS;
		res->lastServerFarmId = player->GetServerFarmID();
		res->byMoveType = req->byMoveType;
		strcpy_s((char*)res->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY, "548CaAutfgfdey6");
		packet.SetPacketLen( sizeof(sCU_CHARACTER_EXIT_RES) );
		g_pApp->Send(player->GetSessionHandle(), &packet);
	}
}
//--------------------------------------------------------------------------------------//
//		USER MOVE TO GAME SERVER
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvUserMove(CNtlPacket * pPacket)
{
	sMC_MOVE_RES * req = (sMC_MOVE_RES*)pPacket->GetPacketData();

	WORD resultcode = req->wResultCode;
	CPlayer* player = g_PlrMgr->GetPlayer(req->accountId);
	if(player && player->GetSessionHandle() != INVALID_HSESSION)
	{
		if(req->wResultCode == CHARACTER_SUCCESS)
		{
			CNtlPacket packet(sizeof(sCU_CHARACTER_SELECT_RES));
			sCU_CHARACTER_SELECT_RES * res = (sCU_CHARACTER_SELECT_RES *)packet.GetPacketData();
			res->wOpCode = CU_CHARACTER_SELECT_RES;
			res->charId = player->GetCharSelect();

			sDBO_SERVER_INFO* pServerInfo = g_pServerInfoManager->GetServerInfo(NTL_SERVER_TYPE_GAME, player->GetServerFarmID(), player->GetServerChannelID(), player->GetServerChannelID());
			if (pServerInfo)
			{
				sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(pServerInfo->serverFarmId, pServerInfo->byServerChannelIndex);
				if (pChannel)
				{
					if (pServerInfo->byRunningState == DBO_SERVER_RUNNING_STATE_RUNNING)
					{
						pChannel->dwLoad += 1;

					//	printf("player online %u in channel %u \n", pChannel->dwLoad, pServerInfo->byServerChannelIndex);

						memcpy(res->abyAuthKey, req->abyAuthKey, sizeof(res->abyAuthKey));
						strcpy_s(res->szGameServerIP, NTL_MAX_LENGTH_OF_IP + 1, pServerInfo->achPublicAddress);
						res->wGameServerPortForClient = pServerInfo->wPortForClient;
					}
					else resultcode = GAME_SERVER_LOCKED;
				}
				else resultcode = GAME_WRONG_SERVER_CHANNEL_HAS_BEEN_SPECIFIED;
			}
			else resultcode = GAME_WRONG_SERVER_CHANNEL_HAS_BEEN_SPECIFIED;

			res->wResultCode = resultcode;
			packet.SetPacketLen( sizeof(sCU_CHARACTER_SELECT_RES) );
			g_pApp->Send(player->GetSessionHandle(), &packet);
		}
	}
}