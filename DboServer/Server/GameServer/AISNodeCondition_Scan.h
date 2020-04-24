#ifndef __SERVER_AISONODE_CONDITION_SCAN__
#define __SERVER_AISONODE_CONDITION_SCAN__


#include "ControlScriptNodeCondition.h"
#include "NtlObject.h"


class CAISNodeCondition_Scan : public CControlScriptNodeCondition
{

public:
	CAISNodeCondition_Scan(const char* lpszName);
	virtual ~CAISNodeCondition_Scan();

	
	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	
	eOBJTYPE					m_eObjType;

	TBLIDX						m_tblidx;

	WORD						m_wRange;

};


#endif
