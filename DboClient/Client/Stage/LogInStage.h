/*****************************************************************************
 *
 * File			: LogInStage.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 17	
 * Abstract		: Login stage class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __LOGIN_STAGE_H__
#define __LOGIN_STAGE_H__

#include "NtlStage.h"
#include "ceventhandler.h"
#include "eventtimer.h"

class CLogInGuiGroup;
class CLogInStageState;

class CLogInStage : public CNtlStage, public RWS::CEventHandler
{
private:

	RwBool				m_bAutoLogIn;
	CLogInStageState	*m_pState;
	CLogInGuiGroup		*m_pGuiGroup;

public:

	CLogInStage(const char *pStageName);
	~CLogInStage();

	virtual bool Create(void);
	virtual void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

// event handler
private:

	void EventProcUpdateTick(RwReal fElapsed);
	void LogInServerConnect(RWS::CMsg &pMsg);
	void LobbyServerConnect(RWS::CMsg &pMsg);
	void LoginStageStateEnter(RWS::CMsg &pMsg);
	void LoginStageStateExit(RWS::CMsg &pMsg);
	void LogInStageTimeOutEventHandler(RWS::CMsg &msg);
	void LogInMessageEventHandler(RWS::CMsg &msg);

public:
	int	ActionGameExit(void);	
};

#endif