#ifndef __SERVER_SPSONODE_ACTION_DIRECTTURNING__
#define __SERVER_SPSONODE_ACTION_DIRECTTURNING__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_DirectTurning : public CControlScriptNodeAction
{


public:
	CSPSNodeAction_DirectTurning(const char* lpszName);
	virtual ~CSPSNodeAction_DirectTurning();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	TBLIDX						m_directionTblidx;

};


#endif
