#ifndef __SERVER_BOT_ASPECT_STATE_KAIOKEN__
#define __SERVER_BOT_ASPECT_STATE_KAIOKEN__

#include "AspectState.h"

class CAspectState_Kaioken : public CAspectState
{

public:
	CAspectState_Kaioken(CCharacter *pParent);
	virtual ~CAspectState_Kaioken();

public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};


#endif