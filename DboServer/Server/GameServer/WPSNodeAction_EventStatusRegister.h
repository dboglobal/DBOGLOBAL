#ifndef __SERVER_WPSNODE_ACTION_EVENTSTATUSREGISTER__
#define __SERVER_WPSNODE_ACTION_EVENTSTATUSREGISTER__


#include "ControlScriptNodeAction.h"
#include "NtlEventDef.h"


class CWPSNodeAction_EventStatusRegister : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_EventStatusRegister(const char* lpszName);
	virtual ~CWPSNodeAction_EventStatusRegister();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	sSVRSCR_EVENTSTATUS_DATA		m_sEventStatusData;

	eSERVERSCRIPT_EVENTSTATUS_TYPE	m_eType;

	SPAWNGROUPID					m_mobSpawnGroupId;

	DWORD							m_eventId;

	WORD							m_wMaxCount;

};


#endif
