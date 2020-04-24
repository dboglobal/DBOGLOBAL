#include "stdafx.h"
#include "GameServer.h"
#include "mg_opcodes.h"
#include "NtlPacketGM.h"
#include "SubNeighborServerInfoManager.h"
#include "GameProcessor.h"
#include "ObjectManager.h"

#include "PacketEventObj.h"



int CMasterServerSession::OnConnect()
{
	NTL_PRINT(PRINT_APP,"CONNECTED TO MASTER SERVER");
	CGameServer * app = (CGameServer*) NtlSfxGetApp();
	app->SetMasterServerSession(this);


	//send server data to master server
	CNtlPacket packet(sizeof(sGM_NOTIFY_SERVER_BEGIN));
	sGM_NOTIFY_SERVER_BEGIN * res = (sGM_NOTIFY_SERVER_BEGIN *)packet.GetPacketData();
	res->wOpCode = GM_NOTIFY_SERVER_BEGIN;

	//server info
	res->serverInfo.bIsOn = true;
	res->serverInfo.byRunningState = DBO_SERVER_RUNNING_STATE_RUNNING;
	res->serverInfo.byServerIndex = app->m_config.byChannel;
	res->serverInfo.serverFarmId = app->m_config.byServerID;
	res->serverInfo.byServerChannelIndex = app->m_config.byChannel;
	res->serverInfo.dwLoad = 0;
	res->serverInfo.dwMaxLoad = DWORD((float)app->m_config.nMaxConnection * 0.95f); //set max connections to 95% limit
	res->serverInfo.wPortForClient = app->m_config.wClientAcceptPort;
	strcpy_s(res->serverInfo.achPublicAddress, NTL_MAX_LENGTH_OF_IP + 1, app->m_config.strPublicClientAcceptAddr.c_str());
	res->serverInfo.byGameServerRole = DBO_GAME_SERVER_ROLE_GENERAL;
	res->serverInfo.byServerType = NTL_SERVER_TYPE_GAME;

	//server farm indo
	res->gameServerFarmInfo.serverFarmId = app->m_config.byServerID;
	wcscpy_s(res->gameServerFarmInfo.wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, s2ws(app->m_config.ServerName.c_str()).c_str());
	res->gameServerFarmInfo.byServerStatus = DBO_SERVER_STATUS_UP;
	res->gameServerFarmInfo.dwMaxLoad = res->serverInfo.dwMaxLoad;
	res->gameServerFarmInfo.dwLoad = 0;

	//channel info
	res->gameServerChannelInfo.serverFarmId = app->m_config.byServerID;
	res->gameServerChannelInfo.byServerChannelIndex = app->m_config.byChannel;
	res->gameServerChannelInfo.bIsVisible = (app->m_config.byChannel == DOJO_CHANNEL_INDEX) ? false : true;
	res->gameServerChannelInfo.byServerStatus = DBO_SERVER_STATUS_UP;
	res->gameServerChannelInfo.dwMaxLoad = res->serverInfo.dwMaxLoad;
	res->gameServerChannelInfo.dwLoad = 0;
	res->gameServerChannelInfo.bIsScrambleChannel = false;
	wcscpy_s(res->gameServerChannelInfo.sChannelBuff.wszServerChannelName, NTL_MAX_SIZE_SERVER_CHANNEL_NAME_UNICODE + 1, s2ws(app->m_config.ChannelName.c_str()).c_str());
	strcpy_s(res->gameServerChannelInfo.sChannelBuff.szServerChannelName, NTL_MAX_SIZE_SERVER_CHANNEL_NAME_UNICODE + 1, app->m_config.ChannelName.c_str());

	g_pServerInfoManager->RefreshServerFarmInfo(&res->gameServerFarmInfo); //add server farm info
	g_pServerInfoManager->RefreshServerChannelInfo(&res->gameServerChannelInfo); //add server channel info

	packet.SetPacketLen( sizeof(sGM_NOTIFY_SERVER_BEGIN) );
	app->Send(GetHandle(), &packet);


	return CNtlSession::OnConnect();
}

void CMasterServerSession::OnClose()
{
	CGameServer * app = (CGameServer*)NtlSfxGetApp();

	NTL_PRINT(PRINT_APP,"DISCONNECTED FROM MASTER SERVER. Player Online: %d", g_pObjectManager->GetPlayerCount());
	ERR_LOG(LOG_SYSTEM, "DISCONNECTED FROM MASTER SERVER. Player Online: %d", g_pObjectManager->GetPlayerCount());

	g_pGameProcessor->OnEvent_ServerShutdown();
}


int CMasterServerSession::OnDispatch(CNtlPacket * pPacket)
{
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("pHeader->wOpCode %d \n", pHeader->wOpCode);

	OpcodeHandler<CMasterServerSession> const* opHandle = mg_opcodeTable->LookupOpcode(pHeader->wOpCode);
	if (opHandle)
	{
		if (opHandle->packetProcessing == PROCESS_INPLACE)
			(this->*opHandle->handler)(pPacket);
		else
			g_pGameProcessor->PostClientPacketEvent(new TPacketEventObj<CMasterServerSession>(this, opHandle->handler, GetHandle(), pPacket, GetUniqueHandle()));
	}
	else
	{
		return CNtlSession::OnDispatch(pPacket);
	}
	

	return NTL_SUCCESS;
}
