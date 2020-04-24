#ifndef __SERVER_WPSNODE_ACTION_SENDEVENTTOTS__
#define __SERVER_WPSNODE_ACTION_SENDEVENTTOTS__


#include "ControlScriptNodeAction.h"

class CNtlVector;

class CWPSNodeAction_SendEventToTs : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SendEventToTs(const char* lpszName);
	virtual ~CWPSNodeAction_SendEventToTs();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

private:

	void						Init();

public:

	BYTE						m_byTriggerType;

	eSSD_TSEVENT_APPLY_TYPE		m_eApplyType;

	BYTE						m_byTargetType;

	TBLIDX						m_targetTblidx;

	CNtlVector					m_vLoc;

	float						m_fRadius;

	DWORD						m_teId;

};


#endif
