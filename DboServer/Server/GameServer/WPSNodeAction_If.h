#ifndef __SERVER_WPSNODE_ACTION_IF__
#define __SERVER_WPSNODE_ACTION_IF__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_If : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_If(const char* lpszName);
	virtual ~CWPSNodeAction_If();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bOperationAND;

	bool						m_bReverse;

};


#endif
