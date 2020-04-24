#ifndef __SERVER_WPSNODE_ACTION_TIMECOUNTDOWN__
#define __SERVER_WPSNODE_ACTION_TIMECOUNTDOWN__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_TimeCountdown : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_TimeCountdown(const char* lpszName);
	virtual ~CWPSNodeAction_TimeCountdown();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	bool						m_bCountdown;

};


#endif
