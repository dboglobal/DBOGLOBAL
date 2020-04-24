#ifndef __SERVER_SPSONODE_CONDITION_BSCHECKNPC__
#define __SERVER_SPSONODE_CONDITION_BSCHECKNPC__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_BSCheckNpc : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_BSCheckNpc(const char* lpszName);
	virtual ~CSPSNodeCondition_BSCheckNpc();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

private:

	TBLIDX						m_npcTblidx;

	bool						m_bIsExist;

};


#endif
