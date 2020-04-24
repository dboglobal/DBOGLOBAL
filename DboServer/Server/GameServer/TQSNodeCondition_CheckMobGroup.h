#ifndef __SERVER_TQSONODE_CONDITION_CHECKMOBGROUP__
#define __SERVER_TQSONODE_CONDITION_CHECKMOBGROUP__


#include "ControlScriptNodeCondition.h"


class CTQSNodeCondition_CheckMobGroup : public CControlScriptNodeCondition
{

public:
	CTQSNodeCondition_CheckMobGroup(const char* lpszName);
	virtual ~CTQSNodeCondition_CheckMobGroup();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);



public:

	SPAWNGROUPID				m_spawnGroupId;

	int							m_nCheckCount;

};


#endif
