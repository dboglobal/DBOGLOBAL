#ifndef __SERVER_WPSNODE_ACTION_EXECWPS__
#define __SERVER_WPSNODE_ACTION_EXECWPS__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_ExecWps : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ExecWps(const char* lpszName);
	virtual ~CWPSNodeAction_ExecWps();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	TBLIDX						m_wpsTblidx;

};


#endif
