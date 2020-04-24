#ifndef __SERVER_SPSONODE_ACTION_SENDESCORTEVENTTOTS__
#define __SERVER_SPSONODE_ACTION_SENDESCORTEVENTTOTS__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SendEscortEventToTs : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SendEscortEventToTs(const char* lpszName);
	virtual ~CSPSNodeAction_SendEscortEventToTs();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	int							m_teId;

};


#endif
