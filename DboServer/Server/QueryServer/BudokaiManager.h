#ifndef __QUERY_BUDOKAI_MANAGER__
#define __QUERY_BUDOKAI_MANAGER__

#include "NtlSingleton.h"
#include <map>
#include <set>
#include "NtlBudokai.h"

class CBudokaiManager : public CNtlSingleton<CBudokaiManager>
{

public:

	CBudokaiManager();
	virtual ~CBudokaiManager();

private:

	void								Init();

	void								Destroy();

public:

	void								SendInitNfy(HSESSION hSession);

	void								UpdateState(sBUDOKAI_STATE_DATA& stateData);
	void								UpdateMatchState(BYTE byMatchType, sBUDOKAI_MATCHSTATE_DATA& stateData);

	void								JoinIndividual(HOBJECT handle, CHARACTERID charId, float fPoint, HSESSION hSession);
	void								LeaveIndividual(HOBJECT handle, CHARACTERID charId, HSESSION hSession);

	void								TournamentIndividualAddEntry(JOINID wJoinId, BYTE byMatchIndex, BYTE byWinnerJoinResult, BYTE byLoserResultCondition, BYTE byLoserJoinState);

	void								JoinTeam(HOBJECT handle, CHARACTERID charId, WCHAR* wszTeamName, BYTE byMemberCount, CHARACTERID* aMembers, float fPoint, sBUDOKAI_TEAM_POINT_INFO* aTeamInfo, HSESSION hSession);
	void								LeaveLeam(HOBJECT handle, CHARACTERID charId, HSESSION hSession);

	void								TournamentTeamAddEntry(JOINID wJoinId, BYTE byMatchIndex, BYTE byWinnerJoinResult, BYTE byLoserResultCondition, BYTE byLoserJoinState);

	void								HistoryWrite(BYTE byBudokaiType, BYTE byMatchType);

	void								Clear();

private:

	JOINID								GenerateJoinId();
	void								EraseJoinId(JOINID wId);

private:

	sBUDOKAI_UPDATEDB_INITIALIZE		m_sBudokai;

	JOINID								m_wJoinId;
	std::vector<JOINID>					m_vevJoinId;


	std::multimap<JOINID, CHARACTERID>			m_mapJoinID;

	std::map<JOINID, sBUDOKAI_JOIN_INFO>		m_mapJoinInfo; //first = wJoinId

	std::map<TBLIDX, CHARACTERID>				m_mapDojoSeed; //list of dojo which already seeded

};

#define GetBudokaiManager()				CBudokaiManager::GetInstance()
#define g_pBudokaiManager				GetBudokaiManager()

#endif