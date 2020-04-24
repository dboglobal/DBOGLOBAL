#ifndef __SERVER_BOT_ASPECT_STATE_SUPERSAIYAN__
#define __SERVER_BOT_ASPECT_STATE_SUPERSAIYAN__

#include "AspectState.h"

class CAspectState_SuperSaiyan : public CAspectState
{

public:
	CAspectState_SuperSaiyan(CCharacter *pParent);
	virtual ~CAspectState_SuperSaiyan();

public:

	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};


#endif