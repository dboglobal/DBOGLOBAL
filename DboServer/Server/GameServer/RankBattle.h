#ifndef __SERVER_RANKBATTLE_MANAGER__
#define __SERVER_RANKBATTLE_MANAGER__


#include "NtlSingleton.h"
#include "RankBattleTable.h"

#include <unordered_map>

class CPlayer;
class CWorld;
class CParty;

/* INFO:
	Owner = Team Blue
	Challenger = Team Red
*/

struct sRANK_SOLO
{
	CPlayer*		pOwner;
	CPlayer*		pChallenger;
};

struct sRANK_PARTY
{
	CParty*		pOwner;
	CParty*		pChallenger;
};

//CRankbattleBattle is used for the actual battle
class CRankbattleBattle
{

public:

	CRankbattleBattle(ROOMID roomid, sRANKBATTLE_TBLDAT* pRankTblData);
	virtual ~CRankbattleBattle();

public:

	bool							TickProcess(DWORD dwTickCount); //return true if battle finished and can be deleted

	void							UpdateBattleStateNfy(eRANKBATTLE_BATTLESTATE eBattleState, BYTE byStage);

	void							AddPlayer(CPlayer* pPlayer, eRANKBATTLE_TEAM_TYPE eTeamType);
	void							AddParty(CParty* pParty, eRANKBATTLE_TEAM_TYPE eTeamType);

	void							EnterBattle(CPlayer* pPlayer); //Used when player enter battle

	void							Cancel(CPlayer* pPlayer); //Used when player cancel teleportation

	void							CheckBattleBegin(DWORD dwTickCount); //wait until all player entered

	void							SendTeamInfo();

	void							UpdatePlayerState(CPlayer* pPlayer, BYTE byState);

	void							ResetPlayers(); //reset players life, buffs, etc

	void							UpdatePlayersAttackable();

	void							CheckArena(); //Checks player left arena

	void							CheckStage();

	void							TimeOut();

	void							UpdateStageWinner(eRANKBATTLE_MATCH_RESULT eMatchResult);

	void							DecideMatchWinner();

	void							TeleportAllOut();

	bool							CheckStageWinner();

	void							LeaveBattle(CPlayer* pPlayer); //when player logout etc while battle

	bool							IsEveryoneOut() { return m_byOwnerCount == 0 && m_byChallengerCount == 0; } //check if all teams left the match. Only use after RANKBATTLE_ROOMSTATE_WAIT

	bool							IsOneTeamOut() { return m_byOwnerCount == 0 || m_byChallengerCount == 0; } //same as "IsEveryoneOut" just checks if one team left.

	void							CancelMatch();

	bool							IsEnd() { return m_bEnd; }
	bool							IsCancel() { return m_bCancelMatch; }

private:

	ROOMID							m_roomId;

	eRANKBATTLE_ROOMSTATE			m_eRoomState;

	eRANKBATTLE_BATTLESTATE			m_BattleState;

	BYTE							m_byStage;

	sRANKBATTLE_TBLDAT*				m_pRankTblData;

	std::map<HOBJECT, CHARACTERID>	m_mapPlayers;

	BYTE							m_byOwnerCount;		//team blue - amount of players inside battle
	BYTE							m_byChallengerCount;//team red - amount of players inside battle
	BYTE							m_byInviteOwnerCount;		//team blue - amount of players still have teleport invite
	BYTE							m_byInviteChallengerCount;	//team red - amount of players still have teleport invite
	BYTE							m_byTotalOwnerCount;		//team blue - amount of players received teleport invite
	BYTE							m_byTotalChallengerCount;	//team red - amount of players received teleport invite

	BYTE							m_byOwnerWins;		//team blue - amount of wins
	BYTE							m_byChallengerWins;	//team red - amount of wins

	BYTE							m_byUnavailableOwnerCount;		//read blue - amount of players which are dead/out of arena..
	BYTE							m_byUnavailableChallengerCount; //read red - amount of players which are dead/out of arena..

	BYTE							m_byOwnerKillScore;
	BYTE							m_byChallengerKillScore;
	BYTE							m_byTotalOwnerKillScore;
	BYTE							m_byTotalChallengerKillScore;

	WCHAR							m_wchOwnerPartyName[NTL_MAX_SIZE_PARTY_NAME + 1];
	WCHAR							m_wchChallengerPartyName[NTL_MAX_SIZE_PARTY_NAME + 1];

	CWorld*							m_pWorld;

	DWORD							m_dwWaitTime;			// 
	DWORD							m_dwDirectionTime;		// 
	DWORD							m_dwMatchReadyTime;		// 
	DWORD							m_dwStageReadyTime;		// Stage preparation time
	DWORD							m_dwStageRunTime;		// 
	DWORD							m_dwStageFinishTime;	// 
	DWORD							m_dwMatchFinishTime;	// 
	DWORD							m_dwBossDirectionTime;	// 
	DWORD							m_dwBossKillTime;		// 
	DWORD							m_dwBossEndingTime;		// 
	DWORD							m_dwEndTime;			// 

	bool							m_bEnd;
	bool							m_bCancelMatch;
};

//CRankbattleRoom is used to apply join room and queue finding match
class CRankbattleRoom
{

public:

	CRankbattleRoom(sRANKBATTLE_TBLDAT* tbldat);
	virtual ~CRankbattleRoom();


public:

	void					TickProcess(DWORD dwTickCount);

	CRankbattleBattle*		GetBattle(ROOMID roomid);

private:

	WORD					m_wRegisterCount;

	sRANKBATTLE_TBLDAT*		m_pRankTblData;

public:

	WORD					GetRegisterCount() { return m_wRegisterCount; }

	TBLIDX					GetTblidx() { return m_pRankTblData->tblidx; }
	BYTE					GetBattleMode() {	return m_pRankTblData->byBattleMode;	}
	BYTE					GetRequiredMinLevel() { return 29;/* m_pRankTblData->byMinLevel;*/ }
	BYTE					GetRequiredMaxLevel() { return m_pRankTblData->byMaxLevel; }

public:

	bool					JoinQueue(CPlayer* pPlayer, HOBJECT hObject); //return false when room is full

	void					LeaveQueue(CPlayer* pPlayer);
	void					LeaveQueue(PARTYID partyId);

	void					UpdateMatchmakingQueue();

private:

	std::map<ROOMID, CRankbattleBattle*>	m_mapRankBattleBattle;

	std::unordered_map<HOBJECT, CHARACTERID>	m_mapSoloQueue;

	std::unordered_map<HOBJECT, CParty*>	m_mapPartyQueue; //hobject = party leader

};



//// Manager
class CRankbattle : public CNtlSingleton<CRankbattle>
{

public:

	CRankbattle();
	virtual ~CRankbattle();


public:

	void								Destroy();

protected:

	void								Init();

public:

	void								TickProcess(DWORD dwTickCount);

public:

	void								LoadRankBattleInfo(CPlayer* pPlayer, BYTE byBattleMode); // eRANKBATTLE_MODE

	void								JoinRoom(CPlayer* pPlayer, TBLIDX roomTblidx, HOBJECT hBoardObject);

	void								LeaveRoom(CPlayer* pPlayer, TBLIDX roomTblidx, bool bIsLogout = false);
	void								LeaveRoom(PARTYID partyId, TBLIDX roomTblidx);

	void								EnterBattle(CPlayer* pPlayer);

	void								CancelTeleport(CPlayer* pPlayer);

	void								LeaveBattle(CPlayer* pPlayer);

	bool								DoesBattleStillExist(TBLIDX battleTblidx, ROOMID roomid);

	void								UpdatePlayerState(TBLIDX battleTblidx, ROOMID roomid, CPlayer* pPlayer, BYTE byState);

	ROOMID								AcquireRoomID() { m_BattleRoomId = UnsignedSafeIncrease<ROOMID>(m_BattleRoomId + 1, 1); return m_BattleRoomId; }

private:

	std::map<TBLIDX, CRankbattleRoom*>	m_mapRankBattleRoom;

	ROOMID								m_BattleRoomId;

	DWORD								m_dwMatchMakingCycle;
	
};

#define GetRankbattleManager()			CRankbattle::GetInstance()
#define g_pRankbattleManager			GetRankbattleManager()

#endif