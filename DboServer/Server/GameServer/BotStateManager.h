#ifndef __SERVER_BOT_STATE_MANAGER__
#define __SERVER_BOT_STATE_MANAGER__


#include "StateManager.h"


class CBotStateManager : public CStateManager
{

public:
	CBotStateManager();
	virtual ~CBotStateManager();

public:

	virtual int				OnUpdate(DWORD dwTickDiff, float fMultiple);

protected:

	virtual void			Init();

private:

	DWORD					m_dwSumDelayTick;

};


#endif