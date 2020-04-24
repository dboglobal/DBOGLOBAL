#ifndef __SERVER_BOT_ASPECT_STATE_VEHICLE__
#define __SERVER_BOT_ASPECT_STATE_VEHICLE__

#include "AspectState.h"

class CAspectState_Vehicle : public CAspectState
{

public:
	CAspectState_Vehicle(CCharacter *pParent);
	virtual ~CAspectState_Vehicle();

public:

	virtual int CopyAspectTo(sASPECTSTATE_DETAIL* pStateDetail);
	virtual void CopyAspectFrom(sASPECTSTATE_DETAIL* pStateDetail);

};


#endif