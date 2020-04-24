#include "stdafx.h"
#include "MasterServerSession.h"
#include "ChatServer.h"
#include "NtlPacketTM.h"
#include "NtlPacketMT.h"

int CMasterServerSession::OnConnect()
{
	CChatServer * app = (CChatServer*)g_pApp;
	NTL_PRINT(PRINT_APP, "CONNECTED TO MASTER SERVER \n");

	app->SetMasterServerSession(this);

	//send server data to master server
	CNtlPacket packet(sizeof(sTM_NOTIFY_SERVER_BEGIN));
	sTM_NOTIFY_SERVER_BEGIN * res = (sTM_NOTIFY_SERVER_BEGIN *)packet.GetPacketData();
	res->wOpCode = TM_NOTIFY_SERVER_BEGIN;
	res->sServerInfo.bIsOn = true;
	res->sServerInfo.byRunningState = DBO_SERVER_RUNNING_STATE_RUNNING;
	res->sServerInfo.byServerIndex = 0;
	res->sServerInfo.serverFarmId = 0;
	res->sServerInfo.byServerChannelIndex = 0;
	res->sServerInfo.dwLoad = 0;
	res->sServerInfo.dwMaxLoad = DWORD((float)app->m_config.nMaxConnection * 0.9f);
	res->sServerInfo.wPortForClient = app->m_config.wClientAcceptPort;
	strcpy_s(res->sServerInfo.achPublicAddress, NTL_MAX_LENGTH_OF_IP + 1, app->m_config.strClientAcceptAddr.c_str());
	res->sServerInfo.byServerType = NTL_SERVER_TYPE_COMMUNITY;
	packet.SetPacketLen(sizeof(sTM_NOTIFY_SERVER_BEGIN));
	app->Send(GetHandle(), &packet);

	return CNtlSession::OnConnect();
}


void CMasterServerSession::OnClose()
{
	CChatServer * app = (CChatServer*)g_pApp;
	app->SetMasterServerSession(NULL);
	NTL_PRINT(PRINT_APP, "DISCONNECTED FROM MASTER SERVER \n");
}


int CMasterServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CChatServer * app = (CChatServer*)g_pApp;
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("pHeader->wOpCode %d \n", pHeader->wOpCode);
	/*switch (pHeader->wOpCode)
	{


		default:{} break;

	}*/


	return NTL_SUCCESS;
}