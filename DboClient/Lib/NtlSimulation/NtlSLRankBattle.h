/*****************************************************************************
*
* File			: NtlRankBattle.h
* Author		: Pee Sangjun
* Copyright		: (주)NTL
* Date			: 2007. 8. 10	
* Abstract		: Simulation Rank Battle
*****************************************************************************
* Desc          : 랭크 배틀.
*****************************************************************************/

#ifndef __NTL_RANK_BATTLE_H__
#define __NTL_RANK_BATTLE_H__

#include "ceventhandler.h"
#include "RankBattleTable.h"

class CNtlSobActor;

class CNtlRankBattle : public RWS::CEventHandler
{
public:
	// !type
	struct stRankBattleMember
	{
		stRankBattleMember(VOID)
		{
			hSerial = INVALID_DWORD;
			byState = INVALID_BYTE;
			byClass = INVALID_BYTE;
		}

		RwUInt32		hSerial;
		RwUInt8			byState;
		RwUInt8			byClass;
		std::wstring	wstrName;
	};

	typedef std::map<RwUInt32,stRankBattleMember>				MAP_RBMEMBER;
	typedef std::map<RwUInt32,stRankBattleMember>::iterator		MAP_RBMEMBER_ITER;

	enum RESULT { NONE, WIN, LOSE, DRAW };	
	enum REQUEST_STATE { RB_REQUEST_STATE_NONE = 0,
						 RB_REQUEST_STATE_REQUEST,
						 RB_REQUEST_STATE_WAIT_MATCH,
						 RB_REQUEST_STATE_IN_MATCH };

public:
	CNtlRankBattle(void);
	virtual ~CNtlRankBattle(void);

	void	Init(void);
	void	ResetMatchData(void);
	virtual RwBool	Create(void);		
	void	Destroy(void);

	void	HandleEvents(RWS::CMsg& msg);
		
	RwBool	IsAttackableEnemy(RwUInt32 hSerialId);
	CNtlSobActor* GetNearAttackableEnemy(CNtlSobActor* pActor);

	sRANKBATTLE_TBLDAT* GetTblData(void) { return m_pTblDat; }
	void		SetTblData( RwUInt32 tblidx );
	
	RwBool	IsPartyBattle(VOID);

	// State
	void		SetRBRequestState( RwUInt32 eRBRequestState ) { m_eRequestState = eRBRequestState; }
	RwUInt32	GetRBRequestState(VOID) { return m_eRequestState; }

	RwBool		IsEnableRBBoard(VOID);	

	RwInt32		GetMyTeamWinScore(VOID) { return m_nMyTeamWinScore; }
	RwInt32		GetEnemyTeamWinScore(VOID) { return m_nEnemyTeamWinScore; }

	RwInt32		GetBlueTeamStraightWinScore(VOID) { return m_nBlueTeamStraightWinScore; }
	RwInt32		GetBlueTeamPerfectWinScore(VOID) { return m_nBlueTeamPerfectWinScore; }

	RESULT		GetResult(void) { return m_eResult; }						// 게임 결과
	RwBool		IsRoomOwner(void) { return m_bRoomOwner; }					// 방 소유
	RwBool		IsRedTeam(VOID) { return IsRoomOwner() ? FALSE : TRUE; }
	RwBool		IsBlueTeam(VOID) { return IsRoomOwner() ? TRUE : FALSE; }
	RwBool		IsRedTeamWin(VOID) { return ( IsRedTeam() && m_eResult == WIN ) ? TRUE : FALSE; }
	RwBool		IsBlueTeamWin(VOID) { return ( IsBlueTeam() && m_eResult == WIN ) ? TRUE : FALSE; }
	
	stRankBattleMember* FindMemberData(RwUInt32 hSerialID);
	MAP_RBMEMBER*		GetMyTeamMap(VOID) { return &m_mapMyTeam; }
	MAP_RBMEMBER*		GetEnemyTeamMap(VOID) { return &m_mapEnemyTeam; }

	MAP_RBMEMBER*		GetRedTeamMap(VOID) { return IsRoomOwner() ? &m_mapEnemyTeam : &m_mapMyTeam; }		
	MAP_RBMEMBER*		GetBlueTeamMap(VOID) { return IsRoomOwner() ? &m_mapMyTeam : &m_mapEnemyTeam; }

	std::wstring		GetMyTeamName(VOID) { return m_MyTeamName; }
	std::wstring		GetEnemyTeamName(VOID) { return m_EnemyTeamName; }

	std::wstring		GetRedTeamName(VOID) { return IsRoomOwner() ? m_EnemyTeamName : m_MyTeamName; }
	std::wstring		GetBlueTeamName(VOID) { return IsRoomOwner() ? m_MyTeamName : m_EnemyTeamName; }

	sRANKBATTLE_SCORE_INFO* GetRankBattleScore(VOID) { return &m_myRankScore; }
	
protected:
	MAP_RBMEMBER	m_mapMyTeam;
	MAP_RBMEMBER	m_mapEnemyTeam;

	std::wstring	m_MyTeamName;					// 개인전인 경우 캐릭터이름
	std::wstring	m_EnemyTeamName;
	
	RwBool			m_bRoomOwner;					
	RESULT			m_eResult;

	RwInt32			m_nMyTeamWinScore;				// 현재 게임내에서의 스코어
	RwInt32			m_nEnemyTeamWinScore;

	RwInt32			m_nBlueTeamStraightWinScore;	// 방주인 연승정보
	RwInt32			m_nBlueTeamPerfectWinScore;		// 방주인 완승정보

	RwUInt32		m_eRequestState;				// REQUEST_STATE

	sRANKBATTLE_SCORE_INFO		m_myRankScore;
	sRANKBATTLE_TBLDAT*			m_pTblDat;				
};

#endif// __NTL_RANK_BATTLE_H__
