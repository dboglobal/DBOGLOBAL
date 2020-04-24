#ifndef __SERVER_WPSNODE_ACTION_CCBD_STAGE__
#define __SERVER_WPSNODE_ACTION_CCBD_STAGE__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD_stage : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD_stage(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD_stage();


public:

	virtual bool				CheckParameter() { return true; }

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	BYTE						m_byStage;
	bool						m_bDirectPlay;

};


#endif
