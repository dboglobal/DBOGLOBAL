#ifndef __SERVER_CONTROLSTATE_FACTORY__
#define __SERVER_CONTROLSTATE_FACTORY__


#include "ControlStateEx.h"

class CControlStateFactory
{

public:

	CControlStateFactory() {}
	virtual ~CControlStateFactory() {}

public:

	virtual CControlState* CreateScriptControlState(int controlStateID) { return NULL; }

	virtual CControlState* CreateNodeControlState(int controlStateID, CControlState* controlstate) { return NULL; }

	virtual bool CanCreateControlState(int controlStateID) { return false; }

	virtual bool CanCreateControlStateLoopCountCheck(DWORD dwScriptNodeIndex, DWORD & rdwLoopCount) { return true; }

};


#endif