#ifndef __SERVER_WPSNODE_ACTION_WAIT__
#define __SERVER_WPSNODE_ACTION_WAIT__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_Wait : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_Wait(const char* lpszName);
	virtual ~CWPSNodeAction_Wait();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bOperationAND;

};


#endif
