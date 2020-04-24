#ifndef __SERVER_SPSONODE_ACTION_ROTATE__
#define __SERVER_SPSONODE_ACTION_ROTATE__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_Rotate : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_Rotate(const char* lpszName);
	virtual ~CSPSNodeAction_Rotate();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	CNtlVector					m_vRotateDir;

};


#endif
