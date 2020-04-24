#ifndef __SERVER_WPSNODE_CONDITION_ALLPLAYERFAINT__
#define __SERVER_WPSNODE_CONDITION_ALLPLAYERFAINT__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_AllPlayerFaint : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_AllPlayerFaint(const char* lpszName);
	virtual ~CWPSNodeCondition_AllPlayerFaint();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	bool						m_bWpsFail;

};


#endif
