#ifndef __SERVER_TQSNODEGAME_BEGIN__
#define __SERVER_TQSNODEGAME_BEGIN__


#include "ControlScriptNodeState.h"


class CTQSNodeGameBegin : public CControlScriptNodeState
{

public:

	CTQSNodeGameBegin();
	virtual ~CTQSNodeGameBegin();

	bool				Create() { return true; }

};


#endif