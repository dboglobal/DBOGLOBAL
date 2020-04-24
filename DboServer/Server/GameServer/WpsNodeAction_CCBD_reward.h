#ifndef __SERVER_WPSNODE_ACTION_CCBD_REWARD__
#define __SERVER_WPSNODE_ACTION_CCBD_REWARD__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD_reward : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD_reward(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD_reward();


public:

	virtual bool				CheckParameter() { return true; }

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	TBLIDX						m_itemTblidx;
	bool						m_bLastStage;

};


#endif
