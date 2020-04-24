#include "stdafx.h"
#include "QueryServerSession.h"
#include "CharServer.h"
#include "qc_opcodes.h"
#include "PacketEventObj.h"


int CQueryServerSession::OnConnect()
{
	NTL_PRINT(PRINT_APP, "CONNECTED TO QUERY SERVER");
	CCharServer * app = (CCharServer*)g_pApp;
	app->SetQueryServerSession(this);

	return CNtlSession::OnConnect();
}


void CQueryServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "DISCONNECTED FROM QUERY SERVER");
}


int CQueryServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CCharServer * app = (CCharServer*)g_pApp;

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();


	OpcodeHandler<CQueryServerSession> const* opHandle = qc_opcodeTable->LookupOpcode(pHeader->wOpCode);
	if (opHandle)
	{
		if (opHandle->packetProcessing == PROCESS_INPLACE)
			(this->*opHandle->handler)(pPacket);
		else
			app->PostClientPacketEvent(new TPacketEventObj<CQueryServerSession>(this, opHandle->handler, GetHandle(), pPacket, GetUniqueHandle()));
	}
	else
	{
		return CNtlSession::OnDispatch(pPacket);
	}

	return NTL_SUCCESS;
}