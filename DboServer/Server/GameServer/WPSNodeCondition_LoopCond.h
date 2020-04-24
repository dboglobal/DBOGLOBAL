#ifndef __SERVER_WPSNODE_CONDITION_LOOPCOND__
#define __SERVER_WPSNODE_CONDITION_LOOPCOND__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_LoopCond : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_LoopCond(const char* lpszName);
	virtual ~CWPSNodeCondition_LoopCond();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	bool						m_bOperationAND;

	bool						m_bReverse;

};


#endif
