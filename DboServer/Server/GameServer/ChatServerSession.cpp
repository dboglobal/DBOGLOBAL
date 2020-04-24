#include "stdafx.h"
#include "GameServer.h"
#include "NtlPacketGT.h"
#include "GameProcessor.h"

#include "tg_opcodes.h"
#include "PacketEventObj.h"

int CChatServerSession::OnConnect()
{
	NTL_PRINT(PRINT_APP,"CONNECTED TO CHAT SERVER");
	CGameServer * app = (CGameServer*)g_pApp;

	app->SetChatServerSession(this);

	CNtlPacket packet(sizeof(sGT_GAME_INFO));
	sGT_GAME_INFO * res = (sGT_GAME_INFO *)packet.GetPacketData();
	res->wOpCode = GT_GAME_INFO;
	res->serverChannelId = app->GetGsChannel();
	packet.SetPacketLen( sizeof(sGT_GAME_INFO) );
	app->Send(GetHandle(), &packet);


	return CNtlSession::OnConnect();
}


void CChatServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP,"DISCONNECTED FROM CHAT SERVER");
	CGameServer * app = (CGameServer*)g_pApp;

	app->SetChatServerSession(NULL);
}


int CChatServerSession::OnDispatch(CNtlPacket * pPacket)
{
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

//	printf("pHeader->wOpCode %d \n", pHeader->wOpCode);
	
	OpcodeHandler<CChatServerSession> const* opHandle = tg_opcodeTable->LookupOpcode(pHeader->wOpCode);
	if (opHandle)
	{
		if (opHandle->packetProcessing == PROCESS_INPLACE)
			(this->*opHandle->handler)(pPacket);
		else
			g_pGameProcessor->PostClientPacketEvent(new TPacketEventObj<CChatServerSession>(this, opHandle->handler, GetHandle(), pPacket, GetUniqueHandle()));
	}
	else
	{
		return CNtlSession::OnDispatch(pPacket);
	}
	

	return NTL_SUCCESS;
}