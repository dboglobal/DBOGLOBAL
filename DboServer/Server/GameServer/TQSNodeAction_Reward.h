#ifndef __SERVER_TQSONODE_ACTION_REWARD__
#define __SERVER_TQSONODE_ACTION_REWARD__


#include "ControlScriptNodeAction.h"


class CTQSNodeAction_Reward : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_Reward(const char* lpszName);
	virtual ~CTQSNodeAction_Reward();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	BYTE						m_byTriggerType;

	WORD						m_teid;

	DWORD						m_dwWaitTime;

};


#endif
