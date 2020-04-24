#ifndef __SERVER_TQSNODEGAME_END__
#define __SERVER_TQSNODEGAME_END__


#include "ControlScriptNodeState.h"


class CTQSNodeGameEnd : public CControlScriptNodeState
{

public:

	CTQSNodeGameEnd();
	virtual ~CTQSNodeGameEnd();

	bool				Create() { return true; }

};


#endif