#include "stdafx.h"
#include "NtlPacketMA.h"
#include "AuthServer.h"
#include "NtlPacketAU.h"
#include "NtlResultCode.h"

//--------------------------------------------------------------------------------------//
//		ADD SERVER INFO
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvServersInfoAdd(CNtlPacket * pPacket, CAuthServer * app)
{
	UNREFERENCED_PARAMETER(app);
	sMA_SERVERS_INFO_ADD * req = (sMA_SERVERS_INFO_ADD*)pPacket->GetPacketData();

	printf("received server info. index %u port %u\n", req->serverInfo.byServerIndex, req->serverInfo.wPortForClient);

	g_pServerInfoManager->RefreshServerInfo(&req->serverInfo);
}

//--------------------------------------------------------------------------------------//
//		PLAYER ONLINE CHECK RESULT. IF OFFLINE THEN SEND LOGIN SUCCESS
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvPlayerOnlineCheck(CNtlPacket * pPacket, CAuthServer * app)
{
	sMA_ON_PLAYER_CHECK_RES * req = (sMA_ON_PLAYER_CHECK_RES*)pPacket->GetPacketData();
	WORD resultcode = AUTH_USER_EXIST_IN_CHARACTER_SERVER;
	CClientSession* session = app->FindPlayer(req->accountId);
	if(session != NULL)
	{
		if(req->bIsOnline == false)
		{
			if(session != NULL)
			{
				resultcode = AUTH_SUCCESS;
			}
			else
			{
				resultcode = AUTH_USER_NOT_FOUND;
			}
		}

		if(resultcode == AUTH_SUCCESS)
		{
			sDBO_SERVER_INFO* srvinfo = g_pServerInfoManager->GetIdlestServerInfo(NTL_SERVER_TYPE_CHARACTER, 0, 0);

			CNtlPacket packet(sizeof(sAU_LOGIN_RES));
			sAU_LOGIN_RES * res = (sAU_LOGIN_RES *)packet.GetPacketData();
			res->wOpCode = AU_LOGIN_RES;
			NTL_SAFE_WCSCPY(res->awchUserId, req->awchUserId);

			memcpy(res->abyAuthKey, req->abyAuthKey, sizeof(res->abyAuthKey));
			res->dwAllowedFunctionForDeveloper = req->dwAllowedFunctionForDeveloper;
			res->accountId = req->accountId;
			if (srvinfo)
			{
				if (srvinfo->dwLoad <= srvinfo->dwMaxLoad)
				{
					ERR_LOG(LOG_USER, "Account %u connect success to char server %u, dwLoad %u, dwMaxLoad %u", req->accountId, srvinfo->byServerIndex, srvinfo->dwLoad, srvinfo->dwMaxLoad);

					resultcode = AUTH_SUCCESS;
					strcpy_s(res->aServerInfo[0].szCharacterServerIP, NTL_MAX_LENGTH_OF_IP + 1, srvinfo->achPublicAddress);
					res->aServerInfo[0].wCharacterServerPortForClient = srvinfo->wPortForClient;
					res->aServerInfo[0].dwLoad = (DWORD)((float)srvinfo->dwLoad / (float)srvinfo->dwMaxLoad * 100.0f);
					res->aServerInfo[0].serverfarmID = srvinfo->serverFarmId;
					res->aServerInfo[0].serverchannelID = srvinfo->byServerChannelIndex;
					res->byServerInfoCount = 1;

					//update load
					srvinfo->dwLoad += 1;
				}
				else resultcode = CHARACTER_USER_SHOULD_WAIT_FOR_CONNECT;
			}
			else resultcode = AUTH_NO_AVAILABLE_CHARACTER_SERVER;

			res->wResultCode = resultcode;
			res->lastServerFarmId = req->lastServerFarmId;
			res->bIsGM = req->bIsGM;

			if (resultcode == AUTH_SUCCESS)
			{
				packet.SetPacketLen(sizeof(sAU_LOGIN_RES));
				app->SendTo(session, &packet);

				GetAccDB.Execute("UPDATE accounts SET last_login=CURRENT_TIMESTAMP, last_ip='%s' WHERE AccountID = %u LIMIT 1", session->GetRemoteIP(), req->accountId);
				GetLogDB.Execute("INSERT INTO auth_login_log(AccountID, IP) VALUES (%u, '%s')", req->accountId, session->GetRemoteIP());

				return;
			}
		}

		ERR_LOG(LOG_USER, "Account % connect failed. Resultcode %d", req->accountId, resultcode);

		//IF NOT SUCCESS SEND ERROR MSG
		CNtlPacket packet2(sizeof(sAU_LOGIN_RES));
		sAU_LOGIN_RES * res2 = (sAU_LOGIN_RES *)packet2.GetPacketData();
		res2->wOpCode = AU_LOGIN_RES;
		res2->wResultCode = resultcode;
		packet2.SetPacketLen(sizeof(sAU_LOGIN_RES));
		app->SendTo(session, &packet2);

		app->DelPlayer(req->accountId);
	}
}

//--------------------------------------------------------------------------------------//
//		UPDATE CHAR SERVER
//--------------------------------------------------------------------------------------//
void CMasterServerSession::RecvServerInfoChangedNfy(CNtlPacket * pPacket, CAuthServer * app)
{
	UNREFERENCED_PARAMETER(app);
	sMA_SERVER_INFO_CHANGED_NFY * req = (sMA_SERVER_INFO_CHANGED_NFY*)pPacket->GetPacketData();
	
	sDBO_SERVER_INFO* pServerInfo = g_pServerInfoManager->GetServerInfo(req->byServerType, req->serverFarmId, req->serverChannelId, req->serverIndex);
	if (pServerInfo)
	{
		pServerInfo->dwLoad = req->dwLoad;
		pServerInfo->byRunningState = req->byRunningState;
		pServerInfo->bIsOn = req->bIsOn;
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "ServerType %u Index %u Farm %u Channel %u not found.", req->byServerType, req->serverIndex, req->serverFarmId, req->serverChannelId);
	}
}