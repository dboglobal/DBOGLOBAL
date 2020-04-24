#ifndef __SERVER_WPSNODE_CONDITION_CCBD_PATTERN__
#define __SERVER_WPSNODE_CONDITION_CCBD_PATTERN__


#include "ControlScriptNodeCondition.h"


class CWpsNodeCondition_CCBD_pattern : public CControlScriptNodeCondition
{

public:
	CWpsNodeCondition_CCBD_pattern(const char* lpszName);
	virtual ~CWpsNodeCondition_CCBD_pattern();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	BYTE						m_byIndex;
	TBLIDX						m_titleIdx;
	TBLIDX						m_subTitleIdx;
	bool						m_bUseTimeLimit;

};


#endif
