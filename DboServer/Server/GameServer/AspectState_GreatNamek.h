#ifndef __SERVER_BOT_ASPECT_STATE_GREATNAMEK__
#define __SERVER_BOT_ASPECT_STATE_GREATNAMEK__

#include "AspectState.h"

class CAspectState_GreatNamek : public CAspectState
{

public:
	CAspectState_GreatNamek(CCharacter *pParent);
	virtual ~CAspectState_GreatNamek();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};


#endif