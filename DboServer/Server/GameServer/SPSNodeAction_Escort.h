#ifndef __SERVER_SPSONODE_ACTION_ESCORT__
#define __SERVER_SPSONODE_ACTION_ESCORT__


#include "ControlScriptNodeAction.h"
#include "NtlQuest.h"

class CSPSNodeAction_Escort : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_Escort(const char* lpszName);
	virtual ~CSPSNodeAction_Escort();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	eESCORT_TYPE				m_eEscortType;

	CNtlVector					m_vDestLoc;

	float						m_fRadius;

	bool						m_bRunMode;

	DWORD						m_eventID;
};


#endif
