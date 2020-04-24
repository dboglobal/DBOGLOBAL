#ifndef __SERVER_TQSONODE_CONDITION_ALLPLAYERFAINT__
#define __SERVER_TQSONODE_CONDITION_ALLPLAYERFAINT__


#include "ControlScriptNodeCondition.h"


class CTQSNodeCondition_AllPlayerFaint : public CControlScriptNodeCondition
{

public:
	CTQSNodeCondition_AllPlayerFaint(const char* lpszName);
	virtual ~CTQSNodeCondition_AllPlayerFaint();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	bool						m_bTmqFail;

};


#endif
