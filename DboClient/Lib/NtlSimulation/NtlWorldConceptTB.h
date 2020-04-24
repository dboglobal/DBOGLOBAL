#pragma once

#include "NtlWorldConceptController.h"
#include "NtlMatch.h"
#include "NtlBudokai.h"
#include "RankBattleTable.h"

/**
* \ingroup NtlSimulation
* \brief 천하제일 무도회, 전투 상태를 제어.
* \date 2008-05-09
* \author peessi
*/

//#define BUDOKAI_TIME_DEBUG

struct stTBudokaiTeam;

struct stTBudokaiMember
{
	stTBudokaiMember(VOID)
	{
		hSerial = INVALID_SERIAL_ID;
		byLevel = INVALID_BYTE;
		byState = INVALID_BYTE;
		byClass = INVALID_BYTE;
		wWinCount = INVALID_WORD;
		wLoseCount = INVALID_WORD;
		wDrawCount = INVALID_WORD;
	}

	RwUInt32		hSerial;
	RwUInt8			byState;
	RwUInt8			byLevel;
	RwUInt8			byClass;
	RwUInt16		wWinCount;
	RwUInt16		wLoseCount;
	RwUInt16		wDrawCount;
	RwUInt8			byIndex;		///< 천하제일 무도회의 멤버 Index
	std::wstring	wstrGuildName;
	std::wstring	wstrName;		
	stTBudokaiTeam*	pTeam;
};

typedef std::vector<stTBudokaiMember*>						VEC_TBTEAMMEMBER;
typedef std::vector<stTBudokaiMember*>::iterator			VEC_TBTEAMMEMBER_ITER;

struct stTBudokaiTeam
{
	stTBudokaiTeam(VOID)
	{
		wTeamType = INVALID_WORD;
		byTeamScore = 0;		
	}

	RwUInt16						wTeamType;
	RwUInt8							byTeamScore;
	std::wstring					wstrTeamName;
	VEC_TBTEAMMEMBER				vecpMember;
};

// !type
typedef std::map<RwUInt32,stTBudokaiMember*>				MAP_TBMEMBER;		// first SerialID
typedef std::map<RwUInt32,stTBudokaiMember*>::iterator		MAP_TBMEMBER_ITER;

typedef std::map<RwUInt16,stTBudokaiTeam*>					MAP_TBTEAM;			// first가 TeamID
typedef std::map<RwUInt16,stTBudokaiTeam*>::iterator		MAP_TBTEAM_ITER;

class CNtlWorldConceptTB : public CNtlWorldConceptController
{
public:
	// ! Constructor
	CNtlWorldConceptTB(void);
	~CNtlWorldConceptTB(void);

	virtual void		Update( RwReal fElapsed );	
	virtual void		ChangeState( RwInt32 eState );	
	
	void				ChangeTBType( RwUInt8 eTBType );
	void				ChangeTBState( RwUInt8 eTBState, RwBool bEnterState );
	
	RwUInt8				GetMatchType(VOID) { return m_eMatchType; }
	RwUInt8				GetMatchState(VOID) { return m_eMatchState; }
	RwBool				GetEnterState(VOID) { return m_bEnterState; }

	virtual RwBool		IsEnableAction( EAvatarAction eAction );
	virtual RwBool		IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos);
	virtual RwBool		IsVisiblePCInCinematic(VOID) { return TRUE; }
	virtual RwBool		CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode);
	RwBool				IsRingOut( RwUInt32 hSerialID );							// 죽건살건 링 안에 존재할때만 FALSE, 링 밖에 있을땐 다 TRUE
	RwBool				IsAttackableEnemy( RwUInt32 hSerialId );
	CNtlSobActor*		GetNearAttackableEnemy( CNtlSobActor* pActor );
	RwBool				IsPartyBattle(VOID);
	
	stTBudokaiMember*	FindMember(RwUInt32 hSerialID);
	stTBudokaiTeam*		FindTeam(RwUInt16 wTeamID);	
	stTBudokaiTeam*		GetMyTeam(VOID);											
	stTBudokaiTeam*		GetEnemyTeam(VOID);											// 본선,결선용 first를 Return할 뿐.
	stTBudokaiTeam*		GetBlueTeam(VOID);											// 본선,결선용 first를 Return할 뿐.
	stTBudokaiTeam*		GetRedTeam(VOID);											// 본선,결선용 first를 Return할 뿐.
	MAP_TBTEAM*			GetTeams(VOID) { return &m_mapTeams; }						// 예선용
	MAP_TBMEMBER*		GetMembers(VOID) { return &m_mapMembers; }

	RwUInt16			GetMyTeamType(VOID) { return m_wMyTeamType; }
	RwBool				MyTeamIsBlue(VOID);
	RwBool				IsBlueTeam( RwUInt16 wTeamType );
	RwBool				IsRedTeam( RwUInt16 wTeamType );
		
	void				SetMatchTbl( RwUInt32 tblidx );
	sRANKBATTLE_TBLDAT* GetMatchTbl(VOID) { return m_pMatchTblDat; }

	RwUInt8				GetMatchDepth(VOID) { return m_byMatchDepth; }
	RwUInt8				GetMatchIndex(VOID) { return m_byMatchIndex; }

	void				ClearMemberData(VOID);
	
protected:
	void				ResetCamera(VOID);
	void				MajorResetCamera(VOID);
	void				FinalResetCamera(VOID);

	MAP_TBTEAM			m_mapTeams;	
	MAP_TBMEMBER		m_mapMembers;												
	
	RwUInt8				m_eMatchType;
	RwUInt8				m_eMatchState;
	RwBool				m_bEnterState;
	RwUInt16			m_wMyTeamType;

	RwUInt8				m_byMatchDepth;
	RwUInt8				m_byMatchIndex;				
	
	sRANKBATTLE_TBLDAT*	m_pMatchTblDat;												// 랭크배틀 테이블 월드인포에서 세팅. 

public:
	void	EventHandler_MinorMatchStateUpdate(void* pData);
	void	EventHandler_MinorMatchTeamInfo(void* pData);
	void	EventHandler_MinorMatchPlayerState(void* pData);
	void	EventHandler_MinorMatchUpdateScore(void* pData);
	void	EventHandler_MinorMatchTeamScore(void* pData);
	void	EventHandler_MinorMatchStageFinish(void* pData);
	void	EventHandler_MinorMatchMatchFinish(void* pData);
	void	EventHandler_MajorMatchStateUpdate(void* pData);
	void	EventHandler_MajorMatchTeamInfo(void* pData);
	void	EventHandler_MajorMatchPlayerState(void* pData);
	void	EventHandler_MajorMatchUpdateScore(void* pData);
	void	EventHandler_MajorMatchStageFinish(void* pData);
	void	EventHandler_MajorMatchMatchFinish(void* pData);
	void	EventHandler_FinalMatchStateUpdate(void* pData);
	void	EventHandler_FinalMatchTeamInfo(void* pData);
	void	EventHandler_FinalMatchPlayerState(void* pData);
	void	EventHandler_FinalMatchUpdateScore(void* pData);
	void	EventHandler_FinalMatchStageFinish(void* pData);
	void	EventHandler_FinalMatchMatchFinish(void* pData);

private:
// Update Function for Direction;
	void	UpdateState_Null( RwReal fElpased );

	void	UpdateState_MinorMatch_None( RwReal fElapsed );
	void	UpdateState_MinorMatch_WaitMinorMatch( RwReal fElapsed );
	void	UpdateState_MinorMatch_Wait( RwReal fElapsed );	
	void	UpdateState_MinorMatch_Direction( RwReal fElapsed );
	void	UpdateState_MinorMatch_Match_Ready( RwReal fElapsed );
	void	UpdateState_MinorMatch_Stage_Ready( RwReal fElapsed );
	void	UpdateState_MinorMatch_Stage_Run( RwReal fElapsed );
	void	UpdateState_MinorMatch_Stage_Finish( RwReal fElapsed );
	void	UpdateState_MinorMatch_Match_Finish( RwReal fElapsed );
	void	UpdateState_MinorMatch_End( RwReal fElapsed );

	void	UpdateState_MajorMatch_None( RwReal fElapsed );
	void	UpdateState_MajorMatch_Wait( RwReal fElapsed );
	void	UpdateState_MajorMatch_Direction( RwReal fElapsed );
	void	UpdateState_MajorMatch_Match_Ready( RwReal fElapsed );
	void	UpdateState_MajorMatch_Stage_Ready( RwReal fElapsed );
	void	UpdateState_MajorMatch_Stage_Run( RwReal fElapsed );
	void	UpdateState_MajorMatch_Stage_Finish( RwReal fElapsed );
	void	UpdateState_MajorMatch_Match_Finish( RwReal fElapsed );
	void	UpdateState_MajorMatch_End( RwReal fElapsed );

	void	UpdateState_FinalMatch_None( RwReal fElapsed );
	void	UpdateState_FinalMatch_Wait( RwReal fElapsed );
	void	UpdateState_FinalMatch_Direction( RwReal fElapsed );
	void	UpdateState_FinalMatch_Match_Ready( RwReal fElapsed );
	void	UpdateState_FinalMatch_Stage_Ready( RwReal fElapsed );
	void	UpdateState_FinalMatch_Stage_Run( RwReal fElapsed );
	void	UpdateState_FinalMatch_Stage_Finish( RwReal fElapsed );
	void	UpdateState_FinalMatch_Match_Finish( RwReal fElapsed );
	void	UpdateState_FinalMatch_Final_Direction( RwReal fElapsed );
	void	UpdateState_FinalMatch_Awarding( RwReal fElapsed );
	void	UpdateState_FinalMatch_End( RwReal fElapsed );

	void	(CNtlWorldConceptTB::*StateUpdate)( RwReal fElapsed );	
};
