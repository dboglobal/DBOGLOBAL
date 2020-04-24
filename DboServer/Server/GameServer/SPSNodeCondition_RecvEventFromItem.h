#ifndef __SERVER_SPSONODE_CONDITION_RECVEVENTFROMITEM__
#define __SERVER_SPSONODE_CONDITION_RECVEVENTFROMITEM__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_RecvEventFromItem : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_RecvEventFromItem(const char* lpszName);
	virtual ~CSPSNodeCondition_RecvEventFromItem();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	TBLIDX						m_itemTblidx;

};


#endif
