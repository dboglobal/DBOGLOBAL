#ifndef __SERVER_TQSONODE_ACTION_SENDTSEVENT__
#define __SERVER_TQSONODE_ACTION_SENDTSEVENT__


#include "ControlScriptNodeAction.h"
#include "NtlVector.h"

enum eTS_EVENT_SEND_TYPE;

class CTQSNodeAction_SendTsEvent : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_SendTsEvent(const char* lpszName);
	virtual ~CTQSNodeAction_SendTsEvent();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eTS_EVENT_SEND_TYPE			m_eSendType;

	BYTE						m_byTriggerType;

	DWORD						m_teid;

	CNtlVector					m_vOrigin;

	float						m_fEffectRadius;

};


#endif
