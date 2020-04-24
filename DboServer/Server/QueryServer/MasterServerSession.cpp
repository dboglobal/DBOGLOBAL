#include "stdafx.h"
#include "MasterServerSession.h"
#include "QueryServer.h"
#include "NtlPacketMQ.h"
#include "NtlPacketQM.h"


int CMasterServerSession::OnConnect()
{
	NTL_PRINT(PRINT_APP, "CONNECTED TO MASTER SERVER \n");

	g_SrvMgr->m_pMasterServerSession = this;

	return CNtlSession::OnConnect();

	return NTL_SUCCESS;
}


void CMasterServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "DISCONNECTED FROM MASTER SERVER \n");
}


int CMasterServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CQueryServer * app = (CQueryServer*)g_pApp;
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("pHeader->wOpCode %d \n", pHeader->wOpCode);
	switch (pHeader->wOpCode)
	{
	case MQ_PING_REQ:
	{
						sMQ_PING_REQ * req = (sMQ_PING_REQ*)pPacket->GetPacketData();

						CNtlPacket packet(sizeof(sQM_PING_RES));
						sQM_PING_RES * res = (sQM_PING_RES *)packet.GetPacketData();
						res->wOpCode = QM_PING_RES;
						res->dwTick = req->dwTick;
						packet.SetPacketLen(sizeof(sQM_PING_RES));
						app->SendTo(this, &packet);
	}break;

	default:{} break;
		
	}


	return NTL_SUCCESS;
}