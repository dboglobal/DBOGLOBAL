#include "stdafx.h"

#include "PacketAuthServer.h"
#include "AuthServer.h"
#include "NtlService.h"
#include "NtlAdmin.h"


//--------------------------------------------------------------------------------------//
//		Get the account ID and log in to Char Server									//
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharLogInReq(CNtlPacket * pPacket, CAuthServer * app) 
{
	sUA_LOGIN_REQ * req = (sUA_LOGIN_REQ *)pPacket->GetPacketData();

	char* username = Ntl_WC2MB(req->awchUserId);
	
		WORD resultcode = AUTH_SUCCESS;
		sDBO_SERVER_INFO* pCharServer = NULL;
		
		if (req->wLVersion == CLIENT_LVER && (int)req->wRVersion == CLIENT_RVER)
		{
			pCharServer = g_pServerInfoManager->GetIdlestServerInfo(NTL_SERVER_TYPE_CHARACTER, 0, 0);
			if (pCharServer == NULL)
				resultcode = AUTH_NO_AVAILABLE_CHARACTER_SERVER;
			else if (!pCharServer->bIsOn)
				resultcode = AUTH_NO_AVAILABLE_CHARACTER_SERVER;
			else if (strlen(username) >= NTL_MAX_SIZE_USERID || strlen(username) < 3)
				resultcode = AUTH_TOO_LONG_ACCOUNT;
			else if (strlen(req->achPasswd) < NTL_MAX_SIZE_USERPW_ENCRYPT)
				resultcode = AUTH_TOO_LONG_PASSWORD;
			else if (app->IsAccountTempBlocked(username))
				resultcode = AUTH_USER_TEMPORARY_BLOCK;
		}
		else
		{
			resultcode = AUTH_VERSION_FAIL;
		}

		if (resultcode == AUTH_SUCCESS)
		{
			char mac[128];
			sprintf_s(mac, 128, "%02X:%02X:%02X:%02X:%02X:%02X",
				req->abyMacAddress[0], req->abyMacAddress[1], req->abyMacAddress[2], req->abyMacAddress[3], req->abyMacAddress[4], req->abyMacAddress[5]);

			ERR_LOG(LOG_USER, "User %s request connection! req->wLVersion %i, req->wRVersion %i, mac %s\n", username, (int)req->wLVersion, (int)req->wRVersion, mac);

			smart_ptr<QueryResult> result = GetAccDB.Query("SELECT AccountID,Password_hash,acc_status,isGm,lastServerFarmId,founder FROM accounts WHERE Username = \"%s\" LIMIT 1", GetAccDB.EscapeString(username).c_str());
			if (result)
			{
				Field* fields = result->Fetch();

				if (0 != strcmp(fields[1].GetString(), req->achPasswd)) //check password
					resultcode = AUTH_WRONG_PASSWORD;
				else
				{
					bool isGm = (fields[3].GetBYTE() > ADMIN_LEVEL_EARLY_ACCESS);
					std::string accstatus = fields[2].GetString();

					if (app->GetDisableConnection() == TRUE) //check if we disable connection
					{
						if (app->GetFounderConnection() == TRUE) //check if we allow founders to connect
						{
							if (fields[5].GetINT() == 0 && isGm == false) //if not founder and not gm then dont allow to connect
								resultcode = AUTH_SERVER_LOCKED;
						}
						else
						{
							if (isGm == false)
								resultcode = AUTH_SERVER_LOCKED;
						}
					}

					if (/*accstatus == "pending" ||*/ accstatus == "block") // pending = need email activate | block = account banned
						resultcode = AUTH_USER_BLOCK;
					else if (pCharServer->dwLoad >= pCharServer->dwMaxLoad && !isGm)
						resultcode = CHARACTER_USER_SHOULD_WAIT_FOR_CONNECT;

					if (resultcode == AUTH_SUCCESS)
					{
						this->AccountID = fields[0].GetUInt32();

						//check if acc already online
						if (app->AddPlayer(this->AccountID, this) == true)
						{
							ERR_LOG(LOG_USER, "%s Auth Success. <Online Check>Sending packet to master server \n", username);

							//send check req if player online to master server
							CNtlPacket packet(sizeof(sAM_ON_PLAYER_CHECK_REQ));
							sAM_ON_PLAYER_CHECK_REQ * res = (sAM_ON_PLAYER_CHECK_REQ *)packet.GetPacketData();
							res->wOpCode = AM_ON_PLAYER_CHECK_REQ;
							res->accountId = this->AccountID;
							wcscpy_s(res->awchUserId, NTL_MAX_SIZE_USERID + 1, req->awchUserId);
							res->bIsGM = isGm;
							res->dwAllowedFunctionForDeveloper = DBO_ALLOWED_FUNC_FOR_DEV_FLAG_HUMAN + DBO_ALLOWED_FUNC_FOR_DEV_FLAG_NAMEK + DBO_ALLOWED_FUNC_FOR_DEV_FLAG_MAJIN;
							res->lastServerFarmId = fields[4].GetBYTE();
							packet.SetPacketLen(sizeof(sAM_ON_PLAYER_CHECK_REQ));
							app->SendTo(app->m_pMasterServerSession, &packet);
						}
						else
						{
							resultcode = AUTH_USER_EXIST_IN_CHARACTER_SERVER;
							app->DelPlayer(this->AccountID);
						}
					}
				}
			}
			else
				resultcode = AUTH_USER_NOT_FOUND;
		}

		if(resultcode != AUTH_SUCCESS)
		{
			ERR_LOG(LOG_SYSTEM, "Session %u, Login trys %u, User %s Connection failed. Resultcode %u \n", GetHandle(), m_byLoginTrys, username, resultcode);

			if(resultcode == AUTH_WRONG_PASSWORD || resultcode == AUTH_USER_NOT_FOUND)
				++m_byLoginTrys;

			CNtlPacket packet(sizeof(sAU_LOGIN_RES));
			sAU_LOGIN_RES * res = (sAU_LOGIN_RES *)packet.GetPacketData();
			res->wOpCode = AU_LOGIN_RES;
			res->wResultCode = resultcode;
			app->Send(GetHandle(), &packet);

			if (m_byLoginTrys >= 5)
			{
				ERR_LOG(LOG_SYSTEM, "Session %u, IP %s, User %s too many login fails. IP blocked for 5 minutes \n", GetHandle(), GetRemoteIP(), username);

				//app->GetNetwork()->RegisterBlockedIp(GetRemoteAddr().GetAddr(), 270000);
				Disconnect(false);
			}
		}

	// free memory
	Ntl_CleanUpHeapString(username);
}

//--------------------------------------------------------------------------------------//
//		Disconnect from Auth Server
//--------------------------------------------------------------------------------------//
void CClientSession::SendLoginDcReq(CNtlPacket * pPacket, CAuthServer * app) 
{
	UNREFERENCED_PARAMETER(pPacket);

	CNtlPacket packet(sizeof(sAU_LOGIN_DISCONNECT_RES));
	sAU_LOGIN_DISCONNECT_RES * res = (sAU_LOGIN_DISCONNECT_RES *)packet.GetPacketData();
	res->wOpCode = AU_LOGIN_DISCONNECT_RES;
	packet.SetPacketLen(sizeof(sAU_LOGIN_DISCONNECT_RES));
	app->Send(GetHandle(), &packet);
}
