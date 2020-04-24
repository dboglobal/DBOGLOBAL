#ifndef __SERVER_WPSNODE_ACTION_UNREGISTERBUFF__
#define __SERVER_WPSNODE_ACTION_UNREGISTERBUFF__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CWPSNodeAction_UnregisterBuff : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_UnregisterBuff(const char* lpszName);
	virtual ~CWPSNodeAction_UnregisterBuff();


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
