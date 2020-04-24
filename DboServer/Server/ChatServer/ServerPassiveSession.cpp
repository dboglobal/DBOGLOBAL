#include "stdafx.h"
#include "gt_opcodes.h"
#include "ChatServer.h"
#include "Party.h"
#include "DojoManager.h"
#include "GuildManager.h"
#include "PacketEventObj.h"


int CServerPassiveSession::OnAccept()
{
	NTL_PRINT(PRINT_APP, "GAME SERVER CONNECTED \n");
	CChatServer * app = (CChatServer*)g_pApp;
	SetChannelID(254);

	g_pGuildManager->SendGuildData(GetHandle());
	g_pDojoManager->SendDojoData(GetHandle());

	return CNtlSession::OnAccept();
}


void CServerPassiveSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "GAME SERVER DISCONNECTED \n");

	//remove all party from channel
	g_pPartyManager->RemoveAllParties(GetChannelID());
}


int CServerPassiveSession::OnDispatch(CNtlPacket * pPacket)
{
	CChatServer * app = (CChatServer*) NtlSfxGetApp();
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//if(pHeader->wOpCode != GT_UPDATE_PLAYER_POSITION)
	//	printf("pHeader->wOpCode %d \n", pHeader->wOpCode);

	OpcodeHandler<CServerPassiveSession> const* opHandle = gt_opcodeTable->LookupOpcode(pHeader->wOpCode);
	if (opHandle)
	{
		if (opHandle->packetProcessing == PROCESS_INPLACE)
			(this->*opHandle->handler)(pPacket);
		else
			app->PostClientPacketEvent(new TPacketEventObj<CServerPassiveSession>(this, opHandle->handler, GetHandle(), pPacket, GetUniqueHandle()));
	}
	else
	{
		return CNtlSession::OnDispatch(pPacket);
	}

	return NTL_SUCCESS;
}