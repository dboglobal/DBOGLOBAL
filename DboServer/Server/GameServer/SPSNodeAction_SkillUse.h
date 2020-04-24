#ifndef __SERVER_SPSONODE_ACTION_USESKILL__
#define __SERVER_SPSONODE_ACTION_USESKILL__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SkillUse : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SkillUse(const char* lpszName);
	virtual ~CSPSNodeAction_SkillUse();


	sPARAMETER_INFO*			GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	BYTE						m_bySkillIndex;

};


#endif
