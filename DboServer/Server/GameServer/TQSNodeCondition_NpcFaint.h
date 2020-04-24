#ifndef __SERVER_TQSONODE_CONDITION_NPCFAINT_
#define __SERVER_TQSONODE_CONDITION_NPCFAINT_


#include "ControlScriptNodeCondition.h"


class CTQSNodeCondition_NpcFaint : public CControlScriptNodeCondition
{

public:
	CTQSNodeCondition_NpcFaint(const char* lpszName);
	virtual ~CTQSNodeCondition_NpcFaint();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	TBLIDX						m_npcTblidx;

	bool						m_bTmqFail;

};


#endif
