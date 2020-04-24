#ifndef __SERVER_SPSONODE_CONDITION_BSCHECKLP__
#define __SERVER_SPSONODE_CONDITION_BSCHECKLP__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_BSCheckLp : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_BSCheckLp(const char* lpszName);
	virtual ~CSPSNodeCondition_BSCheckLp();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	BYTE						m_byMaxPercent;

	BYTE						m_byMinPercent;

	bool						m_bIsInRange;

};


#endif
