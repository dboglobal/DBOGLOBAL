#ifndef __SERVER_SPSONODE_ACTION_SITDOWN__
#define __SERVER_SPSONODE_ACTION_SITDOWN__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SitDown : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SitDown(const char* lpszName);
	virtual ~CSPSNodeAction_SitDown();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	bool						m_bSitDown;

};


#endif
