#ifndef __SERVER_SPSONODE_ACTION_CHANGETIMIDATTRIBUTE__
#define __SERVER_SPSONODE_ACTION_CHANGETIMIDATTRIBUTE__


#include "ControlScriptNodeAction.h"
enum eTIMID_TYPE;

class CSPSNodeAction_ChangeTimidAttribute : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_ChangeTimidAttribute(const char* lpszName);
	virtual ~CSPSNodeAction_ChangeTimidAttribute();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eTIMID_TYPE					m_eTimidType;

	float						m_fRange;

};


#endif
