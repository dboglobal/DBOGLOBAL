#ifndef __SERVER_WPSNODE_ACTION_CHANGECHARATTRIBUTE__
#define __SERVER_WPSNODE_ACTION_CHANGECHARATTRIBUTE__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CWPSNodeAction_ChangeCharAttribute : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ChangeCharAttribute(const char* lpszName);
	virtual ~CWPSNodeAction_ChangeCharAttribute();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	eOBJTYPE					m_eTargetType;

	DWORD						m_dwTargetIndex;

	bool						m_bUseNickname;

	TBLIDX						m_nicknameTblidx;

	BYTE						m_bySizeRate;

	BYTE						m_byImmortalMode;

	BYTE						m_byLpBlockRate;
	
};


#endif
