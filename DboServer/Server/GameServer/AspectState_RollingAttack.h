#ifndef __SERVER_BOT_ASPECT_STATE_ROLLINGATTACK__
#define __SERVER_BOT_ASPECT_STATE_ROLLINGATTACK__

#include "AspectState.h"

class CAspectState_RollingAttack : public CAspectState
{

public:

	CAspectState_RollingAttack(CCharacter *pParent);
	virtual ~CAspectState_RollingAttack();


public:

	virtual void		OnExit();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

};


#endif