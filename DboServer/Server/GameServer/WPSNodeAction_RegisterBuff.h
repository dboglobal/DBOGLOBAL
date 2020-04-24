#ifndef __SERVER_WPSNODE_ACTION_REGISTERBUFF__
#define __SERVER_WPSNODE_ACTION_REGISTERBUFF__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CWPSNodeAction_RegisterBuff : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_RegisterBuff(const char* lpszName);
	virtual ~CWPSNodeAction_RegisterBuff();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eOBJTYPE					m_eTargetType;

	DWORD						m_dwTargetIndex;

	TBLIDX						m_SkillId;

};


#endif
