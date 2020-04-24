#ifndef __SERVER_AISONODE_STATE_IDLE__
#define __SERVER_AISONODE_STATE_IDLE__


#include "AISNodeState.h"


class CAISNodeState_Idle : public CAISNodeState
{

public:
	CAISNodeState_Idle(const char* lpszNodeName);
	virtual ~CAISNodeState_Idle();



};


#endif
