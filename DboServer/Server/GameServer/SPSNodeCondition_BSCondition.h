#ifndef __SERVER_SPSONODE_CONDITION_BSCONDITION__
#define __SERVER_SPSONODE_CONDITION_BSCONDITION__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_BSCondition : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_BSCondition(const char* lpszName);
	virtual ~CSPSNodeCondition_BSCondition();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bOperatorAnd;

	DWORD						m_dwPreCoolTime;

	DWORD						m_dwCoolTime;

};


#endif
