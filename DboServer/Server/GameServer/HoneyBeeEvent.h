#ifndef __HONEY_BEE_EVENT_SYSTEM__
#define __HONEY_BEE_EVENT_SYSTEM__

#include "NtlSingleton.h"
#include "NtlSharedType.h"


class CMonster;
class CCharacter;

class CHoneyBeeEvent : public CNtlSingleton<CHoneyBeeEvent>
{

public:

	CHoneyBeeEvent();
	virtual ~CHoneyBeeEvent();

private:

	void				Init();

public:

	void				StartEvent(BYTE byHours = 3);

	void				EndEvent();

	void				LoadEvent(HSESSION hSession);

public:

	void				TickProcess(DWORD dwTick);

	void				Update(CMonster* pMob, CCharacter* pPlayer);

private:

	bool				m_bOn;

	DBOTIME				m_timeStart;

	DBOTIME				m_timeEnd;

	DWORD				m_dwNextUpdateTick;

	int					m_nMonsterSummoned;

};

#define GetHoneyBeeEvent()			CHoneyBeeEvent::GetInstance()
#define g_pHoneyBeeEvent			GetHoneyBeeEvent()

#endif