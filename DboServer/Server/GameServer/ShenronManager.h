#ifndef __SERVER_SHENRON_MANAGER__
#define __SERVER_SHENRON_MANAGER__

#include "NtlSingleton.h"
#include "DragonBallTable.h"

#include <set>

class CPlayer;

class CShenronManager : public CNtlSingleton<CShenronManager>
{

public:
	CShenronManager();
	virtual ~CShenronManager();

public:

	void			TickProcess(DWORD dwTickDiff, float fMultiple);

	void			SpawnShenron(BYTE byBallType, HOBJECT hAltar, CPlayer* pkSummoner, sITEM_POSITION_DATA* sData);
	void			DespawnShenron(CPlayer* pkSummoner, bool bRemoveBalls = true);

	BYTE			GetBallType(CHARACTERID charid);

	bool			CanSummonShenron(HOBJECT hAltar);

private:

	DWORD			dwLastShenronLoop;

	std::map<HOBJECT, HOBJECT>			m_mapUsedAltar; //HOBJECT 1 = Altar handle 2 = summoner handle
	std::map<HOBJECT, sCOLLECT_BRIEF>	m_map_Shenron; // HOBJECT = altar object handle

};
#define GetShenronManager()			CShenronManager::GetInstance()
#define g_pShenronManager			GetShenronManager()

#endif