#ifndef __SERVER_WPSNODE_CONDITION_CHECKMOBGROUP__
#define __SERVER_WPSNODE_CONDITION_CHECKMOBGROUP__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_CheckMobGroup : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_CheckMobGroup(const char* lpszName);
	virtual ~CWPSNodeCondition_CheckMobGroup();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


private:

	void						Init() {}

public:

	SPAWNGROUPID				m_spawnGroupId;

	int							m_nCheckCount;

};


#endif
