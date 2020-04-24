#ifndef __SERVER_WPS_NODE_GAMEPREPARE__
#define __SERVER_WPS_NODE_GAMEPREPARE__

#include "ControlScriptNodeState.h"


class CWPSNodeGamePrepare : public CControlScriptNodeState
{

public:
	CWPSNodeGamePrepare();
	virtual ~CWPSNodeGamePrepare();

public:

	bool				Create() { return true; }

};


#endif