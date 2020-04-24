#ifndef __SERVER_RANKBATTLE_MANAGER__
#define __SERVER_RANKBATTLE_MANAGER__


#include "NtlSingleton.h"
#include "CallBack.h"
#include "NtlRankBattle.h"
#include <unordered_set>


class CPlayer;

class CRankBattle : public CNtlSingleton<CRankBattle>
{

public:

	CRankBattle();
	virtual ~CRankBattle();

private:

	void			Init();
	void			Destroy();


public:

	void			LoadRankBattleList(HSESSION hSession, DWORD dwPage, BYTE byCompareDay);
	void			OnLoadRankBattleList(QueryResultVector & results, HSESSION hSession, DWORD dwPage, BYTE byCompareDay);

	void			LoadRankBattleListFindName(HSESSION hSession, WCHAR* wchCharName, DWORD dwPage, BYTE byCompareDay);
	void			OnLoadRankBattleListFindName(QueryResultVector & results, HSESSION hSession, DWORD dwPage, BYTE byCompareDay, WCHAR * wchName);

};

#define GetRankBattle()		CRankBattle::GetInstance()
#define g_pRankBattle		GetRankBattle()



#endif