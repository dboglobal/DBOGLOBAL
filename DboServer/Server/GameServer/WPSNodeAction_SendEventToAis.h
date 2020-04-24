#ifndef __SERVER_WPSNODE_ACTION_SENDEVENTTOAIS__
#define __SERVER_WPSNODE_ACTION_SENDEVENTTOAIS__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CWPSNodeAction_SendEventToAis : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SendEventToAis(const char* lpszName);
	virtual ~CWPSNodeAction_SendEventToAis();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eOBJTYPE					m_eTargetType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventId;

};


#endif
