#include "stdafx.h"
#include "MasterServerSession.h"
#include "AuthServer.h"
#include "NtlPacketMA.h"
#include "NtlPacketAM.h"


int CMasterServerSession::OnConnect()
{
	CAuthServer * app = (CAuthServer*)g_pApp;
	NTL_PRINT(PRINT_APP, "CONNECTED TO MASTER SERVER \n");

	app->SetMasterServerSession( this );

	//send server data to master server
	CNtlPacket packet(sizeof(sAM_NOTIFY_SERVER_BEGIN));
	sAM_NOTIFY_SERVER_BEGIN * res = (sAM_NOTIFY_SERVER_BEGIN *)packet.GetPacketData();
	res->wOpCode = AM_NOTIFY_SERVER_BEGIN;
	res->sServerInfo.bIsOn = true;
	res->sServerInfo.byRunningState = DBO_SERVER_RUNNING_STATE_RUNNING;
	res->sServerInfo.byServerIndex = app->m_config.byServerId;
	res->sServerInfo.dwLoad = 0;
	res->sServerInfo.dwMaxLoad = (DWORD)((float)app->m_config.nMaxConnection * 0.9f);
	res->sServerInfo.wPortForClient = app->m_config.wClientAcceptPort;
	strcpy_s(res->sServerInfo.achPublicAddress, NTL_MAX_LENGTH_OF_IP + 1, app->m_config.strClientAcceptAddr.c_str());
	res->sServerInfo.byServerType = NTL_SERVER_TYPE_AUTH;
	packet.SetPacketLen(sizeof(sAM_NOTIFY_SERVER_BEGIN));
	app->Send(GetHandle(), &packet);


	return CNtlSession::OnConnect();
}


void CMasterServerSession::OnClose()
{
	CAuthServer * app = (CAuthServer*)g_pApp;
	app->SetMasterServerSession(NULL);

	NTL_PRINT(PRINT_APP, "DISCONNECTED FROM MASTER SERVER \n");
}


int CMasterServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CAuthServer * app = (CAuthServer*) NtlSfxGetApp();
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("pHeader->wOpCode %d \n", pHeader->wOpCode);
	switch( pHeader->wOpCode )
	{
		case MA_SERVERS_INFO_ADD:			RecvServersInfoAdd(pPacket, app);	break;
		case MA_ON_PLAYER_CHECK_RES:		RecvPlayerOnlineCheck(pPacket,app);	break;
		case MA_SERVER_INFO_CHANGED_NFY:	RecvServerInfoChangedNfy(pPacket, app);	break;

		default:	NTL_PRINT(PRINT_APP, "CMasterServerSession::OnDispatch: received unknown packet %d", pHeader->wOpCode);	break;
	}
	

	return NTL_SUCCESS;
}