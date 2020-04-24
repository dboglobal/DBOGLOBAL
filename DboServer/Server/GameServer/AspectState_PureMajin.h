#ifndef __SERVER_BOT_ASPECT_STATE_PUREMAJIN__
#define __SERVER_BOT_ASPECT_STATE_PUREMAJIN__

#include "AspectState.h"

class CAspectState_PureMajin : public CAspectState
{

public:
	CAspectState_PureMajin(CCharacter *pParent);
	virtual ~CAspectState_PureMajin();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


};


#endif