#ifndef __SERVER_WPSNODE_CONDITION_NPCFAINT__
#define __SERVER_WPSNODE_CONDITION_NPCFAINT__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_NpcFaint : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_NpcFaint(const char* lpszName);
	virtual ~CWPSNodeCondition_NpcFaint();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	TBLIDX						m_npcTblidx;

	bool						m_bWpsFail;

};


#endif
