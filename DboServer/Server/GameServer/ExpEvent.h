#ifndef __EXP_EVENT_SYSTEM__
#define __EXP_EVENT_SYSTEM__

#include "NtlSingleton.h"
#include "NtlSharedType.h"

class CGameObject;

class CExpEvent : public CNtlSingleton<CExpEvent>
{

public:

	CExpEvent();
	virtual ~CExpEvent();

private:

	void				Init();

	void				Destroy();

public:

	void				Create();

public:

	void				TickProcess(DWORD dwTickDiff);

	void				StartEvent(bool bStartByCommand = true);

	void				EndEvent();

public:

	bool				IsOn() { return m_bOnOff; }

	void				LoadEvent(CGameObject* pChar);

	float				GetExpBonus();

private:

	const int			EXP_BONUS = 50;

	bool				m_bOnOff;

	DWORD				m_dwWaitTickCount;
};

#define GetExpEvent()			CExpEvent::GetInstance()
#define g_pExpEvent				GetExpEvent()

#endif