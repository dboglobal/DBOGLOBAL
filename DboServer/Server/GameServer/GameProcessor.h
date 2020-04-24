#ifndef __DBOG_GAME_PROCESSOR_H__
#define __DBOG_GAME_PROCESSOR_H__


#include "NtlSingleton.h"
#include "EventableObject.h"


class CGameMain;
class CPacketEvent;
class CPacketEventObj;

class CGameProcessor : public CNtlSingleton<CGameProcessor>, public EventableObject
{

public:

	CGameProcessor();
	virtual ~CGameProcessor();

protected:

	void					Init();

public:

	bool					Create(DWORD dwTimeTick, CGameMain* pGameMain);

	void					Run(DWORD dwTickCount);

	DWORD					GetTickCount() { return m_dwTickCount; }

	void					PostClientPacketEvent(CPacketEventObj* pEvent);

public:

	void					StartServerShutdownEvent();
	void					EndServerShutdownEvent();
	void					OnEvent_ServerShutdown();

private:

	EventableObjectHolder * m_pEventHolder;

	CGameMain*				m_pGameMainRef;

	CPacketEvent*			m_pPacketEvent;

	DWORD					m_dwTickCount;
	DWORD					m_dwTickOld;
	DWORD					m_dwTickDiff;
	DWORD					m_dwTickTime;

	DWORD					m_dwLastTimeGameMainUpdated;

	float					m_fMultiple;

	DWORD					m_dwLogTick;

	UINT64					m_nSumEventTick;
};

#define GetCGameProcessor()			CGameProcessor::GetInstance()
#define g_pGameProcessor			GetCGameProcessor()

#endif