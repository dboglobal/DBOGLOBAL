#ifndef __SERVER_SPSONODE_CONDITION_CONDITIONGROUP__
#define __SERVER_SPSONODE_CONDITION_CONDITIONGROUP__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_BSConditionGroup : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_BSConditionGroup(const char* lpszName);
	virtual ~CSPSNodeCondition_BSConditionGroup();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bOperatorAnd;

};


#endif
