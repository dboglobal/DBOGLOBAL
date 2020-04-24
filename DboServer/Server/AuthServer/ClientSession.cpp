#include "stdafx.h"
#include "ClientSession.h"
#include "AuthServer.h"

#include "NtlPacketUA.h"
#include "NtlPacketAU.h"
#include "NtlResultCode.h"


CClientSession::~CClientSession()
{
}


int CClientSession::OnAccept()
{
	this->AccountID = NULL;
	m_byLoginTrys = 0;

	//start handshake (with client)
	//unsigned char buf[] = { 0x03, 0x00, 0xac, 0x86, 0xf5, 0x74 };
	//CNtlPacket packet(buf, 0x06);
	//g_pApp->Send(GetHandle(), &packet);

	return CNtlSession::OnAccept();
}

void CClientSession::OnClose()
{
	CAuthServer * app = (CAuthServer*) NtlSfxGetApp();

	if(this->AccountID != NULL)
		app->DelPlayer(this->AccountID);
}

int CClientSession::OnDispatch(CNtlPacket * pPacket)
{
	CAuthServer * app = (CAuthServer*)NtlSfxGetApp();

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("receive packet %u \n", pHeader->wOpCode);

	switch( pHeader->wOpCode )
	{
		case UA_LOGIN_REQ:				SendCharLogInReq(pPacket, app);	break;
		case UA_LOGIN_DISCONNECT_REQ:	SendLoginDcReq(pPacket, app);	break;

		default:	return CNtlSession::OnDispatch(pPacket);			break;
	}

	return NTL_SUCCESS;
}