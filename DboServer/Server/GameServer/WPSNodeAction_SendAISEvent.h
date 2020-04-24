#ifndef __SERVER_WPSNODE_ACTION_SENDAISEVENT__
#define __SERVER_WPSNODE_ACTION_SENDAISEVENT__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CWPSNodeAction_SendAISEvent : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SendAISEvent(const char* lpszName);
	virtual ~CWPSNodeAction_SendAISEvent();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

};


#endif
