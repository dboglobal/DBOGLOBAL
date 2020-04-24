#ifndef __SERVER_TQSONODE_ACTION_CHANGESTAGE__
#define __SERVER_TQSONODE_ACTION_CHANGESTAGE__


#include "ControlScriptNodeAction.h"


class CTQSNodeAction_ChangeStage : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_ChangeStage(const char* lpszName);
	virtual ~CTQSNodeAction_ChangeStage();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	BYTE						m_byNextStage;

};


#endif
