#include "stdafx.h"
#include "CharServerSession.h"
#include "QueryServer.h"
#include "NtlPacketCQ.h"


int CCharServerSession::OnAccept()
{
	NTL_PRINT(PRINT_APP, "CHAR SERVER CONNECTED \n");

	return CNtlSession::OnAccept();
}


void CCharServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "CHAR SERVER DISCONNECTED \n");
}


int CCharServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CQueryServer * app = (CQueryServer*)g_pApp;

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//ERR_LOG(LOG_GENERAL, "Receive packet %u", pHeader->wOpCode);

	switch (pHeader->wOpCode)
	{
		case CQ_CHARACTER_INFO_REQ: RecvCharacterInfoReq(pPacket, app);		break;
		case CQ_CHARACTER_ADD_REQ:	RecvCreateCharReq(pPacket, app);		break;
		case CQ_CHARACTER_DEL_REQ: RecvCharacterDelReq(pPacket, app);	break;
		case CQ_CHARACTER_LOAD_REQ:	RecvCharacterLoadReq(pPacket, app);		break;

		default: ERR_LOG(LOG_NETWORK, "Received unknown CQ packet. opcode %d", pHeader->wOpCode); break;
	}

	return NTL_SUCCESS;
}