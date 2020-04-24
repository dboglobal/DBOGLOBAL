#ifndef __SERVER_TQSONODE_ACTION_TELEPORT__
#define __SERVER_TQSONODE_ACTION_TELEPORT__


#include "ControlScriptNodeAction.h"
#include "NtlVector.h"

class CTQSNodeAction_Teleport : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_Teleport(const char* lpszName);
	virtual ~CTQSNodeAction_Teleport();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

};


#endif
