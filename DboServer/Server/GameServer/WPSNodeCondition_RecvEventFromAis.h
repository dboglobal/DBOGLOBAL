#ifndef __SERVER_WPSNODE_CONDITION_RECVEVENTFROMAIS__
#define __SERVER_WPSNODE_CONDITION_RECVEVENTFROMAIS__


#include "ControlScriptNodeCondition.h"

enum eOBJTYPE;

class CWPSNodeCondition_RecvEventFromAis : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvEventFromAis(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvEventFromAis();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_objectTblidx;

	DWORD						m_eventId;

};


#endif
