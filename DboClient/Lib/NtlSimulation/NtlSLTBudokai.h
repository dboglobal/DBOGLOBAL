/*****************************************************************************
*
* File			: NtlTBudokai.h
* Author		: Pee Sangjun
* Copyright		: (주)NTL
* Date			: 2008. 5. 9	
* Abstract		: Simulation 천하제일 무도회
*****************************************************************************
* Desc          : 
*****************************************************************************/

#ifndef __NTL_T_BUDOKAI_H__
#define __NTL_T_BUDOKAI_H__

#include "ceventhandler.h"
#include "BudokaiTable.h"
#include "RankBattleTable.h"
#include "NtlBudokai.h"
#include "NtlMatch.h"

class CNtlSobActor;

class CNtlTBudokai : public RWS::CEventHandler
{
public:
	CNtlTBudokai(void);
	virtual ~CNtlTBudokai(void);

	void	Init(void);
	virtual RwBool	Create(void);		
	void	Destroy(void);

	void	HandleEvents( RWS::CMsg& msg );

	RwBool		IsBudokaiSeason();
	void		SetSeasonCount( RwUInt16 wSeasonCount )					{ m_wSeasonCount = wSeasonCount; }
	RwUInt16	GetSeasonCount()										{ return m_wSeasonCount; }

	sBUDOKAI_UPDATE_STATE_INFO*			GetMainStateInfo()				{ return &m_sStateInfo; }
	sBUDOKAI_UPDATE_MATCH_STATE_INFO*	GetIndividualMatchStateInfo()	{ return &m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_INDIVIDIAUL]; }
	sBUDOKAI_UPDATE_MATCH_STATE_INFO*	GetTeamMatchStateInfo()			{ return &m_aMatchStateInfo[BUDOKAI_MATCH_TYPE_TEAM]; }
	sBUDOKAI_JOIN_INFO*					GetBudokaiJoinInfo()			{ return &m_sJoinInfo; }

	sBUDOKAI_TBLINFO*					GetBudokaiTblData(VOID)			{ return m_pBudokaiTblDat; }

	RwUInt8								GetBudokaiMainState()			{ return m_sStateInfo.byState; }
	BUDOKAITIME							GetBudokaiMainStateNextTime()	{ return m_sStateInfo.tmNextStepTime; }
	BUDOKAITIME							GetBudokaiMainStateRemainTime()	{ return m_sStateInfo.tmRemainTime; }
	
private:
	RwUInt16							m_wSeasonCount;								///< 천하제일 무도회 횟수
	
	sBUDOKAI_UPDATE_STATE_INFO			m_sStateInfo;								///< MainState
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	m_aMatchStateInfo[MAX_BUDOKAI_MATCH_TYPE];	///< MatchState

	sBUDOKAI_JOIN_INFO					m_sJoinInfo;								///< 아바타의 천하제일 무도회 관련 정보

	sBUDOKAI_TBLINFO*					m_pBudokaiTblDat;							// 무도회 테이블

	RwReal								m_fCurrentTime;
};

#endif// __NTL_T_BUDOKAI_H__
