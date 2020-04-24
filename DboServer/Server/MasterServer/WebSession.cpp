#include "stdafx.h"
#include "MasterServer.h"
#include "PacketHead.h"


int CWebSession::OnAccept()
{
	//NTL_PRINT(PRINT_APP, "CONNECTION FROM WEB-SERVER ACCEPTED");
	return CNtlSession::OnAccept();
}


void CWebSession::OnClose()
{
	//NTL_PRINT(PRINT_APP, "CONNECTION FROM WEB-SERVER CLOSED");
}


int CWebSession::OnDispatch(CNtlPacket * pPacket)
{
	CMasterServer * app = (CMasterServer*)g_pApp;
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("Web Server | pHeader->wOpCode %d \n", pHeader->wOpCode);

	switch (pHeader->wOpCode)
	{
		case WM_GET_PLAYER_COUNT:
		{
			CNtlPacket packet(sizeof(sWM_GET_PLAYER_COUNT_RES));
			sWM_GET_PLAYER_COUNT_RES * res = (sWM_GET_PLAYER_COUNT_RES *)packet.GetPacketData();
			res->wOpCode = WM_GET_PLAYER_COUNT_RES;
			_snprintf(res->chPlayerCount, sizeof(res->chPlayerCount), "%d", g_pSrvMgr->GetPlayerInGameServer() + g_pSrvMgr->GetPlayerInCharServer());
			app->Send(GetHandle(), &packet);
		}
		break;

		case WM_GET_CHANNEL_STATUS:
		{
			CNtlPacket packet(sizeof(sWM_GET_CHANNEL_STATUS_RES));
			sWM_GET_CHANNEL_STATUS_RES * res = (sWM_GET_CHANNEL_STATUS_RES *)packet.GetPacketData();
			res->wOpCode = WM_GET_CHANNEL_STATUS_RES;

			//set to default DOWN..
			_snprintf(res->auth, sizeof(res->auth), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->chat, sizeof(res->chat), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel0, sizeof(res->channel0), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel1, sizeof(res->channel1), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel2, sizeof(res->channel2), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel3, sizeof(res->channel3), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel4, sizeof(res->channel4), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel5, sizeof(res->channel5), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel6, sizeof(res->channel6), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel7, sizeof(res->channel7), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel8, sizeof(res->channel8), "%d", DBO_SERVER_STATUS_DOWN);
			_snprintf(res->channel9, sizeof(res->channel9), "%d", DBO_SERVER_STATUS_DOWN);


			sDBO_SERVER_INFO* pAuth = g_pSrvMgr->GetServerInfo(NTL_SERVER_TYPE_AUTH, 0, 0, 0);
			if (pAuth)
			{
				if (pAuth->bIsOn)
					_snprintf(res->auth, sizeof(res->auth), "%d", DBO_SERVER_STATUS_UP);
			}

			sDBO_SERVER_INFO* pChat = g_pSrvMgr->GetServerInfo(NTL_SERVER_TYPE_COMMUNITY, 0, 0, 0);
			if (pChat)
			{
				if(pChat->bIsOn)
					_snprintf(res->chat, sizeof(res->chat), "%d", DBO_SERVER_STATUS_UP);
			}

			sSERVER_FARM_INFO* pFarm = g_pSrvMgr->GetServerFarmInfo(0);
			if (pFarm)
			{
				for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it = pFarm->mapGameServerChannelInfo.begin(); it != pFarm->mapGameServerChannelInfo.end(); it++)
				{
					sSERVER_CHANNEL_INFO* pChannel = it->second;

					switch (pChannel->byServerChannelIndex)
					{
						case 0: _snprintf(res->channel0, sizeof(res->channel0), "%d", pChannel->byServerStatus); break;
						case 1: _snprintf(res->channel1, sizeof(res->channel1), "%d", pChannel->byServerStatus); break;
						case 2: _snprintf(res->channel2, sizeof(res->channel2), "%d", pChannel->byServerStatus); break;
						case 3: _snprintf(res->channel3, sizeof(res->channel3), "%d", pChannel->byServerStatus); break;
						case 4: _snprintf(res->channel4, sizeof(res->channel4), "%d", pChannel->byServerStatus); break;
						case 5: _snprintf(res->channel5, sizeof(res->channel5), "%d", pChannel->byServerStatus); break;
						case 6: _snprintf(res->channel6, sizeof(res->channel6), "%d", pChannel->byServerStatus); break;
						case 7: _snprintf(res->channel7, sizeof(res->channel7), "%d", pChannel->byServerStatus); break;
						case 8: _snprintf(res->channel8, sizeof(res->channel8), "%d", pChannel->byServerStatus); break;
						case 9: _snprintf(res->channel9, sizeof(res->channel9), "%d", pChannel->byServerStatus); break;
					}
				}
			}

			
			app->Send(GetHandle(), &packet);
		}
		break;

		default: ERR_LOG(LOG_NETWORK, "CWebSession: Undefined Packet. wOpCode = %u", pHeader->wOpCode); break;
	}


	return NTL_SUCCESS;
}