#ifndef __SERVER_WPSNODE_CONDITION_SPAWNEDNPC__
#define __SERVER_WPSNODE_CONDITION_SPAWNEDNPC__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_SpawnedNpc : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_SpawnedNpc(const char* lpszName);
	virtual ~CWPSNodeCondition_SpawnedNpc();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	TBLIDX						m_npcTblidx;

};


#endif
