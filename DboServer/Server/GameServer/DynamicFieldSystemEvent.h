#ifndef __SERVERSCRIPT_DYNAMIC_FIELD_EVENT_SYSTEM__
#define __SERVERSCRIPT_DYNAMIC_FIELD_EVENT_SYSTEM__

#include "NtlSingleton.h"
#include "DynamicFieldSystemTable.h"


class CMonster;
class CCharacter;

class CDynamicFieldSystemEvent : public CNtlSingleton<CDynamicFieldSystemEvent>
{

public:

	CDynamicFieldSystemEvent();
	virtual ~CDynamicFieldSystemEvent();

private:

	void				Init();

	void				Destroy();

public:

	void				Create();

public:

	void				SetCount(DWORD dwCount) { m_dwCurCount = dwCount; }

	void				TickProcess(DWORD dwTickDiff);

	void				LoadDynamicField(CCharacter* pPlayer);

	void				UpdateDynamicFieldCount(DWORD dwCount);

	void				Update(CMonster* pMob, CCharacter* pPlayer); //called when killed a monster

	void				GetBosses(CCharacter* pPlayer);


private:

	int					m_nAddCount;
	BYTE				m_byBossCount;
	DWORD				m_dwCurCount;
	DWORD				m_dwMaxCount;

	DBOTIME				m_wpsEndTime;
	

	DWORD				m_dwNextUpdate;

};

#define GetDynamicFieldSystemEvent()			CDynamicFieldSystemEvent::GetInstance()
#define g_pDynamicFieldSystemEvent				GetDynamicFieldSystemEvent()

#endif