#ifndef __SERVER_TQSONODE_ACTION_TIMECOUNTDOWN__
#define __SERVER_TQSONODE_ACTION_TIMECOUNTDOWN__


#include "ControlScriptNodeAction.h"


class CTQSNodeAction_TimeCountdown : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_TimeCountdown(const char* lpszName);
	virtual ~CTQSNodeAction_TimeCountdown();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bCountdown;

};


#endif
