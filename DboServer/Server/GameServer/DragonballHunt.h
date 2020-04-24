#ifndef __DRAGONBALL_HUNT_EVENT_SYSTEM__
#define __DRAGONBALL_HUNT_EVENT_SYSTEM__

#include "NtlSingleton.h"
#include "NtlDragonBall.h"


class CGameObject;

class CDragonballHunt : public CNtlSingleton<CDragonballHunt>
{

public:

	CDragonballHunt();
	virtual ~CDragonballHunt();

private:

	void				Init();

	void				Destroy();

public:

	void				Create();

public:

	void				TickProcess(DWORD dwTick);

	void				StartEvent(bool bStartByCommand = true);

	void				EndEvent();

	void				IncDragonballDrop() { ++m_dwDragonballDropCount; }

	inline void			InsertMob(HOBJECT hId) { m_setMonsters.insert(hId); }
	inline void			EraseMob(HOBJECT hId) { m_setMonsters.erase(hId); }

public:

	bool				IsOn() { return m_bOnOff; }

	void				LoadEvent(CGameObject* pChar);


private:

	bool				m_bOnOff;

	DBOTIME				m_timeStart;

	DBOTIME				m_timeEnd;

	DWORD				m_dwNextUpdateTick;

	DWORD				m_dwDragonballDropCount;

	std::set<HOBJECT>	m_setMonsters;
};

#define GetDragonballHuntEvent()			CDragonballHunt::GetInstance()
#define g_pDragonballHuntEvent				GetDragonballHuntEvent()

#endif