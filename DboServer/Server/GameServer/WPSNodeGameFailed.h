#ifndef __SERVER_WPS_NODE_GAMEFAILED__
#define __SERVER_WPS_NODE_GAMEFAILED__

#include "ControlScriptNodeState.h"


class CWPSNodeGameFailed : public CControlScriptNodeState
{

public:
	CWPSNodeGameFailed();
	virtual ~CWPSNodeGameFailed();

public:

	bool				Create() { return true; }

};


#endif