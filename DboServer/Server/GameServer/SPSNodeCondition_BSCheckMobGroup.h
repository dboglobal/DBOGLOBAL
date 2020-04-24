#ifndef __SERVER_SPSONODE_CONDITION_BSCHECKMOBGROUP__
#define __SERVER_SPSONODE_CONDITION_BSCHECKMOBGROUP__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_BSCheckMobGroup : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_BSCheckMobGroup(const char* lpszName);
	virtual ~CSPSNodeCondition_BSCheckMobGroup();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

private:

	SPAWNGROUPID				m_spawnGroupId;

	BYTE						m_byMaxCount;

	BYTE						m_byMinCount;

	bool						m_bIsInRange;

};


#endif
