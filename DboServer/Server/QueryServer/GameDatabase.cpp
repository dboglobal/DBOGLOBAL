#include "stdafx.h"
#include "QueryServer.h"
#include "GameDatabase.h"
#include "Servermanager.h"
#include "NtlPacketGQ.h"


CDatabaseGame::CDatabaseGame()
{
	m_set_Packet.clear();

	pThread = CNtlThreadFactory::CreateThread(this, "GameDBQuery");
	pThread->Start();
}

CDatabaseGame::~CDatabaseGame()
{

}

void CDatabaseGame::AddQuery(CNtlPacket * pPacket)
{
	CNtlPacket* packet = new CNtlPacket;
	packet->SetPacket(pPacket->GetPacketData(), pPacket->GetPacketDataSize());

	m_set_Packet.push_back(packet);
}


void CDatabaseGame::Run()
{
	DWORD dwThreadStartProcess = GetTickCount();

	while (IsRunnable())
	{
		dwThreadStartProcess = GetTickCount();

		for (std::vector<CNtlPacket*>::iterator it = m_set_Packet.begin(); it != m_set_Packet.end(); it++)
		{
			CNtlPacket* packet = *it;
			sNTLPACKETHEADERtwo * pHeader = (sNTLPACKETHEADERtwo *)packet->GetPacketData();

			switch (pHeader->wOpCode)
			{
			

			default:{ ERR_LOG(LOG_NETWORK, "Could not find GQ packet %d", pHeader->wOpCode); } break;
			}

			delete packet;
		}

		m_set_Packet.clear();

		if (GetTickCount() - dwThreadStartProcess > 500)
			ERR_LOG(LOG_GENERAL, "Game Database thread needed longer than 500 MS to process");

		Wait(1);
	}
}