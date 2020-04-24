#ifndef __SCRIPT_ALGO_DBOG_ACTION_EVENT_STATUS_REGISTER_H__
#define __SCRIPT_ALGO_DBOG_ACTION_EVENT_STATUS_REGISTER_H__

#include "ScriptAlgoAction.h"
#include "NtlEventDef.h"


class CWpsAlgoAction_EventStatusRegister : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_EventStatusRegister(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_EventStatusRegister();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	sSVRSCR_EVENTSTATUS_DATA		m_sEventStatusData;

	eSERVERSCRIPT_EVENTSTATUS_TYPE	m_eType;

	SPAWNGROUPID					m_mobSpawnGroupId;

	DWORD							m_eventId;

	WORD							m_wMaxCount;

};

#endif