#ifndef __SERVER_WPSNODE_ACTION_SENDEVENTTOSPS__
#define __SERVER_WPSNODE_ACTION_SENDEVENTTOSPS__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CWPSNodeAction_SendEventToSps : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SendEventToSps(const char* lpszName);
	virtual ~CWPSNodeAction_SendEventToSps();


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
