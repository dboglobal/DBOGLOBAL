#ifndef __SERVER_WPSNODE_ACTION_CHANGESTAGE__
#define __SERVER_WPSNODE_ACTION_CHANGESTAGE__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_ChangeStage : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ChangeStage(const char* lpszName);
	virtual ~CWPSNodeAction_ChangeStage();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	BYTE						m_byNextStage;

};


#endif
