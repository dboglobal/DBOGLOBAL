#ifndef __SERVER_WPSNODE_CONDITION_RECVEVENTFROMSPS__
#define __SERVER_WPSNODE_CONDITION_RECVEVENTFROMSPS__


#include "ControlScriptNodeCondition.h"

enum eOBJTYPE;

class CWPSNodeCondition_RecvEventFromSps : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvEventFromSps(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvEventFromSps();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_objectTblidx;

	DWORD						m_eventId;

};


#endif
