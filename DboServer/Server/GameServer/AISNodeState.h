#ifndef __SERVER_AISONODE_STATE__
#define __SERVER_AISONODE_STATE__


#include "ControlScriptNodeAction.h"

class CAISNodeState : public CControlScriptNodeAction
{

public:
	CAISNodeState(int nStateId = 0, const char* lpszStateName = NULL);
	virtual ~CAISNodeState();


public:

	int			GetStateID() { return m_nStateId; }

private:

	int			m_nStateId;

};


#endif
