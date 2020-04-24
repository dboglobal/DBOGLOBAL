#include "stdafx.h"
#include "SubNeighborServerInfoManager.h"
#include "GameServer.h"
#include "NtlPacketGM.h"


CSubNeighborServerInfoManager::CSubNeighborServerInfoManager()
{
}

CSubNeighborServerInfoManager::~CSubNeighborServerInfoManager()
{
}



void CSubNeighborServerInfoManager::SendMasterServerAlive()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGM_PING_RES));
	sGM_PING_RES * res = (sGM_PING_RES *)packet.GetPacketData();
	res->wOpCode = GM_PING_RES;
	res->dwTick = GetTickCount();
	packet.SetPacketLen(sizeof(sGM_PING_RES));

	int rc = app->SendTo(app->GetMasterServerSession(), &packet);
	if (rc != NTL_SUCCESS)
	{
		NTL_PRINT(PRINT_APP, "Failed to send ping to master server. Fail :%d[%s]", rc, NtlGetErrorMessage(rc));
		ERR_LOG(LOG_NETWORK, "Failed to send ping to master server. Fail :%d[%s]", rc, NtlGetErrorMessage(rc));
	}
}