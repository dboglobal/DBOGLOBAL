#include "stdafx.h"
#include "CharServer.h"
#include "NtlPacketCU.h"
#include "Utils.h"
#include "Player.h"


CQueueManager::CQueueManager()
{
}

CQueueManager::~CQueueManager()
{
}


void CQueueManager::TickProcess(DWORD dwTickTime)
{
	m_mutex.lock();

	DWORD dwQueuePos = 0;

	for (std::map<ACCOUNTID, sCHAR_LOGIN_QUEUE>::iterator it = m_mapQueue.begin(); it != m_mapQueue.end(); )
	{
		dwQueuePos += 1;

		sCHAR_LOGIN_QUEUE* pQueue = &it->second;

		CPlayer* pPlayer = g_PlrMgr->GetPlayer(pQueue->accountId);
		if (pPlayer == NULL || pPlayer->GetSession() == NULL)
		{
			it = m_mapQueue.erase(it);
			dwQueuePos -= 1;
			continue;
		}
		
		if (pPlayer->GetServerFarmID() >= INVALID_SERVERFARMID || pPlayer->GetServerChannelID() >= INVALID_SERVERCHANNELID)
		{
			CNtlPacket packet(sizeof(sCU_CONNECT_WAIT_CHECK_RES));
			sCU_CONNECT_WAIT_CHECK_RES * res = (sCU_CONNECT_WAIT_CHECK_RES *)packet.GetPacketData();
			res->wOpCode = CU_CONNECT_WAIT_CHECK_RES;
			res->wResultCode = 215;	//CHARACTER_WRONG_SERVER_FARM_SELECTED
			packet.SetPacketLen( sizeof(sCU_CONNECT_WAIT_CHECK_RES) );
			g_pApp->Send(pPlayer->GetSessionHandle(), &packet);

			pPlayer->SetQueue(false);

			it = m_mapQueue.erase(it);
			dwQueuePos -= 1;
			continue;
		}
		
		pQueue->dwQueueTimeLeft = UnsignedSafeDecrease<DWORD>(pQueue->dwQueueTimeLeft, dwTickTime);

		if(pQueue->dwQueueTimeLeft == 0 && dwQueuePos == 1)
		{
			WORD resultcode = 200; //CHARACTER_SUCCESS
						
			sSERVER_CHANNEL_INFO* pChannelInfo = g_pServerInfoManager->GetServerChannelInfo(pPlayer->GetServerFarmID(), pPlayer->GetServerChannelID());
			if(pChannelInfo)
			{
				if(pChannelInfo->byServerStatus != DBO_SERVER_STATUS_UP)
					resultcode = 506; // GAME_SERVER_LOCKED
				else if (pChannelInfo->dwLoad >= pChannelInfo->dwMaxLoad)
					resultcode = 593; //GAME_CANNOT_CONNECT_TARGET_CHANNEL_FULL
			}
			else resultcode = 506; // GAME_SERVER_LOCKED

			if (resultcode == 200)
			{
				CNtlPacket packet(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
				sCU_CONNECT_WAIT_COUNT_NFY * res = (sCU_CONNECT_WAIT_COUNT_NFY *)packet.GetPacketData();
				res->wOpCode = CU_CONNECT_WAIT_COUNT_NFY;
				res->dwCountWaiting = 0;
				packet.SetPacketLen(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
				g_pApp->Send(pPlayer->GetSessionHandle(), &packet);
			}
			else
			{
				CNtlPacket packet(sizeof(sCU_CONNECT_WAIT_CHECK_RES));
				sCU_CONNECT_WAIT_CHECK_RES * res = (sCU_CONNECT_WAIT_CHECK_RES *)packet.GetPacketData();
				res->wOpCode = CU_CONNECT_WAIT_CHECK_RES;
				res->wResultCode = resultcode;
				packet.SetPacketLen(sizeof(sCU_CONNECT_WAIT_CHECK_RES));
				g_pApp->Send(pPlayer->GetSessionHandle(), &packet);
			}

			ERR_LOG(LOG_SYSTEM, "<QUEUE>User %u finished queue. Target Server %u, Channel %u, Result %u", pQueue->accountId, pPlayer->GetServerFarmID(), pPlayer->GetServerChannelID(), resultcode);

			pPlayer->SetQueue(false);

			it = m_mapQueue.erase(it);
			dwQueuePos -= 1;
			continue;
		}
		else // else update his queue place
		{
			if (dwQueuePos > 1) //if not first in queue then reset time
				pQueue->dwQueueTimeLeft = 1000;

			pQueue->dwLastQueueUpdate = UnsignedSafeDecrease<DWORD>(pQueue->dwLastQueueUpdate, dwTickTime);

			if (pQueue->dwLastQueueUpdate == 0)
			{
				CNtlPacket packet(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
				sCU_CONNECT_WAIT_COUNT_NFY * res = (sCU_CONNECT_WAIT_COUNT_NFY *)packet.GetPacketData();
				res->wOpCode = CU_CONNECT_WAIT_COUNT_NFY;
				res->dwCountWaiting = dwQueuePos;
				packet.SetPacketLen(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
				g_pApp->Send(pPlayer->GetSessionHandle(), &packet);

				pQueue->dwLastQueueUpdate = 1000; //update again in one second
			}

		}

		++it;
	}

	m_mutex.unlock();
}

void CQueueManager::InsertIntoQueue(CPlayer* pPlayer)
{
	m_mutex.lock();

	sCHAR_LOGIN_QUEUE queue;
	queue.accountId = pPlayer->GetAccountID();
	queue.dwLastQueueUpdate = 1000;
	queue.dwQueueTimeLeft = 1000;

	m_mapQueue.insert(std::make_pair(queue.accountId, queue));

	m_mutex.unlock();
}

void CQueueManager::RemoveFromQueue(ACCOUNTID accountId) 
{
	m_mutex.lock();

	m_mapQueue.erase(accountId);

	m_mutex.unlock();
}

