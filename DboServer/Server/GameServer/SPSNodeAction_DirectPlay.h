#ifndef __SERVER_SPSONODE_ACTION_DIRECTPLAY__
#define __SERVER_SPSONODE_ACTION_DIRECTPLAY__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_DirectPlay : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_DirectPlay(const char* lpszName);
	virtual ~CSPSNodeAction_DirectPlay();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	TBLIDX						m_directionTblidx;

	DWORD						m_dwPlayTime;

	eSPS_EVENT_TYPE				m_eEventType;

	DWORD						m_eventID;

};


#endif
