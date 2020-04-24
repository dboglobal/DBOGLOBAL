#ifndef __SERVER_WPSNODE_CONDITION_SPAWNEDMOB__
#define __SERVER_WPSNODE_CONDITION_SPAWNEDMOB__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_SpawnedMob : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_SpawnedMob(const char* lpszName);
	virtual ~CWPSNodeCondition_SpawnedMob();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	SPAWNGROUPID				m_mobSpawnGroupId;

	BYTE						m_byMobCount;

};


#endif
