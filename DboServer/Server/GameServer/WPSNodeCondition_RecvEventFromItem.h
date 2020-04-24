#ifndef __SERVER_WPSNODE_CONDITION_RECVEVENTFROMITEM__
#define __SERVER_WPSNODE_CONDITION_RECVEVENTFROMITEM__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_RecvEventFromItem : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvEventFromItem(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvEventFromItem();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	TBLIDX						m_itemTblidx;

};


#endif
