
/*****************************************************************************
 *
 * File			: LogInStageState.h
 * Author		: HyungSuk, Jang
 * Copyright	: (¡÷)NTL
 * Date			: 2006. 12. 11	
 * Abstract		: Login stage state
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __LOGIN_STAGE_STATE_H__
#define __LOGIN_STAGE_STATE_H__

// core
#include "ceventhandler.h"

// gui
#include "eventtimer.h"

// dbo
#include "DboDef.h"

class CLogInStageState : public RWS::CEventHandler
{
private:

	enum
	{
		LSS_TIME_SERVER_CONNECT = 30000,	// 3∫–.
		LSS_TIMER_LOGIN_REQ		= 180000,	// 3∫–.
	};

	struct sCHAR_SERVER_LOGIN
	{
		RwBool				bDisconnectedAuthServer;
		RwBool				bConnectedCharServer;
		RwBool				bAskedLoginCharServer;
	};

	static CLogInStageState *m_pLoginState;
	ELogInStageState		m_eState;
	CTimerHandle			m_hTimer;
	sCHAR_SERVER_LOGIN		m_tCHAR_SERVER_LOGIN;

	RwReal					m_fNotifyTime;
	RwBool					m_bNotifyShow;

	typedef void (CLogInStageState::*FuncUpdate)(RwReal fElapsed);
	FuncUpdate m_fnUpdate[LOGIN_STATE_END];

private:

	void CreateTimeOutTimer(ELogInStageState eState);

	void EnterState(ELogInStageState eState);
	void ExitState(ELogInStageState eState);

	void UpdateServerConnect(RwReal fElapsed);
	void UpdateLoginReq(RwReal fElapsed);

public:

	CLogInStageState();
	~CLogInStageState();

	static CLogInStageState* GetInstance(void);

	void Update(RwReal fElapsed);

	void ChangeState(ELogInStageState eState);

	ELogInStageState GetCurrentState();

	RwBool IsState(ELogInStageState eState);

	void OnTimer(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
};

static CLogInStageState* GetLogInStageState(void)
{
	return CLogInStageState::GetInstance();
}

#endif