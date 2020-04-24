
/*****************************************************************************
 *
 * File			: GameStageState.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2007. 09. 13	
 * Abstract		: Game stage state
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __GAME_STAGE_STATE_H__
#define __GAME_STAGE_STATE_H__

// gui
#include "eventtimer.h"
#include "ceventhandler.h"

// dbo
#include "DboDef.h"


class CGameStageState : public RWS::CEventHandler
{
private:

	enum
	{
		GSS_TIME_SERVER_CONNECT = 30000,	// 3╨п.
	};

	static CGameStageState *m_pGameState;
	EGameStageState			m_eState;
	CTimerHandle			m_hTimer;	

	RwReal					m_fNotifyTime;
	RwBool					m_bNotifyShow;

	typedef void (CGameStageState::*FuncUpdate)(RwReal fElapsed);
	FuncUpdate m_fnUpdate[GAME_STATE_END];

	float					m_fNPCheckTime;

	RwBool					m_bNPCheck;

private:

	void CreateTimeOutTimer(EGameStageState eState);

	void CharServerConnectEnter(void);
	void CharServerLoginEnter(void);

	void EnterState(EGameStageState eState);
	void ExitState(EGameStageState eState);

	void UpdateServerConnect(RwReal fElapsed);
    

public:

	CGameStageState();
	~CGameStageState();

	static CGameStageState* GetInstance(void);

	void Update(RwReal fElapsed);

	void ChangeState(EGameStageState eState);

	EGameStageState GetCurrentState();

	RwBool IsState(EGameStageState eState);

	void SetNPCheck(RwBool bCheck);

	void OnTimer(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
};

static CGameStageState* GetGameStageState(void)
{
	return CGameStageState::GetInstance();
}



#endif
