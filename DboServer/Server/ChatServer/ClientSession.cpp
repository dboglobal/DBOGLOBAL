#include "stdafx.h"
#include "ClientSession.h"
#include "ChatServer.h"

#include "ut_opcodes.h"
#include "PacketEventObj.h"
#include "NtlPacketUtil.h"


int CClientSession::OnAccept()
{
	cPlayer = NULL;

	m_dwLastMessage = 0;
	m_dwLastGlobalMessage = 0;
	
	return CNtlSession::OnAccept();
}



void CClientSession::OnClose()
{
	//NTL_PRINT(PRINT_APP, "CClientSession::OnClose()");
	cPlayer = NULL;
}

//#include "NtlPacketUtil.h"
int CClientSession::OnDispatch(CNtlPacket * pPacket)
{
	CChatServer * app = (CChatServer*)g_pApp;

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();
	//if (pHeader->wOpCode > 1) NTL_PRINT(PRINT_APP, "%u | received opcode %u [%s]", GetHandle(), pHeader->wOpCode, NtlGetPacketName(pHeader->wOpCode));

	if (cPlayer)
	{
		if (cPlayer->GetClientSessionID() != GetHandle())
		{
			ERR_LOG(LOG_USER, "Clientsesion ID does not match %u != %u", cPlayer->GetClientSessionID(), GetHandle());
			return NTL_FAIL;
		}
	}

	
	OpcodeHandler<CClientSession> const* opHandle = ut_opcodeTable->LookupOpcode(pHeader->wOpCode);
	if (opHandle)
	{
		if (opHandle->packetProcessing == PROCESS_INPLACE)
			(this->*opHandle->handler)(pPacket);
		else
			app->PostClientPacketEvent(new TPacketEventObj<CClientSession>(this, opHandle->handler, GetHandle(), pPacket, GetUniqueHandle()));

		return NTL_SUCCESS;
	}

	return CNtlSession::OnDispatch(pPacket);
}

