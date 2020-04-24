#ifndef __SERVER_TQSONODE_ACTION_INFORMATION__
#define __SERVER_TQSONODE_ACTION_INFORMATION__


#include "ControlScriptNodeAction.h"

enum eTIMEQUEST_INFORMATION_TYPE;

class CTQSNodeAction_Information : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_Information(const char* lpszName);
	virtual ~CTQSNodeAction_Information();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eTIMEQUEST_INFORMATION_TYPE	m_eInfoType;

};


#endif
