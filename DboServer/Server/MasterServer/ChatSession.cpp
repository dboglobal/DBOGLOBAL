#include "stdafx.h"
#include "MasterServer.h"
#include "PacketHead.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//// RECEIVE PACKETS FROM CONNECTED SERVERS
////////////////////////////////////////////////////////////////////////////////////////////////

int CChatServerPassiveSession::OnAccept()
{
	NTL_PRINT(PRINT_APP, "CHAT SERVER CONNECTED");

	return CNtlSession::OnAccept();
}


void CChatServerPassiveSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "CHAT SERVER DISCONNECTED");

	g_pSrvMgr->SetServerOff(NTL_SERVER_TYPE_COMMUNITY, 0, 0, 0);
}


int CChatServerPassiveSession::OnDispatch(CNtlPacket * pPacket)
{
	CMasterServer * app = (CMasterServer*) NtlSfxGetApp();
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("Chat Server | pHeader->wOpCode %d \n", pHeader->wOpCode);
	switch( pHeader->wOpCode )
	{
		case TM_NOTIFY_SERVER_BEGIN: {	Tm_NfyServerBegin(pPacket, app);	}	break;

		case TM_PING_RES:
		{
			sTM_PING_RES * req = (sTM_PING_RES*)pPacket->GetPacketData();

			ResetAliveTime();
		
			DWORD dwTick = GetTickCount();
			if (dwTick - req->dwTick > 10)
				ERR_LOG(LOG_NETWORK, "Chat Server Ping: %d ", dwTick - req->dwTick);
		}
		break;

		default: ERR_LOG(LOG_NETWORK, "Chat: Undefined Packet. wOpCode = %u", pHeader->wOpCode); break;
	}
	

	return NTL_SUCCESS;
}