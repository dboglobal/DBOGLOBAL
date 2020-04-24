#ifndef __SERVER_TQSONODE_ACTION_WAIT__
#define __SERVER_TQSONODE_ACTION_WAIT__


#include "ControlScriptNodeAction.h"


class CTQSNodeAction_Wait : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_Wait(const char* lpszName);
	virtual ~CTQSNodeAction_Wait();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bOperationAND;

};


#endif
