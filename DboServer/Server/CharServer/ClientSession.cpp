#include "stdafx.h"
#include "ClientSession.h"
#include "CharServer.h"
#include "NtlPacketCM.h"
#include "NtlResultCode.h"
#include "uc_opcodes.h"
#include "Player.h"
#include "PacketEventObj.h"



CClientSession::~CClientSession()
{
}


int CClientSession::OnAccept()
{
	eUserState = NTL_USER_STATE_NONE;
	m_pPlayer = NULL;

	//start handshake (with client)
	//unsigned char buf[] = { 0x03, 0x00, 0xac, 0x86, 0xf5, 0x74 };
	//CNtlPacket packet(buf, 0x06);
	//g_pApp->Send(GetHandle(), &packet);

	return CNtlSession::OnAccept();
}


void CClientSession::OnClose()
{
	CCharServer* app = (CCharServer*)g_pApp;

	//send to master server remove player from char list
	if(m_pPlayer)
	{
		//check if in login queue
#ifdef USE_LOGIN_QUEUE_SYSTEM
		if (m_pPlayer->CheckQueue())
		{
			g_pQueueManager->RemoveFromQueue(m_pPlayer->GetAccountID());
			m_pPlayer->SetQueue(false);
		}
#endif

		CNtlPacket packet(sizeof(sCM_LOGOUT_REQ));
		sCM_LOGOUT_REQ * res = (sCM_LOGOUT_REQ *)packet.GetPacketData();
		res->wOpCode = CM_LOGOUT_REQ;
		res->accountId = m_pPlayer->GetAccountID();
		app->SendTo(app->m_pMasterServerSession, &packet);

		m_pPlayer->SetSession(NULL);
		m_pPlayer->SetSessionHandle(INVALID_HSESSION);

		g_PlrMgr->RemovePlayer(m_pPlayer);

		m_pPlayer = NULL;
	}
}


int CClientSession::OnDispatch(CNtlPacket * pPacket)
{
	CCharServer * app = (CCharServer*)g_pApp;

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	OpcodeHandler<CClientSession> const* opHandle = uc_opcodeTable->LookupOpcode(pHeader->wOpCode);
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

