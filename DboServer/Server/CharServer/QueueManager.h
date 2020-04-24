#pragma once

#include "NtlSharedType.h"
#include "NtlSingleton.h"
#include "NtlSharedDef.h"
#include <mutex>

#define USE_LOGIN_QUEUE_SYSTEM


class CClientSession;
class CPlayer;

class CQueueManager : public CNtlSingleton<CQueueManager>
{

	struct sCHAR_LOGIN_QUEUE
	{
		ACCOUNTID	accountId;
		DWORD		dwQueueTimeLeft;	// Time how long should stay in queue. Default 500ms
		DWORD		dwLastQueueUpdate;	// Countdown for updating queue position. Default 1000ms
	};

public:

	CQueueManager();
	virtual ~CQueueManager();

public:

	void	TickProcess(DWORD dwTickTime);

public:

	void	InsertIntoQueue(CPlayer* pPlayer);
	void	RemoveFromQueue(ACCOUNTID accountId);


private:

	std::map<ACCOUNTID, sCHAR_LOGIN_QUEUE>		m_mapQueue;

	std::mutex									m_mutex;

};
#define GetQueueManager()				CQueueManager::GetInstance()
#define g_pQueueManager					GetQueueManager()