#ifndef __SERVER_FREEBATTLE_MANAGER__
#define __SERVER_FREEBATTLE_MANAGER__

#include "NtlVector.h"
#include "NtlSingleton.h"
#include <vector>

class CPlayer;


struct sFreeBattle
{
	CHARACTERID		uiInvitor;
	CHARACTERID		uiTarget;
	sVECTOR3		vRefereeLoc;
	QWORD			dwStartTick;
	bool			bIsStart;
};


class CFreeBattleManager : public CNtlSingleton<CFreeBattleManager>
{

public:
	CFreeBattleManager();
	virtual ~CFreeBattleManager();


private:

	DWORD										m_dwFreeBattleID;
	std::vector<DWORD>							m_vFreeBattleIDVec;

	std::map<DWORD, sFreeBattle*>				m_map_Freebattle;

public:

	void			CreateFreeBattle(CPlayer* invitor, CPlayer* target);
	void			DeleteFreeBattle(DWORD dwFreeBattleID);

	bool			BeginFreeBattle(DWORD dwFreeBattleID);
	void			EndFreeBattle(DWORD dwFreeBattleID, CHARACTERID uiWinner);

	sFreeBattle*	ReturnFreeBattle(DWORD dwFreeBattleID);

	bool			IsOutsideFreebattle(CPlayer* player);
};

#define GetFreeBattleManager()			CFreeBattleManager::GetInstance()
#define g_pFreeBattleManager			GetFreeBattleManager()

#endif