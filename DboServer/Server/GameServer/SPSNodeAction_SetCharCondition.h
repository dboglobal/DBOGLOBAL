#ifndef __SERVER_SPSONODE_ACTION_SETCHARCONDITION__
#define __SERVER_SPSONODE_ACTION_SETCHARCONDITION__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SetCharCondition : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SetCharCondition(const char* lpszName);
	virtual ~CSPSNodeAction_SetCharCondition();


	sPARAMETER_INFO*			GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	QWORD						m_qwAddCharConditionFlag;

	QWORD						m_qwRemoveCharConditionFlag;

};


#endif
