#ifndef __SERVER_WPSNODE_ACTION_CCBD_REGISTER_DIRECT_PLAY__
#define __SERVER_WPSNODE_ACTION_CCBD_REGISTER_DIRECT_PLAY__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD_register_direct_play : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD_register_direct_play(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD_register_direct_play();


public:

	virtual bool				CheckParameter() { return true; }

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	TBLIDX						m_directIndex;

};


#endif
