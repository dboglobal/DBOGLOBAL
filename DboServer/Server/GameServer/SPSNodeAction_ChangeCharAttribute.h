#ifndef __SERVER_SPSONODE_ACTION_CHANGECHARATTRIBUTE__
#define __SERVER_SPSONODE_ACTION_CHANGECHARATTRIBUTE__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_ChangeCharAttribute : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_ChangeCharAttribute(const char* lpszName);
	virtual ~CSPSNodeAction_ChangeCharAttribute();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	bool						m_bUseNickname;

	TBLIDX						m_nicknameTblidx;

	BYTE						m_bySizeRate;

	BYTE						m_byImmortalMode;

};


#endif
