#ifndef __SERVER_BOT_ASPECT_STATE_SPINNINGATTACK__
#define __SERVER_BOT_ASPECT_STATE_SPINNINGATTACK__

#include "AspectState.h"

class CAspectState_SpinningAttack : public CAspectState
{

public:
	CAspectState_SpinningAttack(CCharacter *pParent);
	virtual ~CAspectState_SpinningAttack();

public:

	virtual void		OnExit();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

};


#endif