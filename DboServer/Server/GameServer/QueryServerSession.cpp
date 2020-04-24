#include "stdafx.h"
#include "QueryServerSession.h"
#include "GameServer.h"
#include "GameProcessor.h"
#include "NtlPacketGQ.h"
#include "ObjectManager.h"
#include "DynamicFieldSystemEvent.h"
#include "qg_opcodes.h"
#include "PacketEventObj.h"


int CQueryServerSession::OnConnect()
{
	NTL_PRINT(PRINT_APP, "CONNECTED TO QUERY SERVER");

	CGameServer * app = (CGameServer*)g_pApp;

	app->SetQueryServerSession(this);

	CNtlPacket packet(sizeof(sGQ_NOTIFY_SERVER_BEGIN));
	sGQ_NOTIFY_SERVER_BEGIN* res = (sGQ_NOTIFY_SERVER_BEGIN*)packet.GetPacketData();
	res->wOpCode = GQ_NOTIFY_SERVER_BEGIN;
	res->byServerChannelId = app->GetGsChannel();
	app->SendTo(app->GetQueryServerSession(), &packet);

	g_pDynamicFieldSystemEvent->Create();

	return CNtlSession::OnConnect();
}


void CQueryServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "DISCONNECTED FROM QUERY SERVER");
	CGameServer * app = (CGameServer*)g_pApp;

	app->SetQueryServerSession(NULL);
}


int CQueryServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CGameServer * app = (CGameServer*)g_pApp;
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	OpcodeHandler<CQueryServerSession> const* opHandle = qg_opcodeTable->LookupOpcode(pHeader->wOpCode);
	if (opHandle)
	{
		if (opHandle->packetProcessing == PROCESS_INPLACE)
			(this->*opHandle->handler)(pPacket);
		else
			g_pGameProcessor->PostClientPacketEvent(new TPacketEventObj<CQueryServerSession>(this, opHandle->handler, GetHandle(), pPacket, GetUniqueHandle()));
	}
	else
	{
		return CNtlSession::OnDispatch(pPacket);
	}

	return NTL_SUCCESS;
}


void CQueryServerSession::OnInvalid(CNtlPacket* pPacket)
{
}
